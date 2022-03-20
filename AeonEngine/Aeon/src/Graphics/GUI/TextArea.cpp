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

#include <AEON/Graphics/GUI/TextArea.h>

#include <AEON/Graphics/Text.h>
#include <AEON/Graphics/BasicRenderer2D.h>

namespace ae
{
	TextArea::TextArea()
		: Widget()
		, mContent()
		, mText("")
		, mLines()
		, mMaxSize(0.f, 0.f)
		, mProperties(0)
		, mUpdateContent(true)
	{
		auto firstLine = std::make_unique<Text>();
		mLines.emplace_back(firstLine.get());
		getState(getActiveState()).attachChild(std::move(firstLine));
	}

	void TextArea::setProperties(uint32_t properties)
	{
		mProperties = properties;
	}

	void TextArea::setText(const std::string& text)
	{
		// Check if the same text is being assigned
		if (mText == text) {
			return;
		}

		// Remove all current lines except the first
		if (mLines.size() > 1)
		{
			ae::RectangleShape& activeState = getState(getActiveState());
			for (size_t i = 1; i < mLines.size(); ++i) {
				activeState.detachChild(*mLines[i]);
			}
			mLines.erase(mLines.begin() + 1, mLines.end());
		}

		mText = text;
		mLines.front()->setText(text);

		// Separate the new text into several lines based on the properties set
		separateLines();

		getComponent<Render2DComponent>()->setDirty(true);
	}

	Text* const TextArea::getLine(size_t index) const noexcept
	{
		// TODO: Check index
		return mLines[index];
	}

	void TextArea::updateSelf(const Time& dt)
	{
		Widget::updateSelf(dt);

		Render2DComponent* const renderComponent = getComponent<Render2DComponent>();
		if (renderComponent->isDirty())
		{
			// Set the render texture's clear color to the active state's fill color
			mContent.setClearColor(getState(getActiveState()).getFillColor());

			mUpdateContent = true;
			renderComponent->setDirty(false);
		}

		if (mUpdateContent) {
			renderLines();
			mUpdateContent = false;
		}
	}

	void TextArea::handleEventSelf(Event* const event)
	{
		// Check if the text area has been disabled
		const State ACTIVE_STATE = getActiveState();
		if (ACTIVE_STATE == State::Disabled) {
			return;
		}

		// Check if the text area is being hovered over
		if (event->type == Event::Type::MouseMoved && (ACTIVE_STATE == State::Idle || ACTIVE_STATE == State::Hover)) {
			auto mouseMoveEvent = event->as<MouseMoveEvent>();

			// Set the active state to hover or idle based on mouse position
			const bool HOVERED_OVER = isHoveredOver(mouseMoveEvent->position);
			if (HOVERED_OVER && ACTIVE_STATE == State::Idle) {
				enableState(State::Hover);
			}
			else if (!HOVERED_OVER && ACTIVE_STATE == State::Hover) {
				enableState(State::Idle);
			}
		}
	}

	// Private method(s)
	void TextArea::renderLines()
	{
		// Recreate the content area if the optimal size has changed
		const Vector2f& currentSize = getState(getActiveState()).getSize();
		const Vector2i ACTUAL_SIZE(static_cast<int>(Math::ceil(currentSize.x)), static_cast<int>(Math::ceil(currentSize.y)));
		if (ACTUAL_SIZE != mContent.getFramebufferSize()) {
			mContent.create(ACTUAL_SIZE.x, ACTUAL_SIZE.y);
		}

		// Enable automatic rendering for the lines (so that they may be rendered onto the content area)
		for (Text* const line : mLines) {
			line->activateFunctionality(Func::Render, Target::Self, true);
		}

		// Render the lines onto the content area
		BasicRenderer2D& renderer = BasicRenderer2D::getInstance();
		renderer.beginScene(mContent);
		for (Text* const line : mLines) {
			line->render(RenderStates());
		}
		renderer.endScene();

		// Set the content texture to the text area's states
		const Texture2D* const contentTexture = mContent.getTexture();
		for (size_t i = 0; i < State::StateCount; ++i) {
			getState(static_cast<State>(i)).setTexture(contentTexture, true);
		}

		// Disable automatic rendering for the lines
		for (Text* const line : mLines) {
			line->activateFunctionality(Func::Render, Target::Self, false);
		}
	}

