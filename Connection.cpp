#include"Connection.h"

Connection::Connection(Eventloop*loop,std::unique_ptr<Socket>clientfd)
{
    loop_=loop;
    ch_=move(std::unique_ptr<channel>(new channel(loop,clientfd->fd())));
    clientfd_=(move(clientfd));
    if(loop_)
    
    ch_->Edge();
    ch_->SetReadConnect(std::bind(&Connection::onmessage,this));
    
    ch_->SetColseConnect(std::bind(&Connection::CloseConnection,this));
    ch_->SetSendConnect(std::bind(&Connection::WriteCallback,this));
    ch_->StartReading();
}
Connection::~Connection()
{
    std::cout<<"释放connection\n";
}
void Connection::onmessage()
{
    Stamp_.updateStamp();
    char buffer[1024] = {0};
    while(1)
    {
        ssize_t bytes_read = read(fd(), buffer, 1024);
        if (bytes_read >0) 
        {
            rbuff_.append(buffer);
        }
        else if(bytes_read==-1&&(errno==EAGAIN || errno==EWOULDBLOCK))
        {
            rbuff_.substr(buffer);
            if(onmessagcallback_)
            onmessagcallback_(shared_from_this(),buffer);
           
            break;
        }
        else if(bytes_read==-1&&errno==EINTR)
        {
            continue;
        }
        else if(bytes_read==0)
        {
            closecallback_(shared_from_this());
            break;
        }
  
    }       
}
void Connection::send(const std::string buff)
{
    if(loop_->CurrentLoop())
    {
        sendLoop(buff);
    }
    else
    {
        loop_->Addtask(std::bind(&Connection::sendLoop,this,buff));
    }

}
void Connection::sendLoop(const std::string buff)
{
    Stamp_.updateStamp();
    wbuff_.HeadAppend(buff);
    ch_->Enablewriting();
}
void Connection::WriteCallback()
{
    Stamp_.updateStamp();
    int len = ::send(fd(),wbuff_.data(),wbuff_.size(),0);
    if(len>0)
    {
        wbuff_.erase(0,len);
    }
    if(wbuff_.size()==0)
    {   
        ch_->CloseWrite(); 
        //sendcallback_(shared_from_this());
    }
}
void Connection::ErrorConnection()
{
    errorcallback_(shared_from_this());
}
int Connection::fd()
{
    return clientfd_->fd();
}

void Connection::setclosecallback(std::function<void(spConnection)> closecallback)
{
    closecallback_=closecallback;
}
void Connection::CloseConnection()
{
    std::cout<<"9\n";
    closecallback_(shared_from_this());
    std::cout<<"8\n";
}

void Connection::setonmessagcallback(std::function<void(spConnection,std::string)> onmessagcallback)
{
    onmessagcallback_=onmessagcallback;
}
void Connection::setsendcallback(std::function<void(spConnection)> sendcallback)
{
    sendcallback_=sendcallback;
}
void Connection::seterrorcallback(std::function<void(spConnection)> errorcallback)
{
    errorcallback_=errorcallback;
}

bool Connection::Timeout(int out)
{
    return (Stamp_.now()-Stamp_.Stamp())>out;
}