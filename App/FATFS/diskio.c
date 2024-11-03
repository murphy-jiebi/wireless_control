
/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/* http://elm-chan.org/fsw/ff/archives.html                              */
/* https://blog.csdn.net/hxj0323/article/details/108414536               */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		  /* FatFs lower layer API */
// #include "usbdisk.h"	  /* Example: Header file of existing USB MSD control module */
// #include "atadrive.h"  /* Example: Header file of existing ATA harddisk control module */
// #include "sdcard.h"	  /* Example: Header file of existing MMC/SDC contorl module */

#include "sd_driver.h"
/* Definitions of physical drive number for each drive */
// #define ATA		0	/* Example: Map ATA harddisk to physical drive 0 */
// #define MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
// #define USB		2	/* Example: Map USB MSD to physical drive 2 */


#define SD_CARD	0            /*  SD卡卷标 0  */
// #define SD_BLOCKSIZE 512  /*  SD卡扇区大小 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status  --> 获取设备(SD卡)状态函数                            */
/*-----------------------------------------------------------------------*/

/* Physical drive nmuber to identify the drive (识别驱动器) */
DSTATUS disk_status (BYTE pdrv) {
	switch (pdrv) {
	case SD_CARD:
		return RES_OK; /* 由于本框架不需要获取存储设备的状态 , 直接返回RES_OK即可. */
		// break;
	}
	return STA_NOINIT;
}


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive  --> 初始化驱动器                                   */
/*-----------------------------------------------------------------------*/

/* Physical drive nmuber to identify the drive (识别驱动器) */
DSTATUS disk_initialize (BYTE pdrv) {
	DSTATUS res = RES_OK;  /* DSTATUS --> unsigned char */
    switch (pdrv) {
        case SD_CARD:
            res = SD_Initialize();   /* sd_driver.h */
            break;
        default:
			// res = 1;
            return STA_NOINIT;  /* Drive not initialized (驱动器未初始化) */
    }
    return res == 0 ? RES_OK : STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)   --> 读取扇区                                          */
/*-----------------------------------------------------------------------*/

/* Physical drive nmuber to identify the drive (磁盘编号 , 识别驱动器) */
/* Data buffer to store read data (数据接收缓冲首地址) */
/* Sector address in LBA (扇区首地址) */
/* Number of sectors to read (扇区数量) */

DRESULT disk_read (BYTE pdrv, BYTE *buff, DWORD sector,	UINT count) {

	if(count == 0) {
		return RES_PARERR;  /* count不能等于0 , 否则返回参数错误 */
	}

	DSTATUS res = RES_OK; /* DSTATUS --> unsigned char */
	switch (pdrv) {
		case SD_CARD:
			res = SD_ReadDisk((unsigned char*)buff, sector, count);   /*  sd_driver.h  */
			/*
			unsigned char i = 2;
			do {
    			SD_Initialize();
    			res = SD_ReadDisk((unsigned char*)buff, sector, count);
   			 	i--;
			} while (res && i > 0); */
			break;
		default:
		    // res = 1;
			return RES_ERROR;  /* 1: R/W Error */
	}
	return res == 0 ? RES_OK : RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)  --> 扇区写入                                          */
/*-----------------------------------------------------------------------*/

/* Physical drive nmuber to identify the drive (磁盘编号 , 识别驱动器) */
/* Data buffer to store read data (数据接收缓冲首地址) */
/* Sector address in LBA (扇区首地址) */
/* Number of sectors to read (扇区数量) */

#if _USE_WRITE

DRESULT disk_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count) {
	if(count == 0) {
		return RES_PARERR;  /* count不能等于0 , 否则返回参数错误 */
	}
	DRESULT res = RES_OK; 
	switch (pdrv) {
		case SD_CARD:
		    // enumerated type mixed with another type(188-D)
			res = (DRESULT)SD_WriteDisk((unsigned char*)buff, sector, count);  /* sd_driver.h */

			/* 写入出错可尝试重新初始化SD卡 , 继续写入 , 这里需要控制重试的次数 , 防止死循环 */
			// unsigned char i = 2; /* 最多尝试 2次 ,仍然失败就没有必要再试了 */
			// do {
    			// SD_Initialize();
    			// res = (DRESULT)SD_WriteDisk((unsigned char*)buff, sector, count);
   			 	// i--;
			// } while (res && i > 0);
			break;
	}
	return res == 0 ? RES_OK : RES_ERROR;
}

#endif



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

/* Physical drive nmuber (0..) (磁盘编号 , 识别驱动器) */
/* Control code (执行指令) */
/* Buffer to send/receive control data (数据的缓冲区) */

#if _USE_IOCTL

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff) { 
	DRESULT res = RES_OK; 
	if(pdrv == SD_CARD) {
	    switch(cmd) {
			case CTRL_SYNC:         /* 执行同步刷新缓存操作 , 这里省略 */
				break;
			case GET_SECTOR_COUNT:  /* 总扇区数量  */
				*(DWORD *)buff = SD_GetSectorCount();
				break;
			case GET_SECTOR_SIZE:  /* 每个扇区大小 , 扇区大小必是512 */
				// *(DWORD *)buff = SD_BLOCKSIZE;
				*(DWORD *)buff = 512;
				break;
			case GET_BLOCK_SIZE:  /* 同时擦除扇区个数 */
			 	*(DWORD *)buff = 1;
				break;
			case CTRL_TRIM:       /* 执行擦除操作 , 这里省略*/
				break;
			default:
				res = RES_PARERR; /* cmd指令不正确 , 返回参数错误 */
		    break;
		}
	}else{
		res = RES_ERROR; /* 本框架默认只支持SD卡 , 可自行添加存储对象 */
	}
	return res;
}

#endif


/* 如果不需要使用文件时间功能和文件长名 , 以下函数代码可以不使用 , 长文件名需要malloc存储区 */

/* 

DWORD get_fattime (void) {
	// 没有RTC的时候 , 不建议使用此功能
	// 由 FF_FS_NORTC 决定是否启用 ,FF_FS_NORTC设置为 1 禁用时间戳功能 , 本框架不使用
	return 0; 
}

#include <stdlib.h>

void* ff_memalloc (UINT msize) {
	return malloc(msize);
}

void ff_memfree (void* mblock) {
	free(mblock);
} 

*/

