#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include<ndbm.h>

#include "cd_data.h"

#define CDC_FILE_BASE "cdc_data"
#define CDT_FILE_BASE "cdt_cata"
#define CDC_FILE_DIR "cdc_data.dir"
#define CDC_FILE_PAG "cdc_data.pag"
#define CDT_FILE_DIR "cdt_cata.directory"
#define CDT_FILE_PAG "cdt_cata.pag"

//使用下面两个文件范围变量追踪当前数据库  文件范围变量
static DBM *cdc_dbm_ptr =NULL;
static DBM *cdt_dbm_ptr = NULL;


//强迫其创建一个新的数据库，传递给她一个非0 的new_database 之后可以创建一个新的数据库
int database_initialize(const int new_database)
{
    int open_mode = O_CREAT | O_RDWR;

   if (cdt_dbm_ptr) dbm_close(cdc_dbm_ptr);
    if (cdt_dbm_ptr) dbm_close(cdt_dbm_ptr);

    if (new_database) {
        (void) unlink(CDC_FILE_PAG);
        (void) unlink(CDC_FILE_DIR);
        (void) unlink(CDT_FILE_PAG);
        (void) unlink(CDT_FILE_DIR);
    }

    cdc_dbm_ptr=dbm_open(CDC_FILE_BASE,open_mode,0644);
    cdt_dbm_ptr=dbm_open(CDT_FILE_BASE,open_mode,0644);
    if (!cdc_dbm_ptr||!cdt_dbm_ptr) {
        fprintf(stderr,"Unable to create database\n");
        cdc_dbm_ptr= NULL;  //所以这里也是NULL
        cdt_dbm_ptr= NULL;
        return (0);
    }
    return (1);
}

void database_close(void) {
    if (cdc_dbm_ptr) dbm_close(cdc_dbm_ptr);
    if (cdt_dbm_ptr) dbm_close(cdt_dbm_ptr);

    cdc_dbm_ptr=NULL;  //数据库指针置为空，表示数据库关闭了
    cdt_dbm_ptr=NULL;
}




