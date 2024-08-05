#include"ThreadPools.h"


ThreadPools::ThreadPools(int threadsize):stop_(false)
{
    for(int i=0;i<threadsize;i++)
    {
        polls_.emplace_back([this]
        {
            std::function<void()> task;
            {
                std::unique_lock mu(mu_);

                while(true)
                {
                    condition_.wait(mu);
                    if(task_.size()>0)
                    {
                        task=std::move(task_.front());task_.pop();
                        break;
                    }
                    if(stop_==true)return;
                }
            }
            task();
        });
    }
}
ThreadPools::~ThreadPools()
{
    stop();
}

void ThreadPools::AddTask(std::function<void()> task)
{
    {
    std::lock_guard mu(mu_);
    task_.push(task);
    }
    condition_.notify_one();
    
}

size_t ThreadPools::size()
{
    return polls_.size();
}

void ThreadPools::stop()
{
    if (stop_) return;
    stop_=true;

    condition_.notify_all();
    for(int i=0;i<polls_.size();i++)
    {
        polls_[i].join();
    }
}