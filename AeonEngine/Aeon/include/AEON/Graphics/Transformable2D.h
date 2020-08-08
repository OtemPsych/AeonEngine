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

#ifndef Aeon_Graphics_Transformable2D_H_
#define Aeon_Graphics_Transformable2D_H_

#include <AEON/Config.h>
#include <AEON/Math/Matrix.h>
#include <AEON/Math/AABoxCollider.h>

namespace ae
{
	/*!
	 \brief Abstract base class representing 2D game objects to which transformations can be applied.
	 \note No direct instance of this class may be created.
	*/
	class AEON_API Transformable2D
	{
	public:
		// Public enum(s)
		/*!
		 \brief Origin flags provide an automatic way to set the local origin of an ae::Transformable2D.
		 \details All of the origin flags except the Center origin flag can be paired together.
		 \note Pairing together the Left and Right or the Top and Bottom flags will most likely lead to undesired results.
		*/
		enum OriginFlag {
			Center  = 0,
			CenterX = 1 << 0,
			CenterY = 1 << 1,
			Left    = 1 << 2,
			Right   = 1 << 3,
			Top     = 1 << 4,
			Bottom  = 1 << 5
		};

	public:
		// Public constructor(s)
		/*!
		 \brief Virtual destructor.
		 \details A virtual destructor is needed as this class will be inherited.

		 \since v0.4.0
		*/
		virtual ~Transformable2D();
	public:
		// Public method(s)
		/*!
		 \brief Sets the ae::Transformable2D's position in world-space.
		 \details It's also possible to set the ae::Transformable2D's z
		 position to manually handle z-ordering, but this is also managed
		 automatically by the ae::Actor2D scene graph architecture.
		 \note This method replaces the previous position.
		 A z-index of 0 will not replace the current position along the Z axis.

		 \param[in] position A 2-dimensional ae::Vector containing the new position
		 \param[in] zIndex The z position to manually handle z-ordering, 0 by default

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 sprite->setPosition(ae::Vector2f(25.f, 50.f));
		 \endcode

		 \sa move(), getPosition()

		 \since v0.5.0
		*/
		void setPosition(const Vector2f& position, int zIndex = 0) noexcept;
		/*!
		 \brief Sets the ae::Transformable2D's position in world-space.
		 \details It's also possible to set the ae::Transformable2D's z
		 position to manually handle z-ordering, but this is also managed
		 automatically by the ae::Actor2D scene graph architecture.
		 \note This method replaces the previous position.
		 A z-index of 0 will not replace the current position along the Z axis.

		 \param[in] posX The new horizontal position
		 \param[in] posY The new vertical position
		 \param[in] zIndex The z position to manually handle z-ordering, 0 by default

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 sprite->setPosition(25.f, 50.f);
		 \endcode

		 \sa move(), getPosition()

		 \since v0.4.0
		*/
		void setPosition(float posX, float posY, int zIndex = 0) noexcept;
		/*!
		 \brief Sets the ae::Transformable2D's rotation by providing an angle in degrees.
		 \note This method replaces the previous rotation.

		 \param[in] angle The angle of rotation in degrees

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 sprite->setRotation(90.f);
		 \endcode

		 \sa rotate(), getRotation()

		 \since v0.4.0
		*/
		void setRotation(float angle) noexcept;
		/*!
		 \brief Sets the ae::Transformable2D's scale factors.
		 \details The default scale factors are (1, 1).
		 \note This method replaces the previous scale factors.

		 \param[in] scale A 2-dimensional ae::Vector containing the new scale factors

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 sprite->setScale(ae::Vector2f(0.5f, 0.5f)); // halves the sprite's size
		 sprite->setScale(ae::Vector2f(2.f, 2.f));   // doubles the sprite's size
		 \endcode

		 \sa scale(), getScale()

		 \since v0.4.0
		*/
		void setScale(const Vector2f& scale) noexcept;
		/*!
		 \brief Sets the ae::Transformable2D's scale factors.
		 \details The default scale factors are (1, 1).
		 \note This method replaces the previous scale factors.

		 \param[in] scaleX The new horizontal scale factor
		 \param[in] scaleY The new vertical scale factor

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 sprite->setScale(0.5f, 0.5f); // halves the sprite's size
		 sprite->setScale(2.f, 2.f);   // doubles the sprite's size
		 \endcode

		 \sa scale(), getScale()

		 \since v0.4.0
		*/
		void setScale(float scaleX, float scaleY) noexcept;
		/*!
		 \brief Sets the ae::Transformable2D's local origin (or anchor point) using origin flags.
		 \details Origin flags provide an automatic way to set the local origin of an ae::Transformable2D.
		 \note All of the origin flags except the Center origin flag can be paired together.
		 Pairing together the Left and Right or the Top and Bottom flags will most likely lead to undesired results.

		 \param[in] flags The ae::Transformable2D::OriginFlag to set using the OR bit operator

		 \par Example:
		 \code
		 // The default origin is at (0, 0) which is equivalent to (OriginFlag::Left | OriginFlag::Top)
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);

		 // Sets the origin to the sprite's horizontal center and the top
		 sprite->setOriginFlags(ae::Sprite::OriginFlag::CenterX | ae::Sprite::OriginFlag::Top);
		 \endcode

		 \sa setOrigin(), getOrigin()

		 \since v0.4.0
		*/
		void setOriginFlags(uint32_t flags);
		/*!
		 \brief Displaces the ae::Transformable2D based on the offset provided.
		 \note This method adds the offset provided to the ae::Transformable2D's current position.

		 \param[in] offset A 2-dimensional ae::Vector containing the positional offset

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 sprite->setPosition(ae::Vector2f(25.f, 50.f)); // Position (25, 50)
		 ...
		 sprite->move(ae::Vector2f(5.f, 10.f));         // Position (30, 60)
		 \endcode

		 \sa setPosition(), getPosition()

		 \since v0.4.0
		*/
		void move(const Vector2f& offset) noexcept;
		/*!
		 \brief Displaces the ae::Transformable2D based on the offset provided.
		 \note This method adds the offset provided to the ae::Transformable2D's current position.

		 \param[in] offsetX The horizontal positional offset
		 \param[in] offsetY The vertical positional offset

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 sprite->setPosition(25.f, 50.f); // Position (25, 50)
		 ...
		 sprite->move(5.f, 10.f);         // Position (30, 60)
		 \endcode

		 \sa setPosition(), getPosition()

		 \since v0.4.0
		*/
		void move(float offsetX, float offsetY) noexcept;
		/*!
		 \brief Rotates the ae::Transformable2D based on the angle in degrees provided.
		 \note This method adds the rotation provided to the ae::Transformable2D's current rotation.

		 \param[in] angle The angle of rotation in degrees

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 sprite->setRotation(90.f); // Rotation: 90 degrees
		 ...
		 sprite->rotate(15.f);      // Rotation: 105 degrees
		 \endcode

		 \sa setRotation(), getRotation()

		 \since v0.5.0
		*/
		void rotate(float angle) noexcept;
		/*!
		 \brief Scales the ae::Transformable2D based on the scale factors provided.
		 \note This method multiplies the scale factors provided by the ae::Transformable2D's current scale factors.

		 \param[in] scale A 2-dimensional ae::Vector containing the scale factors

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 sprite->setScale(ae::Vector2f(2.f, 2.f)); // Scale factors (2, 2), doubles the original size
		 ...
		 sprite->scale(ae::Vector2f(0.5f, 0.5f));  // Scale factors (1, 1), halves the current size
		 \endcode

		 \sa setScale(), getScale()

		 \since v0.4.0
		*/
		void scale(const Vector2f& scale) noexcept;
		/*!
		 \brief Scales the ae::Transformable2D based on the scale factors provided.
		 \note This method multiplies the scale factors provided by the ae::Transformable2D's current scale factors.

		 \param[in] scaleX The horizontal scale factor
		 \param[in] scaleY The vertical scale factor

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 sprite->setScale(2.f, 2.f); // Scale factors (2, 2), doubles the original size
		 ...
		 sprite->scale(0.5f, 0.5f);  // Scale factors (1, 1), halves the current size
		 \endcode

		 \sa setScale(), getScale()

		 \since v0.4.0
		*/
		void scale(float scaleX, float scaleY) noexcept;
		/*!
		 \brief Rotates the ae::Transformable2D so that it faces the focus point provided.
		 \details Based on the sprite texture's orientation, the API user may have to rotate by a multiple of 90 degrees.
		 \note This method replaces the previous rotation.

		 \param[in] focus A 2-dimensional ae::Vector containing the point of focus, the target's position

		 \par Example:
		 \code
		 auto sprite1 = std::make_unique<ae::Sprite>(someTexture);
		 sprite1->setPosition(50.f, 25.f);

		 auto sprite2 = std::make_unique<ae::Sprite>(someTexture);
		 sprite2->setPosition(150.f, 120.f);

		 sprite1->lookat(sprite2->getPosition());
		 \endcode

		 \since v0.4.0
		*/
		void lookat(const Vector2f& focus);
		/*!
		 \brief Sets the ae::Transformable2D's local origin (or anchor point).
		 \details An origin of (0, 0) places the origin at the top left part of the entity.
		 \note Origin flags provide a more accessible way to set the local origin rather than directly setting it manually.

		 \param[in] origin A 2-dimensional ae::Vector containing the local origin, (0, 0) by default

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 sprite->setOrigin(getModelBounds().max / 2.f);
		 \endcode

		 \sa setOriginFlags(), getOrigin()

		 \since v0.5.0
		*/
		void setOrigin(const Vector2f& origin) noexcept;
		/*!
		 \brief Retrieves the ae::Transformable2D's model transform.
		 \details The model transform is used to convert the ae::Transformable2D from local coordinates to world coordinates.
		 \note The model transform may be updated (if necessary) before being retrieved.

		 \return The ae::Matrix4f containing the ae::Transformable2D's model transform

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 sprite->setPosition(50.f, 25.f); // transform will be recomputed when requested

		 const ae::Matrix4f& transform = sprite->getTransform(); // transform recomputed
		 \endcode

		 \sa getInverseTransform()

		 \since v0.5.0
		*/
		const Matrix4f& getTransform();
		/*!
		 \brief Retrieves the ae::Transformable2D's inverse model transform.
		 \details The inverse model transform is used to convert the ae::Transformable2D from world coordinates back to local coordinates.
		 \note The inverse model transform may be updated (if necessary) before being retrieved.

		 \return The ae::Matrix4f containing the ae::Transformable2D's inverse model transform

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 sprite->setPosition(50.f, 25.f); // inverse transform will be recomputed when requested

		 const ae::Matrix4f& invTransform = sprite->getInverseTransform(); // inverse transform recomputed
		 \endcode

		 \sa getTransform()

		 \since v0.5.0
		*/
		const Matrix4f& getInverseTransform();
		/*!
		 \brief Retrieves the ae::Transformable2D's position in world-space.

		 \return The 3-dimensional ae::Vector containing the ae::Transformable2D's position in world-space

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ...
		 const ae::Vector3f& position = sprite->getPosition();
		 \endcode

		 \sa setPosition()

		 \since v0.4.0
		*/
		_NODISCARD const Vector3f& getPosition() const noexcept;
		/*!
		 \brief Retrieves the ae::Transformable2D's angle of rotation in degrees.

		 \return The angle of rotation in degrees

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ...
		 float angle = sprite->getRotation();
		 \endcode

		 \sa setRotation()

		 \since v0.5.0
		*/
		_NODISCARD float getRotation() const noexcept;
		/*!
		 \brief Retrieves the ae::Transformable2D's scale factors.

		 \return The 2-dimensional ae::Vector containing the ae::Transformable2D's scale factors

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ...
		 const ae::Vector2f& scale = sprite->getScale();
		 \endcode

		 \sa setScale()

		 \since v0.4.0
		*/
		_NODISCARD const Vector2f& getScale() const noexcept;
		/*!
		 \brief Retrieves the ae::Transformable2D's local origin (or the anchor point).

		 \return The 2-dimensional ae::Vector containing the ae::Transformable2D's local origin

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ...
		 const ae::Vector2f& origin = sprite->getOrigin();
		 \endcode

		 \sa setOrigin()

		 \since v0.4.0
		*/
		_NODISCARD const Vector2f& getOrigin() const noexcept;
		/*!
		 \brief Retrieves the ae::Transformable2D's world bounding box.
		 \details The world bounding box is the model bounding box transformed by the ae::Transformable2D's model transform.

		 \return An ae::Box2f containing the world bounding box

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ...
		 const ae::Box2f& worldBounds = sprite->getWorldBounds();
		 \endcode

		 \sa getModelBounds()

		 \since v0.4.0
		*/
		_NODISCARD Box2f getWorldBounds();

