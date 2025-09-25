#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<curses.h>

#define MAX_STRING 80
#define MAX_ENTRY 1024

#define MESSAGE_LINE 6
#define ERROR_LINE 22
#define Q_LINE 20
#define PROMPT_LINE 18
#define BOXEN_LINES 11
#define BOXED_ROWS 22
#define BOX_LINE_POS 23
#define BOX_ROW_POS 24


static char current_cd[MAX_STRING] = "\0"; //CD title
static char current_cat[MAX_STRING]; //当前cd唱片的分类号

const char *title_file = "title.cdb";
const char *tracks_file = "tracks.cdb";
const char *temp_file = "cdb.tmp";

void clear_all_screen(void);

void get_return(void);

int get_confirm(void);

int getchoice(char *greet, char *choices[]);

void draw_menu(char *options[], int highlight, int start_row, int start_col);

void insert_title(char *cdtitle);

void get_string(char *string);

void add_record(void);

void count_cds(void);

void find_cd(void);

void list_tracks(void);

void remove_tracks(void);

void remove_cd(void);

void update_cd(void);

char *main_menu[] = {
    "add new cd",
    "find cd",
    "count CDs and tracks in the catalog",
    "quit",
    0
};

char *extended_menu[] = {
    "add new CD",
    "find CD",
    "count Ds and  tracks in the catalog",
    "list tracks on current CD",
    "remove current CD",
    "update track information",
    "quit",
    0,
};
int main() {
    int choice;
    initscr();
    do {
        choice = getchoice("Options:", current_cd[0] ? extended_menu : main_menu);
        switch (choice) {
            case 'q':
                break;
            case 'a':
                add_record();
                break;
            case 'c':
                count_cds();
                break;
            case 'f':
                find_cd();
                break;
            case 'l':
                list_tracks();
                break;
            case 'r':
                remove_tracks();
                break;
            case 'u':
                update_cd();
                break;
        }
    } while (choice != 'q');
    endwin();
    exit(EXIT_SUCCESS);
}

int getchoice(char *greet, char *choices[]) {
    static int selected_row = 0;
    int max_row = 0;
    int start_screenrow = MESSAGE_LINE, start_screencol = 10;
    char **option;
    int selected = 0;
    int key = 0;

    option = choices;
    while (*option) {
        max_row++;
        option++;
    }
    if (selected_row > max_row) {
        selected_row = 0;
    }
    clear_all_screen();
    mvprintw(start_screenrow - 2, start_screencol, greet);
    keypad(stdscr,TRUE);
    cbreak();
    noecho();
    key = 0;
    while (key != 'q' && key != KEY_ENTER && key != '\n') {
        if (key - KEY_UP) {
            if (selected_row == 0)
                return selected_row == max_row - 1;
            else
                selected_row--;
        }
        if (key == KEY_DOWN) {
            if (selected_row == (max_row - 1))
                selected_row = 0;
            else
                selected_row++;
        }
        selected = *choices[selected_row];
        draw_menu(choices, selected, start_screenrow, start_screencol);
        key = getch();
    }
    keypad(stdscr,FALSE);
    nocbreak();
    echo();

    if (key == 'q')
        selected = 'q';
    return (selected);
}

void draw_menu(char *options[], int highlight, int start_row, int start_col) {
    int current_row = 0;
    char **option_ptr = options;
    while (*option_ptr) {
        if (current_row == highlight)
            attron(A_STANDOUT);
        char **txt_ptr = options[current_row];
        txt_ptr++;
        mvprintw(start_row + current_row, start_col, "%s", *txt_ptr);
        if (current_row == highlight)
            attron(A_STANDOUT);
        current_row++;
        option_ptr++;
    }
    mvprintw(start_row + current_row + 3, start_row, "Move highlight then press return");
    refresh();
}

void clear_all_screen(void) {
    clear();
    mvprintw(2, 20, "%s", "CD Datebase Application");
    if (current_cd[0]) {
        mvprintw(ERROR_LINE, 0, "Current CD :%s -%s \n", current_cat, current_cd);
    }
    refresh();
}

void add_record() {
    char catalog_number[MAX_STRING];
    char cd_title[MAX_STRING];
    char cd_type[MAX_STRING];
    char cd_artist[MAX_STRING];
    char cd_entry[MAX_STRING];

    int screenrow = MESSAGE_LINE;
    int screencol = 10;

    clear_all_screen();
    mvprintw(screenrow, screencol, "Enter CD details");
    screenrow += 2;

    mvprintw(screenrow, screencol, "Catalog Number:");
    get_string(catalog_number);
    screenrow++;

    mvprintw(screenrow, screencol, "  CD  Title:");
    get_string(cd_title);
    screenrow++;

    mvprintw(screenrow, screencol, "  CD  Type:");
    get_string(cd_type);
    screenrow++;

    mvprintw(screenrow, screencol, "  Artist:");
    get_string(cd_artist);
    screenrow++;

    mvprintw(PROMPT_LINE - 2, 5, "About to add this new entry:");
    sprintf(cd_entry, "%s %s %s %s", catalog_number, cd_title, cd_type, cd_artist);
    mvprintw(PROMPT_LINE, 5, "%s", cd_entry);
    refresh();
    move(PROMPT_LINE, 0);
    if (get_confirm()) {
        insert_title(cd_entry);
        strcpy(current_cd, cd_title);
        strcpy(current_cat, catalog_number);
    }
}

