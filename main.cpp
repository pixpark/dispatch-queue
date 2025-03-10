#include <chrono>
#include <iostream>
#include <thread>

#include "dispatch_queue.h"

int main() {
  // Example 1: Serial Queue (kSerial)
  {
    std::cout << "=== Serial Queue (kSerial) ===" << std::endl;
    pixpark::DispatchQueue serial_queue(pixpark::DispatchQueue::kSerial);

    // Async example: Add tasks to the queue asynchronously
    serial_queue.Async([]() {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(500));  // Simulate work
      std::cout << "Serial Async task 1 is running on thread: "
                << std::this_thread::get_id() << std::endl;
    });

    serial_queue.Async([]() {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(300));  // Simulate work
      std::cout << "Serial Async task 2 is running on thread: "
                << std::this_thread::get_id() << std::endl;
    });

    // Sync example: Add a task to the queue synchronously
    serial_queue.Sync([]() {
      std::cout << "Serial Sync task is running on thread: "
                << std::this_thread::get_id() << std::endl;
    });

    // Wait for all tasks to complete
    serial_queue.Join();
    std::cout << "All Serial Queue tasks completed!" << std::endl;
  }

  // Example 2: Concurrent Queue (kConcurrent)
  {
    std::cout << "\n=== Concurrent Queue (kConcurrent) ===" << std::endl;
    pixpark::DispatchQueue concurrent_queue(
        pixpark::DispatchQueue::kConcurrent);

    // Async example: Add tasks to the queue asynchronously
    concurrent_queue.Async([]() {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(500));  // Simulate work
      std::cout << "Concurrent Async task 1 is running on thread: "
                << std::this_thread::get_id() << std::endl;
    });

    concurrent_queue.Async([]() {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(300));  // Simulate work
      std::cout << "Concurrent Async task 2 is running on thread: "
                << std::this_thread::get_id() << std::endl;
    });

    // Sync example: Add a task to the queue synchronously
    concurrent_queue.Sync([]() {
      std::cout << "Concurrent Sync task is running on thread: "
                << std::this_thread::get_id() << std::endl;
    });

    // Wait for all tasks to complete
    concurrent_queue.Join();
    std::cout << "All Concurrent Queue tasks completed!" << std::endl;
  }

  return 0;
}