
#include <memory>
#include <iostream>
class c
{};
int main()
{
    std::shared_ptr<c> *f;
    std::shared_ptr<c>a(new c());
    f=&a;
    std::weak_ptr<c> b;b=*f;
    //b =std::shared_ptr<c>(f);
    std::cout<<b.expired()<<"\n";

    return 0;
}