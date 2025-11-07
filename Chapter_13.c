/*
 *读取外部程序的输出
 *
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    FILE *read_fp;//用来保存popen()返回的文件流指针
    char buffer[BUFSIZ + 1];  //分配缓冲区来读取命令输出
    int chars_read;  //保存fread返回的读取到的字节数
    memset(buffer, '\0', sizeof(buffer)); //清零
    read_fp = popen("uname -a", "r");  //打开到一个子进程管道
    if (read_fp != NULL) {
        chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
        if (chars_read > 0) {
            printf("Output was:-\n%s\n", buffer);
        }
        pclose(read_fp);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}
*/
/*
 *
//将输出送到外部程序
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    FILE *write_fp;
    char buffer[BUFSIZ + 1];

    sprintf(buffer, "Once upon a time, there was...\n");
    write_fp = popen("od -c", "w");  //启动od -c 最后把处理结果打印到自己的标准输出上
    if (write_fp != NULL) {
        fwrite(buffer, sizeof(char), strlen(buffer), write_fp);
        pclose(write_fp);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}
*/

/*
 *
//通过管道大量读取数据
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
    FILE *read_fp;
    char buffer[BUFSIZ+1];
    int chars_read;

    memset(buffer, '\0', sizeof(buffer));
    read_fp=popen("ps ax","r");
    if(read_fp!=NULL) {
        chars_read=fread(buffer,sizeof(char),BUFSIZ/10,read_fp);
        while (chars_read>0) {
            buffer[chars_read-1]='\0';
            printf("Reading %d:-\n%s\n",BUFSIZ,buffer);
            //这次连续的进行读取数据，没有数据就要停止了
            chars_read=fread(buffer,sizeof(char),BUFSIZ/10,read_fp);
        }
        pclose(read_fp);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}
*/
/*
 *popen启动shell
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    FILE *read_fp;
    char buffer[BUFSIZ + 1];
    int chars_read;

    memset(buffer, '\0', sizeof(buffer));
    read_fp = popen("cat popen*.c | wc -l", "r"); //启动一个shell子进程，执行把匹配popen.c的文件内容cat出来，然后wc -l统计
    if (read_fp != NULL) {
        chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
        while (chars_read > 0) {
            buffer[chars_read - 1] = '\0';
            //从子进程的标准输出读取数据到buffer，循环读取并每次打印以读内容
            printf("Reading:-\n %s\n", buffer);
            chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
        }
        pclose(read_fp);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}
*/

/*
 *
//pipe在一个进程通过一个匿名管道在“写端”和“读端”之间传递数据
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
    int data_processed;
    int file_pipes[2];  //用来保存pipe创建的两个文件描述符 【0】读【1】写
    const char some_data[]="123"; //写入管道的数据
    char buffer[BUFSIZ+1];  //接收读出来的数据

    memset(buffer, '\0', sizeof(buffer));
//创建管道
    if (pipe(file_pipes) == 0) {
        data_processed=write(file_pipes[1],some_data,strlen(some_data)); //写进去
        printf("Wrote %d bytes\n",data_processed);
        data_processed=read(file_pipes[0],buffer,BUFSIZ);//读出来
        printf("Read %d bytes :%s\n",data_processed,buffer);//打印出来
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}
*/


/*
 *跨越fork调用的管道，从而演示父子进程之间通过管道通信的完整过程
 *
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    int data_processed;
    int file_pipes[2];
    const char some_data[] = "123";
    char buffer[BUFSIZ + 1];
    pid_t fork_result;

    memset(buffer, '\0', sizeof(buffer));

    if (pipe(file_pipes) == 0) {
        fork_result = fork(); //她会复制父进程，产生一个新的子进程
        if (fork_result == -1) {
            fprintf(stderr, "Fork failure");
            exit(EXIT_FAILURE);
        }
//这个来区分父进程和子进程
        if (fork_result == 0) {
            // 子进程
            data_processed = read(file_pipes[0], buffer, BUFSIZ);
            printf("Read %d bytes: %s\n", data_processed, buffer);
            exit(EXIT_SUCCESS);
        }
        else {
            // 父进程
            data_processed = write(file_pipes[1], some_data, strlen(some_data));
            printf("Wrote %d bytes\n", data_processed);
        }
    }
    exit(EXIT_SUCCESS);
}
*/
/*
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    int data_processed;
    int file_pipes[2];
    const char some_data[] = "123";
    char buffer[BUFSIZ + 1];
    pid_t fork_result;

    memset(buffer, '\0', sizeof(buffer));

    if (pipe(file_pipes) == 0) {
        fork_result = fork();
        if (fork_result == (pid_t)-1) {
            fprintf(stderr, "Fork failure");
            exit(EXIT_FAILURE);
        }

        if (fork_result == 0) {
            sprintf(buffer, "%d", file_pipes[0]);
            (void)execl("pipe4", "pipe4", buffer, (char *)0);
            exit(EXIT_FAILURE);
        }
        else {
            data_processed = write(file_pipes[1], some_data,
                                   strlen(some_data));
            printf("%d - wrote %d bytes\n", getpid(), data_processed);
        }
    }
    exit(EXIT_SUCCESS);
}
*/


