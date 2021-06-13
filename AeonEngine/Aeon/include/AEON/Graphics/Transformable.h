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

#ifndef Aeon_Graphics_Transformable_H_
#define Aeon_Graphics_Transformable_H_

#include <AEON/Math/internal/Quaternion.h>
#include <AEON/Math/Matrix.h>
#include <AEON/Math/AABoxCollider.h>

namespace ae
{
	/*!
	 \brief Abstract base class representing entities to which transformations can be applied.
	 \note No direct instances of this class may be created.
	*/
	class AEON_API Transformable
	{
	public:
		// Public enum(s)
		/*!
		 \brief Origin flags provide an automatic way to set the local origin of an ae::Transformable.
		 \details All of the origin flags except the Center origin flag can be paired together.
		*/
		enum OriginFlag {
			Center  = 0,
			CenterX = 1 << 0,
			CenterY = 1 << 1,
			CenterZ = 1 << 2,
			Left    = 1 << 3,
			Right   = 1 << 4,
			Top     = 1 << 5,
			Bottom  = 1 << 6,
			Front   = 1 << 7,
			Back    = 1 << 8
		};

	public:
		// Public constructor(s)
		/*!
		 \brief Virtual destructor.
		 \details A virtual destructor is needed as this class will be inherited.

		 \since v0.4.0
		*/
		virtual ~Transformable();
	public:
		// Public method(s)
		/*!
		 \brief Sets the ae::Transformable's position.
		 \details This method replaces the previous position.

		 \param[in] position An ae::Vector3f containing the new position

		 \sa move(), getPosition()

		 \since v0.4.0
		*/
		void setPosition(const Vector3f& position) noexcept;
		/*!
		 \brief Sets the ae::Transformable's rotation by providing an \a angle in radians and the \a axes of rotation.
		 \details This method replaces the previous rotation.

		 \param[in] angle The angle of rotation in radians
		 \param[in] axes A unit ae::Vector3f (normalized) containing the axes of rotation

		 \sa rotate(), getRotation()

		 \since v0.4.0
		*/
		void setRotation(float angle, const Vector3f& axes) noexcept;
		/*!
		 \brief Sets the ae::Transformable's \a scale factors.
		 \details This method replaces the previous scale factors.

		 \param[in] scale An ae::Vector3f containing the scale factors

		 \sa scale(), getScale()

		 \since v0.4.0
		*/
		void setScale(const Vector3f& scale) noexcept;
		/*!
		 \brief Sets the ae::Transformable's local origin position using origin flags.
		 \details Origin flags provide an automatic way to set the local origin of an ae::Transformable.
		 \note All of the origin flags except the Center origin flag can be paired together.

		 \param[in] flags The ae::Transformable::OriginFlag to set using the OR bit operator

		 \par Example:
		 \code
		 setOriginFlags(ae::Transformable::OriginFlag::CenterX | ae::Transformable::OriginFlag::Top);
		 \endcode

		 \sa getOrigin()

		 \since v0.4.0
		*/
		void setOrigin(uint32_t flags) noexcept;

		/*!
		 \brief Moves the ae::Transformable based on the \a offset provided.
		 \details This method adds the \a offset to the ae::Transformable's current position.

		 \param[in] offset An ae::Vector3f containing the positional offset

		 \sa setPosition(), getPosition()

		 \since v0.4.0
		*/
		void move(const Vector3f& offset) noexcept;
		/*!
		 \brief Rotates the ae::Transformable based on the \a angle in radians and the \a axes of rotation provided.
		 \details This method adds the rotation provided to the ae::Transformable's current rotation.

		 \param[in] angle The angle of rotation in radians
		 \param[in] axes A unit ae::Vector3f (normalized) containing the axes of rotation

		 \sa setRotation(), getRotation()

		 \since v0.4.0
		*/
		void rotate(float angle, const Vector3f& axes) noexcept;
		/*!
		 \brief Scales the ae::Transformable based on the \a scale factors provided.
		 \details This method multiplies the \a scale factors provided with the ae::Transformable's current scale factors.

		 \param[in] scale An ae::Vector3f containing the scale factors

		 \sa setScale(), getScale()

		 \since v0.4.0
		*/
		void scale(const Vector3f& scale) noexcept;
		/*!
		 \brief Rotates the ae::Transformable so that it looks towards the \a focus point provided.
		 \details This method replaces the previous rotation.

		 \param[in] focus An ae::Vector3f containing the point of focus, the target

		 \since v0.4.0
		*/
		void lookat(const Vector3f& focus);

