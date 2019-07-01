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

#ifndef Aeon_Math_Vector3_H_
#define Aeon_Math_Vector3_H_

#include <AEON/Math/Vector.h>

namespace ae
{
	/*!
	 \brief The struct that represents the partially specialized template used for a 3-dimensional mathematical vector of type T.
	 \details This struct can be used as a 3D point in space, as a simple 3D vector and a 3D directional vector, and as a container.
	 \note Only arithmetic types are allowed (float, int, etc.).
	*/
	template <typename T>
	struct [[nodiscard]] Vector<T, 3>
	{
		// Member data
		union {
			std::array<T, 3> elements; //!< The array of elements
			struct {
				T            x;        //!< The x element
				T            y;        //!< The y element
				T            z;        //!< The z element
			};
			Vector<T, 2>     xy;       //!< The x and y elements
		};

		// Static member data
		static const Vector<float, 3> Up;       //!< The global upward vector
		static const Vector<float, 3> Down;     //!< The global downward vector
		static const Vector<float, 3> Left;     //!< The global leftward vector
		static const Vector<float, 3> Right;    //!< The global rightward vector
		static const Vector<float, 3> Forward;  //!< The global forward vector
		static const Vector<float, 3> Backward; //!< The global backward vector

		static const Vector<float, 3> XAxis; //!< The global X axis vector
		static const Vector<float, 3> YAxis; //!< The global Y axis vector
		static const Vector<float, 3> ZAxis; //!< The global Z axis vector

