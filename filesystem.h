#include"block.h"
#ifndef FILESYSTEM_H_INCLUDED
#define FILESYSTEM_H_INCLUDED

#define ENV 1 //定义当前环境　１为开发环境　０为生产环境

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
struct opentable openFile;
int fd = -1;
char *dirPath;
char *fdisk;
void initfile();
void format();
void judge();
void exitSystem();
int touch(char *name);
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
void doMain();
int main();

#endif