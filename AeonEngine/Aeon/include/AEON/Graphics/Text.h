// MIT License
// 
// Copyright(c) 2019-2021 Filippos Gleglakos
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

#ifndef Aeon_Graphics_Text_H_
#define Aeon_Graphics_Text_H_

#include <AEON/Graphics/Actor2D.h>

namespace ae
{
	// Forward declaration(s)
	struct Glyph;
	class Font;

	/*!
	 \brief The class representing a renderable string of text.
	*/
	class _NODISCARD AEON_API Text : public Actor2D
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details Sets a white color and a character size of 48.

		 \since v0.6.0
		*/
		Text();
		/*!
		 \brief Copy constructor.

		 \param[in] copy The ae::Text that will be moved

		 \since v0.6.0
		*/
		Text(const Text& copy) = default;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::Text that will be moved

		 \since v0.5.0
		*/
		Text(Text&& rvalue) noexcept;
	public:
		// Public operator(s)
		/*!
		 \brief Assignment operator.

		 \param[in] other The ae::Text that will be copied

		 \return The caller ae::Text

		 \since v0.6.0
		*/
		Text& operator=(const Text& other) = default;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::Text that will be moved

		 \return The caller ae::Text

		 \since v0.5.0
		*/
		Text& operator=(Text&& rvalue) noexcept;
	public:
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

		 \since v0.6.0
		*/
		void setFont(Font& font) noexcept;
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

		\since v0.6.0
		*/
		void setText(const std::string& text) noexcept;
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

		 \since v0.6.0
		*/
		void setCharacterSize(unsigned int characterSize) noexcept;
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

		 \since v0.6.0
		*/
		void setColor(const Color& color) noexcept;
		/*!
		 \brief Calculates and retrieves the position of the character at the \a index provided.
		 \details The position retrieved is situated at the top-left and is in model coordinates.
		 \note The update() method needs to be called if this method is called during initialization.

		 \param[in] index The character's position within the text string to retrieve

		 \return The position of the character in model coordinates

		 \par Example:
		 \code
		 auto text = std::make_unique<ae::Text>();
		 text->setString("Some text");
		 ...
		 Vector2f charPos = text->findCharPos(2);
		 \endcode

		 \since v0.6.0
		*/
		Vector2f findCharPos(size_t index) const;
		/*!
		 \brief Retrieves the ae::Text's font.

		 \return The ae::Font assigned to the ae::Text

		 \par Example:
		 \code
		 auto text = std::make_unique<ae::Text>();
		 ...

		 const Font* const font = text->getFont();
		 \endcode

		 \sa setFont()

		 \since v0.6.0
		*/
		_NODISCARD const Font* const getFont() const noexcept;
		/*!
		 \brief Retrieves the ae::Text's string of characters.

		 \return The string containing the ae::Text's glyphs

		 \par Example:
		 \code
		 auto text = std::make_unique<ae::Text>();
		 ...

		 const std::string& textStr = text->getText();
		 \endcode

		 \sa setText()

		 \since v0.6.0
		*/
		_NODISCARD const std::string& getText() const noexcept;
		/*!
		 \brief Retrieves the ae::Text's character size.

		 \return The size of the ae::Text's glyphs

		 \par Example:
		 \code
		 auto text = std::make_unique<ae::Text>();
		 ...

		 unsigned int characterSize = text->getCharacterSize();
		 \endcode

		 \sa setCharacterSize()

		 \since v0.6.0
		*/
		_NODISCARD unsigned int getCharacterSize() const noexcept;
		/*!
		 \brief Retrieves the ae::Text's color.

		 \return The ae::Color of the ae::Text's glyphs

		 \par Example:
		 \code
		 auto text = std::make_unique<ae::Text>();
		 ...

		 const ae::Color& color = text->getColor();
		 \endcode

		 \sa setColor()

		 \since v0.6.0
		*/
		_NODISCARD const Color& getColor() const noexcept;

		// Public virtual method(s)
		/*!
		 \brief Retrieves the ae::Text's model bounds.
		 \details The model bounds are equivalent to the bounds of the ae::Text's string of characters.

		 \return A 2-dimensional ae::Box containing the position and the size of the ae::Text's model bounds

		 \par Example:
		 \code
		 auto text = std::make_unique<ae::Text>();
		 ...

		 Box2f bounds = text->getModelBounds();
		 \endcode

		 \since v0.6.0
		*/
		_NODISCARD virtual Box2f getModelBounds() const override final;
	private:
		// Private method(s)
		/*!
		 \brief Retrieves the appropriate glyphs and updates the vertices' positions.

		 \sa updateUV(), updateIndices(), updateColor()

		 \since v0.6.0
		*/
		void updatePos();
		/*!
		 \brief Updates the vertices' texture coordinates.

		 \sa updatePos(), updateColor()

		 \since v0.6.0
		*/
		void updateUV();
		/*!
		 \brief Updates the indices if necessary.

		 \sa updatePos()

		 \since v0.6.0
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
		 \brief Checks if the ae::Text's assigned font has been modified.
		 \details If the font has been modified, the vertices' positions will be modified.

		 \param[in] event The polled input ae::Event

		 \since v0.6.0
		*/
		virtual void handleEventSelf(Event* const event) override final;
		/*!
		 \brief Updates the ae::Text's properties if necessary.

		 \param[in] dt The time difference between this frame and the last

		 \sa updatePosUVs(), updateColors()

		 \since v0.6.0
		*/
		virtual void updateSelf(const Time& dt) override final;
		/*!
		 \brief Sets the appropriate render states and sends the ae::Text's glyphs to the renderer.

		 \param[in] states The ae::RenderStates to apply to the ae::Text (transform, blend mode, shader, texture)

		 \since v0.6.0
		*/
		virtual void renderSelf(RenderStates states) const override final;

	private:
		// Private member(s)
		std::string               mText;          //!< The text to render
		std::vector<const Glyph*> mGlyphs;        //!< The collection of glyphs necessary to render the text
		Box2f                     mModelBounds;   //!< The local model bounds of the text
		Color                     mColor;         //!< The color of the text
		Font*                     mFont;          //!< The font used to display the glyphs
		unsigned int              mCharacterSize; //!< The font size to use
		bool                      mUpdatePos;     //!< Whether the vertices' positions need to be updated
		bool                      mUpdateUV;      //!< Whether the vertices' textures coordinates need to be updated
		bool                      mUpdateColor;   //!< Whether the vertices' colors need to be updated
	};
}
#endif // Aeon_Graphics_Text_H_

/*!
 \class ae::Text
 \ingroup graphics

 The ae::Text class is used to render a string of text to a render target. A
 font loaded from a disk can be assigned to it, it has a character size and the
 option to apply a color to the text.

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2020.09.11
 \copyright MIT License
*/