		// Constructor(s)
		/*!
		 \brief Default constructor.
		 \details Sets the elements to the value 0 of the type provided.

		 \since v0.2.0
		*/
		constexpr Vector() noexcept
			: elements()
		{
		}
		/*!
		 \brief Constructs the ae::Vector by providing a \a scalar value of the appropriate type.
		 \details Sets the elements to the value of the \a scalar.

		 \param[in] scalar The scalar value that will be attributed to the elements

		 \par Example:
		 \code
		 constexpr ae::Vector3f vec3f(5.f); // the elements x, y and z are set to 5.f
		 \endcode

		 \since v0.2.0
		*/
		explicit constexpr Vector(T scalar) noexcept
			: x(scalar)
			, y(scalar)
			, z(scalar)
		{
		}
		/*!
		 \brief Constructs the ae::Vector providing specific \a x, \a y and \a z elements.
		 \details Sets the ae::Vector's elements to the elements provided.

		 \param[in] x The element that will be attributed to the x element
		 \param[in] y The element that will be attributed to the y element
		 \param[in] z The element that will be attributed to the z element

		 \par Example:
		 \code
		 constexpr ae::Vector3f vec3f(0.5f, 0.3f, 2.f);
		 \endcode

		 \since v0.2.0
		*/
		constexpr Vector(T x, T y, T z) noexcept
			: x(x)
			, y(y)
			, z(z)
		{
		}
		/*!
		 \brief Constructs the ae::Vector by providing specific \a coordinates.
		 \details Sets the ae::Vector's elements to the \a coordinates provided.
		 \note It's up to the user to provide the correct number of elements.

		 \param[in] coordinates The initializer list of elements

		 \par Example:
		 \code
		 ae::Vector3f vec3f({ 3.f, 2.f, 5.f });
		 // or
		 ae::Vector3f vec3f = { 3.f, 2.f, 5.f };
		 \endcode

		 \since v0.2.0
		*/
		Vector(std::initializer_list<T> coordinates) noexcept
			: elements()
		{
			// Copy the values until the maximum number of elements that both the vector and the list hold
			std::copy_n(coordinates.begin(), Math::min(elements.size(), coordinates.size()), elements.begin());
		}
		/*!
		 \brief Constructs the ae::Vector by providing an std::array of coordinates.
		 \details Set the ae::Vector's elements to the std::array of \a coordinates provided.

		 \param[in] coordinates The C++11 array of coordinates to assign to the ae::Vector

		 \par Example:
		 \code
		 constexpr std::array<float, 3> coordinates = { 5.f, 8.f, 2.f };
		 constexpr ae::Vector3f vec3f(coordinates);
		 \endcode

		 \since v0.2.0
		*/
		explicit constexpr Vector(const std::array<T, 3>& coordinates) noexcept
			: elements(coordinates)
		{
		}
		/*!
		 \brief Constructs the ae::Vector by providing a C-style array of coordinates.
		 \details Sets the ae::Vector's elements to the C-style array of \a coordinates provided.

		 \param[in] coordinates The C-style array of coordinates to assign to the ae::Vector

		 \par Example:
		 \code
		 float coordinates[3] = { 2.f, 1.f, -4.f };
		 ae::Vector3f vec3f(coordinates);
		 \endcode

		 \since v0.2.0
		*/
		explicit Vector(const T(&coordinates)[3]) noexcept
			: elements()
		{
			// Copy the values of the C-style array to the vector's elements
			std::copy_n(coordinates, elements.size(), elements.begin());
		}
		/*!
		 \brief Constructs the ae::Vector by providing an ae::Vector of another type and/or with a different number of elements.
		 \details Sets the ae::Vector's elements to the \a vecUN's elements, up to 3 elements.

		 \param[in] vecUN The ae::Vector of another type and/or with a different number of elements to assign to the ae::Vector

		 \par Example:
		 \code
		 // Different type, same number of elements
		 ae::Vector3i vec3i(5, 3, -2);
		 ae::Vector3f vec3f_1 = vec3i;

		 // Same type, different number of elements
		 ae::Vector4f vec4f(5.f, 3.f, 2.f, 1.f);
		 ae::Vector3f vec3f_2(vec4f);

		 // Different type, different number of elements
		 ae::Vector2i vec2i(5, 3);
		 ae::Vector3f vec3f_3 = vec2i;
		 \endcode

		 \since v0.2.0
		*/
		template <typename U, size_t n2>
		Vector(const Vector<U, n2>& vecUN) noexcept
			: elements()
		{
			const size_t MIN_N = Math::min(elements.size(), n2);

			// Avoid the casts if the T type is equal to the U type during compilation
			if constexpr (std::is_same_v<T, U>) {
				std::copy_n(vecUN.elements.begin(), MIN_N, elements.begin());
			}
			else {
				for (size_t i = 0; i < MIN_N; ++i) {
					elements[i] = static_cast<T>(vecUN.elements[i]);
				}
			}
		}
		/*!
		 \brief Constructs the ae::Vector by providing a 2-dimensional ae::Vector and a \a z element.
		 \details The \a vecU can be of the same type as the ae::Vector to be constructed or of a different type.
		 \note The cast condition won't have any impact on performance during runtime.

		 \param[in] vecU The 2-dimensional ae::Vector to assign to the ae::Vector
		 \param[in] z The element that will be attributed to the z element

		 \par Example:
		 \code
		 // Same type
		 ae::Vector2f vec2f_1(5.f, 3.f);
		 ae::Vector3f vec3f_1(vec2f_1, -1.f);

		 // Different type
		 ae::Vector2i ve2i_1(5, 3);
		 ae::Vector3f vec3f_2(vec2i_1, -1.f);
		 \endcode

		 \since v0.2.0
		*/
		template <typename U>
		constexpr Vector(const Vector<U, 2>& vecU, T z) noexcept
			: xy(vecU)
		{
			this->z = z;
		}
		/*!
		 \brief Copy constructor.
		 \details Sets the ae::Vector's elements to the \a copy's elements.

		 \param[in] copy The ae::Vector of the same type and of the same number of elements that will be copied

		 \par Example:
		 \code
		 constexpr ae::Vector3f vec3f_1(0.5f, 0.3f, -1.f);
		 constexpr ae::Vector3f vec3f_2(vec3f_1);
		 // or
		 constexpr ae::Vector3f vec3f_2 = vec3f_1;
		 \endcode

		 \since v0.2.0
		*/
		constexpr Vector(const Vector<T, 3>& copy) noexcept
			: elements(copy.elements)
		{
		}
		/*!
		 \brief Move constructor.
		 \details Performs a move operation on the \a rvalue's elements to the lvalue ae::Vector's elements.

		 \param[in] rvalue The ae::Vector rvalue that will be moved

		 \par Example:
		 \code
		 constexpr ae::Vector3f vec3f_1(0.5f, 0.3f, -1.f);
		 constexpr ae::Vector3f vec3f_2(0.3f, 0.1f, 0.f);
		 ae::Vector3f vec3f_3(vec3f_1 + vec3f_2);
		 // or
		 ae::Vector3f vec3f_3 = vec3f_1 + vec3f_2;
		 \endcode

		 \since v0.2.0
		*/
		constexpr Vector(Vector<T, 3>&& rvalue) noexcept
			: elements(std::move(rvalue.elements))
		{
		}

