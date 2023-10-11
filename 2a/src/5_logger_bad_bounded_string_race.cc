#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

class Logger {
public:
    Logger(std::mutex &mutex, size_t max_length)
        : mutex_(mutex), max_length_(max_length) {}

    void LogMessage(const std::string &message) {
      // Uh oh, this leads to a data race! Why?
      if (log_.size() + message.size() > max_length_) {
        return;
      }
      mutex_.lock();
      log_ += message;
      mutex_.unlock();
    }

    [[nodiscard]] const std::string &GetLog() const { return log_; }

private:
    std::string log_;
    std::mutex &mutex_;
    const size_t max_length_;
};

static void ThreadBody(size_t nice_id, std::vector<std::string> &favourite_food,
                       Logger &logger) {
  std::stringstream stream;
  stream << "Hello! I am " << std::this_thread::get_id()
         << ", but you can call me " << nice_id << ", and my favourite food is "
         << favourite_food[nice_id] << std::endl;
  logger.LogMessage(stream.str());
}

int main() {
  std::vector<std::string> foods = {"Apple", "Spotted dick", "Pudding",
                                    "Haggis", "Quorn", "Pizza",
                                    "Custard cream", "Katsu curry"};

  for (int j = 0; j < 100; j++) {
    std::mutex the_mutex;
    Logger the_logger(the_mutex, 100);
    std::vector<std::thread> threads;
    for (size_t i = 0; i < 8; i++) {
      threads.emplace_back(ThreadBody, i, std::ref(foods), std::ref(the_logger));
    }

    for (auto &t: threads) {
      t.join();
    }

    std::string log_contents = the_logger.GetLog();
    std::cout << log_contents << std::endl;
    // Run this enough times, and you will see that the log contents sometimes
    // ends up longer than the maximum value of 100 that we wanted to allow.
    std::cout << log_contents.size() << std::endl;
  }
}
