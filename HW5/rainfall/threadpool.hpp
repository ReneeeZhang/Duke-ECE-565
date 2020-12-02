// This class is adapted from ECE751

#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <list>
#include <mutex>
#include <vector>
#include <atomic>
#include <thread>
#include <functional>
#include <condition_variable>

namespace TP
{
  class ThreadPool
  {
    // TODO: Define variables and data structures needed
    std::atomic_bool stop;
    std::atomic_uint idle_threads;
    unsigned int nThreads;
    mutable std::mutex mtx;
    std::condition_variable cv;
    std::condition_variable cv_waitall;
    std::vector<std::thread> threads;
    std::list<std::function<void(void)>> tasks;

    /* TODO:
    * Pulls a task off the queue and runs it
    * Signals to main when it is done
    */
    void Task()
    {
      --idle_threads;
      while(!stop)
      {
        std::function<void(void)> task(get_next_task());
        task();
      }
    }

    /* TODO:
    * Signals to main to indicate that we are executing
    * a task and work may be done.
    * Then grabs a task off of the task queue and executes
    * it, otherwise it will wait indefinitely for a task
    */
    std::function<void(void)> get_next_task()
    {
      std::unique_lock<std::mutex> lck(mtx);
      ++idle_threads;
      cv_waitall.notify_one();
      cv.wait(lck, [this]() { return stop || tasksRemaining() >= 1; });
      if(stop) return [](){};
      --idle_threads;
      std::function<void(void)> task = tasks.front();
      tasks.pop_front();
      return task;
    }

  public:
    /* TODO:
    * Implement constructor
    */
    ThreadPool(unsigned int nThreads) : stop{false}, idle_threads{}, nThreads(nThreads)
    {
      idle_threads = nThreads;
      for(unsigned int i = 0; i < nThreads; ++i)
      {
        threads.emplace_back([this] { Task(); });
      }
    }

    /* TODO:
    * Destructor
    */
    ~ThreadPool()
    {
      joinThreads();
    }

    /* TODO:
    * Returns number of threads
    */
    inline unsigned size() const
    {
      return nThreads;
    }

    /* TODO:
    * Returns number of remaining tasks
    */
    inline unsigned tasksRemaining()
    {
      //	std::lock_guard<std::mutex> lck(mtx);
      return tasks.size();
    }

    /* TODO:
    * This function enqueues a particular tasks and waits
    * for it to run to completion.
    */
    void runTaskToCompletion(std::function<void(void)> task)
    {
      enqueue(task);
      waitAll();
    }

    /* TODO:
     * Enqueues a new task onto the pool. A thread will be woken
     * up to execute the task if one is available.
     */
    void enqueue(std::function<void(void)> task)
    {
      {
        std::lock_guard<std::mutex> lck(mtx);
        tasks.push_back(task);
      }
      cv.notify_one();
    }

    /* TODO:
     * Join all threads - will by default wait until completion,
     * otherwise work will be incomplete
     * Use only if you are done with the threadpool
     */
    void joinThreads(bool waitForAll = true)
    {
      waitAll();
      stop = true;
      cv.notify_all();
      for(auto & thread: threads)
      {
        thread.join();
      }
    }

    /* TODO:
     * Wait until there are no remaining tasks
     */
    void waitAll()
    {
      std::unique_lock<std::mutex> lck(mtx);
      cv_waitall.wait(lck, [this]() { return tasksRemaining() == 0 && idle_threads == nThreads; });
    }
  };
}  // namespace ECE751

#endif
