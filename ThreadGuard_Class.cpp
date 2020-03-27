#include <iostream>
#include <thread>

class ThreadGuard{
    std::thread& t;
public:
    explicit ThreadGuard(std::thread& _t):t(_t){}
    ~ThreadGuard(){
        if(t.joinable())
            t.join();
    }
    ThreadGuard(ThreadGuard const&)= delete;
    ThreadGuard& operator =(ThreadGuard const&)=delete;
};

void func(){
    int i = 0;
    while(i < 1000)
        std::cout << i++ << std::endl;
}

int main()
{
    std::thread mythread(func);
    ThreadGuard g(mythread);
    return 0;
}
