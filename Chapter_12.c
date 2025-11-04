/*#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *thread_function(void *arg);  //定义函数原型

char message[] = "Hello World";

int main() {
    int res;
    pthread_t a_thread;
    void *thread_result;
    res = pthread_create(&a_thread, NULL, thread_function, (void *)message); //a_thread来引用这个新线程。
    //如果调用成功就会有两个线程存在，原来的线程执行pthread_create后的代码，新线程执行thread_function函数
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for thread to finish...\n");
    res = pthread_join(a_thread, &thread_result);//原来的线程确定新线程启动之后开始调用pthread_join等价与手机子进程信息的wait函数
    if (res != 0) {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    printf("Thread joined, it returned %s\n", (char *)thread_result);
    printf("Message is now %s\n", message);
    exit(EXIT_SUCCESS);
}

void *thread_function(void *arg) {
    printf("thread_function is running. Argument was %s\n", (char *)arg);
    sleep(3);
    strcpy(message, "Bye!"); //新线程修改了message的信息
    pthread_exit("Thank you for the CPU time");
}*/


/*
//验证线程是可以同时进行的
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *thread_function(void *arg);  //定义函数原型
int run_row =1;
char message[] = "Hello World";

int main() {
    int res;
    pthread_t a_thread;
    void *thread_result;
    int print_count1=0;

    res = pthread_create(&a_thread, NULL, thread_function, (void *)message); //a_thread来引用这个新线程。
    //如果调用成功就会有两个线程存在，原来的线程执行pthread_create后的代码，新线程执行thread_function函数
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    while (print_count1++<20) {
        if (run_row==1) {
            printf("1");
            run_row=2;
        }else {
            sleep(1);
        }
    }
    printf("Waiting for thread to finish...\n");
    res = pthread_join(a_thread, &thread_result);//原来的线程确定新线程启动之后开始调用pthread_join等价与手机子进程信息的wait函数
    if (res != 0) {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    printf("Thread joined\n");
    exit(EXIT_SUCCESS);
}

void *thread_function(void *arg) {
    int print_count2=0;
    while (print_count2++<20) {
        if (run_row==2) {
            printf("2");
            run_row=1;
        }else {
            sleep(1);
        }
    }
    sleep(3);
}*/
/*
 *
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>


void *thread_function(void *arg);
sem_t bin_sem;

#define WORK_SIZE 1024
char work_area[WORK_SIZE];

int main() {
    int res;
    pthread_t a_thread;

    void *thread_result;

    res=sem_init(&bin_sem, 0, 0);
    if(res!=0) {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
    res =pthread_create(&a_thread,NULL,thread_function,NULL);
    if(res!=0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    //从键盘读取一些数据然后放在工作区work_area里面，然后增加信号量
    printf("Input some text.Enter 'end' to finish\n");
    while (strncmp("end",work_area,3)) {
        if (strncmp("FAST",work_area,4) == 0) {  //输入FAST时程序就会调用sem_post开始使得统计字符的线程开始进行，同时立即用其他数据更新work_area数组
            sem_post(&bin_sem);
            strcpy(work_area,"Wheeee....");
        }else {
            fgets(work_area,WORK_SIZE,stdin);
        }
        sem_post(&bin_sem);
    }
    printf("\n Waiting for thread to finish...\n");
    res=pthread_join(a_thread,&thread_result);
    if(res!=0) {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    printf("Thread join successful\n");
    sem_destroy(&bin_sem);
    exit(EXIT_SUCCESS);

}
void *thread_function(void *arg) {
    //等待信号量，然后统计输入的字符数
    sem_wait(&bin_sem);
    while(strncmp("end",work_area,3)!=0) {
        printf("You input %lu characters\n", strlen(work_area) -1);
        sem_wait(&bin_sem);
    }
    pthread_exit(NULL);
}
*/


/*
 *
//线程互斥量
//保护对于变量的访问
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


void *thread_function(void *arg);
pthread_mutex_t work_mutex; /* protects both work_area and time_to_exit #1#

#define WORK_SIZE 1024
char work_area[WORK_SIZE];  //共享缓冲区，可以再主线程和子线程之间传递输入文本
int time_to_exit = 0;//exit signal

int main() {
    int res;
    pthread_t a_thread;
    void *thread_result;
    res = pthread_mutex_init(&work_mutex, NULL);
    if (res != 0) {
        perror("Mutex initialization failed");
        exit(EXIT_FAILURE);
    }
    res = pthread_create(&a_thread, NULL, thread_function, NULL);
    //创建子线程
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    //主线程随后锁住work_mutex
    pthread_mutex_lock(&work_mutex);
    //主
    printf("Input some text. Enter 'end' to finish\n");
//现在持锁
    while(!time_to_exit) {

        fgets(work_area, WORK_SIZE, stdin);
        pthread_mutex_unlock(&work_mutex);//读取结束立即unlock将共享缓冲区交给子线程去处理
        while(1) {
            pthread_mutex_lock(&work_mutex);
            if (work_area[0] != '\0') {//子线程把work_area清空的标志
                pthread_mutex_unlock(&work_mutex);
                sleep(1);
            }
            else {
                break;
            }
        }
    }
    pthread_mutex_unlock(&work_mutex);
    printf("\nWaiting for thread to finish...\n");
    res = pthread_join(a_thread, &thread_result);
    if (res != 0) {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    printf("Thread joined\n");
    pthread_mutex_destroy(&work_mutex);
    exit(EXIT_SUCCESS);
}

//子
void *thread_function(void *arg) {
    sleep(1);
    pthread_mutex_lock(&work_mutex);//刚开始就锁住互斥量，进入循环读取work_area

    while(strncmp("end", work_area, 3) != 0) {
        printf("You input %lu characters\n", strlen(work_area) -1);
        work_area[0] = '\0';
        pthread_mutex_unlock(&work_mutex);
        sleep(1);
        pthread_mutex_lock(&work_mutex);
        while (work_area[0] == '\0' ) {//表示该条消息已经被销毁
            pthread_mutex_unlock(&work_mutex);//给主线程机会去读取下一条
            sleep(1);
            pthread_mutex_lock(&work_mutex);
        }
    }
    time_to_exit = 1;
    work_area[0] = '\0'; //保证主线程不会iu阻塞等待
    pthread_mutex_unlock(&work_mutex);
    pthread_exit(0);//结束线程
}
*/


