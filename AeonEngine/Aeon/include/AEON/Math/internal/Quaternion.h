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

#ifndef Aeon_Math_Quaternion_H_
#define Aeon_Math_Quaternion_H_

#include <AEON/Config.h>
#include <AEON/Math/Vector2.h>
#include <AEON/Math/Vector3.h>
#include <AEON/Math/Vector4.h>

namespace ae
{
	/*!
	 \brief The class used to represent a rotation whilst avoiding gimbal lock.
	 \details Gimbal lock occurs once two of the three axes of rotation are situated in a parallel configuration.
	 \note This class is considered to be internal but may still be used by the API user.
	*/
	class _NODISCARD AEON_API Quaternion
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details Sets the angle of rotation w to 1.f, and the axes of rotation x, y and z to 0.f.

		 \since v0.3.0
		*/
		Quaternion() noexcept;
		/*!
		 \brief Constructs the ae::Quaternion by providing an angle of rotation \a w and the axes of rotation \a x, \a y and \a z.

		 \param[in] w The angle of rotation that will be assigned to the real number w
		 \param[in] x The x axis of rotation that will be assigned to the imaginary number x
		 \param[in] y The y axis of rotation that will be assigned to the imaginary number y
		 \param[in] z The z axis of rotation that will be assigned to the imaginary number z

		 \par Example:
		 \code
		 ae::Quaternion quat(0.45f, 0.f, 1.f, -1.f);
		 \endcode

		 \since v0.3.0
		*/
		Quaternion(float w, float x, float y, float z) noexcept;
		/*!
		 \brief Constructs the ae::Quaternion by providing an angle of rotation \a w and an ae::Vector containing the \a axes of rotation.

		 \param[in] w The angle of rotation that will be assigned to the real number w
		 \param[in] axes The ae::Vector containing the axes of rotation that will be assigned to the imaginary numbers x, y and z

		 \par Example:
		 \code
		 ae::Quaternion quat(0.45f, ae::Vector3f(0.f, 1.f, -1.f));
		 \endcode

		 \since v0.3.0
		*/
		Quaternion(float w, const Vector3f& axes) noexcept;
		/*!
		 \brief Constructs the ae::Quaternion by providing an ae::Vector containing the 3 axes of rotation and the angle of rotation.
		 \note The w coordinate of the ae::Vector contains the angle of rotation.

		 \param[in] rotation The ae::Vector containing the axes of rotation and the angle of rotation

		 \par Example:
		 \code
		 ae::Quaternion quat(ae::Vector4f(0.f, 1.f, -1.f, 0.45f));
		 \endcode

		 \since v0.3.0
		*/
		explicit Quaternion(const Vector4f& rotation) noexcept;
		/*!
		 \brief Copy constructor.
		 \details Sets the ae::Quaternion's components to the \a copy's components.

		 \param[in] copy The ae::Quaternion that will be copied

		 \par Example:
		 \code
		 ae::Quaternion quat1(0.45f, 0.f, 1.f, -1.f);
		 ae::Quaternion quat2(quat1);
		 // or
		 ae::Quaternion quat2 = quat1;
		 \endcode

		 \since v0.3.0
		*/
		Quaternion(const Quaternion& copy) noexcept;
		/*!
		 \brief Move constructor.
		 \details Performs a move operation on the \a rvalue's components to the lvalue ae::Quaternion's components.

		 \param[in] rvalue The ae::Quaternion rvalue that will be moved

		 \par Example:
		 \code
		 ae::Quaternion quat1(0.45f, 0.f, 1.f, -1.f);
		 ae::Quaternion quat2(0.8f, 1.f, 0.f, -1.f);
		 ae::Quaternion quat3 = quat1 + quat2;
		 \endcode

		 \since v0.3.0
		*/
		Quaternion(Quaternion&& rvalue) noexcept;
	public:
		// Public operator(s)
		/*!
		 \brief Assignment operator.
		 \details Performs a memberwise assignment from the \a other's components to the caller ae::Quaternion's ones.

		 \param[in] other The ae::Quaternion of which its elements will be copied over to the caller's ones

		 \return The caller ae::Quaternion containing the new components, these calls can be chained together one after the other

		 \par Example:
		 \code
		 ae::Quaternion quat1(0.45f, 0.f, 1.f, -1.f);
		 ae::Quaternion quat2(0.8f, 1.f, 0.f, -1.f);
		 ...
		 quat1 = quat2;
		 \endcode

		 \since v0.3.0
		*/
		Quaternion& operator=(const Quaternion& other) noexcept;
		/*!
		 \brief Move assignment operator.
		 \details Performs a memberwise move assignment from the \a rvalue's components to the caller ae::Quaternion's ones.

		 \param[in] rvalue The rvalue ae::Quaternion of which its elements will be moved over to the caller's ones

		 \return The caller ae::Quaternion containing the moved elements, these calls can be chained together one after the other

		 \par Example:
		 \code
		 ae::Quaternion quat1(0.45f, 0.f, 1.f, -1.f);
		 ae::Quaternion quat2(0.8f, 1.f, 0.f, -1.f);
		 ae::Quaternion quat3(0.25f, 0.f, 0.f, 1.f);
		 ...
		 quat3 = quat1 * quat2;
		 \endcode

		 \since v0.3.0
		*/
		Quaternion& operator=(Quaternion&& rvalue) noexcept;

