CC = gcc
CFLAGS = -g -I/usr/include/mysql
LDFLAGS = -L/usr/lib/mysql -lmysqlclient
TARGET = app
SRC = main.c app_mysql.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o
