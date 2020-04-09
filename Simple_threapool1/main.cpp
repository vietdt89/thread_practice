#include <iostream>
#include "threadpool.h"
#include <functional>

std::mutex printLock;
static int nCount = 0;
void test(){
    printf("\nExecuting:...");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    printLock.lock();
    printf("\nFinished:%d", ++nCount);
    printLock.unlock();
}

int main(){
    std::function<void(void)> f1([]{
       std::cout << "\njob 1 in thread " << std::this_thread::get_id()<< std::endl ;
       std::this_thread::sleep_for(std::chrono::seconds(5));
       std::cout << "\nfinish job 1" << std::endl;
    });
    std::function<void(void)> f2([]{
       std::cout << "\njob 2 in thread " << std::this_thread::get_id()<< std::endl ;
       std::this_thread::sleep_for(std::chrono::seconds(2));
       std::cout << "\nfinish job 2" << std::endl;
    });
    std::function<void(void)> f3([]{
       std::cout << "\njob 3 in thread " << std::this_thread::get_id()<< std::endl ;
       std::this_thread::sleep_for(std::chrono::seconds(13));
       std::cout << "\nfinish job 3" << std::endl;
    });
    std::function<void(void)> f4([]{
        std::cout << "\njob 4 in thread " << std::this_thread::get_id()<< std::endl ;
        std::this_thread::sleep_for(std::chrono::seconds(4));
        std::cout << "\nfinish job 4" << std::endl;
    });

    std::cout << "main thread " << std::this_thread::get_id() << std::endl;
    Threadpool p(4);
    p.start();
    std::function<void(void)> f(test);
    for(int i = 0; i < 5; i++)
        p.addJob(f);
    std::this_thread::sleep_for(std::chrono::seconds(8));
    p.addJob(f1);
    p.addJob(f2);
    p.addJob(f3);
    p.addJob(f4);
    return 0;
}
