#ifndef __KEY_H
#define __KEY_H

extern unsigned char Key_Select_Receiver;
extern unsigned char Key_Select_State[28];

extern unsigned char Key8_Port_Select[6];
extern unsigned char Key9_Port_Select[6];
extern unsigned char Key10_Port_Select[6];
extern unsigned char Key11_Port_Select[6];
extern unsigned char Key12_Port_Select[6];
extern unsigned char Key13_Port_Select[6];

extern unsigned char Key15_Port_Select[6];
extern unsigned char Key16_Port_Select[6];
extern unsigned char Key17_Port_Select[6];
extern unsigned char Key18_Port_Select[6];
extern unsigned char Key19_Port_Select[6];
extern unsigned char Key20_Port_Select[6];

void Key_ADC1_Scanf(void);
void Key_ADC2_Scanf(void);
void Key_ADC3_Scanf(void);
void Key_ADC4_Scanf(void);

#endif
