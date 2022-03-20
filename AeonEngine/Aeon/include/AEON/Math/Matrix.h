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

#include <AEON/Math/Vector.h>
#include <AEON/Math/internal/Quaternion.h>

namespace ae
{
	/*!
	 \brief The class that represents the primary template for a NxM matrix of type T in column-major.
	 \details The n represents the number of columns and the m represents the number of rows.
	 \note Only arithmetic types are allowed (float, int, etc.) and the minimum number of columns and rows is 2.
	*/
	template <typename T, size_t n, size_t m, typename = std::enable_if_t<std::is_arithmetic_v<T> && (n >= 2 && m >= 2)>>
	class AEON_API Matrix
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Copy constructor.
		 \details Sets the ae::Matrix's elements to the \a copy's elements.

		 \param[in] copy The ae::Matrix of the same type and the same number of elements that will be copied

		 \par Example:
		 \code
		 ae::Matrix4f translation = ae::Matrix4f::translate(ae::Vector3f(0.25f, 0.1f, 0.f));
		 ae::Matrix4f copy(translation);
		 // or
		 ae::Matrix4f copy = translation;
		 \endcode

		 \since v0.7.0
		*/
		constexpr Matrix(const Matrix<T, n, m>& copy) noexcept
			: mElements(copy.mElements)
		{
		}

		/*!
		 \brief Move constructor.
		 \details Performs a move operation on the \a rvalue's elements to the lvalue ae::Matrix's elements.

		 \param[in] rvalue The ae::Matrix rvalue that will be moved

		 \par Example:
		 \code
		 ae::Matrix4f translation = ae::Matrix4f::translate(ae::Vector3f(0.25f, 0.1f, 0.f));
		 ae::Matrix4f copy(std::move(translation));
		 // or
		 ae::Matrix4f copy = std::move(translation);
		 \endcode

		 \since v0.7.0
		*/
		constexpr Matrix(Matrix<T, n, m>&& rvalue) noexcept
			: mElements(std::move(rvalue.mElements))
		{
		}

		// Public operator(s)
		/*!
		 \brief Assignment operator.
		 \details Performs a memberwise assignment from the \a other's elements to the caller ae::Matrix's ones.

		 \param[in] other The ae::Matrix of which its elements will be copied over to the caller's ones

		 \return The caller ae::Matrix containing the new elements

		 \par Example:
		 \code
		 ae::Matrix4f translation = ae::Matrix4f::translate(ae::Vector3f(0.25f, 0.1f, 0.f));
		 ae::Matrix4f translation2 = ae::Matrix4f::translate(ae::Vector3f(5.25f, 0.1f, 20.f));
		 ...
		 translation2 = translation;
		 \endcode

		 \since v0.7.0
		*/
		inline Matrix<T, n, m>& operator=(const Matrix<T, n, m>& other) noexcept
		{
			assert(this != &other);

			mElements = other.mElements;
			return *this;
		}

		/*!
		 \brief Move assignment operator.
		 \details Performs a memberwise move assignment from the \a rvalue's elements to the caller ae::Matrix's ones.

		 \param[in] rvalue The rvalue ae::Matrix of which its elements will be moved over to the caller's ones

		 \return The caller ae::Matrix containing the moved elements

		 \par Example:
		 \code
		 ae::Matrix4f translation = ae::Matrix4f::translate(ae::Vector3f(0.25f, 0.1f, 0.f));
		 ae::Matrix4f rotation = ae::Matrix4f::rotate(ae::Math::radians(45.f), ae::Vector3f::XAxis);
		 ae::Matrix4f transform = rotation * translation;
		 \endcode

		 \since v0.7.0
		*/
		inline Matrix<T, n, m>& operator=(Matrix<T, n, m>&& rvalue) noexcept
		{
			mElements = std::move(rvalue.mElements);
			return *this;
		}

