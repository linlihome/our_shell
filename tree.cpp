#include<stdio.h>                                                                                                                                                                                                   
#include<unistd.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<libgen.h>
#define MAX 1024
#define ARGc 512
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define HISTORY_FILE_PATH "/home/fan/桌面/shell/history.txt"
void sys_tree(char ** s) {
    printf("%s\n",s[0]);
}
