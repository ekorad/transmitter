#include "sender.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string>
#include <iostream>

using namespace std;

static int sv_sock_fd, client_sock_fd;
static struct sockaddr_in sv_addr, client_addr;

int host(const unsigned port)
{
    int addr_len = sizeof(struct sockaddr_in);
    int opt = 1;

    if ((sv_sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0)
    {
        perror("socket");
        return -1;
    }

    if (setsockopt(sv_sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        return -1;
    }

    sv_addr.sin_family = AF_INET;
    sv_addr.sin_addr.s_addr = INADDR_ANY;
    sv_addr.sin_port = htons(port);

    if (bind(sv_sock_fd, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) < 0)
    {
        perror("bind");
        return -1;
    }

    if (listen(sv_sock_fd, 1) < 0)
    {
        perror("listen");
        return -1;
    }

    cout << "> Awaiting client connection" << endl;

    client_sock_fd = accept(sv_sock_fd, (struct sockaddr *)&client_addr, (socklen_t*)&addr_len);
    if (client_sock_fd < 0)
    {
        perror("accept");
        return -1;
    }

    return 0;
}

int sendToPeer(const DataTransferObject& dto)
{
    double arrbuf[] = {
        dto.xl,
        dto.zl,
        dto.yr
    };

    return send(client_sock_fd, arrbuf, sizeof(arrbuf), 0);
}

void shutdown()
{
    close(client_sock_fd);
    close(sv_sock_fd);
}