		// Public virtual method(s)
		/*!
		 \brief Retrieves the ae::Transformable2D's model bounding box.
		 \note Derived classes define the model bounding box.

		 \return An ae::Box2f containing the model bounding box

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ...
		 ae::Box2f modelBounds = sprite->getModelBounds();
		 \endcode

		 \sa getWorldBounds()

		 \since v0.4.0
		*/
		_NODISCARD virtual Box2f getModelBounds() const = 0;
	protected:
		// Protected constructor(s)
		/*!
		 \brief Default constructor.
		 \note No direct instance of this class may be created.

		 \since v0.4.0
		*/
		Transformable2D() noexcept;
		/*!
		 \brief Deleted copy constructor.

		 \since v0.5.0
		*/
		Transformable2D(const Transformable2D&) = delete;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::Transformable2D that will be moved

		 \since v0.5.0
		*/
		Transformable2D(Transformable2D&& rvalue) noexcept;
	protected:
		// Protected operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.5.0
		*/
		Transformable2D& operator=(const Transformable2D&) = delete;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::Transformable2D that will be moved

		 \return The caller ae::Transformable2D

		 \since v0.5.0
		*/
		Transformable2D& operator=(Transformable2D&& rvalue) noexcept;
	protected:
		// Protected virtual method(s)
		/*!
		 \brief Recalculates the correct origin position based on the origin flags.

		 \sa setOriginFlags()

		 \since v0.5.0
		*/
		virtual void correctProperties();

	private:
		// Private member(s)
		Matrix4f mTransform;          //!< The model transform
		Matrix4f mInvTransform;       //!< The inverse model transform
		Vector3f mPosition;           //!< The position in world-space
		Vector2f mScale;              //!< The scale factors
		Vector2f mOrigin;             //!< The local origin or the anchor point
		float    mRotation;           //!< The rotation in degrees along the Z axis
		uint32_t mOriginFlags;        //!< The origin flags indicating the origin point
		bool     mUpdateTransform;    //!< Whether the model transform needs to be updated
		bool     mUpdateInvTransform; //!< Whether the inverse model transform needs to be updated
	};
}
#endif // Aeon_Graphics_Transformable2D_H_

/*!
 \class ae::Transformable2D
 \ingroup graphics

 The ae::Transformable2D abstract base class is used to represent 2D entities
 on which transformations can be applied, meaning that they can be repositioned,
 rotated and scaled.

 \author Filippos Gleglakos
 \version v0.5.0
 \date 2020.08.07
 \copyright MIT License
*/