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

#include <AEON/Graphics/Text.h>

#include <AEON/Graphics/Font.h>
#include <AEON/Graphics/internal/Glyph.h>
#include <AEON/Graphics/GLResourceFactory.h>
#include <AEON/Graphics/Renderer2D.h>
#include <AEON/Graphics/Sprite.h>

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
		, mUpdatePosUVs(true)
		, mUpdateColors(true)
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
		, mUpdatePosUVs(rvalue.mUpdatePosUVs)
		, mUpdateColors(rvalue.mUpdateColors)
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
		mUpdatePosUVs = rvalue.mUpdatePosUVs;
		mUpdateColors = rvalue.mUpdateColors;

		return *this;
	}

	// Public method(s)
	void Text::setFont(Font& font) noexcept
	{
		mFont = &font;
		mUpdatePosUVs = true;
	}

	void Text::setText(const std::string& text) noexcept
	{
		mText = text;
		mUpdatePosUVs = true;
	}

	void Text::setCharacterSize(unsigned int characterSize) noexcept
	{
		mCharacterSize = characterSize;
		mUpdatePosUVs = true;
	}

	void Text::setColor(const Color& color) noexcept
	{
		mColor = color;
		mUpdateColors = true;
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
	void Text::updatePosUVs()
	{
		// Make sure that a font and a string of text have been set (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (!mFont) {
				AEON_LOG_ERROR("No font found", "A font hasn't yet been set.");
				return;
			}
			if (mText.empty()) {
				AEON_LOG_ERROR("No text found", "A string of text hasn't yet been set.");
				return;
			}
		}

		// Retrieve the glyphs required to display the text
		mGlyphs.clear();
		mGlyphs.reserve(mText.size());
		for (const char& character : mText) {
			const uint32_t CODEPOINT = static_cast<uint32_t>(character);
			const Glyph& glyph = mFont->getGlyph(CODEPOINT, mCharacterSize);
			mGlyphs.emplace_back(&glyph);
		}

		// Additional text data
		const float POS_Z = getPosition().z;
		const Vector2f TEXTURE_SIZE = Vector2f(mGlyphs.front()->texture->getSize());

		// Raise the flag indicating that the colors will need to be updated if the vertex count is different
		std::vector<Vertex2D>& vertices = getVertices();
		if (vertices.size() != mGlyphs.size() * 4) {
			vertices.resize(mGlyphs.size() * 4);
			mUpdateColors = true;
		}

		// Update the vertices
		float offsetX = 0.f;
		for (size_t i = 0; i < mGlyphs.size(); ++i) {
			// Update the positions
			const Vector2f RECT_SIZE = mGlyphs[i]->textureRect.size;
			Vector3f startPos(offsetX + mGlyphs[i]->bearing.x, -mGlyphs[i]->bearing.y, POS_Z);

			vertices[i * 4 + 0].position = Vector3f(Vector2f(startPos.x,               startPos.y)              , POS_Z);
			vertices[i * 4 + 1].position = Vector3f(Vector2f(startPos.x,               startPos.y + RECT_SIZE.y), POS_Z);
			vertices[i * 4 + 2].position = Vector3f(Vector2f(startPos.x + RECT_SIZE.x, startPos.y + RECT_SIZE.y), POS_Z);
			vertices[i * 4 + 3].position = Vector3f(Vector2f(startPos.x + RECT_SIZE.x, startPos.y)              , POS_Z);

			offsetX += (mGlyphs[i]->advance >> 6);

			// Update the uv coordinates
			const Vector2i& rectPos = mGlyphs[i]->textureRect.position;
			vertices[i * 4 + 0].uv = Vector2f(rectPos.x,               rectPos.y)               / TEXTURE_SIZE;
			vertices[i * 4 + 1].uv = Vector2f(rectPos.x,               rectPos.y + RECT_SIZE.y) / TEXTURE_SIZE;
			vertices[i * 4 + 2].uv = Vector2f(rectPos.x + RECT_SIZE.x, rectPos.y + RECT_SIZE.y) / TEXTURE_SIZE;
			vertices[i * 4 + 3].uv = Vector2f(rectPos.x + RECT_SIZE.x, rectPos.y)               / TEXTURE_SIZE;
		}

		// Update the model bounding box's position and size based on the minimum and maximum vertex positions
		Vector2f minPos = vertices[0].position.xy, maxPos = vertices[0].position.xy;
		for (auto vertexItr = vertices.begin() + 1; vertexItr != vertices.end(); ++vertexItr) {
			minPos = min(minPos, vertexItr->position.xy);
			maxPos = max(maxPos, vertexItr->position.xy);
		}
		mModelBounds = Box2f(minPos, maxPos - minPos);

		// Update the indices (if necessary)
		std::vector<unsigned int>& indices = getIndices();
		if (indices.size() != mGlyphs.size() * 6) {
			indices.clear();
			indices.reserve(mGlyphs.size() * 6);
			for (size_t i = 0; i < mGlyphs.size(); ++i) {
				indices.emplace_back(i * 4 + 0);
				indices.emplace_back(i * 4 + 1);
				indices.emplace_back(i * 4 + 2);

				indices.emplace_back(i * 4 + 0);
				indices.emplace_back(i * 4 + 2);
				indices.emplace_back(i * 4 + 3);
			}
		}
	}

	void Text::updateColors()
	{
		const Vector4f COLOR = mColor.normalize();
		std::vector<Vertex2D>& vertices = getVertices();
		for (Vertex2D& vertex : vertices) {
			vertex.color = COLOR;
		}
	}

	bool Text::updateVertices()
	{
		bool dirtyRender = false;

		// Update the vertices' properties
		if (mUpdatePosUVs) {
			updatePosUVs();
			correctProperties();
			dirtyRender = std::exchange(mUpdatePosUVs, false);
		}
		if (mUpdateColors) {
			updateColors();
			dirtyRender = std::exchange(mUpdateColors, false);
		}

		return dirtyRender;
	}

	// Private virtual method(s)
	void Text::handleEventSelf(Event* const event)
	{
		if (event->type == Event::Type::FontUpdated) {
			auto fontEvent = event->as<FontEvent>();
			if (fontEvent->font == mFont) {
				mUpdatePosUVs = true;
			}
		}
	}

	void Text::updateSelf(const Time& dt)
	{
		// Update the text's properties which may raise the dirty render flag
		setDirty(updateVertices());
	}

	void Text::renderSelf(RenderStates states) const
	{
		// Setup the appropriate render states
		if (!states.shader) {
			states.shader = GLResourceFactory::getInstance().get<Shader>("_AEON_Text2D").get();
		}
		states.blendMode = BlendMode::BlendAlpha;
		states.texture = mGlyphs.front()->texture;
		
		// Submit the glyphs
		Renderer2D::getInstance().submit(*this, states, isDirty());
	}
}