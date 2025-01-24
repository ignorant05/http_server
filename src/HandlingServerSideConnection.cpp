//
// Created by ignorant05 on 1/21/25.
//

#include "HandlingServerSideConnection.h"

#include <unistd.h>
#include <spdlog/spdlog.h>

void HandlingServerSideConnection::HandlingConnection() {
    try {
        CreateSocket();
        spdlog::info("ᕙ(`▽´)ᕗ : Socket created Successfully ;)) ");

        BindSocket();
        spdlog::info("ᕙ( ^ₒ^ ᕙ) : Socket Has been bound  ");
        spdlog::info("( ～'ω')～ : Server running at http://localhost:8080 ...");
        spdlog::info("<(^,^)> : Connection established");

        Listen();
        spdlog::info("( ～'ω')～ : Listening on http://localhost:8080 ...");

        AcceptConnection();
    }
    catch (const std::runtime_error& e) {
        close(ServerSocket);
        close(ClientSocket);
        spdlog::info("٩(ŏ﹏ŏ、)۶ : Closing connection, c ya l8r ~~ ");
        spdlog::error("Error: {}", e.what());
        exit(EXIT_FAILURE);
    }
}
