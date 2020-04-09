#include "worker.h"
#include "threadpool.h"
#include <chrono>

void Worker::execute()
{
    while(mPool.getRunning()){
        if(!mPool.getJobQueue().empty()){
            std::function<void(void)> job = mPool.getJobQueue().front();
            mPool.getJobQueue().pop();
            job();
         }
        while(mPool.getJobQueue().empty()){
            std::chrono::milliseconds dur(500);
            std::this_thread::sleep_for(dur);
        }
     }

}
