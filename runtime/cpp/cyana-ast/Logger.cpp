//
// Created by tian wei jun on 2022/11/22 0022.
//
#include "Logger.h"
#include <errno.h>
#include <iostream>
#include <stdarg.h>
#include <string.h>
#include <time.h>

using namespace logger;

const char *Logger::s_level[LEVEL_COUNT] =
    {
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
        "FATAL"};

Logger *Logger::logger = new Logger();

Logger::Logger() : m_level(INFO) {
}

Logger::~Logger() {
}

void Logger::log(Level level, const char *file, int line, const char *format, ...) {
  if (m_level > level) {
    return;
  }

  time_t ticks = time(NULL);
  struct tm *ptm = localtime(&ticks);
  char timestamp[32];
  memset(timestamp, 0, sizeof(timestamp));
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", ptm);

  int len = 0;
  const char *fmt = "%s %s %s:%d ";
  len = snprintf(NULL, 0, fmt, timestamp, s_level[level], file, line);
  if (len > 0) {
    char *buffer = new char[len + 1];
    snprintf(buffer, len + 1, fmt, timestamp, s_level[level], file, line);
    buffer[len] = 0;
    std::cout << buffer;
    delete buffer;
  }

  va_list arg_ptr;
  va_start(arg_ptr, format);
  len = vsnprintf(NULL, 0, format, arg_ptr);
  va_end(arg_ptr);
  if (len > 0) {
    char *content = new char[len + 1];
    va_start(arg_ptr, format);
    vsnprintf(content, len + 1, format, arg_ptr);
    va_end(arg_ptr);
    content[len] = 0;
    std::cout << content;
    delete content;
  }

  std::cout << "\n";
  std::cout.flush();
}

void Logger::level(int level) {
  m_level = level;
}
