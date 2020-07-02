#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include"filesystem.h"
#include"block.h"

/* 信息打印部分 */
void console(char info[]){
    if(ENV){
        printf("\n开始执行%s\n",info);
    }
}

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


int touch(char *name) {
    console("create");
	if (strlen(name) > 10){
        return -1;  
    }
    else{
        int i, j;
        for (j = 2; j < MSD + 2; j++) {
		if (!strcmp(cur_dir->directitem[j].name, name))	
            {break;}
	    }

	    if ((j-2) < MSD){return -4;}
	    
        for (i = 2; (i-2) < MSD ; i++) {
	    	if (cur_dir->directitem[i].firstdisk == -1)	{break;}
	    }
	    
        if ((i-2) >= MSD){return -2;}
	    
        if (openFile.cur_size >= MOFN){return -3;}
	    
        for (j = ROOT_DISK_NO + 1; j < DISK_NUM; j++) {
	    	if (fat[j].em_disk == '0')	{break;}
	    }
	    
        if (j >= DISK_NUM)	{return -5;}
	    
        strcpy(cur_dir->directitem[i].name, name);
	    fat[j].em_disk = '1';
        cur_dir->directitem[i].size = 0;
        cur_dir->directitem[i].firstdisk = j;
	    cur_dir->directitem[i].property = '0';
        cur_dir->directitem[i].next = j;
	    fd = open(name);
	    return 0;
    }
	
}


int open(char *name) {
    console("open");
	int i, j;
	for (i = 2; i < MSD + 2; i++) {
		if (!strcmp(cur_dir->directitem[i].name, name))	break;
	}
	if (i >= MSD + 2)	return -1;
	if (cur_dir->directitem[i].property == '1') return -4;
	for (j = 0; j < MOFN; j++) {
		if (!strcmp(openFile.opeitem[j].name, name))
			break;
	}
	if (j < MOFN)	return -2;
	if (openFile.cur_size >= MOFN)	return -3;

	for (j = 0; j < MOFN; j++) {
		if (openFile.opeitem[j].firstdisk == -1)
			break;
	}
	openFile.opeitem[j].firstdisk = cur_dir->directitem[i].firstdisk;
	strcpy(openFile.opeitem[j].name, name);
	openFile.opeitem[j].size = cur_dir->directitem[i].size;
	openFile.cur_size++;
	return j;
}


int close(char *name) {
    console("close");
	int i;
	for (i = 0; i < MOFN; i++) {
		if (!strcmp(openFile.opeitem[i].name, name))
			break;
	}
	if (i >= MOFN)	return -1;
	strcpy(openFile.opeitem[i].name, "");
	openFile.opeitem[i].firstdisk = -1;
	openFile.opeitem[i].size = 0;
	openFile.cur_size--;

	return 0;
}


int write(int fd, char *buf, int len) {
    console("write");
	char *first;
	int item, i, j, k;
	int ilen1, ilen2, modlen, temp;
	char Space = 32;
	char Endter = '\n';
	for (i = 0; i < len; i++) {
		if (buf[i] == '$')	buf[i] = Space;
		else if (buf[i] == '#') buf[i] = Endter;
	}
	item = openFile.opeitem[fd].firstdisk;
	for (i = 2; i < MSD + 2; i++) {
		if (cur_dir->directitem[i].firstdisk == item)	break;
	}
	temp = i;
	while (fat[item].item != -1) {
		item = fat[item].item;
	}
	first = fdisk + item * DISKSIZE + openFile.opeitem[fd].size%DISKSIZE;
	if (DISKSIZE - openFile.opeitem[fd].size%DISKSIZE > len) {
		strcpy(first, buf);
		openFile.opeitem[fd].size = openFile.opeitem[fd].size + len;
		cur_dir->directitem[temp].size = cur_dir->directitem[temp].size + len;
	}
	else {
		for (i = 0; i < (DISKSIZE - openFile.opeitem[fd].size%DISKSIZE); i++) {
			first[i] = buf[i];
		}
		ilen1 = len - (DISKSIZE - openFile.opeitem[fd].size%DISKSIZE);
		ilen2 = ilen1 / DISKSIZE;
		modlen = ilen1 % DISKSIZE;
		if (modlen > 0)	ilen2 += 1;
		for (j = 0; j < ilen2; j++) {
			for (i = ROOT_DISK_NO + 1; i < DISK_NUM; i++) {
				if (fat[i].em_disk == '0')	break;
			}
			if (i >= DISK_NUM)	return -1;
			first = fdisk + i * DISKSIZE;
			if (j == ilen2 - 1) {
				for (k = 0; k < len - (DISKSIZE - openFile.opeitem[fd].size%DISKSIZE) - j * DISKSIZE; k++)
					first[k] = buf[k];
			}
			else {
				for (k = 0; k < DISKSIZE; k++)
					first[k] = buf[k];
			}
			fat[item].item = i;
			fat[i].em_disk = '1';
			fat[i].item = -1;
		}
		openFile.opeitem[fd].size = openFile.opeitem[fd].size + len;
		cur_dir->directitem[temp].size = cur_dir->directitem[temp].size + len;
	}
	return 0;
}


