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

#ifndef Aeon_Graphics_RectangleShape_H_
#define Aeon_Graphics_RectangleShape_H_

#include <AEON/Graphics/internal/Shape.h>

namespace ae
{
	/*!
	 \brief Class representing a 2D rectangle.
	*/
	class _NODISCARD AEON_API RectangleShape : public Shape
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::RectangleShape by providing an optional \a size.
		 \details Creates a rectangle with no texture and a white fill color.

		 \param[in] size The size of the rectangle

		 \par Example:
		 \code
		 auto rectangle = std::make_unique<ae::RectangleShape>(ae::Vector2f(50.f, 25.f));
		 \endcode

		 \since v0.4.0
		*/
		explicit RectangleShape(const Vector2f& size = Vector2f());
	public:
		// Public method(s)
		/*!
		 \brief Sets the size of the ae::RectangleShape.

		 \param[in] size The new size of the rectangle

		 \par Example:
		 \code
		 auto rectangle = std::make_unique<ae::RectangleShape>();
		 rectangle->setSize(ae::Vector2f(50.f, 25.f));
		 \endcode

		 \sa getSize()

		 \since v0.4.0
		*/
		void setSize(const Vector2f& size) noexcept;
		/*!
		 \brief Sets the size of the ae::RectangleShape.

		 \param[in] sizeX The new horizontal size of the rectangle
		 \param[in] sizeY The new vertical size of the rectangle

		 \par Example:
		 \code
		 auto rectangle = std::make_unique<ae::RectangleShape>();
		 rectangle->setSize(50.f, 25.f);
		 \endcode

		 \sa getSize()

		 \since v0.4.0
		*/
		void setSize(float sizeX, float sizeY) noexcept;
		/*!
		 \brief Retrieves the size of the ae::RectangleShape.

		 \return The 2-dimensional ae::Vector containing the rectangle's size

		 \par Example:
		 \code
		 auto rectangle = std::make_unique<ae::RectangleShape>(ae::Vector2f(50.f, 25.f));
		 ...
		 const ae::Vector2f& rectangleSize = rectangle->getSize();
		 \endcode

		 \sa setSize()

		 \since v0.4.0
		*/
		_NODISCARD const Vector2f& getSize() const noexcept;

	private:
		// Private virtual method(s)		
		/*!
		 \brief Updates the stored vertices' positions.
		 \details Called when the vertices were attempted to be retrieved and the size had changed.

		 \sa updateUV()

		 \since v0.4.0
		*/
		virtual void updatePosition() override final;
		/*!
		 \brief Updates the stored vertices' uv coordinates.
		 \details Called when the vertices were attempted to be retrieved and the texture or the texture rect had changed.

		 \sa updatePosition()

		 \since v0.4.0
		*/
		virtual void updateUV() override final;
		/*!
		 \brief Updates the ae::RectangleShape's stored indices.
		 \details Mostly used for first-time setup of the indices.

		 \since v0.4.0
		*/
		virtual void updateIndices() override final;

	private:
		// Private member(s)
		Vector2f mSize; //!< The size of the rectangle
	};
}
#endif // Aeon_Graphics_RectangleShape_H_

/*!
 \class ae::RectangleShape
 \ingroup graphics

 The ae::RectangleShape class is used to represent a 2D rectangle, instances of
 this class possess a size, a color and can hold an optional texture

 The ae::RectangleShape and ae::Sprite instances behave very similarly, but the
 main difference is that an ae::RectangleShape has an explicit size member
 which remains static even when the texture or the texture rect are modified.

 \author Filippos Gleglakos
 \version v0.4.0
 \date 2020.05.26
 \copyright MIT License
*/