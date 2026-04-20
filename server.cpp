#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>

#define PORT 8080

using namespace std;

// Store all clients
vector<sockaddr_in> clients;

// Function to check if client already exists
bool isNewClient(sockaddr_in &client) {
    for (auto &c : clients) {
        if (c.sin_addr.s_addr == client.sin_addr.s_addr &&
            c.sin_port == client.sin_port) {
            return false;
        }
    }
    return true;
}

int main() {
    int server_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (server_fd < 0) {
        cout << "socket creation failed\n";
        return 0;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cout << "bind failed\n";
        return 0;
    }

    cout << "Server started on port " << PORT << endl;

    char buffer[1024];

    while (true) {
        sockaddr_in clientAddr;
        socklen_t len = sizeof(clientAddr);

        int n = recvfrom(server_fd, buffer, sizeof(buffer), 0,
                         (sockaddr*)&clientAddr, &len);

        if (n < 0) continue;

        buffer[n] = '\0';

        // Convert client info
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
        int clientPort = ntohs(clientAddr.sin_port);

        cout << "[" << clientIP << ":" << clientPort << "] " << buffer << endl;

        // Add new client
        if (isNewClient(clientAddr)) {
            clients.push_back(clientAddr);
        }

        // Broadcast to all clients
        for (auto &client : clients) {
            sendto(server_fd, buffer, strlen(buffer), 0,
                   (sockaddr*)&client, sizeof(client));
        }
    }

    close(server_fd);
}