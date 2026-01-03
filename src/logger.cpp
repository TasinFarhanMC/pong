#include "logger.hpp"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace logger;

static std::ofstream file;
static bool info = false;
static bool verbose = false;

namespace logger {

std::string current_time() {
  using namespace std::chrono;
  auto now = system_clock::now();
  std::time_t now_c = system_clock::to_time_t(now);

  std::tm local_tm {};
  local_tm = *std::localtime(&now_c);

  std::ostringstream ss;
  ss << std::put_time(&local_tm, "%H:%M:%S");
  return ss.str();
}

bool init(bool enable_info, bool enable_verbose) {
  ::info = enable_info;
  ::verbose = enable_verbose;
  file.open("latest.log", std::ios::out | std::ios::trunc);
  return file.is_open();
}

void write_verb(const std::string &str) {
  if (verbose) {
    if (info) std::cout << str;
    file << str;
  }
}

void write_info(const std::string &str) {
  if (info) std::cout << str;
  file << str;
}

void write(const std::string &str) {
  std::cout << str;
  file << str;
}

void clean() { file.close(); }

} // namespace logger
