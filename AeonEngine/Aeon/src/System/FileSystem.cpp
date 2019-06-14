#include <AEON/System/FileSystem.h>

#include <fstream>

#include <AEON/System/DebugLogger.h>

namespace ae
{
	std::string FileSystem::readFile(const std::string& filepath, uint_fast16_t openMode)
	{
		std::string contents = "";
		std::string line = "";

		// Setup the file mode according to the flags provided
		int mode = std::ios::in;
		if (openMode & OpenMode::Binary) {
			mode |= std::ios::binary;
		}
		if (openMode & OpenMode::AtEnd) {
			mode |= std::ios::ate;
		}

		// Log an error message if the mode selected is invalid (ignored in Release mode)
		if constexpr (AEON_DEBUG) {
			if (openMode & OpenMode::Append || openMode & OpenMode::Truncate) {
				AEON_LOG_ERROR("Invalid flags", "The mode flags provided to open the file at \"" + filepath + "\" are incorrect.\nReturning empty string.");
				return contents;
			}
		}

		// Create the input file stream and read in the file's contents
		std::ifstream fin(filepath, mode);
		if (fin) {
			// Read in every line
			while (fin.good() && std::getline(fin, line)) {
				contents += line + '\n';
			}
			fin.close();
		}
		else {
			AEON_LOG_ERROR("Invalid filepath", "Unable to access file at \"" + filepath + "\".\nAborting operation.");
		}

		return contents;
	}

	void FileSystem::writeFile(const std::string& filepath, const std::string& content, uint_fast16_t openMode)
	{
		// Log an error message if the mode selected is invalid (ignored in Release mode)
		if constexpr (AEON_DEBUG) {
			if ((openMode & OpenMode::AtEnd && (openMode & OpenMode::Append || openMode & OpenMode::Truncate)) || (openMode & OpenMode::Append && openMode & OpenMode::Truncate)) {
				AEON_LOG_ERROR("Invalid flags", "The mode flags provided to open the file at \"" + filepath + "\" can't be combined.\nAborting operation.");
				return;
			}
		}

		// Setup the file mode according to the flags provided
		int mode = std::ios::out;
		if (openMode & OpenMode::Binary) {
			mode |= std::ios::binary;
		}
		if (openMode & OpenMode::AtEnd) {
			mode |= std::ios::ate;
		}
		else if (openMode & OpenMode::Append) {
			mode |= std::ios::app;
		}
		else if (openMode & OpenMode::Truncate) {
			mode |= std::ios::trunc;
		}

		// Create the output file stream and insert the provided content
		std::ofstream fout(filepath, mode);
		if (fout) {
			fout << content;
			fout.close();
		}
		else {
			AEON_LOG_ERROR("Invalid filepath", "Unable to open file at \"" + filepath + "\".\nAborting operation.");
		}
	}
}