/**
 * @file include/carinfo-manager/log.hpp
 * @brief Declaration of class MyLogger, LOG_LEVEL, and LOG_TYPE
 * 
 * @details
 * This file contains the declaration of the MyLogger class, LOG_LEVEL class, and LOG_TYPE class.
 * The MyLogger class provides a static method to register loggers and log messages.
 *     - `registerLogger` registers a logger with a logger name, log level, log type, and log file path.
 *     - `log` logs a message with a logger name, log level, and message.
 * The LOG_LEVEL class provides constants for different log levels.
 * The LOG_TYPE class provides constants for different log types.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#pragma once
#include <cstdint>
#include <string>

#ifdef ERROR
#undef ERROR
#endif

class MyLogger {
  public:
	class LOG_LEVEL {
	  public:
		static constexpr uint8_t DEBUG = 1;
		static constexpr uint8_t INFO = 2;
		static constexpr uint8_t WARN = 4;
		static constexpr uint8_t ERROR = 8;
		static constexpr uint8_t CRITICAL = 16;
	};

	class LOG_TYPE {
	  public:
		static constexpr uint8_t CONSOLE = 1;
		static constexpr uint8_t FILE = 2;
	};

  public:
	static void registerLogger(const std::string &name,
							   const uint8_t level,
							   const uint8_t type,
							   const std::string &path = "");
	static void log(const std::string &name, const uint8_t level, const std::string &msg);
};
