/*该头文件主要定义文件系统的磁盘各种变量 */
#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

/*磁盘操作接口定义*/

//定义文件系统的大小
#define MEM_D_SIZE 100*1024*1024
#define DISK_NUM 1024
#define DISKSIZE 1024
//根目录起始盘号
#define ROOT_DISK_NO FATSIZE/(DISKSIZE+1)
//根目录大小
#define ROOT_DISK_SIZE sizeof(struct dirOrFile)
//FAT表大小
#define FATSIZE DISK_NUM*sizeof(struct fatitem)
#define DIR_MAXSIZE 1024
//最大写入长度
#define MAX_WRITE 1024*128
#define MOFN 5
//最大子目录数
#define MSD 10

// 定义创建磁盘空间文件名称
#define DISKNAME "block.dat"

#endif

