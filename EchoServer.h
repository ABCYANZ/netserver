#include "Tcpserver.h"

class EchoServer
{
private:
    Tcpserver server_;
    ThreadPools JobThread_;
    
public:
    EchoServer(const std::string&ip,uint16_t port,int threadsize,int jobsize);

    void Start();//事件循环

    void Stop();//退出程序

    void EchoClose(spConnection ction);//在Tcpserver中回调
    void EchoMessage(spConnection ction,std::string buff);//在Tcpserver中回调,将onmessage函数加入到线程池的任务队列中
    void EchoError(spConnection ction);//在Tcpserver中回调
    void EchoNewConnection(spConnection ction);//在Tcpserver中回调
    void EchoSend(spConnection ction);//在Tcpserver中回调

    void onmessage(spConnection clientfd,std::string buff);//处理请求的函数,添加到线程池的任务队列
    ~EchoServer();
};