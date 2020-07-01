#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<malloc.h>

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
