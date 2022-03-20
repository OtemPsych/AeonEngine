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

#pragma once

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
		// Public enum(s)
		/*!
		 \brief Origin types specify to what transform the origin is used.
		 \details All of the origin types except the All origin type can be paired together using the OR bit operator.
		*/
		enum OriginType {
			Translation = 1 << 0,
			Rotation    = 1 << 1,
			Scale       = 1 << 2,
			All         = Translation | Rotation | Scale
		};
		/*!
		 \brief Origin flags provide an automatic way to set the local origin.
		 \details All of the origin flags except the Custom and Center origin flags can be paired together using the OR bit operator.
		 \note Pairing together the Left and Right or the Top and Bottom flags will most likely lead to undesired results.\n
		 The Custom origin flag is reserved for manually setting the origin.
		*/
		enum OriginFlag {
			Custom  = 0,
			CenterX = 1 << 0,
			CenterY = 1 << 1,
			Left    = 1 << 2,
			Right   = 1 << 3,
			Top     = 1 << 4,
			Bottom  = 1 << 5,
			Center  = CenterX | CenterY
		};

		// Public constructor(s)
		/*!
		 \brief Constructs the component by providing the associated actor.
		 \details The translation origin is set to the top left, the rotation and scale origins to the center.

		 \param[in] associatedActor The ae::Actor to which the component is attached

		 \since v0.7.0
		*/
		explicit Transform2DComponent(Actor& associatedActor) noexcept;
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

		// Public method(s)
		/*!
		 \brief Sets the position in world-space.
		 \details It's also possible to set the position along the Z axis to manually handle z-ordering.
		 \note A z-index of INT_MAX will not replace the current position along the Z axis.

		 \param[in] position A 2-dimensional ae::Vector containing the new position
		 \param[in] zIndex The z position to manually handle z-ordering, INT_MAX by default which doesn't modify it
		 \param[in] zIndexFixed Whether the zIndex provided should remain the same regardless of child node ordering, false by default

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 auto spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 spriteTransform->setPosition(ae::Vector2f(25.f, 50.f));
		 \endcode

		 \sa move(), getPosition()

		 \since v0.7.0
		*/
		void setPosition(const Vector2f& position, int zIndex = INT_MAX, bool zIndexFixed = false) noexcept;
		/*!
		 \brief Sets the position in world-space.
		 \details It's also possible to set the position along the Z axis to manually handle z-ordering.
		 \note A z-index of INT_MAX will not replace that current position along the Z axis.

		 \param[in] posX The horizontal position
		 \param[in] posY The vertical position
		 \param[in] zIndex The z position to manually handle z-ordering, INT_MAX by default which doesn't modify it
		 \param[in] zIndexFixed Whether the zIndex provided should remain the same regardless of child node ordering, false by default

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 auto spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 spriteTransform->setPosition(25.f, 50.f);
		 \endcode

		 \sa move(), getPosition()

		 \since v0.7.0
		*/
		void setPosition(float posX, float posY, int zIndex = INT_MAX, bool zIndexFixed = false) noexcept;
		/*!
		 \brief Sets the rotation by providing an angle in radians.

		 \param[in] angle The angle of rotation in radians
		 
		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 auto spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
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
		 auto spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
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
		 auto spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
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
		 \brief Sets the local origin (or anchor point) for the origin type.
		 \details This method manually sets the origin, it's highly recommended to set the origin using origin flags which automatically updates the origin when needed.

		 \param[in] typeFlags The ae::Transform2DComponent::OriginType specifying what transform is affected by the origin, separated by the OR bit operator
		 \param[in] origin A 2-dimensional ae::Vector containing the local origin, (0, 0) by default

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 auto spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 spriteTransform->setOrigin(ae::Transform2DComponent::OriginType::Translation, ae::Vector2f(25.f, 25.f));
		 \endcode

		 \sa setOriginFlags(), getOrigin()

		 \since v0.7.0
		*/
		void setOrigin(uint32_t typeFlags, const Vector2f& origin) noexcept;
		/*!
		 \brief Sets the local origin (or anchor point) for the origin type using origin flags.
		 \details Origin flags provide an automatic way of setting the local origin.
		 \note This method only has an effect when the associated actor containing this ae::Tranform2DComponent also has an ae::Collider2DComponent.

		 \param[in] typeFlags The ae::Transform2DComponent::OriginType specifying what transform is affected by the origin
		 \param[in] originFlags The ae::Transform2DComponent::OriginFlag to set using the OR bit operator

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);

		 // All rotations applied to the sprite will be applied on its bottom left corner, it will therefore rotate around that corner
		 auto spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 spriteTransform->setOriginFlags(ae::Transform2DComponent::OriginType::Rotation,
		                                 ae::Transform2DComponent::OriginFlag::Left | ae::Transform2DComponent::OriginFlag::Bottom);
		 \endcode

		 \sa setOrigin()

		 \since v0.7.0
		*/
		void setOriginFlags(uint32_t typeFlags, uint32_t originFlags);
		/*!
		 \brief Modifies the position so that it relatively aligns to the associated actor's closest ancestor with an ae::Collider2DComponent based on the flags provided.
		 \note This method only has an effect when the associated actor has an ancestor with an ae::Collider2DComponent.

		 \param[in] flags The ae::Transform2DComponent::OriginFlag to set using the OR bit operator

		 \par Example:
		 \code
		 // Create the parent and child sprites
		 auto parentSprite = std::make_unique<ae::Sprite>(someTexture);
		 auto childSprite = std::make_unique<ae::Sprite>(otherTexture);

		 ae::Sprite* childSpritePtr = childSprite.get();
		 parentSprite->attachChild(std::move(childSprite));

		 // Set the origin of the child sprite to its center and align it to the center of the parent
		 childSpritePtr->setOriginFlags(ae::Sprite::OriginFlag::Center);
		 childSpritePtr->setRelativeAlignment(ae::Sprite::OriginFlag::Center);
		 \endcode

		 \since v0.7.0
		*/
		void setRelativeAlignment(uint32_t flags);
		/*!
		 \brief Applies a translation based on the offset provided.
		 \details This method adds the offset provided to the current position.

		 \param[in] offset A 2-dimensional ae::Vector containing the positional offset

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 auto spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
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
		 auto spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
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
		 auto spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 spriteTransform->setRotation(ae::Math::radians(90.f)); // Rotation: 90 degrees
		 ...
		 spriteTransform->rotate(ae::Math::radians(15.f));      // Rotation: 15 degrees
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
		 auto spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
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
		 auto spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
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
		 auto spriteTransformA = spriteA->getComponent<ae::Transform2DComponent>();
		 spriteTransformA->setPosition(50.f, 25.f);

		 auto spriteB = std::make_unique<ae::Sprite>(someTexture);
		 auto spriteTransformB = spriteB->getComponent<ae::Transform2DComponent>();
		 spriteTransformB->setPosition(150.f, 120.f);

		 spriteTransformA->lookat(spriteTransformB->getPosition());
		 \endcode

		 \since v0.7.0
		*/
		void lookat(const Vector2f& focus);
		/*!
		 \brief Updates the origin flags and the relative alignment should any changes have taken place.

		 \since v0.7.0
		*/
		void updateProperties();
		/*!
		 \brief Retrieves the model transform.
		 \details The model transform is used to convert from model/local coordinates to world coordinates.
		 \note The model transform may be updated (if necessary) before being retrieved.

		 \return The ae::Matrix4f containing the model transform

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 auto spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
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
		 auto spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 spriteTransform->setPosition(50.f, 25.f);                                  // inverse transform will be computed when requested

		 const ae::Matrix4f& invTransform = spriteTransform->getInverseTransform(); // inverse transform computed
		 \endcode

		 \sa getTransform()

		 \since v0.7.0
		*/
		const Matrix4f& getInverseTransform();
		/*!
		 \brief Retrieves the local origin (or the anchor point) of the transform type provided.
		 \note If the ae::Transform2DComponent::OriginType::All is provided, the translation origin will be retrieved.

		 \param[in] originType The ae::Transform2DComponent::OriginType to retrieve

		 \return The 2-dimensional ae::Vector containing the local origin of the transform type

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 auto spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 ...
		 const ae::Vector2f& scaleOrigin = spriteTransform->getOrigin(ae::Transform2DComponent::OriginType::Scale);
		 \endcode

		 \sa setOrigin(), setOriginFlags()

		 \since v0.7.0
		*/
		[[nodiscard]] const Vector2f& getOrigin(uint32_t originType) const noexcept;
		/*!
		 \brief Retrieves the position in world-space.

		 \return The 3-dimensional ae::Vector containing the position in world-space

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 auto spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 ...
		 const ae::Vector3f& position = spriteTransform->getPosition();
		 \endcode

		 \sa setPosition()

		 \since v0.7.0
		*/
		[[nodiscard]] inline const Vector3f& getPosition() const noexcept { return mPosition; }
		/*!
		 \brief Retrieves the angle of rotation in radians.

		 \return The angle of rotation in radians
		 
		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 auto spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 ...
		 float angle = spriteTransform->getRotation();
		 \endcode

		 \sa setRotation()

		 \since v0.7.0
		*/
		[[nodiscard]] inline float getRotation() const noexcept { return mRotation; }
		/*!
		 \brief Retrieves the scale factors.

		 \return The 2-dimensional ae::Vector containing the scale factors

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 auto spriteTransform = sprite->getComponent<ae::Transform2DComponent>();
		 ...
		 const ae::Vector2f& scale = spriteTransform->getScale();
		 \endcode

		 \sa setScale()

		 \since v0.7.0
		*/
		[[nodiscard]] inline const Vector2f& getScale() const noexcept { return mScale; }

	private:
		// Private struct(s)
		/*!
		 \brief Struct representing an alignment relative to the closest ancestor.
		*/
		struct Alignment {
			Vector2f position;  //!< The relative offset
			uint32_t flags;     //!< The flags indicating the alignment
			bool     isAligned; //!< Whether alignment is enabled for this component
		};
		/*!
		 \brief Struct representing an origin with its position and the flags allowing for dynamic repositioning.
		*/
		struct Origin {
			Vector2f position; //!< The actor's anchor point
			uint32_t flags;    //!< The flags indicating where the origin is placed on the actor
		};

		// Private method(s)
		/*!
		 \brief Updates the translation, rotation and scale origins.
		 
		 \param[in] pos The new origin position
		 \param[in] typeFlags The flags indicating to what origin type (translation, rotation and/or scale) the origin is applied
		 \param[in] originFlags The flags indicating where the origin is placed on the actor which are then used to dynamically update the origin position
		 
		 \since v0.7.0
		*/
		void updateOrigins(const Vector2f& pos, uint32_t typeFlags, uint32_t originFlags) noexcept;

		// Private member(s)
		Matrix4f  mTransform;         //!< The model transform
		Matrix4f  mInvTransform;      //!< The inverse model transform
		Alignment mAlignment;         //!< The alignment flags and relative offset
		Origin    mTranslationOrigin; //!< The local translation origin
		Origin    mRotationOrigin;    //!< The local rotation origin
		Origin    mScaleOrigin;       //!< The local scale origin
		Vector3f  mPosition;          //!< The position in world-space
		Vector2f  mScale;             //!< The scale factors
		float     mRotation;          //!< The rotation in radians along the Z axis
		bool      mTransformDirty;    //!< Whether the model transform needs to be updated
		bool      mInvTransformDirty; //!< Whether the inverse model transform needs to be updated
		bool      mHasFixedZIndex;    //!< Whether a fixed z-index has been set
	};
}

/*!
 \class ae::Transform2DComponent
 \ingroup graphics

 The ae::Transform2DComponent class is used to be attached to 2D entities on
 which transformations can be applied, meaning that they can be repositioned,
 rotated and scaled.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2021.12.26
 \copyright MIT License
*/