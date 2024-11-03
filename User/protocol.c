#include "protocol.h"
#include "stdio.h"
#include "crc16.h"
#include "key.h"
#include "ws2812b.h"

// ͨ��Э�����:
// ������ID  ������ID  �û���ID  ������  ָ����  У�����8λ  У�����8λ

// �豸ID��, ���·����ݵ�ʱ��, ң�����豸ID�ž��Ƿ�����ID, �������ݵ�ʱ��, ���ǽ�����ID
unsigned char Device_ID = 0xff;  
// ��ͬң���������ջ����豸�û���ID��ͬ, ��Ҫ���Դ����ֲ�ͬ���豸
unsigned char Device_User_Groups_ID = 0x01;
// 1̨ң�������Կ���12̨���ջ�, ���12̨���ջ�����12����ͬ���豸ID��
unsigned char Receiver_Device_ID[12] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11};
// ������˵��:
// 0x01 (����ָ��, ң�����·������ջ�)
// 0x02 (�����������, ����ִ�н���, ���ջ��Զ��ϱ�������ң����)
// 0x03 (���ָ��, ң�����·������ջ�, ������״̬������״̬)
// 0x04 (��ⷴ��ָ��, ���ջ��ϱ������, ��������״̬������״̬)

// �жϷ�����ID�Ƿ��ڱ��û�����
unsigned char isUserIDPresent(unsigned char sender_id) {
    for (int i = 0; i < 12; i++) {
        if (Receiver_Device_ID[i] == sender_id) {
            return 1;
        }
    }
    return 0;
}

// �������ջ����� -> ��ⷴ��
// ��⵽���߾ͷ�������, ����ֻ����һ��, ��������ɫ, һ�������Ӿ�ֱ�����
void Parse_check_Feedback_data(unsigned char sender_id, unsigned char cmd) {
	unsigned char number = Get_Key_Select_Receiver_Number();

	if(number >= 1 && Key_Select_Receiver == 1) {
		if(cmd == 0x01) {
			WS2812B_SetColor(12, RGB_BLUE);
			WS2812B_SendColor();
		}
	}
	// printf("ѡ����ջ�������:%d\r\n",number);
	// printf("������\r\n");
}


// ����У����Ƶ����
void Parse_Receive_data(char* buff, unsigned short len) {
	if(len >= 7) {
		// printf("len:%d\r\n",len);
		unsigned char crcResult = verifyCRC(buff, len);
		if (crcResult == 1) {
			unsigned char Sender_ID  = buff[0];     // ������ID��������ָ���ǽ��ջ�
			unsigned char Receive_ID = buff[1];     // ������ID��������ָ���Ǳ��豸��ң������
			unsigned char User_Groups_ID = buff[2]; // �û���ID�����ֲ�ͬң��������ͬ�Ľ��ջ������û���ID����
			unsigned char Use_Code = buff[3];       // ������
			unsigned char Cmd_Code = buff[4];       // ָ����
			// ���������ID������豸ID������
			if(Receive_ID == Device_ID) {
				// ����豸�û���ID�����Ҳ����
				if(User_Groups_ID == Device_User_Groups_ID) {
					// ������ID�����ڱ��û�����, ������
					if(isUserIDPresent(Sender_ID) == 1) {
						// ����һ����˵�������ݾ��Ƿ��͸����豸����
						if(Use_Code == 0x04) {
							Parse_check_Feedback_data(Sender_ID, Cmd_Code);
						}
					}
				}
			}
    	}
	}
}