		/*!
		 \brief Equality operator.
		 \details Checks if the caller ae::Matrix's elements are equal to the \a other's elements.

		 \param[in] other The ae::Matrix which its elements will be compared to the caller's ones

		 \return True if all of the \a other's elements are equal to the caller's elements

		 \par Example:
		 \code
		 ae::Matrix4f translation = ae::Matrix4f::translate(ae::Vector3f(0.25f, 0.1f, 0.f));
		 ae::Matrix4f translation2 = ae::Matrix4f::translate(ae::Vector3f(0.25f, 0.1f, 0.f));
		 if (translation == translation2) {
			...
		 }
		 \endcode

		 \sa operator!=()

		 \since v0.7.0
		*/
		[[nodiscard]] inline bool operator==(const Matrix<T, n, m>& other) const noexcept
		{
			for (size_t i = 0; i < mElements.size(); ++i) {
				if (mElements[i] != other.mElements[i]) {
					return false;
				}
			}

			return true;
		}

		/*!
		 \brief Inequality operator.
		 \details Checks if the caller ae::Matrix's elements are inequal to the \a other's elements.

		 \param[in] other The ae::Matrix which its elements will be compared to the caller's ones

		 \return True if at least one of the \a other's elements is inequal to the caller's elements

		 \par Example:
		 \code
		 ae::Matrix4f translation = ae::Matrix4f::translate(ae::Vector3f(0.25f, 0.1f, 0.f));
		 ae::Matrix4f translation2 = ae::Matrix4f::translate(ae::Vector3f(0.25f, 0.1f, 10.f));
		 if (translation != translation2) {
			...
		 }
		 \endcode

		 \sa operator==()

		 \since v0.7.0
		*/
		[[nodiscard]] inline bool operator!=(const Matrix<T, n, m>& other) const noexcept
		{
			return !(*this == other);
		}

