#include "comm.h"

void Usage(char *arg0)
{
    printf("./%s -a|-n <para>\n", arg0);
}

void ShowHostByName(const char *name)
{
    struct hostent *hostPtr = gethostbyname(name);
    if (hostPtr == NULL)
    {
        fprintf(stderr, "%s error: %s", name, hstrerror(h_errno));
        return;
    }
    
    printf("offical name: %s\n", hostPtr->h_name);

    for (char **p = hostPtr->h_aliases; *p != NULL; p++)
    {
        printf("alias: %s\n", *p);
    }

    if (hostPtr->h_addrtype == AF_INET && hostPtr->h_length == 4)
    {
        char ipStr[IPV4_STR_ADDR_LEN];
        
        for (char **p = hostPtr->h_addr_list; *p != NULL; p++)
        {
            Inet_ntop(AF_INET, *p, ipStr, sizeof(ipStr));
            printf("ip: %s\n", ipStr);
        }
    }
}

void ShowHostByAddr(const char *ipStr, int type)
{
    struct in_addr ipAddr;
    Inet_pton(type, ipStr, &ipAddr);
    struct hostent *hostPtr = gethostbyaddr(&ipAddr, sizeof(ipAddr), type);
    if (hostPtr == NULL)
    {
        fprintf(stderr, "%s error: %s", ipStr, hstrerror(h_errno));
        return;
    }
    printf("host: %s", hostPtr->h_name);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        return 0;
    }

    if (argv[1][1] == 'a')
    {
        ShowHostByAddr(argv[2], AF_INET);
    }
    if (argv[1][1] == 'n')
    {
        ShowHostByName(argv[2]);
    }

    return 0;
}