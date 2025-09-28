/*
 *分配内存分配内存 +free
 *malloc realloc free
#include <unistd.h>
#include<stdlib.h>
#include<stdio.h>

#define A_MEGABYTE (1024*1024)
#define PHY_MEN_MEGS 1024

int main() {

    char *some_memory;
    size_t size_to_allcate = A_MEGABYTE;
    int megs_obtained = 0;

    while (megs_obtained < (PHY_MEN_MEGS * 2)) {
        some_memory = (char*)malloc(size_to_allcate);
        if (some_memory) {
            megs_obtained ++;
            sprintf(some_memory,"Hello world");
            printf("%s - now allocated %d Megabytes\n",some_memory,megs_obtained);
        }else {
            exit(PHY_MEN_MEGS);
        }
    }
    exit(   EXIT_SUCCESS);
}
*/

/*
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
int main() {

    char *some_memory=(char *)0;
    printf("A read from null %s\n",some_memory);
    sprintf(some_memory,"A write to null");
    exit(EXIT_SUCCESS);

}
没有Segmentation Fault
*/
/*#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main() {
    char z = *(const char *)0;
    printf("Hello, World!\n");
    exit(EXIT_SUCCESS);
    //没有+1
}*/

/*
 *文件锁定
 *17代表已经存在
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    int file_desc;
    int save_errno;

    file_desc = open("LCK.test", O_RDWR| O_CREAT|O_EXCL,0444);  //第一次调用成功因为文件不存在，但是之后就会失败，因为文件已经存在了。要删除锁文件才能接着成功
    if (file_desc == -1) {
        save_errno = errno;
        printf("Open failed with error %d\n",save_errno);
    }
    else {
        printf("Open succeeded \n");
    }
    exit(EXIT_SUCCESS);
}
*/
#include <unistd.h>
#include <stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include <stdio.h>
const char  *lock_file="TCK.test2";
int main() {
    int file_desc;
    int tries=10;
    while (tries--) {
        file_desc = open("LCK.test2", O_RDWR| O_CREAT|O_EXCL,0444);
        if (file_desc == -1) {
            printf("%d has already present\n",getpid());
            sleep(3);
        }else {
            printf("%d -I have exclusive access\n",getpid());
            sleep(1);
            (void)close(file_desc);
            (void)unlink(lock_file);
            sleep(2);
        }
    }
    exit(EXIT_SUCCESS);
}

//文件锁的竞争
