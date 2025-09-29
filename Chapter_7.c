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
/*#include <unistd.h>
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
*/
//文件锁的竞争

/*
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>

const char *test_file ="/tmp/test_lock";

int main() {

    int file_desc;
    int byte_count;
    char  *byte_to_write ="A";
    struct flock region_1;
    struct flock region_2;       //变量们的声明
    int res;

    //打开文件描述符
    file_desc =open(test_file,O_RDWR|O_CREAT,0666);
    if (!file_desc) {
        fprintf(stderr,"Can't open test file\n");
        exit(EXIT_FAILURE);
    }

    for (byte_count=0;byte_count<100;byte_count++) {
        (void)write(file_desc,byte_to_write,1);  //写数据
    }

    region_1.l_type=F_RDLCK;  //共享锁
    region_1.l_whence=SEEK_SET;
    region_1.l_start=10;
    region_1.l_len=20;

    region_2.l_type=F_WRLCK; //独占锁
    region_2.l_whence=SEEK_SET;
    region_2.l_start=40;
    region_2.l_len=10;

    printf("Process %d locking file \n",getpid());
    res=fcntl(file_desc,F_SETLK,&region_1);
    if (res==-1) {
        fprintf(stderr,"filed to lock region_1\n");
    }
    res=fcntl(file_desc,F_SETLK,&region_2);
    if (res==-1) {
        fprintf(stderr,"filed to lock region_2\n");
    }

    sleep(60);

    printf("Process %d Closing file \n",getpid());
    close(file_desc);
    exit(EXIT_SUCCESS);
}
*/
//DBM程序
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
//#include <ndbm.h>
/* On some systems you need to replace the above with */
#include <gdbm-ndbm.h>
#include<string.h>
#define TEST_DB_FILE "/tmp/dbm1_test"
#define ITEMS_USED 3

struct test_data {
    char misc_chars[15];
    int any_integer;
    char more_chars[21];
};

int main() {
    struct test_data items_to_store[ITEMS_USED];
    struct test_data item_retrieved;

    char key_to_use[20];
    int i, result;


    datum key_datum; //关键字字符
    datum data_datum;

    DBM *dbm_ptr;

    dbm_ptr = dbm_open(TEST_DB_FILE,O_RDWR | O_CREAT, 0666);
    if (!dbm_ptr) {
        fprintf(stderr, "Failed to open database\n");
        exit(EXIT_FAILURE);
    }
    //添加数据
    memset(items_to_store, '\0', sizeof(items_to_store));
    strcpy(items_to_store[0].misc_chars, "First");
    items_to_store[0].any_integer = 47;
    strcpy(items_to_store[1].misc_chars, "Foo");
    items_to_store[1].any_integer = 13;
    strcpy(items_to_store[2].misc_chars, "Bar");
    strcpy(items_to_store[1].more_chars, "Unlucky?");

    strcpy(items_to_store[2].misc_chars, "Tird");
    items_to_store[2].any_integer = 3;
    strcpy(items_to_store[2].more_chars, "baz");

    //为每个数据项提供以后引用的关键字。它被设置为每个字符串的头一个字母+整数。由Key_datum标识

    for (i = 0; i < ITEMS_USED; i++) {
        sprintf(key_to_use, "%c%c%d", items_to_store[i].misc_chars[0], items_to_store[i].more_chars[0],
                items_to_store->any_integer);
        key_datum.dptr = (void *) key_to_use;
        key_datum.dsize = strlen(key_to_use);
        data_datum.dptr = (void *) &items_to_store[i];
        data_datum.dsize = sizeof(struct test_data);

        result = dbm_store(dbm_ptr, key_datum, data_datum,DBM_REPLACE);
        if (result != 0) {
            fprintf(stderr, "dbm_stor failed  on key %s\n", key_to_use);
            exit(2);
        }
    }
    sprintf(key_to_use, "bu%d", 13);
    key_datum.dptr = key_to_use;
    key_datum.dptr = key_to_use;
    key_datum.dsize = strlen(key_to_use);

    data_datum = dbm_fetch(dbm_ptr, key_datum);
    if (data_datum.dsize != 0) {
        printf("data received\n");
        memcpy(&item_retrieved, data_datum.dptr, data_datum.dsize);
        printf("Retrieved item - %s %d %s \n", item_retrieved.misc_chars, item_retrieved.any_integer,
               item_retrieved.more_chars);
    } else {
        printf("No data found for key %s\n", key_to_use);
    }
    dbm_close(dbm_ptr);
    exit(EXIT_SUCCESS);
}
