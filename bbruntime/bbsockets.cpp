#include "bbsockets.h"
#include "std.h"

static bool socks_ok;
static WSADATA wsadata;
static int recv_timeout;
static int read_timeout;
static int accept_timeout;

static void close(const SOCKET sock, const int e)
{
    if (e < 0)
    {
        int opt = 1;
        setsockopt(sock, SOL_SOCKET, SO_DONTLINGER, (char*)&opt, sizeof(opt));
    }
    closesocket(sock);
}

class UDPStream;

class TCPStream;

class TCPServer;

static std::set<UDPStream*> udp_set;
static std::set<TCPStream*> tcp_set;
static std::set<TCPServer*> server_set;

class UDPStream : public bbStream
{
public:
    UDPStream(SOCKET s);

    ~UDPStream() override;

    int read(char* buff, int size) override;

    int write(const char* buff, int size) override;

    int avail() override;

    int eof() override;

    int recv();

    int send(int ip, int port);

    int getIP() const;

    int getPort() const;

    int getMsgIP() const;

    int getMsgPort() const;

private:
    SOCKET sock;
    std::vector<char> in_buf, out_buf;
    sockaddr_in addr, in_addr, out_addr;
    int in_get, e;
};

UDPStream::UDPStream(SOCKET s) : sock(s), in_get(0), e(0)
{
    int len = sizeof(addr);
    getsockname(s, (sockaddr*)&addr, &len);
    in_addr = out_addr = addr;
}

UDPStream::~UDPStream()
{
    close(sock, e);
}

int UDPStream::read(char* buff, int size)
{
    if (e) return 0;
    const int n = in_buf.size() - in_get;
    if (n < size) size = n;
    memcpy(buff, &in_buf[in_get], size);
    in_get += size;
    return size;
}

int UDPStream::write(const char* buff, const int size)
{
    if (e) return 0;
    out_buf.insert(out_buf.end(), buff, buff + size);
    return size;
}

int UDPStream::avail()
{
    if (e) return 0;
    return in_buf.size() - in_get;
}

int UDPStream::eof()
{
    return e ? e : in_get == in_buf.size();
}

//fill buffer, return sender
int UDPStream::recv()
{
    if (e) return 0;
    int tout;
    if (recv_timeout) tout = gx_runtime->getMilliSecs() + recv_timeout;
    for (;;)
    {
        int dt = 0;
        if (recv_timeout)
        {
            dt = tout - gx_runtime->getMilliSecs();
            if (dt < 0) dt = 0;
        }
        fd_set fd = {1, sock};
        timeval tv = {dt / 1000, (dt % 1000) * 1000};
        int n = ::select(0, &fd, nullptr, nullptr, &tv);
        if (!n) return 0;
        if (n != 1)
        {
            e = -1;
            return 0;
        }
        unsigned long sz = -1;
        if (ioctlsocket(sock, FIONREAD, &sz))
        {
            e = -1;
            return 0;
        }
        in_buf.resize(sz);
        in_get = 0;
        int len = sizeof(in_addr);
        n = ::recvfrom(sock, (char*)in_buf.data(), sz, 0, (sockaddr*)&in_addr, &len);
        if (n == SOCKET_ERROR) continue; //{ e=-1;return 0; }
        in_buf.resize(n);
        return getMsgIP();
    }
    return 0;
}

//send, empty buffer
int UDPStream::send(const int ip, const int port)
{
    if (e) return 0;
    const int sz = out_buf.size();
    out_addr.sin_addr.S_un.S_addr = htonl(ip);
    out_addr.sin_port = htons(port ? port : addr.sin_port);
    const int n = ::sendto(sock, (char*)out_buf.data(), sz, 0, (sockaddr*)&out_addr, sizeof(out_addr));
    if (n != sz) return e = -1;
    out_buf.clear();
    return sz;
}

int UDPStream::getIP() const
{
    return ntohl(addr.sin_addr.S_un.S_addr);
}

int UDPStream::getPort() const
{
    return ntohs(addr.sin_port);
}

