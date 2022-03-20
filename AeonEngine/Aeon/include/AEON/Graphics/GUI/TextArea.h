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

#ifndef Aeon_Graphics_GUI_TextArea_H_
#define Aeon_Graphics_GUI_TextArea_H_

#include <AEON/Graphics/GUI/internal/Widget.h>
#include <AEON/Graphics/RectangleShape.h>
#include <AEON/Graphics/RenderTexture.h>

namespace ae
{
	// Forward declaration(s)
	class Text;

	class _NODISCARD AEON_API TextArea : public Widget<RectangleShape>
	{
	public:
		enum Property {
			MultiLine = 1 << 0, //!< The text area supports multiple lines of text
			WordWrap  = 1 << 1, //!< Wraps words to the next line which disables horizontal resizing (MultiLine must be set as well)
			Resize    = 1 << 2, //!< Resizes the text area horizontally (if WordWrap isn't set) and vertically to fit the text, up to the max size specified
		};

	public:
		TextArea();
	public:
		void setProperties(uint32_t properties);
		//void setMaxSize(const Vector2f& size);
		void setText(const std::string& text);
		Text* const getLine(size_t index = 0) const noexcept;
		//size_t getLineCount() const noexcept;

		virtual void updateSelf(const Time& dt) override final;
		virtual void handleEventSelf(Event* const event) override final;
	private:
		void renderLines();
		void separateLines();
		void handleResizing();
		void handleImposedReturns();

		virtual void enableState(uint32_t state) override final;

	private:
		RenderTexture      mContent;
		std::string        mText;
		std::vector<Text*> mLines;
		Vector2f           mMaxSize;
		uint32_t           mProperties;
		bool               mUpdateContent;
	};
}
#endif // Aeon_Graphics_GUI_TextArea_H_