#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <arpa/inet.h>
#define PORT 1234
#define MAXDATASIZE 100

char sendbuf[1024];
char recvbuf[1024];
char name[100];
int fd;

void pthread_recv(void* ptr){
    while(1){
        if((recv(fd,recvbuf,MAXDATASIZE,0)) == -1){ 
            printf("recv() error\n"); 
            exit(1); 
        } 
        printf("Get:\n%s\n",recvbuf);
        memset(recvbuf,0,sizeof(recvbuf));
    }
}

int main(int argc, char *argv[]){ 
    int  numbytes;   
    char buf[MAXDATASIZE];   
    struct hostent *he;       
    struct sockaddr_in server;  
    if(argc !=2){
        printf("Usage: %s <IP Address>\n",argv[0]); 
        exit(1); 
    } 
    if((he=gethostbyname(argv[1]))==NULL){  
        printf("gethostbyname() error\n"); 
        exit(1); 
    } 
    if((fd=socket(AF_INET, SOCK_STREAM, 0))==-1){ 
        printf("socket() error\n"); 
        exit(1); 
    }
    bzero(&server,sizeof(server));

    server.sin_family = AF_INET; 
    server.sin_port = htons(PORT); 
    server.sin_addr = *((struct in_addr *)he->h_addr);  
    if(connect(fd, (struct sockaddr *)&server,sizeof(struct sockaddr))==-1){  
        printf("connect() error\n"); 
        exit(1); 
    } 
    printf("connect success\n");
    char str[]=" have come in\n";
    printf("Pleace ENTER your name：");
    fgets(name,sizeof(name),stdin);
    send(fd,name,(strlen(name)-1),0);
    send(fd,str,(strlen(str)),0);

    pthread_t tid;
    pthread_create(&tid,NULL,(void*)pthread_recv,NULL);

    while(1){
        memset(sendbuf,0,sizeof(sendbuf));
        // input
        fgets(sendbuf,sizeof(sendbuf),stdin);
        
        if(strcmp(sendbuf,"exit")==0){
            memset(sendbuf,0,sizeof(sendbuf));
            printf("You have Quit.\n");
            send(fd,sendbuf,(strlen(sendbuf)),0);
            break;
        }
        send(fd,sendbuf,(strlen(sendbuf)),0);
        printf("send:\n%s\n",sendbuf);
    }
    close(fd);  
 }