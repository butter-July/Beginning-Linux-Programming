//数据库函数们，作用是中转站，把请求给服务器，然后从函数返回服务器的响应

#define CAT_CAT_LEN       30
#define CAT_TITLE_LEN     70
#define CAT_TYPE_LEN      30
#define CAT_ARTIST_LEN    70

typedef struct {
    char catalog[CAT_CAT_LEN + 1];
    char title[CAT_TITLE_LEN + 1];
    char type[CAT_TYPE_LEN + 1];
    char artist[CAT_ARTIST_LEN + 1];
} cdc_entry;


#define TRACK_CAT_LEN     CAT_CAT_LEN
#define TRACK_TTEXT_LEN   70

//每个entry的
typedef struct {
    char catalog[TRACK_CAT_LEN + 1];
    int  track_no;
    char track_txt[TRACK_TTEXT_LEN + 1];
} cdt_entry;
