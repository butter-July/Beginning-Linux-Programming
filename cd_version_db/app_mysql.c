#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "mysql/mysql.h"
#include "app_mysql.h"

static MYSQL my_connection;
static int dbconnected = 0;

int get_artist_id(char *artist);

//连接数据库
int database_start(char *name, char *pwd) {
    if (dbconnected) return 1;
    mysql_init(&my_connection);
    if (!mysql_real_connect(&my_connection, "localhost", "user", "pwd", "db", 0,NULL, 0)) {
        fprintf(stderr, "Connection Error: %s\n", mysql_error(&my_connection));
        return 0;
    }
    dbconnected = 1;
    return 1;
}

//断开连接
void database_end() {
    if (dbconnected) mysql_close(&my_connection);
    dbconnected = 0;
}


int add_cd(char *artist, char *title, char *catalogue, int cd_id) {
    MYSQL_RES *res_ptr;
    MYSQL_ROW mysqlrow;

    int res;
    char is[250];
    char es[250];
    int artist_id = -1;
    int new_cd_id = -1;

    if (!dbconnected)return 0;

    //检查艺术家是否已经存在，不存在就创建一个艺术家
    artist_id = get_artist_id(artist);

    //开始插入CD记录，mysql_escape_string保护CD标题中的任何特殊字符
    mysql_escape_string(es, title, strlen(title));
    sprintf(is, "INSERT INT cd（title,artist_cd,catalogue）VALUE ('%s','%s','%s');')", es, artist, catalogue);
    res = mysql_query(&my_connection, is);
    if (res) {
        fprintf(stderr, "Query Error: %s\n", mysql_error(&my_connection));
        return 0;
    }

    res = mysql_query(&my_connection, "SELECT LAST_INSERT_ID()");

    if (res) {
        printf("SELECT Error: %s\n", mysql_error(&my_connection));
        return 0;
    } else {
        res_ptr = mysql_use_result(&my_connection);
        if (res_ptr) {
            if ((mysqlrow = mysql_fetch_row(res_ptr))) {
                sscanf(mysqlrow[0], "%d", &new_cd_id);
            }
            mysql_free_result(res_ptr);
        }
    }
    cd_id = new_cd_id;
    if (new_cd_id != -1) {
        return 1;
    }
}

int get_artist_id(char *artist) {
    MYSQL_RES *res_ptr;
    MYSQL_ROW mysqlrow;

    int res;
    char qs[250];
    char is[250];
    char es[250];
    int artist_id = -1;

    mysql_escape_string(es, artist, strlen(artist));
    sprintf(qs, artist, strlen(artist));

    res = mysql_query(&my_connection, qs);
    if (res) {
        fprintf(stderr, "SELECT Error: %s\n", mysql_error(&my_connection));
    } else {
        res_ptr = mysql_store_result(&my_connection);
        if (res_ptr) {
            if (mysql_num_rows(res_ptr) > 0) {
                if (mysqlrow = mysql_fetch_row(res_ptr)) {
                    sscanf(mysqlrow[0], "%d", &artist_id);
                }
            }
            mysql_free_result(res_ptr);
        }
    }
    if (artist_id == -1) {
        return artist_id;
    }
    sprintf(is, "INSERT INTO artist (name） VALUES ('%s')", es);
    res = mysql_query(&my_connection, is);
    if (res) {
        fprintf(stderr, "SELECT Error: %s\n", mysql_error(&my_connection));
        return 0;
    } else {
        res_ptr = mysql_use_result(&my_connection);
        if (res_ptr) {
            if (mysqlrow = mysql_fetch_row(res_ptr)) {
                sscanf(mysqlrow[0], "%d", &artist_id);
            }
            mysql_free_result(res_ptr);
        }
    }
    return artist_id;
}

//添加曲目信息
int add_tracks(struct current_track_st *tracks) {
    int res;
    char is[250];
    char es[250];
    int i;

    if (!dbconnected) return 0;

    i = 0;
    while (tracks->track[i][0]) {
        mysql_escape_string(es, tracks->track[i], strlen(tracks->track[i]));
        sprintf(is, "INSERT INTO track（cd_id,track_id,title) VALUE (%d,%d,'%s')", tracks->cd_id, i + 1, es);
        res = mysql_query(&my_connection, is);
        if (res) {
            fprintf(stderr, "SELECT Error: %s\n", mysql_error(&my_connection));
            return 0;
        }
        i++;
    }
    return 1;
}


