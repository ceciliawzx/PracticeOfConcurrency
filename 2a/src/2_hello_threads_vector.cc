#include <iostream>
#include <thread>
#include <vector>

static void ThreadBody() {
  // As you will see from the output, the built-in thread ID is not very useful!
  std::cout << "Hello! I am " << std::this_thread::get_id() << std::endl;
}

int main() {
  std::vector<std::thread> threads;
  for (size_t i = 0; i < 8; i++) {
    threads.emplace_back(ThreadBody);
  }

  for (auto& t : threads) {
    t.join();
  }
}
