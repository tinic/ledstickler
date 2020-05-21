#include "./datagram.h"

namespace ledstickler {

#ifdef WIN32
WSAData datagram_socket::wsaData = { 0 };
bool datagram_socket::s_init = false;
#endif  // #ifdef WIN32

void datagram_socket::bind(uint16_t port, uint32_t address, bool broadcast, bool reusesock) {
    if (init) {
        return;
    }

#ifdef WIN32
    if (!s_init) {
        s_init = true;
        (void)WSAStartup(MAKEWORD(2,2),&wsaData);
    }
#endif  // #ifdef WIN32
    
    sockaddr_in addr;
    sock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    memset(&outaddr,0,sizeof(outaddr));
    outaddr.sin_family = AF_INET;
    outaddr.sin_addr.s_addr = address;
    outaddr.sin_port = htons(port);

#ifdef WIN32
    bool bOptVal = 1;
    int bOptLen = sizeof(bool);
#else  // #ifdef WIN32
    int OptVal = 1;
#endif  // #ifdef WIN32

    if (broadcast) {
#ifdef WIN32
        (void)LLsetsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&bOptVal, bOptLen);
#else  // #ifdef WIN32
        (void)::setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &OptVal, sizeof(OptVal));
#endif  // #ifdef WIN32
    }
    
    if (reusesock) {
#ifdef WIN32
        (void)::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&bOptVal, bOptLen);
#else  // #ifdef WIN32
        (void)::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &OptVal, sizeof(OptVal));
#endif  // #ifdef WIN32
    }

    (void)::bind(sock,(struct sockaddr *)&addr,sizeof(addr));

    init = true;
}

ssize_t datagram_socket::send(const uint8_t *data, size_t len) const {
    return ::sendto(sock, data, len, 0, (struct sockaddr *)&outaddr,sizeof(outaddr));
}

};

