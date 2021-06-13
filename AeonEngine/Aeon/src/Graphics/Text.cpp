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

#include <AEON/Graphics/Text.h>

#include <AEON/Graphics/internal/Glyph.h>
#include <AEON/Graphics/internal/Renderer2D.h>
#include <AEON/Graphics/Font.h>
#include <AEON/Graphics/GLResourceFactory.h>

namespace ae
{
	// Public constructor(s)
	Text::Text()
		: Actor2D()
		, mText("")
		, mGlyphs()
		, mModelBounds()
		, mColor(Color::White)
		, mFont(nullptr)
		, mCharacterSize(48)
		, mUpdatePos(false)
		, mUpdateUV(false)
		, mUpdateColor(false)
	{
	}

	Text::Text(Text&& rvalue) noexcept
		: Actor2D(std::move(rvalue))
		, mText(std::move(rvalue.mText))
		, mGlyphs(std::move(rvalue.mGlyphs))
		, mModelBounds(std::move(rvalue.mModelBounds))
		, mColor(std::move(rvalue.mColor))
		, mFont(rvalue.mFont)
		, mCharacterSize(rvalue.mCharacterSize)
		, mUpdatePos(rvalue.mUpdatePos)
		, mUpdateUV(rvalue.mUpdateUV)
		, mUpdateColor(rvalue.mUpdateColor)
	{
	}

	// Public operator(s)
	Text& Text::operator=(Text&& rvalue) noexcept
	{
		// Copy the rvalue's trivial data and move the rest
		Actor2D::operator=(std::move(rvalue));
		mText = std::move(rvalue.mText);
		mGlyphs = std::move(rvalue.mGlyphs);
		mModelBounds = std::move(rvalue.mModelBounds);
		mColor = std::move(rvalue.mColor);
		mFont = rvalue.mFont;
		mCharacterSize = rvalue.mCharacterSize;
		mUpdatePos = rvalue.mUpdatePos;
		mUpdateUV = rvalue.mUpdateUV;
		mUpdateColor = rvalue.mUpdateColor;

		return *this;
	}