		/*!
		 \brief Retrieves the ae::Transformable's model transform.
		 \details The model transform will be updated (if necessary) before being retrieved.

		 \return The ae::Matrix4f containing the ae::Transformable's final model transform

		 \sa getInverseTransform()

		 \since v0.4.0
		*/
		_NODISCARD const Matrix4f& getTransform();
		/*!
		 \brief Retrieves the ae::Transformable's inverse model transform.
		 \details The inverse model transform will be updated (if necessary) before being retrieved.

		 \return The ae::Matrix4f containing the ae::Transformable's final inverse model transform

		 \sa getTransform()

		 \since v0.4.0
		*/
		_NODISCARD const Matrix4f& getInverseTransform();
		/*!
		 \brief Retrieves the ae::Transformable's position.

		 \return The ae::Vector3f containing the ae::Transformable's position

		 \sa setPosition()

		 \since v0.4.0
		*/
		_NODISCARD const Vector3f& getPosition() const noexcept;
		/*!
		 \brief Retrieves the ae::Transformable's rotation.

		 \return The ae::Quaternion containing the ae::Transformable's rotation

		 \sa setRotation()

		 \since v0.4.0
		*/
		_NODISCARD const Quaternion& getRotation() const noexcept;
		/*!
		 \brief Retrieves the ae::Transformable's scale factors.

		 \return The ae::Vector3f containing the ae::Transformable's scale factors

		 \sa setScale()

		 \since v0.4.0
		*/
		_NODISCARD const Vector3f& getScale() const noexcept;
		/*!
		 \brief Retrieves the ae::Transformable's origin position.

		 \return The ae::Vector3f containing the ae::Transformable's origin position

		 \sa setOrigin()

		 \since v0.4.0
		*/
		_NODISCARD const Vector3f& getOrigin() const noexcept;
		/*!
		 \brief Retrieves the ae::Transformable's world bounding box.
		 \details The world bounding box is the model bounding box multiplied by the ae::Transformable's model transform.

		 \return An ae::Box3f containing the world bounding box coordinates.

		 \since v0.4.0
		*/
		_NODISCARD Box3f getWorldBounds();

		// Public virtual method(s)
		/*!
		 \brief Sets the ae::Transformable's local \a origin position.
		 \details An origin of (0, 0, 0) places the center at the middle part of the entity.

		 \param[in] origin An ae::Vector3f containing the local origin, (0.f, 0.f, 0.f) by default

		 \sa getOrigin()

		 \since v0.4.0
		*/
		virtual void setOrigin(const Vector3f& origin) noexcept;
		/*!
		 \brief Retrieves the ae::Transformable's model bounding box.

		 \return An ae::Box3f containing the model bounding box coordinates.

		 \since v0.4.0
		*/
		_NODISCARD virtual Box3f getModelBounds() const = 0;
	protected:
		// Protected constructor(s)
		/*!
		 \brief Default constructor.
		 \details No direct instance of this class may be created.

		 \since v0.4.0
		*/
		Transformable() noexcept;
		/*!
		 \brief Copy constructor.

		 \param[in] copy The ae::Transformable that'll be copied

		 \since v0.4.0
		*/
		Transformable(const Transformable& copy) noexcept = default;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::Transformable that'll be moved

		 \since v0.4.0
		*/
		Transformable(Transformable&& rvalue) noexcept = default;
	protected:
		// Protected operator(s)
		/*!
		 \brief Assignment operator.

		 \param[in] other The ae::Transformable that'll be copied

		 \return The caller ae::Transformable

		 \since v0.4.0
		*/
		Transformable& operator=(const Transformable& other) noexcept = default;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae:Transformable that'll be moved

		 \return The caller ae::Transformable

		 \since v0.4.0
		*/
		Transformable& operator=(Transformable&& rvalue) noexcept = default;

	private:
		// Private member(s)
		Matrix4f      mTransform;          //!< The model transform
		Matrix4f      mInvTransform;       //!< The inverse model transform
		Quaternion    mRotation;           //!< The entity's rotation
		Vector3f      mPosition;           //!< The entity's position in world-space
		Vector3f      mScale;              //!< The entity's scale factors
		Vector3f      mOrigin;             //!< The entity's local origin
		bool          mUpdateTransform;    //!< Whether the model transform needs to be updated
		bool          mUpdateInvTransform; //!< Whether the inverse model transform needs to be updated
	};
}
#endif // Aeon_Graphics_Transformable_H_

/*!
 \class ae::Transformable
 \ingroup graphics

 The ae::Transformable abstract base class is used to represent entities on
 which transformations can be applied, meaning that they can be repositioned,
 rotated and scaled. These entities can either be 2-dimensional or 3-dimensional.

 \author Filippos Gleglakos
 \version v0.4.0
 \date 2020.02.19
 \copyright MIT License
*/