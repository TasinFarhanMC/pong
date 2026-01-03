#pragma once
#include <fmt/core.h>

#define __REL_FILE__ (__FILE__ + sizeof(PONG_SRC_DIR) - 1)

namespace logger {
std::string current_time();

bool init(bool info, bool verbose);
void clean();
void write_verb(const std::string &str);
void write_info(const std::string &str);
void write(const std::string &str);

#define LOG_VERB(name, fmt_str, ...)                                                                                                                   \
  logger::write_verb(                                                                                                                                  \
      fmt::format(                                                                                                                                     \
          "[" name "] [{}/VERB] "                                                                                                                      \
          "[{}:{}"                                                                                                                                     \
          "]: " fmt_str "\n",                                                                                                                          \
          logger::current_time(), __REL_FILE__, __LINE__, ##__VA_ARGS__                                                                                \
      )                                                                                                                                                \
  )

#define LOG_INFO(name, fmt_str, ...)                                                                                                                   \
  logger::write_info(                                                                                                                                  \
      fmt::format(                                                                                                                                     \
          "[" name "] [{}/INFO] "                                                                                                                      \
          "[{}:{}"                                                                                                                                     \
          "]: " fmt_str "\n",                                                                                                                          \
          logger::current_time(), __REL_FILE__, __LINE__, ##__VA_ARGS__                                                                                \
      )                                                                                                                                                \
  )

#define LOG_WARN(name, fmt_str, ...)                                                                                                                   \
  logger::write_info(                                                                                                                                  \
      fmt::format(                                                                                                                                     \
          "[" name "] [{}/WARN] "                                                                                                                      \
          "[{}:{}"                                                                                                                                     \
          "]: " fmt_str "\n",                                                                                                                          \
          logger::current_time(), __REL_FILE__, __LINE__, ##__VA_ARGS__                                                                                \
      )                                                                                                                                                \
  )

#define LOG_ERROR(name, fmt_str, ...)                                                                                                                  \
  logger::write(                                                                                                                                       \
      fmt::format(                                                                                                                                     \
          "[" name "] [{}/ERROR] "                                                                                                                     \
          "[{}:{}"                                                                                                                                     \
          "]: " fmt_str "\n",                                                                                                                          \
          logger::current_time(), __REL_FILE__, __LINE__, ##__VA_ARGS__                                                                                \
      )                                                                                                                                                \
  )
} // namespace logger