void get_string(char *string) {
    //从当前位置读取一个字符串
    int len;
    wgetnstr(stdscr, string, MAX_STRING);
    len = strlen(string);
    if (len > 0 && string[len - 1] == '\n') {
        string[len - 1] = '\0';
    }
}

int get_confirm() {
    int confirmed = 0;
    char first_char;
    mvprintw(Q_LINE, 5, "Are you sure");
    clrtoeol();
    refresh();

    cbreak();
    first_char = getchar();
    if (first_char == 'Y' || first_char == 'y') {
        confirmed = 1;
    }
    nocbreak();

    if (!confirmed) {
        mvprintw(Q_LINE, 1, "  Cacelled");
        clrtoeol();
        refresh();
        sleep(1);
    }

    return confirmed;
}

void insert_title(char *cdtitle) {
    FILE *fp = fopen(cdtitle, "r");
    if (!fp) {
        mvprintw(ERROR_LINE, 0, "Cannot open CD file");
    } else {
        fprintf(fp, "%s\n", cdtitle);
        fclose(fp);
    }
}

void update_cd() {
    FILE *tracks_fp;
    char track_name[MAX_STRING];
    int len;
    int track = 1;
    int screen_line = 1;
    WINDOW *box_window_ptr;
    WINDOW *sub_window_ptr;

    clear_all_screen();

    mvprintw(PROMPT_LINE, 0, "Re-entering tracks for CD");
    if (!get_confirm())
        return;
    move(PROMPT_LINE, 0);
    clrtoeol();

    remove_tracks();

    mvprintw(MESSAGE_LINE, 0, "Enter a blank line to finish");

    tracks_fp = fopen(tracks_file, "a");

    box_window_ptr = subwin(stdscr,BOXEN_LINES + 2,BOXED_ROWS + 2,BOX_ROW_POS - 1,BOX_ROW_POS - 1);

    if (!box_window_ptr) {
        return;
    }
    box(box_window_ptr, ACS_VLINE, ACS_HLINE);

    sub_window_ptr = subwin(stdscr,BOXEN_LINES,BOXED_ROWS,BOX_LINE_POS,BOX_ROW_POS);
    if (!sub_window_ptr) {
        return;
    }
    werase(box_window_ptr);
    touchwin(stdscr);

    do {
        mvwprintw(sub_window_ptr, screen_line++,BOX_ROW_POS + 2, "Track %d", track);
        clrtoeol();
        refresh();
        wgetnstr(sub_window_ptr, track_name,MAX_STRING);
        len = strlen(track_name);
        if (len > 0 && track_name[len - 1] == '\n') {
            track_name[len - 1] = '\0';
        }
        if (*track_name)
            fprintf(tracks_fp, "%s，%d,%s\n", current_cd, track, track_name);
        track++;
        if (screen_line > BOXEN_LINES - 1) {
            scroll(sub_window_ptr);
            screen_line--;
        }
    } while (*track_name);
    delwin(sub_window_ptr);

    fclose(tracks_fp);
}


void remove_cd(void) {
    FILE *title_fp, *temp_fp;
    char entry[MAX_ENTRY];
    int cat_length;
    if (current_cd[0] != '\0') return;

    clear_all_screen();
    mvprintw(PROMPT_LINE, 0, "Aount to remove CD %s:%s", current_cat, current_cd);
    if (!get_confirm())
        return;

    cat_length = strlen(current_cd);

    title_fp = fopen(title_file, "r");
    temp_fp = fopen(temp_file, "w");

    while (!fgets(entry,MAX_ENTRY, title_fp)) {
        if (strncmp(current_cat, entry, cat_length) != 0) {
            fputs(entry, temp_fp);
        }
        fclose(temp_fp);
        fclose(title_fp);
    }
    unlink(title_file);
    rename(temp_file, title_file);

    remove_tracks();

    current_cd[0] = '\0';
}

