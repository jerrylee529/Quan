
#ifndef _CLIENT_SOCKET_H
#define _CLIENT_SOCKET_H

#include "Socket.h"


class ClientSocket : public Socket
{
public:
    ClientSocket() : Socket(), _noQued(0)
    {
    }

    ClientSocket(long noQued) : Socket(), _noQued(noQued)
    {

    }

    ClientSocket(ClientSocket &sock) : Socket((Socket &)sock), _noQued(0)
    {
        _noQued = sock._noQued;
    }

    virtual ~ClientSocket()
    {
        close();
    }

    // connect methods
    bool connect(const string &host, const long port)
    {
        // get socket
        _socket = socket(PF_INET,SOCK_STREAM,0);

        if (_socket == INVALID_SOCKET)
            return false;

        // init address in
        _port = port;
        initSockAddrIn(host, port, _addrIn);

        // connect to socket
        if (::connect(_socket, (const SOCKADDR *)&_addrIn, sizeof(_addrIn)) == SOCKET_ERROR)
        {
            int error = getLastError();
            closesocket(_socket);
            return false;
        }


        return true;
    }

protected:
    long    _noQued;
};

#endif // _CLIENT_SOCKET_H