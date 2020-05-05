// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 
int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server";
    uid_t id = 65534;	
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }

	// Privilege Separation
	pid_t fid = fork();
	//printf("Debug: fork id is %d.\n",fid);
	
	// Child Process
	if (fid == 0){
	   printf("-------------------------------\n");
	   printf("Child process in progress\n");
		
	   // Drop privilege to "nobody" user
	   setuid(id);
	   //printf("Debug: child real ID is %d\n", getuid());
	   printf("Child process privilege dropped\n");
	   printf("-------------------------------\n");

       // Process data from client
	   valread = read( new_socket , buffer, 1024); 
	   printf("%s\n",buffer ); 
	   send(new_socket , hello , strlen(hello) , 0 ); 
	   printf("Hello message sent\n"); 

	   printf("-------------------------------\n");		
	   printf("Child process complete\n");
	}
	// Parent Process
	else{
	   int status = 0;
	   // Wait for child process to exit before proceeding
	   while ((wait(&status) > 0));
	   printf("Parent process complete\n");
	}
	
	//printf("Exiting program\n");
    return 0;
	
} 
