#ifndef _DATAGRAM_H_
#define _DATAGRAM_H_

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#define TRUE 1
#define FALSE 0
#endif

namespace ledstickler {

    class datagram_socket {
    public:
        datagram_socket(uint16_t port, bool broadcast = false, bool reusesock = false);
        ~datagram_socket();
        ssize_t sendTo(uint32_t address, const uint8_t *data, size_t len) const;

#ifdef WIN32
        static WSAData wsaData;
        static bool s_init;
        SOCKET sock = 0;
#else  // #ifdef WIN32
        int sock = 0;
#endif  // #ifdef WIN32

    private:
        uint16_t port;
        bool init = false;
    };

};

#endif  // #ifndef _DATAGRAM_H_
