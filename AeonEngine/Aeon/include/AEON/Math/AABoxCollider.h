// MIT License
// 
// Copyright(c) 2019 Filippos Gleglakos
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

#ifndef Aeon_Math_AABoxCollider_H_
#define Aeon_Math_AABoxCollider_H_

#include <AEON/Math/Vector.h>
#include <AEON/Math/Vector2.h>
#include <AEON/Math/Vector3.h>
#include <AEON/Math/Vector4.h>

namespace ae
{
	/*!
	 \brief The struct that represents an n-dimensional Axis-Aligned Box Collider used to store minimum and maximum coordinates or a position and a size.
	 \details This struct can be used for collision detection or simply as a container.
	*/
	template <typename T, size_t n, typename = VECTOR_POLICY<T, n>>
	struct _NODISCARD AABoxCollider
	{
		// Member data
		union {
			struct {
				Vector<T, n> min;      //!< The minimum coordinates of the box
				Vector<T, n> max;      //!< The maximum coordinates of the box
			};
			struct {
				Vector<T, n> position; //!< The position of the box
				Vector<T, n> size;     //!< The size of the box
			};
		};

		// Constructor(s)
		/*!
		 \brief Default constructor.
		 \details Sets the member data to their default values.

		 \since v0.3.0
		*/
		_CONSTEXPR17 AABoxCollider() noexcept
			: min()
			, max()
		{
		}
		/*!
		 \brief Constructs the ae::AABoxCollider by providing the minimum and maximum coordinates or the position and the size.
		 
		 \param[in] v0 The ae::Vector containing the minimum coordinates or the position
		 \param[in] v1 The ae::Vector containing the maximum coordinates or the size

		 \par Example:
		 \code
		 constexpr ae::AABoxCollider2f boxCollider2f(ae::Vector2f(0.f, 0.f), ae::Vector2f(2.f, 2.f));
		 \endcode

		 \since v0.3.0
		*/
		AABoxCollider(const Vector<T, n>& v0, const Vector<T, n>& v1) noexcept
			: min(v0)
			, max(v1)
		{
		}
		/*!
		 \brief Constructs the ae::AABoxCollider by providing specific values.
		 \details If four values are provided while the ae::AABoxCollider is 3-dimensional, the first two values will be assigned
		 to the minimum coordinates or the position and the other two will be assigned to the maximum coordinates or the size.
		 \note It's up to the user to provide the correct number of values and of the right type.

		 \param[in] val0 The first value
		 \param[in] val1 The second value
		 \param[in] val2 The third value
		 \param[in] val3 The fourth value
		 \param[in] values The remaining values that will be used if the ae::AABoxCollider contains more than 2 dimensions

		 \par Example:
		 \code
		 // 2-dimensional AABoxCollider
			// correct number of values
		 ae::AABoxCollider2f boxCollider2f_1(0.f, 1.f, 2.f, 3.f);      // min/position: (0.f, 1.f), max/size: (2.f, 3.f)
			// greater number of values (error)
		 ae::AABoxCollider2f boxCollider2f_2(0.f, 1.f, 2.f, 3.f, 4.f); // error

		 // 3-dimensional AABoxCollider
			// correct number of values
		 ae::AABoxCollider3f boxCollider3f_1(0.f, 1.f, 2.f, 3.f, 4.f, 5.f);      // min/position: (0.f, 1.f, 2.f), max/size: (3.f, 4.f, 5.f)
			// greater number of values (error)
		 ae::AABoxCollider3f boxCollider3f_2(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f); // error
			// fewer number of values (values are evenly distributed)
		 ae::AABoxCollider3f boxCollider3f_3(0.f, 1.f, 2.f, 3.f);                // min/position: (0.f, 1.f, 0.f), max/size: (2.f, 3.f, 0.f)
		 \endcode

		 \since v0.3.0
		*/
		template <typename... Values>
		AABoxCollider(T val0, T val1, T val2, T val3, Values... values) noexcept
			: min()
			, max()
		{
			// Add the parameters received into a C++11 array
			_CONSTEXPR17 const size_t MIN_N_ALL = Math::min(n * 2, sizeof...(Values) + 4);
			const std::array<T, MIN_N_ALL> VALUES_PACK = { val0, val1, val2, val3, values... };

			// Copy the values from the C++11 array to the 'min' and 'max' members
			_CONSTEXPR17 const size_t MIN_N = MIN_N_ALL / 2;
			for (size_t i = 0; i < MIN_N; ++i) {
				min[i] = VALUES_PACK[i];
				max[i] = VALUES_PACK[MIN_N + i];
			}
		}
		/*!
		 \brief Constructs the ae::AABoxCollider by providing an ae::AABoxCollider of another type and/or with a different number of values.
		 \details Sets the ae::AABoxCollider's values to the \a aaboxUN's values, up to the maximum number of values that both objects hold.\n
		 If the \a aaboxUN possesses less values, the ae::AABoxCollider's remaining values will be set to 0.

		 \param[in] aaboxUN The ae::AABoxCollider of another type and/or with a different number of values to assign to the ae::AABoxCollider

		 \par Example:
		 \code
		 // Different type, same number of values
		 ae::AABoxCollider2i boxCollider2i(5, 3, 8, 10);
		 ae::AABoxCollider2f boxCollider2f_1 = boxCollider2i;

		 // Same type, different number of values
		 ae::AABoxCollider3f boxCollider3f(ae::Vector3f(5.f, 3.f, 2.f), ae::Vector3f(8.f, 10.f, 6.f));
		 ae::AABoxCollider2f boxCollider2f_2(boxCollider3f); // min/position: (5.f, 3.f), max/size: (8.f, 10.f)
		 \endcode

		 \since v0.3.0
		*/
		template <typename U, size_t n2>
		AABoxCollider(const AABoxCollider<U, n2>& aaboxUN) noexcept
			: min(aaboxUN.min)
			, max(aaboxUN.max)
		{
		}
		/*!
		 \brief Copy constructor.
		 \details Sets the ae::AABoxCollider's values to the \a copy's values.

		 \param[in] copy The ae::AABoxCollider of which its values will be copied over to the caller ae::AABoxCollider

		 \par Example:
		 \code
		 ae::AABoxCollider2i boxCollider2i_1(5, 3, 8, 10);
		 ae::AABoxCollider2i boxCollider2i_2(boxCollider2i_1);
		 \endcode

		 \since v0.3.0
		*/
		AABoxCollider(const AABoxCollider<T, n>& copy) noexcept
			: min(copy.min)
			, max(copy.max)
		{
		}
		/*!
		 \brief Move constructor.
		 \details Performs a memberwise move from the \a rvalue's values to the ae::AABoxCollider's.

		 \param[in] rvalue The ae::AABoxCollider of which its values will be moved over to the caller ae::AABoxCollider

		 \par Example:
		 \code
		 ae::AABoxCollider2i boxCollider2i_1(5, 3, 8, 10);
		 ...
		 ae::AABoxCollider2i boxCollider2i_2(std::move(boxCollider2i_1));
		 \endcode

		 \since v0.3.0
		*/
		AABoxCollider(AABoxCollider<T, n>&& rvalue) noexcept
			: min(std::move(rvalue.min))
			, max(std::move(rvalue.max))
		{
		}

