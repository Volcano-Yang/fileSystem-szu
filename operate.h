#include <time.h>

#ifndef OPERATE_H_INCLUDED
#define OPERATE_H_INCLUDED
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

int createFile(char fileName[], int fileSize, int auth);
int creatFCB(int fcbBlockNum, int fileBlockNum, int fileSize, int auth);
int findFile(char name[]);
int deleteFile(char fileName[]);
int releaseFile(int FCBBlock);
int addToDir(struct dir* dir, char fileName[], int type, int fcbBlock);
int doWrite(struct FCB* fcb, char content[]);
int doRead(struct FCB* fcb, int length);
int doDelete(int index);
int readFile(char fileName[], int length);
int writeFile(char fileName[], char content[]);
int renameFile(char oldName[], char newName[]);
int changeDir(char dirName[]);
// int mkdir(char name[]);
int releaseMemory(int fcbBlock);
int releaseBlock(int blockNum, int blockSize);
int removeDir(char dirName[]);
int renameItem(char name[]);
void ls();
int deleteInParentDir(int unitIndex);
void exitSystem();

void do_main();
int main();
void start();
#endif