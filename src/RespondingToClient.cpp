//
// Created by ignorant05 on 1/20/25.
//

#include "RespondingToClient.h"

#include "spdlog/spdlog.h"
#include "cstring"
#include <unordered_map>
#include <unordered_set>

using RequestHandler = std::function<void(std::string& Header, std::string& Body)>;

void HandlingGET(std::string& header, std::string& body);
void HandlingPOST(std::string& header, std::string& body);
void HandlingPUT(std::string& header, std::string& body);
void HandlingDELETE(std::string& header, std::string& body);
void HandlingPATCH(std::string& header, std::string& body);
void HandlingOPTIONS(std::string& header, std::string& body);

void HandlingGET (std::string& Header, std::string& Body){
    Header = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
    Body = "Hello, this is a GET response!";
}

void HandlingPOST (std::string& Header, std::string& Body) {
    Header = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
    Body = "Hello, this is a POST response!";
}

void HandlingPUT(std::string& Header, std::string& Body) {
    Header = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
    Body = "Hello, this is a PUT response!";
}

void HandlingDELETE(std::string& Header, std::string& Body){
    Header = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
    Body = "Hello, this is a DELETE response!";
}

void HandlingPATCH(std::string& Header, std::string& Body){
    Header = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
    Body = "Hello, this is a PATCH response!";
}

void HandlingOPTIONS(std::string& Header, std::string& Body) {
    Header = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
    Body = "Hello, this is a OPTIONS response!";
}

void PermissionDeniedHandling(std::string& Header, std::string& Body) {
    Header = "HTTP/1.1 301 Permission Denied! \r\nContent-Type: text/plain\r\n\r\n";
    Body= "Permission Denied";
}
void NotFoundErrorHandling(std::string& Header, std::string& Body){
    Header = "HTTP/1.1 404 Invalid Method/Path! \r\nContent-Type: text/plain\r\n\r\n";
    Body= "Invalid Method/Path";
}

void RespondingToClient::handle_request(int ClientSocket, const char *request) {
    std::string Header ;
    std::string Body ;

    std::unordered_map<std::string, RequestHandler> MethodsHandle = {
        {"GET", HandlingGET},
        {"POST", HandlingPOST},
        {"PUT", HandlingPUT},
        {"DELETE", HandlingDELETE},
        {"PATCH", HandlingPATCH},
        {"OPTIONS", HandlingOPTIONS}
    };

    // The fpllowing sets are just an example of (un)defined paths so you can play with them ;)
    std::unordered_set<std::string> PermittedToNonEmployeePathsList = {
    "/", "/login", "/index.html", "/index.php", "/main.php", "/index.js", "/index.json"
    };

    std::unordered_set<std::string> NotPermittedToNonEmployeePathsList = {
    "/admin", "/phpmyadmin", "/resources", "/payment.php", "main.cpp", "/code"
    };

    spdlog::info("Successfully received request.");

    char Method[10], Path[100];
    sscanf(request, "%s %s", Method, Path);

    std::unordered_map<std::string, RequestHandler>::iterator MethodExistanceIndicator = MethodsHandle.find(Method);

    std::unordered_set<std::string>::iterator FoundInPermittedToNonEmployeePaths = PermittedToNonEmployeePathsList.find(Path);

    std::unordered_set<std::string>::iterator FoundInNotPermittedToNonEmployeePaths = NotPermittedToNonEmployeePathsList.find(Path);

    if (
        (MethodExistanceIndicator != MethodsHandle.end()) &&
        (FoundInPermittedToNonEmployeePaths== PermittedToNonEmployeePathsList.end()) &&
        (FoundInNotPermittedToNonEmployeePaths != NotPermittedToNonEmployeePathsList.end())
        )
        PermissionDeniedHandling(Header, Body);

    else if(
        (MethodExistanceIndicator != MethodsHandle.end()) &&
        (FoundInPermittedToNonEmployeePaths!= PermittedToNonEmployeePathsList.end()) &&
        (FoundInNotPermittedToNonEmployeePaths == NotPermittedToNonEmployeePathsList.end())
        )
        MethodsHandle[Method](Header, Body);
    else NotFoundErrorHandling(Header, Body);


    char *ResponseHeader = new char[Header.length()+1];
    char *ResponseBody= new char[Body.length()+1];

    ResponseHeader= strcpy(ResponseHeader, Header.c_str());
    ResponseBody = strcpy(ResponseBody, Body.c_str());

    write(ClientSocket, ResponseHeader, strlen(ResponseHeader));
    write(ClientSocket, ResponseBody, strlen(ResponseBody));

    delete[] ResponseHeader;
    delete[] ResponseBody;

    close(ClientSocket);
}
