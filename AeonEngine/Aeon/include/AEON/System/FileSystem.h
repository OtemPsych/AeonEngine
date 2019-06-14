// MIT License
// 
// Copyright(c) 2019 Filippos Gleglakos
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

#ifndef Aeon_System_FileSystem_H_
#define Aeon_System_FileSystem_H_

#include <string>
#include <cstdint>

#include <AEON/Config.h>

namespace ae
{
	/*!
	 \brief A static class that provides I/O access to system files.
	*/
	class AEON_API FileSystem
	{
	public:
		/*!
		 \brief The enumeration providing the different ways of accessing a file.
		 \details The different enumeration bit flags may be combined together.
		*/
		enum OpenMode
		{
			None     = 0,      //!< Open the file using the default mode
			Binary   = 1 << 0, //!< Open in binary mode
			AtEnd    = 1 << 1, //!< Set the initial position at the end of the file
			Append   = 1 << 2, //!< All output operations are performed at the end of the file, appending the content to the current contect of the file
			Truncate = 1 << 3  //!< If the file is opened for output operations and it already existed, its previous content is deleted and replaced by the new one
		};

	public:
		/*!
		 \brief Deleted default constructor.
		 \details No instance of this class may be created.

		 \since v0.1.0
		*/
		FileSystem() = delete;
		/*!
		 \brief Deleted copy constructor.
		 
		 \since v0.1.0
		*/
		FileSystem(const FileSystem&) = delete;
	public:
		/*!
		 \brief Deleted assignment operator.
		 
		 \since v0.1.0
		*/
		FileSystem& operator=(const FileSystem&) = delete;
	public:
		/*!
		 \brief Reads in the file situated at the \a filepath provided using the \a openMode provided.
		 \details

		 \note Only the OpenMode::Default, OpenMode::Binary and OpenMode::AtEnd bit flags may be used and combined together.

		 \param[in] filepath The filepath of the file to read in
		 \param[in] openMode The ae::FileSystem::OpenMode used to access the file

		 \return The contents of the file

		 \par Example:
		 \code
		 // Reads in the contents of a binary file
		 std::string binaryFileContents = ae::FileSystem::readFile("data.dat", ae::FileSystem::OpenMode::Binary);
		 \endcode

		 \since v0.1.0
		*/
		[[nodiscard]] static std::string readFile(const std::string& filepath, uint_fast16_t openMode = OpenMode::None);
		/*!
		 \brief Writes into the file situated at the \a filepath provided, adding the \a content and using the \a openMode provided.

		 \note The OpenMode::AtEnd, OpenMode::Append and OpenMode::Truncate bit flags must not be combined together, an error log will be generated if they are and the operation will fail.

		 \param[in] filepath The filepath of the file to write into
		 \param[in] content The content to write into the file
		 \param[in] openMode The ae::FileSystem::OpenMode used to access the file

		 \par Example:
		 \code
		 // Creates a new text file and inserts the desired content
		  ae::FileSystem::writeFile("data.txt", newFileContents);
		 \endcode

		 \since v0.1.0
		*/
		static void writeFile(const std::string& filepath, const std::string& content, uint_fast16_t openMode = OpenMode::None);
	};
}
#endif // Aeon_System_FileSystem_H_

/*!
 \class ae::FileSystem
 \ingroup system

 The ae::FileSystem singleton class may be utilized by the engine user in order
 to have I/O access to the operating system's files on disk.

 The files may be opened using the different flags available; however, certain
 flags are only available whilst reading in a file and others whilst writing to
 one.

 Read-Write access flags:
 \li OpenMode::Default
 \li OpenMode::Binary

 Read access flags:
 \li OpenMode::AtEnd

 Write access flags:
 \li OpenMode::AtEnd (as long as OpenMode::Append and OpenMode::Truncate aren't being used)
 \li OpenMode::Append (as long as OpenMode::AtEnd and OpenMode::Truncate aren't being used)
 \li OpenMode::Truncate (as long as OpenMode::AtEnd and OpenMode::Append aren't being used)

 Usage example:
 \code
 // Reads in the contents of a binary file
 std::string binaryFileContents = ae::FileSystem::readFile("data.dat", ae::FileSystem::OpenMode::Binary);

 // Reads in the contents of a text file
 std::string fileContents = ae::FileSystem::readFile("data.txt");

 // Delete the contents of the text file and insert new content
 std::string newFileContents = "...";
 ae::FileSystem::writeFile("data.txt", newFileContents, ae::FileSystem::OpenMode::Truncate);
 \endcode

 \author Filippos Gleglakos
 \version v0.1.0
 \date 2019-06-10
 \copyright MIT License
*/