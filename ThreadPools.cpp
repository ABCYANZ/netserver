#include"ThreadPools.h"


ThreadPools::ThreadPools(int threadsize,const std::string threadtype):stop_(false),threadtype_(threadtype)
{
    if(threadsize==0)return;
    for(int i=0;i<threadsize;i++)
    {
       
        polls_.emplace_back([this]
        {
            std::function<void()> task;
            while(stop_==false)
            {
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
                tasksize_--;
                task();
                task=nullptr;
            }
        }); 
    }

    if(threadtype_=="WORKS")
    {
        polls_.emplace_back([this]
        {
            int sleeptime_=5;
            while(stop_==false)
            {
                
                // std::cout<<"threadtype_="<<threadtype_<<"\n";
                // std::cout<<"task_size="<<task_.size()<<"\n";
                // std::cout<<"sleeptime_="<<sleeptime_<<"\n";
                // std::cout<<"tasksize_="<<tasksize_<<"\n";
                sleep(sleeptime_);
                sleeptime_+=sleeptime_;
                while(tasksize_)
                {
                    //int i=tasksize_;
                    if(tasksize_)
                    for(int j=0;j<tasksize_;j++)
                    {
                        condition_.notify_one();
                        sleeptime_ = 1;  
                        //std::cout<< "唤醒";
                    }
                    if(stop_==true)return;
                }
                if(sleeptime_==1)continue;
                
                if(sleeptime_>11)
                sleeptime_=2;

            }
        });
    }
}
ThreadPools::~ThreadPools()
{
    stop();
}

void ThreadPools::AddTask(std::function<void()> task)
{
    //std::cout<<"AddTask_"<<"\n";
    {
    std::lock_guard mu(mu_);
    task_.push(task);
    }
    tasksize_++;
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