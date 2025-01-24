//
// Created by ignorant05 on 1/20/25.
//

#ifndef RESPONDING_TO_CLIENT_H
#define RESPONDING_TO_CLIENT_H

class RespondingToClient {
    public:
        static void handle_request(int client_socket, const char *request);
};

#endif //RESPONDING_TO_CLIENT_H
