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

#ifndef Aeon_Graphics_GUI_Textbox_H_
#define Aeon_Graphics_GUI_Textbox_H_

#include <AEON/Graphics/GUI/internal/Widget.h>
#include <AEON/Graphics/RectangleShape.h>
#include <AEON/Graphics/RenderTexture.h>

namespace ae
{
	// Forward declaration(s)
	class Text;
	class Sprite;

	/*!
	 \brief Class representing a GUI textbox.
	 \details The Idle, Hover and Click states need to be configured.
	*/
	class AEON_API Textbox : public Widget<RectangleShape>
	{
	public:
		// Public constructor(s)
		Textbox();
	public:
		// Public method(s)
		void focus();
		inline RectangleShape& getSelectionRegion() noexcept { return *mSelectedRegion; }
		inline Text& getText() noexcept { return *mText; }
		inline Text& getPlaceholder() noexcept { return *mPlaceholder; }
		inline void setCharacterCount(int count) noexcept { mCharacterCount = count; }
		inline RectangleShape& getCaret() noexcept { return *mCaret.first; }

		// Public static method(s)
		static void enableIBeamCursor(bool flag);
	private:
		// Private method(s)
		bool addCharacters(const std::string& str);
		bool removeCharacters();
		bool removeCharacter(bool backwards);
		size_t findClosestCharacterIndex();
		void updateCaretPosition();
		void updateSelectedRegion();
		void updateSelectedRegionOnMouseMove();
		void updateCaretIndexOnMouseClick();
		void updateCaretFade(bool forceShow);

		// Private virtual method(s)
		virtual void enableState(uint32_t state) override final;
		virtual void updateSelf(const Time& dt) override final;
		virtual void handleEventSelf(Event* const event) override final;
		
	private:
		// Private member(s)
		RectangleShape*                        mSelectedRegion;
		bool                                   mSelectingText;
		size_t                                 mSelectionEndIndex;
		Text*                                  mText;
		Text*                                  mPlaceholder;
		std::pair<ae::RectangleShape*, size_t> mCaret;
		Time                                   mCaretFadeTime;
		Time                                   mCaretFadeElapsedTime;
		int                                    mCharacterCount;

		// Private static member(s)
		static bool                            mEnableIBeamCursor;
		static std::pair<bool, bool>           mShowIBeamCursor;
	};
}
#endif // Aeon_Graphics_GUI_Textbox_H_

/*!
 \class ae::Textbox
 \ingroup graphics

 The ae::Textbox class provided typical functionality of a GUI textbox, such as
 typing text

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2021.07.18
 \copyright MIT License
*/