		/*!
		 \brief Addition operator.
		 \details Performs a memberwise addition of the angle and the axes of rotation of the caller and of the \a other.

		 \param[in] other The ae::Quaternion of which its angle and its axes of rotation will be added with those of the caller

		 \return An ae::Quaternion containing the sum of the angle and of the axes of rotation of the two ae::Quaternion objects

		 \par Example:
		 \code
		 ae::Quaternion quat1(0.45f, 0.f, 1.f, -1.f);
		 ae::Quaternion quat2(0.8f, 1.f, 0.f, -1.f);
		 ae::Quaternion quat3 = quat1 + quat2;
		 \endcode

		 \sa operator-(), operator*(const Quaternion&)

		 \since v0.3.0
		*/
		_NODISCARD Quaternion operator+(const Quaternion& other) const noexcept;
		/*!
		 \brief Subtraction operator.
		 \details Performs a memberwise subtraction of the angle and the axes of rotation of the caller and of the \a other.

		 \param[in] other The ae::Quaternion of which its angle and its axes of rotation will subtract those of the caller

		 \return An ae::Quaternion containing the difference of the angle and of the axes of rotation of the two ae::Quaternion objects

		 \par Example:
		 \code
		 ae::Quaternion quat1(0.45f, 0.f, 1.f, -1.f);
		 ae::Quaternion quat2(0.8f, 1.f, 0.f, -1.f);
		 ae::Quaternion quat3 = quat1 - quat2;
		 \endcode

		 \sa operator+(), operator*(const Quaternion&)

		 \since v0.3.0
		*/
		_NODISCARD Quaternion operator-(const Quaternion& other) const noexcept;
		/*!
		 \brief Multiplication operator.
		 \details Performs a non-commutative multiplication of the angle and the axes of rotation of the caller and of the \a other.

		 \param[in] other The ae::Quaternion of which its angle and its axes of rotation will be multiplied with those of the caller

		 \return An ae::Quaternion containing the non-commutative product of the angle and of the axes of rotation of the two ae::Quaternion objects

		 \par Example:
		 \code
		 ae::Quaternion quat1(0.45f, 0.f, 1.f, -1.f);
		 ae::Quaternion quat2(0.8f, 1.f, 0.f, -1.f);
		 ae::Quaternion quat3 = quat1 * quat2;
		 \endcode

		 \sa operator+(), operator-()

		 \since v0.3.0
		*/
		_NODISCARD Quaternion operator*(const Quaternion& other) const noexcept;

