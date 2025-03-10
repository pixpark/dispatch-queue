# DispatchQueue

A lightweight C++ task queue for executing tasks serially or concurrently on single or multiple threads. Supports both synchronous (`Sync`) and asynchronous (`Async`) operations, making it ideal for efficient task management in multithreaded applications.

---

## Features

- **Serial and Concurrent Execution**: Choose between `kSerial` (tasks run sequentially) or `kConcurrent` (tasks run in parallel).
- **Sync and Async Operations**: Use `Sync` for blocking tasks and `Async` for non-blocking tasks.
- **Thread Safety**: Built with `std::mutex` and `std::condition_variable` for safe concurrent access.
- **Easy Integration**: Simple API for adding and managing tasks.

---

## Usage

### Example Code

```cpp
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
```

### Build and Run

1. Clone the repository:
   ```bash
   git clone https://github.com/pixpark/dispatch-queue.git
   cd dispatch-queue
   ```

2. Create a build directory:
   ```bash
   mkdir build && cd build
   ```

3. Generate build files with CMake:
   ```bash
   cmake ..
   ```

4. Build the project:
   ```bash
   make
   ```

5. Run the example:
   ```bash
   ./DispatchQueueExample
   ```

---

## API Overview

### DispatchQueue Class

- **`DispatchQueue(QueueType type)`**: Constructor to create a queue of type `kSerial` or `kConcurrent`.
- **`void Async(const std::function<void()>& task)`**: Add a task to the queue asynchronously.
- **`void Sync(const std::function<void()>& task)`**: Add a task to the queue synchronously and wait for it to complete.
- **`bool Busy()`**: Check if the queue is busy (tasks are running or waiting).
- **`void Stop()`**: Stop the queue and skip pending tasks.
- **`void Wait()`**: Wait for all tasks to start execution.
- **`void Join()`**: Wait for all tasks to complete.

---

## Requirements

- C++11 or higher
- CMake (for building)

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

## Contributing

Contributions are welcome! Please open an issue or submit a pull request for any improvements or bug fixes.

---

## Author

[Jaaron Kot@PixPark]  
GitHub: [Jaaron Kot](https://github.com/jaaronkot)  
Email: jaaronkot@gmail.com

---

## Acknowledgments

- Inspired by modern task queue implementations.
- Built with the help of the C++ Standard Library.

---

Enjoy using **DispatchQueue**! 🚀