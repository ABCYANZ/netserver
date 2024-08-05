#include"Eventloop.h"
//#include <iostream>
Eventloop::Eventloop(int timeout):ep_(new Epoll()),timeout_(timeout),threadid_(std::this_thread::get_id()),efd_(eventfd(0, EFD_NONBLOCK)), efdch_(new channel(this,efd_))
{
    efdch_->SetReadConnect(std::bind(&Eventloop::TaskSend,this));
    efdch_->StartReading();
}
Eventloop::~Eventloop()
{

}

void Eventloop::run()
{ 
    std::vector<channel*> ev ;
    while (true) 
    {
        ev = std::move(ep_->loop());
        for(auto &a:ev)
        {

            a->Handleevent();
        }
    }
}
void Eventloop::UpdateChannel(channel* ch)
{
    ep_->UpdateChannel(ch);
}

void Eventloop::AddConnection(spConnection conn)
{
    conn_[conn->fd()]=conn;
}
bool Eventloop::CurrentLoop()
{
    return threadid_==std::this_thread::get_id();
}
void Eventloop::Addtask(std::function<void()> task)
{
    std::unique_lock mu(mu_);
    task_.push(task);
    if (eventfd_write(efd_, 1) == -1) {
        perror("eventfd_write");
    }
}
void Eventloop::TaskSend()
{
    uint64_t val;
    read(efd_,&val,sizeof(val));
    std::unique_lock mu(mu_);
    std::function<void()> task;
    while(!task_.empty())
    {
        task=task_.front();task_.pop();
        task();
    }

}