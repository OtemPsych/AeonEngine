#include <AEON/System/DebugLogger.h>

#include <string>
#include <iostream>

#include <AEON/System/FileSystem.h>

namespace ae
{
	// DebugLogger::Log
		// Public Constructor(s)
	DebugLogger::Log::Log(std::string&& title, std::string&& description, std::string&& file,
	                      std::string&& function, Log::Level level, int line)
		: formattedInfo()
		, metadata("File: " + file + "\nLine: " + std::to_string(line) + "\nFunction: " + function)
		, description(std::move(description))
		, title(std::move(title))
		, level(level)
	{
		// Create a string of the level of importance
		std::string levelStr = "Info";
		switch (this->level)
		{
		case DebugLogger::Log::Level::Warning:
			levelStr = "Warning";
			break;
		case DebugLogger::Log::Level::Error:
			levelStr = "Error";
		}

		// Create the string containing the formatted information
		formattedInfo =
			"=====================================================\n" +
			levelStr + " - " + this->title +
			"\n-----------------------------------------------------\n" +
			this->description + "\n\n" +
			metadata +
			"\n=====================================================\n\n";
	}

	DebugLogger::Log::Log(Log&& rvalue) noexcept
		: formattedInfo(std::move(rvalue.formattedInfo))
		, metadata(std::move(rvalue.metadata))
		, description(std::move(rvalue.description))
		, title(std::move(rvalue.title))
		, level(rvalue.level)
	{
	}

		// Public Friend Operator(s)
	std::ostream& operator<<(std::ostream& os, const DebugLogger::Log& log)
	{
		// Create a string of the level of importance
		std::string levelStr = "Info";
		switch (log.level)
		{
		case DebugLogger::Log::Level::Warning:
			levelStr = "Warning";
			break;
		case DebugLogger::Log::Level::Error:
			levelStr = "Error";
		}

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
		// Append the log to the log file on disk if it's not an informational log
		if (ownedLog.level != Log::Level::Info) {
			FileSystem::writeFile(mErrorLog, ownedLog.formattedInfo, FileSystem::OpenMode::Append);
		}
	}

	std::vector<DebugLogger::Log> DebugLogger::getLogs() noexcept
	{
		std::vector<Log> logs;
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