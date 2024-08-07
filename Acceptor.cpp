#include"Acceptor.h"

Acceptor::Acceptor(Eventloop *loop, const std::string&ip, uint16_t port):loop_(loop),sockfd_(new Socket(Nonblocking())),ch_(new channel(loop_,sockfd_->fd()))
{
    NetAddress addr(ip.c_str(),port);
    sockfd_->setreuseport(true);
    sockfd_->setkeepalive(true);
    sockfd_->setnodelay(true);
    sockfd_->setreuseaddr(true);

    sockfd_->bind(addr);
    sockfd_->listen();

    //ch_=new channel(loop_,fd_.fd());
    ch_->SetReadConnect(std::bind(&Acceptor::newConnection,this));

    ch_->StartReading();
}
Acceptor::~Acceptor()
{
    //delete ch_;
}

void Acceptor::newConnection()
{
    NetAddress addr;
    //Socket* clientfd = new Socket(sockfd_->accept(addr));
    std::unique_ptr<Socket> clientfd(new Socket(sockfd_->accept(addr)));
    clientfd->setipprot(addr.ip(),addr.port());
    //Connection* ch=new Connection(loop_,clientfd);
    ConnectionManage_(move(clientfd));
    
    return ;
}

void Acceptor::SetConnectionManage(std::function<void(std::unique_ptr<Socket>)> ConnectionManage)
{
    ConnectionManage_=ConnectionManage;
}
bool Acceptor::chbool()
{
    return ch_.use_count();
}