		/*!
		 \brief Multiplication operator.
		 \details Performs a memberwise multiplication of the angle and the axes of rotation of the ae::Quaternion and of the \a scalar value.

		 \param[in] scalar The scalar value that will be multiplied with the angle and the axes of rotation of the ae::Quaternion

		 \return An ae::Quaternion containing the product of the angle and of the axes of rotation of the ae::Quaternion and of the \a scalar value

		 \par Example:
		 \code
		 ae::Quaternion quat1(0.45f, 0.f, 1.f, -1.f);
		 ae::Quaternion quat2 = quat1 * 0.8f;
		 \endcode

		 \sa operator/()

		 \since v0.3.0
		*/
		_NODISCARD Quaternion operator*(float scalar) const noexcept;
		/*!
		 \brief Division operator.
		 \details Performs a memberwise division of the angle and the axes of rotation of the ae::Quaternion and of the \a scalar value.
		 \note If the \a scalar value provided is equal to 0, an error message will be logged and the operation will be aborted.

		 \param[in] scalar The scalar value that will act as the divisor of the angle and of the axes of rotation of the ae::Quaternion

		 \return An ae::Quaternion containing the quotient of the angle and the axes of rotation of the ae::Quaternion and of the \a scalar value

		 \par Example:
		 \code
		 ae::Quaternion quat1(0.45f, 0.f, 1.f, -1.f);
		 ae::Quaternion quat2 = quat1 / 0.8f;
		 \endcode

		 \sa operator*(float)

		 \since v0.3.0
		*/
		_NODISCARD Quaternion operator/(float scalar) const;

		/*!
		 \brief Vector transformation operator.
		 \details Rotates the ae::Vector provided by using the ae::Quaternion's rotation.

		 \param[in] vec The 3-dimensional ae::Vector to rotate

		 \return The rotated 3-dimensional ae::Vector

		 \par Example:
		 \code
		 ae::Quaternion quat(0.45f, 1.f, 0.f, -1.f);
		 ae::Vector3f vec3f(0.65f, 0.f, 0.3f);
		 ae::Vector3f rotatedVec3f = quat * vec3f;
		 \endcode

		 \sa rotate()

		 \since v0.3.0
		*/
		_NODISCARD Vector3f operator*(const Vector3f& vec) const noexcept;

		/*!
		 \brief Addition and assignment operator.
		 \details Performs a memberwise addition of the caller's and the \a other's components, and assigns the sum to the caller ae::Quaternion.

		 \param[in] other The ae::Quaternion of which its angle and its axes of rotation will be added with those of the caller

		 \return The caller ae::Quaternion containing the sum of the addition, these calls can be chained together one after the other

		 \par Example:
		 \code
		 ae::Quaternion quat1(0.45f, 0.f, 1.f, -1.f);
		 ae::Quaternion quat2(0.8f, 1.f, 0.f, -1.f);
		 ...
		 quat1 += quat2;
		 \endcode

		 \sa operator-=(), operator*=(const Quaternion&)

		 \since v0.3.0
		*/
		Quaternion& operator+=(const Quaternion& other) noexcept;
		/*!
		 \brief Subtraction and assignment operator.
		 \details Performs a memberwise subtraction of the caller's and the \a other's components, and assigns the difference to the caller ae::Quaternion.

		 \param[in] other The ae::Quaternion of which its angle and its axes of rotation will subtact those of the caller

		 \return The caller ae::Quaternion containing the difference of the subtraction, these calls can be chained together one after the other

		 \par Example:
		 \code
		 ae::Quaternion quat1(0.45f, 0.f, 1.f, -1.f);
		 ae::Quaternion quat2(0.8f, 1.f, 0.f, -1.f);
		 ...
		 quat1 -= quat2;
		 \endcode

		 \sa operator+=(), operator*=(const Quaternion&)

		 \since v0.3.0
		*/
		Quaternion& operator-=(const Quaternion& other) noexcept;
		/*!
		 \brief Multiplication and assignment operator.
		 \details Performs a non-commutative multiplication of the caller's and the \a other's components, and assigns the product to the caller ae::Quaternion.

		 \param[in] other The ae::Quaternion of which its angle and its axes of rotation will be multiplied with those of the caller

		 \return The caller ae::Quaternion containing the product of the non-commutative multiplication, these calls can be chained together one after the other

		 \par Example:
		 \code
		 ae::Quaternion quat1(0.45f, 0.f, 1.f, -1.f);
		 ae::Quaternion quat2(0.8f, 1.f, 0.f, -1.f);
		 ...
		 quat1 *= quat2;
		 \endcode

		 \sa operator+=(), operator-=()

		 \since v0.3.0
		*/
		Quaternion& operator*=(const Quaternion& other) noexcept;