int UDPStream::getMsgIP() const
{
    return ntohl(in_addr.sin_addr.S_un.S_addr);
}

int UDPStream::getMsgPort() const
{
    return ntohs(in_addr.sin_port);
}

class TCPStream : public bbStream
{
public:
    TCPStream(SOCKET s, TCPServer* t);

    ~TCPStream() override;

    int read(char* buff, int size) override;

    int write(const char* buff, int size) override;

    int avail() override;

    int eof() override;

    int getIP() const;

    int getPort() const;

private:
    SOCKET sock;
    TCPServer* server;
    int e, ip, port;
};

class TCPServer
{
public:
    TCPServer(SOCKET S);

    ~TCPServer();

    TCPStream* accept();

    void remove(TCPStream* s);

private:
    int e;
    SOCKET sock;
    std::set<TCPStream*> accepted_set;
};

TCPStream::TCPStream(const SOCKET s, TCPServer* t) : sock(s), server(t), e(0)
{
    sockaddr_in addr;
    int len = sizeof(addr);
    if (getpeername(s, (sockaddr*)&addr, &len))
    {
        ip = port = 0;
        return;
    }
    ip = ntohl(addr.sin_addr.S_un.S_addr);
    port = ntohs(addr.sin_port);
}

TCPStream::~TCPStream()
{
    if (server) server->remove(this);
    close(sock, e);
}

int TCPStream::read(char* buff, const int size)
{
    if (e) return 0;
    char* b = buff,* l = buff + size;
    int tout;
    if (read_timeout) tout = gx_runtime->getMilliSecs() + read_timeout;
    while (b < l)
    {
        int dt = 0;
        if (read_timeout)
        {
            dt = tout - gx_runtime->getMilliSecs();
            if (dt < 0) dt = 0;
        }
        fd_set fd = {1, sock};
        timeval tv = {dt / 1000, (dt % 1000) * 1000};
        int n = ::select(0, &fd, nullptr, nullptr, &tv);
        if (n != 1)
        {
            e = -1;
            break;
        }
        n = ::recv(sock, b, l - b, 0);
        if (n == 0)
        {
            e = 1;
            break;
        }
        if (n == SOCKET_ERROR)
        {
            e = -1;
            break;
        }
        b += n;
    }
    return b - buff;
}

int TCPStream::write(const char* buff, const int size)
{
    if (e) return 0;
    const int n = ::send(sock, buff, size, 0);
    if (n == SOCKET_ERROR)
    {
        e = -1;
        return 0;
    }
    return n;
}

int TCPStream::avail()
{
    unsigned long t;
    const int n = ::ioctlsocket(sock, FIONREAD, &t);
    if (n == SOCKET_ERROR)
    {
        e = -1;
        return 0;
    }
    return t;
}

int TCPStream::eof()
{
    if (e) return e;
    fd_set fd = {1, sock};
    const timeval tv = {0, 0};
    switch (::select(0, &fd, nullptr, nullptr, &tv))
    {
    case 0:
        break;
    case 1:
        if (!avail()) e = 1;
        break;
    default:
        e = -1;
    }
    return e;
}

int TCPStream::getIP() const
{
    return ip;
}

int TCPStream::getPort() const
{
    return port;
}

TCPServer::TCPServer(const SOCKET s) : sock(s), e(0)
{
}

TCPServer::~TCPServer()
{
    while (accepted_set.size()) delete *accepted_set.begin();
    close(sock, e);
}

TCPStream* TCPServer::accept()
{
    if (e) return nullptr;
    fd_set fd = {1, sock};
    const timeval tv = {accept_timeout / 1000, (accept_timeout % 1000) * 1000};
    const int n = ::select(0, &fd, nullptr, nullptr, &tv);
    if (n == 0) return nullptr;
    if (n != 1)
    {
        e = -1;
        return nullptr;
    }
    const SOCKET t = ::accept(sock, nullptr, nullptr);
    if (t == INVALID_SOCKET)
    {
        e = -1;
        return nullptr;
    }
    TCPStream* s = d_new TCPStream(t, this);
    accepted_set.insert(s);
    return s;
}

