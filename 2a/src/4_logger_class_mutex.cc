#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

class Logger {
 public:
  explicit Logger(std::mutex& mutex) : mutex_(mutex) {}

  void LogMessage(const std::string& message) {
    // If we get rid of the protection provided by the mutex, we see lots of
    // effects due to nondeterminism. Try it, by commenting out the lock() and
    // unlock() lines.
    mutex_.lock();
    log_ += message;
    mutex_.unlock();
  }

  [[nodiscard]] const std::string& GetLog() const { return log_; }

 private:
  std::string log_;
  std::mutex& mutex_;
};

static void ThreadBody(size_t nice_id, std::vector<std::string>& favourite_food,
                       Logger& logger) {
  std::stringstream stream;
  stream << "Hello! I am " << std::this_thread::get_id()
         << ", but you can call me " << nice_id << ", and my favourite food is "
         << favourite_food[nice_id] << std::endl;
  logger.LogMessage(stream.str());
}

int main() {
  std::vector<std::string> foods = {"Apple",         "Spotted dick", "Pudding",
                                    "Haggis",        "Quorn",        "Pizza",
                                    "Custard cream", "Katsu curry"};

  std::mutex the_mutex;
  Logger the_logger(the_mutex);
  std::vector<std::thread> threads;
  for (size_t i = 0; i < 8; i++) {
    threads.emplace_back(ThreadBody, i, std::ref(foods), std::ref(the_logger));
  }

  for (auto& t : threads) {
    t.join();
  }

  std::cout << the_logger.GetLog() << std::endl;
}
