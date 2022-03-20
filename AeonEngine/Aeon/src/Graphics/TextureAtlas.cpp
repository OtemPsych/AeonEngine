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

#include <AEON/Graphics/TextureAtlas.h>

#include <GL/glew.h>

#include <rectpack2D/finders_interface.h>

#include <AEON/Graphics/internal/GLCommon.h>
#include <AEON/Graphics/GLResourceFactory.h>
#include <AEON/Graphics/Sprite.h>
#include <AEON/Graphics/BasicRenderer2D.h>

namespace ae
{
	// Public constructor(s)
	TextureAtlas::TextureAtlas(Texture2D::InternalFormat format)
		: mAtlas()
		, mRects()
		, mUnpackedRects()
		, mPreviousAtlas()
		, mHelperSprite()
	{
		mAtlas.setColorProperties(Texture::Filter::Linear, Texture::Wrap::ClampToEdge, format);
		mPreviousAtlas.setColorProperties(Texture::Filter::Linear, Texture::Wrap::ClampToEdge, format);
	}

	TextureAtlas::TextureAtlas(TextureAtlas&& rvalue) noexcept
		: mAtlas(std::move(rvalue.mAtlas))
		, mRects(std::move(rvalue.mRects))
		, mUnpackedRects(std::move(rvalue.mUnpackedRects))
		, mPreviousAtlas(std::move(rvalue.mPreviousAtlas))
		, mHelperSprite(std::move(rvalue.mHelperSprite))
	{
	}

	// Public operator(s)
	TextureAtlas& TextureAtlas::operator=(TextureAtlas&& rvalue) noexcept
	{
		// Move the rvalue's data
		mAtlas = std::move(rvalue.mAtlas);
		mRects = std::move(rvalue.mRects);
		mUnpackedRects = std::move(rvalue.mUnpackedRects);
		mPreviousAtlas = std::move(rvalue.mPreviousAtlas);
		mHelperSprite = std::move(rvalue.mHelperSprite);

		return *this;
	}

	// Public method(s)
	void TextureAtlas::add(uint32_t textureID, const Texture2D& texture)
	{
		// Add the texture and its size to the hashmap
		const Vector2u& textureSize = texture.getSize();
		const bool SUCCESS = mUnpackedRects.try_emplace(textureID, std::make_pair(&texture, Box2i(0, 0, textureSize.x, textureSize.y))).second;

		// Check if the texture was successfully added
		if (!SUCCESS) {
			AEON_LOG_WARNING("Texture emplacement failed v2", "The texture provided may have already been previously added.");
		}
	}

	void TextureAtlas::pack()
	{
		BasicRenderer2D& renderer = BasicRenderer2D::getInstance();

		// Check if there is at least one texture added
		if (mUnpackedRects.empty()) {
			AEON_LOG_WARNING("No textures added v2", "No textures have yet been added to the texture atlas.\nAborting packing.");
			return;
		}

		// Keep a copy of the previous rects
		auto prevRects = mRects;

		// Add the unpacked rects to the current rects
		for (auto& unpackedItr : mUnpackedRects) {
			mRects.insert(std::make_pair(unpackedItr.first, unpackedItr.second.second));
		}

		// Calculate the optimal positions for each texture within the texture atlas and create it
		const Vector2i ATLAS_SIZE = computePacking();
		mAtlas.create(ATLAS_SIZE.x, ATLAS_SIZE.y);

		// Copy over the old texture atlas
		renderer.beginScene(mAtlas);
		if (!prevRects.empty()) {
			mHelperSprite.setTexture(*mPreviousAtlas.getTexture());
			for (auto rectItr : prevRects) {
				mHelperSprite.setTextureRect(Box2f(rectItr.second));
				mHelperSprite.getComponent<Transform2DComponent>()->setPosition(mRects.find(rectItr.first)->second.min);
				mHelperSprite.update(Time::Zero);
				mHelperSprite.render(BlendMode::BlendNone);
			}
		}

		// Copy each unpacked texture's image data
		for (auto& texture : mUnpackedRects) {
			mHelperSprite.setTexture(*texture.second.first, true);
			mHelperSprite.getComponent<Transform2DComponent>()->setPosition(mRects.find(texture.first)->second.min);
			mHelperSprite.update(Time::Zero);
			mHelperSprite.render(BlendMode::BlendNone);
		}
		renderer.endScene();

		// Update atlas textures rects
		mUnpackedRects.clear();

		// Overwrite previous atlas with new one
		mPreviousAtlas.create(ATLAS_SIZE.x, ATLAS_SIZE.y);
		renderer.beginScene(mPreviousAtlas);
		mHelperSprite.setTexture(*mAtlas.getTexture(), true);
		mHelperSprite.getComponent<Transform2DComponent>()->setPosition(0.f, 0.f);
		mHelperSprite.update(Time::Zero);
		mHelperSprite.render(BlendMode::BlendNone);
		renderer.endScene();
	}

	const Texture2D& TextureAtlas::getTexture() const noexcept
	{
		return *mAtlas.getTexture();
	}

	Box2i TextureAtlas::getTextureRect(uint32_t textureId) const noexcept
	{
		// Find the texture requested
		auto itr = mRects.find(textureId);
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (itr == mRects.end()) {
				AEON_LOG_WARNING("Invalid texture identifier", "The identifier provided hasn't been added to the texture atlas.\nReturning empty rect.");
				return Box2i();
			}
		}

		// Return the corresponding texture rectangle
		return itr->second;
	}

	// Private method(s)
	Vector2i TextureAtlas::computePacking()
	{
		// Setup rectpack2D parameters
		using rp2d_spacesType = rectpack2D::empty_spaces<false, rectpack2D::default_empty_spaces>;
		using rp2d_rectType = rectpack2D::output_rect_t<rp2d_spacesType>;
		int rp2d_maxSize;
		GLCall(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &rp2d_maxSize));

		// Create the rectpack2D rectangles
		std::vector<rp2d_rectType> rp2d_rects;
		rp2d_rects.reserve(mRects.size());
		for (const auto& texture : mRects) {
			rp2d_rects.emplace_back(0, 0, texture.second.size.x + 1, texture.second.size.y + 1);
		}

		// Calculate the optimal positions for the rectpack2D rectangles
		const auto rp2d_textureAtlas = rectpack2D::find_best_packing<rp2d_spacesType>(
			rp2d_rects,
			rectpack2D::make_finder_input(
				rp2d_maxSize,
				1,
				[](rp2d_rectType&) { return rectpack2D::callback_result::CONTINUE_PACKING; },
				[](rp2d_rectType&) { return rectpack2D::callback_result::ABORT_PACKING; },
				rectpack2D::flipping_option::DISABLED
			)
			);

		// Assign the optimal positions to the stored texture rects
		auto rp2dItr = rp2d_rects.begin();
		for (auto itr = mRects.begin(); itr != mRects.end(); ++itr, ++rp2dItr) {
			itr->second.position.x = rp2dItr->x;
			itr->second.position.y = rp2dItr->y;
		}

		// Return the dimensions of the texture atlas required to store all textures
		return Vector2i(rp2d_textureAtlas.w, rp2d_textureAtlas.h);
	}
}