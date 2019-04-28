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

u8 binSearch16(u16 const *a,u8 len,u16 ele)//a[0]���a[len - 1]
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
u8 binSearch(u8 const *a,u8 len,u8 ele)//a[0]���a[len - 1]��С
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

/*����CRC��Ĳ���Ϊ��

(1).Ԥ��16λ�Ĵ���ΪFFFFH���ƴ˼Ĵ���ΪCRC�Ĵ�����

(2).�ѵ�һ��8λ������CRC�Ĵ����ĵ�λ����򣬰ѽ������CRC�Ĵ�����

(3).�ѼĴ�������������һλ(����λ)����0����λ��������λ��

(4).������λΪ0���ظ���3��(�ٴ���λ)������λΪ1��CRC�Ĵ��������ʽA001��1010 0000 0000 0001���������

(5).�ظ�����3��4��ֱ������8�Σ���������8λ����ȫ�������˴���

(6).�ظ�����2������5��������һ��8λ���ݵĴ���

(7).���õ���CRC�Ĵ�����ΪCRC�롣(CRC�� = CRC_L +CRC_H)

*/

/*****************crcУ��**********************************************/

//crc���ɺ���������crc�洢��Ԥ�ȶ��ڵ�������

//���÷�ʽcrc16��ָ�����ݵ�ָ�룬��ҪУ������ݳ��ȣ�

unsigned char ACRC[2];//��������

void Acrc16(unsigned char *ptr,unsigned int len)

{

unsigned long wcrc=0XFFFF;//Ԥ��16λcrc�Ĵ�������ֵȫ��Ϊ1

unsigned char temp;//�����м����

int i=0,j=0;//�������

for(i=0;i<len;i++)//ѭ������ÿ������

{

temp=*ptr&0X00FF;//����λ������crc�Ĵ������

ptr++;//ָ���ַ���ӣ�ָ���¸�����

wcrc^=temp;//�����ݴ���crc�Ĵ���

for(j=0;j<8;j++)//ѭ���������ݵ�

{

if(wcrc&0X0001)//�ж����Ƴ����ǲ���1�������1�������ʽ�������

{

wcrc>>=1;//�Ƚ���������һλ

wcrc^=0XA001;//������Ķ���ʽ�������

}

else//�������1����ֱ���Ƴ�

{

wcrc>>=1;//ֱ���Ƴ�

}

}

}

temp=wcrc;//crc��ֵ

ACRC[0]=wcrc;//crc�ĵͰ�λ

ACRC[1]=wcrc>>8;//crc�ĸ߰�λ

}

