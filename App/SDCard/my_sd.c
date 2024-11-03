#include "my_sd.h"
#include "sd_driver.h"
#include "ff.h"
#include "delay.h"
#include "stdio.h"
#include "string.h"
#include "LCD_USART3.h"

FATFS fp;
FIL file;

void My_SD_Init(void) {
	unsigned char i = 0;
	while(SD_Initialize() && i < 5) {
		Delay_Ms(500);
		i++;
	}
	printf("SD卡初始化成功\r\n");
	printf("SD卡容量大小:%u\r\n",SD_GetSectorCount());
	
	unsigned char res = f_mount (&fp, "0", 1);
	if(res == FR_OK) {
	  printf("注册工作区成功\r\n");
	}else {
	  printf("注册工作区失败\r\n");
	}
}

void My_SD_OpenLogFile(void) {
	unsigned char res = f_open(&file, "0:/log.txt", FA_READ | FA_WRITE | FA_OPEN_EXISTING);
	if (res == FR_OK) {
		printf("日志文件打开成功\r\n");
	} else if (res == FR_NO_FILE) {

		res = f_open(&file, "0:/log.txt", FA_READ | FA_WRITE | FA_CREATE_NEW);
		if (res == FR_OK) {
			printf("创建打开日志文件成功\r\n");
		} else {
			printf("创建系统日志文件失败\r\n");
			res=f_close(&file); 
		}
	} else {
		printf("日志文件打开出错:%d\r\n", res);	
		// res=f_close(&file); 
	}
} 


void My_SD_WriteLog(unsigned char* write_buff) {
	UINT bw = 0;
	f_lseek(&file, f_size(&file)); // 移动文件指针，实现追加写入
	unsigned char res = f_write(&file,write_buff,strlen((const char*)write_buff),&bw);
	if(res==FR_OK) {
		printf("成功写入数据:%d\r\n",bw);
		res=f_close(&file); 
	}else {
		printf("日志数据写入失败！！\r\n");
		res=f_close(&file); 
	}
}


void My_SD_ReadLog(void) {
    f_lseek(&file, 0);
    unsigned char read_buff[51]; 
    UINT br = 0;
    FRESULT res;
    
    do {
        res = f_read(&file, read_buff, sizeof(read_buff) - 1, &br);
        if (res == FR_OK) {
            read_buff[br] = '\0';
            unsigned char log[80];
            sprintf((char *)log, "slt_show_cont.txt+=\"%s\"", read_buff);
            printf("log:%s\r\n", log);
            LCD_USART3_SendString((unsigned char *)log);
            LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
        } else if (res != FR_OK) {
            break;
        }
    } while (br == sizeof(read_buff) - 1);
	res=f_close(&file); 
}
