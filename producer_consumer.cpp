#include <iostream>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <thread>

struct data{

};

std::mutex mutex;
std::queue<data> data_queue;
std::condition_variable cond;

data more_data(){
    return data();
}

void producer(){
    while(data_queue.size() < 5){
        std::cout << "produce" << std::endl;
        data const chunk = more_data();
        std::lock_guard<std::mutex> lk(mutex);
        data_queue.push(chunk);
        cond.notify_one();
    }
}

void consumer(){
    while(true){
        std::cout << "consume" << std::endl;
        std::unique_lock<std::mutex> lk(mutex);
        cond.wait(lk,[]{return !data_queue.empty();});
        data chunk = data_queue.front();
        data_queue.pop();
        lk.unlock();
    }
}

int main()
{
    std::thread t1(producer);
    std::thread t2(consumer);

    t1.join();
    t2.join();
    return 0;
}
