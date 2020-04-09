#ifndef WORKER_H
#define WORKER_H
#include <vector>
#include <queue>
#include <iostream>
#include <future>
#include <thread>
#include <functional>
#include <mutex>
#include "blockingqueue.h"

class Threadpool;
class Worker{
public:
    Worker(Threadpool& pool):mPool(pool){
        mThread = new std::thread(&Worker::execute, this);
    }
    void join(){
        mThread->join();
    }

private:
    Threadpool& mPool;
    std::thread* mThread;
    void execute();
};

#endif // WORKER_H