		// Operator(s)
		/*!
		 \brief Assignment operator.
		 \details Performs a memberwise copy of the \a other's values to the ae::AABoxCollider's values.

		 \param[in] other The ae::AABoxCollider of which its values will be copied over to the caller ae::AABoxCollider

		 \return The caller ae::AABoxCollider containing the new values

		 \par Example:
		 \code
		 ae::AABoxCollider2i boxCollider2i_1(5, 3, 8, 10);
		 ae::AABoxCollider2i boxCollider2i_2(2, 6, 0, 5);
		 ...
		 boxCollider2i_1 = boxCollider2i_2;
		 \endcode

		 \since v0.3.0
		*/
		AABoxCollider<T, n>& operator=(const AABoxCollider<T, n>& other) noexcept
		{
			// Check that the object won't be assigned to itself (ignored in Release mode)
			if _CONSTEXPR_IF (AEON_DEBUG) {
				if (this == &other) {
					AEON_LOG_ERROR("Invalid assignment", "Attempt to assign an object to itself.\nAborting operation.");
					return *this;
				}
			}

			min = other.min;
			max = other.max;

			return *this;
		}
		/*!
		 \brief Move assignment operator.
		 \details Performs a memberwise move from the \a rvalue's values to the ae::AABoxCollider's values.

		 \param[in] rvalue The ae::AABoxCollider of which its values will be moved over to the caller ae::AABoxCollider

		 \return The caller ae::AABoxCollider containing the moved values

		 \par Example:
		 \code
		 ae::AABoxCollider2i boxCollider2i_1(5, 3, 8, 10);
		 ae::AABoxCollider2i boxCollider2i_2(2, 6, 0, 5);
		 ...
		 boxCollider2i_1 = std::move(boxCollider2i_2);
		 \endcode

		 \since v0.3.0
		*/
		AABoxCollider<T, n>& operator=(AABoxCollider<T, n>&& rvalue) noexcept
		{
			min = std::move(rvalue.min);
			max = std::move(rvalue.max);
			
			return *this;
		}
		/*!
		 \brief Equality operator.
		 \details Checks if the respective values of the caller and of the \a other are equal.

		 \param[in] other The ae::AABoxCollider that will be compared with the caller

		 \return True if all the values of the caller and of the \a other are respectively equal, false otherwise

		 \par Example:
		 \code
		 ae::AABoxCollider2f boxCollider2f_1(1.f, 0.f, 5.f, 3.f);
		 ae::AABoxCollider2f boxCollider2f_2(3.f, -1.f, 10.f, 1.f);
		 if (boxCollider2f_1 == boxCollider2f_2) {
			...
		 }
		 \endcode

		 \sa operator!=()

		 \since v0.3.0
		*/
		_NODISCARD bool operator==(const AABoxCollider<T, n>& other) const noexcept
		{
			return (min == other.min) && (max == other.max);
		}
		/*!
		 \brief Inequality operator.
		 \details Checks if the respective values of the caller and of the \a other are inequal.

		 \param[in] other The ae::AABoxCollider that will be compared with the caller

		 \return True if at least one of the values of the caller and of the \a other is inequal, false otherwise

		 \par Example:
		 \code
		 ae::AABoxCollider2f boxCollider2f_1(1.f, 0.f, 5.f, 3.f);
		 ae::AABoxCollider2f boxCollider2f_2(3.f, -1.f, 10.f, 1.f);
		 if (boxCollider2f_1 != boxCollider2f_2) {
			...
		 }
		 \endcode

		 \sa operator==()

		 \since v0.3.0
		*/
		_NODISCARD bool operator!=(const AABoxCollider<T, n>& other) const noexcept
		{
			return !(*this == other);
		}

