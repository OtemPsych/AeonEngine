// MIT License
// 
// Copyright(c) 2019-2020 Filippos Gleglakos
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

#ifndef Aeon_Graphics_Color_H_
#define Aeon_Graphics_Color_H_

#include <AEON/Math/Vector4.h>

namespace ae
{
	/*!
	 \brief The utility class used to manipulate RGBA colors.
	*/
	class _NODISCARD AEON_API Color
	{
	public:
		// Public static member(s)
		static const Color Black;       //!< The predefined black color
		static const Color White;       //!< The predefined white color
		static const Color Red;         //!< The predefined red color
		static const Color Green;       //!< The predefined green color
		static const Color Blue;        //!< The predefined blue color
		static const Color Yellow;      //!< The predefined yellow color
		static const Color Magenta;     //!< The predefined magenta color
		static const Color Cyan;        //!< The predefined cyan color
		static const Color Transparent; //!< The predefined transparent color

	public:
		// Public member(s)
		uint8_t r; //!< The red component
		uint8_t g; //!< The green component
		uint8_t b; //!< The blue component
		uint8_t a; //!< The alpha (opacity) component

	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details Constructs an opaque black color equivalent to ae::Color(0, 0, 0, 255).
		 
		 \since v0.3.0
		*/
		Color() noexcept;
		/*!
		 \brief Constructs the ae::Color by providing the 4 RGBA components.
		 \details The \a alpha component is optional and defaults to 255 (opaque) if not provided.

		 \param[in] red The red component (in the range [0,255])
		 \param[in] green The green component (in the range [0,255])
		 \param[in] blue The blue component (in the range [0,255])
		 \param[in] alpha The optional opacity component (in the range [0,255]), 255 by default

		 \par Example:
		 \code
		 ae::Color semiTranparentYellow(255, 255, 0, 128);
		 \endcode

		 \since v0.3.0
		*/
		Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255) noexcept;
		/*!
		 \brief Constructs the ae::Color by providing a 32-bit \a hexcode.

		 \param[in] hexcode The hexadecimal number containing the RGBA components (in that order)

		 \par Example:
		 \code
		 // r (ff = 255), g (ff = 255), b (00 = 0), a (80 = 128)
		 ae::Color yellow(0xffff0080);
		 \endcode

		 \since v0.3.0
		*/
		explicit Color(uint32_t hexcode) noexcept;
	public:
		// Public operator(s)
		/*!
		 \brief Addition operator.
		 \details Performs a memberwise addition of the caller's and the \a other's components.\n
		 The components exceeding 255 are blocked at 255.

		 \param[in] other The ae::Color of which its components will be added with the caller's ones

		 \return An ae::Color containing the sum of the caller's and the \a other's components

		 \par Example:
		 \code
		 ae::Color color1(150, 35, 20);
		 ae::Color color2(80, 200, 135);
		 ae::Color color3 = color1 + color2;
		 \endcode

		 \sa operator-(), operator*()

		 \since v0.3.0
		*/
		_NODISCARD Color operator+(const Color& other) const noexcept;
		/*!
		 \brief Subtraction operator.
		 \details Performs a memberwise subtraction of the caller's and the \a other's components.\n
		 The components inferior to 0 are blocked at 0.

		 \param[in] other The ae::Color of which its components will subtract from the caller's ones

		 \return An ae::Color containing the difference of the caller's and the \a other's components

		 \par Example:
		 \code
		 ae::Color color1(150, 35, 20);
		 ae::Color color2(80, 200, 135);
		 ae::Color color3 = color1 - color2;
		 \endcode

		 \sa operator+(), operator*()

		 \since v0.3.0
		*/
		_NODISCARD Color operator-(const Color& other) const noexcept;
		/*!
		 \brief Multiplication operator.
		 \details Performs a memberwise multiplication of the caller's and the \a other's components.\n
		 The components will be divided by 255 so that they remain within the range [0,255].

		 \param[in] other The ae::Color of which its components will be multiplied (modulated) with the caller's ones

		 \return An ae::Color containing the modulated color of the caller's and the \a other's components

		 \par Example:
		 \code
		 ae::Color color1(150, 35, 20);
		 ae::Color color2(80, 200, 135);
		 ae::Color color3 = color1 * color2;
		 \endcode

		 \sa operator+(), operator-()

		 \since v0.3.0
		*/
		_NODISCARD Color operator*(const Color& other) const noexcept;

