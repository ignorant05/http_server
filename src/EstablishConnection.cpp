//
// Created by ignorant05 on 1/21/25.
//

#include "EstablishConnection.h"

#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <spdlog/spdlog.h>
#include "sys/socket.h"
#include "netinet/in.h"
#include <thread>

// Default constants, they are adjustable btw
const int MAX_CLIENTS = 10;
const int BUFFER_SIZE = 1024 ;
const int PORT = 8080 ;

// Multithreadibg variables
std::mutex client_mutex;
std::vector<std::thread> ClientThreads;

int EstablishConnection::ServerSocket = -1;
int EstablishConnection::ClientSocket = -1;

int EstablishConnection::CreateSocket() {
    ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ServerSocket < 0) {
        throw std::runtime_error("(╥﹏╥) : Failed to Create socket!");
    }
    return ServerSocket;
}

void EstablishConnection::BindSocket() {
    sockaddr_in ServerAddress{};
    memset(&ServerAddress, 0, sizeof(ServerAddress));
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_addr.s_addr = INADDR_ANY;
    ServerAddress.sin_port = htons(PORT);

    if (bind(ServerSocket, reinterpret_cast<sockaddr*>(&ServerAddress), sizeof(ServerAddress)) < 0) {
        throw std::runtime_error("(╥﹏╥) : Failed to Bind socket!");
    }
}

void EstablishConnection::Listen() {
    if (listen(ServerSocket, 5) < 0) {
        spdlog::error("(╥﹏╥) : Listening Failed!");
        close(ServerSocket);
        throw std::runtime_error("(╥﹏╥) : Listening Failed!");
    }
}

void EstablishConnection::AcceptConnection() {
    sockaddr_in ClientAddress{};
    socklen_t ClientAddressLength = sizeof(ClientAddress);

    while (true) {
        ClientSocket = accept(ServerSocket, reinterpret_cast<sockaddr*>(&ClientAddress), &ClientAddressLength);
        if (ClientSocket < 0) {
            spdlog::error("(╥﹏╥) : Could not accept connection");
            continue;
        }
        {
            std::lock_guard<std::mutex> lock(client_mutex);
            if (ClientThreads.size() >= MAX_CLIENTS) {
                spdlog::error("(╥﹏╥) : Maximum clients reached. Closing connection." );
                close(ClientSocket);
                continue;
            }
            char Buff[BUFFER_SIZE];
            ssize_t bytes_read = read(ClientSocket, Buff, sizeof(Buff) - 1);
            if (bytes_read > 0) {
                Buff[bytes_read] = '\0';
                ClientThreads.emplace_back(handle_request, ClientSocket, Buff);
            } else {
                spdlog::error("(╥﹏╥) : Reading from socket failed!");
                close(ClientSocket);
            }
        }
        for (auto it = ClientThreads.begin(); it != ClientThreads.end();) {
            if (it->joinable()) {
                it->join();
                it = ClientThreads.erase(it);
            } else {
                ++it;
            }
        }
    }
}