		// Operator(s)
		/*!
		 \brief Assignment operator.
		 \details Performs a memberwise assignment from the \a other's elements to the caller ae::Vector's ones.

		 \param[in] other The ae::Vector of which its elements will be copied over to the caller's ones

		 \return The caller ae::Vector containing the new elements, these calls can be chained together one after the other

		 \par Example:
		 \code
		 ae::Vector3f vec3f_1(0.5f, 0.3f, -1.f);
		 ae::Vector3f vec3f_2(0.3f, 0.1f, 1.f);
		 ...
		 vec3f_1 = vec3f_2;
		 \endcode

		 \since v0.2.0
		*/
		Vector<T, 3>& operator=(const Vector<T, 3>& other) noexcept
		{
			// Check that the caller object won't be assigned to itself (ignored in Release mode)
			if constexpr (AEON_DEBUG) {
				if (this == &other) {
					AEON_LOG_ERROR("Invalid assignment", "Attempt to assign an object to itself.\nAborting operation.");
					return *this;
				}
			}

			elements = other.elements;
			return *this;
		}
		/*!
		 \brief Move assignment operator.
		 \details Performs a memberwise move assignment from the \a rvalue's elements to the caller ae::Vector's ones.

		 \param[in] rvalue The rvalue ae::Vector of which its elements will be moved over to the caller's ones

		 \return The caller ae::Vector containing the moved elements, these calls can be chained together one after the other

		 \par Example:
		 \code
		 ae::Vector3f vec3f_1(0.5f, 0.3f, -1.f);
		 ae::Vector3f vec3f_2(0.3f, 0.1f, 1.f);
		 ae::Vector3f vec3f_3(0.5f, 0.8f, -1.f);
		 ...
		 vec3f_3 = vec3f_1 + vec3f_2;
		 \endcode

		 \since v0.2.0
		*/
		Vector<T, 3>& operator=(Vector<T, 3>&& rvalue) noexcept
		{
			elements = std::move(rvalue.elements);
			return *this;
		}
		/*!
		 \brief Element access operator.
		 \details Retrieves the element situated at the \a index provided.
		 \note This operator won't verify if the \a index is situated within the array's limits, it's up to the user to provide a valid index.

		 \param[in] index The position of the element to retrieve

		 \return The element situated at the indicated position within the array

		 \par Example:
		 \code
		 ae::Vector3f vec3f(0.5f, 0.3f, -1.f);
		 float& firstElement = vec3f[0]; // 0.5f
		 \endcode

		 \sa at()

		 \since v0.2.0
		*/
		[[nodiscard]] constexpr T& operator[](size_t index) noexcept
		{
			// Log an error message if the index isn't within the array's limits (ignored in Release mode)
			if constexpr (AEON_DEBUG) {
				if (index >= elements.size()) {
					AEON_LOG_ERROR("Invalid array index", "The index provided isn't situated within the array's limits.\nRetrieving the array's x element.");
					return x;
				}
			}

			return elements[index];
		}
		/*!
		 \brief Element access operator.
		 \details Retrieves the element situated at the \a index provided.
		 \note This operator won't verify if the \a index is situated within the array's limits, it's up to the user to provide a valid index.

		 \param[in] index The position of the element to retrieve

		 \return The element situated at the indicated position within the array

		 \par Example:
		 \code
		 constexpr ae::Vector3f vec3f(0.5f, 0.3f, -1.f);
		 const float& firstElement = vec3f[0]; // 0.5f
		 \endcode

		 \sa at()

		 \since v0.2.0
		*/
		[[nodiscard]] constexpr const T& operator[](size_t index) const noexcept
		{
			// Log an error message if the index isn't within the array's limits (ignored in Release mode)
			if constexpr (AEON_DEBUG) {
				if (index >= elements.size()) {
					AEON_LOG_ERROR("Invalid array index", "The index provided isn't situated within the array's limits.\nRetrieving the array's x element.");
					return x;
				}
			}

			return elements[index];
		}