void TCPServer::remove(TCPStream* s)
{
    accepted_set.erase(s);
}

static inline void debugUDPStream(UDPStream* p)
{
    if (debug && !udp_set.count(p))
    {
        RTEX("UDP Stream does not exist");
    }
}

static inline void debugTCPStream(TCPStream* p)
{
    if (debug && !tcp_set.count(p))
    {
        RTEX("TCP Stream does not exist");
    }
}

static inline void debugTCPServer(TCPServer* p)
{
    if (debug && !server_set.count(p))
    {
        RTEX("TCP Server does not exist");
    }
}

static std::vector<int> host_ips;

int bbCountHostIPs(BBStr* host)
{
    host_ips.clear();
    const HOSTENT* h = gethostbyname(host->c_str());
    delete host;
    if (!h) return 0;
    char** p = h->h_addr_list;
    while (char* t = *p++) host_ips.push_back(ntohl(*(int*)t));
    return host_ips.size();
}

int bbHostIP(const int index)
{
    if (debug)
    {
        if (index < 1 || index > host_ips.size())
        {
            RTEX("Host index out of range");
        }
    }
    return host_ips[index - 1];
}

UDPStream* bbCreateUDPStream(const int port)
{
    if (!socks_ok) return nullptr;
    const SOCKET s = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (s != INVALID_SOCKET)
    {
        sockaddr_in addr = {AF_INET, htons(port)};
        if (!::bind(s, (sockaddr*)&addr, sizeof(addr)))
        {
            UDPStream* p = d_new UDPStream(s);
            udp_set.insert(p);
            return p;
        }
        ::closesocket(s);
    }
    return nullptr;
}

void bbCloseUDPStream(UDPStream* p)
{
    debugUDPStream(p);
    udp_set.erase(p);
    delete p;
}

int bbRecvUDPMsg(UDPStream* p)
{
    debugUDPStream(p);
    return p->recv();
}

void bbSendUDPMsg(UDPStream* p, const int ip, const int port)
{
    debugUDPStream(p);
    p->send(ip, port);
}

int bbUDPStreamIP(UDPStream* p)
{
    debugUDPStream(p);
    return p->getIP();
}

int bbUDPStreamPort(UDPStream* p)
{
    debugUDPStream(p);
    return p->getPort();
}

int bbUDPMsgIP(UDPStream* p)
{
    debugUDPStream(p);
    return p->getMsgIP();
}

int bbUDPMsgPort(UDPStream* p)
{
    debugUDPStream(p);
    return p->getMsgPort();
}

void bbUDPTimeouts(const int rt)
{
    recv_timeout = rt;
}

BBStr* bbDottedIP(const int ip)
{
    return d_new BBStr(
        itoa((ip >> 24) & 255) + "." + itoa((ip >> 16) & 255) + "." +
        itoa((ip >> 8) & 255) + "." + itoa(ip & 255));
}

static int findHostIP(const std::string& t)
{
    const int ip = inet_addr(t.c_str());
    if (ip != INADDR_NONE) return ip;
    const HOSTENT* h = gethostbyname(t.c_str());
    if (!h) return -1;
    char* p;
    for (char** list = h->h_addr_list; p = *list; ++list)
    {
        return *(int*)p;
    }
    return 0;
}

TCPStream* bbOpenTCPStream(BBStr* server, const int port, const int local_port)
{
    if (!socks_ok)
    {
        delete server;
        return nullptr;
    }
    const int ip = findHostIP(*server);
    delete server;
    if (ip == -1) return nullptr;
    const SOCKET s = ::socket(AF_INET, SOCK_STREAM, 0);
    if (s != INVALID_SOCKET)
    {
        if (local_port)
        {
            sockaddr_in addr = {AF_INET, htons(local_port)};
            if (::bind(s, (sockaddr*)&addr, sizeof(addr)))
            {
                ::closesocket(s);
                return nullptr;
            }
        }
        sockaddr_in addr = {AF_INET, htons(port)};
        addr.sin_addr.S_un.S_addr = ip;
        if (!::connect(s, (sockaddr*)&addr, sizeof(addr)))
        {
            TCPStream* p = d_new TCPStream(s, nullptr);
            tcp_set.insert(p);
            return p;
        }
        ::closesocket(s);
    }
    return nullptr;
}

