/*
	This is a simple echo server.  This demonstrates the steps to set up
	a streaming server.
 
 //Run using gcc Project3Server and run a.out
 //Telnet is telnet localhost 28900


 C socket server example
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>    
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <unistd.h>   
#include <curl/curl.h> 
#include <sys/select.h>

int main(int argc , char *argv[])
{
    char *whois = "Who is this?";
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
    fd_set readfds;
//  CURL *curl;
//  CURLcode res;
      
    //curl_global_init(CURL_GLOBAL_DEFAULT);
    
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(28900);
    
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
    
    //Listen
    listen(socket_desc , 3);
    
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");
    
    //your username with a space at the end -- don't delete the ending space
    char id_space[30];
    strcat(id_space, "jwada ");

    //Receive a message from client   
    while((read_size = recv(client_sock , client_message , 2000 , 0)) > 0)
    {
        if (strcmp(client_message,whois) != 0){
            //run terminal command to get your network location (access point)
            FILE *cmd = popen("airport -I |grep BSSID|awk '{print $2}'", "r");
            //send username + " " to the connected client
            write(client_sock, id_space, strlen(id_space));
            printf("Username and network location have been sent\n");
            //buf = allocated buffer for our network location
            char buf[18];
            while (fgets(buf, sizeof(buf), cmd) != NULL)
            {
                //send your network location to the connected client
                write(client_sock, buf, strlen(buf));
            }
            pclose(cmd);
        }
    }
    
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
    
    return 0;
}