		// Method(s)
		/*!
		 \brief Retrieves the element situated at the \a index provided.
		 \details Unlike the [] operator, this method will verify that the \a index is situated within the array's limits.

		 \param[in] index The position of the element to retrieve

		 \return The element situated at the indication position or the first position if the \a index isn't within the array's limits

		 \par Example:
		 \code
		 ae::Vector3f vec3f(0.5f, 0.3f, -1.f);
		 float& firstElement = vec3f.at(0); // 0.5f
		 \endcode

		 \sa operator[]()

		 \since v0.2.0
		*/
		[[nodiscard]] constexpr T& at(size_t index) noexcept
		{
			// Log an error message if the index isn't within the array's limits (ignored in Release mode)
			if constexpr (AEON_DEBUG) {
				if (index >= elements.size()) {
					AEON_LOG_ERROR("Invalid array index", "The index provided isn't situated within the array's limits.\nRetrieving the array's x element.");
					return x;
				}
			}

			return elements.at(index);
		}
		/*!
		 \brief Retrieves the element situated at the \a index provided.
		 \details Unlike the [] operator, this method will verify that the \a index is situated within the array's limits.

		 \param[in] index The position of the element to retrieve

		 \return The element situated at the indication position or the first position if the \a index isn't within the array's limits

		 \par Example:
		 \code
		 constexpr ae::Vector3f vec3f(0.5f, 0.3f, -1.f);
		 const float& firstElement = vec3f.at(0); // 0.5f
		 \endcode

		 \sa operator[]()

		 \since v0.2.0
		*/
		[[nodiscard]] constexpr const T& at(size_t index) const noexcept
		{
			// Log an error message if the index isn't within the array's limits (ignored in Release mode)
			if constexpr (AEON_DEBUG) {
				if (index >= elements.size()) {
					AEON_LOG_ERROR("Invalid array index", "The index provided isn't situated within the array's limits.\nRetrieving the array's x element.");
					return x;
				}
			}

			return elements.at(index);
		}

		/*!
		 \brief Calculates and retrieves the ae::Vector's magnitude (or length).
		 \details The magnitude is calculated using the Pythagorean theorem: \f$ \| \overrightarrow{V} \| = \sqrt{x^2 + y^2 + z^2} \f$.

		 \return The ae::Vector's magnitude

		 \par Example:
		 \code
		 ae::Vector3f vec3f(0.5f, 0.2f, -1.f);
		 float magnitude = vec3f.magnitude();
		 \endcode

		 \since v0.2.0
		*/
		[[nodiscard]] T magnitude() const
		{
			return Math::sqrt(x * x + y * y + z * z);
		}
		/*!
		 \brief Calculates and retrieves the unit vector of the ae::Vector.
		 \details A unit vector (or a normalized vector) is a vector with a magnitude of 1.
		 Unit vectors are used when we only need to represent a direction and not a magnitude.\n
		 The unit vector is calculated using the following equation: \f$ \hat{V} = \frac{\overrightarrow{V}}{\| \overrightarrow{V} \|} \f$.
		 \note Only floating point types are allowed (float, double, long double).

		 \return The ae::Vector's unit vector (or normalized vector)

		 \par Example:
		 \code
		 ae::Vector3f vec3f(2.5f, 5.2f, -1.f);
		 ae::Vector3f unitVec3f = vec3f.normalize();
		 \endcode

		 \since v0.2.0
		*/
		template <typename = Math::FLOATING_POINT_POLICY<T>>
		[[nodiscard]] Vector<T, 3> normalize() const
		{
			const T MAGNITUDE = magnitude();

			// Protection against division by 0 (ignored in Release mode)
			if constexpr (AEON_DEBUG) {
				if (MAGNITUDE == static_cast<T>(0)) {
					AEON_LOG_WARNING("Division by 0", "The ae::Vector's magnitude is equal to 0.\nReturning copy of caller.");
					return *this;
				}
			}

			return (*this / MAGNITUDE);
		}
	};

