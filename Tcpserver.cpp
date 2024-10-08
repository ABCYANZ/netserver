#include"Tcpserver.h"


Tcpserver::Tcpserver(const std::string&ip,uint16_t port,int threadsize):sockfd_(new Acceptor(loop_.get(),ip,port)),ThreadPools_(threadsize,"IO"),loop_(new Eventloop(true))
{
    sockfd_->SetConnectionManage(std::bind(&Tcpserver::AddConnetion,this, std::placeholders::_1));
    for(int i=0;i<threadsize;i++)
    {
        loops_.emplace_back(new Eventloop(false));
        ThreadPools_.AddTask(std::bind(&Eventloop::run,loops_[i].get()));
        loops_[i]->setclosecallback(std::bind(&Tcpserver::CloseConnection,this, std::placeholders::_1));
    }
}
Tcpserver::~Tcpserver()
{
    //delete sockfd_;
}

void Tcpserver::start()
{
    loop_->run();
    return ;
}
void Tcpserver::stop()
{
    loop_->stop();
    for(auto &a:loops_)
    a->stop();
    ThreadPools_.stop();
}
void Tcpserver::AddConnetion(Socket* clientfd)
{
    int fd = clientfd->fd();
    spConnection con(new Connection(loops_[fd%loops_.size()].get(), clientfd));
    
    loops_[fd%loops_.size()]->AddConnection(con);
    conn_[fd] = con;
    
    // 设置回调
    conn_[fd]->setclosecallback(std::bind(&Tcpserver::CloseConnection,this, std::placeholders::_1));
    conn_[fd]->setonmessagcallback(std::bind(&Tcpserver::onmessage,this, std::placeholders::_1,std::placeholders::_2));
    conn_[fd]->setsendcallback(std::bind(&Tcpserver::WriteCallbac,this, std::placeholders::_1));
    conn_[fd]->seterrorcallback(std::bind(&Tcpserver::ErrorConnection,this, std::placeholders::_1));
    if(newConnetioncallback_)
    newConnetioncallback_(conn_[fd]);
    
}

void Tcpserver::CloseConnection(spConnection clientfd)
{
    std::unique_lock connmu(mu_);
    conn_.erase(clientfd->fd());
    if(closecallback_)
    closecallback_(clientfd);
    //delete clientfd;
    
}
void Tcpserver::onmessage(spConnection clientfd,std::string buff)
{ //std::cout<<"onmessage\n";
    readcallback_(clientfd,buff);
    //std::this_thread::sleep_for(std::chrono::seconds(1)); 
}
void Tcpserver::WriteCallbac(spConnection clientfd)
{
    if(sendcallback_)
    sendcallback_(clientfd);
}
void Tcpserver::ErrorConnection(spConnection clientfd)
{
    if(errorcallback_)
    errorcallback_(clientfd);
}

void Tcpserver::setEchoClose(std::function<void(spConnection)> closecallback)
{
    closecallback_=closecallback;
}
void Tcpserver::setEchoMessage(std::function<void(spConnection,std::string)> readcallback)
{
    readcallback_=readcallback;
}
void Tcpserver::setEchoError(std::function<void(spConnection)> errorcallback)
{
    errorcallback_=errorcallback;
}
void Tcpserver::setEchoNewConnection(std::function<void(spConnection)> newConnetioncallback)
{
    newConnetioncallback_=newConnetioncallback;
}
void Tcpserver::setEchoSend(std::function<void(spConnection)> sendcallback)
{
    sendcallback_=sendcallback;
}