//
// Created by ignorant05 on 1/21/25.
//

#ifndef ESTABLISH_CONNECTION_H
#define ESTABLISH_CONNECTION_H

#include "RespondingToClient.h"

class EstablishConnection : public RespondingToClient {
    public:
        static int ServerSocket;
        static int ClientSocket;

        static int CreateSocket();
        static void BindSocket();
        static void Listen();
        static void AcceptConnection();
};

#endif // ESTABLISH_CONNECTION_H