	/*!
	 \relates Vector
	 \brief Calculates and retrieves the cross product between the \a v1 and the \a v2.
	 \details The cross product of the two vectors is a third vector that is perpendicular to the plane in which the first two vectors reside in.
	 \note The order in which the two vectors are provided is imporant.\n
	 The vector retrieved will have to be normalized to get the normal vector to the plane unless the vectors \a v1 and \a v2 were already unit vectors.
	 
	 \param[in] v1 The first ae::Vector that will be used to calculate the cross product between itself and the \a v2 vector
	 \param[in] v2 The second ae::Vector that will be used to calculate the cross product between the \a v1 vector and itself

	 \return An ae::Vector containing the cross product between the vectors \a v1 and \a v2

	 \par Example:
	 \code
	 constexpr ae::Vector3f vec3f_1(10.f, 15.f, -5.f);
	 constexpr ae::Vector3f vec3f_2(2.f, 10.f, -2.f);
	 ae::Vector3f crossProduct = ae::cross(vec3f_1.normalize(), vec3f_2.normalize());
	 \endcode

	 \since v0.2.0
	*/
	template <typename T>
	[[nodiscard]] constexpr Vector<T, 3> cross(const Vector<T, 3>& v1, const Vector<T, 3>& v2) noexcept
	{
		return Vector<T, 3>(v1.y * v2.z - v1.z * v2.y,
		                    v1.z * v2.x - v1.x * v2.z,
		                    v1.x * v2.y - v1.y * v2.x);
	}

	// Typedef(s)
	template <typename T>
	using Vector3 = Vector<T, 3>;           //!< A 3-dimensional vector of type T

	using Vector3f = Vector3<float>;        //!< A 3-dimensional vector of floats
	using Vector3d = Vector3<double>;       //!< A 3-dimensional vector of doubles
	using Vector3i = Vector3<int>;          //!< A 3-dimensional vector of ints
	using Vector3u = Vector3<unsigned int>; //!< A 3-dimensional vector of unsigned ints

	// Static member data
	template <typename T>
	const Vector<float, 3> Vector<T, 3>::Up(0.f, 1.f, 0.f);       //!< The global upward vector
	template <typename T>
	const Vector<float, 3> Vector<T, 3>::Down(0.f, -1.f, 0.f);    //!< The global downward vector
	template <typename T>
	const Vector<float, 3> Vector<T, 3>::Left(-1.f, 0.f, 0.f);    //!< The global leftward vector
	template <typename T>
	const Vector<float, 3> Vector<T, 3>::Right(1.f, 0.f, 0.f);    //!< The global rightward vector
	template <typename T>
	const Vector<float, 3> Vector<T, 3>::Forward(0.f, 0.f, -1.f); //!< The global forward vector
	template <typename T>
	const Vector<float, 3> Vector<T, 3>::Backward(0.f, 0.f, 1.f); //!< The global backward vector

	template <typename T>
	const Vector<float, 3> Vector<T, 3>::XAxis(1.f, 0.f, 0.f); //!< The global X axis vector
	template <typename T>
	const Vector<float, 3> Vector<T, 3>::YAxis(0.f, 1.f, 0.f); //!< The global Y axis vector
	template <typename T>
	const Vector<float, 3> Vector<T, 3>::ZAxis(0.f, 0.f, 1.f); //!< The global Z axis vector
}
#endif // Aeon_Math_Vector3_H_

/*!
 \struct ae::Vector<T, 3>
 \ingroup math

 The ae::Vector<T, 3> struct is a partially specialized template that defines a
 3-dimensional mathematical vector (x, y, z).

 The coordinates of the partially specialized template can be accessed through
 the 'elements' member or through the 'x', 'y' and 'z' members or through the
 'xy' member that is a 2-dimensional vector. These members occupy the same
 block of memory, they simple offer a more intuitive way of accessing said
 elements.

 Usage example:
 \code
 ae::Vector3f vec3f_1(0.5f, 1.f, 5.f);
 vec3f_1.x = 5.f;

 ae::Vector3f vec3f_2(2.f, 5.f, 8.f);
 ae::Vector3f vec3f_3 = vec3f_1 + vec3f_2;

 bool different = (vec3f_1 != vec3f_2);

 float distance = ae::distance(vec3f_1, vec3f_2);
 \endcode

 \author Filippos Gleglakos
 \version v0.2.0
 \date 2019-06-18
 \copyright MIT License
*/