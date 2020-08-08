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

#ifndef Aeon_Math_Matrix_H_
#define Aeon_Math_Matrix_H_

#include <string>

#include <AEON/Math/Vector.h>
#include <AEON/Math/Vector2.h>
#include <AEON/Math/Vector3.h>
#include <AEON/Math/Vector4.h>
#include <AEON/Math/Misc.h>
#include <AEON/Math/internal/Quaternion.h>
#include <AEON/System/DebugLogger.h>

namespace ae
{
	// Template policies
	template <typename T, size_t n, size_t m>
	using MATRIX_POLICY = std::enable_if_t<std::is_arithmetic_v<T> && (n >= 2 && m >= 2)>; //!< The template parameters will only be enabled if T is an arithmetic type and n and m are greater or equal to 2
	template <size_t n, size_t m>
	using MATRIX_SQUARE_POLICY = std::enable_if_t<(n == m)>;                               //!< The template parameters will only be enabled if the n and m dimensions are equal

	/*!
	 \brief The struct that represents the primary template for a NxM matrix of type T in column-major.
	 \details The n represents the number of columns and the m represents the number of rows.
	 \note Only arithmetic types are allowed (float, int, etc.) and the minimum number of columns and rows is 2.
	*/
	template <typename T, size_t n, size_t m, typename = MATRIX_POLICY<T, n, m>>
	struct _NODISCARD Matrix
	{
		// Public member data
		union {
			std::array<T, n * m>        elements; //!< The NxM elements of the matrix
			std::array<Vector<T, m>, n> columns;  //!< The n columns of the matrix
		};

		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details Sets the elements to the value 0 of the type provided.

