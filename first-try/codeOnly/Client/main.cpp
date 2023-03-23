//
//  main.cpp
//  Client
//
//  Created by 김세인 on 2023/03/20.
//

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        cerr << "Could not create socket" << endl;
        return 1;
    }
    
    sockaddr_in server;
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // change to your server IP address
    server.sin_family = AF_INET;
    server.sin_port = htons(8888); // change to your desired port number
    
    if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0) {
        cerr << "Connection error" << endl;
        return 1;
    }
    
    char message[2000];
    while (true) {
        cout << "Enter message: ";
        cin.getline(message, 2000);
        
        if (send(sock, message, strlen(message), 0) < 0) {
            cerr << "Send failed" << endl;
            return 1;
        }
        
        memset(message, 0, sizeof(message));
        
        if (recv(sock, message, 2000, 0) < 0) {
            cerr << "Receive failed" << endl;
            break;
        }
        
        cout << "Server reply: " << message << endl;
    }
    
    close(sock);
    
    return 0;
}
