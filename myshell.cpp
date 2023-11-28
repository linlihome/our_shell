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
#define HISTORY_FILE_PATH "/home/student/history.txt"


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




void list_processes() {
    DIR *dir;
    struct dirent *entry;
    dir = opendir("/proc");
    if (dir == NULL) {
        perror("Error opening /proc directory");
        exit(EXIT_FAILURE);
    }
    printf("PID\tTTY\t\tTIME\tCMD\n");
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && atoi(entry->d_name) != 0) {
            char path[1024];
            snprintf(path, sizeof(path), "/proc/%s/stat", entry->d_name);
            FILE *file = fopen(path, "r");
            if (file != NULL) {
                int pid;
                char tty[32], time[32], cmd[1024];
                fscanf(file, "%d %*s %*s %*s %*s %*s %*s %*s %*s %*s %s %s", &pid, tty, time, cmd);
                fclose(file);
                printf("%d\t%s\t%s\t%s\n", pid, tty, time, cmd);
            }
        }
    }
    closedir(dir);
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
        if (strcmp(s[0], "man") == 0)
        {
            continue;
        }
        if (strcmp(s[0], "ls") == 0)
        {
            custom_ls();
            continue;
        }
        if (strcmp(s[0], "cd") == 0)//cd指令直接使用chdir函数改变父进程所在路径
        {
            if(s[1] != NULL)
                if(chdir(s[1]) != 0)
                    printf("Directory not found!\n");
            continue;
        }
        if (strcmp(s[0], "cp") == 0) 
        {
            // Check if enough arguments are provided
            if (s[1] == NULL || s[2] == NULL) 
            {
                fprintf(stderr, "Usage: cp <source> <destination>\n");
                continue;
            }
            struct stat source_stat;
            if (stat(s[1], &source_stat) != 0) 
            {
                perror("Error getting source file/directory information");
                continue;
            }
            if (S_ISDIR(source_stat.st_mode)) 
            {
                copy_directory(s[1], s[2]);
                continue;
            }
            else 
            {
                copy_file(s[1], s[2]);
                continue;
            }
        }
        if (strcmp(s[0], "mv") == 0)
        {
            if (s[1] != NULL && s[2] != NULL) 
                custom_mv(s[1], s[2]);
            else 
                printf("Usage: mv <source> <destination>\n");
            continue;
        }
        if (strcmp(s[0], "rm") == 0)
        {
            if (s[1] == NULL)
                printf("Usage: rm <filename>\n");
            else
            {
                struct stat statbuf;
                stat(s[1], &statbuf);
                if (S_ISDIR(statbuf.st_mode))
                    remove_recursive(s[1]);
                else
                    remove(s[1]);
            }
            continue;
        }
        if (strcmp(s[0], "pwd") == 0)//当前路径
        {
            printf("%s\n", dir);
            continue;
        }
        if (strcmp(s[0], "history") == 0)
        {
            display_history();
            continue;
        }
        if (strcmp(s[0], "ps") == 0)
        {
            //list_processes();
            continue;
        }
        if (strcmp(s[0], "tree") == 0)
        {
            print_tree(dir,0);
            chdir(dir);
            continue;
        }
        pid_t id = fork();
        if (id == 0)
        {
            execvp(*s, s);
            exit(1);
        }
        int status = 0;
        waitpid(id, &status, 0);
    }
}
