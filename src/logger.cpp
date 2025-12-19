#include "logger.hpp"

#include <chrono>
#include <fstream>
#include <iostream>

using namespace logger;

static std::ofstream file;
static bool info, verbose;

namespace logger {
std::string current_time() {
  using namespace std::chrono;
  auto now = system_clock::now();
  auto in_time_t = system_clock::to_time_t(now);

  std::ostringstream ss;
  ss << std::put_time(std::localtime(&in_time_t), "%H:%M:%S");
  return ss.str();
}

bool init(bool info, bool verbose) {
  ::info = info;
  ::verbose = verbose;
  file.open("latest.log");
  return file.is_open();
}

void write_verb(const std::string &str) {
  if (verbose) {
    if (info) { std::cout << str; }
    file << str;
  }
}

void write_info(const std::string &str) {
  if (info) { std::cout << str; }
  file << str;
}

void write(const std::string &str) {
  std::cout << str;
  file << str;
}
void clean() { file.close(); }
} // namespace logger
