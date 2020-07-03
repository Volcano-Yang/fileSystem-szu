/*该头文件主要定义了文件系统的数据结构，全局变量定义*/

#include"block.h"
#ifndef FILESYSTEM_H_INCLUDED
#define FILESYSTEM_H_INCLUDED

//定义当前环境 １为开发环境 ０为生产环境
#define ENV 0 

struct fatitem {
	//存放文件下一个磁盘的指针
	int item;
	//磁盘块是否空闲的标志位 ０为空闲
	char em_disk;
};


struct openFile {
	//当前打开的文件表
	struct openFileItem {
		//文件名
		char name[9];
		//起始盘块号
		int firstdisk;
		//文件的大小
		int size;
	}opeitem[MOFN];
	//当前打开的文件数目
	int cur_size;
};


struct dirOrFile {
	//文件控制块信息
	struct FCB {
		//文件名
		char name[9];
		//0为普通文件 1为目录
		char property;
		//文件的大小
		int size;
		//起始盘块号
		int firstdisk;
		//子目录起始盘号
		int next;
		//1表示根目录
		int sign;
	}dirOrFileitem[MSD + 2];
};

struct fatitem *fat;
struct dirOrFile *root;
struct dirOrFile *nowDir;
struct openFile openFile;
//当前打开文件的下标
int fd = -1; 
char *dirPath;
char *fdisk;
void initfile();
void format();
void initRoot();
void exitSystem();
void console(char info[]);
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