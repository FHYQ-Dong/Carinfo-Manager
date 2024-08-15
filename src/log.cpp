/**
 * @file src/log.cpp
 * @brief Implementation of class MyLogger
 * 
 * @details
 * This file contains the implementation of the MyLogger class.
 * The MyLogger class provides a static method to register loggers and log messages.
 *     - `registerLogger` registers a logger with a logger name, log level, log type, and log file path.
 *     - `log` logs a message with a logger name, log level, and message.
 * The LOG_LEVEL class provides constants for different log levels.
 * The LOG_TYPE class provides constants for different log types.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/log.hpp"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/spdlog.h"
#ifdef ERROR
#undef ERROR
#endif

/**
 * @brief Register a logger with a logger name, log level, log type, and log file path
 * 
 * @param name The name of the logger
 * @param level The log level of the logger
 * @param type The log type of the logger
 * @param path The log file path (needed when the log type contains LOG_TYPE::FILE)
 * 
 * @return void
 */
void MyLogger::registerLogger(const std::string &name,
							  const uint8_t level,
							  const uint8_t type,
							  const std::string &path) {
	std::vector<spdlog::sink_ptr> sinks;
	if (type & LOG_TYPE::CONSOLE) {
		auto console_sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
		console_sink->set_level(spdlog::level::info);
		sinks.push_back(console_sink);
	}
	if (type & LOG_TYPE::FILE && !path.empty()) {
		auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(path, 1024 * 1024 * 5, 3);
		file_sink->set_level(spdlog::level::debug);
		sinks.push_back(file_sink);
	}

	auto logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
	spdlog::flush_on(spdlog::level::debug);

	switch (level) {
		case LOG_LEVEL::DEBUG:
			logger->set_level(spdlog::level::debug);
			break;
		case LOG_LEVEL::INFO:
			logger->set_level(spdlog::level::info);
			break;
		case LOG_LEVEL::WARN:
			logger->set_level(spdlog::level::warn);
			break;
		case LOG_LEVEL::ERROR:
			logger->set_level(spdlog::level::err);
			break;
		case LOG_LEVEL::CRITICAL:
			logger->set_level(spdlog::level::critical);
			break;
		default:
			break;
	}
	spdlog::register_logger(logger);
}

/**
 * @brief Log a message with a logger name, log level, and message
 * 
 * @param name The name of the logger
 * @param level The log level of the message
 * @param msg The message to log
 * 
 * @return void
 */
void MyLogger::log(const std::string &name, const uint8_t level, const std::string &msg) {
	auto logger = spdlog::get(name);
	if (logger == nullptr)
		return;
	switch (level) {
		case LOG_LEVEL::DEBUG:
			logger->debug(msg);
			break;
		case LOG_LEVEL::INFO:
			logger->info(msg);
			break;
		case LOG_LEVEL::WARN:
			logger->warn(msg);
			break;
		case LOG_LEVEL::ERROR:
			logger->error(msg);
			break;
		case LOG_LEVEL::CRITICAL:
			logger->critical(msg);
			break;
		default:
			break;
	}
}
