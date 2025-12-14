#pragma once
#include <format>

namespace logger {
std::string current_time();

bool init(bool console);
void clean();
void write(const std::string &str);

#define LOG_INFO(name, fmt, ...)                                                                                                                       \
  logger::write(                                                                                                                                       \
      std::format(                                                                                                                                     \
          "[" name "] [{}/INFO] "                                                                                                                      \
          "[" __FILE_NAME__ ":{}"                                                                                                                      \
          "]: " fmt "\n",                                                                                                                              \
          logger::current_time(), __LINE__, ##__VA_ARGS__                                                                                              \
      )                                                                                                                                                \
  )

#define LOG_WARN(name, fmt, ...)                                                                                                                       \
  logger::write(                                                                                                                                       \
      std::format(                                                                                                                                     \
          "[" name "] [{}/WARN] "                                                                                                                      \
          "[" __FILE_NAME__ ":{}"                                                                                                                      \
          "]: " fmt "\n",                                                                                                                              \
          logger::current_time(), __LINE__, ##__VA_ARGS__                                                                                              \
      )                                                                                                                                                \
  )

#define LOG_ERROR(name, fmt, ...)                                                                                                                      \
  logger::write(                                                                                                                                       \
      std::format(                                                                                                                                     \
          "[" name "] [{}/ERROR] "                                                                                                                     \
          "[" __FILE_NAME__ ":{}"                                                                                                                      \
          "]: " fmt "\n",                                                                                                                              \
          logger::current_time(), __LINE__, ##__VA_ARGS__                                                                                              \
      )                                                                                                                                                \
  )
} // namespace logger
