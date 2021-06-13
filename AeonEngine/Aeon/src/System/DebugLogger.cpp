#include <AEON/System/DebugLogger.h>

#include <string>
#include <iostream>
#include <ctime>

#include <AEON/System/FileSystem.h>
#include <AEON/System/Time.h>

namespace ae
{
	// DebugLogger::Log
		// Public Constructor(s)
	DebugLogger::Log::Log(std::string&& title, std::string&& description, std::string&& file,
	                      std::string&& function, Log::Level level, int line)
		: formattedInfo()
		, metadata("File: " + file +  "\nLine: " + std::to_string(line) + "\nFunction: " + function)
		, description(std::move(description))
		, title(std::move(title))
		, level(level)
	{
		// Create a string of the level of importance
		std::string levelStr = "Info";
		switch (level)
		{
		case DebugLogger::Log::Level::Warning:
			levelStr = "Warning";
			break;
		case DebugLogger::Log::Level::Error:
			levelStr = "Error";
		}

		// Create the string containing the formatted information
		const size_t LENGTH = ("File: " + file).size();
		const std::string EQUAL_STR = std::string(LENGTH, '=');
		formattedInfo =
			EQUAL_STR + '\n' +
			Time::getSystemDate() + ' ' + Time::getSystemTime() + '\n' +
			levelStr + " - " + this->title + '\n' +
			std::string(LENGTH, '-') + '\n' +
			this->description + "\n\n" +
			metadata + '\n' +
			EQUAL_STR + "\n\n";
	}

		// Friend Operator(s)
	std::ostream& operator<<(std::ostream& os, const DebugLogger::Log& log)
	{
		// Pass the formatted information to the output stream
		return os << log.formattedInfo;
	}

	// DebugLogger
		// Public Method(s)
	void DebugLogger::log(std::string&& title, std::string&& description, std::string&& file,
	                      std::string&& function, Log::Level level, int line)
	{
		// Take ownership of the log
		Log& ownedLog = mLogs.emplace_back(std::forward<std::string>(title), std::forward<std::string>(description), std::forward<std::string>(file),
		                                   std::forward<std::string>(function), level, line);

		// Display the log to the console (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			std::cerr << ownedLog << "\n";
		}
		// Append the log to the log file on disk
		FileSystem::writeFile(mErrorLog, ownedLog.formattedInfo, FileSystem::OpenMode::Append);
	}

	std::list<DebugLogger::Log> DebugLogger::getLogs() noexcept
	{
		std::list<Log> logs;
		mLogs.swap(logs);
	
		return logs;
	}

		// Public Static Method(s)
	DebugLogger& DebugLogger::getInstance()
	{
		static DebugLogger instance;
		return instance;
	}

		// Private Constructor(s)
	DebugLogger::DebugLogger()
		: mLogs()
		, mErrorLog("aeon_errors.log")
	{
	}
}