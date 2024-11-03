#include "protocol.h"
#include "stdio.h"
#include "crc16.h"
#include "key.h"
#include "ws2812b.h"

// 通信协议组成:
// 发送者ID  接收者ID  用户组ID  功能码  指令码  校验码低8位  校验码高8位

// 设备ID号, 在下发数据的时候, 遥控器设备ID号就是发送者ID, 接收数据的时候, 就是接收者ID
unsigned char Device_ID = 0xff;  
// 不同遥控器、接收机的设备用户组ID不同, 主要是以此区分不同的设备
unsigned char Device_User_Groups_ID = 0x01;
// 1台遥控器可以控制12台接收机, 因此12台接收机就有12个不同的设备ID号
unsigned char Receiver_Device_ID[12] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11};
// 功能码说明:
// 0x01 (引爆指令, 遥控器下发给接收机)
// 0x02 (引爆结果反馈, 引爆执行结束, 接收机自动上报反馈给遥控器)
// 0x03 (检测指令, 遥控器下发给接收机, 检测端子状态和引爆状态)
// 0x04 (检测反馈指令, 接收机上报检测结果, 包含端子状态和引爆状态)

// 判断发送者ID是否在本用户组内
unsigned char isUserIDPresent(unsigned char sender_id) {
    for (int i = 0; i < 12; i++) {
        if (Receiver_Device_ID[i] == sender_id) {
            return 1;
        }
    }
    return 0;
}

// 解析接收机数据 -> 检测反馈
// 检测到接线就反馈正常, 哪怕只接了一个, 都反馈绿色, 一个都不接就直接灭灯
void Parse_check_Feedback_data(unsigned char sender_id, unsigned char cmd) {
	unsigned char number = Get_Key_Select_Receiver_Number();

	if(number >= 1 && Key_Select_Receiver == 1) {
		if(cmd == 0x01) {
			WS2812B_SetColor(12, RGB_BLUE);
			WS2812B_SendColor();
		}
	}
	// printf("选择接收机的数量:%d\r\n",number);
	// printf("进来了\r\n");
}


// 接收校验射频数据
void Parse_Receive_data(char* buff, unsigned short len) {
	if(len >= 7) {
		// printf("len:%d\r\n",len);
		unsigned char crcResult = verifyCRC(buff, len);
		if (crcResult == 1) {
			unsigned char Sender_ID  = buff[0];     // 发送者ID，在这里指的是接收机
			unsigned char Receive_ID = buff[1];     // 接收者ID，在这里指的是本设备（遥控器）
			unsigned char User_Groups_ID = buff[2]; // 用户组ID，区分不同遥控器、不同的接收机依靠用户组ID区分
			unsigned char Use_Code = buff[3];       // 功能码
			unsigned char Cmd_Code = buff[4];       // 指令码
			// 如果接收者ID不相等设备ID不处理
			if(Receive_ID == Device_ID) {
				// 如果设备用户组ID不相等也处理
				if(User_Groups_ID == Device_User_Groups_ID) {
					// 发送者ID不属于本用户组内, 不处理
					if(isUserIDPresent(Sender_ID) == 1) {
						// 到这一步就说明此数据就是发送给本设备的了
						if(Use_Code == 0x04) {
							Parse_check_Feedback_data(Sender_ID, Cmd_Code);
						}
					}
				}
			}
    	}
	}
}

