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
#include<pwd.h>
#define MAX 1024
#define ARGc 512
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define HISTORY_FILE_PATH "/home/fan/桌面/shell/history.txt"

void sys_help();
void sys_ls(char ** s);
void sys_cd(char ** s);
void sys_cp(char ** s);
void sys_mv(char ** s);
void sys_rm(char ** s);
void sys_history(char ** s);
void sys_ps(char ** s);
void sys_redirect(char ** s);
void sys_pipe(char ** s);
void sys_tree(char ** s);
void sys_pwd(char ** s);
void sys_background(char ** s);

void save_history(const char* command)
{
    FILE* history_file = fopen(HISTORY_FILE_PATH, "ab");
    if (history_file == NULL)
    {
        perror("Error opening history file");
        exit(EXIT_FAILURE);
    }
    fprintf(history_file, "%s\n", command);
    fclose(history_file);
}
void shift_commend(char** s, char* c, int n)
{
    assert(s);
    assert(c);
    int num = 0;
    *s = c;
    ++s;
    while (num < n)
    {
        if (*(c + num) == ' ')//将每个空格字符替换为'\0',再将选项的首地址存入s
        {
            *(c + num) = '\0';
            if (*(c + num + 1) != ' ')//防止用户输入多个空字符
            {
                *s = c + 1 + num;
                ++s;
            }
        }
        ++num;
    }
}
int main()
{
    char user_env[32][256];//存储自定义环境变量
    int env_num = 0;
    FILE* history_file = fopen(HISTORY_FILE_PATH, "wb");
    fclose(history_file);
    while (1)
    {
    	char dir[500], hostname[256];
    	getcwd(dir, 500);
    	gethostname(hostname, 256);
        printf(ANSI_COLOR_GREEN"[%s@%s:"ANSI_COLOR_BLUE"%s"ANSI_COLOR_GREEN"]$ "ANSI_COLOR_RESET, getenv("USER"), hostname, dir);
        fflush(stdout);
        char comment[MAX] = { 0 };
        char* p = fgets(comment, sizeof(comment), stdin);
        assert(p);//assert函数在release版本下会被编译器删去    
        (void)p;//防止在release版本下assert函数被删除导致p指针从未被使用而报错    
        comment[strlen(comment) - 1] = '\0';//去除fgets函数最后获取的\n字符    
        char* s[ARGc] = { NULL };
        shift_commend(s, comment, strlen(comment));//分割输入的命令行字符串,将每一个选项的地址存入指针数组s
        save_history(comment);
        if (strcmp(s[1], "<") == 0 || strcmp(s[1], ">") == 0)
        {
            sys_redirect(s);
            continue;
        }
        if (strcmp(s[1], "|") == 0)
        {
            sys_pipe(s);
            continue;
        }
        if (strcmp(s[0], "&") == 0)
        {
            sys_background(s);
            continue;
        }
        if (strcmp(s[0], "help") == 0)
        {
            sys_help();
            continue;
        }
        if (strcmp(s[0], "ls") == 0)
        {
            sys_ls(s);
            continue;
        }
        if (strcmp(s[0], "cd") == 0)
        {
            sys_cd(s);
            continue;
        }
        if (strcmp(s[0], "cp") == 0) 
        {
            sys_cp(s);
            continue;
        }
        if (strcmp(s[0], "mv") == 0)
        {
            sys_mv(s);
            continue;
        }
        if (strcmp(s[0], "rm") == 0)
        {
            sys_rm(s);
            continue;
        }
        if (strcmp(s[0], "pwd") == 0)
        {
            sys_pwd(s);
            continue;
        }
        if (strcmp(s[0], "history") == 0)
        {
            sys_history(s);
            continue;
        }
        if (strcmp(s[0], "ps") == 0)
        {
            sys_ps(s);
            continue;
        }
        if (strcmp(s[0], "tree") == 0)
        {
            sys_tree(s);
            continue;
        }
    }
}
