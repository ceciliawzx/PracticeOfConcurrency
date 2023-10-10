#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

class Logger {
 public:
  Logger(std::mutex& mutex, size_t max_length)
      : mutex_(mutex), max_length_(max_length) {}

  void LogMessage(const std::string& message) {
    mutex_.lock();
    // Good, we avoid a data race by only inspecting the size of the log when
    // we hold the mutex.
    if (log_.size() + message.size() > max_length_) {
      // Uh oh! If we return here, we have not unlocked the mutex! This means
      // that other threads will not be able to acquire it. Deadlock!
      return;
    }
    log_ += message;
    mutex_.unlock();
  }

  [[nodiscard]] const std::string& GetLog() const { return log_; }

 private:
  std::string log_;
  std::mutex& mutex_;
  const size_t max_length_;
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
  Logger the_logger(the_mutex, 100);
  std::vector<std::thread> threads;
  for (size_t i = 0; i < 8; i++) {
    threads.emplace_back(ThreadBody, i, std::ref(foods), std::ref(the_logger));
  }

  for (auto& t : threads) {
    t.join();
  }

  std::string log_contents = the_logger.GetLog();
  std::cout << log_contents << std::endl;
  std::cout << log_contents.size() << std::endl;
}
