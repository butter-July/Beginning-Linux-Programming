/*
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int sockfd;
    int len;
    struct sockaddr_un address;
    int result;
    char ch='A';

    //创建一个套接字 for client
    sockfd =socket(AF_UNIX, SOCK_STREAM, 0);

    //给套接字命名
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path,"server_socket");
    len=sizeof(address);

    //连接我们的socket to the server socket
    result=connect(sockfd,(struct sockaddr*)&address,len);
    if (result == -1) {
        perror("oops:client1");
        exit(1);
    }

    //read and write
    write(sockfd,&ch,1);
    read(sockfd,&ch,1);
    printf("char from server = %c\n",ch);
    close(sockfd);
    exit(0);

}
*/

//网络客户
/*
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result;
    char ch = 'A';

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = 9704;
    len = sizeof(address);

    result = bind(sockfd, (struct sockaddr *)&address, len);
    if (result == -1) {
        perror("oops:client2");
        exit(1);
    }


    write(sockfd, &ch, 1);  //首先把A写进去

    //读出来server返回的
    read(sockfd, &ch, 1);
    printf("char from server = %c\n", ch);
    close(sockfd);
    exit(0);

}*/


/*
//获取一台主机的有关信息

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    char *host ,**names,**addrs;
    struct hostent *hostinfo;

//把host变量设置为getname程序提供的命令行参数，或默认设置为用户主机的主机名
    if(argc == 1) {
        char myname[256];
        gethostname(myname, 255);
        host = myname;
    }
    else
        host = argv[1];


    hostinfo = gethostbyname(host);
    if(!hostinfo ) {
        fprintf(stderr, "cannot get info for host: %s\n", host);
        exit(1);
    }

    //显示主机名和她可能的别名
    printf("results for host %s:\n", host);
    printf("Name: %s\n", hostinfo -> h_name);
    printf("Aliases:");
    names = hostinfo -> h_aliases;
    while(*names) {
        printf(" %s", *names);
        names++;
    }
    printf("\n");

    //检查查询的主机是不是一个IP主机，不是就警告+退出

    if(hostinfo -> h_addrtype != AF_INET) {
        fprintf(stderr, "not an IP host!\n");
        exit(1);
    }

    //否则，显示她的所有IP地址
    addrs = hostinfo -> h_addr_list;
    while(*addrs) {
        printf(" %s", inet_ntoa(*(struct in_addr *)*addrs));
        addrs++;
    }
    printf("\n");
    exit(0);

}
*/


/*
//查看服务器当前的日期和时间


#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char *host;
    int sockfd;
    int len, result;
    struct sockaddr_in address;
    struct hostent *hostinfo;
    struct servent *servinfo;
    char buffer[128];

    if (argc == 1)
        host = "localhost";
    else
        host = argv[1];

    //查询host address
    hostinfo = gethostbyname(host);
    if (!hostinfo) {
        fprintf(stderr, "no host: %s\n", host);
        exit(1);
    }

    //检查是否有daytime服务
    servinfo = getservbyname("daytime", "tcp");
    if (!servinfo) {
        fprintf(stderr, "no daytime service\n");
        exit(1);
    }
    printf("daytime port is %d\n", ntohs(servinfo->s_port));

    //创建一个socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //构造connect调用要使用的地址
    address.sin_family = AF_INET;
    address.sin_port = servinfo->s_port;
    address.sin_addr = *(struct in_addr *) *hostinfo->h_addr_list;
    len = sizeof(address);

    //然后连接，获取信息

    result = connect(sockfd, (struct sockaddr *) &address, len);
    if(result == -1) {
        perror("oops: getdate");
        exit(1);
    }

    result=read(sockfd,buffer,sizeof(buffer));
    buffer[result] = '\0';
    printf("read %d bytes: %s", result, buffer);

    close(sockfd);
    exit(0);
}*/
/*
 *
 *
 *server
 *
 *
 *
 *
//接受client的连接
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>


int main() {
    int server_sockfd, client_sockfd;
    int server_len ,client_len;
    struct sockaddr_un server_address;
    struct sockaddr_un client_address;

    unlink("server_socket");
    //create a socket for server
    server_sockfd=socket(AF_UNIX,SOCK_STREAM,0);  //UNIX 域的流式socket

    //命名
    server_address.sun_family=AF_UNIX;
    strcpy(server_address.sun_path,"server_socket");
    server_len=sizeof(server_address);
    bind(server_sockfd,(struct sockaddr*)&server_address,server_len);

    //创建一个连接队列，开始等待客户进行连接

    listen(server_sockfd, 5);
    while(1) {
        char ch;

        printf("server waiting\n");
        //接受一个连接
        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd,(struct sockaddr*)&client_address,&client_len);

        //对client_socket进行读写操作
        read(client_sockfd,&ch,1);  //从client读出来
        ch++;  //把A变成B

        write(client_sockfd, &ch, 1); //写回去
        close(client_sockfd);
    }
}
*/


/*
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = 9704;
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);


    listen(server_sockfd, 5);
    while (1) {
        char ch;
        printf("Waiting for client to receive...\n");

        //接受一个连接
        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd,
            (struct sockaddr *)&client_address, &client_len);

        read(client_sockfd, &ch, 1);
        ch++;
        write(client_sockfd, &ch, 1);
        close(client_sockfd);
    }

}
*/



//多客户连接同一个服务器


#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>


int main(){
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9734);
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

    //创建一个连接队列，忽略子进程的退出细节，等待客户的到来
    listen(server_sockfd, 5);

    signal(SIGINT, SIG_DFL);
    while(1) {
        char ch;
        printf("sserver waiting");


        //接受连接
        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd,
            (struct sockaddr *)&client_address, &client_len);

        //通过fork调用为这个用户创建一个子进程
        if(fork() == 0) {//测试是父or子

            read(client_sockfd, &ch, 1);
            sleep(5);
            ch++;
            write(client_sockfd, &ch, 1);
            close(client_sockfd);
            exit(0);

        }else {
            close(client_sockfd); //fu
        }
    }
}
