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

#include <AEON/Graphics/Collider2DComponent.h>

#include <AEON/Graphics/Actor.h>

namespace ae
{
	// Public constructor(s)
	Collider2DComponent::Collider2DComponent(Actor& associatedActor) noexcept
		: Component(associatedActor)
		, mModelBounds()
	{
	}

	// Public method(s)
	Box2f Collider2DComponent::getWorldBounds()
	{
		const Matrix4f transform = mAssociatedActor.getTransform();
		return Box2f(Vector2f(transform * Vector4f(Vector3f(mModelBounds.min), 1.f)), Vector2f(transform * Vector4f(Vector3f(mModelBounds.max), 1.f)));
	}

	Box2f Collider2DComponent::getGlobalBounds()
	{
		const Matrix4f TRANSFORM = mAssociatedActor.getGlobalTransform();
		return Box2f(Vector2f(TRANSFORM * Vector4f(Vector3f(mModelBounds.min), 1.f)), Vector2f(TRANSFORM * Vector4f(Vector3f(mModelBounds.max), 1.f)));
	}

	void Collider2DComponent::setModelBounds(const Box2f& bounds) noexcept
	{
		mModelBounds = bounds;
		ae::Transform2DComponent* transform = mAssociatedActor.getComponent<ae::Transform2DComponent>();
		if (transform) {
			transform->setDirty(true);
		}
	}
}