		// Method(s)
		/*!
		 \brief Checks if a n-dimensional point is situated within the ae::AABoxCollider's zone.

		 \param[in] point A n-dimensional ae::Vector containing the point's coordinates

		 \return True if the \a point is situated within the ae::AABoxCollider, false otherwise

		 \par Example:
		 \code
		 ae::AABoxCollider2f boxCollider(0.f, 0.f, 5.f, 10.f);
		 ae::Vector2f point(0.f, 5.f);
		 if (boxCollider.contains(point)) {
			...
		 }
		 \endcode

		 \sa intersects()

		 \since v0.3.0
		*/
		_NODISCARD bool contains(const Vector<T, n>& point) const noexcept
		{
			return (point >= min && point <= max);
		}
		/*!
		 \brief Checks if the two ae::AABoxCollider objects are intersecting.
		 \details The ae::AABoxCollider containing the intersection in min and max coordinates will be assigned to the parameter \a intersection (if it was provided).

		 \param[in] other The ae::AABoxCollider to test with the caller
		 \param[in,out] intersection A pointer to the ae::AABoxCollider that will be assigned to the intersection, nullptr by default

		 \return True if the two ae::AABoxCollider objects are intersecting, false otherwise

		 \par Example:
		 \code
		 ae::AABoxCollider2f boxCollider_1(ae::Vector2f(0.f, 0.f), ae::Vector2f(5.f, 5.f));
		 ae::AABoxCollider2f boxCollider_2(ae::Vector2f(0.f, 5.f), ae::Vector2f(12.f, 12.f));
		 ae::AABoxCollider2f intersection;

		 if (boxCollider_1.intersects(boxCollider_2, &intersection)) {
			...
		 }
		 \endcode

		 \sa contains()

		 \since v0.3.0
		*/
		_NODISCARD bool intersects(const AABoxCollider<T, n>& other, AABoxCollider<T, n>* const intersection = nullptr) const noexcept
		{
			// Check if an intersection pointer was provided
			if (intersection) {
				// Calculate the intersection edges and check if they're intersecting
				intersection->min = ae::max(min, other.min);
				intersection->max = ae::min(max, other.max);

				return (intersection->min <= intersection->max);
			}

			return (min <= other.max && max >= other.min);
		}
	};