/*
 *
 *
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *arg);

char message[] ="Hello Wrold";
int thread_finished =0;
int main() {
    int res ;
    pthread_t a_thread;
    void *thread_result;
    //声明一个线程属性并对她进行初始化
    pthread_attr_t thread_attr;

    res = pthread_attr_init(&thread_attr);
    if (res != 0) {
        perror("Attribute creation failed");
        exit(EXIT_FAILURE);
    }
    //把属性设置为脱离属性当线程结束时，系统会自动回收其资源；
    res =pthread_attr_setdetachstate(&thread_attr,PTHREAD_CREATE_DETACHED);
    if (res != 0) {
        perror("Setting deached attribute failed");
        exit(EXIT_FAILURE);
    }
    res =pthread_create(&a_thread,&thread_attr,thread_function,(void *)message);
    //thread_attr线程属性
    //(void *)message传递给线程函数的参数
    if (res!=0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    //销毁线程属性对象，不影响已经创建的线程
    (void)pthread_attr_destroy(&thread_attr);
    while (!thread_finished) {
        printf("Waiting for thread to say it's finished...\n");
        sleep(1);
    }
    printf("Other thread finished,bye!\n");
    exit(EXIT_SUCCESS);
}
void *thread_function(void *arg) {
    printf("thread_Function is running.Argument was %s\n",(char *)arg);
    sleep(4);
    printf("Second thread setting finished flag,and exiting now\n");
    thread_finished = 1;//通知主线程子线程结束，这个在两个线程之间是同步的
    pthread_exit(NULL);
}
*/


/*
//取消一个线程

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void *thread_function(void *arg);

int main() {
    int  res;
    pthread_t a_thread;
    void *thread_result;

    res = pthread_create(&a_thread, NULL, thread_function, NULL);
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    sleep(3);

    printf("Cancle thread...\n");
    res =pthread_cancel(a_thread);
    if (res != 0) {
        perror("Tread cancellation failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for thread to finish..\n");
    res=pthread_cancel(a_thread);
    if (res != 0) {
        perror("Tread cancellation failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for thread to finish...\n");
    res =pthread_join(a_thread, &thread_result);
    if (res != 0) {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

void *thread_function(void *arg) {
    int  i ,res;

    //将取消状态设置为允许取消
    res =pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    if (res != 0) {
        perror("Thread pthread_setcancelstate failed");
        exit(EXIT_FAILURE);
    }
    //取消类型设置为延迟取消
    res=pthread_setcancelstate(PTHREAD_CANCEL_DEFERRED, NULL);
    if (res != 0) {
        perror("Thread pthread_setcancelstate failed");
        exit(EXIT_FAILURE);
    }

    printf("thread_function is running\n");
    //最后线程再循环中被取消
    for (i = 0 ; i < 10 ; i++) {
        printf("thread #%i is running\n", i);
        sleep(1);
    }

    pthread_exit(0);
}
*/


//多个线程
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 6

void *thread_function(void *arg);

int main() {
    int res;
    pthread_t a_thread[NUM_THREADS];
    void *thread_result;
    int lots_of_threads;
    for (lots_of_threads=0;lots_of_threads<NUM_THREADS;lots_of_threads++) {
        res=pthread_create(&a_thread[lots_of_threads],NULL,thread_function,(void *)&lots_of_threads);
        if (res!=0) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }
    printf("Waiting for threads to finish....\n");
    //再原来的线程中等待合并这些子线程，但并不是以创建他们的顺序来合并
    for (lots_of_threads=NUM_THREADS-1;lots_of_threads>=0;lots_of_threads--) {
        res=pthread_join(a_thread[lots_of_threads],&thread_result);
        if (res!=0) {
            perror("Thread join failed");
        }else {
            printf("Thread joined\n");
        }
        printf("All done");
        exit(EXIT_SUCCESS);
    }

}void *thread_function(void *arg) {
    int my_number=*(int *)arg;
    int rand_num;

    printf("thread_function is running,Argument was %d\n",my_number);
    rand_num=1+(int)(90*rand()/RAND_MAX+1.0);
    sleep(rand_num);
    printf("Bye from %d\n",my_number);

    pthread_exit(NULL);
}
