#include <pic12f1572.h>
#include <stdint.h>

#include "drv_osc.h"

void osc_init()
{
#if		_XTAL_FREQ == 16000000
	OSCCON = 0x7A;		//HF
	while(!OSCSTATbits.HFIOFR);
#elif	_XTAL_FREQ == 8000000
	OSCCON = 0x72;		//HF
	while(!OSCSTATbits.HFIOFR);
#elif	_XTAL_FREQ == 4000000
	OSCCON = 0x6A;		//HF
	while(!OSCSTATbits.HFIOFR);
#elif	_XTAL_FREQ == 2000000
	OSCCON = 0x62;		//HF
	while(!OSCSTATbits.HFIOFR);
#elif	_XTAL_FREQ == 1000000
	OSCCON = 0x5A;		//HF
	while(!OSCSTATbits.HFIOFR);
#elif	_XTAL_FREQ == 500000
	OSCCON = 0x3A;		//MF
	while(!OSCSTATbits.MFIOFR);
#elif	_XTAL_FREQ == 250000
	OSCCON = 0x32;		//MF
	while(!OSCSTATbits.MFIOFR);
#elif	_XTAL_FREQ == 125000
	OSCCON = 0x2A;		//MF
	while(!OSCSTATbits.MFIOFR);
#elif	_XTAL_FREQ == 62500
	OSCCON = 0x22;		//MF
	while(!OSCSTATbits.MFIOFR);
#elif	_XTAL_FREQ == 31250
	OSCCON = 0x12;		//MF
	while(!OSCSTATbits.MFIOFR);
#elif	_XTAL_FREQ == 31000
	OSCCON = 0x02;		//LF
	while(!OSCSTATbits.LFIOFR);
#else
#error	"_XTAL_FREQ value invalid."
#endif	
}
