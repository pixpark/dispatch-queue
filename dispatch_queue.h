// dispatch_queue.h
#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <set>
#include <thread>
#include <vector>

namespace pixpark {

/**
 * @brief Task queue that can execute tasks on a single or multiple threads.
 *
 * Execution threads are detached from the current thread.
 */
class DispatchQueue {
 public:
  /**
   * @brief Type of the task queue.
   * kSerial: Serial queue, tasks are executed sequentially in a single thread.
   * kConcurrent: Concurrent queue, tasks are executed concurrently in multiple threads,
   *             with the number of threads equal to the number of CPU cores.
   */
  enum QueueType { kSerial, kConcurrent };

  /**
   * @brief Constructs a new task queue.
   * @param type The type of the queue, determining whether tasks are executed serially or concurrently.
   */
  explicit DispatchQueue(QueueType type);

  /**
   * @brief Asynchronously executes a task by adding it to the queue.
   * If the queue is of type kSerial, tasks are executed sequentially in a single thread.
   * If the queue is of type kConcurrent, tasks are executed concurrently in multiple threads.
   * @param task The task to execute, typically a lambda expression.
   */
  void Async(const std::function<void()>& task);

  /**
   * @brief Synchronously executes a task and waits for it to complete.
   * If the queue is of type kSerial, tasks are executed sequentially in a single thread.
   * If the queue is of type kConcurrent, tasks are executed concurrently in multiple threads.
   * @param task The task to execute, typically a lambda expression.
   */
  void Sync(const std::function<void()>& task);

  /**
   * @brief Checks if the queue is busy, i.e., if there are tasks being executed or waiting to be executed.
   * @return true if the queue is busy, false otherwise.
   */
  bool Busy();

  /**
   * @brief Stops the queue, waits for all currently executing tasks to complete, and skips tasks that have not started.
   */
  void Stop();

  /**
   * @brief Waits for all tasks to start execution.
   */
  void Wait();

  /**
   * @brief Waits for all tasks to start execution and complete.
   */
  void Join();

 private:
  /**
   * @brief The execution function for worker threads, which takes tasks from the queue and executes them.
   * @param id The ID of the worker thread.
   */
  void Worker(size_t id);

  /**
   * @brief Checks if the calling thread is the same as the worker thread to avoid nested call deadlocks.
   * @return true if the calling thread is the worker thread, false otherwise.
   */
  bool IsWorkerThread();

  bool running_;  // Flag indicating whether the queue is running.
  int n_working_;  // Number of threads currently executing tasks.
  std::mutex mutex_;  // Mutex for protecting shared resources.
  std::condition_variable cv_;  // Condition variable for thread communication.
  std::queue<std::function<void()>> task_queue_;  // Task queue.
  std::vector<std::thread> workers_;  // Collection of worker threads.
  std::thread::id serial_worker_thread_id_;  // ID of the serial worker thread for kSerial queues.
};

}  // namespace pixpark
