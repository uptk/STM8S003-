#include "common.h"


void Mymemcpy(u8 *dst,u8 *src,u8 n)
{
	while(n--)
		*(dst++) = *(src++);
}

u8 Mymemcmp(u8 *dst,u8 *src,u8 n)
{
	while(n--){
		if((*(dst++)) != (*(src++)))
			return 0;
	}
	return 1;
}

u8 checkSum(u8 *s,u8 n)
{
	u8 sum = 0;
	while(n--)
		sum += *(s++);
	return sum;
}

u8 cal_1(u8 val)
{
	u8 ret = 0;
	while(val){
		ret += val&1;
		val >>= 1;
	}
	return ret;
}


void nopDelay(u8 val)
{
	while(val--){
		#if defined(KEIL_C51)
		_nop_();
		#elif defined(STM8S003)
		nop();
		#endif
	}
}

void  delay_us(u8 us)
{
    while(us--)
    {
    	;
    }
}

void delay_ms(u8 ms)
{
	while(ms>0){
		u8 i=0;
		while(i<11){
			delay_us(125);
			i++;
		}
		ms--;
	}
}
s8 rawBinSearch16(u16 const *a,u8 len,u16 ele)
{
	u8 f,b,m;
	if(len == 0)
		return -1;
	if(a[0] <= ele)
		return 0;
	if(a[len - 1] == ele)
		return len-1;
	if(a[len - 1] > ele)
		return len;
	f = 0;
	b = len - 1;
	while(f <=  b){
		m = (f+b)/2;
		if(ele > a[m]){
			b = m-1;
		}else if(ele < a[m]){
			f = m+1;
		}else
			return m;
	}
	//a[b]>ele>a[f]
	return f;
}

s8 rawBinSearch(u8 const *a,u8 len,u8 ele)
{
	u8 f,b,m;
	if(len == 0)
		return -1;
	if(a[0] <= ele)
		return 0;
	if(a[len - 1] == ele)
		return len-1;
	if(a[len - 1] > ele)
		return len;
	f = 0;
	b = len - 1;
	while(f <=  b){
		m = (f+b)/2;
		if(ele > a[m]){
			b = m-1;
		}else if(ele < a[m]){
			f = m+1;
		}else
			return m;
	}
	//a[b]>ele>a[f]
	return f;
}

u8 binSearch16(u16 const *a,u8 len,u16 ele)//a[0]最大，a[len - 1]
{
	s8 ret = rawBinSearch16(a,len,ele);
	if(ret <= 0){
		return 0;
	}else if(ret >= len){
		return (len-1);
	}else{
		return ((a[ret-1] - ele)<=(ele - a[ret]))?(ret-1):ret;
	}

}
u8 binSearch(u8 const *a,u8 len,u8 ele)//a[0]最大，a[len - 1]最小
{
	s8 ret = rawBinSearch(a,len,ele);
	if(ret <= 0){
		return 0;
	}else if(ret >= len){
		return (len-1);
	}else{
		return ((a[ret-1] - ele)<=(ele - a[ret]))?(ret-1):ret;
	}
}

u16 getMid16(u16 *buf)
{
	if((buf[2] >= buf[1])){
		if(buf[1] >= buf[0])
			return buf[1];
		if(buf[0] >= buf[2])
			return buf[2];
		return buf[0];
	}else{
		if(buf[2] >= buf[0])
			return buf[2];
		if(buf[0] >= buf[1])
			return buf[1];
		return buf[0];
	}
}

u8 tryGetInc(u8 rold,u8 rnew)
{
	u8 ret;
	if(rnew >= rold)
		ret = (rnew - rold);
	else
		ret = (0xff - (rold - rnew) + 1);
	
	return ret;
}

/*计算CRC码的步骤为：

(1).预置16位寄存器为FFFFH。称此寄存器为CRC寄存器；

(2).把第一个8位数据与CRC寄存器的低位相异或，把结果放于CRC寄存器；

(3).把寄存器的内容右移一位(朝低位)，用0填补最高位，检查最低位；

(4).如果最低位为0：重复第3步(再次移位)如果最低位为1：CRC寄存器与多项式A001（1010 0000 0000 0001）进行异或；

(5).重复步骤3和4，直到右移8次，这样整个8位数据全部进行了处理；

(6).重复步骤2到步骤5，进行下一个8位数据的处理；

(7).最后得到的CRC寄存器即为CRC码。(CRC码 = CRC_L +CRC_H)

*/

/*****************crc校验**********************************************/

//crc生成函数，并将crc存储在预先定于的数组中

//调用方式crc16（指向数据的指针，需要校验的数据长度）

unsigned char ACRC[2];//定义数组

void Acrc16(unsigned char *ptr,unsigned int len)

{

unsigned long wcrc=0XFFFF;//预置16位crc寄存器，初值全部为1

unsigned char temp;//定义中间变量

int i=0,j=0;//定义计数

for(i=0;i<len;i++)//循环计算每个数据

{

temp=*ptr&0X00FF;//将八位数据与crc寄存器亦或

ptr++;//指针地址增加，指向下个数据

wcrc^=temp;//将数据存入crc寄存器

for(j=0;j<8;j++)//循环计算数据的

{

if(wcrc&0X0001)//判断右移出的是不是1，如果是1则与多项式进行异或。

{

wcrc>>=1;//先将数据右移一位

wcrc^=0XA001;//与上面的多项式进行异或

}

else//如果不是1，则直接移出

{

wcrc>>=1;//直接移出

}

}

}

temp=wcrc;//crc的值

ACRC[0]=wcrc;//crc的低八位

ACRC[1]=wcrc>>8;//crc的高八位

}

