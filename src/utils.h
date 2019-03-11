

#ifndef _UTILS_H
#define _UTILS_H

#include <memory>
#include <atomic>
#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <list>
#include <iostream>
#include <string>

namespace LibUtils
{

using namespace std;

template <typename TValue>
struct TValueHolder
{
    bool hasValue;
    TValue task;
};

/**
 * Distributes task using lock blocking queue 
 * One producer uses round-robin alg. to distribute tasks between 
 * workers. Each worker has index and can get task from queue with index
 * 
 */
template <typename TTask>
class TaskDistributor
{

  public:

    /**
     * Creates instance of distributer able to handle defined number of workers. 
     * 
     */
    // TaskDistributor(uint8_t workers, uint16_t maxQueueSize);

    virtual ~TaskDistributor(){};

    /**
     * Submits tasks to workers queue (shared or individual)
     */
    virtual void submitTask(TTask task) = 0;

    /**
     * Pulls task from queue for worker with specified index.
     * 
     */
    virtual TValueHolder<TTask> pullNextTask(uint8_t workerIndex) = 0;
};

template <typename TTask>
class BasicTaskDistributor : public TaskDistributor<TTask>
{

  public:
    explicit BasicTaskDistributor(uint8_t maxWorkers, uint16_t maxQueueSize)
        : workersQueueMutexes(0), workersQueues(0),
          numOfWorkers(maxWorkers), mutexIter(), queueIter()
    {
        for (uint8_t i = 0; i < maxWorkers; i++)
        {
            workersQueueMutexes.push_back(new std::recursive_mutex());
            workersQueues.push_back(new std::deque<TTask>());
        }

        mutexIter = workersQueueMutexes.begin();
        queueIter = workersQueues.begin();
    }

    virtual ~BasicTaskDistributor()
    {
        for (auto iter = workersQueueMutexes.begin(); iter != workersQueueMutexes.end(); iter++ ) {
            delete *iter;
        }

        for (auto iter = workersQueues.begin(); iter != workersQueues.end(); iter++) {
            delete *iter;
        }
    };

    void submitTask(TTask task)
    {
        std::recursive_mutex *mutex = NULL;
        if (mutexIter == workersQueueMutexes.end())
        {
            mutexIter = workersQueueMutexes.begin();
            queueIter = workersQueues.begin();            
        }


        mutex = (*mutexIter);

        std::lock_guard<std::recursive_mutex> lock(*mutex);

        auto taskQueue = (*queueIter);
        taskQueue->push_front(task);
        mutexIter++;
        queueIter++;
    }

    TValueHolder<TTask> pullNextTask(uint8_t workerIndex)
    {
        std::lock_guard<std::recursive_mutex> lock(*workersQueueMutexes.at(workerIndex));
        auto taskQueue = workersQueues.at(workerIndex);
        if (taskQueue->size() > 0)
        {
            TTask task = taskQueue->back();
            taskQueue->pop_back();
            return TValueHolder<TTask> {true, task};
        }
        else
        {
            return TValueHolder<TTask> {false};
        }
    }

  private:
    std::vector<std::recursive_mutex*> workersQueueMutexes;
    std::vector<std::deque<TTask>*> workersQueues;

  protected:
    uint8_t numOfWorkers;

    typename std::vector<std::deque<TTask>*>::iterator queueIter;
    std::vector<std::recursive_mutex*>::iterator mutexIter;
};

} // namespace LibUtils

#endif