		/*!
		 \brief Multiplication and assignment operator.
		 \details Performs a memberwise multiplication of the ae::Quaternion's angle and the axes of rotation and of the \a scalar value, and assigns the product to the ae::Quaternion.

		 \param[in] scalar The scalar value that will be multiplied with the ae::Quaternion's angle and axes of rotation

		 \return The caller ae::Quaternion containing the product of the multiplication, these calls can be chained together one after the other

		 \par Example:
		 \code
		 ae::Quaternion quat(0.45f, 0.f, 1.f, -1.f);
		 ...
		 quat *= 0.8f;
		 \endcode

		 \sa operator*=(float)

		 \since v0.3.0
		*/
		Quaternion& operator*=(float scalar) noexcept;
		/*!
		 \brief Division and assignment operator.
		 \details Performs a memberwise division of the ae::Quaternion's angle and the axes of rotation and of the \a scalar value, and assigns the quotient to the ae::Quaternion.
		 \note If the \a scalar value provided is equal to 0, an error message will be logged and the operation will be aborted.

		 \param[in] scalar The scalar value that will be the divisor of the ae::Quaternion's angle and axes of rotation

		 \return The caller ae::Quaternion containing the quotient of the division, these calls can be chained together one after the other

		 \par Example:
		 \code
		 ae::Quaternion quat(0.45f, 0.f, 1.f, -1.f);
		 ...
		 quat /= 0.8f;
		 \endcode

		 \sa operator*=(float)

		 \since v0.3.0
		*/
		Quaternion& operator/=(float scalar);

		/*!
		 \brief Equality operator.
		 \details Checks if the respective components of the caller and of the \a other are equal.

		 \param[in] other The ae::Quaternion that will be compared with the caller ae::Quaternion

		 \return True if all of the caller and the \a other's components are respectively equal, false otherwise

		 \par Example:
		 \code
		 ae::Quaternion quat1(0.45f, 0.f, 1.f, -1.f);
		 ae::Quaternion quat2(0.8f, 1.f, 0.f, -1.f);
		 if (quat1 == quat2) {
			...
		 }
		 \endcode

		 \sa operator!=()

		 \since v0.3.0
		*/
		_NODISCARD bool operator==(const Quaternion& other) const noexcept;
		/*!
		 \brief Inequality operator.
		 \details Checks if the respective components of the caller and of the \a other are inequal.

		 \param[in] other The ae::Quaternion that will be compared with the caller ae::Quaternion

		 \return True if at least one of the caller and the \a other's components is inequal, false otherwise

		 \par Example:
		 \code
		 ae::Quaternion quat1(0.45f, 0.f, 1.f, -1.f);
		 ae::Quaternion quat2(0.8f, 1.f, 0.f, -1.f);
		 if (quat1 != quat2) {
			...
		 }
		 \endcode

		 \sa operator==()

		 \since v0.3.0
		*/
		_NODISCARD bool operator!=(const Quaternion& other) const noexcept;

		// Friend operator(s)
		/*!
		 \brief Unary negation operator.
		 \details Performs a memberwise negation of the ae::Quaternion's components in which their sign will be inversed (positive to negative and vice-versa).

		 \param[in] quat The ae::Quaternion of which the sign of its angle and its axes of rotation will be inversed (positive to negative and vice-versa)

		 \return An ae::Quaternion containing components with the opposite sign of the \a quat provided

		 \par Example:
		 \code
		 ae::Quaternion quat1(1.45f, 0.f, 1.f, -1.f);
		 ae::Quaternion quat2 = -quat1; // (-1.45f, 0.f, -1.f, 1.f)
		 \endcode

		 \since v0.3.0
		*/
		friend AEON_API Quaternion operator-(const Quaternion& quat) noexcept;

