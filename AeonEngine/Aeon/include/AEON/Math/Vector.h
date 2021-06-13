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

#ifndef Aeon_Math_Vector_H_
#define Aeon_Math_Vector_H_

#include <array>

#include <AEON/Math/Misc.h>

namespace ae
{
	// Template policy
	template <typename T, size_t n>
	using VECTOR_POLICY = std::enable_if_t<std::is_arithmetic_v<T> && (n >= 2)>; //!< The template parameters will only be enabled if T is an arithmetic type and n is greater or equal to 2

	/*!
	 \brief The struct that represents the primary template used for a mathematical vector of type T with n dimensions.
	 \note Only arithmetic types are allowed (float, int, etc.) and the minimum number of dimensions is 2.
	*/
	template <typename T, size_t n, typename = VECTOR_POLICY<T, n>>
	struct Vector
	{
		// Member(s)
		std::array<T, n> elements; //!< The array of elements

		// Constructor(s)
		/*!
		 \brief Default constructor.
		 \details Sets the elements to the value 0 of the type provided.

		 \since v0.6.0
		*/
		inline _CONSTEXPR17 Vector() noexcept
			: elements()
		{
		}
		/*!
		 \brief Constructs the ae::Vector by providing a \a scalar value of the appropriate type.
		 \details Sets the elements to the value of the \a scalar.

		 \param[in] scalar The scalar value that will be attributed to the elements

		 \par Example:
		 \code
		 ae::Vector<float, 6> vec6f(0.5f); // all elements set to 0.5f
		 \endcode

		 \since v0.6.0
		*/
		explicit inline _CONSTEXPR17 Vector(T scalar)
			: elements()
		{
			elements.fill(scalar);
		}
		/*!
		 \brief Constructs the ae::Vector by providing specific coordinates.
		 \details Sets the ae::Vector's elements to the coordinates provided.
		 \note It's up to the user to provide the correct number of coordinates and of the right type.

		 \param[in] c1 The first coordinate to pass to the ae::Vector
		 \param[in] c2 The second coordinate to pass to the ae::Vector
		 \param[in] c3 The third coordinate to pass to the ae::Vector
		 \param[in] c4 The fourth coordinate to pass to the ae::Vector
		 \param[in] c5 The fifth coordinate to pass to the ae::Vector
		 \param[in] cN The remaining coordinates to pass to the ae::Vector

		 \par Example:
		 \code
		 ae::Vector<float, 5> vec5f(0.5f, 0.3f, -1.f, 1.f, 10.f);
		 \endcode

		 \since v0.6.0
		*/
		template <typename... Coords>
		inline _CONSTEXPR17 Vector(T c1, T c2, T c3, T c4, T c5, Coords... coordinates)
			: elements({ c1, c2, c3, c4, c5, coordinates... })
		{
		}
		/*!
		 \brief Constructs the ae::Vector by providing specific \a coordinates.
		 \details Sets the ae::Vector's elements to the \a coordinates provided.
		 \note It's up to the user to provide the correct number of elements.

		 \param[in] coordinates The initializer list of elements

		 \par Example:
		 \code
		 ae::Vector<float, 5> vec5f({ 0.5f, 0.3f, -1.f, 1.f, 10.f });
		 // or
		 ae::Vector<float, 5> vec5f = { 0.5f, 0.3f, -1.f, 1.f, 10.f };
		 \endcode

		 \since v0.6.0
		*/
		inline Vector(std::initializer_list<T> coordinates)
			: elements()
		{
			// Copy the maximum number of elements that both the vector and the list hold
			const size_t MIN_COUNT = Math::min(n, coordinates.size());
			std::memcpy(elements.data(), coordinates.begin(), MIN_COUNT * sizeof(T));
		}
		/*!
		 \brief Constructs the ae::Vector by providing an std::array of coordinates.
		 \details Sets the ae::Vector's elements to the std::array of \a coordinates provided.

		 \param[in] coordinates The C++11 array of coordinates to assign to the ae::Vector

		 \par Example:
		 \code
		 std::array<float, 5> COORDINATES = { 0.5f, 0.3f, -1.f, 1.f, 10.f };
		 ae::Vector<float, 5> VEC5F(COORDINATES);
		 \endcode

		 \since v0.6.0
		*/
		explicit inline _CONSTEXPR17 Vector(const std::array<T, n>& coordinates) noexcept
			: elements(coordinates)
		{
		}
		/*!
		 \brief Constructs the ae::Vector by providing a C-style array of coordinates.
		 \details Sets the ae::Vector's elements to the C-style array of \a coordinates provided.

		 \param[in] coordinates The C-style array of coordinates to assign to the ae::Vector

		 \par Example:
		 \code
		 float coordinates[5] = { 0.5f, 0.3f, -1.f, 1.f, 10.f };
		 ae::Vector<float, 5> vec5f(coordinates);
		 \endcode

		 \since v0.6.0
		*/
		explicit inline Vector(const T(&coordinates)[n])
			: elements()
		{
			// Copy the values of the C-style array to the vector's elements
			std::memcpy(elements.data(), coordinates, n * sizeof(T));
		}
		/*!
		 \brief Constructs the ae::Vector by providing an ae::Vector of another type and/or with a different number of elements.
		 \details Sets the ae::Vector's elements to the \a vecUN's elements, up to the maximum number of elements that both vectors hold.\n
		 If the \a vecUN possesses less elements, the ae::Vector's remaining elements will be set to 0.

		 \param[in] vecUN The ae::Vector of another type and/or with a different number of elements to assign to the ae::Vector

		 \par Example:
		 \code
		 // Different type, same number of elements
		 ae::Vector<int, 5> vec5i(5, 3, 8, 1, 0);
		 ae::Vector<float, 5> vec5f = vec5i;

		 // Same type, different number of elements
		 ae::Vector<float, 5> vec5f(5.f, 3.f, 2.f, 0.f, -1.f);
		 ae::Vector<float, 7> vec7f(vec5f); // last two elements set to 0.f

		 // Different type, different number of elements
		 ae::Vector<int, 5> vec5i(5, 3, 2, 0, 8);
		 ae::Vector<double, 7> vec7d(vec5i); // last two elements set to 0.0
		 \endcode

		 \since v0.6.0
		*/
		template <typename U, size_t n2>
		inline Vector(const Vector<U, n2>& vecUN)
			: elements()
		{
			// Avoid the casts if the T type is equal to the U type during compilation
			_CONSTEXPR17 const size_t MIN_N = Math::min(n, n2);
			if _CONSTEXPR_IF (std::is_same_v<T, U>) {
				std::memcpy(elements.data(), vecUN.elements.data(), MIN_N * sizeof(T));
			}
			else {
				for (size_t i = static_cast<size_t>(0); i < MIN_N; ++i) {
					elements[i] = static_cast<T>(vecUN.elements[i]);
				}
			}
		}
		/*!
		 \brief Copy constructor.
		 \details Sets the ae::Vector's elements to the \a copy's elements.

		 \param[in] copy The ae::Vector of the same type and of the same number of elements that will be copied

		 \par Example:
		 \code
		 ae::Vector<float, 5> vec5f_1(0.5f, 0.3f, -1.f, 1.f, 0.f);
		 ae::Vector<float, 5> vec5f_2(vec5f_1);
		 // or
		 ae::Vector<float, 5> vec5f_2 = vec5f_1;
		 \endcode

		 \since v0.6.0
		*/
		inline _CONSTEXPR17 Vector(const Vector<T, n>& copy) noexcept
			: elements(copy.elements)
		{
		}
		/*!
		 \brief Move constructor.
		 \details Performs a move operation on the \a rvalue's elements to the lvalue ae::Vector's elements.

		 \param[in] rvalue The ae::Vector rvalue that will be moved

		 \par Example:
		 \code
		 ae::Vector<float, 5> vec5f_1(0.5f, 0.3f, 0.8f, -1.f, 1.f);
		 ae::Vector<float, 5> vec5f_2(0.3f, 0.1f, 0.25f, 1.f, 0.f);
		 ae::Vector<float, 5> vec5f_3(vec5f_1 + vec5f_2);
		 // or
		 ae::Vector<float, 5> vec5f_3 = vec5f_1 + vec5f_2;
		 \endcode

		 \since v0.6.0
		*/
		inline _CONSTEXPR17 Vector(Vector<T, n>&& rvalue) noexcept
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
		 ae::Vector<float, 5> vec5f_1 = { 0.5f, 0.3f, 1.f, 0.f, 2.f };
		 ae::Vector<float, 5> vec5f_2 = { 0.3f, 0.1f, -1.f, 10.f, 1.f };
		 ...
		 vec5f_1 = vec5f_2;
		 \endcode

