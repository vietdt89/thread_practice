#include <vector>
#include <queue>
#include <iostream>
#include <atomic>
#include <thread>
#include <functional>

class ThreadPool{
public:
    ThreadPool(unsigned int pool):threadCount(pool), stop(false){
        try{
            for(int i = 0; i < threadCount; i++)
                mThreads.push_back(std::thread(&ThreadPool::execute, this));
        }
        catch(...){
            stop = true;
            throw;
        }
    }

    ~ThreadPool(){
        stop = true;
    }

    template<typename FuncType>
    void addWork(FuncType f){
        workQueue.push(std::function<void()>(f));
    }

private:
    unsigned int threadCount;
    bool stop;
    std::vector<std::thread> mThreads;
    std::queue <std::function<void(void)>> workQueue;

    void execute(){
        while(!stop){
            if(!workQueue.empty()){
                std::function<void(void)> job = workQueue.front();
                job();
                workQueue.pop();
            }else
                std::this_thread::yield();
        }
    }
};

int main(){
    return 0;
}
