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

#include <AEON/Graphics/Text.h>

#include <AEON/Graphics/internal/Glyph.h>
#include <AEON/Graphics/internal/Renderer2D.h>
#include <AEON/Graphics/internal/FontManager.h>
#include <AEON/Graphics/Font.h>
#include <AEON/Graphics/GLResourceFactory.h>

namespace ae
{
	// Public constructor(s)
	Text::Text()
		: Actor()
		, mText("")
		, mGlyphs()
		, mColor(Color::White)
		, mFont(nullptr)
		, mCharacterSize(48)
	{
		// Add components
		addComponent<Transform2DComponent>();
		addComponent<Collider2DComponent>();
		addComponent<Render2DComponent>();
	}

	Text::Text(const Text& copy)
		: Actor(copy)
		, mText()
		, mGlyphs()
		, mColor()
		, mFont()
		, mCharacterSize(copy.mCharacterSize)
	{
		// Add components
		addComponent<Transform2DComponent>();
		addComponent<Collider2DComponent>();
		addComponent<Render2DComponent>();

		if (mFont) {
			setFont(*copy.mFont);
		}
		setText(copy.mText);
		setColor(copy.mColor);
	}

	Text::~Text()
	{
		if (mFont) {
			mFont->removeText(*this);
		}
	}

	// Public method(s)
	void Text::setFont(Font& font)
	{
		if (mFont == &font) {
			return;
		}
		else if (mFont) {
			mFont->removeText(*this);
		}

		mFont = &font;
		updatePos();
		updateUV();
		mFont->addText(*this);
	}

	void Text::setText(const std::string& text)
	{
		if (mText == text) {
			return;
		}

		mText = text;
		updatePos();
		updateUV();
	}

	void Text::setCharacterSize(uint32_t characterSize)
	{
		if (mCharacterSize == characterSize) {
			return;
		}

		mCharacterSize = characterSize;
		updatePos();
		updateUV();
	}

	void Text::setColor(const Color& color)
	{
		if (mColor == color) {
			return;
		}

		mColor = color;
		updateColor();
	}

	Vector2f Text::findCharPos(size_t index) const
	{
		assert(!mGlyphs.empty());
		assert(index < mGlyphs.size());

		// Calculate the character's horizontal advance
		float offsetX = 0.f;
		for (size_t i = 0; i < index; ++i) {
			offsetX += static_cast<float>(mGlyphs[i]->advance >> 6);
		}

		// Calculate the character's top-left position and return it
		return Vector2f(offsetX + static_cast<float>(mGlyphs[index]->bearing.x),
		                -static_cast<float>(mGlyphs[index]->bearing.y));
	}

	void Text::updateUV()
	{
		if (mGlyphs.empty()) {
			return;
		}

		const Vector2f TEXTURE_SIZE = mGlyphs.front()->texture->getSize();

		// Update the vertices' uv coordinates
		std::vector<Vertex2D>& vertices = getComponent<Render2DComponent>()->getVertices();
		if (vertices.size() != mGlyphs.size() * 4) {
			return;
		}
		for (size_t i = 0; i < mGlyphs.size(); ++i) {
			const Vector2f RECT_POS = mGlyphs[i]->textureRect.position;
			const Vector2f RECT_SIZE = mGlyphs[i]->textureRect.size;

			vertices[i * 4 + 0].uv = Vector2f(RECT_POS.x,               RECT_POS.y)               / TEXTURE_SIZE;
			vertices[i * 4 + 1].uv = Vector2f(RECT_POS.x,               RECT_POS.y + RECT_SIZE.y) / TEXTURE_SIZE;
			vertices[i * 4 + 2].uv = Vector2f(RECT_POS.x + RECT_SIZE.x, RECT_POS.y + RECT_SIZE.y) / TEXTURE_SIZE;
			vertices[i * 4 + 3].uv = Vector2f(RECT_POS.x + RECT_SIZE.x, RECT_POS.y)               / TEXTURE_SIZE;
		}
	}

	// Private method(s)
	void Text::updatePos()
	{
		if (!mFont) {
			return;
		}

		Render2DComponent* const renderComponent = getComponent<Render2DComponent>();

		if (mText.empty()) {
			mGlyphs.clear();
			renderComponent->getVertices().clear();
			renderComponent->getIndices().clear();
			return;
		}

		// Retrieve the glyphs required to display the text
		mGlyphs.clear();
		mGlyphs.reserve(mText.size());
		for (const char& character : mText) {
			const uint32_t CODEPOINT = static_cast<uint32_t>(character);
			const Glyph& glyph = mFont->getGlyph(CODEPOINT, mCharacterSize);
			mGlyphs.push_back(&glyph);
		}

		// Raise the flag indicating that the colors will need to be updated if the vertex count is different
		std::vector<Vertex2D>& vertices = renderComponent->getVertices();
		if (vertices.size() != mGlyphs.size() * 4) {
			vertices.resize(mGlyphs.size() * 4);
			updateColor();
		}

		// Update the vertices
		const float POS_Z = getComponent<Transform2DComponent>()->getPosition().z;
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
		getComponent<Collider2DComponent>()->setModelBounds(Box2f(minPos, maxPos - minPos));

		// Update the indices (if necessary)
		updateIndices();
	}

	void Text::updateIndices()
	{
		const size_t GLYPH_COUNT = mGlyphs.size();
		std::vector<uint32_t>& indices = getComponent<Render2DComponent>()->getIndices();
		if (indices.size() == GLYPH_COUNT * 6) {
			return;
		}

		// Update indices based on the current number of glyphs
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

	void Text::updateColor()
	{
		const Vector4f COLOR = mColor.normalize();

		std::vector<Vertex2D>& vertices = getComponent<Render2DComponent>()->getVertices();
		for (Vertex2D& vertex : vertices) {
			vertex.color = COLOR;
		}
	}

	// Private virtual method(s)
	void Text::renderSelf(RenderStates states) const
	{
		if (mGlyphs.empty()) {
			return;
		}

		// Setup the appropriate render states
		if (!states.shader) {
			states.shader = GLResourceFactory::getInstance().get<Shader>("_AEON_Text2D").get();
		}
		states.blendMode = BlendMode::BlendAlpha;
		states.texture = mGlyphs.front()->texture;
		
		// Submit the glyphs
		Renderer2D::getActiveInstance()->submit(*getComponent<Render2DComponent>(), states);
	}
}