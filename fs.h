#include <time.h>
#ifndef FS_H_INCLUDED
#define FS_H_INCLUDED


// 以下是
// 关于磁盘的定义

//系统大小100m
#define MEM_D_SIZE 512*1024*1024
//盘块大小，按kb划分
#define DISKSIZE 1024
#define DISK_NUM 1024
#define FATSIZE DISK_NUM*sizeof(struct fatitem)
#define ROOT_DISK_NO FATSIZE/(DISKSIZE+1)
#define ROOT_DISK_SIZE sizeof(struct direct)
#define DIR_MAXSIZE 1024
#define MSD 10
#define MOFN 5
#define MAX_WRITE 1024*128

struct fatitem {
	int item;
	char em_disk;
};
struct direct {
	struct FCB {
		char name[9];
		char property;
		int size;
		int firstdisk;
		int next;
		int sign;
	}directitem[MSD + 2];
};
struct opentable {
	struct openttableitem {
		char name[9];
		int firstdisk;
		int size;
	}opeitem[MOFN];
	int cur_size;
};
struct fatitem *fat;
struct direct *root;
struct direct *cur_dir;
struct opentable u_opentable;
int fd = -1;
char *bufferdir;
char *fdisk;
void initfile();
void format();
void enter();
void halts();
int create(char *name);
int open(char *name);
int close(char *name);
int write(int fd, char *buf, int len);
int read(int fd, char *buf);
int del(char *name);
int mkdir(char *name);
int rmdir(char *name);
void dir();
int cd(char *name);
void print();
void show();
int main();

#endif