# rfsniffer
Целью проекта является поддержка приема сигналов от различных радиоустройств диапазона 433.92 (метеодатчики, пульты и т.п.) с помощью радиомодуля RFM69 и ядерного linux драйвера, эмулирующего lirc устройство

Для сборки требуется Docker контейнер из http://contactless.ru/wiki/index.php/%D0%9A%D0%B0%D0%BA_%D1%80%D0%B0%D0%B7%D1%80%D0%B0%D0%B1%D0%B0%D1%82%D1%8B%D0%B2%D0%B0%D1%82%D1%8C_%D0%9F%D0%9E_%D0%B4%D0%BB%D1%8F_Wiren_Board

Сборка:
autoreconf -fvi
./configure
make

В результате получается 4 нужных файла:
libutils.so.0 (libutils/.libs/libutils.so.0)
librf.so.0 (librf/.libs/librf.so.0)
rftest (tests/.libs/rftest)
rfsniffer (rfsniffer/.libs/rfsniffer)

Необходимо скопировать lib файла на WB /usr/lib, а rftest и rfsniffer куда-либо на устройство
