#include <chrono>
#include <iostream>
#include <thread>

static void ThreadBody() {
  // Do not do this in real code! But having this thread sleep for a short time
  // allows us to see executions where the other thread prints their message
  // first.
  std::this_thread::sleep_for(std::chrono::milliseconds(2));
  std::cout << "Hello!" << std::endl;
}

int main() {
  // A thread launched using a function reference.
  std::thread t1(ThreadBody);

  // A thread launched using a lambda.
  std::thread t2([]() -> void { std::cout << "World!" << std::endl; });

  t1.join();
  t2.join();
}