	// Public method(s)
	void Text::setFont(Font& font) noexcept
	{
		// Check if the same font is being set (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (mFont == &font) {
				AEON_LOG_WARNING("Unnecessary call", "The font provided is already assigned to the text instance.");
				return;
			}
		}

		mFont = &font;
		mUpdatePos = true;
		mUpdateUV = true;
	}

	void Text::setText(const std::string& text) noexcept
	{
		// Check if the same text is being set (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (mText == text) {
				AEON_LOG_WARNING("Unnecessary call", "The text string provided is already assigned to the text instance.");
				return;
			}
		}

		mText = text;
		mUpdatePos = true;
		mUpdateUV = true;
	}

	void Text::setCharacterSize(unsigned int characterSize) noexcept
	{
		// Check if the same character size is being set (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (mCharacterSize == characterSize) {
				AEON_LOG_WARNING("Unnecessary call", "The character size provided is already assigned to the text instance.");
				return;
			}
		}

		mCharacterSize = characterSize;
		mUpdatePos = true;
		mUpdateUV = true;
	}

	void Text::setColor(const Color& color) noexcept
	{
		// Check if the same color is being set (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (mColor == color) {
				AEON_LOG_WARNING("Unnecessary call", "The color provided is already assigned to the text instance.");
				return;
			}
		}

		mColor = color;
		mUpdateColor = true;
	}

	Vector2f Text::findCharPos(size_t index) const
	{
		// Check if a text has been set and whether the index is valid (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (mGlyphs.empty()) {
				AEON_LOG_WARNING("Unassigned text", "No text string has been assigned.\nAborting operation.");
				return Vector2f();
			}
			if (index >= mGlyphs.size()) {
				AEON_LOG_WARNING("Invalid index", "The character index provided is invalid.\nAborting operation.");
				return Vector2f();
			}
		}

		// Calculate the character's horizontal advance
		float offsetX = 0.f;
		for (size_t i = 0; i < index; ++i) {
			offsetX += static_cast<float>(mGlyphs[i]->advance >> 6);
		}

		// Calculate the character's top-left position and return it
		return Vector2f(offsetX + static_cast<float>(mGlyphs[index]->bearing.x),
		                -static_cast<float>(mGlyphs[index]->bearing.y));
	}

	const Font* const Text::getFont() const noexcept
	{
		return mFont;
	}

	const std::string& Text::getText() const noexcept
	{
		return mText;
	}

	unsigned int Text::getCharacterSize() const noexcept
	{
		return mCharacterSize;
	}

	const Color& Text::getColor() const noexcept
	{
		return mColor;
	}

	// Public virtual method(s)
	Box2f Text::getModelBounds() const
	{
		return mModelBounds;
	}

	// Private method(s)
	void Text::updatePos()
	{
		// Make sure that a font has been set (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (!mFont) {
				AEON_LOG_ERROR("No font found", "A font hasn't yet been set.");
				return;
			}
		}

		if (mText.empty()) {
			mGlyphs.clear();
			getVertices().clear();
			getIndices().clear();
			mUpdateUV = false;
			mUpdateColor = false;
			return;
		}

		// Retrieve the glyphs required to display the text
		mGlyphs.clear();
		mGlyphs.reserve(mText.size());
		for (const char& character : mText) {
			const uint32_t CODEPOINT = static_cast<uint32_t>(character);
			const Glyph& glyph = mFont->getGlyph(CODEPOINT, mCharacterSize);
			mGlyphs.emplace_back(&glyph);
		}

		// Raise the flag indicating that the colors will need to be updated if the vertex count is different
		std::vector<Vertex2D>& vertices = getVertices();
		if (vertices.size() != mGlyphs.size() * 4) {
			vertices.resize(mGlyphs.size() * 4);
			mUpdateColor = true;
		}

		// Update the vertices
		const float POS_Z = getPosition().z;
		float offsetX = 0.f;
		for (size_t i = 0; i < mGlyphs.size(); ++i) {
			// Update the positions
			const Vector2f RECT_SIZE = mGlyphs[i]->textureRect.size;
			Vector2f startPos(offsetX + mGlyphs[i]->bearing.x, -mGlyphs[i]->bearing.y);

			vertices[i * 4 + 0].position = Vector3f(Vector2f(startPos.x,               startPos.y)              , POS_Z);
			vertices[i * 4 + 1].position = Vector3f(Vector2f(startPos.x,               startPos.y + RECT_SIZE.y), POS_Z);
			vertices[i * 4 + 2].position = Vector3f(Vector2f(startPos.x + RECT_SIZE.x, startPos.y + RECT_SIZE.y), POS_Z);
			vertices[i * 4 + 3].position = Vector3f(Vector2f(startPos.x + RECT_SIZE.x, startPos.y)              , POS_Z);

			offsetX += static_cast<float>(mGlyphs[i]->advance >> 6);
		}

		// Update the model bounding box's position and size based on the minimum and maximum vertex positions
		Vector2f minPos = vertices[0].position.xy, maxPos = vertices[0].position.xy;
		for (auto vertexItr = vertices.begin() + 1; vertexItr != vertices.end(); ++vertexItr) {
			minPos = min(minPos, vertexItr->position.xy);
			maxPos = max(maxPos, vertexItr->position.xy);
		}
		mModelBounds = Box2f(minPos, maxPos - minPos);

		// Update the indices (if necessary)
		updateIndices();
	}

	void Text::updateUV()
	{
		if (!mGlyphs.empty()) {
			const Vector2f TEXTURE_SIZE = mGlyphs.front()->texture->getSize();

			// Update the vertices' uv coordinates
			std::vector<Vertex2D>& vertices = getVertices();
			for (size_t i = 0; i < mGlyphs.size(); ++i) {
				const Vector2f RECT_POS = mGlyphs[i]->textureRect.position;
				const Vector2f RECT_SIZE = mGlyphs[i]->textureRect.size;

				vertices[i * 4 + 0].uv = Vector2f(RECT_POS.x,               RECT_POS.y)               / TEXTURE_SIZE;
				vertices[i * 4 + 1].uv = Vector2f(RECT_POS.x,               RECT_POS.y + RECT_SIZE.y) / TEXTURE_SIZE;
				vertices[i * 4 + 2].uv = Vector2f(RECT_POS.x + RECT_SIZE.x, RECT_POS.y + RECT_SIZE.y) / TEXTURE_SIZE;
				vertices[i * 4 + 3].uv = Vector2f(RECT_POS.x + RECT_SIZE.x, RECT_POS.y)               / TEXTURE_SIZE;
			}
		}
	}

	void Text::updateIndices()
	{
		const size_t GLYPH_COUNT = mGlyphs.size();

		// Check if the indices need to be updated based on the current number of glyphs
		std::vector<unsigned int>& indices = getIndices();
		if (indices.size() != GLYPH_COUNT * 6)
		{
			// Recalculate the appropriate indices
			indices.clear();
			indices.reserve(GLYPH_COUNT * 6);
			for (size_t i = 0; i < GLYPH_COUNT; ++i) {
				indices.emplace_back(i * 4 + 0);
				indices.emplace_back(i * 4 + 1);
				indices.emplace_back(i * 4 + 2);

				indices.emplace_back(i * 4 + 0);
				indices.emplace_back(i * 4 + 2);
				indices.emplace_back(i * 4 + 3);
			}
		}
	}

	void Text::updateColor()
	{
		const Vector4f COLOR = mColor.normalize();

		std::vector<Vertex2D>& vertices = getVertices();
		for (Vertex2D& vertex : vertices) {
			vertex.color = COLOR;
		}
	}

	// Private virtual method(s)
	void Text::handleEventSelf(Event* const event)
	{
		if (event->type == Event::Type::FontUpdated) {
			auto fontEvent = event->as<FontEvent>();
			if (fontEvent->font == mFont) {
				mUpdateUV = true;
			}
		}
	}

	void Text::updateSelf(const Time& dt)
	{
		// Update the text's properties which may raise the dirty render flag
		if (mUpdatePos) {
			updatePos();
			correctProperties();
			setDirty(std::exchange(mUpdatePos, false));
		}
		if (mUpdateUV) {
			updateUV();
			setDirty(std::exchange(mUpdateUV, false));
		}
		if (mUpdateColor) {
			updateColor();
			setDirty(std::exchange(mUpdateColor, false));
		}
	}

	void Text::renderSelf(RenderStates states) const
	{
		if (!mGlyphs.empty())
		{
			// Setup the appropriate render states
			if (!states.shader) {
				states.shader = GLResourceFactory::getInstance().get<Shader>("_AEON_Text2D").get();
			}
			states.blendMode = BlendMode::BlendAlpha;
			states.texture = mGlyphs.front()->texture;
			states.dirty = isDirty();
		
			// Submit the glyphs
			Renderer2D::getActiveInstance()->submit(getVertices(), getIndices(), states);

			// Drop the dirty render flag
			setDirty(false);
		}
	}
}