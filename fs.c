#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<malloc.h>
#include"fs.h"
#include<time.h>

void helpInfo(){
    printf("=====================================================================\n");
    printf("Welcome To My fileSystem(2017152032 杨志发sparkyang\n\n");
    printf("* help\t\t无\t\t\t提示信息\n");

    printf("* ls\t\t无\t\t\t查看目录当前文件和文件夹\n");
    printf("* mkdir\t\t目录名\t\t\t在当前目录下创建文件夹\n");
    printf("* rmdir\t\t目录\t\t\t在当前目录下删除指定目录\n");
    printf("* rndir\t\t旧文件名 新文件名\t重命名\n");
    printf("* cd\t\t目录名\t\t\t切换到该目录\n");

    printf("* write\t\t文件名 数据\t\t从末尾开始写数据到该文件\n");
    printf("* read\t\t文件名 长度\t\t读取该文件\n");
    printf("* touch\t\t文件名 大小\t\t在当前目录下创建指定大小文件\n");
    printf("* delete\t文件名\t\t\t在当前目录下删除指定文件\n");
    
    printf("* exit\t\t无\t\t\t退出系统\n");
    printf("=====================================================================\n\n");
}

// 分配磁盘块
int getBlock(int blockSize)
{
    printf("getBlock()");
    int startBlock = 0;
    int sum = 0;
    for(int i = 0; i<BLOCKCOUNT; i++)
    {
        if(systemStartAddr[i] == '0') // 0为可用盘块
        {
            if(sum == 0)
        startBlock = i;
            sum++;
            if(sum == blockSize)
            {
        for(int j=startBlock; j<startBlock+blockSize; j++)
            systemStartAddr[j] = '1'; // 1表示分配盘块
        return startBlock;
            }
        } else sum = 0;
    }
    printf("get block fail, because memory is full or hasn't contiguous memory\n");
    return -1;
}

// 获得盘块的物理地址
char* getBlockAddr(int blockNum)
{
    printf("getBlockAddr()");
    return systemStartAddr + blockNum * BLOCKSIZE; //单位为byte
}

// 释放盘块
int releaseBlock(int blockNum, int blockSize)
{
    printf("releaseBlock()");
    int endBlock = blockNum + blockSize;
    for(int i=blockNum; i<endBlock; i++)
        systemStartAddr[i] = '0';
    return 0;
}

//退出系统12
void exitSystem()
{
    free(systemStartAddr);
    printf("exitSystem()");
}


// mkdir
int mkdir(char name[])
{
    printf("mkdir()");
    //为目录表分配空间
    int fcbBlock = getBlock(1);
    if(fcbBlock == -1) {
        printf("get block fail");
        return -1;
    }
    // 添加到当前目录下
    addToDir(currentDir, name, 1, fcbBlock);

    struct dir* newDir = (struct dir*)getBlockAddr(fcbBlock);
    newDir->total = 0;
    char parent[] = "../";
    // 为该目录创建父目录，这样就能实现cd ../上一级跳转
    addToDir(newDir, parent, 0, getAddr((char*)currentDir));
    return 0;
}

// rndir
int renameFile(char oldName[], char newName[])
{
    printf("renameFile()");
    int index = findFile(oldName);
    if(index == -1)
    {
        printf("Not found\n");
        return -1;
    }
    struct dirFile* item = &currentDir->items[index];
    int fcbBlock = item->startBlock;

    // 重命名
    strcpy(item->fileName, newName);
    
    return 0;
}

// rmdir
int removeDir(char dirName[])
{
    printf("removeDir()");
    int index = findFile(dirName);
    if(index == -1)
    {
        printf("Not found\n");
        return -1;
    }

    struct dirFile *item = &currentDir->items[index];
    int fcbBlock = item->startBlock;
    struct dir* temp = (struct dir*)getBlockAddr(fcbBlock);
    // 递归删掉该目录下的文件
    for(int i=1; i<temp->total; i++)
    {
        renameItem(temp->items[i].fileName);
    }
    //释放目录表空间
    releaseBlock(fcbBlock, 1);

    deleteInParentDir(index);
    return 0;
}


void start()
{
    printf("start()");
    //创建空间
    systemStartAddr = (char*)malloc(SYSTEMSIZE * sizeof(char));
    // 初始化盘块的位示图
    for(int i=0; i<BLOCKCOUNT; i++)
        systemStartAddr[i] = '0';
    int size = BLOCKCOUNT * sizeof(char) / BLOCKSIZE;//位示图占用盘块数
    for(int i=0; i<size; i++) // 从零开始分配
        systemStartAddr[i] = '1'; //1表示盘块已被使用

    // 分配空间给根目录
    int startBlock = getBlock(1);
    rootdir = (struct dir*)getBlockAddr(startBlock);
    rootdir->total = 0;
    currentDir = rootdir;
}


// 展示当前目录
void ls()
{
    int total = currentDir->total;
    for(int i=0; i<total; i++)
    {
        struct dirFile item = currentDir->items[i];
        if (strcmp(item.fileName, "../") == 0) continue; // 忽略../
        else if (item.type == 1) printf("\033[37m%s\t\033[0m", item.fileName); // 如果是文件夹，输出蓝色
        else printf("\033[32m%s\t\033[0m", item.fileName);
    }
    printf("\n");
}

void do_main()
{
    // 循环读取指令
    int flag = 1;
    while(flag)
    {
        printf("请输入你想执行的指令：");
        char cmd[100];
        gets(cmd); // 为了防止溢出用fgets代替gets 直至换行

        char command[100];

        // 3个变量存可选参数
        char option1[100];
        char option2[100];
        char option3[100];
        char space[] = " "; //定义空格变量
        

        // 分割命令字符串
        char *p = strtok(cmd, space);
        strcpy(command, p);
        p = strtok(NULL,space);
        if(p) {
            strcpy(option1, p);
            p = strtok(NULL,space); 
        }
        if(p) {
            strcpy(option2, p);
            p = strtok(NULL,space); 
        }
        if(p) {
            strcpy(option3, p);
        }

        // 判断指令　根据指令和参数执行对应操作
        //　这里换成switch会比较麻烦，因为switch不能直接判断字符串
        if (strcmp(command, "help") == 0) {
            printf("执行%s指令\n",command);
        } else if (strcmp(command, "ls") == 0) {
            printf("执行%s指令\n",command);
            ls();
        } else if (strcmp(command, "cd") == 0) {
            printf("执行%s指令\n",command);
        } else if (strcmp(command, "mkdir") == 0) {
            printf("执行%s指令\n",command);
        } else if (strcmp(command, "rmdir") == 0) {
            printf("执行%s指令\n",command);
        } else if (strcmp(command, "rndir") == 0) {
            printf("执行%s指令\n",command);
        } else if (strcmp(command, "touch") == 0) {
            printf("执行%s指令\n",command);
        } else if (strcmp(command, "read") == 0) {
            printf("执行%s指令\n",command);
        } else if (strcmp(command, "write") == 0) {
            printf("执行%s指令\n",command);
        } else if (strcmp(command, "delete") == 0) {
            printf("执行%s指令\n",command);
        } else if (strcmp(command, "exit") == 0) {
            printf("执行%s指令\n",command);
            printf("谢谢你使用我的文件管理系统!\n");
            flag = 0; // 退出循环
        } else {
            printf("没有找到对应指令\n");
        }
    }
}


int main()
{
    helpInfo();
    do_main();
    return 0;
}