		// Public method(s)
		/*!
		 \brief Calculates and retrieves the ae::Quaternion's rotation in Euler angles in XYZ order.

		 \return A 3-dimensional ae::Vector containing the ae::Quaternion's rotation in Euler angles in radians

		 \par Example:
		 \code
		 ae::Quaternion quat = ae::Quaternion::rotation(0.45f, ae::Vector3f(1.f, 0.f, -1.f));
		 ae::Vector3f eulerAngles = quat.toEulerAngles();
		 \endcode

		 \sa fromEulerAngles()

		 \since v0.3.0
		*/
		_NODISCARD Vector3f toEulerAngles() const noexcept;
		/*!
		 \brief Rotates the \a vec provided by the ae::Quaternion's current rotation.

		 \param[in] vec The 3-dimensional ae::Vector to rotate based on the ae::Quaternion's rotation

		 \return The rotated 3-dimensional ae::Vector

		 \par Example:
		 \code
		 ae::Quaternion quat = ae::Quaternion::rotation(0.45f, ae::Vector3f(1.f, 0.f, -1.f));
		 ae::Vector3f vec(0.65f, 0.f, 0.3f);
		 ae::Vector3f rotatedVec = quat.rotate(vec);
		 // or
		 ae::Vector3f rotatedVec = quat * vec;
		 \endcode

		 \since v0.3.0
		*/
		_NODISCARD Vector3f rotate(const Vector3f& vec) const noexcept;
		/*!
		 \brief Retrieves the conjugate of the ae::Quaternion.
		 \details The conjugate of a quaternion has the same components but with inverted signs except for the 'w' component that remains the same.

		 \return The ae::Quaternion's conjugate

		 \par Example:
		 \code
		 ae::Quaternion quat = ae::Quaternion::rotation(0.45f, ae::Vector3f(1.f, 0.f, -1.f));
		 ae::Quaternion conjugateQuat = quat.conjugate();
		 \endcode

		 \since v0.3.0
		*/
		_NODISCARD Quaternion conjugate() const noexcept;
		/*!
		 \brief Calculates and retrieves the magnitude of the ae::Quaternion.

		 \return The magnitude of the ae::Quaternion

		 \par Example:
		 \code
		 ae::Quaternion quat = ae::Quaternion::rotation(0.45f, ae::Vector3f(1.f, 0.f, -1.f));
		 float magnitude = quat.magnitude();
		 \endcode

		 \since v0.3.0
		*/
		_NODISCARD float magnitude() const noexcept;
		/*!
		 \brief Calculates and retrieves the unit quaternion (normalized) of the ae::Quaternion.

		 \return The unit version of the ae::Quaternion

		 \par Example:
		 \code
		 ae::Quaternion quat = ae::Quaternion::rotation(0.45f, ae::Vector3f(1.f, 0.f, -1.f));
		 ae::Quaternion unitQuat = quat.normalize();
		 \endcode

		 \since v0.4.0
		*/
		_NODISCARD Quaternion normalize() const;

		/*!
		 \brief Retrieves the angle of rotation of the ae::Quaternion.

		 \return The angle of rotation stored in the 'w' component

		 \par Example:
		 \code
		 ae::Quaternion quat = ae::Quaternion::rotation(0.45f, ae::Vector3f(1.f, 0.f, -1.f));
		 float quatAngle = quat.getAngle();
		 \endcode

		 \sa getAxes()

		 \since v0.3.0
		*/
		_NODISCARD float getAngle() const noexcept;
		/*!
		 \brief Retrieves the axes of rotation of the ae::Quaternion.

		 \return The 3-dimensional ae::Vector containing the axes of rotation 'x', 'y' and 'z'

		 \par Example:
		 \code
		 ae::Quaternion quat = ae::Quaternion::rotation(0.45f, ae::Vector3f(1.f, 0.f, -1.f));
		 const ae::Vector3f& quatAxes = quat.getAxes();
		 \endcode

		 \sa getAngle()

		 \since v0.3.0
		*/
		_NODISCARD const Vector3f& getAxes() const noexcept;

