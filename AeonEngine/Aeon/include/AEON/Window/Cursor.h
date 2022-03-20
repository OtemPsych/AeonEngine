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

#ifndef	Aeon_Window_Cursor_H_
#define Aeon_Window_Cursor_H_

#include <AEON/Config.h>
#include <AEON/Math/Vector.h>

// Forward declaration(s)
struct GLFWcursor;

namespace ae
{
	/*!
	 \brief The class used to define the appearance of the application's cursor.
	*/
	class AEON_API Cursor
	{
	public:
		// Public enum(s)
		/*!
		 \brief The various standard cursor types.
		*/
		enum class Type {
			Arrow     = 0x00036001, //!< The regular arrow shape
			IBeam     = 0x00036002, //!< The text input I-beam shape
			Crosshair = 0x00036003, //!< The crosshair shape
			Hand      = 0x00036004, //!< The hand shape
			HResize   = 0x00036005, //!< The horizontal resize arrow shape
			VResize   = 0x00036006  //!< The vertical resize arrow shape
		};

	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details Sets the cursor to nullptr.

		 \since v0.7.0
		*/
		Cursor() noexcept;
		/*!
		 \brief Constructs the ae::Cursor by providing the pixel data.
		 \details The \a pixels provided must be in 32-bit (8 bits per channel) and organized in sequential lines, starting from the top left corner.

		 \param[in] pixels The 32-bit pixel data (8 bits per channel)
		 \param[in] size The ae::Vector2 containing the image size
		 \param[in] hotspot The ae::Vector2 containing the position of the cursor's action

		 \par Example:
		 \code
		 // 16x16 image with 4 color channels (RGBA)
		 std::array<unsigned char, 16 * 16 * 4> pixels;
		 pixels.fill(0xff);

		 // Cursor with white 16x16 image with its position of action at the top left corner
		 ae::Cursor cursor(pixels.data(), ae::Vector2i(16, 16), ae::Vector2i(0, 0));
		 \endcode

		 \since v0.7.0
		*/
		Cursor(unsigned char* pixels, const Vector2i& size, const Vector2i& hotspot);
		/*!
		 \brief Constructs the ae::Cursor by providing the cursor type.

		 \param[in] type The ae::Cursor::Type of the system-standard cursor

		 \par Example:
		 \code
		 ae::Cursor cursor(ae::Cursor::Type::Hand);
		 \endcode

		 \since v0.7.0
		*/
		explicit Cursor(Type type);
		/*!
		 \brief Deleted copy constructor.

		 \since v0.7.0
		*/
		Cursor(const Cursor&) = delete;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::Cursor that will be moved

		 \since v0.7.0
		*/
		Cursor(Cursor&& rvalue) noexcept = default;
		/*!
		 \brief Destructor.
		 \details Destroys the handle to the GLFW cursor.

		 \since v0.7.0
		*/
		~Cursor();
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.7.0
		*/
		Cursor& operator=(const Cursor&) = delete;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::Cursor that will be moved

		 \return The caller ae::Cursor

		 \since v0.7.0
		*/
		Cursor& operator=(Cursor&& rvalue) noexcept = default;

	public:
		// Public method(s)
		/*!
		 \brief Sets a personalized cursor by providing the pixel data.
		 \details The \a pixels provided must be in 32-bit (8 bits per channel) and organized in sequential lines, starting from the top left corner.\n
		 If the cursor creation fails, the previous cursor (if there was one) will remain active.

		 \param[in] pixels The 32-bit pixel data (8 bits per channel)
		 \param[in] size The ae::Vector2 containing the image size
		 \param[in] hotspot The ae::Vector2 containing the position of the cursor's action

		 \par Example:
		 \code
		 ae::Cursor cursor;

		 // 16x16 image with 4 color channels (RGBA)
		 std::array<unsigned char, 16 * 16 * 4> pixels;
		 pixels.fill(0xff);

		 // Cursor with white 16x16 image with its position of action at the top left corner
		 cursor.createCustom(pixels.data(), ae::Vector2i(16, 16), ae::Vector2i(0, 0));
		 \endcode

		 \sa createStandard()

		 \since v0.7.0
		*/
		void createCustom(unsigned char* pixels, const Vector2i& size, const Vector2i& hotspot);
		/*!
		 \brief Sets a standard cursor by providing the cursor type.
		 \details If the cursor creation fails, the previous cursor (if there was one) will remain active.

		 \param[in] type The ae::Cursor::Type of the cursor

		 \par Example:
		 \code
		 ae::Cursor cursor;
		 cursor.createStandard(ae::Cursor::Type::Hand);
		 \endcode

		 \sa createCustom()

		 \since v0.7.0
		*/
		void createStandard(Type type);
		/*!
		 \brief Retrieves the handle to the GLFW cursor.
		 \details This method is used internally, the API user shouldn't have need of this handle.

		 \return The pointer to the GLFW cursor handle

		 \since v0.7.0
		*/
		inline GLFWcursor* const getHandle() const noexcept { return mCursor; }
	private:
		// Private method(s)
		/*!
		 \brief Destroys the active cursor (if there was one) and sets the new cursor as active.

		 \param[in] cursor The pointer to the new cursor's handle

		 \since v0.7.0
		*/
		void replaceCursor(GLFWcursor* cursor);

	private:
		// Private member(s)
		GLFWcursor* mCursor; //!< The GLFW handle to the cursor
	};
}
#endif // Aeon_Window_Cursor_H_

/*!
 \class ae::Cursor
 \ingroup window

 The ae::Cursor class lets the API user to create a personalized cursor. It can
 be created either by directly providing the pixel data or by selecting one of
 the standard cursor types provided by the OS.

 It's strongly recommended to let the ae::Window instance handle the
 standard-themed OS cursors, but personalized cursors have local scope, they
 therefore need to be stored, otherwise the active cursor will revert to the
 default one.

 Usage:
 \code
 // The 'mWindow' member is a protected member of the ae::State class
 mWindow.setCursor(ae::Cursor::Type::Crosshair);
 \endcode

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2021.07.10
 \copyright MIT License
*/