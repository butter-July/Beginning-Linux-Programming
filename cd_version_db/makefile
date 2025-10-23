all: app

app: app_mysql.c app_mysql.h
	gcc -g -o app -I/usr/include/mysql app_mysql.c  -lmysqlclient -L/usr/lib/x86_64-linux-gnu
