#pragma once
#include <string.h>
#include <string>
#include <iostream>
//#include <iostream>
class Buffer
{
private:
    std::string buff_;
public:
    Buffer();

    void append(char *buff,size_t len);
    void HeadAppend(const std::string &buff);
    bool substr(std::string &buff);
    void erase(size_t pos,size_t nn);
    const char* data();
    int size();

    ~Buffer();
};