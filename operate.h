/*该头文件主要是定义各种指令的操作函数 */
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include"filesystem.h"
#include"block.h"

#ifndef OPERATE_H_INCLUDED
#define OPERATE_H_INCLUDED

void console(char info[]){
    if(ENV){
        printf("\n开始执行%s\n",info);
    }
}

// todo: 函数逻辑还需优化
int touch(char *name) {
    console("touch");
	if (strlen(name) > 10){
        return -1;  
    }
    else{
        int i, j;
        for (j = 2; j < MSD + 2; j++) {
		if (!strcmp(nowDir->dirOrFileitem[j].name, name))	
            {break;}
	    }

	    if ((j-2) < MSD){return -4;}
	    
        for (i = 2; (i-2) < MSD ; i++) {
	    	if (nowDir->dirOrFileitem[i].firstdisk == -1)	{break;}
	    }
	    
        if ((i-2) >= MSD){return -2;}
	    
        if (openFile.cur_size >= MOFN){return -3;}
	    
        for (j = ROOT_DISK_NO + 1; j < DISK_NUM; j++) {
	    	if (fat[j].em_disk == '0')	{break;}
	    }
	    
        if (j >= DISK_NUM)	{return -5;}
	    
        strcpy(nowDir->dirOrFileitem[i].name, name);
	    fat[j].em_disk = '1';
        nowDir->dirOrFileitem[i].size = 0;
        nowDir->dirOrFileitem[i].firstdisk = j;
	    nowDir->dirOrFileitem[i].property = '0';
        nowDir->dirOrFileitem[i].next = j;
	    // fd = open(name);
	    return 0;
    }
	
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


int open(char *name) {
    console("open");
	int i, j;
	for (i = 2; i < MSD + 2; i++) {
		if (!strcmp(nowDir->dirOrFileitem[i].name, name))	break;
	}
	if (i >= MSD + 2)	return -1;
	if (nowDir->dirOrFileitem[i].property == '1') return -4;
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
	openFile.opeitem[j].firstdisk = nowDir->dirOrFileitem[i].firstdisk;
	strcpy(openFile.opeitem[j].name, name);
	openFile.opeitem[j].size = nowDir->dirOrFileitem[i].size;
	openFile.cur_size++;
	return j;
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
		if (nowDir->dirOrFileitem[i].firstdisk == item)	break;
	}
	temp = i;
	while (fat[item].item != -1) {
		item = fat[item].item;
	}
	first = fdisk + item * DISKSIZE + openFile.opeitem[fd].size%DISKSIZE;
	if (DISKSIZE - openFile.opeitem[fd].size%DISKSIZE > len) {
		strcpy(first, buf);
		openFile.opeitem[fd].size = openFile.opeitem[fd].size + len;
		nowDir->dirOrFileitem[temp].size = nowDir->dirOrFileitem[temp].size + len;
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
		nowDir->dirOrFileitem[temp].size = nowDir->dirOrFileitem[temp].size + len;
	}
	return 0;
}


// 删除文件或者目录
int delete(char *name) {
    console("delete");
	int cur_item, item, temp, i;
	for (i = 2; i < MSD + 2; i++) {
		if (!strcmp(nowDir->dirOrFileitem[i].name, name))
			break;
	}
	cur_item = i;
	if (i >= MSD + 2)	return-1;
	if (nowDir->dirOrFileitem[cur_item].property != '0')	
		return -3;

	for (i = 0; i < MOFN; i++) {
		if (!strcmp(openFile.opeitem[i].name, name))
			return -2;
	}
	item = nowDir->dirOrFileitem[cur_item].firstdisk;
	while (item != -1) {
		temp = fat[item].item;
		fat[item].item = -1;
		fat[item].em_disk = '0';
		item = temp;
	}
	nowDir->dirOrFileitem[cur_item].sign = 0;
	nowDir->dirOrFileitem[cur_item].firstdisk = -1;
	strcpy(openFile.opeitem[cur_item].name, "");
	nowDir->dirOrFileitem[cur_item].next = -1;
	nowDir->dirOrFileitem[cur_item].property = '0';
	nowDir->dirOrFileitem[cur_item].size = 0;

	return 0;
}


int rn(char *oldName, char *newName) {
    console("rn");
	if (strlen(newName) > 10){
        return -1;  
    }
    else{
		int j;
        for (j = 2; (j-2) < MSD ; j++) {
			if (!strcmp(nowDir->dirOrFileitem[j].name, oldName))	
        	break;
	    }
        strcpy(nowDir->dirOrFileitem[j].name, newName);
	    return 0;
    }
}

