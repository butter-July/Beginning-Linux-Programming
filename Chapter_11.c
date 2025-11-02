/*
 *fork
 *
 *
 *#include <stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int main(void) {
    pid_t pid;
    char *message;
    int n;

    printf("fork program starting\n");
    pid = fork();
    switch (pid) {
        case -1:
            perror("fork failed");
            exit(1);
        case 0:
            message = "This is the child";
            n=5;
            break;
        default:
            message = "This is the parent";
            n=3;
            break;
    }
    for (;n>0;n--) {
        puts(message);
        sleep(1);
    }
    exit(0);
}*/
/*
 *增加父进程等待并且检查子进程的退出状态

#include<sys/types.h>
#include<sys/wait.h> ///--wait
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    char *message;
    int n;
    int exit_code;

    printf("fork program starting\n");
    pid = fork();
    switch (pid) {
        case -1:
            exit(1);
        case 0:
            message = "This is the child";
            n=5;
            exit_code = 37;
            break;
        default:
            message = "This is the parent";
            n=3;
            exit_code = 0;
            break;
    }

    for(; n > 0; n--) {
        puts(message);
        sleep(1);
    }

    //程序这一部分等待子进程完成

    if (pid!=0) {
        int stat_val;
        pid_t child_pid;

        child_pid = wait(&stat_val);

        printf("Child has finised :PID = %d\n",child_pid);

        if (WIFEXITED(stat_val)) {
            printf("Child exited with code %d\n",WEXITSTATUS(stat_val));
        }else {
            printf("Child terminated abnormally\n");
        }
        exit(exit_code);
    }
}
*/


/*
//fork2.c 僵尸进程
#include <stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int main(void) {
    pid_t pid;
    char *message;
    int n;

    printf("fork program starting\n");
    pid = fork();
    switch (pid) {
        case -1:
            perror("fork failed");
            exit(1);
        case 0:
            message = "This is the child";
            n=3;
            break;
        default:
            message = "This is the parent";
            n=5;//交换父子进程的输出消息的次数
            break;
    }
    for (;n>0;n--) {
        puts(message);
        sleep(1);
    }
    exit(0);
}
*/
/*
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>

int main() {
    int ch;
    while((ch=getchar())!=EOF) {
        putchar(toupper(ch));
    }
    exit(0);
}
*/


/*
 *
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

int main(int argc, char *argv[]) {
    char *filename;

    if(argc !=2) {
        fprintf(stderr,"Usage :useupper file\n");
        exit(1);
    }

    filename = argv[1];

    if (!freopen(filename,"r",stdin)) {
        fprintf(stderr,"Can't open file %s\n",filename);
        exit(2);
    }
    execlp("upper","upper",0);
    perror("Could not exec ./upper");
    exit(3);
}
*/


/*
 *信号
 *signal
#include<signal.h>
#include<stdio.h>
#include<unistd.h>

void ouch(int sig) {
    printf("OUCH! -I got signal %d\n",sig);
    (void) signal(SIGINT,SIG_DFL);  //ctrl c 按下来产生这个信号SIGINT

}
int main() {
    (void) signal(SIGINT,ouch);

    while(1) {
        printf("Hello World!\n");
        sleep(1);
    }
}*/


/*
 *
//模拟闹钟--发送信号 alarm函数

#include<sys/types.h>
#include<signal.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

static int alarm_fired =0;
void ding() {
    alarm_fired = 1;
}

//告诉子进程，等待5s后发送一个SIGALRM信号给他的父进程

int main()
{
    pid_t pid;

    printf("alarm application starting\n");

    pid = fork();
    switch(pid) {
        case -1:
            /* Failure #1#
            perror("fork failed");
            exit(1);
        case 0:
            /* child #1#
            sleep(5);
            kill(getppid(), SIGALRM);
            exit(0);
    }


    printf("waiting for alarm to go off\n");
    (void) signal(SIGALRM, ding);

    pause();   //把程序挂起，知道有一个信号出现为止
    if (alarm_fired)
        printf("Ding!\n");

    printf("done\n");
    exit(0);
}
*/
#include<signal.h>
#include<stdio.h>
#include<unistd.h>

void ouch(int sig) {

    printf("OUCH! -I got signal %d\n",sig);

}
int main() {
    struct sigaction act;
    act.sa_handler = ouch;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);

    while(1) {
        printf("Hello World!\n");
        sleep(1);
    }
}
