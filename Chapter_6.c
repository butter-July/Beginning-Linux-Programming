/*
#include <stdio.h>
#include<curses.h>
#include <stdlib.h>
#include <unistd.h>
int main(void) {
    initscr();
    move(5,15);
    printw("%s","hello world");     //printw!!!!!!!!!!
    refresh();

    sleep(2);
    endwin();
    exit(EXIT_SUCCESS);
}
*/

/*
 *管理基于文本的屏幕
 *
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>

int main() {

    const char witch_one[]="First Witch";
    const char witch_two[]="Second Witch";
    const char *scan_ptr;
    initscr();

    move(5,15);
    attron(A_BOLD);
    printw("%s ","Macbeth");
    attroff(A_BOLD);
    refresh();
    sleep(1);

    move(8,15);
    attron(A_STANDOUT);
    printw("%s","Thunder and Lightning");
    attroff(A_STANDOUT);
    refresh();
    sleep(1);

    move(10,10);
    printw("%s","When shall we three meeet again");
    move(11,23);
    printw("%s","In thunder Lightning ,or in rain ?");
    move(13,10);
    printw("%s","When the hurlyburly's done");
    move(14,23);
    printw("%s","When the battle's lost and won");
    refresh();
    sleep(1);

    attron(A_DIM);
    scan_ptr=witch_one+strlen(witch_one)-1;
    while (scan_ptr!=witch_one) {
        move(10,10);
        insch(*scan_ptr--);
    } //确定演员并将他们的名字一次一个‘’的形式插入指定位置

    attron(A_DIM);
    scan_ptr=witch_two+strlen(witch_two)-1;
    while (scan_ptr!=witch_two) {
        move(13,10);
        insch(*scan_ptr--);
    }
    attron(A_DIM);
    refresh();
    sleep(1);

    move(LINES-1,COLS-1);
    refresh();
    sleep(1);

    endwin();
    exit(EXIT_SUCCESS);
}
*/

/*
 *键盘
 *ipmode.c
#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>

#define PW_LEN 256
#define NAME_LEN 256

int main() {
    char name[NAME_LEN];
    char password[PW_LEN];
    const char *real_password = "12323";
    int i = 0;

    initscr();

    move(5, 10);
    printw("%s", "Please login:");

    move(7, 10);
    printw("%s", "User name: ");
    getstr(name);

    move(8, 10);
    printw("%s", "Password: ");
    refresh();

    cbreak();
    noecho();

    memset(password, '\0', sizeof(password));
    while (i < PW_LEN) {
        password[i] = getch();
        if (password[i] == '\n') break;
        move(8, 20 + i);
        addch('*');
        refresh();
        i++;
    }
    echo();
    nocbreak();

    move(11, 10);
    if (strncmp(real_password, password, strlen(real_password)) == 0)
        printw("%s", "Correct");
    else printw("%s", "Wrong");
    printw("%s", " password");
    refresh();
    sleep(2);

    endwin();
    exit(EXIT_SUCCESS);
}
*/