		 \since v0.3.0
		*/
		_CONSTEXPR17 Matrix() noexcept
			: elements()
		{
		}
		/*!
		 \brief Constructs the ae::Matrix by providing a \a diagonal scalar value.
		 \details Sets the diagonally-aligned elements to the \a diagonal scalar value provided and the remaining elements to 0.
		 \note Only square matrices can use this constructor, meaning NxN matrices (3x3, 4x4, etc.).

		 \param[in] diagonal The scalar value that will be attributed to the diagonally-aligned elements

		 \par Example:
		 \code
		 constexpr ae::Matrix4f mat4f(1.f);
		 \endcode

		 \since v0.3.0
		*/
		template <typename = MATRIX_SQUARE_POLICY<n, m>>
		explicit _CONSTEXPR17 Matrix(T diagonal) noexcept
			: elements()
		{
			for (size_t i = 0; i < n; ++i) {
				elements[i + i * n] = diagonal;
			}
		}
		/*!
		 \brief Constructs the ae::Matrix by providing the n columns of the matrix.
		 \details The n ae::Vector objects provided will be attributed to the n columns of the matrix.
		 \note It's up to the user to provide the correct number of columns and of the right type.

		 \param[in] column0 The ae::Vector containing the elements that will be attributed to the first column
		 \param[in] column1 The ae::Vector containing the elements that will be attributed to the second column
		 \param[in] columns The remaining ae::Vector objects containing the elements that will be attributed to the columns

		 \par Example:
		 \code
		 constexpr ae::Matrix4f mat4f(ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f),
		                              ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f));
		 \endcode

		 \since v0.3.0
		*/
		template <typename... Columns>
		_CONSTEXPR17 Matrix(const Vector<T, m>& column0, const Vector<T, m>& column1, Columns... columns) noexcept
			: columns({ column0, column1, columns... })
		{
		}
		/*!
		 \brief Constructs the ae::Matrix by providing the NxM elements of the matrix.
		 \note It's up to the user to provide the correct number of elements and of the right type.

		 \param[in] element0 The first element that will be attributed to the ae::Matrix
		 \param[in] element1 The second element that will be attributed to the ae::Matrix
		 \param[in] element2 The third element that will be attributed to the ae::Matrix
		 \param[in] element3 The third element that will be attributed to the ae::Matrix
		 \param[in] elements The remaining elements that will be attributed to the ae::Matrix

		 \par Example:
		 \code
		 constexpr ae::Matrix4f mat4f(0.5f, 0.25f, -1.f, 1.f, 0.5f, 0.25f, -1.f, 1.f,
		                              0.5f, 0.25f, -1.f, 1.f, 0.5f, 0.25f, -1.f, 1.f);
		 \endcode

		 \since v0.3.0
		*/
		template <typename... Elements>
		_CONSTEXPR17 Matrix(T element0, T element1, T element2, T element3, Elements... elements) noexcept
			: elements({ element0, element1, element2, element3, elements... })
		{
		}
		/*!
		 \brief Constructs the ae::Matrix by providing the n columns of the matrix.
		 \details The n ae::Vector objetcts provided will be attributed to the n columns of the matrix.
		 \note It's up to the user to provide the correct number of columns.

		 \param[in] columns The initializer list of columns

		 \par Example:
		 \code
		 ae::Matrix4f mat4f({ ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f),
		                      ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f) });
		 // or
		 ae::Matrix4f mat4f = { ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f),
		                        ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f) };
		 \endcode

		 \since v0.2.0
		*/
		Matrix(std::initializer_list<Vector<T, m>> columns) noexcept
			: columns()
		{
			// Copy the columns until the maximum number of columns that both the matrix and the list hold
			std::copy_n(columns.begin(), Math::min(n, columns.size()), this->columns.begin());
		}
		/*!
		 \brief Constructs the ae::Matrix by providing the NxM elements of the matrix.
		 \note It's up to the user to provide the correct number of elements.

		 \param[in] elements The initializer list of elements

		 \par Example:
		 \code
		 ae::Matrix4f mat4f({ 0.5f, 0.25f, -1.f, 1.f, 0.5f, 0.25f, -1.f, 1.f,
		                      0.5f, 0.25f, -1.f, 1.f, 0.5f, 0.25f, -1.f, 1.f });
		 // or
		 ae::Matrix4f mat4f = { 0.5f, 0.25f, -1.f, 1.f, 0.5f, 0.25f, -1.f, 1.f,
		                        0.5f, 0.25f, -1.f, 1.f, 0.5f, 0.25f, -1.f, 1.f };
		 \endcode

		 \since v0.2.0
		*/
		Matrix(std::initializer_list<T> elements) noexcept
			: elements()
		{
			// Copy the elements until the maximum number of elements that both the matrix and the list hold
			std::copy_n(elements.begin(), Math::min(this->elements.size(), elements.size()), this->elements.begin());
		}
		/*!
		 \brief Constructs the ae::Matrix by providing an std::array of columns.
		 \details Sets the ae::Matrix's columns to the std::array of \a columns provided.

		 \param[in] columns The C++11 array of columns to assign to the ae::Matrix

		 \par Example:
		 \code
		 constexpr std::array<ae::Vector<float, 4>, 4> columns = {
			 ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f),
			 ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f)
		 };
		 constexpr ae::Matrix4f mat4f(columns);
		 \endcode

		 \since v0.3.0
		*/
		explicit _CONSTEXPR17 Matrix(const std::array<Vector<T, m>, n>& columns) noexcept
			: columns(columns)
		{
		}
		/*!
		 \brief Constructs the ae::Matrix by providing an std::array of elements.
		 \details Sets the ae::Matrix's elements to the std::array of \a elements provided.

		 \param[in] elements The C++11 array of elements to assign to the ae::Matrix

		 \par Example:
		 \code
		 constexpr std::array<float, 16> elements = {
			 0.5f, 0.25f, -1.f, 1.f, 0.5f, 0.25f, -1.f, 1.f,
			 0.5f, 0.25f, -1.f, 1.f, 0.5f, 0.25f, -1.f, 1.f
		 };
		 constexpr ae::Matrix4f mat4f(elements);
		 \endcode

		 \since v0.3.0
		*/
		explicit _CONSTEXPR17 Matrix(const std::array<T, n * m>& elements) noexcept
			: elements(elements)
		{
		}
		/*!
		 \brief Constructs the ae::Matrix by providing a C-style array of columns.
		 \details Sets the ae::Matrix's columns to the C-style array of \a columns provided.

		 \param[in] columns The C-style array of columns to assign to the ae::Matrix

		 \par Example:
		 \code
		 ae::Vector<float, 4> columns[4] = {
			ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f),
			ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f)
		 };
		 ae::Matrix4f mat4f(columns);
		 \endcode
		 
		 \since v0.2.0
		*/
		explicit Matrix(const Vector<T, m>(&columns)[n]) noexcept
			: columns()
		{
			// Copy the columns of the C-style array to the matrix's columns
			std::copy_n(columns, n, this->columns.begin());
		}
		/*!
		 \brief Constructs the ae::Matrix by providing a C-style array of elements.
		 \details Sets the ae::Matrix's elements to the C-style array of \a elements provided.

		 \param[in] elements The C-style array of elements to assign to the ae::Matrix
		 
		 \par Example:
		 \code
		 float elements[16] = {
			0.5f, 0.25f, -1.f, 1.f, 0.5f, 0.25f, -1.f, 1.f,
			0.5f, 0.25f, -1.f, 1.f, 0.5f, 0.25f, -1.f, 1.f
		 };
		 ae::Matrix4f mat4f(elements);
		 \endcode

		 \since v0.2.0
		*/
		explicit Matrix(const T(&elements)[n * m]) noexcept
			: elements()
		{
			// Copy the elements of the C-style array to the matrix's elements
			std::copy_n(elements, this->elements.size(), this->elements.begin());
		}
		/*!
		 \brief Constructs the ae::Matrix by providing a ae::Matrix of another type and/or with a different number of columns/rows.
		 \details Sets the ae::Matrix's elements to the \a matU's elements, up to the maximum numbers of columns/rows that both matrices hold.\n
		 If the ae::Matrix that's about to be constructed is a 3x3 and the ae::Matrix provided is a 4x4, the upper left 3x3 of the 4x4 will be copied.\n
		 Essentially, the last column and the last row will be eliminated.

		 \param[in] matU The ae::Matrix of another type and/or with a different number of columns/rows to assign to the ae::Matrix

		 \par Example:
		 \code
		 ae::Matrix4i mat4i(
			 0,  1,  2,  3,
			 4,  5,  6,  7,
			 8,  9, 10, 11,
			12, 13, 14, 15);
		 // mat4i Layout:
		 // 0 4  8 12
		 // 1 5  9 13
		 // 2 6 10 14
		 // 3 7 11 15

		 ae::Matrix3f mat3f(mat4i);
		 // mat3f Layout:
		 // 0 4  8
		 // 1 5  9
		 // 2 6 10
		 \endcode

		 \since v0.3.0
		*/
		template <typename U, size_t n2, size_t m2>
		Matrix(const Matrix<U, n2, m2>& matU) noexcept
			: columns()
		{
			_CONSTEXPR17 const size_t MIN_N = Math::min(n, n2);
			_CONSTEXPR17 const size_t MIN_M = Math::min(m, m2);

			for (size_t i = 0; i < MIN_N; ++i) {
				for (size_t j = 0; j < MIN_M; ++j) {
					if _CONSTEXPR_IF (std::is_same_v<T, U>) {
						columns[i][j] = matU.columns[i][j];
					}
					else {
						columns[i][j] = static_cast<T>(matU.columns[i][j]);
					}
				}
			}
		}
		/*!
		 \brief Copy constructor.
		 \details Sets the ae::Matrix's elements to the \a copy's elements.

		 \param[in] copy The ae::Matrix of the same type and the same number of elements that will be copied

		 \par Example:
		 \code
		 constexpr ae::Matrix4f mat4f_1(ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f),
		                                ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f));
	     constexpr ae::Matrix4f mat4f_2(mat4f_1);
		 // or
		 constexpr ae::Matrix4f mat4f_2 = mat4f_1;
		 \endcode

		 \since v0.3.0
		*/
		_CONSTEXPR17 Matrix(const Matrix<T, n, m>& copy) noexcept
			: elements(copy.elements)
		{
		}
		/*!
		 \brief Move constructor.
		 \details Performs a move operation on the \a rvalue's elements to the lvalue ae::Matrix's elements.

		 \param[in] rvalue The ae::Matrix rvalue that will be moved

		 \par Example:
		 \code
		 constexpr ae::Matrix4f mat4f_1(ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f),
		                                ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f));
		 ...
		 constexpr ae::Matrix4f mat4f_2(std::move(mat4f_1));
		 // or
		 constexpr ae::Matrix4f mat4f_2 = std::move(mat4f_1);
		 \endcode

		 \since v0.3.0
		*/
		_CONSTEXPR17 Matrix(Matrix<T, n, m>&& rvalue) noexcept
			: elements(std::move(rvalue.elements))
		{
		}

