#include "FileDataProvider.h"
#include <iostream>
#include <mutex>

const size_t WORDS_CHUNK_SIZE = 800;

bool FileDataProvider::init_file(const std::string &file_name) {
  m_file.open(file_name);
  if (!m_file.is_open()) {
    std::cout << "Error opening file!" << std::endl;
    return false;
  }
  return true;
}
std::shared_ptr<FileDataProvider> FileDataProvider::get_instance() {
  static std::shared_ptr<FileDataProvider> file_data_provider;
  if (!file_data_provider) {
    file_data_provider = std::make_shared<FileDataProvider>();
  }
  return file_data_provider;
}

std::vector<std::string> FileDataProvider::get_next_data() {
  std::lock_guard<std::mutex> lock(m_queue_mutex);
  if (m_queue.empty()) {
    return std::vector<std::string>();
  }
  auto next_data = m_queue.front();
  m_queue.pop();

  return std::move(next_data);
}

bool FileDataProvider::read_next_chunk() {
  if (!m_file.is_open() || m_file.eof()) {
    return false;
  }
  std::string word;
  std::vector<std::string> words(WORDS_CHUNK_SIZE);
  size_t local_chunk_size = 0;
  while (m_file >> word && local_chunk_size < WORDS_CHUNK_SIZE) {
    words.emplace_back(word);
    ++local_chunk_size;
  }
  words.shrink_to_fit();
  std::lock_guard<std::mutex> lock(m_queue_mutex);
  m_queue.push(std::move(words));
  return true;
}

// std::mutex m_queue_mutex;
// std::queue<std::string> m_queue;
