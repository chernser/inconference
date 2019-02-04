

#ifndef _UTILS_H
#define _UTILS_H

#include <memory>
#include <atomic>
#include <thread>
#include <mutex>
#include <queue>
#include <vector>

namespace LibUtils
{

using namespace std;

template <typename TTask>
class BasicTaskDistributor
{

  public:
    BasicTaskDistributor(uint8_t maxWorkers, uint16_t maxQueueSize);
    ~BasicTaskDistributor();
    void submitTask(shared_ptr<TTask> task);
    shared_ptr<TTask> pullNextTask(uint8_t workerIndex);

  private:
    std::vector<std::recursive_mutex> workersQueueMutexes;
    std::vector<std::deque<std::shared_ptr<TTask>>> workersQueues;

  protected:
    uint8_t numOfWorkers;
    std::vector<std::recursive_mutex>::iterator mutexIter;
    std::vector<std::deque<std::shared_ptr<TTask>>> queueIter;

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
    TaskDistributor(uint8_t workers, uint16_t maxQueueSize);
    virtual ~TaskDistributor();

    void submitTask(shared_ptr<TTask> task);
    shared_ptr<TTask> pullNextTask(uint8_t workerIndex);

  private:
    uint8_t maxWorkers;
    uint16_t queueLen;

    atomic_int8_t lastWorker;

    TTask **queues;
};

} // namespace LibUtils

#endif