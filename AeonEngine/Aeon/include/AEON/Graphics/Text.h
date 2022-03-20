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

#pragma once

#include <AEON/Graphics/Actor.h>
#include <AEON/Graphics/Color.h>

namespace ae
{
	// Forward declaration(s)
	struct Glyph;
	class Font;

	/*!
	 \brief The class representing a renderable string of text.
	*/
	class AEON_API Text : public Actor
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details Sets a white color and a character size of 48.

		 \since v0.7.0
		*/
		Text();
		/*!
		 \brief Copy constructor.

		 \param[in] copy The ae::Text that will be copied

		 \since v0.7.0
		*/
		Text(const Text& copy);
		/*!
		 \brief Move constructor.

		 \since v0.7.0
		*/
		Text(Text&&) noexcept = default;
		~Text();

		// Public operator(s)
		/*!
		 \brief Assignment operator.

		 \return The caller ae::Text

		 \since v0.7.0
		*/
		Text& operator=(const Text&) = default;
		/*!
		 \brief Move assignment operator.

		 \return The caller ae::Text

		 \since v0.7.0
		*/
		Text& operator=(Text&&) noexcept = default;

		// Public method(s)
		/*!
		 \brief Sets the ae::Font that the ae::Text will use.
		 \note An error may occur if a font isn't set.

		 \param[in] font An ae::Font which the ae::Text will use to extract the necessary glyphs

		 \par Example:
		 \code
		 // Create a font instance and load it a font from a file
		 ae::Font font;
		 font.loadFromFile("Assets/Fonts/Arial.ttf");

		 // Create a text instance and set the font previously created
		 auto text = std::make_unique<ae::Text>();
		 text->setFont(font);
		 \endcode

		 \sa getFont()

		 \since v0.7.0
		*/
		void setFont(Font& font);
		/*!
		\brief Sets the string of characters that the ae::Text will hold.

		\param[in] text A string of characters to assign to the ae::Text

		\par Example:
		\code
		auto text = std::make_unique<ae::Text>();
		...
		text->setText("Example text");
		\endcode

		\sa setFont(), getText()

		\since v0.7.0
		*/
		void setText(const std::string& text);
		/*!
		 \brief Sets the size of the ae::Text's characters.
		 \details It's preferable to increase the text's character size rather than scaling it as the text quality will be significantly better.

		 \param[in] characterSize The size of the ae::Text's glyphs

		 \par Example:
		 \code
		 auto text = std::make_unique<ae::Text>();
		 ...
		 text->setCharacterSize(24);
		 \endcode

		 \sa getCharacterSize()

		 \since v0.7.0
		*/
		void setCharacterSize(uint32_t characterSize);
		/*!
		 \brief Sets the color of the ae::Text's characters.
		 
		 \param[in] color The ae::Color of the ae::Text's glyphs

		 \par Example:
		 \code
		 auto text = std::make_unique<ae::Text>();
		 ...
		 text->setColor(ae::Color::Cyan);
		 \endcode

		 \sa getColor()

		 \since v0.7.0
		*/
		void setColor(const Color& color);
		/*!
		 \brief Calculates and retrieves the position of the character at the \a index provided.
		 \details The position retrieved is situated at the top-left and is in model coordinates.

		 \param[in] index The character's position within the text string to retrieve

		 \return The position of the character in model coordinates

		 \par Example:
		 \code
		 auto text = std::make_unique<ae::Text>();
		 text->setString("Some text");
		 ...
		 Vector2f charPos = text->findCharPos(2);
		 \endcode

		 \since v0.7.0
		*/
		Vector2f findCharPos(size_t index) const;
		/*!
		 \brief Retrieves the font.

		 \return The ae::Font assigned to the ae::Text

		 \par Example:
		 \code
		 auto text = std::make_unique<ae::Text>();
		 ...

		 const Font* const font = text->getFont();
		 \endcode

		 \sa setFont()

		 \since v0.7.0
		*/
		[[nodiscard]] inline const Font* const getFont() const noexcept { return mFont; }
		/*!
		 \brief Retrieves the string of characters.

		 \return The string containing the ae::Text's glyphs

		 \par Example:
		 \code
		 auto text = std::make_unique<ae::Text>();
		 ...

		 const std::string& textStr = text->getText();
		 \endcode

		 \sa setText()

		 \since v0.7.0
		*/
		[[nodiscard]] inline const std::string& getText() const noexcept { return mText; }
		/*!
		 \brief Retrieves the character size.

		 \return The size of the ae::Text's glyphs

		 \par Example:
		 \code
		 auto text = std::make_unique<ae::Text>();
		 ...

		 uint32_t characterSize = text->getCharacterSize();
		 \endcode

		 \sa setCharacterSize()

		 \since v0.7.0
		*/
		[[nodiscard]] inline uint32_t getCharacterSize() const noexcept { return mCharacterSize; }
		/*!
		 \brief Retrieves the color.

		 \return The ae::Color of the ae::Text's glyphs

		 \par Example:
		 \code
		 auto text = std::make_unique<ae::Text>();
		 ...

		 const ae::Color& color = text->getColor();
		 \endcode

		 \sa setColor()

		 \since v0.7.0
		*/
		[[nodiscard]] inline const Color& getColor() const noexcept { return mColor; }

		/*!
		 \brief Updates the vertices' texture coordinates.
		 \details This method is called automatically by the ae::FontManager whenever the associated font's texture atlas has been updated.

		 \sa updatePos(), updateColor()

		 \since v0.7.0
		*/
		void updateUV();
	private:
		// Private method(s)
		/*!
		 \brief Retrieves the appropriate glyphs and updates the vertices' positions.

		 \sa updateUV(), updateIndices(), updateColor()

		 \since v0.7.0
		*/
		void updatePos();
		/*!
		 \brief Updates the indices if necessary.

		 \sa updatePos()

		 \since v0.7.0
		*/
		void updateIndices();
		/*!
		 \brief Updates the vertices' color.

		 \sa updatePos(), updateUV()

		 \since v0.6.0
		*/
		void updateColor();

		// Private virtual method(s)
		/*!
		 \brief Sets the appropriate render states and sends the ae::Text's glyphs to the renderer.

		 \param[in] states The ae::RenderStates to apply to the ae::Text (transform, blend mode, shader, texture)

		 \since v0.7.0
		*/
		virtual void renderSelf(RenderStates states) const override final;

	private:
		// Private member(s)
		std::string               mText;          //!< The text to render
		std::vector<const Glyph*> mGlyphs;        //!< The collection of glyphs necessary to render the text
		Color                     mColor;         //!< The color of the text
		Font*                     mFont;          //!< The font used to display the glyphs
		uint32_t                  mCharacterSize; //!< The font size to use
	};
}

/*!
 \class ae::Text
 \ingroup graphics

 The ae::Text class is used to render a string of text to a render target. A
 font loaded from a disk can be assigned to it, it has a character size and the
 option to apply a color to the text.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2022.02.10
 \copyright MIT License
*/