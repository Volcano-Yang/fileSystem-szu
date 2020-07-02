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

#endif