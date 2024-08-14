#include "EchoServer.h"


EchoServer::EchoServer(const std::string&ip,uint16_t port,int threadsize,int jobsize):server_(ip,port,threadsize),JobThread_(jobsize,"WORKS")
{
    server_.setEchoClose(std::bind(&EchoServer::EchoClose,this, std::placeholders::_1));
    server_.setEchoMessage(std::bind(&EchoServer::EchoMessage,this, std::placeholders::_1,std::placeholders::_2));
    server_.setEchoError(std::bind(&EchoServer::EchoError,this, std::placeholders::_1));
    server_.setEchoNewConnection(std::bind(&EchoServer::EchoNewConnection,this, std::placeholders::_1));
    server_.setEchoSend(std::bind(&EchoServer::EchoSend,this,std::placeholders::_1));
}
void EchoServer::Start()
{
    server_.start();
}

void EchoServer::EchoClose(spConnection ction)
{

}
void EchoServer::EchoMessage(spConnection ction,std::string buff)
{
    //std::cout<<"Echomessage\n";
    if(JobThread_.size()==0)
    onmessage(ction,buff);
    else
    JobThread_.AddTask(std::bind(&EchoServer::onmessage,this,ction,buff));
}
void EchoServer::EchoError(spConnection ction)
{

}
void EchoServer::EchoNewConnection(spConnection ction)
{

}
void EchoServer::EchoSend(spConnection ction)
{

}

void EchoServer::onmessage(spConnection clientfd,std::string buff)
{
    clientfd->send(buff);   
    //buff = "Received: " + buff;
    //std::cout<<buff<<"\n"; 

    //std::this_thread::sleep_for(std::chrono::seconds(1)); 
}

void EchoServer::Stop()
{
    server_.stop();
    JobThread_.stop();
}
EchoServer::~EchoServer()
{

}