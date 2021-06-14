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

#ifndef Aeon_Graphics_Transform2DComponent_H_
#define Aeon_Graphics_Transform2DComponent_H_

#include <climits>

#include <AEON/Math/Matrix.h>
#include <AEON/Graphics/Component.h>

namespace ae
{
	/*!
	 \brief Class representing 2D entitites' component to which transformations can be applied.
	*/
	class AEON_API Transform2DComponent : public Component
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.

		 \since v0.7.0
		*/
		Transform2DComponent() noexcept;
		/*!
		 \brief Copy constructor.

		 \since v0.7.0
		*/
		Transform2DComponent(const Transform2DComponent&) = default;
		/*!
		 \brief Move constructor.

		 \since v0.7.0
		*/
		Transform2DComponent(Transform2DComponent&&) noexcept = default;
	public:
		// Public operator(s)
		/*!
		 \brief Assignment operator.

		 \return The caller ae::Transform2DComponent

		 \since v0.7.0
		*/
		Transform2DComponent& operator=(const Transform2DComponent&) = default;
		/*!
		 \brief Move assignment operator.

		 \return The caller ae::Transform2DComponent

		 \since v0.7.0
		*/
		Transform2DComponent& operator=(Transform2DComponent&&) noexcept = default;
	public:
		// Public method(s)
		/*!
		 \brief Sets the position in world-space.
		 \details It's also possible to set the position along the Z axis to manually handle z-ordering.
		 \note A z-index of INT_MAX will not replace the current position along the Z axis.

		 \param[in] position A 2-dimensional ae::Vector containing the new position
		 \param[in] zIndex The z position to manually handle z-ordering, INT_MAX by default which doesn't modify it

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ae::Transform2DComponent* spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 spriteTransform->setPosition(ae::Vector2f(25.f, 50.f));
		 \endcode

		 \sa move(), getPosition()

		 \since v0.7.0
		*/
		void setPosition(const Vector2f& position, int zIndex = INT_MAX) noexcept;
		/*!
		 \brief Sets the position in world-space.
		 \details It's also possible to set the position along the Z axis to manually handle z-ordering.
		 \note A z-index of INT_MAX will not replace that current position along the Z axis.

		 \param[in] posX The horizontal position
		 \param[in] posY The vertical position
		 \param[in] zIndex The z position to manually handle z-ordering, INT_MAX by default which doesn't modify it

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ae::Transform2DComponent* spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 spriteTransform->setPosition(25.f, 50.f);
		 \endcode

		 \sa move(), getPosition()

		 \since v0.7.0
		*/
		void setPosition(float posX, float posY, int zIndex = INT_MAX) noexcept;
		/*!
		 \brief Sets the rotation by providing an angle in radians.

		 \param[in] angle The angle of rotation in radians
		 
		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ae::Transform2DComponent* spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 spriteTransform->setRotation(ae::Math::toRadians(90.f));
		 \endcode

		 \sa rotate(), getRotation()

		 \since v0.7.0
		*/
		void setRotation(float angle) noexcept;
		/*!
		 \brief Sets the scale factors.
		 \details The default scale factors are (1, 1).

		 \param[in] scale A 2-dimensional ae::Vector containing the new scale factors

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ae::Transform2DComponent* spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 spriteTransform->setScale(ae::Vector2f(0.5f, 0.5f)); // halves the sprite's size
		 ...
		 spriteTransform->setScale(ae::Vector2f(2.f, 2.f));   // doubles the sprite's size
		 \endcode

		 \sa scale(), getScale()

		 \since v0.7.0
		*/
		void setScale(const Vector2f& scale) noexcept;
		/*!
		 \brief Sets the horizontal scale factor and an optional vertical scale factor.
		 \details If the vertical scale factor is 0.f, the horizontal scale factor is applied to both axes.\n
		 The default scale factors are (1, 1).

		 \param[in] scaleX The horizontal scale factor
		 \param[in] scaleY The vertical scale factor, 0 by default indicating that the horizontal scale factor will be applied

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ae::Transform2DComponent* spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 spriteTransform->setScale(0.5f, 0.5f); // halves the sprite's size
		 spriteTransform->setScale(0.5f);       // halves the sprite's size (applies 0.5f to both axes)
		 ...
		 spriteTransform->setScale(2.f, 2.f);   // doubles the sprite's size
		 spriteTransform->setScale(2.f);        // doubles the sprite's size (applies 2.f to both axes)
		 \endcode

		 \sa scale(), getScale()

		 \since v0.7.0
		*/
		void setScale(float scaleX, float scaleY = 0.f) noexcept;
		/*!
		 \brief Applies a translation based on the offset provided.
		 \details This method adds the offset provided to the current position.

		 \param[in] offset A 2-dimensional ae::Vector containing the positional offset

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ae::Transform2DComponent* spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 spriteTransform->setPosition(ae::Vector2f(25.f, 50.f)); // Position (25, 50)
		 ...
		 spriteTransform->move(ae::Vector2f(5.f, 10.f));         // Position (30, 60)
		 \endcode

		 \sa setPosition(), getPosition()

		 \since v0.7.0
		*/
		void move(const Vector2f& offset) noexcept;
		/*!
		 \brief Applies a translation based on the offset provided.
		 \details This method adds the offsets provided to the current position.

		 \param[in] offsetX The horizontal positional offset
		 \param[in] offsetY The vertical positional offset

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ae::Transform2DComponent* spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 spriteTransform->setPosition(25.f, 50.f); // Position (25, 50)
		 ...
		 spriteTransform->move(5.f, 10.f);         // Position (30, 60)
		 \endcode

		 \sa setPosition(), getPosition()

		 \since v0.7.0
		*/
		void move(float offsetX, float offsetY) noexcept;
		/*!
		 \brief Applies a rotation based on the angle in radians provided.
		 \details This method adds the rotation provided to the current rotation.

		 \param[in] angle The angle of rotation in radians

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ae::Transform2DComponent* spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 spriteTransform->setRotation(ae::Math::toRadians(90.f)); // Rotation: 90 degrees
		 ...
		 spriteTransform->rotate(ae::Math::toRadians(15.f));      // Rotation: 105 degrees
		 \endcode

		 \sa setRotation(), getRotation()

		 \since v0.7.0
		*/
		void rotate(float angle) noexcept;
		/*!
		 \brief Applies a scale based on the scale factors provided.
		 \details This method multiplies the current scale by the scale factors provided.

		 \param[in] scale A 2-dimensional ae::Vector containing the scale factors

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ae::Transform2DComponent* spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 spriteTransform->setScale(ae::Vector2f(2.f, 2.f)); // Scale factors (2, 2), doubles the original size
		 ...
		 spriteTransform->scale(ae::Vector2f(0.5f, 0.5f));  // Scale factors (1, 1), halves the current size
		 \endcode

		 \sa setScale(), getScale()

		 \since v0.7.0
		*/
		void scale(const Vector2f& scale) noexcept;
		/*!
		 \brief Applies a scale based on the factors provided.
		 \details If the vertical scale factor is 0.f, the horizontal scale factor is applied to both axes.\n
		 This method multiplies the current scale by the scale factors provided.

		 \param[in] scaleX The horizontal scale factor
		 \param[in] scaleY The vertical scale factor, 0 by default indicating that the horizontal scale factor will be applied

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ae::Transform2DComponent* spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 spriteTransform->setScale(2.f, 2.f); // Scale factors (2, 2), doubles the original size
		 spriteTransform->setScale(2.f);      // Scale factors (2, 2), doubles the original size (applies 2.f to both axes)
		 ...
		 spriteTransform->scale(0.5f, 0.5f);  // Scale factors (1, 1), halves the current size
		 spriteTransform->scale(0.5f);        // Scale factors (1, 1), halves the current size (applies 0.5f to both axes)
		 \endcode

		 \sa setScale(), getScale()

		 \since v0.7.0
		*/
		void scale(float scaleX, float scaleY = 0.f) noexcept;
		/*!
		 \brief Rotates the ae::Transform2DComponent so that it faces the focus point provided.
		 \details If a texture is used and based on its orientation, the API user may have to rotate by a multiple of 90 degrees.

		 \param[in] focus A 2-dimensional ae::Vector containing the point of focus, the target's position

		 \par Example:
		 \code
		 auto spriteA = std::make_unique<ae::Sprite>(someTexture);
		 ae::Transform2DComponent* spriteTransformA = spriteA->getComponent<ae::Transform2DComponent>();
		 spriteTransformA->setPosition(50.f, 25.f);

		 auto spriteB = std::make_unique<ae::Sprite>(someTexture);
		 ae::Transform2DComponent* spriteTransformB = spriteB->getComponent<ae::Transform2DComponent>();
		 spriteTransformB->setPosition(150.f, 120.f);

		 spriteTransformA->lookat(spriteTransformB->getPosition());
		 \endcode

		 \since v0.7.0
		*/
		void lookat(const Vector2f& focus);
		/*!
		 \brief Sets the local origin (or anchor point).
		 \details An origin of (0, 0) doesn't offset the position, the point of rotation or the scale application point.

		 \param[in] origin A 2-dimensional ae::Vector containing the local origin, (0, 0) by default

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ae::Transform2DComponent* spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 spriteTransform->setOrigin(ae::Vector2f(25.f, 25.f));
		 \endcode

		 \sa getOrigin()

		 \since v0.7.0
		*/
		void setOrigin(const Vector2f& origin) noexcept;
		/*!
		 \brief Retrieves the model transform.
		 \details The model transform is used to convert from model/local coordinates to world coordinates.
		 \note The model transform may be updated (if necessary) before being retrieved.

		 \return The ae::Matrix4f containing the model transform

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ae::Transform2DComponent* spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 spriteTransform->setPosition(50.f, 25.f);                        // transform will be computed when requested

		 const ae::Matrix4f& transform = spriteTransform->getTransform(); // transform computed
		 \endcode

		 \sa getInverseTransform()

		 \since v0.7.0
		*/
		const Matrix4f& getTransform();
		/*!
		 \brief Retrieves the inverse model transform.
		 \details The inverse model transform is used to convert from world coordinates back to model/local coordinates.
		 \note The inverse model transform may be updated (if necessary) before being retrieved.

		 \return The ae::Matrix4f containing the inverse model transform

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ae::Transform2DComponent* spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 spriteTransform->setPosition(50.f, 25.f);                                  // inverse transform will be computed when requested

		 const ae::Matrix4f& invTransform = spriteTransform->getInverseTransform(); // inverse transform computed
		 \endcode

		 \sa getTransform()

		 \since v0.7.0
		*/
		const Matrix4f& getInverseTransform();
		/*!
		 \brief Retrieves the position in world-space.

		 \return The 3-dimensional ae::Vector containing the position in world-space

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ae::Transform2DComponent* spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 ...
		 const ae::Vector3f& position = spriteTransform->getPosition();
		 \endcode

		 \sa setPosition()

		 \since v0.7.0
		*/
		_NODISCARD inline const Vector3f& getPosition() const noexcept { return mPosition; }
		/*!
		 \brief Retrieves the angle of rotation in radians.

		 \return The angle of rotation in radians
		 
		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ae::Transform2DComponent* spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 ...
		 float angle = spriteTransform->getRotation();
		 \endcode

		 \sa setRotation()

		 \since v0.7.0
		*/
		_NODISCARD inline float getRotation() const noexcept { return mRotation; }
		/*!
		 \brief Retrieves the scale factors.

		 \return The 2-dimensional ae::Vector containing the scale factors

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ae::Transform2DComponent* spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 ...
		 const ae::Vector2f& scale = spriteTransform->getScale();
		 \endcode

		 \sa setScale()

		 \since v0.7.0
		*/
		_NODISCARD inline const Vector2f& getScale() const noexcept { return mScale; }
		/*!
		 \brief Retrieves the local origin (or the anchor point).

		 \return The 2-dimensional ae::Vector containing the local origin

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ae::Transform2DComponent* spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 ...
		 const ae::Vector2f& origin = spriteTransform->getOrigin();
		 \endcode

		 \sa setOrigin()

		 \since v0.7.0
		*/
		_NODISCARD inline const Vector2f& getOrigin() const noexcept { return mOrigin; }

	private:
		// Private member(s)
		Matrix4f mTransform;         //!< The model transform
		Matrix4f mInvTransform;      //!< The inverse model transform
		Vector3f mPosition;          //!< The position in world-space
		Vector2f mScale;             //!< The scale factors
		Vector2f mOrigin;            //!< The local origin or the anchor point
		float    mRotation;          //!< The rotation in radians along the Z axis
		bool     mTransformDirty;    //!< Whether the model transform needs to be updated
		bool     mInvTransformDirty; //!< Whether the inverse model transform needs to be updated
	};
}
#endif // Aeon_Graphics_Transform2DComponent_H_

/*!
 \class ae::Transform2DComponent
 \ingroup graphics

 The ae::Transform2DComponent class is used to be attached to 2D entities on
 which transformations can be applied, meaning that they can be repositioned,
 rotated and scaled.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2021.06.15
 \copyright MIT License
*/