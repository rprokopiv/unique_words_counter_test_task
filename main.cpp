#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>

#include "FileDataProvider.h"

const size_t DEFAULT_NUM_OF_THREADS = 8;

std::atomic<bool> is_chunks_coming = true;

std::set<std::string> unique_words;
std::mutex unique_words_mutex;

// NOTE: My laptop has 64GB of RAM, so it worked okay)
// it is possible that the large files could lead to huge memory consumption and
// crash. I have tried to do the best solutioon with the least efforts on this:)

int main(int argc, char *argv[]) {
  // probably boost program_options would be better here, but its not standart
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    return 1;
  }

  std::string file_name = argv[1];

  // would be better to use thread pool, but its too much to write custom thread
  // pool for the test project)
  size_t num_threads = std::thread::hardware_concurrency();

  if (num_threads == 0) {
    num_threads = DEFAULT_NUM_OF_THREADS;
  }
  auto file_data_provider = FileDataProvider::get_instance();
  if (!file_data_provider) {
    return -1;
  }
  if (!file_data_provider->init_file(file_name)) {
    std::cout << "failed to init file, stop the program" << std::endl;
    return 0;
  }

  std::vector<std::thread> threads;
  threads.emplace_back(std::thread([]() {
    auto file_data_provider = FileDataProvider::get_instance();
    if (!file_data_provider) {
      return;
    }
    while (file_data_provider->read_next_chunk()) {
      continue;
    }
    is_chunks_coming.store(false);
  }));

  for (size_t i = 1; i < DEFAULT_NUM_OF_THREADS; ++i) {
    threads.emplace_back([]() {
      auto file_data_provider = FileDataProvider::get_instance();
      std::set<std::string> unique_local_words;
      while (true) {
        auto next_chunk = file_data_provider->get_next_data();
        if (next_chunk.empty()) {
          if (is_chunks_coming.load(std::memory_order_relaxed)) {
            std::cout << "Sleep called, check system performance please"
                      << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
          } else {
            break;
          }
        }
        for (const auto &word : next_chunk) {
          unique_local_words.insert(word);
        }
      }
      std::lock_guard<std::mutex> lock(unique_words_mutex);
      unique_words.insert(unique_local_words.begin(), unique_local_words.end());
    });
  }

  for (auto &thread : threads) {
    thread.join();
  }
  std::lock_guard<std::mutex> lock(unique_words_mutex);

  for (auto &i : unique_words) {
    std::cout << i << " ";
  }
  std::cout << std::endl << unique_words.size() << std::endl;

  return 0;
}