/*
#include <unistd.h>
#include<stdlib.h>
#include<curses.h>

int main() {
    WINDOW *new_window_ptr;
    WINDOW *popup_window_ptr;
    int x_loop;
    int y_loop;
    char a_letter = 'a';

    initscr();
    //用字符填充基本窗口,填充逻辑屏幕之后，开始刷新物理屏幕
    move(5, 5);
    printw("%s", "Testing multiple windows");
    refresh();

    for (y_loop = 0; y_loop < LINES - 1; y_loop++) {
        for (x_loop = 0; x_loop < COLS - 1; x_loop++) {
            mvwaddch(stdscr, y_loop, x_loop, a_letter);
            a_letter++;
            if (a_letter == 'z')
                a_letter = 'a';
        }
    }
    refresh();
    sleep(2);

    //创建窗口 10*20 并添加文本
    new_window_ptr = newwin(10, 20, 5, 5);
    mvwprintw(new_window_ptr, 2, 2, "%s", "Hello World");
    mvwprintw(new_window_ptr, 5, 2, "%s", "Noticing how very long lines  wrap inside the windows");
    wrefresh(new_window_ptr);
    sleep(2);

    //对背景窗口的内容进行修改。
    a_letter = '0';
    for (y_loop = 0; y_loop < LINES - 1; y_loop++) {
        for (x_loop = 0; x_loop < COLS - 1; x_loop++) {
            mvwaddch(new_window_ptr, y_loop, x_loop, a_letter);
            a_letter++;
            if (a_letter > '9')
                a_letter = '0';
        }
    }

    refresh();
    sleep(2);

    wrefresh(new_window_ptr);
    sleep(2);

    //把新窗口调到最前面
    touchwin(new_window_ptr);
    wrefresh(new_window_ptr);
    sleep(2);

    //增加另一个边框的重叠窗口
    popup_window_ptr = newwin(10, 20, 8, 8);
    box(popup_window_ptr, '|', '-');
    mvwprintw(popup_window_ptr, 5, 2, "%s", "Pop up Windows");
    wrefresh(popup_window_ptr);
    sleep(2);

    //在清屏幕和删除这两个窗口之间在屏幕上轮流显示他们
    touchwin(popup_window_ptr);
    wrefresh(popup_window_ptr);
    sleep(2);
    wclear(new_window_ptr);
    wrefresh(new_window_ptr);
    sleep(2);
    delwin(new_window_ptr);
    touchwin(new_window_ptr);
    wrefresh(new_window_ptr);
    sleep(2);
    delwin(new_window_ptr);
    touchwin(new_window_ptr);
    refresh();
    sleep(2);
    endwin();
    exit(EXIT_SUCCESS);
    /*
     *不对+1
     *
     *fghij段错误 (核心已转储) fghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstu
    butter-july@butter-july-ASUS-Zenbook-14-UX3405MA-UX3405MA:~/CLionProjects/untitled6$ git push --set-upstream origin master
    fatal: 不是 git 仓库（或者任何父目录）：.gitghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabc
    butter-july@butter-july-ASUS-Zenbook-14-UX3405MA-UX3405MA:~/CLionProjects/untitled6$ nopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrs
    tuvwx                      |wxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghij
    klmno                      |nopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxya
    bcdef                      |efghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopq
    rstuv                      |uvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefgh
    ijklm                      |lmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwx
    yabcd                      |cdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmnopqrstuvwxyabcdefghijklmno
            |                  |
            |                  |
            └-┘
            #1#
}
*/

/*
#include <unistd.h>
#include <stdlib.h>
#include <curses.h>

int main() {
    //文本化初始窗口的显示
    WINDOW *sub_window_ptr;
    int x_loop;
    int y_loop;
    int counter;
    char a_letter = '1';
    initscr();

    for (y_loop = 0; y_loop < LINES - 1; y_loop++) {
        for (x_loop = 0; x_loop < COLS - 1; x_loop++) {
            mvwaddch(stdscr, y_loop, x_loop, a_letter);
            a_letter++;
            if (a_letter > '9') a_letter = '1';
        }
    }
    //创建新的卷动子窗口

    sub_window_ptr = subwin(stdscr, 10, 20, 10, 20);
    scrollok(sub_window_ptr, 1);

    touchwin(stdscr);
    refresh();
    sleep(1);


    //删除子窗口的内容，重新输出一些文字，然后刷新它
    werase(sub_window_ptr);
    mvwprintw(sub_window_ptr, 2, 0, "%s", "This window will now scroll");
    wrefresh(sub_window_ptr);
    sleep(1);

    for (counter = 1; counter < 10; counter++) {
        wprintw(sub_window_ptr, "%s", "This text is bouth wrapping and scrolling");
        wrefresh(sub_window_ptr);
        sleep(1);
    }

    delwin(sub_window_ptr);

    touchwin(stdscr);
    refresh();
    sleep(1);

    endwin();
    exit(EXIT_SUCCESS);
}
*/


