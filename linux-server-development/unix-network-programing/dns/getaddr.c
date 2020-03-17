#include "comm.h"


int main(int argc, char *argv[]) 
{
    if (argc != 3)
    {
        Usage(argv[0]);
        return 0;
    }
    struct addrinfo  *result = NULL;

    result = Host_serv(argv[1], argv[2], AF_UNSPEC, SOCK_STREAM);
    
    ShowAddrInfo(result);
    
    freeaddrinfo(result);
}