int rmdir(char *name) {
    console("rmdir");
	int i, j, item;
	struct dirOrFile *temp_dir;
	for (i = 2; i < MSD + 2; i++) {
		if (!strcmp(nowDir->dirOrFileitem[i].name, name))
			break;
	}
	if (i >= MSD + 2)	return -1;
	if (nowDir->dirOrFileitem[i].property != '1')
		return -3;

	temp_dir = (struct dirOrFile *)(fdisk + nowDir->dirOrFileitem[i].next*DISKSIZE);
	for (j = 2; j < MSD + 2; j++) {
		if (temp_dir->dirOrFileitem[j].next != -1)
			break;
	}
	if (j < MSD + 2)	return -2;

	item = nowDir->dirOrFileitem[i].firstdisk;
	fat[item].em_disk = '0';

	nowDir->dirOrFileitem[i].sign = 0;
	nowDir->dirOrFileitem[i].firstdisk = -1;
	strcpy(nowDir->dirOrFileitem[i].name, name);
	nowDir->dirOrFileitem[i].next = -1;
	nowDir->dirOrFileitem[i].property = '0';
	nowDir->dirOrFileitem[i].size = 0;

	return 0;

}

int mkdir(char *name) {
    console("mkdir");
	int i, j;
	struct dirOrFile *cur_mkdir;
	if (!strcmp(name, ".")||!strcmp(name, "..")) return -4;
	if (strlen(name) > 10)	return -1;
	for (i = 2; i < MSD + 2; i++) {
		if (nowDir->dirOrFileitem[i].firstdisk == -1)
			break;
	}
	if (i >= MSD + 2)	return -2;
	for (j = 2; j < MSD + 2; j++) {
		if (!strcmp(nowDir->dirOrFileitem[j].name, name))
			break;
	}
	if (j < MSD + 2)	return -3;
	for (j = ROOT_DISK_NO + 1; j < DISK_NUM; j++) {
		if (fat[j].em_disk == '0')
			break;
	}
	if (j >= DISK_NUM)	return -5;
	fat[j].em_disk = '1';
	strcpy(nowDir->dirOrFileitem[i].name, name);
	nowDir->dirOrFileitem[i].firstdisk = j;
	nowDir->dirOrFileitem[i].size = ROOT_DISK_SIZE;
	nowDir->dirOrFileitem[i].next = j;
	nowDir->dirOrFileitem[i].property = '1';

	cur_mkdir = (struct dirOrFile *)(fdisk + nowDir->dirOrFileitem[i].firstdisk*DISKSIZE);

	cur_mkdir->dirOrFileitem[0].sign = 0;
	cur_mkdir->dirOrFileitem[0].firstdisk = nowDir->dirOrFileitem[i].firstdisk;
	strcpy(cur_mkdir->dirOrFileitem[0].name, ".");
	cur_mkdir->dirOrFileitem[0].next = cur_mkdir->dirOrFileitem[0].firstdisk;
	cur_mkdir->dirOrFileitem[0].property = '1';
	cur_mkdir->dirOrFileitem[0].size = ROOT_DISK_SIZE;

	cur_mkdir->dirOrFileitem[1].sign = nowDir->dirOrFileitem[0].sign;
	cur_mkdir->dirOrFileitem[1].firstdisk = nowDir->dirOrFileitem[0].firstdisk;
	strcpy(cur_mkdir->dirOrFileitem[1].name, "..");
	cur_mkdir->dirOrFileitem[1].next = cur_mkdir->dirOrFileitem[1].firstdisk;
	cur_mkdir->dirOrFileitem[1].property = '1';
	cur_mkdir->dirOrFileitem[1].size = ROOT_DISK_SIZE;
	for (i = 2; i < MSD + 2; i++) {
		cur_mkdir->dirOrFileitem[i].sign = 0;
		cur_mkdir->dirOrFileitem[i].firstdisk = -1;
		strcpy(cur_mkdir->dirOrFileitem[i].name, "");
		cur_mkdir->dirOrFileitem[i].next = -1;
		cur_mkdir->dirOrFileitem[i].property = '0';
		cur_mkdir->dirOrFileitem[i].size = 0;
	}
	return 0;

}



void ls() {
    console("ls");
	int i;
	for (i = 2; i < MSD + 2; i++) {
		if (nowDir->dirOrFileitem[i].firstdisk != -1) {
			printf("%s\t", nowDir->dirOrFileitem[i].name);
			if (nowDir->dirOrFileitem[i].property == '0')
				printf("%d\t\t\n", nowDir->dirOrFileitem[i].size);
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
	struct dirOrFile *temp_dir;
	temp_dir = nowDir;
	str = name;
	if (!strcmp("\\", name)) {
		nowDir = root;
		strcpy(dirPath, "Root:");
		return 0;
	}
	temp = (char *)malloc(DIR_MAXSIZE * sizeof(char));

	for (i = 0; i < (int)strlen(str); i++)
		temp[i] = str[i];
	temp[i] = '\0';
	for (j = 0; j < MSD + 2; j++) {
		if (!strcmp(temp_dir->dirOrFileitem[j].name, temp))
			break;
	}
	free(temp);

	if (j >= MSD + 2)	return -1;
	item = temp_dir->dirOrFileitem[j].firstdisk;
	temp_dir = (struct dirOrFile *)(fdisk + item * DISKSIZE);
	if (!strcmp("..", name)) {
		if (nowDir->dirOrFileitem[j - 1].sign != -1) {
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
	nowDir = temp_dir;
	return 0;
}

#endif