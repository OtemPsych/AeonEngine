// MIT License
// 
// Copyright(c) 2019-2022 Filippos Gleglakos
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef Aeon_System_DebugLogger_H_
#define Aeon_System_DebugLogger_H_

#include <list>
#include <string>

#include <AEON/Config.h>

// Define the logging macros
#define AEON_LOG(level, title, description) ae::DebugLogger::getInstance().log(title, description, __FILE__, __func__, level, __LINE__)
#define AEON_LOG_INFO(title, description) AEON_LOG(ae::DebugLogger::Log::Level::Info, title, description)
#define AEON_LOG_WARNING(title, description) AEON_LOG(ae::DebugLogger::Log::Level::Warning, title, description)
#define AEON_LOG_ERROR(title, description) AEON_LOG(ae::DebugLogger::Log::Level::Error, title, description)

namespace ae
{
	/*!
	 \brief A singleton class used to log debug information concerning Aeon.
	 \note This class may be used by the engine user if they so desire but the information formatting can't be modified.
	*/
	class AEON_API DebugLogger
	{
	public:
		/*!
		 \brief The structure representing a debug log.
		*/
		struct AEON_API Log
		{
			// Public enum(s)
			/*!
			 \brief Enumeration of the available levels of severity of a log.
			*/
			enum class Level { Info, Warning, Error };

			// Public member(s)
			std::string formattedInfo; //!< The log's formatted information
			std::string metadata;      //!< The log's medata (file, line and function)
			std::string description;   //!< The log's description
			std::string title;         //!< The log's title
			Level       level;         //!< The level of importance (Info, Warning, Error)

			// Public constructor(s)
			/*!
			 \brief Constructs the ae::DebugLogger::Log by providing the \a level of importance, the \a title, the \a description and the metadata.
			 \note The metadata \a file, \a line and \a function will be automatically provided by using one of the available macros.

			 \param[in] title An std::string rvalue containing the log's title
			 \param[in] description An std::string rvalue containing the log's description
			 \param[in] file The metadata of the file's name that will be automatically provided by using the macro
			 \param[in] function The metadata of the function's name that will be automatically provided by using the macro
			 \param[in] level The ae::DebugLogger::Log::Level containing the level of importance
			 \param[in] line The metadata of the line that will be automatically provided by using the macro

			 \par Example:
			 \code
			 // The metadata parameters will be automatically filled in
			 AEON_LOG_ERROR("Title of the error message", "Description of the error message");
			 \endcode

			 \since v0.6.0
			*/
			Log(std::string&& title, std::string&& description, std::string&& file,
			    std::string&& function, Log::Level level, int line);
			/*!
			 \brief Copy constructor.

			 \since v0.6.0
			*/
			Log(const Log&) = default;
			/*!
			 \brief Move constructor.

			 \since v0.6.0
			*/
			Log(Log&&) noexcept = default;

			// Public operator(s)
			/*!
			 \brief Assignment operator.

			 \since v0.6.0
			*/
			Log& operator=(const Log&) = default;
			/*!
			 \brief Move assignment operator.

			 \since v0.6.0
			*/
			Log& operator=(Log&&) noexcept = default;

			// Friend operator(s)
			/*!
			 \brief Passes the \a log's formatted information to the output stream.

			 \param[in, out] os The output stream
			 \param[in] log The ae::DebugLogger::Log containing the information that will be passed to the \a os

			 \return The output stream \a os that was provided

			 \par Example:
			 \code
			 std::vector<ae::DebugLogger::Log> logs = ae::DebugLogger::getInstance().getLogs();
			 for (const ae::DebugLogger::Log& log : logs) {
				std::cout << log << std::endl;
			 }
			 \endcode

			 \since v0.6.0
			*/
			friend AEON_API std::ostream& operator<<(std::ostream& os, const Log& log);
		};

