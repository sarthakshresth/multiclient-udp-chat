#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>

#define PORT 8080

using namespace std;

int client_fd;
sockaddr_in serverAddr;

// thread 1
void sendMsg(){
    char buffer[1024];
    while(1){
        cout << "You: ";
        cin.getline(buffer, 1024);

        sendto(client_fd, buffer, strlen(buffer), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
    }
}

// thread 2
void recieveMsg(){
    char buffer[1024];

    while (true) {
        sockaddr_in from;
        socklen_t len = sizeof(from);

        int n = recvfrom(client_fd, buffer, sizeof(buffer), 0, (sockaddr*)&from, &len);
        // recvfrom(socket, buffer, size, flags, address, address_len)

        if (n < 0) continue;

        buffer[n] = '\0';
        cout << "\nServer: " << buffer << endl;
    }
}

int main(){
    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(client_fd < 0){
        cout<<"socket creation failed"<<'\n';
        return 0;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    // Internet Presentation → Network
    // inet_pton(address_family, ip_string, destination)
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        cout << "Invalid address\n";
    }

    // client needs server ka ip address
    cout<<"client connected to server... type messsages... \n";

    thread t1(sendMsg);
    thread t2(recieveMsg);

    t1.join();
    t2.join();

    close(client_fd);

}