		// Public operator(s)
		/*!
		 \brief Assignment operator.
		 \details Performs a memberwise assignment from the \a other's elements to the caller ae::Matrix's ones.

		 \param[in] other The ae::Matrix of which its elements will be copied over to the caller's ones

		 \return The caller ae::Matrix containing the new elements, these calls can be chained together one after the other

		 \par Example:
		 \code
		 ae::Matrix4f mat4f_1(ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f),
		                      ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f));
		 ae::Matrix4f mat4f_2(1.f);
		 ...
		 mat4f_1 = mat4f_2;
		 \endcode

		 \since v0.3.0
		*/
		Matrix<T, n, m>& operator=(const Matrix<T, n, m>& other) noexcept
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
		 \details Performs a memberwise move assignment from the \a rvalue's elements to the caller ae::Matrix's ones.

		 \param[in] rvalue The rvalue ae::Matrix of which its elements will be moved over to the caller's ones

		 \return The caller ae::Matrix containing the moved elements, these calls can be chained together one after the other

		 \par Example:
		 \code
		 ae::Matrix4f mat4f_1(ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f),
		                      ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f));
		 ae::Matrix4f mat4f_2(ae::Vector4f(5.f, 2.25f, -1.f, 1.f), ae::Vector4f(5.5f, 1.25f, -1.f, 1.f),
		                      ae::Vector4f(5.f, 2.25f, -1.f, 1.f), ae::Vector4f(3.5f, 1.25f, -1.f, 1.f));
		 ae::Matrix4f mat4f_3(1.f);
		 ...
		 mat4f_3 = mat4f_1 * mat4f_2;
		 \endcode

		 \since v0.2.0
		*/
		Matrix<T, n, m>& operator=(Matrix<T, n, m>&& rvalue) noexcept
		{
			elements = std::move(rvalue.elements);
			return *this;
		}
		/*!
		 \brief Equality operator.
		 \details Checks if the caller ae::Matrix's elements are equal to the \a other's elements.

		 \param[in] other The ae::Matrix which its elements will be compared to the caller's ones

		 \return True if all of the \a other's elements are equal to the caller's elements

		 \par Example:
		 \code
		 ae::Matrix4f mat4f_1(ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f),
		                      ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f));
		 ae::Matrix4f mat4f_2(ae::Vector4f(5.f, 2.25f, -1.f, 1.f), ae::Vector4f(5.5f, 1.25f, -1.f, 1.f),
		                      ae::Vector4f(5.f, 2.25f, -1.f, 1.f), ae::Vector4f(3.5f, 1.25f, -1.f, 1.f));
		 if (mat4f_1 == mat4f_2) {
			...
		 }
		 \endcode

		 \sa operator!=()

		 \since v0.5.0
		*/
		_NODISCARD bool operator==(const Matrix<T, n, m>& other) const
		{
			return elements == other.elements;
		}
		/*!
		 \brief Inequality operator.
		 \details Checks if the caller ae::Matrix's elements are inequal to the \a other's elements.

		 \param[in] other The ae::Matrix which its elements will be compared to the caller's ones

		 \return True if at least one of the \a other's elements is inequal to the caller's elements

		 \par Example:
		 \code
		 ae::Matrix4f mat4f_1(ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f),
							  ae::Vector4f(0.5f, 0.25f, -1.f, 1.f), ae::Vector4f(0.5f, 0.25f, -1.f, 1.f));
		 ae::Matrix4f mat4f_2(ae::Vector4f(5.f, 2.25f, -1.f, 1.f), ae::Vector4f(5.5f, 1.25f, -1.f, 1.f),
							  ae::Vector4f(5.f, 2.25f, -1.f, 1.f), ae::Vector4f(3.5f, 1.25f, -1.f, 1.f));
		 if (mat4f_1 != mat4f_2) {
			...
		 }
		 \endcode

		 \sa operator==()

		 \since v0.5.0
		*/
		_NODISCARD bool operator!=(const Matrix<T, n, m>& other) const
		{
			return elements != other.elements;
		}
		/*!
		 \brief Transformation application operator.
		 \details Applies the transformation \a other to the caller ae::Matrix.\n
		 The resulting matrix will have the same number of rows as the first matrix and the same number of columns as the second matrix.
		 \note The number of columns n of the first matrix must equal the number of rows m of the second matrix.

		 \param[in] other The ae::Matrix that will be applied to the caller ae::Matrix

		 \par Example:
		 \code
		 ae::Matrix4f mat4f_1(2.f);
		 ae::Matrix4f mat4f_2(0.5f);
		 ae::Matrix4f mat4f_3 = mat4f_1 * mat4f_2;
		 \endcode

		 \since v0.3.0
		*/
		template <size_t n2, size_t m2, typename = std::enable_if_t<(n == m2)>>
		_NODISCARD _CONSTEXPR17 Matrix<T, n2, m> operator*(const Matrix<T, n2, m2>& other) const noexcept
		{
			// Perform the non-commutative multiplication of the two matrices
			Matrix<T, n2, m> mat;
			for (size_t i = 0; i < m; ++i) {
				for (size_t j = 0; j < n2; ++j) {
					for (size_t k = 0; k < m2; ++k) {
						mat.columns[j][i] += columns[k][i] * other.columns[j][k];
					}
				}
			}

			return mat;
		}
		/*!
		 \brief Transformation application and assigment operator.
		 \details Applies the transformation \a other to the caller ae::Matrix, and assigns the result to the caller ae::Matrix.\n
		 The resulting matrix will have the same number of rows as the first matrix and the same number of columns as the second matrix.
		 \note The number of columns n of the first matrix must equal the number of rows m of the second matrix.

		 \param[in] other The ae::Matrix that will be applied to the caller ae::Matrix

		 \par Example:
		 \code
		 ae::Matrix4f mat4f_1(2.f);
		 ae::Matrix4f mat4f_2(0.5f);
		 ...
		 mat4f_1 *= mat4f_2;
		 \endcode

		 \since v0.2.0
		*/
		template <size_t n2, size_t m2, typename = std::enable_if_t<(n == m2)>>
		Matrix<T, n2, m> operator*=(const Matrix<T, n2, m2>& other) noexcept
		{
			return (*this = *this * other);
		}

		/*!
		 \brief Vector transformation application operator.
		 \details Applies the ae::Matrix's transformation to the ae::Vector \a vec.
		 \note The number of elements of the vector must equal the number of columns n of the matrix.

		 \param[in] vec The n-dimensional ae::Vector that will be transformed based on the ae::Matrix's transformation

		 \return An m-dimensional ae::Vector containing the transformed \a vec

		 \par Example:
		 \code
		 // Uniform matrix
		 ae::Matrix4f mat4f(0.5f);
		 ae::Vector4f vec4f(0.5f, 0.7f, -1.f, 1.f);
		 ae::Vector4f transformedVec4f = mat4f * vec4f;

		 // Non-uniform matrix (5 columns, 3 rows)
		 ae::Matrix<float, 5, 3> mat53f(...);

			// In order to perform a matrix-vector multiplication, the vector must have the same number of elements as the matrix has columns (5 in this case)
		 ae::Vector5f vec5f(5.f, 2.f, 4.f, 2.f, 1.f);

			// The transformed vector will have the same number of elements as the matrix has rows (3 in this case)
		 ae::Vector3f vec3f = mat53f * vec5f;
		 \endcode

		 \sa operator*(const Vector<T, n2>&)

		 \since v0.3.0
		*/
		_NODISCARD _CONSTEXPR17 Vector<T, m> operator*(const Vector<T, n>& vec) const noexcept
		{
			Vector<T, m> result;
			for (size_t i = 0; i < m; ++i) {
				result[i] = dot(getRow(i), vec);
			}

			return result;
		}
		/*!
		 \brief Vector transformation application operator.
		 \details Applies the ae::Matrix's transformation to the ae::Vector \a vec.\n
		 This is used to multiply, for example, a 3-dimensional vector with a 4x4 matrix
		 \note The number of elements of the vector must equal the number of columns n - 1 of the matrix.
		 
		 \param[in] vec The ae::Vector with n-1 dimensions that will be transformed based on the ae::Matrix's transformation plus its additional dimension

		 \return An ae::Vector with m-1 dimensions containing the transformed \a vec

		 \par Example:
		 \code
		 // Uniform matrix
		 ae::Matrix4f mat4f(0.5f);
		 ae::Vector3f vec3f(0.5f, 0.7f, -1.f);
		 ae::Vector3f transformedVec3f = mat4f * vec3f;
		 \endcode

		 \sa operator*(const Vector<T, n>&)

		 \since v0.3.0
		*/
		template <size_t n2, typename = std::enable_if_t<(n2 == n - 1)>>
		_NODISCARD _CONSTEXPR17 auto operator*(const Vector<T, n2>& vec) const noexcept
		{
			_CONSTEXPR17 const size_t m2 = m - 1;

			Vector<T, m2> result;
			for (size_t i = 0; i < m2; ++i) {
				Vector<T, n> row = getRow(i);
				result[i] = dot(Vector<T, n2>(row), vec) + row.elements.back();
			}

			return result;
		}

		// Public method(s)
		/*!
		 \brief Retrieve the row indicated by the \a index provided of the ae::Matrix.
		 \note If the index isn't situated between 0 and m-1, an error message will be logged and erroneous data will be retrieved.

		 \param[in] index The row to retrieve situated in the range [0,m-1]

		 \return An ae::Vector containing the row's elements

		 \par Example:
		 \code
		 ae::Matrix4f mat4f(1.f);
		 ae::Vector4f secondRow = mat4f.getRow(1);
		 \endcode

		 \sa setRow();

		 \since v0.3.0
		*/
		_NODISCARD Vector<T, n> getRow(size_t index) const noexcept
		{
			// Check that the index provided is within the array limits (ignored in Release mode)
			if _CONSTEXPR_IF (AEON_DEBUG) {
				if (index > m - 1) {
					AEON_LOG_ERROR("Invalid index", "The index provided isn't situated within the matrix's limits [0," + std::to_string(m - 1) + "].\nReturning erroneous data.");
					return Vector<T, n>();
				}
			}

			// Retrieve the specified row's elements
			Vector<T, n> row;
			for (size_t i = 0; i < n; ++i) {
				row[i] = elements[index + i * m];
			}

			return row;
		}
		/*!
		 \brief Sets the values of the row indicated by the \a index provided of the ae::Matrix.
		 \note If the index isn't situated between 0 and m-1, an error message will be logged and the operation will be aborted.

		 \param[in] row An n-dimensional ae::Vector containing the new values of the row
		 \param[in] index The row that will be modified situated in the range [0,m-1]

		 \par Example:
		 \code
		 ae::Matrix4f mat4f(1.f);
		 ae::Vector4f secondRow(2.f, 0.5f, -1.f, 1.f);
		 mat4f.setRow(secondRow, 1);
		 \endcode

		 \sa getRow()

		 \since v0.3.0
		*/
		void setRow(const Vector<T, n>& row, size_t index) noexcept
		{
			// Check that the index provided is within the array limits (ignored in Release mode)
			if _CONSTEXPR_IF (AEON_DEBUG) {
				if (index > m - 1) {
					AEON_LOG_ERROR("Invalid index", "The index provided isn't situated within the matrix's limits [0," + std::to_string(m - 1) + "].\nAborting operation.");
					return;
				}
			}

			// Modify the specified row's elements
			for (size_t i = 0; i < n; ++i) {
				elements[index + i * m] = row[i];
			}
		}

		/*!
		 \brief Retrieves a submatrix of the ae::Matrix by deleting the column and the row provided.

		 \param[in] col The column of the ae::Matrix to delete, 0-based indexing (column 0 is the first column)
		 \param[in] row The row of the ae::Matrix to delete, 0-based indexing (row 0 is the first row)

		 \return An ae::Matrix containing the submatrix of the caller ae::Matrix

		 \par Example:
		 \code
		 ae::Matrix4i mat4i(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
		 ae::Matrix4i mat4i_submatrix = mat4i.getSubmatrix(2, 3);
		 // Original matrix:
		 // 0 4  8 12
		 // 1 5  9 13
		 // 2 6 10 14
		 // 3 7 11 15
		 //
		 // Submatrix:
		 // 0 4 0 12
		 // 1 5 0 13
		 // 2 6 0 14
		 // 0 0 0  0
		 \endcode

		 \since v0.4.0
		*/
		_NODISCARD _CONSTEXPR17 Matrix<T, n, m> getSubmatrix(size_t col, size_t row) const noexcept
		{
			Matrix<T, n, m> submatrix;

			size_t i = 0, j = 0;
			for (size_t r = 0; r < m; ++r) {
				for (size_t c = 0; c < n; ++c) {
					// Copy only the elements which aren't in the given column and row
					if (r != row && c != col) {
						submatrix.columns[j++][i] = columns[c][r];

						// If the row is filled, reset the column index and increase the row index
						if (j == n - 1) {
							j = 0;
							++i;
						}
					}
				}
			}

			return submatrix;
		}
		/*!
		 \brief Calculates and retrieves the determinant of the square ae::Matrix.
		 \details The determinant of a square matrix is a scalar value that encodes certain properties of the linear transformation described by said matrix.
		 \note Only square matrices can use this method, meaning NxN matrices (3x3, 4x4, etc.).

		 \param[in] actualN This parameter is meant to be used internally as this method is recursive and is used to count the inner dimensions of the submatrices

		 \return The scalar value representing the determinant of the ae::Matrix

		 \par Example:
		 \code
		 ae::Matrix4f mat4f(1.f, 1.f, 4.f, 5.f, 3.f, 3.f, 3.f, 2.f, 5.f, 1.f, 9.f, 0.f, 9.f, 7.f, 7.f, 9.f);
		 float mat4f_determinant = mat4f.getDeterminant(); // -376.f
		 \endcode

		 \since v0.3.0
		*/
		template <typename = MATRIX_SQUARE_POLICY<n, m>>
		_NODISCARD _CONSTEXPR17 T getDeterminant(size_t actualN = n) const noexcept
		{
			// If the matrix only contains a single element, retrieve it
			if (actualN == 1) {
				return elements[0];
			}

			T determinant = static_cast<T>(0);
			T sign = static_cast<T>(1);

			for (size_t i = 0; i < n; ++i) {
				// Calculate the determinant using the submatrix of Matrix[i][0]
				determinant += sign * columns[i][0] * getSubmatrix(i, 0).getDeterminant(actualN - 1);

				// Reverse the sign
				sign = -sign;
			}

			return determinant;
		}
		/*!
		 \brief Calculates and retrieves the adjoint matrix of the ae::Matrix.
		 \details The adjoint matrix of a square matrix is the transpose of its cofactor matrix.
		 \note Only square matrices can use this method, meaning NxN matrices (3x3, 4x4, etc.).

		 \return An ae::Matrix containing the adjoint matrix of the ae::Matrix

		 \par Example:
		 \code
		 ae::Matrix3f mat3f_1(3.f, 1.f, 2.f, 1.f, 3.f, 4.f, 1.f, -1.f, 1.f);
		 ae::Matrix3f mat3f_1_adjoint = mat3f_1.getAdjoint();
		 \endcode

		 \since v0.4.0
		*/
		template <typename = MATRIX_SQUARE_POLICY<n, m>>
		_NODISCARD _CONSTEXPR17 Matrix<T, n, m> getAdjoint() const noexcept
		{
			_CONSTEXPR17 const T T_ONE = static_cast<T>(1);
			T sign = T_ONE;

			Matrix<T, n, m> adjoint;
			for (size_t i = 0; i < n; ++i) {
				for (size_t j = 0; j < n; ++j) {
					// The sign of adj[i][j] is positive if the sum of the row and columns indices is even
					sign = ((i + j) % 2 == 0) ? T_ONE : -T_ONE;

					// Interchange rows and columns to get the transpose of the cofactor matrix
					adjoint.columns[i][j] = sign * getSubmatrix(j, i).getDeterminant(n - 1);
				}
			}

			return adjoint;
		}
		/*!
		 \brief Calculates and retrieves the inverse of the NxN ae::Matrix.
		 \details The inverse of a NxN matrix can be used to cancel a concatenated transformation in order to retrieve the original transformation.
		 \note Only square matrices can use this constructor, meaning NxN matrices (3x3, 4x4, etc.).

		 \par Example:
		 \code
		 ae::Matrix4f mat4f(2.f);
		 ae::Matrix4f invertedMat4f = mat4f.invert();
		 \endcode

		 \since v0.3.0
		*/
		template <typename = MATRIX_SQUARE_POLICY<n, m>>
		_NODISCARD _CONSTEXPR17 Matrix<T, n, m> invert() const noexcept
		{
			// Calculate the determinant of the matrix and check if it's valid (ignored in Release mode)
			const T DETERMINANT = getDeterminant();
			if _CONSTEXPR_IF (AEON_DEBUG) {
				if (DETERMINANT == static_cast<T>(0)) {
					AEON_LOG_WARNING("Singular matrix", "The caller matrix is singular, its inverse can't be calculated.\nReturning caller matrix.");
					return *this;
				}
			}

			// Calculate the adjoint of the matrix
			const Matrix<T, n, m> ADJOINT = getAdjoint();

			// Calculate the inverse using the following formula: "inv(A) = adj(A) / det(A)"
			Matrix<T, n, m> inverse;
			for (size_t i = 0; i < n; ++i) {
				for (size_t j = 0; j < n; ++j) {
					inverse.columns[j][i] = ADJOINT.columns[j][i] / DETERMINANT;
				}
			}

			return inverse;
		}
		/*!
		 \brief Calculates and retrieves the transpose of the ae::Matrix.
		 \details The transpose of a matrix is an operator which flips a flip over its diagonal.\n
		 It can be used to convert a matrix in column-major into row-major and vice-versa.

		 \return An ae::Matrix containing the transpose of the caller ae::Matrix

		 \par Example:
		 \code
		 ae::Matrix4f mat4f_columnMajor(2.f);
		 ae::Matrix4f mat4f_rowMajor = mat4f_columnMajor.transpose();
		 \endcode

		 \since v0.3.0
		*/
		_NODISCARD _CONSTEXPR17 Matrix<T, m, n> transpose() const noexcept
		{
			Matrix<T, m, n> transposedMatrix;
			for (size_t i = 0; i < m; ++i) {
				transposedMatrix.columns[i] = getRow(i);
			}

			return transposedMatrix;
		}

		// Public static method(s)
		/*!
		 \brief Constructs an ae::Matrix with its diagonally-aligned elements set to the value 1 and its remaining elements to 0.
		 \details This matrix is the base matrix, if this matrix is multiplied with another, it will remain the same.\n
		 This static method is equivalent to constructing a matrix by providing the scalar value of 1.
		 \note Only square matrices can be constructed using this static method, meaning NxN matrices (3x3, 4x4, etc.).

		 \return An ae::Matrix with its diagonally-aligned elements set to 1.

		 \par Example:
		 \code
		 ae::Matrix4f identityMatrix = ae::Matrix4f::identity();
		 \endcode

		 \since v0.3.0
		*/
		template <typename = MATRIX_SQUARE_POLICY<n, m>>
		_NODISCARD static Matrix<T, n, m> identity() noexcept
		{
			return Matrix<T, n, m>(static_cast<T>(1));
		}

		/*!
		 \brief Constructs a 4x4 ae::Matrix containing an orthographic projection.
		 \details An orthographic projection matrix linearly maps view-space coordinates to clip-space coordinates.\n
		 This projection can be used for representing 3-dimensional objects in 2 dimensions in which all projection lines are orthogonal to the projection plane.

		 \param[in] left The left edge of the scene's view space (X axis)
		 \param[in] right The right edge of the scene's view space (X axis)
		 \param[in] bottom The bottom edge of the scene's view space (Y axis)
		 \param[in] top The top edge of the scene's view space (Y axis)
		 \param[in] near The near plane of the scene's view space (Z axis)
		 \param[in] far The far plane of the scene's view space (Z axis)

		 \return A 4x4 ae::Matrix contenaining the appropriate orthographic projection

		 \par Example:
		 \code
		 ae::Matrix4f ortho = ae::Matrix4f::orthographic(0.f, windowWidth, windowHeight, 0.f, 0.f, 1000.f);
		 \endcode

		 \sa orthographic(T, T, T, T), perspective()

		 \since v0.4.0
		*/
		_NODISCARD static Matrix<T, 4, 4> orthographic(T left, T right, T bottom, T top, T near, T far)
		{
			_CONSTEXPR17 const T T_TWO = static_cast<T>(2);

			const T RMINUSL = right - left;
			const T TMINUSB = top - bottom;
			const T FMINUSN = far - near;

			Matrix<T, 4, 4> ortho = ae::Matrix<T, 4, 4>::identity();
			ortho.columns[0][0] = T_TWO / RMINUSL;
			ortho.columns[1][1] = T_TWO / TMINUSB;
			ortho.columns[2][2] = -T_TWO / FMINUSN;
			ortho.columns[3][0] = -((right + left) / RMINUSL);
			ortho.columns[3][1] = -((top + bottom) / TMINUSB);
			ortho.columns[3][2] = -((far + near) / FMINUSN);

			return ortho;
		}
		/*!
		 \brief Constructs a 4x4 ae::Matrix containing an orthographic projection.
		 \details An orthographic projection matrix linearly maps view-space coordinates to clip-space coordinates.\n
		 This projection can be used for a 2D camera or for the user interface (UI) of a 3D game.

		 \param[in] left The left edge of the scene's view space (X axis)
		 \param[in] right The right edge of the scene's view space (X axis)
		 \param[in] bottom The bottom edge of the scene's view space (Y axis)
		 \param[in] top The top edge of the scene's view space (Y axis)

		 \return A 4x4 ae::Matrix contenaining the appropriate orthographic projection

		 \par Example:
		 \code
		 ae::Matrix4f ortho = ae::Matrix4f::orthographic(0.f, windowWidth, windowHeight, 0.f);
		 \endcode

		 \sa orthographic(T, T, T, T, T, T), perspective()

		 \since v0.4.0
		*/
		_NODISCARD static Matrix<T, 4, 4> orthographic(T left, T right, T bottom, T top)
		{
			_CONSTEXPR17 const T T_TWO = static_cast<T>(2);

			const T RMINUSL = right - left;
			const T TMINUSB = top - bottom;

			Matrix<T, 4, 4> ortho = ae::Matrix<T, 4, 4>::identity();
			ortho.columns[0][0] = T_TWO / RMINUSL;
			ortho.columns[1][1] = T_TWO / TMINUSB;
			ortho.columns[2][2] = -static_cast<T>(1);
			ortho.columns[3][0] = -((right + left) / RMINUSL);
			ortho.columns[3][1] = -((top + bottom) / TMINUSB);

			return ortho;
		}
		/*!
		 \brief Constructs a 4x4 ae::Matrix containing a perspective projection.
		 \details A perspective projection maps view-space coordinates to clip-space coordinates.\n
		 This projection is often used in 3D graphics.

		 \param[in] fov The angle in degrees of the field of view
		 \param[in] aspectRatio The aspect ratio derived from dividing the width of the window by its height
		 \param[in] near The position of the near plane
		 \param[in] far The position of the far plane

		 \return A 4x4 ae::Matrix containing the appropriate perspective projection

		 \par Example:
		 \code
		 ae::Matrix4f persp = ae::Matrix4f::perspective(90.f, windowWidth / windowHeight, -1.f, 1000.f);
		 \endcode

		 \sa orthographic(T, T, T, T, T, T), orthographic(T, T, T, T)

		 \since v0.3.0
		*/
		_NODISCARD static Matrix<T, 4, 4> perspective(T fov, T aspectRatio, T near, T far)
		{
			_CONSTEXPR17 const T T_ONE = static_cast<T>(1);
			_CONSTEXPR17 const T T_TWO = static_cast<T>(2);

			const T TAN_HALF_FOV = Math::tan(Math::toRadians(fov / T_TWO));

			Matrix<T, 4, 4> persp;
			persp.columns[0][0] = T_ONE / (aspectRatio * TAN_HALF_FOV);
			persp.columns[1][1] = T_ONE / TAN_HALF_FOV;
			persp.columns[2][2] = -((far + near) / (far - near));
			persp.columns[2][3] = -T_ONE;
			persp.columns[3][2] = -((T_TWO * far * near) / (far - near));

			return persp;
		}

		/*!
		 \brief Constructs a lookat ae::Matrix.
		 \details A lookat matrix is relatively explicit, it places the camera at a certain position and rotates it to the right direction.

		 \param[in] eye The 3-dimensional ae::Vector containing the origin point of the eye/camera
		 \param[in] target The 3-dimensional ae::Vector containing the position of the point of interest of which the eye will look at
		 \param[in] up The 3-dimensional ae::Vector containing the direction that is considered to be up

		 \return An ae::Matrix containing the appropriate lookat matrix

		 \par Example:
		 \code
		 ae::Matrix4f lookat = ae::Matrix4f::lookat(cameraPosition, targetPosition, ae::Vector3f::Up);
		 \endcode

		 \since v0.4.0
		*/
		_NODISCARD static Matrix<T, 4, 4> lookat(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up) noexcept
		{
			const Vector3<T> FWD = (target - eye).normalize();
			const Vector3<T> SIDE = cross(FWD, up).normalize();
			const Vector3<T> UP = cross(SIDE, FWD);

			Matrix<T, 4, 4> mat = Matrix<T, 4, 4>::identity();
			mat.columns[0][0] = SIDE.x;
			mat.columns[0][1] = UP.x;
			mat.columns[0][2] = -FWD.x;

			mat.columns[1][0] = SIDE.y;
			mat.columns[1][1] = UP.y;
			mat.columns[1][2] = -FWD.y;

			mat.columns[2][0] = SIDE.z;
			mat.columns[2][1] = UP.z;
			mat.columns[2][2] = -FWD.z;

			mat.columns[3][0] = -dot(SIDE, eye);
			mat.columns[3][1] = -dot(UP, eye);
			mat.columns[3][2] = dot(FWD, eye);

			return mat;
		}

		/*!
		 \brief Constructs a translation ae::Matrix.
		 \details A translation matrix is used to move vertices along one or several axes.

		 \param[in] translation A 3-dimensional ae::Vector indicating the displacement of the vertices in the 3 axes.

		 \return A translation ae::Matrix

		 \par Example:
		 \code
		 ae::Matrix4f translation = ae::Matrix4f::translate(ae::Vector3f(0.25f, 0.1f, 0.f));
		 \endcode

		 \sa rotate(), scale()

		 \since v0.3.0
		*/
		_NODISCARD static Matrix<T, 4, 4> translate(const Vector3<T>& translation) noexcept
		{
			Matrix<T, 4, 4> mat = Matrix<T, 4, 4>::identity();
			mat.columns[3][0] = translation.x;
			mat.columns[3][1] = translation.y;
			mat.columns[3][2] = translation.z;

			return mat;
		}
		/*!
		 \brief Constructs a rotation ae::Matrix by providing an angle in radians and the axes of rotation.
		 \details A rotation matrix is used to rotate vertices around the object's origin along one or several axes.
		 \note It's recommended to construct a rotation matrix by providing an ae::Quaternion.

		 \param[in] angle The angle of rotation in radians
		 \param[in] axes A unit (normalized) 3-dimensional ae::Vector containing the axes of rotation

		 \return A rotation ae::Matrix

		 \par Example:
		 \code
		 ae::Matrix4f rotation = ae::Matrix4f::rotate(ae::Math::toRadians(45.f), ae::Vector3f::XAxis);
		 \endcode

		 \sa translate(), scale()

		 \since v0.4.0
		*/
		_NODISCARD static Matrix<T, 4, 4> rotate(float angle, const Vector3<T>& axes) noexcept
		{
			const float COS = Math::cos(angle);
			const float SIN = Math::sin(angle);
			const float OMC = 1.f - COS;

			const float XSIN = axes.x * SIN;
			const float YSIN = axes.y * SIN;
			const float ZSIN = axes.z * SIN;

			const float XOMC = axes.x * OMC;
			const float YOMC = axes.y * OMC;
			const float ZOMC = axes.z * OMC;

			const float XYOMC = axes.x * YOMC;
			const float XZOMC = axes.x * ZOMC;

			const float YZOMC = axes.y * ZOMC;

			Matrix<T, 4, 4> mat = Matrix<T, 4, 4>::identity();
			mat.columns[0][0] = axes.x * XOMC + COS;
			mat.columns[0][1] = XYOMC + ZSIN;
			mat.columns[0][2] = XZOMC - YSIN;

			mat.columns[1][0] = XYOMC - ZSIN;
			mat.columns[1][1] = axes.y * YOMC + COS;
			mat.columns[1][2] = YZOMC + XSIN;

			mat.columns[2][0] = XZOMC + YSIN;
			mat.columns[2][1] = YZOMC - XSIN;
			mat.columns[2][2] = axes.z * ZOMC + COS;

			return mat;
		}
		/*!
		 \brief Constructs a rotation ae::Matrix by providing an ae::Quaternion that will prevent gimbal lock.
		 \details A rotation matrix is used to rotate vertices around the object's origin along one or several axes.\n
		 Gimbal lock occurs once two of the three axes of rotation are situated in a parallel configuration.

		 \param[in] quat An ae::Quaternion that represents the angle and the axes of rotation

		 \return A rotation ae::Matrix

		 \par Example:
		 \code
		 ae::Quaternion rotationQuat = ae::Quaternion::rotation(ae::Math::toRadians(45.f), ae::Vector3f::XAxis);
		 ae::Matrix4f rotationMatrix = ae::Matrix4f::rotate(rotationQuat);
		 \endcode

		 \sa translate(), scale()

		 \since v0.4.0
		*/
		_NODISCARD static Matrix<T, 4, 4> rotate(const Quaternion& quat) noexcept
		{
			const float W = quat.getAngle();
			const Vector3f AXES = quat.getAxes();

			const float WX = W * AXES.x;
			const float WY = W * AXES.y;
			const float WZ = W * AXES.z;

			const float XX = AXES.x * AXES.x;
			const float XY = AXES.x * AXES.y;
			const float XZ = AXES.x * AXES.z;

			const float YY = AXES.y * AXES.y;
			const float YZ = AXES.y * AXES.z;

			const float ZZ = AXES.z * AXES.z;

			Matrix<T, 4, 4> mat = Matrix<T, 4, 4>::identity();
			mat.columns[0][0] = 1.f - 2.f * (YY - ZZ);
			mat.columns[0][1] = 2.f * (XY + WZ);
			mat.columns[0][2] = 2.f * (XZ - WY);

			mat.columns[1][0] = 2.f * (XY - WZ);
			mat.columns[1][1] = 1.f - 2.f * (XX - ZZ);
			mat.columns[1][2] = 2.f * (YZ + WX);

			mat.columns[2][0] = 2.f * (XZ + WY);
			mat.columns[2][1] = 2.f * (YZ - WX);
			mat.columns[2][2] = 1.f - 2.f * (XX - YY);

			return mat;
		}
		/*!
		 \brief Constructs a scale ae::Matrix.
		 \details A scale matrix is used to modify the position of the vertices according to the object's origin.

		 \param[in] scale A 3-dimensional ae::Vector indicating the scaling to perform on the 3 axes.

		 \return A scale ae::Matrix

		 \par Example:
		 \code
		 ae::Matrix4f scale = ae::Matrix4f::scale(ae::Vector3f(0.5f, 0.5f, 0.5f)); // half the size of the objects
		 \endcode

		 \sa translate(), rotate()

		 \since v0.3.0
		*/
		_NODISCARD static Matrix<T, 4, 4> scale(const Vector3<T>& scale) noexcept
		{
			Matrix<T, 4, 4> mat = Matrix<T, 4, 4>::identity();
			mat.columns[0][0] = scale.x;
			mat.columns[1][1] = scale.y;
			mat.columns[2][2] = scale.z;

			return mat;
		}
	};

	// Typedef(s)
	template <typename T>
	using Matrix3 = Matrix<T, 3, 3>;  //!< A 3x3 matrix of type T
	template <typename T>
	using Matrix4 = Matrix<T, 4, 4>;  //!< A 4x4 matrix of type T

	using Matrix3i = Matrix3<int>;    //!< A 3x3 matrix of ints
	using Matrix3f = Matrix3<float>;  //!< A 3x3 matrix of floats
	using Matrix3d = Matrix3<double>; //!< A 3x3 matrix of doubles
	using Matrix4i = Matrix4<int>;    //!< A 4x4 matrix of ints
	using Matrix4f = Matrix4<float>;  //!< A 4x4 matrix of floats
	using Matrix4d = Matrix4<double>; //!< A 4x4 matrix of doubles
}
#endif // Aeon_Math_Matrix_H_

/*!
 \struct ae::Matrix
 \ingroup math

 The ae::Matrix struct is a primary template and with its specialized templates,
 they define a NxM matrix in column-major.
 While variable dimensions are available, the ones most often used will be
 uniform matrices in NxN and more specifically, 3x3 for 2D scenes and 4x4 for
 3D scenes.

 Matrices can be used in the representation of transformations, such as:
 translating, rotating and scaling physical objects, as well as transforming
 one coordinate system to another.

 The elements of the primary template can be accessed through the 'elements'
 member which is a linear array containing all the NxM elements in column-major
 ordering or through the 'columns' member which possesses N ae::Vector of M
 elements each.

 \par Example showing the order of a 4x4 matrix:
 \code
  --------------
 | 0  4   8  12 |
 | 1  5   9  13 |
 | 2  6  10  14 |
 | 3  7  11  15 |
  --------------
 \endcode

 \author Filippos Gleglakos
 \version v0.5.0
 \date 2020.08.07
 \copyright MIT License
*/