/*
 *kaypad
 *
#include<unistd.h>
#include <stdlib.h>
#include<curses.h>

#define LOCAL_ESCAPE_KEY 27

int main() {
    int key;
    //对程序和curses库进行初始化，然后使用keypad模式
    initscr();
    crmode();
    keypad(stdscr, TRUE);

    noecho(); //防止光标在我按下方向键时发生移动
    clear(); //清屏
    mvprintw(5, 5, "%s", "Ket Pad demostration,Press 'q ' to quit");
    move(7, 5);
    refresh();
    key = getch();  //getch

    while (key != ERR && key != 'q') {
        move(7, 5);
        clrtoeol();

        if ((key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z')) {
            printw("Key was “%c", (char) key);  //这里在输出好神奇
        } else {
            switch (key) {
                case LOCAL_ESCAPE_KEY: printw("%s", "Escape key");
                    break;
                case KEY_END: printw("%s", "End key");
                    break;
                case KEY_BEG: printw("%s", "BEGINNING KEY");
                    break;
                case KEY_RIGHT: printw("%s", "RIGHT KEY DOWN");
                    break;
                case KEY_LEFT: printw("%s", "LEFT KEY DOWN");
                    break;
                case KEY_UP: printw("%s", "UP KEY DOWN");
                    break;
                case KEY_DOWN: printw("%s", "DOWN KEY DOWN");
                    break;
            }
        }
        refresh();
        key = getch();
    }
    endwin();
    exit(EXIT_SUCCESS);
}
*/
/*
 *彩色显示
 *
#include <unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<curses.h>

int main() {
    int i;

    initscr();

    if (!has_colors()) {
        endwin();
        fprintf(stderr, "Error -no color support on this terminal\n");
        exit(1);
    }

    if (start_color() != OK) {
        endwin();
        fprintf(stderr, "Error -counld not initialize colors\n");
        exit(2);
    }
    clear();
    mvprintw(5, 5, "These are %d COLORS and %d COLOR_PAIRS avaliable", COLORS, COLOR_PAIRS);
    refresh();
    //初始化颜色组合
    init_pair(1,COLOR_RED,COLOR_BLUE);
    init_pair(2,COLOR_GREEN,COLOR_BLUE);
    init_pair(3,COLOR_CYAN,COLOR_BLUE);
    init_pair(4,COLOR_YELLOW,COLOR_BLUE);
    init_pair(5,COLOR_WHITE,COLOR_BLACK);


    //打印出来 one by one
    for (i = 1; i <= 4; i++) {
        attroff(A_BOLD);
        attrset(COLOR_PAIR(i));
        mvprintw(5 + i, 5, "Color pair %d", i);
        attrset(COLOR_PAIR(i)|A_BOLD);
        mvprintw(5 + i, 25, "Color pair %d", i);
        refresh();
        sleep(1);
    }
    endwin();
    exit(EXIT_SUCCESS);
}
*/
/*
 *use pad
 *
#include <unistd.h>
#include<stdlib.h>
#include<curses.h>

int main() {
    WINDOW *pad_ptr;
    int x, y;
    int pad_lines;
    int pad_cols;
    char disp_char; //初始化pad结构

    initscr();
    pad_lines = LINES + 50;
    pad_cols = COLS + 50;
    pad_ptr = newpad(pad_lines, pad_cols);
    disp_char = 'a';
    //用字符填充pad结构
    for (x = 0; x < pad_lines; x++) {
        for (y = 0; y < pad_cols; y++) {
            mvwaddch(pad_ptr, x, y, disp_char);
            if (disp_char++ == 'z') {
                disp_char = 'a';
            } else {
                disp_char++;
            }
        }
    }
    prefresh(pad_ptr,5,7,2,2,9,9);
    sleep(1);
    prefresh(pad_ptr,LINES+5,COLS+7,5,5,21,19);
    sleep(1);
    delwin(pad_ptr);
    endwin();
    exit(EXIT_SUCCESS);
}
*/
