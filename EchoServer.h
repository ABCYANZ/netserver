#include "Tcpserver.h"

class EchoServer
{
private:
    Tcpserver server_;
    ThreadPools JobThread_;
    
public:
    EchoServer(const std::string&ip,uint16_t port,int threadsize,int jobsize);

    void Start();

    void Stop();

    void EchoClose(spConnection ction);
    void EchoMessage(spConnection ction,std::string buff);
    void EchoError(spConnection ction);
    void EchoNewConnection(spConnection ction);
    void EchoSend(spConnection ction);

    void onmessage(spConnection clientfd,std::string buff);

    ~EchoServer();
};