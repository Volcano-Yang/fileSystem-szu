#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>


void main()
{
    printf("*************************************************************\n");
    printf("Welcome To My fileSystem(2017152032 杨志发sparkyang\n\n");
    printf("* 命令名\t命令参数\t\t命令说明\n");
    printf("* ls\t\t无\t\t\n");
    printf("* help\t\t无\t\t\t提示信息\n");
    printf("* exit\t\t无\t\t\t退出系统\n");
    printf("* cd\t\t目录名\t\t\t切换到该目录\n");
    printf("* write\t\t文件名 数据\t\t从末尾开始写数据到该文件\n");
    printf("* read\t\t文件名 长度\t\t读取该文件\n");
    printf("* touch\t\t文件名 大小 权限\t在当前目录下创建指定大小文件，并给用户权限\n");
    printf("* mkdir\t\t目录名\t\t\t在当前目录下创建文件夹\n");
    printf("* rm\t\t文件名/目录\t\t在当前目录下删除指定文件/目录\n");
    printf("* mv\t\t旧文件名 新文件名\t重命名\n");
    printf("*********************************************************************\n\n");
	/*********************************************************************/
	printf("Thank you for using my file system!\n");
}
