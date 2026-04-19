#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <bits/stdc++.h>

using namespace std;

int main(){
    int server_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if(server_fd < 0){
        cout<<"socket creation failed \n"; 
        return 0;
    }
}