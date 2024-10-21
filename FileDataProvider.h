#pragma once
#include <fstream>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <vector>

class FileDataProvider {
public:
  bool init_file(const std::string &file_name);
  static std::shared_ptr<FileDataProvider> get_instance();

  std::vector<std::string> get_next_data();
  bool read_next_chunk();

private:
  std::mutex m_queue_mutex;
  std::queue<std::vector<std::string>> m_queue;
  std::ifstream m_file;
};