int read(int fd, char *buf) {
    console("read");
	int len = openFile.opeitem[fd].size;
	char *first;
	int i, j, item;
	int ilen1, modlen;
	item = openFile.opeitem[fd].firstdisk;
	ilen1 = len / DISKSIZE;
	modlen = len % DISKSIZE;
	if (modlen != 0)	ilen1 += 1;
	first = fdisk + item * DISKSIZE;
	for (i = 0; i < ilen1; i++) {
		if (i == ilen1 - 1) {
			for (j = 0; j < len - i * DISKSIZE; j++)
				buf[i*DISKSIZE + j] = first[j];
		}
		else {
			for (j = 0; j < len - i * DISKSIZE; j++)
				buf[i*DISKSIZE + j] = first[j];
			item = fat[item].item;
			first = fdisk + item * DISKSIZE;
		}
	}
	return 0;
}


int del(char *name) {
    console("del");
	int cur_item, item, temp, i;
	for (i = 2; i < MSD + 2; i++) {
		if (!strcmp(cur_dir->directitem[i].name, name))
			break;
	}
	cur_item = i;
	if (i >= MSD + 2)	return-1;
	if (cur_dir->directitem[cur_item].property != '0')	
		return -3;

	for (i = 0; i < MOFN; i++) {
		if (!strcmp(openFile.opeitem[i].name, name))
			return -2;
	}
	item = cur_dir->directitem[cur_item].firstdisk;
	while (item != -1) {
		temp = fat[item].item;
		fat[item].item = -1;
		fat[item].em_disk = '0';
		item = temp;
	}
	cur_dir->directitem[cur_item].sign = 0;
	cur_dir->directitem[cur_item].firstdisk = -1;
	strcpy(openFile.opeitem[cur_item].name, "");
	cur_dir->directitem[cur_item].next = -1;
	cur_dir->directitem[cur_item].property = '0';
	cur_dir->directitem[cur_item].size = 0;

	return 0;
}