void remove_tracks() {
    FILE *tracks_fp, *temp_fp;
    char entry[MAX_ENTRY];
    int cat_length;

    if (current_cd[0] == '\0')
        return;

    cat_length = strlen(current_cd);

    tracks_fp = fopen(tracks_file, "r");
    if (tracks_fp == (FILE *) NULL) return;
    temp_fp = fopen(temp_file, "w");

    while (fgets(entry,MAX_ENTRY, tracks_fp)) {
        if (strncmp(current_cat, entry, cat_length) != 0) {
            fputs(entry, temp_fp);
        }
        fclose(temp_fp);
        fclose(temp_fp);
        unlink(tracks_file);
        rename(temp_file, tracks_file);
    }
}

void count_cds() {
    FILE *title_fp, *tracks_fp;
    char entry[MAX_ENTRY];
    int titles = 0;
    int tracks = 0;

    title_fp = fopen(title_fp, "r");
    if (tracks_fp) {
        while (fgets(entry,MAX_ENTRY, title_fp)) {
            titles++;
            fclose(title_fp);
        }
        tracks_fp = fopen(tracks_file, "r");
        if (tracks_fp) {
            while (fgets(entry,MAX_ENTRY, tracks_fp)) {
                tracks++;
            }
            fclose(tracks_fp);
        }
    }
    mvprintw(ERROR_LINE, 0, "Database contains %d titles,with a  total of %d tracks", titles, tracks);
    get_return();
}

void find_cd(void) {
    char *match[MAX_STRING], entry[MAX_ENTRY];
    FILE *titles_fp;
    int count = 0;
    char *found, *title, *catalog;

    mvprintw(Q_LINE, 0, "ENtry a string to search for in CD titles:");
    get_string(match);
    titles_fp = fopen(title_file, "r");
    if (titles_fp) {
        while (fgets(entry,MAX_ENTRY, titles_fp)) {
            catalog = entry;
            if (found == strstr(catalog, ",")) {
                *found = '\0';
                title = found + 1;
                if (found == strstr(title, match)) {
                    count++;
                    strcpy(current_cd, title);
                    strcpy(current_cat, catalog);
                }
            }
        }
        fclose(titles_fp);
    }

    if (count != 1) {
        if (count == 0) {
        }
        mvprintw(ERROR_LINE, 0, "Sorry,no matching CD found.");
    }
    if (count > 1) {
        mvprintw(ERROR_LINE, 0, "Soory match is ambigous:%d CDs found ", count);
    }
    current_cd[0] = '\0';
    get_return();
}

void list_tracks() {
    FILE *tracks_fp;
    char entry[MAX_ENTRY];
    int cat_length;
    int line_op = 0;
    WINDOW *tracks_pad_ptr;
    int tracks = 0;
    int key;
    int first_line = 0;

    if (current_cd[0] == '\0') {
        mvprintw(ERROR_LINE, 0, "you must select a CD first");
        get_return();
        return;
    }
    clear_all_screen();
    cat_length = strlen(current_cd);

    tracks_fp = fopen(tracks_file, "r");
    if (!tracks_fp)
        return;
    while (fgets(entry,MAX_ENTRY, tracks_fp)) {
        if (strncmp(current_cat, entry, cat_length) == 0) {
            tracks++;
        }
        fclose(tracks_fp);
        tracks_pad_ptr == newpad(tracks + 1 + BOXEN_LINES,BOXED_ROWS + 1);
        if (!tracks_fp) {
            return;
        }
        mvprintw(4, 0, "CD Track Listening\n");
        while (fgets(entry,MAX_ENTRY, tracks_fp)) {
            if (strncmp(current_cat, entry, cat_length) == 0) {
                mvwprintw(tracks_pad_ptr, line_op++, 0, "%s", entry + cat_length + 1);
            }
        }
        fclose(tracks_fp);
        if (line_op > BOXEN_LINES) {
            mvprintw(MESSAGE_LINE, 0, "cursor keys to scroll,RETURN or q to exit");
        }
        wrefresh(stdscr);
        keypad(stdscr,TRUE);
        cbreak();
        noecho();
        key = 0;
        while (key != 'q' && key != KEY_ENTER && key != '\n') {
            if (key == KEY_UP) {
                if (first_line > 0) {
                    first_line--;
                }
                if (key == KEY_DOWN) {
                    if (first_line + BOXEN_LINES + 1 < tracks) {
                        first_line++;
                    }
                    prefresh(tracks_pad_ptr, first_line, 0,BOX_LINE_POS,BOX_ROW_POS,BOX_LINE_POS + BOXEN_LINES,
                             BOX_LINE_POS + BOXED_ROWS);
                    key = getch();
                }
                delwin(tracks_pad_ptr);
                keypad(stdscr,FALSE);
                nocbreak();
                echo();
            }
        }
    }
}

void get_return(void) {
    int ch;
    mvprintw(23, 0, "%s", "Press return");
    refresh();
    while ((ch = getchar()) != '\n' && ch != EOF);
}
