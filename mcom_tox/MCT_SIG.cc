#include "MCOMMTOX.h"
#include "xwintox.h"

void MCOMMTOX::recvSignal(unsigned int dwType, PBMessage_t* msg)
{
	switch (dwType)
	{
		case clSaveData:
			saveToxData_();
			break;
	}
}