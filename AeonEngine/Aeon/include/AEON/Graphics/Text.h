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

		 \since v0.5.0
		*/
		Text();
		/*!
		 \brief Deleted copy constructor.

		 \since v0.5.0
		*/
		Text(const Text&) = delete;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::Text that will be moved

		 \since v0.5.0
		*/
		Text(Text&& rvalue) noexcept;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.5.0
		*/
		Text& operator=(const Text&) = delete;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::Text that will be moved

		 \return The caller ae::Text

		 \since v0.5.0
		*/
		Text& operator=(Text&& rvalue) noexcept;
	public:
		// Public method(s)
		void setFont(Font& font) noexcept;
		void setText(const std::string& text) noexcept;
		void setCharacterSize(unsigned int characterSize) noexcept;
		void setColor(const Color& color) noexcept;
		_NODISCARD const std::string& getText() const noexcept;
		_NODISCARD unsigned int getCharacterSize() const noexcept;
		_NODISCARD const Color& getColor() const noexcept;

		// Public virtual method(s)
		_NODISCARD virtual Box2f getModelBounds() const override final;
	private:
		// Private method(s)
		void updatePosUVs();
		void updateColors();
		bool updateVertices();

		// Private virtual method(s)
		virtual void handleEventSelf(Event* const event) override final;
		virtual void updateSelf(const Time& dt) override final;
		virtual void renderSelf(RenderStates states) const override final;

	private:
		// Private member(s)
		std::string               mText;          //!< The text to render
		std::vector<const Glyph*> mGlyphs;        //!< The collection of glyphs necessary to render the text
		Box2f                     mModelBounds;   //!< The local model bounds of the text
		Color                     mColor;         //!< The color of the text
		Font*                     mFont;          //!< The font used to display the glyphs
		unsigned int              mCharacterSize; //!< The font size to use
		bool                      mUpdatePosUVs;  //!< Whether the vertices' positions and texture coordinates need to be updated
		bool                      mUpdateColors;  //!< Whether the vertices' colors need to be updated
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
 \version v0.5.0
 \date 2020.06.10
 \copyright MIT License
*/