void bbCloseTCPStream(TCPStream* p)
{
    debugTCPStream(p);
    tcp_set.erase(p);
    delete p;
}

TCPServer* bbCreateTCPServer(const int port)
{
    const SOCKET s = ::socket(AF_INET, SOCK_STREAM, 0);
    if (s != INVALID_SOCKET)
    {
        sockaddr_in addr = {AF_INET, htons(port)};
        if (!::bind(s, (sockaddr*)&addr, sizeof(addr)))
        {
            if (!::listen(s, SOMAXCONN))
            {
                TCPServer* p = d_new TCPServer(s);
                server_set.insert(p);
                return p;
            }
        }
        ::closesocket(s);
    }
    return nullptr;
}

void bbCloseTCPServer(TCPServer* p)
{
    debugTCPServer(p);
    server_set.erase(p);
    delete p;
}

TCPStream* bbAcceptTCPStream(TCPServer* server)
{
    debugTCPServer(server);
    if (!gx_runtime->idle())
        RTEX(0);
    if (TCPStream* tcp = server->accept())
    {
        tcp_set.insert(tcp);
        return tcp;
    }
    return nullptr;
}

int bbTCPStreamIP(TCPStream* p)
{
    debugTCPStream(p);
    return p->getIP();
}

int bbTCPStreamPort(TCPStream* p)
{
    debugTCPStream(p);
    return p->getPort();
}

void bbTCPTimeouts(const int rt, const int at)
{
    read_timeout = rt;
    accept_timeout = at;
}

bool sockets_create()
{
    socks_ok = WSAStartup(0x0101, &wsadata) == 0;
    recv_timeout = 0;
    read_timeout = 10000;
    accept_timeout = 0;
    return true;
}

bool sockets_destroy()
{
    while (udp_set.size()) bbCloseUDPStream(*udp_set.begin());
    while (tcp_set.size()) bbCloseTCPStream(*tcp_set.begin());
    while (server_set.size()) bbCloseTCPServer(*server_set.begin());
    if (socks_ok) WSACleanup();
    return true;
}

void sockets_link(void (*rtSym)(const char*, void*))
{
    rtSym("$DottedIP%IP", bbDottedIP);
    rtSym("%CountHostIPs$host_name", bbCountHostIPs);
    rtSym("%HostIP%host_index", bbHostIP);

    rtSym("%CreateUDPStream%port=0", bbCreateUDPStream);
    rtSym("CloseUDPStream%udp_stream", bbCloseUDPStream);
    rtSym("SendUDPMsg%udp_stream%dest_ip%dest_port=0", bbSendUDPMsg);
    rtSym("%RecvUDPMsg%udp_stream", bbRecvUDPMsg);
    rtSym("%UDPStreamIP%udp_stream", bbUDPStreamIP);
    rtSym("%UDPStreamPort%udp_stream", bbUDPStreamPort);
    rtSym("%UDPMsgIP%udp_stream", bbUDPMsgIP);
    rtSym("%UDPMsgPort%udp_stream", bbUDPMsgPort);
    rtSym("UDPTimeouts%recv_timeout", bbUDPTimeouts);

    rtSym("%OpenTCPStream$server%server_port%local_port=0", bbOpenTCPStream);
    rtSym("CloseTCPStream%tcp_stream", bbCloseTCPStream);
    rtSym("%CreateTCPServer%port", bbCreateTCPServer);
    rtSym("CloseTCPServer%tcp_server", bbCloseTCPServer);
    rtSym("%AcceptTCPStream%tcp_server", bbAcceptTCPStream);
    rtSym("%TCPStreamIP%tcp_stream", bbTCPStreamIP);
    rtSym("%TCPStreamPort%tcp_stream", bbTCPStreamPort);
    rtSym("TCPTimeouts%read_millis%accept_millis", bbTCPTimeouts);
}
