#ifndef __WS2812B_H
#define __WS2812B_H

#define RGB_RED 	0x00FF00 // 红色
#define RGB_GREEN 	0xFF0000 // 绿色
#define RGB_BLUE	0x0000FF // 蓝色
#define RGB_PURPLE	0x00FFFF // 紫色
#define RGB_YELLOW  0xFFFF00 // 黄色
#define RGB_BLACK   0x000000 // 无色（熄灭）
#define RGB_WHITE   0xFFFFFF // 白色

void WS2812B_Init(void);
void WS2812B_SetColor(unsigned char RGB, unsigned int Color);
void WS2812B_SendColor(void);
void WS2812B_WholeOFF(void);
void WS2812B_Reset(void);

void WS2812_test(void);

#endif
