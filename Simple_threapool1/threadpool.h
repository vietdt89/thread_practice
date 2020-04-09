#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <vector>
#include <queue>
#include <iostream>
#include <future>
#include <thread>
#include <functional>
#include <mutex>
#include "blockingqueue.h"
#include "worker.h"

class Threadpool{
public:
    Threadpool(unsigned int poolSize)
        :threadNum(poolSize), isRunning(true){
    }

    ~Threadpool(){
        std::unique_lock<std::mutex> lock(statusLock);
        //isRunning = false;
        for(Worker* worker:workerList){
            worker->join();
        }
    }

    void addJob(std::function<void(void)> f){
        jobQueue.push(f);
    }

    void start(){
        for(unsigned int i = 0 ; i< threadNum; i++){
            Worker* pWorker = new Worker(*this);
            workerList.push_back(pWorker);
        }
    }

    volatile bool& getRunning(){
        return isRunning;
    }

    BlockingQueue<std::function<void(void)>>& getJobQueue(){
        return jobQueue;
    }

    std::mutex queueJobLock;

private:
    unsigned int threadNum;
    volatile bool isRunning;

    std::mutex statusLock;
    std::vector<Worker*> workerList;
    BlockingQueue<std::function<void(void)>> jobQueue;
};

#endif // THREADPOOL_H
