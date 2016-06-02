#include "stdafx.h"
#include "Serializable.h"
#include "../libutils/logging.h"


void CSerializable::Dump(CLog *log)
{
	log->Printf(1, "CSerializable::Dump");
}
