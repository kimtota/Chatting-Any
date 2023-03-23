//
//  main.cpp
//  Server
//
//  Created by 김세인 on 2023/03/20.
//

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <vector>

using namespace std;

vector<int> client_sockets;

void *client_handler(void *arg)
{
    int client_socket = *(int *)arg;
    char message[256];

    while (true)
    {
        memset(message, 0, sizeof(message));
        int bytes_read = read(client_socket, message, sizeof(message));
        if (bytes_read <= 0)
        {
            close(client_socket);
            pthread_exit(NULL);
        }

        cout << message << endl;

        for (int i = 0; i < client_sockets.size(); i++)
        {
            if (client_sockets[i] != client_socket)
            {
                write(client_sockets[i], message, strlen(message));
            }
        }
    }
}

int main()
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        cout << "Error creating socket" << endl;
        return -1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(12345);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        cout << "Error binding socket" << endl;
        return -1;
    }

    listen(server_socket, 5);

    while (true)
    {
        struct sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);

        if (client_socket < 0)
        {
            cout << "Error accepting client connection" << endl;
            continue;
        }

        cout << "Client connected: " << inet_ntoa(client_address.sin_addr) << endl;

        pthread_t thread_id;
        int *client_socket_ptr = new int;
        *client_socket_ptr = client_socket;
        client_sockets.push_back(client_socket);
        pthread_create(&thread_id, NULL, client_handler, (void *)client_socket_ptr);
    }

    close(server_socket);
    return 0;
}