/*
//管道和dup函数

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {at
    int data_processed ;
    int file_pipes[2];
    const char some_data[]="123";
    pid_t fork_result ;

    if (pipe(file_pipes) == 0) {
        fork_result = fork();
        if (fork_result==(pid_t)-1) {
            fprintf(stderr, "fork failed\n");
            exit(EXIT_FAILURE);
        }

        //子进程。把管道读端重定向到标准输出并exec od -c
        if (fork_result == (pid_t)0) {
            close(0);  //关闭当前进程的标准输入文件描述符，这样dup会返回iu最小可用fd，通常是0
            dup(file_pipes[0]); //复制file_pipes[0]为一个新的fd.管道的读端被重定向为标准输入
            close(file_pipes[0]);//关闭原来的读端
            close(file_pipes[1]); //关闭写端点

            execlp("od","od","-c",(char *)0); //用od -c替换子进程映像
            exit(EXIT_FAILURE);
        }else {
            close(file_pipes[0]);   //父进程关闭读端，可以避免父进程保持读端打开阻止子进程看到EOF
            data_processed = write(file_pipes[1], some_data, strlen(some_data));
//写入管道
            close(file_pipes[1]);
//写完关闭写端
            printf("%d - wrote %d bytes\n",(int)getpid(),data_processed);
        }
    }
    exit(EXIT_SUCCESS);
}
*/


/*
//创建命名管道
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    int res =mkfifo("/tmp/my_fifo", 0777);
    if (res == 0) {
        printf("fifo created\n");
        exit(EXIT_SUCCESS);
    }
}
*/

//打开FIFO文件

/*
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/my_fifo"

int main(int argc, char *argv[])
{
    int res;
    int open_mode = 0;
    int i;
//检查命令行参数，至少要有一个
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <some combination of\
               O_RDONLY O_WRONLY O_NONBLOCK>\n", *argv);
        exit(EXIT_FAILURE);
    }

 //解析参数并生成open_mode
    for(i = 1; i < argc; i++) {
        if (strncmp(*++argv, "O_RDONLY", 8) == 0)
            open_mode |= O_RDONLY;
        if (strncmp(*argv, "O_WRONLY", 8) == 0)
            open_mode |= O_WRONLY;
        if (strncmp(*argv, "O_NONBLOCK", 10) == 0)
            open_mode |= O_NONBLOCK;
    }
//创建FIFO 如果不存在
    if (access(FIFO_NAME, F_OK) == -1) {
        res = mkfifo(FIFO_NAME, 0777);
        if (res != 0) {
            fprintf(stderr, "Could not create fifo %s\n", FIFO_NAME);
            exit(EXIT_FAILURE);
        }
    }

    //打开FIFO
    printf("Process %d opening FIFO\n", getpid());
    res = open(FIFO_NAME, open_mode);
    printf("Process %d result %d\n", getpid(), res);
    sleep(5);
    if (res != -1) (void)close(res);
    printf("Process %d finished\n", getpid());
    exit(EXIT_SUCCESS);
}
*/


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/my_fifo"
#define BUFFER_SIZE PIPE_BUF

int main()
{
    int pipe_fd;
    int res;
    int open_mode = O_RDONLY;
    char buffer[BUFFER_SIZE + 1];
    int bytes_read = 0;

    memset(buffer, '\0', sizeof(buffer));

    printf("Process %d opening FIFO O_RDONLY\n", getpid());
    pipe_fd = open(FIFO_NAME, open_mode);
    printf("Process %d result %d\n", getpid(), pipe_fd);

    if (pipe_fd != -1) {
        do {
            res = read(pipe_fd, buffer, BUFFER_SIZE);
            bytes_read += res;
        } while (res > 0);
        (void)close(pipe_fd);
    }
    else {
        exit(EXIT_FAILURE);
    }

    printf("Process %d finished, %d bytes read\n", getpid(), bytes_read);
    exit(EXIT_SUCCESS);
}
