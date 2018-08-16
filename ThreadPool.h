#ifndef THREADPOOL
#define THREADPOOL

#include "Worker.h"
#include <queue>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

class CThreadPool
{
public:
    static CThreadPool * Instance();
    static void DestoryInstance();
    void init(int taskSize);
    void addTask(CWork *work);
private:
    static CThreadPool * _instance;
    queue<CWork *> m_tasks;
    int m_task_size;
    
    
    
}
#endif