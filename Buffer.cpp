#include"Buffer.h"

Buffer::Buffer()
{

}
void Buffer::append(char *buff,size_t len)
{
    buff_.append(buff,len);
}

void Buffer::HeadAppend(const std::string &buff)
{   
    int len =buff.size();
    buff_.append((char*)&len,4);
    buff_.append(buff);
}

bool Buffer::substr(std::string &buff)
{
    int len=0;
    if(buff_.size()<4)return false;
    memcpy(&len,buff_.data(),4);
    if(buff_.size()<len+4)return false;

    buff=buff_.substr(4,len);
    buff_.erase(0,4+len);
    return true;
}
void Buffer::erase(size_t pos,size_t nn)
{
    buff_.erase(pos,nn);
}

const char* Buffer::data()
{
    return buff_.data();
}
int Buffer::size()
{
    return buff_.size();
}
Buffer::~Buffer()
{

}
