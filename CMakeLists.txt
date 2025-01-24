cmake_minimum_required(VERSION 3.30)
project(http_server)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES main.cpp
        src/EstablishConnection.cpp
        src/HandlingServerSideConnection.cpp
        src/RespondingToClient.cpp
)

add_executable(http_server ${SOURCES})

target_include_directories(http_server PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/lib
)
