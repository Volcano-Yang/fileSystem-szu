#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include"block.h"
#include"operate.h"
#include"filesystem.h"

/* 信息打印部分 */
void helpinfo() {
    console("helpinfo");
	printf("=====================================================================\n");
    printf("Welcome To My fileSystem(2017152032 杨志发VolcanoYang\n\n");
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

void writeTerminalHead() {
    console("writeTerminalHead");
	printf("VolcanoYang2017152032&%s>", dirPath);
}


/* 初始化部分 */
void initfile() {
    console("initfile");
    //创建空间
	fdisk = (char*)malloc(MEM_D_SIZE * sizeof(char));
	format();
    printf("成功创建了一个100M的block.dat用来模拟文件系统\n");
}

void judge() {
    console("enter");
	
	fdisk = (char*)malloc(MEM_D_SIZE * sizeof(char));
	FILE *fp;

    if ((fp = fopen(DISKNAME, "rb")) == NULL) {
		printf("Error:\n不可以打开文件\n");
		return;
	}

	else if (!fread(fdisk, MEM_D_SIZE, 1, fp)) {
		printf("Error:\n不可以读取文件\n");
		exit(0);
	}

    else{
        fat = (struct fatitem*)(fdisk + DISKSIZE);
	    root = (struct direct*)(fdisk + DISKSIZE + FATSIZE);
	    for (int i = 0; i < MOFN; i++) {
	    	strcpy(openFile.opeitem[i].name, "");
	    	openFile.opeitem[i].size = 0;
	    	openFile.opeitem[i].firstdisk = -1;
	    }
	    cur_dir = root;
	    dirPath = (char *)malloc(DIR_MAXSIZE * sizeof(char));
	    openFile.cur_size = 0;
        strcpy(dirPath, "Root");
    }
	
}

void format() {
    console("format");
	int i;
	FILE *fp;
	fat = (struct fatitem*)(fdisk + DISKSIZE);
	fat[0].item = -1;
	fat[0].em_disk = '1';
	for (i = 1; i < ROOT_DISK_NO - 1; i++) {
		fat[i].item = i + 1;
		fat[i].em_disk = '1';
	}
	fat[ROOT_DISK_NO].item = -1;
	fat[ROOT_DISK_NO].em_disk = '1';
	for (i = ROOT_DISK_NO + 1; i < DISK_NUM; i++) {
		fat[i].item = -1;
		fat[i].em_disk = '0';
	}
	root = (struct direct*)(fdisk + DISKSIZE + FATSIZE);
	root->directitem[0].sign = 1;
	root->directitem[0].firstdisk = ROOT_DISK_NO;
	strcpy(root->directitem[0].name, ".");
	root->directitem[0].next = root->directitem[0].firstdisk;
	root->directitem[0].property = '1';
	root->directitem[0].size = ROOT_DISK_SIZE;

	root->directitem[1].sign = 1;
	root->directitem[1].firstdisk = ROOT_DISK_NO;
	strcpy(root->directitem[1].name, "..");
	root->directitem[1].next = root->directitem[0].firstdisk;
	root->directitem[1].property = '1';
	root->directitem[1].size = ROOT_DISK_SIZE;
	if ((fp = fopen(DISKNAME, "wb")) == NULL) {
		printf("Error:\nCannot open file\n");
		return;
	}
	for (i = 2; i < MSD+2; i++) {
		root->directitem[i].sign = 0;
		root->directitem[i].firstdisk = -1;
		strcpy(root->directitem[i].name, "");
		root->directitem[i].next = -1;
		root->directitem[i].property = '0';
		root->directitem[i].size = 0;
	}
	if ((fp = fopen(DISKNAME, "wb")) == NULL) {
		printf("Error:\nCannot open file\n");
		return;
	}
	if (fwrite(fdisk, MEM_D_SIZE, 1, fp) != 1) {
		printf("Error:\nFile write error!\n");
	}
	fclose(fp);
}


void start(){
    console("start");
    FILE *fp;
    char ch;
	if ((fp = fopen(DISKNAME, "rb")) == NULL) {
		printf("You have not format, do you want format?(y/s)");
		scanf("%c", &ch);
		if (ch == 'y') {
			initfile();
			printf("Succssfully format!\n");
		}
		else {
			return;
		}
	}
	judge();
	helpinfo();
	writeTerminalHead();
    doMain();
}

/* 释放空间部分 */

void exitSystem() {
    console("exitSystem");
	int i;
	FILE *fp;
	if ((fp = fopen(DISKNAME, "wb")) == NULL) {
		printf("Error:\n不可以打开文件！\n");
		return;
	}
	if (!fwrite(fdisk, MEM_D_SIZE, 1, fp)) {
		printf("Error:\n不可以读取文件!\n");
	}

	fclose(fp);
	free(dirPath);
	free(fdisk);
    return;
}


/*文件系统操作部分*/


/*循环控制部分*/
void doMain(){
    console("domain");
    char *contect;
	contect = (char *)malloc(MAX_WRITE * sizeof(char));
    int flag;
    int isRun=1;
    int r_size;
    int commandToalNumber =12;
    char *commandArr[12] = {"exit","touch","open","close","write","read","delete","mkdir","rmdir","ls","cd","help"};
    // 指令对照表
    // 0"exit"　
    // 1"touch"　
    // 2"open",
    // 3"close",
    // 4"write",
    // 5"read",
    // 6"delete",
    // 7"mkdir",
    // 8"rmdir",
    // 9"ls",
    // 10"cd",
    char name[10]; //定义输入的第二个参数
    
     while(isRun)
    {
        /*处理用户输入的指令 */
        char command[100];
        int commandNumber = -1;
        scanf("%s", command);
		for (int i = 0; i < commandToalNumber; i++) {
			if (!strcmp(commandArr[i], command))	
            {
                commandNumber=i;
                break;
            }
		}

        /*根据指令和参数执行对应操作*/
        switch (commandNumber) {
		case 0:
			free(contect);
			exitSystem();
            isRun=0;
            break;
		case 1:
			scanf("%s", name);
			int touchStatus = touch(name);
            switch(touchStatus){
                case -1:
                printf("Error：文件名过长,不能超过十位字符！\n");
                break;
                case -2:
                printf("Error:　该文件夹已满！\n");
                break;
                case -3:
                printf("Error:　当前打开的文件太多，请用close+文件名指令关闭！\n");
                break;
                case -4:
                printf("Error:　文件名重复！\n");
                break;
                case -5:
                printf("Error:　磁盘空间已满！\n");
                case 0:
                printf("创建文件成功！\n");
                break;
                default:
                printf("Error:　创建文件失败\n");
            }
			writeTerminalHead();
			break;
		case 2:
			scanf("%s", name);
			fd = open(name);
			if (fd == -1) {
				printf("Error：\nThe file not exit！\n");
			}
			else if (fd == -2) {
				printf("Error：\nThe file have already opened！\n");
			}
			else if (fd == -3) {
				printf("Error：\nThe number of file is too much！\n");
			}
			else if (fd == -4) {
				printf("Error：\nIt's a direct, can't open for read or write！\n");
			}
			else {
				printf("Successfully opened！\n");
			}
			writeTerminalHead();
			break;
		case 3:
			scanf("%s", name);
			flag = close(name);
			if (flag == -1) {
				printf("Error：\nThe file is not opened！\n");
			}
			else {
				printf("Successfully closed！\n");
			}
			writeTerminalHead();
			break;
		case 4:
			if (fd == -1) {
				printf("Error：\nThe file is not opened\n");
			}
			else {
				printf("Please input：");
				scanf("%s", contect);
				flag = write(fd, contect, strlen(contect));
				if (flag == 0) {
					printf("Successfully write！\n");
				}
				else {
					printf("Error：\nThe disk size is not enough!\n");
				}
				writeTerminalHead();
				break;
		case 5:
			if (fd == -1) {
				printf("Error：\nThe file is not opened\n");
			}
			else {
				flag = read(fd, contect);
				if (flag == 0) {
					for (int i = 0; i < openFile.opeitem[fd].size; i++) {
						printf("%c", contect[i]);
					}
					printf("\n");
				}
			}
			writeTerminalHead();
			break;
		case 6:
			scanf("%s", name);
			flag = del(name);
			if (flag == -1) {
				printf("Error：\nThe file not exit\n");
			}
			else if (flag == -2) {
				printf("Error：\nThe file is opened, please first close it！\n");
			}
			else if (flag == -3) {
				printf("Error：\nThe delete is not file！\n");
			}
			else {
				printf("Successfully delete！\n");
			}
			writeTerminalHead();
			break;
		case 7:
			scanf("%s", name);
			flag = mkdir(name);
			if (flag == -1) {
				printf("Error：\nThe length of name is too long！\n");
			}
			else if (flag == -2) {
				printf("Error：\nThe direct item is already full！\n");
			}
			else if (flag == -3) {
				printf("Error：\nThe name is already in the direct！\n");
			}
			else if (flag == -4) {
				printf("Error：\nCan not as the name of the direct！\n");
			}
			else if (flag == -5) {
				printf("Error：\nThe disk space is full！\n");
			}
			else if (flag == 0) {
				printf("Succssfully make direct！\n");
			}
			writeTerminalHead();
			break;
		case 8:
			scanf("%s", name);
			flag = rmdir(name);
			if (flag == -1) {
				printf("Error：\nThe direct is not exist！\n");
			}
			else if (flag == -2) {
				printf("Error：\nThe direct has son direct, please first remove the direct！\n");
			}
			else if (flag == -3) {
				printf("Error：\nThe remove is not direct！\n");
			}
			else if (flag == 0) {
				printf("Successfully remove direct！\n");
			}
			writeTerminalHead();
			break;
		case 9:
			dir();
			writeTerminalHead();
			break;
		case 10:
			scanf("%s", name);
			flag = cd(name);
			if (flag == -1) {
				printf("Error：\nThe path no correct！\n");
			}
			else if (flag == -2) {
				printf("Error：\nThe opened is not direct！\n");
			}
			writeTerminalHead();
			break;
        case 11:
            helpinfo();
            writeTerminalHead();
			break;
		default:
			printf("Error：\nThe command is wrong！\n");
			writeTerminalHead();
			}
		}
    }
}


int main()
{
	start();
    return 0;
}
