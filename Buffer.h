#pragma once
#include <string.h>
#include <string>
//#include <iostream>
class Buffer
{
private:
    std::string buff_;
public:
    Buffer();

    void append(const std::string &buff);
    void HeadAppend(const std::string &buff);
    bool substr(const std::string &buff);
    void erase(size_t pos,size_t nn);
    const char* data();
    int size();

    ~Buffer();
};