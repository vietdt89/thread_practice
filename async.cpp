#include <iostream>
#include <future>
#include <thread>

thread_local int g_num;

int read_it(int x) {
    return g_num + x;
}

int read_it2(int x, std::promise<int>* prObj){
    prObj->set_value(g_num+x);
    return g_num+x;
}

int main()
{
    g_num = 100;
    int arg;
    arg = 1;
    std::future<int> fut = std::async(std::launch::deferred, read_it, arg);
    std::future<int> fut2 = std::async(std::launch::async, read_it, arg);
    //std::cout << "Defer: " << fut.get() << std::endl;  //101 run on calling thread, Ownership of the future can be transferred to another thread
    //std::cout << "Async: " << fut2.get() << std::endl; // 1 run on new thread

    std::promise<int> promiseObj;
    std::future<int> fut3  =  promiseObj.get_future();
    //std::thread t1(read_it2, arg,&promiseObj);
    //std::cout << fut3.get() << std::endl;
    //t1.join();

    std::packaged_task<int(int)> task(read_it);
    std::future<int> fut4 = task.get_future();
    std::thread t2(std::move(task), arg);
    std::cout << fut4.get() << std::endl;
    t2.join();
    return 0;
}
