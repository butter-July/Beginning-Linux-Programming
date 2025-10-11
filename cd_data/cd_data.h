#define CAT_CAT_LEN 30 //catalog table
#define CAT_TITLE_LEN 70
#define CAT_TYPE_LEN 30
#define CAT_ARTIST_LEN 70

typedef struct {
    char  catalog [CAT_CAT_LEN+1];
    char title[CAT_TITLE_LEN+1];
    char type[CAT_TYPE_LEN+1];
    char artist[CAT_ARTIST_LEN+1];
}cdc_entry;

//tracks table one entry per track

#define TRACK_CAT_LEN CAT_CAT_LEN
#define TRACK_TTEXT_LEN 70

typedef struct {
    char catalog[TRACK_CAT_LEN+1];
    int track_no;
    char track_txt[TRACK_TTEXT_LEN+1];
} cdt_entry;

//开始定义需要的访问历程，cdc 处理标题数据项，cdt曲目数据项

//data initialize adn termination function
int database_initialize( int new_database);//要不要const
void database_close(void);

//data retrieval function
cdc_entry get_cdc_entry(const char *cd_catalog_ptr);
cdt_entry get_cdt_entry(const char *cd_catalog_ptr,int track_no); //要不要const?

//data add function
int add_cdc_entry(cdc_entry entry_to_add);//const
int add_cdt_entry(cdt_entry entry_to_add);//const

//data delete function
int del_cdc_entry(const char *cd_catalog_ptr);
int del_cdt_entry(const char *cd_catalog_ptr,int track_no);

//search function
cdc_entry searvh_cdc_entry(const char *cd_catalog_ptr,int *first_call_ptr);
