#include <iostream>
#include <thread>
#include <vector>

static void ThreadBody(size_t nice_id,
                       std::vector<std::string>& favourite_food) {
  std::cout << "Hello! I am " << std::this_thread::get_id()
            << ", but you can call me " << nice_id
            << ", and my favourite food is " << favourite_food[nice_id]
            << std::endl;
}

int main() {
  std::vector<std::string> foods = {"Apple",         "Spotted dick", "Pudding",
                                    "Haggis",        "Quorn",        "Pizza",
                                    "Custard cream", "Katsu curry"};

  std::vector<std::thread> threads;
  for (size_t i = 0; i < 8; i++) {
    threads.emplace_back(ThreadBody, i, std::ref(foods));
  }

  for (auto& t : threads) {
    t.join();
  }
}
