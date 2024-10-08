#include"Eventloop.h"
//#include <iostream>

int setup_timerfd(int seconds=30) {

    int timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);

    if (timer_fd == -1) {
        perror("Failed to create timer");
        exit(EXIT_FAILURE);
    }
    struct itimerspec its;
    memset(&its, 0, sizeof(struct itimerspec));

    // 设置定时器的到期时间
    its.it_value.tv_sec = seconds;
    its.it_value.tv_nsec = 0;

    // 设置定时器的周期（这里设置为一次性定时器）
    its.it_interval.tv_sec = seconds;
    its.it_interval.tv_nsec = 0;

    if (timerfd_settime(timer_fd, 0, &its, nullptr) == -1) 
    {
        perror("Failed to set timer");
        exit(EXIT_FAILURE);
    }
    return timer_fd;
}

Eventloop::Eventloop(bool loop,int timeout,int conntimeout):ep_(new Epoll()),timeout_(timeout),timeoutfd_(setup_timerfd(timeout)),efd_(eventfd(0, EFD_NONBLOCK)), efdch_(new channel(this,efd_)),
                                    timeoutfdch_(new channel(this,timeoutfd_)),
                                    conntimeout_(conntimeout),stop_(false),loop_(loop)
{
    //closeioandsignal();
    if(!loop_)
    {
        efdch_->SetReadConnect(std::bind(&Eventloop::TaskSend,this));
        efdch_->StartReading();
        timeoutfdch_->SetReadConnect(std::bind(&Eventloop::TimeouEvent,this));
        timeoutfdch_->StartReading();
    }

}
Eventloop::~Eventloop()
{
    //delete ep_;
}

void Eventloop::run()
{ 
    threadid_=std::this_thread::get_id();
    std::vector<channel*> ev;
    while (stop_==false) 
    {
        ev = std::move(ep_->loop());
        for(auto &a:ev)
        {    
            if(a->fd()==efd_)
            {
                TaskSend();continue;
            }
            if(a->fd()==timeoutfd_)
            {
                TimeouEvent();continue;
            }

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
    std::unique_lock connmu(connmu_);
    conn_[conn->fd()]=conn;
}
bool Eventloop::CurrentLoop()
{
    return (threadid_==std::this_thread::get_id());
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
void Eventloop::TimeouEvent()
{
    
    uint64_t expirations;
    ssize_t nread = read(timeoutfd_, &expirations, sizeof(expirations));

    if (nread != sizeof(expirations)) 
    {
        perror("Failed to read from timerfd");
        close(timeoutfd_);
        exit(EXIT_FAILURE);
    }

    for(auto it=conn_.begin();it!=conn_.end();)
    {
        if(it->second->Timeout(timeout_))
        {
            closecallback_(it->second);
            std::unique_lock connmu(connmu_);
            it=conn_.erase(it);
        }
        else it++;
    }
}

void Eventloop::stop()
{
    stop_=true;
    if (eventfd_write(efd_, 1) == -1) 
    {
        perror("eventfd_write");
        if (eventfd_write(efd_, 1) == -1) 
        {
            perror("eventfd_write");
        }
    }
}
void Eventloop::setclosecallback(std::function<void(spConnection)> closecallback)
{
    closecallback_=closecallback;
}