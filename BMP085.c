


#define BMP085_R 0xEF //   address
#define BMP085_W 0xEE //  address
#define OSS 3 // Oversampling Setting
#include <math.h>
#include <delay.h>

signed short ac1;
signed short ac2;
signed short ac3;
signed short b1;
signed short b2;
signed short mb;
signed short mc;
signed short md;
unsigned short ac4;
unsigned short ac5;
unsigned short ac6;
unsigned long pressure_1;

void bmp085Convert(unsigned long press, unsigned long *pressure, long *temperature)
{
 long  x1, x2, x3, b3, b5 ,b6, ut, up, p;
unsigned long b4, b7;

   
   up = press;

x1 = (((long) 27898 - (long) ac6) * (long) ac5) >> 15;     //исправить 27898 на реальное ut
x2 = ((long) mc << 11) / (x1 + md);
b5 = x1 + x2;
*temperature = ((b5 + 8) >> 4);


b6 = b5 - 4000;
x1 = (b2 * ((b6 * b6) >> 12)) >> 11;
x2 = (ac2 * b6) >> 11;
x3 = x1 +x2;
b3 = ((((long)ac1 *4 + x3) <<OSS) + 2) >> 2;
x1 = (ac3* b6) >> 13;
x2 = (b1 * ((b6*b6) >> 12) ) >> 16;
x3 = ((x1 + x2) + 2) >> 2;
b4 = (ac4 * (unsigned long) (x3 + 32768)) >> 15;
b7 = ((unsigned long)up - b3) * (50000>>OSS);
if (b7 < 0x80000000)
    p = (b7 << 1) / b4;
else
    p = (b7 / b4) << 1;

x1 = (p >> 8)*(p >> 8);
x1 = (x1 * 3038) >> 16;
x2 = (-7357 * p) >> 16;

*pressure = pressure_1 = p + ((x1 + x2 + 3791) >> 4); //press in Pa

*pressure*=100;
                    //для отображения с десятыми долями на моем устройстве.
*pressure/=  1333;        // press in mm Hg   //надо делить на 133.322

}