		// Public static method(s)
		/*!
		 \brief Constructs an ae::Quaternion by providing two 3-dimensional unit vectors (normalized) and calculating the rotation between them.

		 \param[in] unitVec0 The first unit ae::Vector that'll be the starting point
		 \param[in] unitVec1 The second unit ae::Vector that'll be the ending point

		 \return An ae::Quaternion containing the necessary rotation to go from the first vector to the second

		 \par Example:
		 \code
		 ae::Vector3f vec0(35.f, 0.f, 15.f);
		 ae::Vector3f vec1(60.f, -25.f, -1.f);
		 ae::Quaternion rotation = ae::Quaternion::rotation(vec0.normalize(), vec1.normalize());
		 \endcode

		 \since v0.4.0
		*/
		_NODISCARD static Quaternion rotation(const Vector3f& unitVec0, const Vector3f& unitVec1);
		/*!
		 \brief Constructs an ae::Quaternion by providing an \a angle in radians and the \a axes of rotation.
		 \note The 3-dimensional ae::Vector containing the \a axes of rotation must be a unit vector (normalized).

		 \param[in] angle The angle of rotation in radians
		 \param[in] axes The 3-dimensional unit ae::Vector containing the axes of rotation

		 \return An ae::Quaternion containing the equivalent rotation

		 \par Example:
		 \code
		 ae::Quaternion rotation = ae::Quaternion::rotation(1.13f, ae::Vector3f(20.f, 12.f, -5.f).normalize());
		 \endcode

		 \since v0.4.0
		*/
		_NODISCARD static Quaternion rotation(float angle, const Vector3f& axes) noexcept;
		/*!
		 \brief Constructs an ae::Quaternion by providing an \a angle in radians along the X axis.

		 \param[in] angle The angle of rotation in radians

		 \return An ae::Quaternion containing the equivalent rotation along the X axis

		 \par Example:
		 \code
		 ae::Quaternion rotationX = ae::Quaternion::rotationX(1.13f);
		 \endcode

		 \sa rotationY(), rotationZ()

		 \since v0.3.0
		*/
		_NODISCARD static Quaternion rotationX(float angle) noexcept;
		/*!
		 \brief Constructs an ae::Quaternion by providing an \a angle in radians along the Y axis.

		 \param[in] angle The angle of rotation in radians

		 \return An ae::Quaternion containing the equivalent rotation along the Y axis

		 \par Example:
		 \code
		 ae::Quaternion rotationY = ae::Quaternion::rotationY(1.13f);
		 \endcode

		 \sa rotationX(), rotationZ()

		 \since v0.3.0
		*/
		_NODISCARD static Quaternion rotationY(float angle) noexcept;
		/*!
		 \brief Constructs an ae::Quaternion by providing an \a angle in radians along the Z axis.

		 \param[in] angle The angle of rotation in radians

		 \return An ae::Quaternion containing the equivalent rotation along the Z axis

		 \par Example:
		 \code
		 ae::Quaternion rotationZ = ae::Quaternion::rotationZ(1.13f);
		 \endcode

		 \sa rotationX(), rotationY()

		 \since v0.3.0
		*/
		_NODISCARD static Quaternion rotationZ(float angle) noexcept;
		/*!
		 \brief Calculates and retrieves the dot product between the \a q0 and \a q1 ae::Quaternion objects.

		 \param[in] q0 The first ae::Quaternion that'll be used to calculate the dot product between itself and \a q1
		 \param[in] q1 The second ae::Quaternion that'll be used to calculate the dot product between \a q0 and itself

		 \return The dot product between the \a q0 and \a q1 ae::Quaternion objects

		 \par Example:
		 \code
		 ae::Quaternion quat0 = ae::Quaternion::rotation(1.13f, ae::Vector3f(20.f, 12.f, -5.f).normalize());
		 ae::Quaternion quat1 = ae::Quaternion::rotationX(0.45f);
		 float dotProduct = ae::Quaternion::dot(quat0, quat1);
		 \endcode

		 \since v0.3.0
		*/
		_NODISCARD static float dot(const Quaternion& q0, const Quaternion& q1) noexcept;

	private:
		// Member data
		float         w; //!< The real number that stores the angle
		union {
			Vector3f  v; //!< The imaginary numbers that store the axes of rotation
			struct {
				float x; //!< The imaginary number that stores the x axis
				float y; //!< The imaginary number that stores the y axis
				float z; //!< The imaginary number that stores the z axis
			};
		};
	};
}
#endif // Aeon_Math_Quaternion_H_

/*!
 \class ae::Quaternion
 \ingroup math

 The ae::Quaternion class can be used to represent a rotation in a 3-dimensional
 space whilst avoiding gimbal lock which is susceptible to Euler angles.

 The quaternion is comprised of a real number 'w' that stores the angle and three
 imaginary numbers 'x', 'y' and 'z' which store the axes of rotation.

 Euler angles are much easier to visualize but they're unfortunately susceptible
 to gimbal lock that occurs once two of the three axes of rotation are situated
 in a parallel configuration. Because of this parallel configuration, we
 effectively lose one degree of freedom which means that once we try to perform
 a rotation on one of the two blocked axes, both of the linked axes will be
 rotated.

 Layout of a quaternion: q = w(angle) + xi(x axis) + yj(y axis) + zk(z axis)

 \author Filippos Gleglakos
 \version v0.4.0
 \date 2020.05.04
 \copyright MIT License
*/