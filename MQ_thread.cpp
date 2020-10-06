#include <iostream>
#include <queue>
#include <list>
#include <unistd.h>
#include "Thread.h"
#include <pthread.h>
#include <errno.h>

struct Message{
    std::string data;
    int src;
    int des;
};

class IObserver{
public:
    virtual void update(Message& msg)=0;
    virtual int getID() = 0;
    virtual ~IObserver(){};
};

class ISubject{
public:
    virtual void attach(IObserver* ob)=0;
    virtual void detachme(IObserver* ob)=0;
    virtual void notify() = 0;
    virtual ~ISubject(){};
};

class Subject : public ISubject,public Thread{
public:
    void attach(IObserver *ob) override;
    void detachme(IObserver *ob) override;
    void notify() override;
    void sendMsg(Message& msg){
        mq.push(msg);
    }
protected:
    int run() override{
        while(true){
            if(!mq.empty()){
                _msg = mq.front();
                mq.pop();
                notify();
            }
            else
                sleep(1U);
        }
    }
private:
    std::queue<Message> mq;
    std::list<IObserver*> obList;
    Message _msg;
};
void Subject::attach(IObserver* ob){
    obList.push_back(ob);
}

void Subject::detachme(IObserver *ob){
    obList.remove(ob);
}

void Subject::notify(){
    std::list<IObserver*>::iterator i = obList.begin();
    while(i != obList.end()){
        if((*i)->getID() == _msg.des){
            (*i)->update(_msg);
            break;
        }
        ++i;
    }
}
class Observer1: public IObserver,public Thread{
private:
    Subject* subject;
    int ID;
public:
    explicit Observer1(Subject* _subject,int _id): subject(_subject),ID(_id){
        subject->attach(this);
    }
    void update(Message &msg) override{
        std::cout << msg.data << " from " << msg.src << " to " << msg.des << "~" << ID << std::endl;
        std::cout.flush();
    }
    int getID() override{
        return ID;
    }
protected:
    int run() override{
        while(true){
            sleep(1U);
            Message m;
            m.data = "message content";
            m.src = 1;
            m.des = 2;
            subject->sendMsg(m);
            sleep(1U);

        }
    }
};

class Observer2: public IObserver,public Thread{
private:
    Subject* subject;
    int ID;
public:
    explicit Observer2(Subject* _subject,int _id): subject(_subject),ID(_id){
        subject->attach(this);
    }
    void update(Message &msg) override{
        std::cout << msg.data << " from " << msg.src << " to " << msg.des << "~" << ID << std::endl;
        std::cout.flush();
    }
    int getID() override{
        return ID;
    }
protected:
    int run() override{
        while(true){
            sleep(2U);
            Message m;
            m.data = "message content";
            m.src = 2;
            m.des = 1;
            subject->sendMsg(m);
            sleep(3U);

        }
    }
};

int main()
{
    std::cout << "Start...\n";
    Subject* sub =  new Subject() ;
    Observer1* ob1 = new Observer1(sub,1);
    Observer2* ob2 = new Observer2(sub,2);

    sub->start();
    sub->join();
    ob1->start();
    ob1->join();
    ob2->start();
    ob2->join();
    getchar();
    return 0;
}
