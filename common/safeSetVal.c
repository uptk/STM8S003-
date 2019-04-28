#include "common.h"

u8 safeDec16(u16 *dat,u16 val,u16 max,u16 min)
{
	u8 ret = 0;
	u16 data_bak = *dat;
		if((*dat >= val) && ((*dat - val) >= min)){
			*dat -= val;
		}else
			*dat = min;
		if(data_bak != *dat)
			ret |= 1;
		if(min == *dat){
			ret |= 2;
		}
	return ret;
}
u8 safeInc16(u16 *dat,u16 val,u16 max,u16 min)
{
	u8 ret = 0;
	u16 data_bak = *dat;
		if((max >= val) && ((max - val) >= *dat)){
			*dat += val;
		}else
			*dat = max;
		if(data_bak != *dat)
			ret |= 1;
		if((max == *dat)){
			ret |= 2;
		}
	return ret;
}