	void TextArea::handleResizing()
	{
		if (mProperties & Property::Resize)
		{
			// Resize horizontally if word-wrapping isn't enabled
			if (!(mProperties & Property::WordWrap)) {

			}
		}
	}

	void TextArea::separateLines()
	{
		// Create separate lines if any imposed returns are found
		if (mProperties & Property::MultiLine) {
			handleImposedReturns();
		}

		// Wrap words to the next line once the horizontal length limit is reached
		if (mProperties & Property::MultiLine && mProperties & Property::WordWrap)
		{
			const Vector2f& size = getState(getActiveState()).getSize();

			for (size_t i = 0; i < mLines.size(); )
			{
				mLines[i]->update(Time::Zero);
				const float LENGTH = mLines[i]->getComponent<Collider2DComponent>()->getModelBounds().size.x;
				if (LENGTH >= size.x)
				{
					// Create a new line if this is the last one
					if (i + 1 >= mLines.size()) {
						auto newLine = std::make_unique<Text>(*mLines.front());
						newLine->setText("");
						newLine->getComponent<Transform2DComponent>()->move(0.f, mLines.front()->getCharacterSize() * mLines.front()->getComponent<Transform2DComponent>()->getScale().y * mLines.size());
						mLines.emplace_back(newLine.get());
						getState(getActiveState()).attachChild(std::move(newLine));
					}

					// Attempt to locate the last space character to cut off the last word
					const std::string& currentText = mLines[i]->getText();
					size_t lastSpaceLocation = currentText.rfind(' ');
					if (lastSpaceLocation + 1 == currentText.size()) {
						lastSpaceLocation = currentText.rfind(' ', 1);
					}

					if (lastSpaceLocation != std::string::npos) {
						mLines[i + 1]->setText(currentText.substr(lastSpaceLocation + 1, std::string::npos) + mLines[i + 1]->getText());
						mLines[i]->setText(currentText.substr(0, lastSpaceLocation + 1));
					}
					// Otherwise remove one character from the current line and add it to the next
					else {
						mLines[i + 1]->setText(currentText.substr(currentText.size() - 1, std::string::npos) + mLines[i + 1]->getText());
						mLines[i]->setText(currentText.substr(0, currentText.size() - 1));
					}
				}
				else {
					++i;
				}
			}
		}
	}

	void TextArea::handleImposedReturns()
	{
		size_t characterOffset = 0;
		size_t lineIndex = 0;
		for (size_t i = 0; i < mText.size(); ++i)
		{
			if (mText[i] == '\n')
			{
				// Set the text found prior to the newline
				const std::string LINE_TEXT = mText.substr(characterOffset, i - characterOffset);
				mLines[lineIndex]->setText(LINE_TEXT);

				// Update the line index and the character offset
				characterOffset = i + 1;
				++lineIndex;

				// Create a new line (if necessary)
				if (lineIndex >= mLines.size()) {
					auto newLine = std::make_unique<Text>(*mLines.front());
					mLines.emplace_back(newLine.get());
					getState(getActiveState()).attachChild(std::move(newLine));
				}

				// Set the text found after the newline to the next line
				mLines[lineIndex]->setText(mText.substr(characterOffset, std::string::npos));
				mLines[lineIndex]->getComponent<Transform2DComponent>()->move(0.f, mLines.front()->getCharacterSize() * mLines.front()->getComponent<Transform2DComponent>()->getScale().y * (mLines.size() - 1));
			}
		}
	}

	// Private virtual method(s)
	void TextArea::enableState(uint32_t state)
	{
		// Detach the lines from the currently-active state
		ae::RectangleShape& previousState = getState(getActiveState());
		std::vector<std::unique_ptr<Actor>> detachedLines;
		detachedLines.reserve(mLines.size());
		for (const Text* const line : mLines) {
			detachedLines.push_back(previousState.detachChild(*line));
		}

		// Activate the new state requested
		Widget::enableState(state);

		// Attach the previously-detached lines to the active state
		ae::RectangleShape& currentState = getState(getActiveState());
		for (std::unique_ptr<Actor>& line : detachedLines) {
			currentState.attachChild(std::move(line));
		}

		// Set the render texture's clear color to the active state's fill color
		mContent.setClearColor(currentState.getFillColor());
	}
}