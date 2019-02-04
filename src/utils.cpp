
#include <memory>
#include <atomic>
#include <thread>
#include <mutex>
#include <queue>
#include <list>

#include "utils.h"

namespace LibUtils
{

template <typename TTask>
BasicTaskDistributor<TTask>::BasicTaskDistributor(uint8_t maxWorkers, uint16_t maxQueueSize)
    : workersQueueMutexes(maxWorkers), workersQueues(maxWorkers),
      numOfWorkers(maxWorkers), mutexIter(workersQueueMutexes.begin), queueIter(workersQueues.begin)
{
    for (uint8_t i = 0; i < maxWorkers; i++)
    {
        workersQueueMutexes[i] = std::recursive_mutex();
        workersQueues[i] = std::queue<shared_ptr<TTask>>();
    }
}
template <typename TTask>
BasicTaskDistributor<TTask>::~BasicTaskDistributor()
{
}

template <typename TTask>
void BasicTaskDistributor<TTask>::submitTask(shared_ptr<TTask> task)
{
    if (mutexIter == workersQueueMutexes.end())
    {
        mutexIter = workersQueueMutexes.begin();
        queueIter = workersQueues.begin();
    }
    
    std::lock_guard<std::recursive_mutex> lock(*mutexIter);

    *queueIter.push_back(task);

    mutexIter++;
    queueIter++;
}

template <typename TTask>
shared_ptr<TTask> BasicTaskDistributor<TTask>::pullNextTask(uint8_t workerIndex)
{
    std::lock_guard<std::recursive_mutex> lock(workersQueueMutexes.at(workerIndex));
    std::deque<std::shared_ptr<TTask>> queue = workersQueues.at(workerIndex);
    return queue.pop_front();
}

template <typename TTask>
TaskDistributor<TTask>::TaskDistributor(uint8_t workers, uint16_t maxQueueSize) : maxWorkers(workers), queueLen(maxQueueSize)
{
    queues = new TTask *[maxWorkers];
    for (uint8_t i = 0; i < maxWorkers; i++)
    {
        queues[i] = new TTask[queueLen];
        memset(queues[i], 0, queueLen);
    }
}

template <typename TTask>
TaskDistributor<TTask>::~TaskDistributor()
{
    for (uint8_t i = 0; i < maxWorkers; i++)
    {
        delete queues[i];
    }
}

template <typename TTask>
void TaskDistributor<TTask>::submitTask(shared_ptr<TTask> task)
{
}

template <typename TTask>
shared_ptr<TTask> TaskDistributor<TTask>::pullNextTask(uint8_t workerIndex)
{
}

} // namespace LibUtils