	public:
		// Public constructor(s)
		/*!
		 \brief Deleted copy constructor.

		 \since v0.1.0
		*/
		DebugLogger(const DebugLogger&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.6.0
		*/
		DebugLogger(DebugLogger&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.1.0
		*/
		DebugLogger& operator=(const DebugLogger&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.6.0
		*/
		DebugLogger& operator=(DebugLogger&&) = delete;
	public:
		// Public method(s)
		/*!
		 \brief Logs a new debug log that will be kept until the list is retrieved.
		 \details The log's information will be displayed to the console in Debug mode and stored on a file on disk on Debug and Release mode.
		 \note The metadata \a file, \a line and \a function will be automatically provided by using one of the available macros.

		 \param[in] title An std::string rvalue containing the log's title
		 \param[in] description An std::string rvalue containing the log's description
		 \param[in] file The metadata of the file's name that will be automatically provided by using the macro
		 \param[in] function The metadata of the function's name that will be automatically provided by using the macro
		 \param[in] level The ae::DebugLogger::Log::Level containing the level of importance
		 \param[in] line The metadata of the line that will be automatically provided by using the macro

		 \par Example:
		 \code
		 // The metadata parameters will be automatically filled in
		 AEON_LOG_ERROR("Title of the error message", "Description of the error message");
		 \endcode

		 \sa getLogs()

		 \since v0.6.0
		*/
		void log(std::string&& title, std::string&& description, std::string&& file,
		         std::string&& function, Log::Level level, int line);
		/*!
		 \brief Retrieves the list of all the currently stored debug logs and empties the list.

		 \return The list of all the currently stored debug logs

		 \par Example:
		 \code
		 auto logs = ae::DebugLogger::getInstance().getLogs();
		 for (const auto& log : logs) {
			std::cout << log << std::endl;
		 }
		 \endcode

		 \sa log()

		 \since v0.6.0
		*/
		_NODISCARD std::list<Log> getLogs() noexcept;

		// Public static method(s)
		/*!
		 \brief Retrieves the ae::DebugLogger's single instance.
		 \details The ae::DebugLogger will be constructed for the first time by calling this static method.

		 \return The ae::DebugLogger's single instance

		 \par Example:
		 \code
		 ae::DebugLogger& debugLogger = ae::DebugLogger::getInstance();
		 \endcode

		 \since v0.6.0
		*/
		_NODISCARD static DebugLogger& getInstance();
	private:
		// Private constructor(s)
		/*!
		 \brief Default constructor.
		 \details All error and warning logs will be written in the aeon_errors.log file.

		 \since v0.6.0
		*/
		DebugLogger();

	private:
		// Private member(s)
		std::list<Log> mLogs;     //!< The list of currently stored debug logs
		std::string    mErrorLog; //!< The name of the file in which the logs will be stored
	};
}
#endif // Aeon_System_DebugLogger_H_

/*!
 \class ae::DebugLogger
 \ingroup system

 The ae::DebugLogger singleton class is used by the Aeon engine to display to
 the console informational messages, warnings and errors during the program's
 execution. It also logs the same information (apart from the informational
 messages) to the log file \b aeon_errors.log.

 The log's format is the following:
 \code
 =====================================================
 YYYY.MM.DD HH:MM:SS
 <Level of importance> - <Title>
 -----------------------------------------------------
 <Description>

 File: <Filename>
 Line: <LineNumber>
 Function: <FunctionName>
 =====================================================
 \endcode

 Usage example:
 \code
 // Debug-only log
 if constexpr (AEON_DEBUG) {
	if (...) {
		// The metadata parameters will be automatically filled in
		AEON_LOG_ERROR("Title of debug-only error", "Description of error message");
	}
 }

 // Debug and release log
 if (...) {
	// The metadata parameters will be automatically filled in
	AEON_LOG_ERROR("Title of debug and release error", "Description of error message");
 }
 \endcode

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2021.01.23
 \copyright MIT License
*/