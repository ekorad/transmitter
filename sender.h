#ifndef SOCKET_CTRL_H
#define SOCKET_CTRL_H

#include <netinet/in.h>
#include "data_struct.h"

#define DEFAULT_PORT    1337

int host(const unsigned port = DEFAULT_PORT);
int sendToPeer(const DataTransferObject& dto);
void shutdown();

#endif