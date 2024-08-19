#include"Connection.h"

Connection::Connection(Eventloop*loop,Socket*clientfd):ch_(new channel(loop,clientfd->fd())),loop_(loop),clientfd_(clientfd)
{
    
    ch_->Edge();
    ch_->SetReadConnect(std::bind(&Connection::onmessage,this));
    ch_->SetErrorConnect(std::bind(&Connection::ErrorConnection,this));
    ch_->SetColseConnect(std::bind(&Connection::CloseConnection,this));
    ch_->SetSendConnect(std::bind(&Connection::WriteCallback,this));
    ch_->StartReading();
}
Connection::~Connection()
{
    //delete clientfd_;delete ch_;
    //std::cout<<"connection ip="<< <<"\n";
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
            rbuff_.append(buffer,bytes_read);
            std::string str;
            while(true)
            {
                if(!rbuff_.substr(str))break;
                if(onmessagcallback_)
                onmessagcallback_(shared_from_this(),str);
            }
        }
        else if(bytes_read==-1&&(errno==EAGAIN || errno==EWOULDBLOCK))
        {           
            break;
        }
        else if(bytes_read==-1&&errno==EINTR)
        {
            continue;
        }
        else if(bytes_read==0)
        {
            CloseConnection();
            break;
        }
  
    }       
}
void Connection::send(const std::string buff)
{
    if(stop_==true)return;
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
    if(stop_==true)return;
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
        sendcallback_(shared_from_this());
    }
}
void Connection::ErrorConnection()
{
    stop_=true;
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
    stop_=true;
    closecallback_(shared_from_this());
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