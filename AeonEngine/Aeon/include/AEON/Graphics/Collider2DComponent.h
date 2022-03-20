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

#ifndef Aeon_Graphics_Collider2DComponent_H_
#define Aeon_Graphics_Collider2DComponent_H_

#include <functional>

#include <AEON/Math/AABoxCollider.h>
#include <AEON/Graphics/Component.h>

namespace ae
{
	/*!
	 \brief Class representing 2D entities' component that describe their minimum and maximum coordinates.
	*/
	class AEON_API Collider2DComponent : public Component
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the component by providing the associated actor.

		 \param[in] associatedActor The ae::Actor to which the component is attached

		 \since v0.7.0
		*/
		explicit Collider2DComponent(Actor& associatedActor) noexcept;
		/*!
		 \brief Copy constructor.

		 \since v0.7.0
		*/
		Collider2DComponent(const Collider2DComponent&) = default;
		/*!
		 \brief Move constructor.

		 \since v0.7.0
		*/
		Collider2DComponent(Collider2DComponent&&) noexcept = default;
	public:
		// Public operator(s)
		/*!
		 \brief Assignment operator.

		 \return The caller ae::Collider2DComponent

		 \since v0.7.0
		*/
		Collider2DComponent& operator=(const Collider2DComponent&) = default;
		/*!
		 \brief Move assignment operator.

		 \return The caller ae::Collider2DComponent

		 \since v0.7.0
		*/
		Collider2DComponent& operator=(Collider2DComponent&&) noexcept = default;
	public:
		// Public method(s)
		/*!
		 \brief Retrieves the world bounding box.
		 \details The world bounding box is the model bounding box transformed by the model transform.

		 \return The world bounding box

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ...
		 auto spriteCollider = sprite->getComponent<ae::Collider2DComponent>();
		 Box2f worldBounds = spriteCollider->getWorldBounds();
		 \endcode

		 \sa getGlobalBounds(), getModelBounds()

		 \since v0.7.0
		*/
		Box2f getWorldBounds();
		/*!
		 \brief Retrieves the global bounding box.
		 \details The global bounding box is the final bounding box resulting from multiplying every transform of the associated actor until the root node is reached.

		 \return The global bounding box

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ...
		 auto spriteCollider = sprite->getComponent<ae::Collider2DComponent>();
		 Box2f globalBounds = spriteCollider->getGlobalBounds();
		 \endcode

		 \sa getWorldBounds(), getModelBounds()

		 \since v0.7.0
		*/
		Box2f getGlobalBounds();
		/*!
		 \brief Sets the model bounding box.

		 \param[in] bounds An ae::Box2f containing the model bounds

		 \sa getModelBounds()

		 \since v0.7.0
		*/
		void setModelBounds(const Box2f& bounds) noexcept;
		/*!
		 \brief Retrieves the model bounding box.

		 \return An ae::Box2f containing the model bounds

		 \sa setModelBounds(),getWorldBounds(), getGlobalBounds()

		 \since v0.7.0
		*/
		inline const Box2f& getModelBounds() const noexcept { return mModelBounds; }

	private:
		// Private member(s)
		Box2f mModelBounds; //!< The model bounds
	};
}
#endif // Aeon_Graphics_Collider2DComponent_H_ 

/*!
 \class ae::Collider2DComponent
 \ingroup graphics

 The ae::Collider2DComponent class is used to be attached to 2D entities on
 than can collide with other 2D entities or if their shape can be represented
 by an AABB (Axis-Aligned Bounding Box) to show their minimum and maximum
 coordinates.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2021.06.20
 \copyright MIT License
*/