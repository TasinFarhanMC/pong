#include "logger.hpp"
#include <betr/namespace.hpp>

#include <chrono>
#include <fstream>
#include <iostream>

using namespace logger;

static std::ofstream file;
static bool console;

namespace logger {
betr::String current_time() {
  using namespace std::chrono;
  auto now = system_clock::now();
  auto in_time_t = system_clock::to_time_t(now);

  std::ostringstream ss;
  ss << std::put_time(std::localtime(&in_time_t), "%H:%M:%S");
  return ss.str();
}

bool init(bool console) {
  ::console = console;
  file.open("latest.log");
  return file.is_open();
}

void write(const String &str) {
  if (console) { std::cout << str; }
  file << str;
}
void clean() { file.close(); }
} // namespace logger