		/*!
		 \brief Transformation application operator.
		 \details Applies the transformation \a other to the caller ae::Matrix.\n
		 The resulting matrix will have the same number of rows as the first matrix and the same number of columns as the second matrix.
		 \note The number of columns n of the first matrix must equal the number of rows m of the second matrix.

		 \param[in] other The ae::Matrix that will be applied to the caller ae::Matrix

		 \par Example:
		 \code
		 ae::Matrix4f translation = ae::Matrix4f::translate(ae::Vector3f(0.25f, 0.1f, 0.f));
		 ae::Matrix4f rotation = ae::Matrix4f::rotate(ae::Math::radians(45.f), ae::Vector3f::XAxis);
		 ae::Matrix4f transform = rotation * translation;
		 \endcode

		 \since v0.7.0
		*/
		template <size_t n2, size_t m2>
		[[nodiscard]] constexpr Matrix<T, n2, m> operator*(const Matrix<T, n2, m2>& other) const noexcept
		{
			static_assert(n == m2, "'operator*' is only available for NxM * LxN matrices");

			// Perform the non-commutative multiplication of the two matrices
			Matrix<T, n2, m> mat;
			for (size_t i = 0; i < m; ++i) {
				for (size_t j = 0; j < n2; ++j) {
					for (size_t k = 0; k < n; ++k) {
						mat.mColumns[j][i] += mColumns[k][i] * other.mColumns[j][k];
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

		 \since v0.7.0
		*/
		template <size_t n2, size_t m2>
		inline Matrix<T, n2, m> operator*=(const Matrix<T, n2, m2>& other) noexcept
		{
			static_assert(n == m2, "'operator*=' is only available for NxM * LxN matrices");
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
		 // Square matrix
		 ae::Matrix4f mat4f(0.5f);
		 ae::Vector4f vec4f(0.5f, 0.7f, -1.f, 1.f);
		 ae::Vector4f transformedVec4f = mat4f * vec4f;

		 // Non-square matrix (5 columns, 3 rows)
		 ae::Matrix<float, 5, 3> mat53f(...);
			// In order to perform a matrix-vector multiplication, the vector must have the same number of elements as the matrix has columns (5 in this case)
		 ae::Vector5f vec5f(5.f, 2.f, 4.f, 2.f, 1.f);
			// The transformed vector will have the same number of elements as the matrix has rows (3 in this case)
		 ae::Vector3f vec3f = mat53f * vec5f;
		 \endcode

		 \sa operator*(const Vector<T, n2>&)

		 \since v0.7.0
		*/
		[[nodiscard]] constexpr Vector<T, m> operator*(const Vector<T, n>& vec) const noexcept
		{
			Vector<T, m> result;
			for (size_t i = 0; i < m; ++i) {
				result[i] = dot(getRow(i), vec);
			}

			return result;
		}

		// Public method(s)
		/*!
		 \brief Retrieve the row indicated by the \a index provided of the ae::Matrix.

		 \param[in] index The row to retrieve situated in the range [0 ; m[

		 \return An ae::Vector containing the row's elements

		 \par Example:
		 \code
		 ae::Matrix4f mat4f(1.f);
		 ae::Vector4f secondRow = mat4f.getRow(1);
		 \endcode

		 \since v0.7.0
		*/
		[[nodiscard]] inline Vector<T, n> getRow(size_t index) const noexcept
		{
			assert(index < m);

			// Retrieve the specified row's elements
			Vector<T, n> row;
			for (size_t i = 0; i < n; ++i) {
				row[i] = mElements[index + i * m];
			}

			return row;
		}

		/*!
		 \brief Retrieves a submatrix of the ae::Matrix by deleting the column and the row provided.
		 \details The indicated column and row aren't actually deleted, but replaced with values of 0 to facilitate subsequent calculations.

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

		 \since v0.7.0
		*/
		[[nodiscard]] constexpr Matrix<T, n, m> getSubmatrix(size_t col, size_t row) const noexcept
		{
			Matrix<T, n, m> submatrix;

			size_t i = 0, j = 0;
			for (size_t r = 0; r < m; ++r) {
				for (size_t c = 0; c < n; ++c) {
					// Copy only the elements which aren't in the given column and row
					if (r != row && c != col) {
						submatrix.mColumns[j++][i] = mColumns[c][r];

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

		 \return The scalar value representing the determinant of the ae::Matrix

		 \par Example:
		 \code
		 ae::Matrix4f translation = ae::Matrix4f::translate(ae::Vector3f(0.25f, 0.1f, 0.f));
		 float determinant = translation.getDeterminant();
		 \endcode

		 \since v0.7.0
		*/
		[[nodiscard]] constexpr T getDeterminant() const noexcept
		{
			static_assert(n == m, "'getDeterminant' is only available for square matrices");
			return getDeterminant(n);
		}

		/*!
		 \brief Calculates and retrieves the adjoint matrix of the ae::Matrix.
		 \details The adjoint matrix of a square matrix is the transpose of its cofactor matrix.
		 \note Only square matrices can use this method, meaning NxN matrices (3x3, 4x4, etc.).

		 \return An ae::Matrix containing the adjoint matrix of the ae::Matrix

		 \par Example:
		 \code
		 ae::Matrix4f translation = ae::Matrix4f::translate(ae::Vector3f(0.25f, 0.1f, 0.f));
		 ae::Matrix3f adjoint = translation.getAdjoint();
		 \endcode

		 \since v0.7.0
		*/
		[[nodiscard]] constexpr Matrix<T, n, m> getAdjoint() const noexcept
		{
			static_assert(n == m, "'getAdjoint' is only available for square matrices");

			T sign = static_cast<T>(1);

			Matrix<T, n, m> adjoint;
			for (size_t i = 0; i < n; ++i) {
				for (size_t j = 0; j < n; ++j) {
					// The sign of adj[i][j] is positive if the sum of the row and column indices is even
					sign = ((i + j) % 2 == 0) ? static_cast<T>(1) : -static_cast<T>(1);

					// Interchange rows and columns to get the transpose of the cofactor matrix
					adjoint.mColumns[i][j] = sign * getSubmatrix(j, i).getDeterminant(n - 1);
				}
			}

			return adjoint;
		}

		/*!
		 \brief Calculates and retrieves the inverse of the NxN ae::Matrix.
		 \details The inverse of a NxN matrix can be used to cancel a concatenated transformation in order to retrieve the original transformation.
		 \note Only square matrices can use this method, meaning NxN matrices (3x3, 4x4, etc.).

		 \par Example:
		 \code
		 ae::Matrix4f translation = ae::Matrix4f::translate(ae::Vector3f(0.25f, 0.1f, 0.f));
		 ae::Matrix4f invertedTranslation = translation.invert();
		 \endcode

		 \since v0.7.0
		*/
		[[nodiscard]] constexpr Matrix<T, n, m> invert() const
		{
			static_assert(n == m, "'invert' is only available for square matrices");

			const T DETERMINANT = getDeterminant();
			assert(DETERMINANT != static_cast<T>(0));

			// Calculate the inverse using the following formula: "inv(A) = adj(A) / det(A)"
			const Matrix<T, n, m> ADJOINT = getAdjoint();
			Matrix<T, n, m> inverse;
			for (size_t i = 0; i < n; ++i) {
				for (size_t j = 0; j < n; ++j) {
					inverse.mColumns[j][i] = ADJOINT.mColumns[j][i] / DETERMINANT;
				}
			}

			return inverse;
		}

		/*!
		 \brief Calculates and retrieves the transpose of the ae::Matrix.
		 \details The transpose of a matrix is an operator which flips a matrix over its diagonal.\n
		 It can be used to convert a matrix in column-major into row-major and vice-versa.

		 \return An ae::Matrix containing the transpose of the caller ae::Matrix

		 \par Example:
		 \code
		 ae::Matrix4f columnMajor = ae::Matrix4f::translate(ae::Vector3f(0.25f, 0.1f, 0.f));
		 ae::Matrix4f rowMajor = columnMajor.transpose();
		 \endcode

		 \since v0.7.0
		*/
		[[nodiscard]] constexpr Matrix<T, m, n> transpose() const noexcept
		{
			Matrix<T, m, n> transposedMatrix;
			for (size_t i = 0; i < m; ++i) {
				transposedMatrix.mColumns[i] = getRow(i);
			}

			return transposedMatrix;
		}

		/*!
		 \brief Retrieves the elements of the ae::Matrix.
		 \details This method is mostly necessary for transferring the matrix to the GPU. The API user shouldn't have any need of it in most cases.

		 \return The elements of the matrix

		 \since v0.7.0
		*/
		[[nodiscard]] constexpr const std::array<T, n * m>& getElements() const noexcept
		{
			return mElements;
		}

		// Public static method(s)
		/*!
		 \brief Constructs an ae::Matrix with its diagonally-aligned elements set to the value 1 and its remaining elements to 0.
		 \details This matrix is the base matrix, if this matrix is multiplied with another, it will remain the same.
		 \note Only square matrices can be constructed using this static method, meaning NxN matrices (3x3, 4x4, etc.).

		 \return An ae::Matrix with its diagonally-aligned elements set to 1.

		 \par Example:
		 \code
		 ae::Matrix4f identityMatrix = ae::Matrix4f::identity();
		 \endcode

		 \since v0.7.0
		*/
		[[nodiscard]] inline static Matrix<T, n, m> identity() noexcept
		{
			static_assert(n == m, "'identity' is only available for square matrices");

			Matrix<T, n, m> identityMatrix;
			for (size_t i = 0; i < n; ++i) {
				identityMatrix.mElements[i + i * n] = static_cast<T>(1);
			}

			return identityMatrix;
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

		 \since v0.7.0
		*/
		[[nodiscard]] inline static Matrix<T, 4, 4> orthographic(T left, T right, T bottom, T top, T near, T far)
		{
			constexpr T TWO = static_cast<T>(2);

			const T RMINUSL = right - left;
			const T TMINUSB = top - bottom;
			const T FMINUSN = far - near;

			Matrix<T, 4, 4> ortho = ae::Matrix<T, 4, 4>::identity();
			ortho.mColumns[0][0] = TWO / RMINUSL;
			ortho.mColumns[1][1] = TWO / TMINUSB;
			ortho.mColumns[2][2] = -TWO / FMINUSN;
			ortho.mColumns[3][0] = -((right + left) / RMINUSL);
			ortho.mColumns[3][1] = -((top + bottom) / TMINUSB);
			ortho.mColumns[3][2] = -((far + near) / FMINUSN);

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

		 \since v0.7.0
		*/
		[[nodiscard]] inline static Matrix<T, 4, 4> orthographic(T left, T right, T bottom, T top)
		{
			constexpr T TWO = static_cast<T>(2);

			const T RMINUSL = right - left;
			const T TMINUSB = top - bottom;

			Matrix<T, 4, 4> ortho = ae::Matrix<T, 4, 4>::identity();
			ortho.mColumns[0][0] = TWO / RMINUSL;
			ortho.mColumns[1][1] = TWO / TMINUSB;
			ortho.mColumns[2][2] = -static_cast<T>(1);
			ortho.mColumns[3][0] = -((right + left) / RMINUSL);
			ortho.mColumns[3][1] = -((top + bottom) / TMINUSB);

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

		 \since v0.7.0
		*/
		[[nodiscard]] inline static Matrix<T, 4, 4> perspective(T fov, T aspectRatio, T near, T far)
		{
			constexpr T ONE = static_cast<T>(1);
			constexpr T TWO = static_cast<T>(2);

			const T TAN_HALF_FOV = Math::tan(Math::radians(fov / TWO));

			Matrix<T, 4, 4> persp;
			persp.mColumns[0][0] = ONE / (aspectRatio * TAN_HALF_FOV);
			persp.mColumns[1][1] = ONE / TAN_HALF_FOV;
			persp.mColumns[2][2] = -((far + near) / (far - near));
			persp.mColumns[2][3] = -ONE;
			persp.mColumns[3][2] = -((TWO * far * near) / (far - near));

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

		 \since v0.7.0
		*/
		[[nodiscard]] inline static Matrix<T, 4, 4> lookat(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up) noexcept
		{
			const Vector3<T> FWD = (target - eye).normalize();
			const Vector3<T> SIDE = cross(FWD, up).normalize();
			const Vector3<T> UP = cross(SIDE, FWD);

			Matrix<T, 4, 4> mat = Matrix<T, 4, 4>::identity();
			mat.mColumns[0][0] = SIDE.x;
			mat.mColumns[0][1] = UP.x;
			mat.mColumns[0][2] = -FWD.x;

			mat.mColumns[1][0] = SIDE.y;
			mat.mColumns[1][1] = UP.y;
			mat.mColumns[1][2] = -FWD.y;

			mat.mColumns[2][0] = SIDE.z;
			mat.mColumns[2][1] = UP.z;
			mat.mColumns[2][2] = -FWD.z;

			mat.mColumns[3][0] = -dot(SIDE, eye);
			mat.mColumns[3][1] = -dot(UP, eye);
			mat.mColumns[3][2] = dot(FWD, eye);

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

		 \since v0.7.0
		*/
		[[nodiscard]] inline static Matrix<T, 4, 4> translate(const Vector3<T>& translation) noexcept
		{
			Matrix<T, 4, 4> mat = Matrix<T, 4, 4>::identity();
			mat.mColumns[3][0] = translation.x;
			mat.mColumns[3][1] = translation.y;
			mat.mColumns[3][2] = translation.z;

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
		 ae::Matrix4f rotation = ae::Matrix4f::rotate(ae::Math::radians(45.f), ae::Vector3f::XAxis);
		 \endcode

		 \sa translate(), scale()

		 \since v0.7.0
		*/
		[[nodiscard]] inline static Matrix<T, 4, 4> rotate(float angle, const Vector3<T>& axes) noexcept
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
			mat.mColumns[0][0] = axes.x * XOMC + COS;
			mat.mColumns[0][1] = XYOMC + ZSIN;
			mat.mColumns[0][2] = XZOMC - YSIN;

			mat.mColumns[1][0] = XYOMC - ZSIN;
			mat.mColumns[1][1] = axes.y * YOMC + COS;
			mat.mColumns[1][2] = YZOMC + XSIN;

			mat.mColumns[2][0] = XZOMC + YSIN;
			mat.mColumns[2][1] = YZOMC - XSIN;
			mat.mColumns[2][2] = axes.z * ZOMC + COS;

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
		 ae::Quaternion rotationQuat = ae::Quaternion::rotation(ae::Math::radians(45.f), ae::Vector3f::XAxis);
		 ae::Matrix4f rotationMatrix = ae::Matrix4f::rotate(rotationQuat);
		 \endcode

		 \sa translate(), scale()

		 \since v0.7.0
		*/
		[[nodiscard]] inline static Matrix<T, 4, 4> rotate(const Quaternion& quat) noexcept
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
			mat.mColumns[0][0] = 1.f - 2.f * (YY - ZZ);
			mat.mColumns[0][1] = 2.f * (XY + WZ);
			mat.mColumns[0][2] = 2.f * (XZ - WY);

			mat.mColumns[1][0] = 2.f * (XY - WZ);
			mat.mColumns[1][1] = 1.f - 2.f * (XX - ZZ);
			mat.mColumns[1][2] = 2.f * (YZ + WX);

			mat.mColumns[2][0] = 2.f * (XZ + WY);
			mat.mColumns[2][1] = 2.f * (YZ - WX);
			mat.mColumns[2][2] = 1.f - 2.f * (XX - YY);

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

		 \since v0.7.0
		*/
		[[nodiscard]] inline static Matrix<T, 4, 4> scale(const Vector3<T>& scale) noexcept
		{
			Matrix<T, 4, 4> mat = Matrix<T, 4, 4>::identity();
			mat.mColumns[0][0] = scale.x;
			mat.mColumns[1][1] = scale.y;
			mat.mColumns[2][2] = scale.z;

			return mat;
		}

	private:
		// Private constructor(s)
		/*!
		 \brief Default constructor.
		 \details Sets the elements to the value 0 of the type provided.

		 \since v0.7.0
		*/
		constexpr Matrix() noexcept
			: mElements()
		{
		}

		// Private method(s)
		/*!
		 \brief Calculates and retrieves the determinant of the square ae::Matrix.
		 \details The determinant of a square matrix is a scalar value that encodes certain properties of the linear transformation described by said matrix.
		 \note Only square matrices can use this method, meaning NxN matrices (3x3, 4x4, etc.).

		 \param[in] actualN This parameter is used to count the inner dimensions of the submatrices as this method is recursive

		 \return The scalar value representing the determinant of the ae::Matrix

		 \par Example:
		 \code
		 ae::Matrix4f translation = ae::Matrix4f::translate(ae::Vector3f(0.25f, 0.1f, 0.f));
		 float determinant = translation.getDeterminant();
		 \endcode

		 \since v0.7.0
		*/
		[[nodiscard]] constexpr T getDeterminant(size_t actualN) const noexcept
		{
			// If the matrix only contains a single element, retrieve it
			if (actualN == 1) {
				return mElements[0];
			}

			T determinant = static_cast<T>(0);
			T sign = static_cast<T>(1);

			for (size_t i = 0; i < n; ++i) {
				// Calculate the determinant using the submatrix of Matrix[i][0]
				determinant += sign * mColumns[i][0] * getSubmatrix(i, 0).getDeterminant(actualN - 1);
				sign = -sign;
			}

			return determinant;
		}

		// Private member(s)
		union {
			std::array<T, n * m>        mElements; //!< The NxM elements of the matrix
			std::array<Vector<T, m>, n> mColumns;  //!< The n columns of the matrix
		};
	};

	// Typedef(s)
	template <typename T>
	using Matrix3 = Matrix<T, 3, 3>;  //!< A 3x3 matrix of type T
	template <typename T>
	using Matrix4 = Matrix<T, 4, 4>;  //!< A 4x4 matrix of type T

	using Matrix3i = Matrix3<int32_t>; //!< A 3x3 matrix of ints
	using Matrix3f = Matrix3<float>;   //!< A 3x3 matrix of floats
	using Matrix3d = Matrix3<double>;  //!< A 3x3 matrix of doubles
	using Matrix4i = Matrix4<int32_t>; //!< A 4x4 matrix of ints
	using Matrix4f = Matrix4<float>;   //!< A 4x4 matrix of floats
	using Matrix4d = Matrix4<double>;  //!< A 4x4 matrix of doubles
}

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
 \version v0.7.0
 \date 2021.12.18
 \copyright MIT License
*/