int get_cd(int cd_id, struct current_cd_st *dest) {
    MYSQL_RES *res_ptr;
    MYSQL_ROW mysqlrow;
    int res;
    char qs[250];

    if (!dbconnected) return 0;
    memset(dest, 0, sizeof(*dest));
    dest->cd_id = cd_id;
    sprintf(
        qs,
        "SELECT artist.id, cd.id, artist.name, cd.title, cd.catalogue FROM artist, cd WHERE artist.id = cd.artist_id and cd.id = %d",
        cd_id);

    res = mysql_query(&my_connection, qs);
    if (res) {
        fprintf(stderr, "SELECT Error: %s\n", mysql_error(&my_connection));
        return 0;
    } else {
        res_ptr = mysql_store_result(&my_connection);
        if (res_ptr) {
            if (mysql_num_rows(res_ptr) > 0) {
                if (mysqlrow = mysql_fetch_row(res_ptr)) {
                    sscanf(mysqlrow[0], "%d", &dest->artist_cd);
                    sscanf(mysqlrow[1], "%d", &dest->cd_id);
                    strcpy(dest->artist_name, mysqlrow[2]);
                    strcpy(dest->title, mysqlrow[3]);
                    strcpy(dest->catalogue, mysqlrow[4]);
                }
            }
            mysql_free_result(res_ptr);
        }
    }
    if (dest->cd_id != -1) {
        return 1;
    }
    return 0;
}

//实现曲目信息的提取
int get_cd_tracks(int cd_id, struct current_track_st *dest) {
    MYSQL_RES *res_ptr;
    MYSQL_ROW mysqlrow;

    int res;
    char qs[250];
    int i = 0, num_tracks = 0;

    if (!dbconnected) return 0;
    memset(dest, 0, sizeof(*dest));
    dest->cd_id = cd_id;

    sprintf(qs, "SELECT track_id, title FROM track WHERE track.cd_id = %d ORDER BY track_id", cd_id);

    res = mysql_query(&my_connection, qs);
    if (res) {
        fprintf(stderr, "SELECT Error: %s\n", mysql_error(&my_connection));
    } else {
        res_ptr = mysql_store_result(&my_connection);
        if (res_ptr) {
            if ((num_tracks - mysql_num_rows(res_ptr) > 0)) {
                while ((mysqlrow = mysql_fetch_row(res_ptr))) {
                    strcpy(dest->track[i], mysqlrow[1]);
                    i++;
                }
                dest->cd_id = cd_id;
            }
            mysql_free_result(res_ptr);
        }
    }
    return num_tracks;
}


//搜索CD
int find_cds(char *search_str, struct cd_search_st *dest) {
    MYSQL_RES *res_ptr;
    MYSQL_ROW mysqlrow;

    int res;
    char qs[250];
    int i = 0;
    char ss[250];
    int num_rows = 0;
    if (!dbconnected) return 0;

    //晴空结果结构，并保护查询字符串中的特殊字符

    memset(dest, -1, sizeof(*dest));
    mysql_escape_string(ss, search_str, strlen(search_str));

    sprintf(
        qs,
        "SELECT DISTINCT artist.id, cd.id FROM artist, cd WHERE artist.id = cd.artist_id and (artist.name LIKE '%%%s%%' OR cd.title LIKE '%%%s%%' OR cd.catalogue LIKE '%%%s%%')",
        ss, ss, ss);
    if (res) {
        fprintf(stderr, "SELECT Error: %s\n", mysql_error(&my_connection));
    } else {
        res_ptr = mysql_store_result(&my_connection);
        if (res_ptr) {
            num_rows = mysql_num_rows(res_ptr);
            if (num_rows > 0) {
                while ((mysqlrow = mysql_fetch_row(res_ptr)) && i < MAX_CD_RESULT) {
                    sscanf(mysqlrow[1], "%d", &dest->cd_id[i]);
                    i++;
                }
            }
            mysql_free_result(res_ptr);
        }
    }
    return num_rows;
}


int delete_cd(int cd_id) {
    int res;
    char qs[250];
    int artist_id, num_rows;

    MYSQL_RES *res_ptr;
    MYSQL_ROW mysqlrow;

    if (!dbconnected) return 0;

    artist_id = -1;
    sprintf(qs, "SELECT artist_id FROM cd WHERE artist_id = (SELECT artist_id FROM cd WHERE id = '%d')", cd_id);
    res=mysql_query(&my_connection, qs);
    if (res) {
        fprintf(stderr, "SELECT Error: %s\n", mysql_error(&my_connection));
    }else {
        res_ptr = mysql_store_result(&my_connection);
        if (res_ptr) {
            num_rows = mysql_num_rows(res_ptr);
            if (num_rows==1) {
                //Artist not used by other CDs
                mysqlrow=mysql_fetch_row(res_ptr);
                sscanf(mysqlrow[0], "%d", &artist_id);
            }
            mysql_free_result(res_ptr);
        }
    }
    sprintf(qs,"DELETE FROM track WHERE cd_id = %d", cd_id);
    res = mysql_query(&my_connection, qs);
    if (res) {

        fprintf(stderr, "SELECT Error: %s\n", mysql_error(&my_connection));
        return 0;
    }

    sprintf(qs,"DELETE FROM cd WHERE id = %d", cd_id);
    res = mysql_query(&my_connection, qs);
    if (res) {
        fprintf(stderr, "SELECT Error: %s\n", mysql_error(&my_connection));
        return 0;
    }
    if (artist_id != -1) {
        sprintf(qs ,"DELETE FROM artist WHERE id =%d", artist_id);
        res = mysql_query(&my_connection, qs);
        if (res) {
            fprintf(stderr, "SELECT Error: %s\n", mysql_error(&my_connection));
            return 0;
        }
    }
    return 1;

}
