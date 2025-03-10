#include "dispatch_queue.h"
#include <iostream>

int main() {
    // Create a serial dispatch queue
    pixpark::DispatchQueue serial_queue(pixpark::DispatchQueue::kSerial);

    // Async example: Add tasks to the queue asynchronously
    serial_queue.Async([]() {
        std::cout << "Async task 1 is running on thread: " << std::this_thread::get_id() << std::endl;
    });

    serial_queue.Async([]() {
        std::cout << "Async task 2 is running on thread: " << std::this_thread::get_id() << std::endl;
    });

    // Sync example: Add a task to the queue synchronously
    serial_queue.Sync([]() {
        std::cout << "Sync task is running on thread: " << std::this_thread::get_id() << std::endl;
    });

    // Wait for all tasks to complete
    serial_queue.Join();

    std::cout << "All tasks completed!" << std::endl;

    return 0;
}