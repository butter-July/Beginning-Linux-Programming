/*
 *Version1  他可以让我输入q,a之类的然后回车获得You........
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
char *menu[]={
    "a - add new record",
    "d - delete record",
    "q - quit",
    NULL,
};

int getchoice(char *great,char *choices[] ,FILE *in, FILE *out);

int main(int argc, char *argv[]) {

    int choice=0;

    FILE *input,*output;


    if (!isatty(fileno(stdout))) {
        fprintf(stderr,"You are not a terminal!,OK.\n");
    }
    input = fopen("dev/tty","r");
    output = fopen("dev/tty","w");
    if (!input||!output) {
        fprintf(stderr,"You are not a terminal,OK.\n");
        exit(1);
    }

    do {
        choice = getchoice("Please select an option",menu,input,output);
        printf("You have chosen %c\n",choice);;
    }while (choice != 'q');
    exit(0);
}

int getchoice(char *great,char *choices[],FILE *in, FILE *out) {

    int chosen=0;
    int selected;
    char **option;

    do {

        fprintf("Chioce :%s\n",great);
        option =choices;
        while (*option) {
            fprintf(" %s\n",*option);
            option++;
        }
        do {
            selected=fgetc(in);
        }while (selected != '\n');
        option=choices;
        while (*option) {
            if (selected == *option[0]) {
                chosen=1;
                break;
            }
            option++;
        }
        if (!chosen) {
            fprintf(out,"Incorrect choice , select again\n");
        }
        //while (getchar()!='\n');
    }while (!chosen);

    return selected;

}
*/




/*
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>

#define PASSWORD_LEN 8

int main(int argc, char *argv[]) {
    struct termios initialrsettings,newrsettings;
    char password[PASSWORD_LEN+1];

    tcgetattr(fileno(stdin), &initialrsettings);
    newrsettings = initialrsettings;
    newrsettings.c_lflag &= ~ECHO;

    printf("Enter your password:");

    if (tcsetattr(fileno(stdin),TCSAFLUSH,&newrsettings)!=0) {
        fprintf(stderr,"Could not set attributes\n");
    }else {
        fgets(password,PASSWORD_LEN,stdin);
        tcsetattr(fileno(stdin),TCSANOW,&initialrsettings);
        fprintf(stdout,"\n You entered %s\n",password);
    }
    exit(0);
}
*/

/*
 *Version2  输入a...之后不需要回车，修改了终端的..巴拉巴拉之类的
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int isatty(int fd);

char *menu[]={
    "a - add new record",
    "d - delete record",
    "q - quit",
    NULL,
};

int getchoice(char *great,char *choices[] ,FILE *in, FILE *out);

int main(int argc, char *argv[]) {
    int choice=0;
    FILE *input,*output;
    struct termios  initial_settings,new_settings;

    if (!isatty(fileno(stdout))) {
        fprintf(stderr,"You are not a terminal!,OK.\n");
    }
    input = fopen("/dev/tty","r");
    output = fopen("/dev/tty","w");  //这里是/dev!
    if (!input||!output) {
        fprintf(stderr,"Unable to open  /dev/tty\n");
        exit(1);
    }

    //调用getchoice之前改变终端的特性

    tcgetattr(fileno(input),&initial_settings);
    new_settings=initial_settings;
    new_settings.c_lflag &= ~ECHO;  //本地回显
    new_settings.c_lflag &= ~ICANON; //标准输入处理
    new_settings.c_cc [VMIN]=1; //MIN
    new_settings.c_cc [VTIME]=0;//TIME
    new_settings.c_lflag &=~ISIG;  //启用信号

    if (tcsetattr(fileno(input),TCSANOW,&new_settings)!=0) {
        fprintf(stderr,"could not set attributes\n");
    }

    do {
        choice = getchoice("Please select an option",menu,input,output);
        printf("You have chosen %c\n",choice);;
    }while (choice != 'q');
    tcsetattr(fileno(input),TCSANOW,&initial_settings);  //退出程序之前把终端属性变回去
    exit(0);

}


int getchoice(char *great,char *choices[],FILE *in, FILE *out) {

    int chosen=0;
    int selected;
    char **option;

    do {

        fprintf(out,"Chioce :%s \n",great);
        option =choices;
        while (*option) {
            fprintf(out," %s\n",*option);
            option++;
        }
        do {
            selected=fgetc(in);
        }while (selected == '\n' || selected == '\r');
        option=choices;
        while (*option) {
            if (selected == *option[0]) {
                chosen=1;
                break;
            }
            option++;
        }
        if (!chosen) {
            fprintf(out,"Incorrect choice , select again\n");
        }
        //while (getchar()!='\n');
    }while (!chosen);

    return selected;

}*/
//Version 3 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <term.h>
#include <curses.h>

static FILE *output_stream = (FILE *)0;

char *menu[] = {
    "a - add new record",
    "d - delete record",
    "q - quit",
    NULL,
};

int getchoice(char *greet, char *choices[], FILE *in, FILE *out);
int char_to_terminal(int char_to_write);

int main()
{
    int choice = 0;
    FILE *input;
    FILE *output;
    struct termios initial_settings, new_settings;

    if (!isatty(fileno(stdout))) {
        fprintf(stderr,"You are not a terminal, OK.\n");
    }

    input = fopen("/dev/tty", "r");
    output = fopen("/dev/tty", "w");
    if(!input || !output) {
        fprintf(stderr, "Unable to open /dev/tty\n");
        exit(1);
    }
    tcgetattr(fileno(input),&initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_lflag &= ~ISIG;
    if(tcsetattr(fileno(input), TCSANOW, &new_settings) != 0) {
        fprintf(stderr,"could not set attributes\n");
    }

    do {
        choice = getchoice("Please select an action", menu, input, output);
        printf("You have chosen: %c\n", choice);
	sleep(1);
    } while (choice != 'q');

    tcsetattr(fileno(input),TCSANOW,&initial_settings);
    exit(0);
}

int getchoice(char *greet, char *choices[], FILE *in, FILE *out)
{
    int chosen = 0;
    int selected;
    int screenrow, screencol = 10;

    char **option;
    char *cursor, *clear;

    output_stream = out;

    setupterm(NULL,fileno(out), (int *)0);
    cursor = tigetstr("cup");
    clear = tigetstr("clear");

    screenrow = 4;
    tputs(clear, 1, char_to_terminal);
    tputs(tparm(cursor, screenrow, screencol), 1, char_to_terminal);
    fprintf(out, "Choice: %s", greet);
    screenrow += 2;
    option = choices;
    while(*option) {
        tputs(tparm(cursor, screenrow, screencol), 1, char_to_terminal);
        fprintf(out,"%s", *option);
        screenrow++;
        option++;
    }
    fprintf(out, "\n");

    do {
	fflush(out);
        selected = fgetc(in);
        option = choices;
        while(*option) {
            if(selected == *option[0]) {
                chosen = 1;
                break;
            }
            option++;
        }
        if(!chosen) {
            tputs(tparm(cursor, screenrow, screencol), 1, char_to_terminal);
            fprintf(out,"Incorrect choice, select again\n");
        }
    } while(!chosen);
    tputs(clear, 1, char_to_terminal);
    return selected;
}

int char_to_terminal(int char_to_write)
{
    if (output_stream) putc(char_to_write, output_stream);
    return 0;
}
