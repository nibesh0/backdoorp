#include <iostream>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

class Client {
public:
    Client(const std::string& server_ip, const std::string& port) {
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            throw std::runtime_error("WSAStartup failed: " + std::to_string(iResult));
        }

        sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sockfd == INVALID_SOCKET) {
            WSACleanup();
            throw std::runtime_error("Socket creation failed: " + std::to_string(WSAGetLastError()));
        }

        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(std::stoi(port));
        server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());

        if (server_addr.sin_addr.s_addr == INADDR_NONE) {
            closesocket(sockfd);
            WSACleanup();
            throw std::runtime_error("Invalid address.");
        }

        iResult = connect(sockfd, (SOCKADDR*)&server_addr, sizeof(server_addr));
        if (iResult == SOCKET_ERROR) {
            closesocket(sockfd);
            sockfd = INVALID_SOCKET;
            WSACleanup();
            throw std::runtime_error("Unable to connect to server!");
        }
    }

    void run() {
        std::string command;

        while (true) {
            std::cout << "reQ: ";
            std::getline(std::cin, command);

            if (command == "..out") {
                break;
            }

            send(sockfd, command.c_str(), command.length(), 0);

            char buffer[1024];
            int bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received > 0) {
                buffer[bytes_received] = '\0'; 
                std::cout << "re$: " << buffer << std::endl;
            } else {
                std::cerr << "Connection closed by server." << std::endl;
                break;
            }
        }

        closesocket(sockfd);
    }

    ~Client() {
        closesocket(sockfd);
        WSACleanup();
    }

private:
    SOCKET sockfd;
};

int main(int argc, char *argv[]) {
    if( argc < 2 ) {
        std::cout << "error Ip";
        return 0;
    }
    try {
        Client client(argv[1], "6969");
        client.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