		/*!
		 \brief Addition and assignment operator.
		 \details Performs a memberwise addition of the caller's and the \a other's components, and assigns the sum to the caller.\n
		 The components exceeding 255 are blocked at 255.

		 \param[in] other The ae::Color of which its components will be added with the caller's ones

		 \return The caller ae::Color containing the sum of the caller's and the \a other's components, these calls can be chained together one after the other

		 \par Example:
		 \code
		 ae::Color color1(150, 35, 20);
		 ae::Color color2(80, 200, 135);
		 color1 += color2;
		 \endcode

		 \sa operator-=(), operator*=()

		 \since v0.3.0
		*/
		Color& operator+=(const Color& other) noexcept;
		/*!
		 \brief Subtraction and assignment operator.
		 \details Performs a memberwise subtraction of the caller's and the \a other's components, and assigns the difference to the caller.\n
		 The components inferior to 0 are blocked at 0.

		 \param[in] other The ae::Color of which its components will subtract from the caller's ones

		 \return The caller ae::Color containing the difference of the caller's and the \a other's components, these calls can be chained together one after the other

		 \par Example:
		 \code
		 ae::Color color1(150, 35, 20);
		 ae::Color color2(80, 200, 135);
		 color1 -= color2;
		 \endcode

		 \sa operator+=(), operator*=()

		 \since v0.3.0
		*/
		Color& operator-=(const Color& other) noexcept;
		/*!
		 \brief Multiplication and assignment operator.
		 \details Performs a memberwise multiplication of the caller's and the \a other's components, and assigns the product to the caller.\n
		  The components will be divided by 255 so that they remain within the range [0,255].

		 \param[in] other The ae::Color of which its components will be multiplied (modulated) with the caller's ones

		 \return The caller ae::Color containing the modulated color of the caller's and the \a other's components, these calls can be chained together one after the other

		 \par Example:
		 \code
		 ae::Color color1(150, 35, 20);
		 ae::Color color2(80, 200, 135);
		 color1 *= color2;
		 \endcode

		 \sa operator+=(), operator-=()

		 \since v0.3.0
		*/
		Color& operator*=(const Color& other) noexcept;

		/*!
		 \brief Equality operator.
		 \details Checks if the caller's and the \a other's respective components are equal.

		 \param[in] other The ae::Color that will be compared with the caller ae::Color

		 \return True if all of the caller's and the \a other's components are respectively equal, false otherwise

		 \par Example:
		 \code
		 ae::Color red1 = ae::Color::Red;
		 ae::Color red2(255, 0, 0);
		 if (red1 == red2) {
			...
		 }
		 \endcode

		 \sa operator!=()

		 \since v0.3.0
		*/
		_NODISCARD bool operator==(const Color& other) const noexcept;
		/*!
		 \brief Inequality operator.
		 \details Checks if the caller's and the \a other's respective components are inequal.

		 \param[in] other The ae::Color that will be compared with the caller ae::Color

		 \return True if at least one of the caller's and the \a other's components is inequal, false otherwise

		 \par Example:
		 \code
		 ae::Color red = ae::Color::Red;
		 ae::Color blue = ae::Color::Blue;
		 if (red != blue) {
			...
		 }
		 \endcode

		 \sa operator==()

		 \since v0.3.0
		*/
		_NODISCARD bool operator!=(const Color& other) const noexcept;
	public:
		// Public method(s)
		/*!
		 \brief Retrieves the ae::Color in hexadecimal.

		 \return The color converted to hexadecimal code as an unsigned 32-bit integer

		 \par Example:
		 \code
		 ae::Color yellow(255, 255, 0, 255);
		 uint32_t yellowHexcode = yellow.toHexcode();
		 \endcode

		 \since v0.3.0
		*/
		_NODISCARD uint32_t toHexcode() const noexcept;
		/*!
		 \brief Retrieves the normalized color of the ae::Color.
		 \details The ae::Color's components will be normalized from integers in the range [0,255] to floating-point values in the range [0,1].\n
		 This method is primarily used internally to send the color to the shaders.

		 \return A 4-dimensional ae::Vector containing the normalized components of the ae::Color situated in the range [0,1]

		 \par Example:
		 \code
		 ae::Color yellow(255, 255, 0);
		 ae::Vector4f yellowVec4f = yellow.normalize(); // (1.f, 1.f, 0.f, 1.f)
		 \endcode

		 \since v0.3.0
		*/
		_NODISCARD Vector4f normalize() const noexcept;
	};
}
#endif // Aeon_Graphics_Color_H_

/*!
 \class ae::Color
 \ingroup graphics

 The ae::Color class is very simple, it's comprised of 4 components:
 \li r (Red)
 \li g (Green)
 \li b (Blue)
 \li a (Alpha - Opacity)

 Each component is a public member situated in the range [0,255].
 Therefore, all colors can very easily be constructed and manipulated.

 The 4th color components which is the 'alpha' component represents the opacity
 of the color. A color with an alpha value of 255 will be completely opaque,
 whereas an alpha value of 0 will render the color completely transparent.

 The most common colors are defined as static members:
 \code
 ae::Color black = ae::Color::Black;
 ae::Color red = ae::Color::Red;
 \endcode

 Colors can be added together and multiplied together (modulated) by using the
 overloaded operators + and *.

 Usage example:
 \code
 ae::Color color(255, 0, 0); // red
 color.r = 0;                // black
 color.b = 128;              // dark blue
 \endcode

 \author Filippos Gleglakos
 \version v0.3.0
 \date 2019.07.14
 \copyright MIT License
*/