/*
 *环境变量
 *
#include <stdio.h>
#include <stdlib.h>
extern char **environ;
int main(void) {
    char **env = environ;

    while (*env) {
        printf("%s\n", *env);  //这里会打印出来.........一大堆
        //environ 环境变量 程序的环境由“名字=值”的字符串组成
        env++;
    }
    exit(0);
}
*/



/*
 *时间和日期
 *
 *#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    /* time
     *int i;
    time_t the_time;
    for (i = 0; i <= 10; i++) {
        the_time = time((time_t *) 0);
        printf("I am at %ld\n", the_time);
        sleep(2); //time 函数，返回底层时间，
    }
    exit(0);
    #1#
    /*  //gmtime and localtime tm结构体
    struct tm *tm_ptr;
    time_t the_time;
    (void) time(&the_time);
   // tm_ptr = gmtime(&the_time);// //111.The gmtime() function in C takes a pointer to type t_time value which represents type in seconds and converts it to struct tm.    --the_time − This is the pointer to a time_t value representing a calendar time.
    tm_ptr = localtime(&the_time); //2222.
    printf("Raw time is %ld\n",the_time);
    printf("gmtime gives:\n");
    printf("date: %02d/%02d/%02d\n",tm_ptr->tm_year,tm_ptr->tm_mon+1,tm_ptr->tm_mday);
    printf("time is %02d:%02d:%02d\n",tm_ptr->tm_hour,tm_ptr->tm_min,tm_ptr->tm_sec);  //但是现在是按照GMT返回时间所以相同，看当地时间可以用localtime函数，把gmtim直接换成localtime，这样就是当地时间啦
    exit(0);
    #1#

    /*
     *ctime
    time_t timeval;

    (void) time(&timeval);
    printf("The date is :%s\n",ctime(&timeval));  //以原来的时间为参数返回更容易读的本地时间 == asctime(localtime(timeval))
    exit(0);
    #1#



     /*
     // strftime //控制格式，像是一个针对日期和时间的Sprintf函数
    struct tm *tm_ptr ,timestruct;
    time_t the_time;
    char buf[256];
    char *result;

    (void) time(&the_time);
    tm_ptr=localtime(&the_time);
    strftime(buf,256,"%A %d %B,%I :%M %p",tm_ptr);

    printf("strftime gives :%s \n",buf);

    strcpy(buf ,"The 18 Sep will do fine");

    printf("Calling strftime with : %s\n",buf);
    tm_ptr=&timestruct;

    result =strptime(buf,"%a %d %b %Y，%R",tm_ptr); //他的作用是读取日期，以一个代表日期和时间的字符串为参数，并创建表示同一个日期和时间的tm结构
    printf("strptime consumed up to %s\n",result);

    printf("strptime gives:\n");
    printf("date: %02d/%02d/%02d\n",tm_ptr->tm_year % 100,tm_ptr->tm_mon+1,tm_ptr->tm_mday);
    printf("time is %02d:%02d:\n",tm_ptr->tm_hour,tm_ptr->tm_min);
    exit(0);

#1#
}*/


/*
 *临时文件
 *
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char tmpname[L_tmpnam];
    char *filename;
    FILE *tmpfp;

    filename=tmpnam(tmpname);  //tmpnam generates and returns a valid temporary filename which does not exist.

    printf("Temporary file name is :%s\n",filename);
    tmpfp=tmpfile();
    if (tmpfp) {
        printf("Opened a temporary file OK\n");
    }else
        perror("tmpfile() failed");

    exit(0);  //the use of `tmpnam' is dangerous, better use `mkstemp'
}
*/

/*
 *用户信息
 *
#include <sys/types.h>
#include <pwd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    uid_t uid;  //owner
    gid_t gid; //group

    struct passwd *pw;
    uid=geteuid();
    gid = geteuid();
    printf("User is %s\n",getlogin());

    printf("user IDs :uid=%d,gid=%d",uid,gid);
    pw = getpwuid(uid);

    printf("UID passwd entry :\n name =%s ,uid =%d,gid=%d,home=%s,shell=%s\n",pw->pw_name,pw->pw_uid,pw->pw_gid,pw->pw_dir,pw->pw_shell);

    pw=getpwnam("root");

    printf("root passwd entry:\n");
    printf("name =%s,uid=%d,gid=%d,home=%s,shell=%s\n",pw->pw_name,pw->pw_uid,pw->pw_gid,pw->pw_dir,pw->pw_shell);
    exit(0);
}
*/


/*
 *主机信息
 *
#include <sys/utsname.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char comouter[256];
    struct  utsname uts; //Structure describing the system and machine.  ----utsname

    if (gethostname(comouter,255)!=0 || uname(&uts)!=0) {
        fprintf(stderr, "Couldn't get host information\n");
        exit(1);
    }

    printf("Computer host name is %s \n",comouter);
    printf("Systen is %s on %s hardware\n",uts.sysname,uts.machine);
    printf("Nodename is %s \n",uts.nodename);
    printf("Version is %s \n",uts.version);
    printf("hostid:%ld",gethostid());  //每台主机的唯一标识符
    exit(0);
}
*/

/*
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
void work() {
    FILE *f;
    int i;
    double x=4.5;
    f= tmpfile();
    for (i=0; i<10000; i++) {
        fprintf(f,"Do some output");
        if (ferror(f)) {
            fprintf(stderr, "Error writing to file\n");
            exit(1);
        }
    }
    for (i=0; i<10000; i++) {
        x=log(x*x+3.21);
    }
}

int main() {
    struct rusage r_usage;
    struct rlimit r_limit;
    int priority;

    work();
    getrusage(RUSAGE_SELF, &r_usage);

    printf("CPU usage: User:%ld.%06ld,System =%ld.%06ld\n",r_usage.ru_utime.tv_sec,r_usage.ru_utime.tv_usec,r_usage.ru_utime.tv_sec,r_usage.ru_utime.tv_usec);
    priority=getpriority(PRIO_PROCESS,getpid());  //find current priority
    printf("Current priority =%d\n",priority);

    getrlimit(RLIMIT_FSIZE,&r_limit);  //find current FSIZE limit
    printf("Current FSIZE limit: soft =%ld,hard =%ld\n",r_limit.rlim_cur,r_limit.rlim_max);

    r_limit.rlim_cur=2048;
    r_limit.rlim_max=4096;
    printf("Setting a 2K file size limit:\n");
    setrlimit(RLIMIT_FSIZE,&r_limit);

    work();
    exit(0);

}

/usr/bin/ld: /tmp/ccQdpMCo.o: in function `work':
main.c:(.text+0xc2): undefined reference to `log'
collect2: error: ld returned 1 exit status
*/

