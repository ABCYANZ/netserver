#include"Eventloop.h"
#include <iostream>
Eventloop::Eventloop(int timeout):ep_(new Epoll()),timeout_(timeout),threadid_(std::this_thread::get_id()),efd_(eventfd(0, EFD_NONBLOCK)), efdch_(new channel(this,efd_))
{
   
}
Eventloop::~Eventloop()
{

}

void Eventloop::run()
{
    std::vector<channel*> ev ;
    std::cout<<"E\n";
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
    std::cout<<"8\n";
    conn_[conn->fd()]=conn;
}
bool Eventloop::CurrentLoop()
{
    return threadid_==std::this_thread::get_id();
}
void Eventloop::Addtask(std::function<void(std::string)> task)
{
    std::unique_lock mu(mu_);
    task_.push(task);
}