int mkdir(char *name) {
    console("mkdir");
	int i, j;
	struct direct *cur_mkdir;
	if (!strcmp(name, "."))	return -4;
	if (!strcmp(name, ".."))	return -4;
	if (strlen(name) > 8)	return -1;
	for (i = 2; i < MSD + 2; i++) {
		if (cur_dir->directitem[i].firstdisk == -1)
			break;
	}
	if (i >= MSD + 2)	return -2;
	for (j = 2; j < MSD + 2; j++) {
		if (!strcmp(cur_dir->directitem[j].name, name))
			break;
	}
	if (j < MSD + 2)	return -3;
	for (j = ROOT_DISK_NO + 1; j < DISK_NUM; j++) {
		if (fat[j].em_disk == '0')
			break;
	}
	if (j >= DISK_NUM)	return -5;
	fat[j].em_disk = '1';
	strcpy(cur_dir->directitem[i].name, name);
	cur_dir->directitem[i].firstdisk = j;
	cur_dir->directitem[i].size = ROOT_DISK_SIZE;
	cur_dir->directitem[i].next = j;
	cur_dir->directitem[i].property = '1';

	cur_mkdir = (struct direct *)(fdisk + cur_dir->directitem[i].firstdisk*DISKSIZE);

	cur_mkdir->directitem[0].sign = 0;
	cur_mkdir->directitem[0].firstdisk = cur_dir->directitem[i].firstdisk;
	strcpy(cur_mkdir->directitem[0].name, ".");
	cur_mkdir->directitem[0].next = cur_mkdir->directitem[0].firstdisk;
	cur_mkdir->directitem[0].property = '1';
	cur_mkdir->directitem[0].size = ROOT_DISK_SIZE;

	cur_mkdir->directitem[1].sign = cur_dir->directitem[0].sign;
	cur_mkdir->directitem[1].firstdisk = cur_dir->directitem[0].firstdisk;
	strcpy(cur_mkdir->directitem[1].name, "..");
	cur_mkdir->directitem[1].next = cur_mkdir->directitem[1].firstdisk;
	cur_mkdir->directitem[1].property = '1';
	cur_mkdir->directitem[1].size = ROOT_DISK_SIZE;
	for (i = 2; i < MSD + 2; i++) {
		cur_mkdir->directitem[i].sign = 0;
		cur_mkdir->directitem[i].firstdisk = -1;
		strcpy(cur_mkdir->directitem[i].name, "");
		cur_mkdir->directitem[i].next = -1;
		cur_mkdir->directitem[i].property = '0';
		cur_mkdir->directitem[i].size = 0;
	}
	return 0;

}



int rmdir(char *name) {
    console("rmdir");
	int i, j, item;
	struct direct *temp_dir;
	for (i = 2; i < MSD + 2; i++) {
		if (!strcmp(cur_dir->directitem[i].name, name))
			break;
	}
	if (i >= MSD + 2)	return -1;
	if (cur_dir->directitem[i].property != '1')
		return -3;

	temp_dir = (struct direct *)(fdisk + cur_dir->directitem[i].next*DISKSIZE);
	for (j = 2; j < MSD + 2; j++) {
		if (temp_dir->directitem[j].next != -1)
			break;
	}
	if (j < MSD + 2)	return -2;

	item = cur_dir->directitem[i].firstdisk;
	fat[item].em_disk = '0';

	cur_dir->directitem[i].sign = 0;
	cur_dir->directitem[i].firstdisk = -1;
	strcpy(cur_dir->directitem[i].name, name);
	cur_dir->directitem[i].next = -1;
	cur_dir->directitem[i].property = '0';
	cur_dir->directitem[i].size = 0;

	return 0;

}



void dir() {
    console("dir");
	int i;
	for (i = 2; i < MSD + 2; i++) {
		if (cur_dir->directitem[i].firstdisk != -1) {
			printf("%s\t", cur_dir->directitem[i].name);
			if (cur_dir->directitem[i].property == '0')
				printf("%d\t\t\n", cur_dir->directitem[i].size);
			else
				printf("\t<目录>\t\n");
		}
	}
}



int cd(char *name) {
    console("cd");
	int i, j, item;
	char *str;
	char *temp, *point, *point1;
	struct direct *temp_dir;
	temp_dir = cur_dir;
	str = name;
	if (!strcmp("\\", name)) {
		cur_dir = root;
		strcpy(dirPath, "Root:");
		return 0;
	}
	temp = (char *)malloc(DIR_MAXSIZE * sizeof(char));

	for (i = 0; i < (int)strlen(str); i++)
		temp[i] = str[i];
	temp[i] = '\0';
	for (j = 0; j < MSD + 2; j++) {
		if (!strcmp(temp_dir->directitem[j].name, temp))
			break;
	}
	free(temp);

	if (j >= MSD + 2)	return -1;
	item = temp_dir->directitem[j].firstdisk;
	temp_dir = (struct direct *)(fdisk + item * DISKSIZE);
	if (!strcmp("..", name)) {
		if (cur_dir->directitem[j - 1].sign != -1) {
			point = strchr(dirPath, '\\');
			while (point != NULL) {
				point1 = point + 1;
				point = strchr(point1, '\\');
			}
			*(point1 - 1) = '\0';
		}
	}
	else {
		//if(name[0]!='\\')
			dirPath = strcat(dirPath, "\\");
		dirPath = strcat(dirPath, name);
	}
	cur_dir = temp_dir;
	return 0;
}

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
