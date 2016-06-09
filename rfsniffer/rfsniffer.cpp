#include "stdafx.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "../libutils/logging.h"
#include "../libutils/Exception.h"
#include "../librf/spidev_lib++.h"
#include "../librf/RFM69OOKregisters.h"
#include "../librf/RFM69OOK.h"
#include "../librf/RFParser.h"
#include "../librf/RFProtocolLivolo.h"
#include "../librf/RFProtocolX10.h"
#include "../librf/RFProtocolRST.h"
#include "../librf/RFProtocolRaex.h"
#include "MqttConnect.h"

typedef uint32_t __u32;
#define LIRC_GET_REC_MODE              _IOR('i', 0x00000002, __u32)
#define lirc_t base_type
#define PULSE_BIT       0x01000000
#define PULSE_MASK      0x00FFFFFF

int waitfordata(int fd, unsigned long maxusec)
{
	fd_set fds;
	int ret;
	struct timeval tv;

	while (1) {
		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		do { 
			do {
				if (maxusec > 0) {
					tv.tv_sec = maxusec / 1000000;
					tv.tv_usec = maxusec % 1000000;
					ret = select(fd + 1, &fds, NULL, NULL, &tv);
					if (ret == 0)
						return (0);
				} else {
					ret = select(fd + 1, &fds, NULL, NULL, NULL);
				}
			}
			while (ret == -1 && errno == EINTR);
			if (ret == -1) {
				CLog::GetLog("Main")->Printf(0, "RF select() failed\n");
				continue;
			}
		}
		while (ret == -1);

		if (FD_ISSET(fd, &fds)) {
			/* we will read later */
			return (1);
		}
	}

	return false;
}


int main(int argc, char* argv[])
{
	bool bDebug = false;

	if (argc>=2 && !strcmp(argv[1], "-D"))
	{
		bDebug = true;
	}

	CLog *m_Log = CLog::GetLog("Main");
	m_Log->SetLogLevel(3);
	m_Log->SetConsoleLogLevel(4);

	spi_config_t spi_config;
	spi_config.mode=0;
	spi_config.speed=500000;
	spi_config.delay=0;
	spi_config.bits_per_word=8;
	SPI mySPI("/dev/spidev32766.0",&spi_config);
	if (!mySPI.begin())
	{
		m_Log->Printf(0, "SPI init failed");
		return 1;
	}

    RFM69OOK rfm(&mySPI);
    rfm.initialize();
    rfm.receiveBegin();

	CMqttConnection conn("localhost", m_Log);
//	conn.NewMessage("X10:D10ON");
	//conn.NewMessage("RST:id=1b00 h=37 t=27.2");

	string m_lircDevice="/dev/lirc0";
	int fd = open(m_lircDevice.c_str(), O_RDONLY);
	if (fd == -1) {
		m_Log->Printf(0, "error opening %s\n", m_lircDevice.c_str());
		exit(EXIT_FAILURE);
	};

	struct stat s;
	__u32 mode = 2;

	if ((fstat(fd, &s) != -1) && (S_ISFIFO(s.st_mode))) {
		/* can't do ioctls on a pipe */
	}
	else if ((fstat(fd, &s) != -1) && (!S_ISCHR(s.st_mode))) {
		close(fd);
		throw CHaException(CHaException::ErrBadParam,"%s is not a character device\n", m_lircDevice.c_str());
	}
	else if (ioctl(fd, LIRC_GET_REC_MODE, &mode) == -1) {
		throw CHaException(CHaException::ErrBadParam,"This program is only intended for receivers supporting the pulse/space layer.");
	}

	CRFParser m_parser(m_Log, bDebug?".":"");
	m_parser.AddProtocol("All");

	const size_t BUFFER_SIZE = 1024 * 1024;
	lirc_t *data = new lirc_t[BUFFER_SIZE];
	lirc_t *data_ptr = data;
	time_t lastReport = 0, packetStart = time(NULL);
	bool m_bExecute=true;
	int lastRSSI = -1000, minGoodRSSI=0;

	while (m_bExecute) {
		int result;
		usleep(10);

		size_t count = sizeof(lirc_t)*BUFFER_SIZE - (data_ptr - data)*sizeof(lirc_t);

		if (count == 0)
		{
			m_Log->Printf(0, "RF buffer full");
			data_ptr = data;
			continue;
		}

		if (lastReport != time(NULL) && data_ptr - data>=32)
		{
			m_Log->Printf(4,"RF got data %ld bytes. RSSI=%d", data_ptr - data, lastRSSI);
			lastReport = time(NULL);
		}

		if (data_ptr == data)
		{
			packetStart = time(NULL);
		}
		else if (data_ptr - data<32)
		{

		}
		else if (!waitfordata(fd, 300000) || data_ptr - data>10000 || time(NULL)-packetStart>2)
		{
			string parsedResult = m_parser.Parse(data, data_ptr - data);
			if (parsedResult.length())
			{
				m_Log->Printf(3, "RF Recieved: %s. RSSI=%d (%d)", parsedResult.c_str(), lastRSSI, minGoodRSSI);
				conn.NewMessage(parsedResult);
				if (minGoodRSSI>lastRSSI)
					minGoodRSSI=lastRSSI;
			}
			else
			{
				m_Log->Printf(4, "Recieved %ld signals. Not decoded\n", data_ptr - data);
			}
			data_ptr = data;
			packetStart = time(NULL);
		}

		result = read(fd, (void *)data_ptr, count);
		if (result == 0) {
			fprintf(stderr, "read() failed\n");
			break;
		}
		lastRSSI = rfm.readRSSI();

		data_ptr += result / sizeof(lirc_t);
	};

	delete []data;
	close(fd);


}