		 \since v0.6.0
		*/
		inline Vector<T, n>& operator=(const Vector<T, n>& other) noexcept
		{
			// Check that the caller object won't be assigned to itself (ignored in Release mode)
			if _CONSTEXPR_IF (AEON_DEBUG) {
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
		 ae::Vector<float, 5> vec5f_1 = { 0.5f, 0.3f, 1.f, 0.f, 2.f };
		 ae::Vector<float, 5> vec5f_2 = { 0.3f, 0.1f, -1.f, 10.f, 1.f };
		 ae::Vector<float, 5> vec5f_3 = { 0.5f, 0.8f, 1.f, 5.f, 0.2f };
		 ...
		 vec5f_3 = vec5f_1 + vec5f_2;
		 \endcode

		 \since v0.6.0
		*/
		inline Vector<T, n>& operator=(Vector<T, n>&& rvalue) noexcept
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
		 ae::Vector<float, 5> vec5f = { 0.5f, 0.3f, 0.f, -1.f, 1.f };
		 float& firstElement = vec5f[0]; // 0.5f
		 \endcode

		 \sa at()

		 \since v0.6.0
		*/
		_NODISCARD inline T& operator[](size_t index) noexcept
		{
			// Log an error message if the index isn't within the array's limits (ignored in Release mode)
			if _CONSTEXPR_IF (AEON_DEBUG) {
				if (index >= n) {
					AEON_LOG_ERROR("Invalid array index", "The index provided isn't situated within the array's limits.\nRetrieving the array's first element.");
					return elements[0];
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
		 ae::Vector<float, 5> vec5f = { 0.5f, 0.3f, 0.f, -1.f, 1.f };
		 const float& firstElement = vec5f[0]; // 0.5f
		 \endcode

		 \sa at()

		 \since v0.6.0
		*/
		_NODISCARD inline const T& operator[](size_t index) const noexcept
		{
			// Log an error message if the index isn't within the array's limits (ignored in Release mode)
			if _CONSTEXPR_IF (AEON_DEBUG) {
				if (index >= n) {
					AEON_LOG_ERROR("Invalid array index", "The index provided isn't situated within the array's limits.\nRetrieving the array's first element.");
					return elements[0];
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
		 ae::Vector<float, 5> vec5f(0.5f, 0.3f, 0.f, -1.f, 1.f);
		 float& firstElement = vec5f.at(0); // 0.5f
		 \endcode

		 \sa operator[]()

		 \since v0.6.0
		*/
		_NODISCARD inline T& at(size_t index) noexcept
		{
			// Log an error message if the index isn't within the array's limits
			if (index >= n) {
				AEON_LOG_ERROR("Invalid array index", "The index provided isn't situated within the array's limits.\nRetrieving the array's first element.");
				return elements[0];
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
		 ae::Vector<float, 5> vec5f(0.5f, 0.3f, 0.f, -1.f, 1.f);
		 const float& firstElement = vec5f.at(0); // 0.5f
		 \endcode

		 \sa operator[]()

		 \since v0.6.0
		*/
		_NODISCARD inline const T& at(size_t index) const noexcept
		{
			// Log an error message if the index isn't within the array's limits
			if (index >= n) {
				AEON_LOG_ERROR("Invalid array index", "The index provided isn't situated within the array's limits.\nRetrieving the array's first element.");
				return elements[0];
			}

			return elements.at(index);
		}

		/*!
		 \brief Calculates and retrieves the ae::Vector's magnitude (or length).

		 \return The ae::Vector's magnitude

		 \par Example:
		 \code
		 ae::Vector<float, 5> vec5f({ 0.5f, 0.2f, 0.8f, -1.f, 1.f });
		 float magnitude = vec5f.magnitude();
		 \endcode

		 \since v0.6.0
		*/
		_NODISCARD inline T magnitude() const
		{
			// Calculate the sum of the elements squared
			T sum = static_cast<T>(0);
			for (const T& element : elements) {
				sum += element * element;
			}

			// Return the square root of the sum
			return Math::sqrt(sum);
		}
		/*!
		 \brief Calculates and retrieves the unit vector of the ae::Vector.
		 \details A unit vector (or a normalized vector) is a vector with a magnitude of 1.
		 Unit vectors are used when we only need to represent a direction and not a magnitude.
		 \note Only floating point types are allowed (float, double, long double).

		 \return The ae::Vector's unit vector (or normalized vector)

		 \par Example:
		 \code
		 ae::Vector<float, 5> vec5f({ 0.5f, 0.2f, 0.8f, -1.f, 1.f });
		 ae::Vector<float, 5> unitVec5 = vec5f.normalize();
		 \endcode

		 \since v0.6.0
		*/
		template <typename = Math::FLOATING_POINT_POLICY<T>>
		_NODISCARD inline Vector<T, n> normalize() const
		{
			return *this * Math::rsqrt(dot(*this, *this));
		}
	};

	/*!
	 \brief The struct that represents the partially specialized templated used for a 2-dimensional mathematical vector of type T.
	 \details This struct can be used as a 2D point in space, as a simple 2D vector and a 2D directional vector, and as a container.
	 \note Only arithmetic types are allowed (float, int, etc.).
	*/
	template <typename T>
	struct Vector<T, 2>
	{
		// Member(s)
		union {
			std::array<T, 2> elements; //!< The array of elements
			struct {
				T            x;        //!< The x element
				T            y;        //!< The y element
			};
		};

		// Constructor(s)
		/*!
		 \brief Default constructor.
		 \details Sets the elements to the value 0 of the type provided.

		 \since v0.6.0
		*/
		template <typename = VECTOR_POLICY<T, 2>>
		inline _CONSTEXPR17 Vector() noexcept
			: elements()
		{
		}
		/*!
		 \brief Constructs the ae::Vector by providing a \a scalar value of the appropriate type.
		 \details Sets the elements to the value of the \a scalar.

		 \param[in] scalar The scalar value that will be attributed to the elements

		 \par Example:
		 \code
		 ae::Vector2f vec2f(5.f); // the elements x and y are set to 5.f
		 \endcode

		 \since v0.6.0
		*/
		explicit inline _CONSTEXPR17 Vector(T scalar) noexcept
			: x(scalar)
			, y(scalar)
		{
		}
		/*!
		 \brief Constructs the ae::Vector providing specific \a x and \a y elements.
		 \details Sets the ae::Vector's elements to the elements provided.

		 \param[in] x The element that will be attributed to the x element
		 \param[in] y The element that will be attributed to the y element

		 \par Example:
		 \code
		 ae::Vector2f vec2f(0.5f, 0.3f);
		 \endcode

		 \since v0.6.0
		*/
		inline _CONSTEXPR17 Vector(T x, T y) noexcept
			: x(x)
			, y(y)
		{
		}
		/*!
		 \brief Constructs the ae::Vector by providing specific \a coordinates.
		 \details Sets the ae::Vector's elements to the \a coordinates provided.
		 \note It's up to the user to provide the correct number of elements.

		 \param[in] coordinates The initializer list of elements

		 \par Example:
		 \code
		 ae::Vector2f vec2f({ 3.f, 2.f });
		 // or
		 ae::Vector2f vec2f = { 3.f, 2.f };
		 \endcode

		 \since v0.6.0
		*/
		inline Vector(std::initializer_list<T> coordinates) noexcept
			: elements()
		{
			// Copy the maximum number of elements that both the vector and the list hold
			const size_t MIN_COUNT = Math::min(elements.size(), coordinates.size());
			std::memcpy(elements.data(), coordinates.begin(), MIN_COUNT * sizeof(T));
		}
		/*!
		 \brief Constructs the ae::Vector by providing an std::array of coordinates.
		 \details Set the ae::Vector's elements to the std::array of \a coordinates provided.

		 \param[in] coordinates The C++11 array of coordinates to assign to the ae::Vector

		 \par Example:
		 \code
		 std::array<float, 2> coordinates = { 5.f, 8.f };
		 ae::Vector2f vec2f(coordinates);
		 \endcode

		 \since v0.6.0
		*/
		explicit inline _CONSTEXPR17 Vector(const std::array<T, 2>& coordinates) noexcept
			: elements(coordinates)
		{
		}
		/*!
		 \brief Constructs the ae::Vector by providing a C-style array of coordinates.
		 \details Sets the ae::Vector's elements to the C-style array of \a coordinates provided.

		 \param[in] coordinates The C-style array of coordinates to assign to the ae::Vector

		 \par Example:
		 \code
		 float coordinates[2] = { 2.f, 1.f };
		 ae::Vector2f vec2f(coordinates);
		 \endcode

		 \since v0.6.0
		*/
		explicit inline Vector(const T(&coordinates)[2]) noexcept
			: elements()
		{
			// Copy the values of the C-style array to the vector's elements
			std::memcpy(elements.data(), coordinates, 2 * sizeof(T));
		}
		/*!
		 \brief Constructs the ae::Vector by providing an ae::Vector of another type and/or with a different number of elements.
		 \details Sets the ae::Vector's elements to the \a vecUN's elements, up to 2 elements.

		 \param[in] vecUN The ae::Vector of another type and/or with a different number of elements to assign to the ae::Vector

		 \par Example:
		 \code
		 // Different type, same number of elements
		 ae::Vector2i vec2i(5, 3);
		 ae::Vector2f vec2f_1 = vec2i;

		 // Same type, different number of elements
		 ae::Vector3f vec3f(5.f, 3.f, 2.f);
		 ae::Vector2f vec2f_2(vec3f);

		 // Different type, different number of elements
		 ae::Vector3i vec3i(5, 3, 2);
		 ae::Vector2f vec2f_3 = vec3i;
		 \endcode

		 \since v0.6.0
		*/
		template <typename U, size_t n2>
		inline _CONSTEXPR17 Vector(const Vector<U, n2>& vecUN) noexcept
			: x(static_cast<T>(vecUN[0]))
			, y(static_cast<T>(vecUN[1]))
		{
		}
		/*!
		 \brief Copy constructor.
		 \details Sets the ae::Vector's elements to the \a copy's elements.

		 \param[in] copy The ae::Vector of the same type and of the same number of elements that will be copied

		 \par Example:
		 \code
		 ae::Vector2f vec2f_1(0.5f, 0.3f);
		 ae::Vector2f vec2f_2(vec2f_1);
		 // or
		 ae::Vector2f vec2f_2 = vec2f_1;
		 \endcode

		 \since v0.6.0
		*/
		inline _CONSTEXPR17 Vector(const Vector<T, 2>& copy) noexcept
			: elements(copy.elements)
		{
		}
		/*!
		 \brief Move constructor.
		 \details Performs a move operation on the \a rvalue's elements to the lvalue ae::Vector's elements.

		 \param[in] rvalue The ae::Vector rvalue that will be moved

		 \par Example:
		 \code
		 ae::Vector2f vec2f_1(0.5f, 0.3f);
		 ae::Vector2f vec2f_2(0.3f, 0.1f);
		 ae::Vector2f vec2f_3(vec2f_1 + vec2f_2);
		 // or
		 ae::Vector2f vec2f_3 = vec2f_1 + vec2f_2;
		 \endcode

		 \since v0.6.0
		*/
		inline _CONSTEXPR17 Vector(Vector<T, 2>&& rvalue) noexcept
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
		 ae::Vector2f vec2f_1(0.5f, 0.3f);
		 ae::Vector2f vec2f_2(0.3f, 0.1f);
		 ...
		 vec2f_1 = vec2f_2;
		 \endcode

		 \since v0.6.0
		*/
		inline Vector<T, 2>& operator=(const Vector<T, 2>& other) noexcept
		{
			// Check that the caller object won't be assigned to itself (ignored in Release mode)
			if _CONSTEXPR_IF (AEON_DEBUG) {
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
		 ae::Vector2f vec2f_1(0.5f, 0.3f);
		 ae::Vector2f vec2f_2(0.3f, 0.1f);
		 ae::Vector2f vec2f_3(0.5f, 0.8f);
		 ...
		 vec2f_3 = vec2f_1 + vec2f_2;
		 \endcode

		 \since v0.6.0
		*/
		inline Vector<T, 2>& operator=(Vector<T, 2>&& rvalue) noexcept
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
		 ae::Vector2f vec2f(0.5f, 0.3f);
		 float& firstElement = vec2f[0]; // 0.5f
		 \endcode

		 \sa at()

		 \since v0.6.0
		*/
		_NODISCARD inline _CONSTEXPR17 T& operator[](size_t index) noexcept
		{
			// Log an error message if the index isn't within the array's limits (ignored in Release mode)
			if _CONSTEXPR_IF(AEON_DEBUG) {
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
		 ae::Vector2f vec2f(0.5f, 0.3f);
		 const float& firstElement = vec2f[0]; // 0.5f
		 \endcode

		 \sa at()

		 \since v0.6.0
		*/
		_NODISCARD inline _CONSTEXPR17 const T& operator[](size_t index) const noexcept
		{
			// Log an error message if the index isn't within the array's limits (ignored in Release mode)
			if _CONSTEXPR_IF (AEON_DEBUG) {
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
		 ae::Vector2f vec2f(0.5f, 0.3f);
		 float& firstElement = vec2f.at(0); // 0.5f
		 \endcode

		 \sa operator[]()

		 \since v0.6.0
		*/
		_NODISCARD inline _CONSTEXPR17 T& at(size_t index) noexcept
		{
			// Log an error message if the index isn't within the array's limits
			if (index >= elements.size()) {
				AEON_LOG_ERROR("Invalid array index", "The index provided isn't situated within the array's limits.\nRetrieving the array's x element.");
				return x;
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
		 ae::Vector2f vec2f(0.5f, 0.3f);
		 const float& firstElement = vec2f.at(0); // 0.5f
		 \endcode

		 \sa operator[]()

		 \since v0.6.0
		*/
		_NODISCARD inline _CONSTEXPR17 const T& at(size_t index) const noexcept
		{
			// Log an error message if the index isn't within the array's limits
			if (index >= elements.size()) {
				AEON_LOG_ERROR("Invalid array index", "The index provided isn't situated within the array's limits.\nRetrieving the array's x element.");
				return x;
			}

			return elements.at(index);
		}

		/*!
		 \brief Calculates and retrieves the ae::Vector's magnitude (or length).
		 \details The magnitude is calculated using the Pythagorean theorem: \f$ \| \overrightarrow{V} \| = \sqrt{x^2 + y^2} \f$.

		 \return The ae::Vector's magnitude

		 \par Example:
		 \code
		 ae::Vector2f vec2f(2.5f, 5.2f);
		 float magnitude = vec2f.magnitude();
		 \endcode

		 \since v0.6.0
		*/
		_NODISCARD inline T magnitude() const
		{
			return Math::sqrt(x * x + y * y);
		}
		/*!
		 \brief Calculates and retrieves the unit vector of the ae::Vector.
		 \details A unit vector (or a normalized vector) is a vector with a magnitude of 1.
		 Unit vectors are used when we only need to represent a direction and not a magnitude.
		 \note Only floating point types are allowed (float, double, long double).

		 \return The ae::Vector's unit vector (or normalized vector)

		 \par Example:
		 \code
		 ae::Vector2f vec2f(0.5f, 0.2f);
		 ae::Vector2f unitVec2f = vec2f.normalize();
		 \endcode

		 \since v0.6.0
		*/
		template <typename = Math::FLOATING_POINT_POLICY<T>>
		_NODISCARD inline Vector<T, 2> normalize() const
		{
			return *this * Math::rsqrt(dot(*this, *this));
		}
	};

	/*!
	 \brief The struct that represents the partially specialized template used for a 3-dimensional mathematical vector of type T.
	 \details This struct can be used as a 3D point in space, as a simple 3D vector and a 3D directional vector, and as a container.
	 \note Only arithmetic types are allowed (float, int, etc.).
	*/
	template <typename T>
	struct Vector<T, 3>
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

		 \since v0.6.0
		*/
		inline _CONSTEXPR17 Vector() noexcept
			: elements()
		{
		}
		/*!
		 \brief Constructs the ae::Vector by providing a \a scalar value of the appropriate type.
		 \details Sets the elements to the value of the \a scalar.

		 \param[in] scalar The scalar value that will be attributed to the elements

		 \par Example:
		 \code
		 ae::Vector3f vec3f(5.f); // the elements x, y and z are set to 5.f
		 \endcode

		 \since v0.6.0
		*/
		explicit inline _CONSTEXPR17 Vector(T scalar) noexcept
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
		 ae::Vector3f vec3f(0.5f, 0.3f, 2.f);
		 \endcode

		 \since v0.6.0
		*/
		inline _CONSTEXPR17 Vector(T x, T y, T z) noexcept
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

		 \since v0.6.0
		*/
		inline Vector(std::initializer_list<T> coordinates) noexcept
			: elements()
		{
			// Copy the maximum number of elements that both the vector and the list hold
			const size_t MIN_COUNT = Math::min(elements.size(), coordinates.size());
			std::memcpy(elements.data(), coordinates.begin(), MIN_COUNT * sizeof(T));
		}
		/*!
		 \brief Constructs the ae::Vector by providing an std::array of coordinates.
		 \details Set the ae::Vector's elements to the std::array of \a coordinates provided.

		 \param[in] coordinates The C++11 array of coordinates to assign to the ae::Vector

		 \par Example:
		 \code
		 std::array<float, 3> coordinates = { 5.f, 8.f, 2.f };
		 ae::Vector3f vec3f(coordinates);
		 \endcode

		 \since v0.6.0
		*/
		explicit inline _CONSTEXPR17 Vector(const std::array<T, 3>& coordinates) noexcept
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

		 \since v0.6.0
		*/
		explicit inline Vector(const T(&coordinates)[3]) noexcept
			: elements()
		{
			// Copy the values of the C-style array to the vector's elements
			std::memcpy(elements.data(), coordinates, 3 * sizeof(T));
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

		 \since v0.6.0
		*/
		template <typename U, size_t n2>
		inline Vector(const Vector<U, n2>& vecUN) noexcept
			: elements()
		{
			const size_t MIN_N = Math::min(elements.size(), n2);

			// Avoid the casts if the T type is equal to the U type during compilation
			if _CONSTEXPR_IF (std::is_same_v<T, U>) {
				std::memcpy(elements.data(), vecUN.elements.data(), MIN_N * sizeof(T));
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

		 \since v0.6.0
		*/
		template <typename U>
		inline _CONSTEXPR17 Vector(const Vector<U, 2>& vecU, T z) noexcept
			: x(static_cast<T>(vecU.x))
			, y(static_cast<T>(vecU.y))
			, z(z)
		{
		}
		/*!
		 \brief Copy constructor.
		 \details Sets the ae::Vector's elements to the \a copy's elements.

		 \param[in] copy The ae::Vector of the same type and of the same number of elements that will be copied

		 \par Example:
		 \code
		 ae::Vector3f vec3f_1(0.5f, 0.3f, -1.f);
		 ae::Vector3f vec3f_2(vec3f_1);
		 // or
		 ae::Vector3f vec3f_2 = vec3f_1;
		 \endcode

		 \since v0.6.0
		*/
		inline _CONSTEXPR17 Vector(const Vector<T, 3>& copy)
			: elements(copy.elements)
		{
		}
		/*!
		 \brief Move constructor.
		 \details Performs a move operation on the \a rvalue's elements to the lvalue ae::Vector's elements.

		 \param[in] rvalue The ae::Vector rvalue that will be moved

		 \par Example:
		 \code
		 ae::Vector3f vec3f_1(0.5f, 0.3f, -1.f);
		 ae::Vector3f vec3f_2(0.3f, 0.1f, 0.f);
		 ae::Vector3f vec3f_3(vec3f_1 + vec3f_2);
		 // or
		 ae::Vector3f vec3f_3 = vec3f_1 + vec3f_2;
		 \endcode

		 \since v0.6.0
		*/
		inline _CONSTEXPR17 Vector(Vector<T, 3>&& rvalue) noexcept
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

		 \since v0.6.0
		*/
		inline Vector<T, 3>& operator=(const Vector<T, 3>& other)
		{
			// Check that the caller object won't be assigned to itself (ignored in Release mode)
			if _CONSTEXPR_IF(AEON_DEBUG) {
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

		 \since v0.6.0
		*/
		inline Vector<T, 3>& operator=(Vector<T, 3>&& rvalue) noexcept
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

		 \since v0.6.0
		*/
		_NODISCARD inline _CONSTEXPR17 T& operator[](size_t index) noexcept
		{
			// Log an error message if the index isn't within the array's limits (ignored in Release mode)
			if _CONSTEXPR_IF (AEON_DEBUG) {
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
		 ae::Vector3f vec3f(0.5f, 0.3f, -1.f);
		 const float& firstElement = vec3f[0]; // 0.5f
		 \endcode

		 \sa at()

		 \since v0.6.0
		*/
		_NODISCARD inline _CONSTEXPR17 const T& operator[](size_t index) const noexcept
		{
			// Log an error message if the index isn't within the array's limits (ignored in Release mode)
			if _CONSTEXPR_IF (AEON_DEBUG) {
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

		 \since v0.6.0
		*/
		_NODISCARD inline _CONSTEXPR17 T& at(size_t index) noexcept
		{
			// Log an error message if the index isn't within the array's limits
			if (index >= elements.size()) {
				AEON_LOG_ERROR("Invalid array index", "The index provided isn't situated within the array's limits.\nRetrieving the array's x element.");
				return x;
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
		 ae::Vector3f vec3f(0.5f, 0.3f, -1.f);
		 const float& firstElement = vec3f.at(0); // 0.5f
		 \endcode

		 \sa operator[]()

		 \since v0.6.0
		*/
		_NODISCARD inline _CONSTEXPR17 const T& at(size_t index) const noexcept
		{
			// Log an error message if the index isn't within the array's limits
			if (index >= elements.size()) {
				AEON_LOG_ERROR("Invalid array index", "The index provided isn't situated within the array's limits.\nRetrieving the array's x element.");
				return x;
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

		 \since v0.6.0
		*/
		_NODISCARD inline T magnitude() const
		{
			return Math::sqrt(x * x + y * y + z * z);
		}
		/*!
		 \brief Calculates and retrieves the unit vector of the ae::Vector.
		 \details A unit vector (or a normalized vector) is a vector with a magnitude of 1.
		 Unit vectors are used when we only need to represent a direction and not a magnitude.
		 \note Only floating point types are allowed (float, double, long double).

		 \return The ae::Vector's unit vector (or normalized vector)

		 \par Example:
		 \code
		 ae::Vector3f vec3f(2.5f, 5.2f, -1.f);
		 ae::Vector3f unitVec3f = vec3f.normalize();
		 \endcode

		 \since v0.6.0
		*/
		template <typename = Math::FLOATING_POINT_POLICY<T>>
		_NODISCARD inline Vector<T, 3> normalize() const
		{
			return *this * Math::rsqrt(dot(*this, *this));
		}
	};

	/*!
	 \brief The struct that represents the partially specialized template used for a 3-dimensional mathematical vector of type T with a homogeneous component.
	 \details This struct can be used as a way to pass 3D data to GLSL, as a simple 3D vector (w set to 1) and a 3D directional vector (w set to 0), and a a container.
	 \note Only arithmetic types are allowed (float, int, etc.).
	*/
	template <typename T>
	struct Vector<T, 4>
	{
		// Member data
		union {
			std::array<T, 4> elements; //!< The array of elements
			struct {
				T            x;        //!< The x element
				T            y;        //!< The y element
				T            z;        //!< The z element
				T            w;        //!< The w homogeneous element
			};
			Vector<T, 2>     xy;       //!< The x and y elements
			Vector<T, 3>     xyz;      //!< The x, y and z elements
		};

		// Constructor(s)
		/*!
		 \brief Default constructor.
		 \details Sets the elements to the value 0 of the type provided.

		 \since v0.6.0
		*/
		inline _CONSTEXPR17 Vector() noexcept
			: elements()
		{
		}
		/*!
		 \brief Constructs the ae::Vector by providing a \a scalar value of the appropriate type.
		 \details Sets the elements to the value of the \a scalar.

		 \param[in] scalar The scalar value that will be attributed to the elements

		 \par Example:
		 \code
		 ae::Vector4f vec4f(5.f); // the elements x, y, z and w are set to 5.f
		 \endcode

		 \since v0.6.0
		*/
		explicit inline _CONSTEXPR17 Vector(T scalar) noexcept
			: x(scalar)
			, y(scalar)
			, z(scalar)
			, w(scalar)
		{
		}
		/*!
		 \brief Constructs the ae::Vector providing specific \a x, \a y, \a z and \a w elements.
		 \details Sets the ae::Vector's elements to the elements provided.

		 \param[in] x The element that will be attributed to the x element
		 \param[in] y The element that will be attributed to the y element
		 \param[in] z The element that will be attributed to the z element
		 \param[in] w The element that will be attributed to the w element

		 \par Example:
		 \code
		 ae::Vector4f vec4f(0.5f, 0.3f, 2.f, 1.f);
		 \endcode

		 \since v0.6.0
		*/
		inline _CONSTEXPR17 Vector(T x, T y, T z, T w) noexcept
			: x(x)
			, y(y)
			, z(z)
			, w(w)
		{
		}
		/*!
		 \brief Constructs the ae::Vector by providing specific \a coordinates.
		 \details Sets the ae::Vector's elements to the \a coordinates provided.
		 \note It's up to the user to provide the correct number of elements.

		 \param[in] coordinates The initializer list of elements

		 \par Example:
		 \code
		 ae::Vector4f vec4f({ 3.f, 2.f, 5.f, 1.f });
		 // or
		 ae::Vector4f vec4f = { 3.f, 2.f, 5.f, 1.f };
		 \endcode

		 \since v0.6.0
		*/
		inline Vector(std::initializer_list<T> coordinates) noexcept
			: elements()
		{
			// Copy the maximum number of elements that both the vector and the list hold
			const size_t MIN_COUNT = Math::min(elements.size(), coordinates.size());
			std::memcpy(elements.data(), coordinates.begin(), MIN_COUNT * sizeof(T));
		}
		/*!
		 \brief Constructs the ae::Vector by providing an std::array of coordinates.
		 \details Set the ae::Vector's elements to the std::array of \a coordinates provided.

		 \param[in] coordinates The C++11 array of coordinates to assign to the ae::Vector

		 \par Example:
		 \code
		 std::array<float, 4> coordinates = { 5.f, 8.f, 2.f, 1.f };
		 ae::Vector4f vec4f(coordinates);
		 \endcode

		 \since v0.6.0
		*/
		explicit inline _CONSTEXPR17 Vector(const std::array<T, 4>& coordinates) noexcept
			: elements(coordinates)
		{
		}
		/*!
		 \brief Constructs the ae::Vector by providing a C-style array of coordinates.
		 \details Sets the ae::Vector's elements to the C-style array of \a coordinates provided.

		 \param[in] coordinates The C-style array of coordinates to assign to the ae::Vector

		 \par Example:
		 \code
		 float coordinates[4] = { 2.f, 1.f, -4.f, 1.f };
		 ae::Vector4f vec4f(coordinates);
		 \endcode

		 \since v0.6.0
		*/
		explicit inline Vector(const T(&coordinates)[4]) noexcept
			: elements()
		{
			// Copy the values of the C-style array to the vector's elements
			std::memcpy(elements.data(), coordinates, 4 * sizeof(T));
		}
		/*!
		 \brief Constructs the ae::Vector by providing an ae::Vector of another type and/or with a different number of elements.
		 \details Sets the ae::Vector's elements to the \a vecUN's elements, up to 4 elements.

		 \param[in] vecUN The ae::Vector of another type and/or with a different number of elements to assign to the ae::Vector

		 \par Example:
		 \code
		 // Different type, same number of elements
		 ae::Vector4i vec4i(5, 3, -2, 1);
		 ae::Vector4f vec4f_1 = vec3i;

		 // Same type, different number of elements
		 ae::Vector3f vec3f(5.f, 3.f, 2.f);
		 ae::Vector4f vec4f_2(vec3f);

		 // Different type, different number of elements
		 ae::Vector2i vec2i(5, 3);
		 ae::Vector4f vec4f_3 = vec2i;
		 \endcode

		 \since v0.6.0
		*/
		template <typename U, size_t n2>
		inline Vector(const Vector<U, n2>& vecUN) noexcept
			: elements()
		{
			const size_t MIN_N = Math::min(elements.size(), n2);

			// Avoid the casts and the loop if the T type is equal to the U type during compilation
			if _CONSTEXPR_IF(std::is_same_v<T, U>) {
				std::memcpy(elements.data(), vecUN.elements.data(), MIN_N * sizeof(T));
			}
			else {
				for (size_t i = 0; i < MIN_N; ++i) {
					elements[i] = static_cast<T>(vecUN.elements[i]);
				}
			}
		}
		/*!
		 \brief Constructs the ae::Vector by providing a 2-dimensional ae::Vector, a \a z element and a \a w element.
		 \details The \a vecU can be of the same type as the ae::Vector to be constructed or of a different type.
		 \note The cast condition won't have any impact on performance during runtime.

		 \param[in] vecU The 2-dimensional ae::Vector to assign to the ae::Vector
		 \param[in] z The element that will be attributed to the z element
		 \param[in] w The element that will be attributed to the w element

		 \par Example:
		 \code
		 // Same type
		 ae::Vector2f vec2f_1(5.f, 3.f);
		 ae::Vector4f vec4f_1(vec2f_1, -1.f, 1.f);

		 // Different type
		 ae::Vector2i ve2i_1(5, 3);
		 ae::Vector4f vec4f_2(vec2i_1, -1.f, 1.f);
		 \endcode

		 \since v0.6.0
		*/
		template <typename U>
		inline _CONSTEXPR17 Vector(const Vector<U, 2>& vecU, T z, T w) noexcept
			: x(static_cast<T>(vecU.x))
			, y(static_cast<T>(vecU.y))
			, z(z)
			, w(w)
		{
		}
		/*!
		 \brief Constructs the ae::Vector by providing a 3-dimensional ae::Vector and a \a w element.
		 \details The \a vecU can be of the same type as the ae::Vector to be constructed or of a different type.
		 \note The cast condition won't have any impact on performance during runtime.

		 \param[in] vecU The 3-dimensional ae::Vector to assign to the ae::Vector
		 \param[in] w The element that will be attributed to the w element

		 \par Example:
		 \code
		 // Same type
		 ae::Vector3f vec3f_1(5.f, 3.f, -1.f);
		 ae::Vector4f vec4f_1(vec3f_1, 1.f);

		 // Different type
		 ae::Vector3i ve2i_1(5, 3, -1);
		 ae::Vector4f vec4f_2(vec3i_1, 1.f);
		 \endcode

		 \since v0.6.0
		*/
		template <typename U>
		inline _CONSTEXPR17 Vector(const Vector<U, 3>& vecU, T w) noexcept
			: x(static_cast<T>(vecU.x))
			, y(static_cast<T>(vecU.y))
			, z(static_cast<T>(vecU.z))
			, w(w)
		{
		}
		/*!
		 \brief Copy constructor.
		 \details Sets the ae::Vector's elements to the \a copy's elements.

		 \param[in] copy The ae::Vector of the same type and of the same number of elements that will be copied

		 \par Example:
		 \code
		 ae::Vector4f vec4f_1(0.5f, 0.3f, -1.f, 1.f);
		 ae::Vector4f vec4f_2(vec4f_1);
		 // or
		 ae::Vector4f vec4f_2 = vec4f_1;
		 \endcode

		 \since v0.6.0
		*/
		inline _CONSTEXPR17 Vector(const Vector<T, 4>& copy)
			: elements(copy.elements)
		{
		}
		/*!
		 \brief Move constructor.
		 \details Performs a move operation on the \a rvalue's elements to the lvalue ae::Vector's elements.

		 \param[in] rvalue The ae::Vector rvalue that will be moved

		 \par Example:
		 \code
		 constexpr ae::Vector4f vec4f_1(0.5f, 0.3f, -1.f, 1.f);
		 constexpr ae::Vector4f vec4f_2(0.3f, 0.1f, 0.f, 1.f);
		 ae::Vector4f vec4f_3(vec4f_1 + vec4f_2);
		 // or
		 ae::Vector4f vec4f_3 = vec4f_1 + vec4f_2;
		 \endcode

		 \since v0.6.0
		*/
		inline _CONSTEXPR17 Vector(Vector<T, 4>&& rvalue) noexcept
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
		 ae::Vector4f vec4f_1(0.5f, 0.3f, -1.f, 1.f);
		 ae::Vector4f vec4f_2(0.3f, 0.1f, 1.f, 1.f);
		 ...
		 vec4f_1 = vec4f_2;
		 \endcode

		 \since v0.6.0
		*/
		inline Vector<T, 4>& operator=(const Vector<T, 4>& other)
		{
			// Check that the caller object won't be assigned to itself (ignored in Release mode)
			if _CONSTEXPR_IF(AEON_DEBUG) {
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
		 ae::Vector4f vec4f_1(0.5f, 0.3f, -1.f, 1.f);
		 ae::Vector4f vec4f_2(0.3f, 0.1f, 1.f, 0.f);
		 ae::Vector4f vec4f_3(0.5f, 0.8f, -1.f, 1.f);
		 ...
		 vec4f_3 = vec4f_1 + vec4f_2;
		 \endcode

		 \since v0.6.0
		*/
		inline Vector<T, 4>& operator=(Vector<T, 4>&& rvalue) noexcept
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
		 ae::Vector4f vec4f(0.5f, 0.3f, -1.f, 1.f);
		 float& firstElement = vec4f[0]; // 0.5f
		 \endcode

		 \sa at()

		 \since v0.6.0
		*/
		_NODISCARD inline _CONSTEXPR17 T& operator[](size_t index) noexcept
		{
			// Log an error message if the index isn't within the array's limits (ignored in Release mode)
			if _CONSTEXPR_IF(AEON_DEBUG) {
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
		 ae::Vector4f vec4f(0.5f, 0.3f, -1.f, 1.f);
		 const float& firstElement = vec4f[0]; // 0.5f
		 \endcode

		 \sa at()

		 \since v0.6.0
		*/
		_NODISCARD inline _CONSTEXPR17 const T& operator[](size_t index) const noexcept
		{
			// Log an error message if the index isn't within the array's limits (ignored in Release mode)
			if _CONSTEXPR_IF(AEON_DEBUG) {
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
		 ae::Vector4f vec4f(0.5f, 0.3f, -1.f, 1.f);
		 float& firstElement = vec4f.at(0); // 0.5f
		 \endcode

		 \sa operator[]()

		 \since v0.6.0
		*/
		_NODISCARD inline _CONSTEXPR17 T& at(size_t index) noexcept
		{
			// Log an error message if the index isn't within the array's limits
			if (index >= elements.size()) {
				AEON_LOG_ERROR("Invalid array index", "The index provided isn't situated within the array's limits.\nRetrieving the array's x element.");
				return x;
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
		 ae::Vector4f vec4f(0.5f, 0.3f, -1.f, 1.f);
		 const float& firstElement = vec4f.at(0); // 0.5f
		 \endcode

		 \sa operator[]()

		 \since v0.6.0
		*/
		_NODISCARD inline _CONSTEXPR17 const T& at(size_t index) const noexcept
		{
			// Log an error message if the index isn't within the array's limits
			if (index >= elements.size()) {
				AEON_LOG_ERROR("Invalid array index", "The index provided isn't situated within the array's limits.\nRetrieving the array's x element.");
				return x;
			}

			return elements.at(index);
		}

		/*!
		 \brief Calculates and retrieves the ae::Vector's magnitude (or length).
		 \details The magnitude is calculated using the Pythagorean theorem: \f$ \| \overrightarrow{V} \| = \sqrt{x^2 + y^2 + z^2 + w^2} \f$.

		 \return The ae::Vector's magnitude

		 \par Example:
		 \code
		 ae::Vector4f vec4f(0.5f, 0.2f, -1.f, 1.f);
		 float magnitude = vec4f.magnitude();
		 \endcode

		 \since v0.6.0
		*/
		_NODISCARD inline T magnitude() const
		{
			return Math::sqrt(x * x + y * y + z * z + w * w);
		}
		/*!
		 \brief Calculates and retrieves the unit vector of the ae::Vector.
		 \details A unit vector (or a normalized vector) is a vector with a magnitude of 1.
		 Unit vectors are used when we only need to represent a direction and not a magnitude.
		 \note Only floating point types are allowed (float, double, long double).

		 \return The ae::Vector's unit vector (or normalized vector)

		 \par Example:
		 \code
		 ae::Vector4f vec4f(2.5f, 5.2f, -1.f, 1.f);
		 ae::Vector4f unitVec4f = vec4f.normalize();
		 \endcode

		 \since v0.6.0
		*/
		template <typename = Math::FLOATING_POINT_POLICY<T>>
		_NODISCARD inline Vector<T, 4> normalize() const
		{
			return *this * Math::rsqrt(dot(*this, *this));
		}
	};

	// Vector<T, n> function(s)
	/*!
	 \relates Vector
	 \brief Addition operator.
	 \details Performs a memberwise addition of the elements of \a lhs and \a rhs.

	 \param[in] lhs The ae::Vector on the left side of the + operator
	 \param[in] rhs The ae::Vector on the right side of the + operator

	 \return An ae::Vector containing the sum of the elements of \a lhs and \a rhs

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1({ 0.5f, 0.3f });
	 ae::Vector2f vec2f_2({ 0.2f, 0.7f });
	 ae::Vector2f vec2f_3 = vec2f_1 + vec2f_2;
	 \endcode

	 \sa operator-(const Vector<T, n>&, const Vector<T, n>&), operator*(const Vector<T, n>&, const Vector<T, n>&), operator/(const Vector<T, n>&, const Vector<T, n>&)

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline Vector<T, n> operator+(const Vector<T, n>& lhs, const Vector<T, n>& rhs) noexcept
	{
		// Assign the sum of the two vectors to result
		Vector<T, n> result = lhs;
		for (size_t i = 0; i < n; ++i) {
			result.elements[i] += rhs.elements[i];
		}

		return result;
	}

	/*!
	 \relates Vector
	 \brief Subtraction operator.
	 \details Performs a memberwise subtraction of the elements of \a lhs and \a rhs.

	 \param[in] lhs The ae::Vector on the left side of the - operator
	 \param[in] rhs The ae::Vector on the right side of the - operator

	 \return An ae::Vector containing the difference of the elements of \a lhs and \a rhs

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1({ 0.5f, 0.3f });
	 ae::Vector2f vec2f_2({ 0.2f, 0.7f });
	 ae::Vector2f vec2f_3 = vec2f_1 - vec2f_2;
	 \endcode

	 \sa operator+(const Vector<T, n>&, const Vector<T, n>&), operator*(const Vector<T, n>&, const Vector<T, n>&), operator/(const Vector<T, n>&, const Vector<T, n>&)

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline Vector<T, n> operator-(const Vector<T, n>& lhs, const Vector<T, n>& rhs) noexcept
	{
		// Assign the difference of the two vectors to result
		Vector<T, n> result = lhs;
		for (size_t i = 0; i < n; ++i) {
			result.elements[i] -= rhs.elements[i];
		}

		return result;
	}
	/*!
	 \relates Vector
	 \brief Multiplication operator.
	 \details Performs a memberwise multiplication of the elements of \a lhs and \a rhs.

	 \param[in] lhs The ae::Vector on the left side of the * operator
	 \param[in] rhs The ae::Vector on the right side of the * operator

	 \return An ae::Vector containing the product of the elements of \a lhs and \a rhs

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1({ 0.5f, 0.3f });
	 ae::Vector2f vec2f_2({ 0.2f, 0.7f });
	 ae::Vector2f vec2f_3 = vec2f_1 * vec2f_2;
	 \endcode

	 \sa operator+(const Vector<T, n>&, const Vector<T, n>&), operator-(const Vector<T, n>&, const Vector<T, n>&), operator/(const Vector<T, n>&, const Vector<T, n>&)

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline Vector<T, n> operator*(const Vector<T, n>& lhs, const Vector<T, n>& rhs) noexcept
	{
		// Assign the product of the two vectors to result
		Vector<T, n> result = lhs;
		for (size_t i = 0; i < n; ++i) {
			result.elements[i] *= rhs.elements[i];
		}

		return result;
	}
	/*!
	 \relates Vector
	 \brief Division operator.
	 \details Performs a memberwise division of the elements of \a lhs and \a rhs.
	 \note Division by zero isn't handled in Release mode and results in undefined behaviour based on the type.

	 \param[in] lhs The ae::Vector on the left side of the / operator
	 \param[in] rhs The ae::Vector on the right side of the / operator

	 \return An ae::Vector containing the quotient of the elements of \a lhs and \a rhs

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1({ 0.5f, 0.3f });
	 ae::Vector2f vec2f_2({ 0.2f, 0.7f });
	 ae::Vector2f vec2f_3 = vec2f_1 / vec2f_2;
	 \endcode

	 \sa operator+(const Vector<T, n>&, const Vector<T, n>&), operator-(const Vector<T, n>&, const Vector<T, n>&), operator*(const Vector<T, n>&, const Vector<T, n>&)

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline Vector<T, n> operator/(const Vector<T, n>& lhs, const Vector<T, n>& rhs)
	{
		// Check if rhs's elements are equal to 0 (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			for (const T& element : rhs.elements) {
				if (element == static_cast<T>(0)) {
					AEON_LOG_ERROR("Division by zero", "Attempt to divide vector by 0\nReturning left-hand side vector.");
					return lhs;
				}
			}
		}

		// Assign the quotient of the two vectors to result
		Vector<T, n> result = lhs;
		for (size_t i = 0; i < n; ++i) {
			result.elements[i] /= rhs.elements[i];
		}

		return result;
	}

	/*!
	 \relates Vector
	 \brief Addition operator.
	 \details Performs a memberwise addition of the \a vec's elements and of the \a scalar.

	 \param[in] vec The ae::Vector of which its elements will be added with the \a scalar value
	 \param[in] scalar The value that will be added with the \a vec's elements

	 \return An ae::Vector containing the sum of the \a vec's elements and of the \a scalar

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1 = { 0.5f, 0.7f };
	 ae::Vector2f vec2f_2 = vec2f_1 + 0.2f;
	 \endcode

	 \sa operator-(const Vector<T, n>&, T), operator*(const Vector<T, n>&, T), operator/(const Vector<T, n>&, T)

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline Vector<T, n> operator+(const Vector<T, n>& vec, T scalar) noexcept
	{
		// Assign the sum of the vector and of the scalar to result
		Vector<T, n> result = vec;
		for (T& element : result.elements) {
			element += scalar;
		}

		return result;
	}

	/*!
	 \relates Vector
	 \brief Subtraction operator.
	 \details Performs a memberwise subtraction of the \a vec's elements and of the \a scalar.

	 \param[in] vec The ae::Vector of which its elements will be subtracted from the \a scalar value
	 \param[in] scalar The value that will subtract the \a vec's elements

	 \return An ae::Vector containing the difference of the \a vec's elements and of the \a scalar

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1 = { 0.5f, 0.7f };
	 ae::Vector2f vec2f_2 = vec2f_1 - 0.2f;
	 \endcode

	 \sa operator+(const Vector<T, n>&, T), operator*(const Vector<T, n>&, T), operator/(const Vector<T, n>&, T)

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline Vector<T, n> operator-(const Vector<T, n>& vec, T scalar) noexcept
	{
		// Assign the difference of the vector and of the scalar to result
		Vector<T, n> result = vec;
		for (T& element : result.elements) {
			element -= scalar;
		}

		return result;
	}
	/*!
	 \relates Vector
	 \brief Multiplication operator.
	 \details Performs a memberwise multiplication of the \a vec's elements and of the \a scalar.

	 \param[in] vec The ae::Vector of which its elements will be mulitplied with the \a scalar value
	 \param[in] scalar The value that will be multiplied with the \a vec's elements

	 \return An ae::Vector containing the product of the \a vec's elements and of the \a scalar

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1 = { 0.5f, 0.7f };
	 ae::Vector2f vec2f_2 = vec2f_1 * 0.2f;
	 \endcode

	 \sa operator+(const Vector<T, n>&, T), operator-(const Vector<T, n>&, T), operator/(const Vector<T, n>&, T)

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline Vector<T, n> operator*(const Vector<T, n>& vec, T scalar) noexcept
	{
		// Assign the product of the vector and of the scalar to result
		Vector<T, n> result = vec;
		for (T& element : result.elements) {
			element *= scalar;
		}

		return result;
	}
	/*!
	 \relates Vector
	 \brief Division operator.
	 \details Performs a memberwise division of the \a vec's elements and of the \a scalar.
	 \note Division by zero isn't handled in Release mode and results in undefined behaviour based on the type.

	 \param[in] vec The ae::Vector of which its elements will be divided by the \a scalar value
	 \param[in] scalar The value that will be the divisor of the \a vec's elements

	 \return An ae::Vector containing the quotient of the \a vec's elements and of the \a scalar

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1 = { 0.5f, 0.7f };
	 ae::Vector2f vec2f_2 = vec2f_1 / 0.2f;
	 \endcode

	 \sa operator+(const Vector<T, n>&, T), operator-(const Vector<T, n>&, T), operator*(const Vector<T, n>&, T)

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline Vector<T, n> operator/(const Vector<T, n>& vec, T scalar)
	{
		// Check if scalar is equal to 0 (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (scalar == static_cast<T>(0)) {
				AEON_LOG_ERROR("Division by zero", "Attempt to divide vector by 0\nReturning vector.");
				return vec;
			}
		}
		
		// Assign the quotient of the vector and of the scalar to result
		Vector<T, n> result = vec;
		for (T& element : result.elements) {
			element /= scalar;
		}

		return result;
	}

	/*!
	 \relates Vector
	 \brief Addition and assignment operator.
	 \details Performs a memberwise addition of the elements of \a lhs and \a rhs, and assigns the sum to \a lhs.

	 \param[in,out] lhs The ae::Vector on the left side of the += operator
	 \param[in] rhs The ae::Vector on the right side of the += operator

	 \return The ae::Vector \a lhs containing the sum of the addition, these calls can be chained together one after the other

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1 = { 0.2f, 0.5f };
	 ae::Vector2f vec2f_2 = { 0.3f, 0.1f };
	 ...
	 vec2f_1 += vec2f_2;
	 \endcode

	 \sa operator-=(Vector<T, n>&, const Vector<T, n>&), operator*=(Vector<T, n>&, const Vector<T, n>&), operator/=(Vector<T, n>&, const Vector<T, n>&)

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	inline Vector<T, n>& operator+=(Vector<T, n>& lhs, const Vector<T, n>& rhs) noexcept
	{
		// Assign the sum of the two vectors to lhs
		for (size_t i = 0; i < n; ++i) {
			lhs.elements[i] += rhs.elements[i];
		}

		return lhs;
	}
	/*!
	 \relates Vector
	 \brief Subtraction and assignment operator.
	 \details Performs a memberwise subtraction of the elements of \a lhs and \a rhs, and assigns the difference to \a lhs.

	 \param[in,out] lhs The ae::Vector on the left side of the -= operator
	 \param[in] rhs The ae::Vector on the right side of the -= operator

	 \return The ae::Vector \a lhs containing the difference of the subtraction, these calls can be chained together one after the other

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1 = { 0.2f, 0.5f };
	 ae::Vector2f vec2f_2 = { 0.3f, 0.1f };
	 ...
	 vec2f_1 -= vec2f_2;
	 \endcode

	 \sa operator+=(Vector<T, n>&, const Vector<T, n>&), operator*=(Vector<T, n>&, const Vector<T, n>&), operator/=(Vector<T, n>&, const Vector<T, n>&)

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	inline Vector<T, n>& operator-=(Vector<T, n>& lhs, const Vector<T, n>& rhs) noexcept
	{
		// Assign the difference of the two vectors to lhs
		for (size_t i = 0; i < n; ++i) {
			lhs.elements[i] -= rhs.elements[i];
		}

		return lhs;
	}
	/*!
	 \relates Vector
	 \brief Multiplication and assignment operator.
	 \details Performs a memberwise multiplication of the elements of \a lhs and \a rhs, and assigns the product to \a lhs.

	 \param[in,out] lhs The ae::Vector on the left side of the *= operator
	 \param[in] rhs The ae::Vector on the right side of the *= operator

	 \return The ae::Vector \a lhs containing the product of the multiplication, these calls can be chained together one after the other

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1 = { 0.2f, 0.5f };
	 ae::Vector2f vec2f_2 = { 0.3f, 0.1f };
	 ...
	 vec2f_1 *= vec2f_2;
	 \endcode

	 \sa operator+=(Vector<T, n>&, const Vector<T, n>&), operator-=(Vector<T, n>&, const Vector<T, n>&), operator/=(Vector<T, n>&, const Vector<T, n>&)

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	inline Vector<T, n>& operator*=(Vector<T, n>& lhs, const Vector<T, n>& rhs) noexcept
	{
		// Assign the product of the two vectors to lhs
		for (size_t i = 0; i < n; ++i) {
			lhs.elements[i] *= rhs.elements[i];
		}

		return lhs;
	}
	/*!
	 \relates Vector
	 \brief Division and assignment operator.
	 \details Performs a memberwise division of the elements of \a lhs and \a rhs, and assigns the quotient to \a lhs.
	 \note Division by zero isn't handled in Release mode and results in undefined behaviour based on the type.

	 \param[in,out] lhs The ae::Vector on the left side of the /= operator
	 \param[in] rhs The ae::Vector on the right side of the /= operator

	 \return The ae::Vector \a lhs containing the quotient of the division, these calls can be chained together one after the other

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1 = { 0.2f, 0.5f };
	 ae::Vector2f vec2f_2 = { 0.3f, 0.1f };
	 ...
	 vec2f_1 /= vec2f_2;
	 \endcode

	 \sa operator+=(Vector<T, n>&, const Vector<T, n>&), operator-=(Vector<T, n>&, const Vector<T, n>&), operator*=(Vector<T, n>&, const Vector<T, n>&)

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	inline Vector<T, n>& operator/=(Vector<T, n>& lhs, const Vector<T, n>& rhs)
	{
		// Check if rhs's elements are equal to 0 (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			for (const T& element : rhs.elements) {
				if (element == static_cast<T>(0)) {
					AEON_LOG_ERROR("Division by zero", "Attempt to divide vector by 0\nReturning left-hand side vector.");
					return lhs;
				}
			}
		}

		// Assign the quotient of the two vectors to lhs
		for (size_t i = 0; i < n; ++i) {
			lhs.elements[i] /= rhs.elements[i];
		}

		return lhs;
	}

	/*!
	 \relates Vector
	 \brief Addition and assignment operator.
	 \details Performs a memberwise addition of the \a vec's elements and of the \a scalar value, and assigns the sum to \a vec.

	 \param[in,out] vec The ae::Vector of which its elements will be added with the \a scalar value
	 \param[in] scalar The value that will be added with the \a vec's elements

	 \return The ae::Vector \a vec containing the sum of the addition, these calls can be chained together one after the other

	 \par Example:
	 \code
	 ae::Vector2f vec2f = { 0.3f, 0.5f };
	 ...
	 vec2f += 0.2f;
	 \endcode

	 \sa operator-=(Vector<T, n>&, T), operator*=(Vector<T, n>&, T), operator/=(Vector<T, n>&, T)

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	inline Vector<T, n>& operator+=(Vector<T, n>& vec, T scalar) noexcept
	{
		// Add the scalar value to the vec's elements
		for (T& element : vec.elements) {
			element += scalar;
		}

		return vec;
	}
	/*!
	 \relates Vector
	 \brief Subtraction and assignment operator.
	 \details Performs a memberwise subtraction of the \a vec's elements and of the \a scalar value, and assigns the difference to \a vec.

	 \param[in,out] vec The ae::Vector of which its elements will be subtracted by the \a scalar value
	 \param[in] scalar The value that will subtract the \a vec's elements

	 \return The ae::Vector \a vec containing the difference of the subtraction, these calls can be chained together one after the other

	 \par Example:
	 \code
	 ae::Vector2f vec2f = { 0.3f, 0.5f };
	 ...
	 vec2f -= 0.2f;
	 \endcode

	 \sa operator+=(Vector<T, n>&, T), operator*=(Vector<T, n>&, T), operator/=(Vector<T, n>&, T)

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	inline Vector<T, n>& operator-=(Vector<T, n>& vec, T scalar) noexcept
	{
		// Subtract the scalar value from the vec's elements
		for (T& element : vec.elements) {
			element -= scalar;
		}

		return vec;
	}
	/*!
	 \relates Vector
	 \brief Multiplication and assignment operator.
	 \details Performs a memberwise multiplication of the \a vec's elements and of the \a scalar value, and assigns the product to \a vec.

	 \param[in,out] vec The ae::Vector of which its elements will be multiplied with the \a scalar value
	 \param[in] scalar The value that will be multiplied with the \a vec's elements

	 \return The ae::Vector \a vec containing the product of the multiplication, these calls can be chained together one after the other

	 \par Example:
	 \code
	 ae::Vector2f vec2f = { 0.3f, 0.5f };
	 ...
	 vec2f *= 0.2f;
	 \endcode

	 \sa operator+=(Vector<T, n>&, T), operator-=(Vector<T, n>&, T), operator/=(Vector<T, n>&, T)

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	inline Vector<T, n>& operator*=(Vector<T, n>& vec, T scalar) noexcept
	{
		// Multiply the scalar value with the vec's elements
		for (T& element : vec.elements) {
			element *= scalar;
		}

		return vec;
	}
	/*!
	 \relates Vector
	 \brief Division and assignment operator.
	 \details Performs a memberwise division of the \a vec's elements and of the \a scalar value, and assigns the quotient to \a vec.
	 \note Division by zero isn't handled in Release mode and results in undefined behaviour based on the type.

	 \param[in,out] vec The ae::Vector of which its elements will be divided by the \a scalar value
	 \param[in] scalar The value that will be the divisor of the \a vec's elements

	 \return The ae::Vector \a vec containing the quotient of the division, these calls can be chained together one after the other

	 \par Example:
	 \code
	 ae::Vector2f vec2f = { 0.3f, 0.5f };
	 ...
	 vec2f /= 0.2f;
	 \endcode

	 \sa operator+=(Vector<T, n>&, T), operator-=(Vector<T, n>&, T), operator*=(Vector<T, n>&, T)

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	inline Vector<T, n>& operator/=(Vector<T, n>& vec, T scalar)
	{
		// Check if scalar is equal to 0 (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (scalar == static_cast<T>(0)) {
				AEON_LOG_ERROR("Division by zero", "Attempt to divide vector by 0\nReturning vector.");
				return vec;
			}
		}
		
		// Divide the vec's elements by the scalar value
		for (T& element : vec.elements) {
			element /= scalar;
		}

		return vec;
	}

	/*!
	 \relates Vector
	 \brief Equality operator.
	 \details Checks if the respective elements of \a lhs and \a rhs are equal.

	 \param[in] lhs The ae::Vector on the left side of the == operator
	 \param[in] rhs The ae::Vector on the right side of the == operator

	 \return True if all of the \a lhs's elements are respectively equal to the \a rhs's ones, false otherwise

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1(0.3f, 0.5f);
	 ae::Vector2f vec2f_2(0.3f, 0.5f);
	 if (vec2f_1 == vec2f_2) {
		...
	 }
	 \endcode

	 \sa operator!=()

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline bool operator==(const Vector<T, n>& lhs, const Vector<T, n>& rhs) noexcept
	{
		return std::equal(lhs.elements.begin(), lhs.elements.end(), rhs.elements.begin(),
			[](T lhsElement, T rhsElement) -> bool { return lhsElement == rhsElement; }
		);
	}
	/*!
	 \relates Vector
	 \brief Inequality operator.
	 \details Checks if the respective elements of \a lhs and \a rhs are inequal.

	 \param[in] lhs The ae::Vector on the left side of the != operator
	 \param[in] rhs The ae::Vector on the right side of the != operator

	 \return True if at least one of the elements of \a lhs and \a rhs is respectively inequal, false otherwise

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1(0.3f, 0.5f);
	 ae::Vector2f vec2f_2(0.3f, 0.4f);
	 if (vec2f_1 != vec2f_2) {
		...
	 }
	 \endcode

	 \sa operator==()

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline bool operator!=(const Vector<T, n>& lhs, const Vector<T, n>& rhs) noexcept
	{
		return !(lhs == rhs);
	}
	/*!
	 \relates Vector
	 \brief Less than operator.
	 \details Checks if the \a lhs's elements are less than the \a rhs's ones.

	 \param[in] lhs The ae::Vector on the left side of the < operator
	 \param[in] rhs The ae::Vector on the right side of the < operator

	 \return True if all of the \a lhs's elements are less than the \a rhs's ones, false otherwise

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1 = { 0.1f, 0.18f };
	 ae::Vector2f vec2f_2 = { 0.3f, 0.2f };
	 if (vec2f_1 < vec2f_2) {
		...
	 }
	 \endcode

	 \sa operator<=(), operator>(), operator>=()

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline bool operator<(const Vector<T, n>& lhs, const Vector<T, n>& rhs) noexcept
	{
		return std::equal(lhs.elements.begin(), lhs.elements.end(), rhs.elements.begin(),
			[](T lhsElement, T rhsElement) -> bool { return lhsElement < rhsElement; }
		);
	}
	/*!
	 \relates Vector
	 \brief Less than or equal to operator.
	 \details Checks if the \a lhs's elements are less than or equal to the \a rhs's ones.

	 \param[in] lhs The ae::Vector on the left side of the <= operator
	 \param[in] rhs The ae::Vector on the right side of the <= operator

	 \return True if all of the \a lhs's elements are less than or equal to the \a rhs's ones, false otherwise

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1 = { 0.1f, 0.2f };
	 ae::Vector2f vec2f_2 = { 0.3f, 0.2f };
	 if (vec2f_1 <= vec2f_2) {
		...
	 }
	 \endcode

	 \sa operator<(), operator>(), operator>=()

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline bool operator<=(const Vector<T, n>& lhs, const Vector<T, n>& rhs) noexcept
	{
		return std::equal(lhs.elements.begin(), lhs.elements.end(), rhs.elements.begin(),
			[](T lhsElement, T rhsElement) -> bool { return lhsElement <= rhsElement; }
		);
	}
	/*!
	 \relates Vector
	 \brief Greater than operator.
	 \details Checks if the \a lhs's elements are greater than the \a rhs's ones.

	 \param[in] lhs The ae::Vector on the left side of the > operator
	 \param[in] rhs The ae::Vector on the right side of the > operator

	 \return True if all of the \a lhs's elements are greater than the \a rhs's ones, false otherwise

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1 = { 0.3f, 0.2f };
	 ae::Vector2f vec2f_2 = { 0.1f, 0.18f };
	 if (vec2f_1 > vec2f_2) {
		...
	 }
	 \endcode

	 \sa operator>=(), operator<(), operator<=()

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline bool operator>(const Vector<T, n>& lhs, const Vector<T, n>& rhs) noexcept
	{
		return std::equal(lhs.elements.begin(), lhs.elements.end(), rhs.elements.begin(),
			[](T lhsElement, T rhsElement) -> bool { return lhsElement > rhsElement; }
		);
	}
	/*!
	 \relates Vector
	 \brief Greater than or equal to operator.
	 \details Checks if the \a lhs's elements are greater than or equal to the \a rhs's ones.

	 \param[in] lhs The ae::Vector on the left side of the >= operator
	 \param[in] rhs The ae::Vector on the right side of the >= operator

	 \return True if all of the \a lhs's elements are greater than or equal to the \a rhs's ones, false otherwise

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1 = { 0.3f, 0.2f };
	 ae::Vector2f vec2f_2 = { 0.1f, 0.2f };
	 if (vec2f_1 >= vec2f_2) {
		...
	 }
	 \endcode

	 \sa operator>(), operator<=(), operator<()

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline bool operator>=(const Vector<T, n>& lhs, const Vector<T, n>& rhs) noexcept
	{
		return std::equal(lhs.elements.begin(), lhs.elements.end(), rhs.elements.begin(),
			[](T lhsElement, T rhsElement) -> bool { return lhsElement >= rhsElement; }
		);
	}

	/*!
	 \relates Vector
	 \brief Multiplication operator.
	 \details Performs a memberwise multiplication of the \a scalar value and the \a vec's elements.

	 \param[in] scalar The value that will be multiplied with the \a vec's elements
	 \param[in] vec The ae::Vector of which its elements will be multiplied with the \a scalar value

	 \return An ae::Vector containing the product of the \a scalar value and of the \a vec's elements

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1({ 0.5f, 0.7f });
	 ae::Vector2f vec2f_2 = 0.8f * vec2f_1;
	 \endcode

	 \sa operator*(const Vector<T, n>&, T), operator/(T, const Vector<T, n>&)

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline Vector<T, n> operator*(T scalar, const Vector<T, n>& vec) noexcept
	{
		return vec * scalar;
	}
	/*!
	 \relates Vector
	 \brief Division operator.
	 \details Performs a memberwise division of the \a scalar value and the \a vec's elements.
	 \note Division by zero isn't handled in Release mode and results in undefined behaviour based on the type.

	 \param[in] scalar The value that will be divided by the \a vec's elements
	 \param[in] vec The ae::Vector of which its elements will be the divisor of the \a scalar value

	 \return An ae::Vector containing the quotient of the \a scalar value and of the \a vec's elements

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1({ 0.5f, 0.7f });
	 ae::Vector2f vec2f_2 = 0.8f / vec2f_1;
	 \endcode

	 \sa operator/(const Vector<T, n>&, T), operator*(T, const Vector<T, n>&)

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline Vector<T, n> operator/(T scalar, const Vector<T, n>& vec)
	{
		// Check if vec's elements are equal to 0 (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			for (const T& element : vec.elements) {
				if (element == static_cast<T>(0)) {
					AEON_LOG_ERROR("Division by zero", "Attempt to divide scalar by 0\nReturning vector.");
					return vec;
				}
			}
		}

		// Divide the scalar value by the vec's elements
		Vector<T, n> result(scalar);
		for (size_t i = 0; i < n; ++i) {
			result.elements[i] /= vec.elements[i];
		}

		return result;
	}

	/*!
	 \relates Vector
	 \brief Unary negation operator.
	 \details Performs a memberwise negation of the \a vec's elements where their sign will be inverted (positive to negative and vice-versa).
	 \note This function is only available for floating-point types and for signed integer types (float, double, int, short, etc.).

	 \param[in] vec The ae::Vector of which its elements' sign will be inverted (positive to negative and vice-versa)

	 \return An ae::Vector containing the opposite signs of the \a vec provided

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1(0.5f, -0.7f);
	 ae::Vector2f vec2f_2 = -vec2f_1; // (-0.5f, 0.7f)
	 \endcode

	 \since v0.6.0
	*/
	template <typename T, size_t n, typename = std::enable_if_t<std::is_signed_v<T>>>
	_NODISCARD inline Vector<T, n> operator-(const Vector<T, n>& vec) noexcept
	{
		// Assign the inverted signs of the vec to result
		Vector<T, n> result;
		for (size_t i = 0; i < n; ++i) {
			result.elements[i] = -vec.elements[i];
		}

		return result;
	}

	/*!
	 \relates Vector
	 \brief Calculates and retrieves the reflection vector of the \a vec and of a \a normal to a surface.
	 \note The \a vec and the \a normal should be unit vectors (normalized) in order to get the desired result.

	 \param[in] vec The ae::Vector that will serve as a base for the calculation of the reflection vector
	 \param[in] normal An ae::Vector containing the normal to a surface, a vector that represents a 90 degree angle and has a magnitude of 1

	 \return The reflection vector based on the \a vec and the \a normal to a surface

	 \par Example:
	 \code
	 ae::Vector3f unitNormalVector = ae::cross(triangleVertex1, triangleVertex2).normalize();
	 ae::Vector3f unitBaseVector = baseVector.normalize();
	 ae::Vector3f reflectionVector = ae::reflect(unitBaseVector, unitNormalVector);
	 \endcode

	 \sa refract()

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline _CONSTEXPR17 Vector<T, n> reflect(const Vector<T, n>& vec, const Vector<T, n>& normal) noexcept
	{
		return (vec - static_cast<T>(2) * dot(vec, normal) * normal);
	}
	/*!
	 \relates Vector
	 \brief Calculates and retrieves the refraction vector of the \a vec, of a \a normal to a surface and of a refraction index \a eta.
	 \note The \a vec and the \a normal should be units vectors (normalized) in order to get the desired result.

	 \param[in] vec The ae::Vector that will serve as a base for the calculation of the refraction vector
	 \param[in] normal An ae::Vector containing the normal to a surface, a vector that represents a 90 degree angle and has a magnitude of 1
	 \param[in] eta The index of refraction of the vector to retrieve

	 \return The refraction vector based on the \a vec, the \a normal to a surface and the index of refraction \a eta

	 \par Example:
	 \code
	 ae::Vector3f unitNormalVector = ae::cross(triangleVertex1, triangleVertex2).normalize();
	 ae::Vector3f unitBaseVector = baseVector.normalize();
	 ae::Vector3f refractionVector = ae::refract(unitBaseVector, unitNormalVector, 2);
	 \endcode

	 \sa reflect()
	 
	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline Vector<T, n> refract(const Vector<T, n>& vec, const Vector<T, n>& normal, T eta)
	{
		const T D = dot(vec, normal);
		const T K = static_cast<T>(1) - eta * eta * (static_cast<T>(1) - D * D);

		if (K < static_cast<T>(0)) {
			return Vector<T, n>();
		}

		return (eta * vec - (eta * D + Math::sqrt(K)) * normal);
	}

	/*!
	 \relates Vector
	 \brief Calculates and retrieves the distance between \a v1 and \a v2.

	 \param[in] v1 The first ae::Vector that will be used to calculate the distance between itself and the \a v2 ae::Vector
	 \param[in] v2 The second ae::Vector that will be used to calculate the distance between the \a v1 ae::Vector and itself

	 \return The distance between \a v1 and \a v2

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1 = { 10.f, 25.f };
	 ae::Vector2f vec2f_2 = { 5.f, -10.f };
	 float distance = ae::distance(vec2f_1, vec2f_2);
	 \endcode

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline T distance(const Vector<T, n>& v1, const Vector<T, n>& v2)
	{
		return (v2 - v1).magnitude();
	}
	/*!
	 \relates Vector
	 \brief Calculates and retrieves the dot product of the \a v1 and the \a v2.
	 \details The dot product of two vectors is the cosine of the angle in radians between them that is scaled by their magnitudes.
	 \note In order to retrieve the unscaled cosine of the angle between them, the two vectors will have to be normalized to retrieve their unit vectors.

	 \param[in] v1 The first ae::Vector that will be used to calculate the dot product between itself and the \a v2 ae::Vector
	 \param[in] v2 The second ae::Vector that will be used to calculate the dot product between the \a v1 ae::Vector and itself

	 \return The dot product (the cosine of the angle in radians) between the \a v1 and the \a v2

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1({ 10.f, 25.f });
	 ae::Vector2f vec2f_2({ 5.f, -10.f });

	 // The two ae::Vector have to be normalized in order to retrieve the unscaled dot product
	 float dotProduct = ae::dot(vec2f_1.normalize(), vec2f_2.normalize()); // Result = cos(angle in radians)
	 \endcode

	 \sa normalize(), angle()

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline _CONSTEXPR17 T dot(const Vector<T, n>& v1, const Vector<T, n>& v2) noexcept
	{
		T dotProduct = static_cast<T>(0);
		for (size_t i = 0; i < n; ++i) {
			dotProduct += v1.elements[i] * v2.elements[i];
		}

		return dotProduct;
	}
	/*!
	 \relates Vector
	 \brief Calculates and retrieves the angle in radians between the \a v1 and the \a v2.
	 \details This function normalizes the two ae::Vector and retrieves the inverse cosine of the angle between them.
	 \note Only floating point types are allowed (float, double, long double).

	 \param[in] v1 The first ae::Vector that will be used to calculate the angle in radians between itself and the \a v2 ae::Vector
	 \param[in] v2 The second ae::Vector that will be used to calculate the angle in radians between the \a v1 ae::Vector and itself

	 \return The angle in radians between the \a v1 and the \a v2

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1({ 10.f, 25.f });
	 ae::Vector2f vec2f_2({ 5.f, -10.f });
	 float angleRadians = ae::angle(vec2f_1, vec2f_2);

	 // The helper function angle() is equivalent to:
	 float angleRadians = ae::Math::acos(ae::dot(vec2f_1.normalize(), vec2f_2.normalize()));
	 \endcode

	 \sa normalize(), dot()

	 \since v0.6.0
	*/
	template <typename T, size_t n, typename = Math::FLOATING_POINT_POLICY<T>>
	_NODISCARD inline T angle(const Vector<T, n>& v1, const Vector<T, n>& v2)
	{
		return Math::acos(dot(v1.normalize(), v2.normalize()));
	}

	/*!
	 \relates Vector
	 \brief Retrieves an ae::Vector containing the minimum elements of the \a v1 and the \a v2 provided.
	 \note The minimum coordinates don't have to belong to the same ae::Vector.

	 \param[in] v1 The first ae::Vector of which its elements will be compared with the \a v2's ones
	 \param[in] v2 The second ae::Vector of which its elements will be compared with the \a v1's ones

	 \return An ae::Vector containing the minimum elements of the \a v1 and \a v2

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1(10.f, 25.f);
	 ae::Vector2f vec2f_2(5.f, 30.f);
	 ae::Vector2f minVec2f = ae::min(vec2f_1, vec2f_2); // (5.f, 25.f)
	 \endcode

	 \sa max(), clamp()

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline Vector<T, n> min(const Vector<T, n>& v1, const Vector<T, n>& v2) noexcept
	{
		Vector<T, n> result;
		for (size_t i = 0; i < n; ++i) {
			result.elements[i] = Math::min(v1.elements[i], v2.elements[i]);
		}

		return result;
	}

	/*!
	 \relates Vector
	 \brief Retrieves an ae::Vector containing the maximum elements of the \a v1 and the \a v2 provided.
	 \note The maximum coordinates don't have to belong to the same ae::Vector.

	 \param[in] v1 The first ae::Vector of which its elements will be compared with the \a v2's ones
	 \param[in] v2 The second ae::Vector of which its elements will be compared with the \a v1's ones

	 \return An ae::Vector containing the maximum elements of the \a v1 and \a v2

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1(10.f, 25.f);
	 ae::Vector2f vec2f_2(5.f, 30.f);
	 ae::Vector2f maxVec2f = ae::max(vec2f_1, vec2f_2); // (10.f, 30.f)
	 \endcode

	 \sa min(), clamp()

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline Vector<T, n> max(const Vector<T, n>& v1, const Vector<T, n>& v2) noexcept
	{
		Vector<T, n> result;
		for (size_t i = 0; i < n; ++i) {
			result.elements[i] = Math::max(v1.elements[i], v2.elements[i]);
		}

		return result;
	}
	/*!
	 \relates Vector
	 \brief Retrieves an ae::Vector situated between the range [\a minVec,\a maxVec].
	 \details If the \a vec is less than the \a minVec, the ae::Vector retrieved will be equal to \a minVec.\n
	 If the \a vec is situated between the range [\a minVec,\a maxVec], the ae::Vector retrieved will be equal to \a vec.\n
	 If the \a vec is greater than the \a maxVec, the ae::Vector retrieved will be equal to \a maxVec.

	 \param[in] vec The ae::Vector to check if it's situated between the range [\a minVec,\a maxVec]
	 \param[in] minVec The minimum ae::Vector allowed
	 \param[in] maxVec The maximum ae::Vector allowed

	 \return An ae::Vector situated between the range [\a minVec,\a maxVec]

	 \par Example:
	 \code
	 ae::Vector2f minVec(0.3f, 0.1f);
	 ae::Vector2f maxVec(0.8f, 0.6f);
	 ae::Vector2f vec(0.2f, 0.5f);
	 ae::Vector2f clampedVec = ae::clamp(vec, minVec, maxVec); // (0.3f, 0.5f)
	 \endcode

	 \sa min(), max()

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline Vector<T, n> clamp(const Vector<T, n>& vec, const Vector<T, n>& minVec, const Vector<T, n>& maxVec) noexcept
	{
		return min(max(vec, minVec), maxVec);
	}

	/*!
	 \relates Vector
	 \brief Smoothstep is a technique that will interpolate between two edges based on the \a vec provided.
	 \details This technique will apply a curve function so the movement starts out slow, speeds up in the middle, then slows down again.\n
	 The ae::Vector retrieved will contain the interpolation factors within the range [0,1].
	 \note If the \a edge1 edge is greater than the \a edge0 edge, the behaviour will be considered undefined.

	 \param[in] vec The ae::Vector that will be used as the target of interpolation
	 \param[in] edge0 The ae::Vector that will be used as the first edge (the minimum point of interpolation)
	 \param[in] edge1 The ae::Vector that will be used as the second edge (the maximum point of interpolation)

	 \return An ae::Vector containing the factors of interpolation of the \a vec according to the edges provided

	 \par Example:
	 \code
	 ae::Vector2f edge0(0.5f, 0.3f);
	 ae::Vector2f edge1(0.65f, 0.7f);
	 ae::Vector2f vec(0.5f, 0.5f);
	 ae::Vector2f interpolationFactors = ae::smoothstep(vec, edge0, edge1); // ([0.f,1.f], [0.f,1.f])

	 // In order to retrieve the interpolated vector between the two edges, the user will have to do the following:
	 ae::Vector2f interpolatedVec = edge0 + ae::smoothstep(vec, edge0, edge1) * (edge1 - edge0); // ([0.5f,0.65f], [0.3f,0.7f])
	 // or
	 ae::Vector2f interpolatedVec = ae::lerp(edge0, edge1, ae::smoothstep(vec, edge0, edge1));
	 \endcode

	 \since v0.6.0
	*/
	template <typename T, size_t n>
	_NODISCARD inline Vector<T, n> smoothstep(const Vector<T, n>& vec, const Vector<T, n>& edge0, const Vector<T, n>& edge1)
	{
		const Vector<T, n> INTERP = clamp((vec - edge0) / (edge1 - edge0), Vector<T, n>(static_cast<T>(0)), Vector<T, n>(static_cast<T>(1)));
		return (INTERP * INTERP * (Vector<T, n>(static_cast<T>(3)) - Vector<T, n>(static_cast<T>(2)) * INTERP));
	}

	/*!
	 \relates Vector
	 \brief Lerp (or mix) is a technique that will linearly interpolate between two control points based on the \a t percentage.
	 \details If \a t is less than 0, the interpolated vector will be situated before the \a A vector.\n
	 If \a t is 0, the interpolated vector will be equal to the \a A vector.\n
	 If \a t is between 0 and 1, the interpolated vector will be situated between the range [\a A,\a B].\n
	 If \a t is 1, the interpolated vector will be equal to the \a B vector.\n
	 If \a t is greater than 1, the interpolated vector will be situated after the \a B vector.
	 \note Only floating point types are allowed (float, double, long double).

	 \param[in] A The ae::Vector indicating the first control point
	 \param[in] B The ae::Vector indicating the second control point
	 \param[in] t The value (in the range [0,1]) that will act as a percentage for the interpolation

	 \return An interpolated ae::Vector situated between the range [\a A,\a B] based on the \a t percentage

	 \par Example:
	 \code
	 ae::Vector2f A(5.f, 2.f);
	 ae::Vector2f B(20.f, 15.f);
	 ae::Vector2f LERP = ae::lerp(A, B, 0.5f);
	 \endcode

	 \since v0.6.0
	*/
	template <typename T, size_t n, typename = Math::FLOATING_POINT_POLICY<T>>
	_NODISCARD inline Vector<T, n> lerp(const Vector<T, n>& A, const Vector<T, n>& B, T t) noexcept
	{
		return (A + t * (B - A));
	}

	// Vector<T, 2> function(s)
	/*!
	 \relates Vector
	 \brief Calculates and retrieves the normal of the segment formed by \a v1 and \a v2.
	 \details The normal of a segment is the perpendicular vector of said segment.
	 \note Only floating point types are allowed (float, double, long double).

	 \param[in] v1 The ae::Vector that will serve as the first point of the segment
	 \param[in] v2 The ae::Vector that will serve as the second point of the segment

	 \return The normal (perpendicular vector) of the segment formed by the two vectors provided

	 \par Example:
	 \code
	 ae::Vector2f vec2f_1(10.f, 5.f);
	 ae::Vector2f vec2f_2(5.f, -2.f);

	 ae::Vector2f normal = ae::normal(vec2f_1, vec2f_2);
	 \endcode

	 \since v0.6.0
	*/
	template <typename T, typename = Math::FLOATING_POINT_POLICY<T>>
	_NODISCARD inline Vector<T, 2> normal(const Vector<T, 2>& v1, const Vector<T, 2>& v2)
	{
		const Vector<T, 2> NORMAL(v1.y - v2.y, v2.x - v1.x);
		const T MAGNITUDE = NORMAL.magnitude();

		return (MAGNITUDE != static_cast<T>(0)) ? NORMAL / MAGNITUDE : NORMAL;
	}

	// Vector<T, 3> function(s)
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
	 ae::Vector3f vec3f_1(10.f, 15.f, -5.f);
	 ae::Vector3f vec3f_2(2.f, 10.f, -2.f);
	 ae::Vector3f crossProduct = ae::cross(vec3f_1.normalize(), vec3f_2.normalize());
	 \endcode

	 \since v0.6.0
	*/
	template <typename T>
	_NODISCARD inline _CONSTEXPR17 Vector<T, 3> cross(const Vector<T, 3>& v1, const Vector<T, 3>& v2) noexcept
	{
		return Vector<T, 3>(v1.y * v2.z - v1.z * v2.y,
		                    v1.z * v2.x - v1.x * v2.z,
		                    v1.x * v2.y - v1.y * v2.x);
	}

	// Typedef(s)
	template <typename T>
	using Vector2 = Vector<T, 2>;           //!< A 2-dimensional vector of type T

	using Vector2f = Vector2<float>;        //!< A 2-dimensional vector of floats
	using Vector2d = Vector2<double>;       //!< A 2-dimensional vector of doubles
	using Vector2i = Vector2<int>;          //!< A 2-dimensional vector of ints
	using Vector2u = Vector2<unsigned int>; //!< A 2-dimensional vector of unsigned ints

	template <typename T>
	using Vector3 = Vector<T, 3>;           //!< A 3-dimensional vector of type T

	using Vector3f = Vector3<float>;        //!< A 3-dimensional vector of floats
	using Vector3d = Vector3<double>;       //!< A 3-dimensional vector of doubles
	using Vector3i = Vector3<int>;          //!< A 3-dimensional vector of ints
	using Vector3u = Vector3<unsigned int>; //!< A 3-dimensional vector of unsigned ints

	template <typename T>
	using Vector4 = Vector<T, 4>;           //!< A 3-dimensional vector with a homogeneous component of type T

	using Vector4f = Vector4<float>;        //!< A 3-dimensional vector with a homogeneous component of floats
	using Vector4d = Vector4<double>;       //!< A 3-dimensional vector with a homogeneous component of doubles
	using Vector4i = Vector4<int>;          //!< A 3-dimensional vector with a homogeneous component of ints
	using Vector4u = Vector4<unsigned int>; //!< A 3-dimensional vector with a homogeneous component of unsigned ints 

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
#endif // Aeon_Math_Vector_H_

/*!
 \struct ae::Vector
 \ingroup math

 The ae::Vector struct defines a mathematical vector n dimensions
 (x, y, z, w, ..., n). In order to instantiate a ae::Vector object, 5 or more
 dimensions need to be provided, the specialized ae::Vector2, ae::Vector3 or
 ae::Vector4 should be used for 2 to 4 dimensions.

 The coordinates of the primary template can only be accessed through the
 'elements' member whereas the specialized vectors offer more intuitive means.

 The ae::Vector struct may be used to represent a point in space, a direction or
 simply a n-dimensional entity, for example: a size, a velocity, etc.

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2021.04.07
 \copyright MIT License
*/

/*!
 \struct ae::Vector<T, 2>
 \ingroup math

 The ae::Vector<T, 2> struct is a partially specialized template that defines a
 2-dimensional mathematical vector (x, y).

 The coordinates of the partially specialized template can be accessed through
 the 'elements' member or through the 'x' and 'y' members. These members occupy
 the same block of memory, they simple offer a more intuitive way of accessing
 said elements.

 Usage example:
 \code
 ae::Vector2f vec2f_1(0.5f, 1.f);
 vec2f_1.x = 5.f;

 ae::Vector2f vec2f_2(2.f, 5.f);
 ae::Vector2f vec2f_3 = vec2f_1 + vec2f_2;

 bool different = (vec2f_1 != vec2f_2);

 float distance = ae::distance(vec2f_1, vec2f_2);
 \endcode

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2021.04.07
 \copyright MIT License
*/

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
 \version v0.6.0
 \date 2021.04.07
 \copyright MIT License
*/

/*!
 \struct ae::Vector<T, 4>
 \ingroup math

 The ae::Vector<T, 4> struct is a partially specialized template that defines a
 3-dimensional mathematical vector with a homogeneous component (x, y, z, w).

 The coordinates of the partially specialized template can be accessed through
 the 'elements' member or through the 'x', 'y', 'z' and 'w' members or through
 the 'xy' and the 'xzy' members that are a 2-dimensional vector and a
 3-dimensional vector respectively. These members occupy the same block of
 memory, they simply offer a more intuitive way of accessing said elements.

 Usage example:
 \code
 ae::Vector4f vec4f_1(0.5f, 1.f, 5.f, 1.f);
 vec4f_1.x = 5.f;

 ae::Vector4f vec4f_2(2.f, 5.f, 8.f, 1.f);
 ae::Vector4f vec4f_3 = vec4f_1 + vec4f_2;

 bool different = (vec4f_1 != vec4f_2);
 \endcode

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2020.05.01
 \copyright MIT License
*/