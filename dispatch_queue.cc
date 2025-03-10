// dispatch_queue.cc
#include "dispatch_queue.h"

namespace pixpark {

void DispatchQueue::Worker(size_t id) {
  std::function<void()> task;
  while (running_) {
    {
      std::unique_lock<std::mutex> lock(mutex_);
      // Wait until there is a task in the queue or the queue stops running.
      cv_.wait(lock, [&]() { return !running_ || !task_queue_.empty(); });

      if (!running_) break;

      // Take a task from the queue and increment the number of working threads.
      task = task_queue_.front();
      task_queue_.pop();
      n_working_++;
    }
    task();  // Execute the task.
    {
      std::unique_lock<std::mutex> lock(mutex_);
      n_working_--;  // Task execution completed, decrement the number of working threads.
    }
  }
}

DispatchQueue::DispatchQueue(QueueType type) : running_(true), n_working_(0) {
  // Create the first worker thread.
  workers_.emplace_back(&DispatchQueue::Worker, this, 0);
  serial_worker_thread_id_ = workers_[0].get_id();

  if (type == kConcurrent) {
    // If it's a concurrent queue, create worker threads equal to the number of CPU cores.
    size_t n = std::thread::hardware_concurrency();
    for (size_t i = 1; i < n; i++) {
      workers_.emplace_back(&DispatchQueue::Worker, this, i);
    }
  }
}

bool DispatchQueue::Busy() {
  bool queue_busy;
  {
    std::unique_lock<std::mutex> lock(mutex_);
    // Check if the queue is busy: task queue is not empty or there are threads executing tasks.
    queue_busy = !task_queue_.empty() || n_working_ > 0;
  }
  return queue_busy;
}

void DispatchQueue::Stop() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    running_ = false;  // Stop the queue from running.
  }
  cv_.notify_all();  // Notify all waiting threads.
  for (auto& worker : workers_) {
    worker.join();  // Wait for all worker threads to finish.
  }
  workers_.clear();  // Clear the collection of worker threads.
}

void DispatchQueue::Wait() {
  // Wait for all tasks to start execution.
  while (Busy()) std::this_thread::yield();
}

void DispatchQueue::Join() {
  Wait();  // Wait for all tasks to start execution.
  Stop();  // Stop the queue and wait for all tasks to complete.
}

void DispatchQueue::Async(const std::function<void()>& task) {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    task_queue_.push(task);
  }
  cv_.notify_one();  // Notify one waiting thread.
}

void DispatchQueue::Sync(const std::function<void()>& task) {
  if (IsWorkerThread()) {
    // Nested Sync calls in a serial queue can cause tasks to be stuck in the queue.
    // If it's the worker thread, execute the task directly to avoid this issue.
    // This problem only occurs with kSerial queues.
    task();
  } else {
    auto queue_task = std::make_shared<std::packaged_task<void()>>(std::move(task));
    auto future = queue_task->get_future();
    Async([queue_task]() { (*queue_task)(); });
    // Wait for the task to complete.
    future.get();
  }
}

bool DispatchQueue::IsWorkerThread() {
  std::thread::id current_thread_id = std::this_thread::get_id();
  return current_thread_id == serial_worker_thread_id_;
}

}  // namespace pixpark
