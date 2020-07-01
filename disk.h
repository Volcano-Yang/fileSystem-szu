#ifndef DISK_H_INCLUDED
#define DISK_H_INCLUDED
//磁盘操作接口
#define SYSTEMSIZE 100*1024*1024   //系统大小100m
#define BLOCKSIZE 1024 //盘块大小，按kb划分
#define BLOCKCOUNT SYSTEMSIZE/BLOCKSIZE //系统盘块数目

void start();
//磁盘分配
int getBlock(int blockSize);
//获得盘块的物理地址
char* getBlockAddr(int blockNum);
//获得物理地址盘块号
int getAddr(char* addr);
//释放盘块
int releaseBlock(int blockNum, int blockSize);
//退出系统
void exitSystem();

char* systemStartAddr;  //系统起始地址

#endif