	// Typedef(s)
	template <typename T>
	using AABoxCollider2 = AABoxCollider<T, 2>;           //!< A 2-dimensional AABoxCollider of type T for collision detection
	template <typename T>
	using AABoxCollider3 = AABoxCollider<T, 3>;           //!< A 3-dimensional AABoxCollider of type T for collision detection

	using AABoxCollider2f = AABoxCollider2<float>;        //!< A 2-dimensional AABoxCollider of floats for collision detection
	using AABoxCollider2d = AABoxCollider2<double>;       //!< A 2-dimensional AABoxCollider of doubles for collision detection
	using AABoxCollider2i = AABoxCollider2<int>;          //!< A 2-dimensional AABoxCollider of ints for collision detection
	using AABoxCollider2u = AABoxCollider2<unsigned int>; //!< A 2-dimensional AABoxCollider of unsigned ints for collision detection

	using AABoxCollider3f = AABoxCollider3<float>;        //!< A 3-dimensional AABoxCollider of floats for collision detection
	using AABoxCollider3d = AABoxCollider3<double>;       //!< A 3-dimensional AABoxCollider of doubles for collision detection
	using AABoxCollider3i = AABoxCollider3<int>;          //!< A 3-dimensional AABoxCollider of ints for collision detection
	using AABoxCollider3u = AABoxCollider3<unsigned int>; //!< A 3-dimensional AABoxCollider of unsigned ints for collision detection

	template <typename T>
	using Box2 = AABoxCollider<T, 2>;                     //!< A 2-dimensional AABoxCollider of type T for containers
	template <typename T>
	using Box3 = AABoxCollider<T, 3>;                     //!< A 3-dimensional AABoxCollider of type T for containers
	
	using Box2f = Box2<float>;                            //!< A 2-dimensional AABoxCollider of floats for containers
	using Box2d = Box2<double>;                           //!< A 2-dimensional AABoxCollider of doubles for containers
	using Box2i = Box2<int>;                              //!< A 2-dimensional AABoxCollider of ints for containers
	using Box2u = Box2<unsigned int>;                     //!< A 2-dimensional AABoxCollider of unsigned ints for containers
	
	using Box3f = Box3<float>;                            //!< A 3-dimensional AABoxCollider of floats for containers
	using Box3d = Box3<double>;                           //!< A 3-dimensional AABoxCollider of doubles for containers
	using Box3i = Box3<int>;                              //!< A 3-dimensional AABoxCollider of ints for containers
	using Box3u = Box3<unsigned int>;                     //!< A 3-dimensional AABoxCollider of unsigned ints for containers
}
#endif // Aeon_Math_AABoxCollider_H_

/*!
 \struct ae::AABoxCollider
 \ingroup math

 The ae::AABoxCollider struct stands for Axis-Aligned Box Collider which contains the
 'min' and 'max' n-dimensional ae::Vector member data representing minimum and
 maximum coordinates. These elements can be used for collision detection
 between 2D or 3D entities, or simply as a container for minimum and maximum
 coordinates.

 The ae::AABoxCollider struct also possesses the 'position' and 'size' member which
 share the memory/adress as the 'min' and 'max' members respectively. They're simply
 meant to facilitate comprehension based on how the API user decides to use this
 struct.

 If the ae::AABoxCollider is used as a container, the typedef Box<T, n> is recommended
 as the API user won't be using it to detect any collisions.

 \author Filippos Gleglakos
 \version v0.3.0
 \date 2019.07.21
 \copyright MIT License
*/