#include <time.h>
#ifndef FS_H_INCLUDED
#define FS_H_INCLUDED


// 以下是
// 关于磁盘的定义

//系统大小100m
#define SYSTEMSIZE 100*1024*1024   
//盘块大小，按kb划分
#define BLOCKSIZE 1024 
//系统盘块数目
#define BLOCKCOUNT SYSTEMSIZE/BLOCKSIZE 

void start();
//磁盘分配
int getBlock(int blockSize);
//获得盘块的物理地址
char* getBlockAddr(int blockNum);
//获得物理地址盘块号
int getAddr(char* addr);
//释放盘块
int releaseBlock(int blockNum, int blockSize);
//系统起始地址
char* systemStartAddr;  
// #endif

// 以下是
// 关于文件系统操作的定义
#define dirMaxSize 15

// 文件控制块
struct FCB {
    int protect; // 权限标志, 0为不可读不可写，1为只读，2为可读可写
    int readptr;    //读指针，字节为单位
    int blockNum;   //文件数据起始盘块号
    int fileSize;   //文件大小，盘块为单位
    int dataSize;   //已写入的内容大小，字节为单位
};
// 文件或目录结构：
struct dirFile{
    char fileName[32];  //文件名
    int startBlock; //FCB起始盘块
    char* time; // 创建时间
    int type; // 0为文件 1为目录
};

//目录表结构：
struct dir {
    int total; // 目录项数目
    struct dirFile items[dirMaxSize];// 目录中的文件列表
};
 

struct dir *rootdir; // 根目录
struct dir *currentDir; // 当前目录

void ls();


void do_main();
void helpInfo();
int main();

#endif