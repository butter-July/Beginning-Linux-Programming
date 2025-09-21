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
    //用字符填充基本窗口
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
    popup_window_ptr=newwin(10, 20, 8,8);
    box(popup_window_ptr, '|','-');
    mvwprintw(popup_window_ptr, 5, 2, "%s", "Pop up Windows");
    wrefresh(popup_window_ptr);
    sleep(2);

    //
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
            */

}
