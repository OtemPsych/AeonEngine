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

#ifndef Aeon_Math_Misc_H_
#define Aeon_Math_Misc_H_

#include <cmath>

#include <AEON/Config.h>
#include <AEON/System/DebugLogger.h>

namespace ae
{
	/*!
	 \brief The namespace providing various mathematical functions.
	*/
	namespace Math
	{
		// Template policies
		template <typename T>
		using ARITHMETIC_POLICY = std::enable_if_t<std::is_arithmetic_v<T>>;         //!< The template parameter will only be enabled if it's an arithmetic type
		template <typename T>
		using FLOATING_POINT_POLICY = std::enable_if_t<std::is_floating_point_v<T>>; //!< The template parameter will only be enabled if it's a floating point

		// Constant(s)
		constexpr float PI = 3.14159265358f; //!< The mathematical constant of pi

		/*!
		 \brief Converts the angle provided in \a degrees into radians.
		 \details The formula used is the following: \f$ \theta\,rad = \theta^\circ\cdot\frac{\pi}{180} \f$.
		 \note Only floating point types are allowed (float, double, long double).\n
		 The cast condition won't have an impact on performance during execution.
		 
		 \param[in] degrees The angle to convert to radians

		 \return The angle provided in \a degrees converted into radians.

		 \par Example:
		 \code
		 constexpr float RADIANS = ae::Math::toRadians(60.f);
		 \endcode
		 
		 \sa toDegrees()

		 \since v0.1.0
		*/
		template <typename T, typename = FLOATING_POINT_POLICY<T>>
		[[nodiscard]] constexpr T toRadians(T degrees) noexcept
		{
			return degrees * (PI / 180.f);
		}
		/*!
		 \brief Converts the angle provided in \a radians into degrees.
		 \details The formula used is the following: \f$ \theta^\circ = \theta\,rad \cdot \frac{180}{\pi} \f$.
		 \note Only floating point types are allowed (float, double, long double).\n
		 The cast condition won't have an impact on performance during execution.

		 \param[in] radians The angle to convert to degrees

		 \return The angle provided in \a radians converted into degrees.

		 \par Example:
		 \code
		 constexpr float DEGREES = ae::Math::toDegrees(1.0472f);
		 \endcode

		 \sa toRadians()

		 \since v0.1.0
		*/
		template <typename T, typename = FLOATING_POINT_POLICY<T>>
		[[nodiscard]] constexpr T toDegrees(T radians) noexcept
		{
			return radians * (180.f / PI);
		}

		/*!
		 \brief Retrieves a random value situated between the values \a min and \a max.
		 \details The retrieved value will be of the same type as the values provided.

		 \param[in] min The minimum value
		 \param[in] max The maximum value

		 \return A random value situated between the values \a min and \a max

		 \par Example:
		 \code
		 float randomFloat = ae::Math::random(2.f, 5.f);
		 int randomInt = ae::Math::random(2, 5);
		 \endcode

		 \since v0.1.0
		*/
		template <typename T, typename = ARITHMETIC_POLICY<T>>
		[[nodiscard]] inline T random(T min, T max)
		{
			// Check if the values provided are the same to avoid division by 0 (ignored in Release mode)
			if constexpr (AEON_DEBUG) {
				if (min == max) {
					AEON_LOG_WARNING("Invalid values", "The values provided are equal to one another.");
					return min;
				}
			}

			return min + static_cast<T>(static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / static_cast<float>(max - min)));
		}

		/*!
		 \brief Calculates the cosine of the \a value provided after casting it to a double (if necessary).
		 \note The cast condition won't have an impact on performance during execution.

		 \param[in] value The value of which its cosine will be calculated, only arithmetic types are allowed (float, int, etc.)

		 \return The cosine of the \a value after casting it to its initial type

		 \par Example:
		 \code
		 float cosValue = ae::Math::cos(1.0472f);
		 \endcode

		 \sa sin(), tan(), acos()

		 \since v0.1.0
		*/
		template <typename T, typename = ARITHMETIC_POLICY<T>>
		[[nodiscard]] inline T cos(T value) noexcept
		{
			// Avoid the casts if the T type is already a double during compilation
			if constexpr (std::is_same_v<T, double>) {
				return ::cos(value);
			}
			else {
				return static_cast<T>(::cos(static_cast<double>(value)));
			}
		}
		/*!
		 \brief Calculates the sine of the \a value provided after casting it to a double (if necessary).
		 \note The cast condition won't have an impact on performance during execution.

		 \param[in] value The value of which its sine will be calculated, only arithmetic types are allowed (float, int, etc.)

		 \return The sine of the \a value after casting it to its initial type

		 \par Example:
		 \code
		 float sinValue = ae::Math::sin(1.0472f);
		 \endcode

		 \sa cos(), tan(), asin()

		 \since v0.1.0
		*/
		template <typename T, typename = ARITHMETIC_POLICY<T>>
		[[nodiscard]] inline T sin(T value) noexcept
		{
			// Avoid the casts if the T type is already a double during compilation
			if constexpr (std::is_same_v<T, double>) {
				return ::sin(value);
			}
			else {
				return static_cast<T>(::sin(static_cast<double>(value)));
			}
		}
		/*!
		 \brief Calculates the tangent of the \a value provided after casting it to a double (if necessary).
		 \note The cast condition won't have an impact on performance during execution.

		 \param[in] value The value of which its tangent will be calculated, only arithmetic types are allowed (float, int, etc.)

		 \return The tangent of the \a value after casting it to its initial type

		 \par Example:
		 \code
		 float tanValue = ae::Math::tan(1.0472f);
		 \endcode

		 \sa cos(), sin(), atan(), atan2()

		 \since v0.1.0
		*/
		template <typename T, typename = ARITHMETIC_POLICY<T>>
		[[nodiscard]] inline T tan(T value) noexcept
		{
			// Avoid the casts if the T type is already a double during compilation
			if constexpr (std::is_same_v<T, double>) {
				return ::tan(value);
			}
			else {
				return static_cast<T>(::tan(static_cast<double>(value)));
			}
		}

		/*!
		 \brief Calculates the inverse cosine of the \a value provided after casting it to a double (if necessary).
		 \note The cast condition won't have an impact on performance during execution.

		 \param[in] value The value of which its inverse cosine will be calculated, only floating point types are allowed (float, double, long double)

		 \return The inverse cosine of the \a value after casting it to its initial type

		 \par Example:
		 \code
		 float acosValue = ae::Math::acos(0.5f);
		 \endcode

		 \sa asin(), atan(), cos()

		 \since v0.1.0
		*/
		template <typename T, typename = FLOATING_POINT_POLICY<T>>
		[[nodiscard]] inline T acos(T value) noexcept
		{
			// Avoid the casts if the T type is already a double during compilation
			if constexpr (std::is_same_v<T, double>) {
				return ::acos(value);
			}
			else {
				return static_cast<T>(::acos(static_cast<double>(value)));
			}
		}
		/*!
		 \brief Calculates the inverse sine of the \a value provided after casting it to a double (if necessary).
		 \note The cast condition won't have an impact on performance during execution.

		 \param[in] value The value of which its inverse sine will be calculated, only floating point types are allowed (float, double, long double)

		 \return The inverse sine of the \a value after casting it to its initial type

		 \par Example:
		 \code
		 float asinValue = ae::Math::asin(0.5f);
		 \endcode

		 \sa acos(), atan(), sin()

		 \since v0.1.0
		*/
		template <typename T, typename = FLOATING_POINT_POLICY<T>>
		[[nodiscard]] inline T asin(T value) noexcept
		{
			// Avoid the casts if the T type is already a double during compilation
			if constexpr (std::is_same_v<T, double>) {
				return ::asin(value);
			}
			else {
				return static_cast<T>(::asin(static_cast<double>(value)));
			}
		}
		/*!
		 \brief Calculates the inverse tangent of the \a value provided after casting it to a double (if necessary).
		 \note The cast condition won't have an impact on performance during execution.

		 \param[in] value The value of which its inverse tangent will be calculated, only arithmetic types are allowed (float, int, etc.)

		 \return The inverse tangent of the \a value after casting it to its initial type

		 \par Example:
		 \code
		 float atanValue = ae::Math::atan(0.5f);
		 \endcode

		 \sa acos(), asin(), atan2(), tan()

		 \since v0.1.0
		*/
		template <typename T, typename = ARITHMETIC_POLICY<T>>
		[[nodiscard]] inline T atan(T value) noexcept
		{
			// Avoid the casts if the T type is already a double during compilation
			if constexpr (std::is_same_v<T, double>) {
				return ::atan(value);
			}
			else {
				return static_cast<T>(::atan(static_cast<double>(value)));
			}
		}
		/*!
		 \brief Calculates the inverse tangent of the \a v1 and \a v2 values after casting them to doubles (if necessary).
		 \details This functions permits the calculation of a signed angle in radians (possibility to have a negative angle) and protection against division by 0.
		 \note The cast condition won't have an impact on performance during execution.

		 \param[in] v1 The first value of which its inverse tangent will be calculated, only arithmetic types are allowed (float, int, etc.)
		 \param[in] v2 The second value of which its inverse tangent will be calculated, only arithmetic types are allowed (float, int, etc.)

		 \return The inverse tangent of the \a v1 and \a v2 values after casting it to the values' initial type

		 \par Example:
		 \code
		 float angle = ae::Math::atan2(mouseY - objectY, mouseX - objectX);
		 \endcode

		 \sa atan()

		 \since v0.1.0
		*/
		template <typename T, typename = ARITHMETIC_POLICY<T>>
		[[nodiscard]] inline T atan2(T v1, T v2) noexcept
		{
			// Avoid the casts if the T type is already a double during compilation
			if constexpr (std::is_same_v<T, double>) {
				return ::atan2(v1, v2);
			}
			else {
				return static_cast<T>(::atan2(static_cast<double>(v1), static_cast<double>(v2)));
			}
		}

		/*!
		 \brief Calculates the square root of the \a value provided after casting it to a double (if necessary).
		 \note The cast condition won't have an impact on performance during execution.

		 \param[in] value The value of which its square root will be calculated, only arithmetic types are allowed (float, int, etc.)

		 \return The square root of the \a value after casting it to its initial type

		 \par Example:
		 \code
		 float sqrtValue = ae::Math::sqrt(5.f);
		 \endcode

		 \sa rsqrt()

		 \since v0.1.0
		*/
		template <typename T, typename = ARITHMETIC_POLICY<T>>
		[[nodiscard]] inline T sqrt(T value)
		{
			// Avoid the casts if the T type is already a double during compilation
			if constexpr (std::is_same_v<T, double>) {
				return ::sqrt(value);
			}
			else {
				return static_cast<T>(::sqrt(static_cast<double>(value)));
			}
		}
		/*!
		 \brief Calculates the inverse square root of the \a value provided after casting it to a double (if necessary).
		 \note The cast condition won't have an impact on performance during execution.

		 \param[in] value The value of which its inverse square root will be calculated, only arithmetic types are allowed (float, int, etc.)

		 \return The inverse square root of the \a value after casting it to its initial type

		 \par Example:
		 \code
		 float rsqrtValue = ae::Math::rsqrt(5.f);
		 \endcode

		 \sa sqrt()

		 \since v0.1.0
		*/
		template <typename T, typename = ARITHMETIC_POLICY<T>>
		[[nodiscard]] inline T rsqrt(T value)
		{
			// Avoid the casts if the T type is already a double during compilation
			if constexpr (std::is_same_v<T, double>) {
				return (1.0 / ::sqrt(value));
			}
			else {
				return static_cast<T>(1.0 / ::sqrt(static_cast<double>(value)));
			}
		}

		/*!
		 \brief Retrieves the rounded down value to the nearest integer of the \a value provided.
		 
		 \param[in] value The value that will be rounded down to the nearest integer, only floating point types are allowed (float, double, long double)

		 \return The rounded down value to the nearest integer of the \a value provided.

		 \par Example:
		 \code
		 float floorValue = ae::Math::floor(5.25f); // 5.f
		 \endcode

		 \sa ceil()

		 \since v0.1.0
		*/
		template <typename T, typename = FLOATING_POINT_POLICY<T>>
		[[nodiscard]] inline T floor(T value) noexcept
		{
			// Avoid the casts if the T type is already a double during compilation
			if constexpr (std::is_same_v<T, double>) {
				return ::floor(value);
			}
			else {
				return static_cast<T>(::floor(static_cast<double>(value)));
			}
		}
		/*!
		 \brief Retrieves the rounded up value to the nearest integer of the \a value provided.

		 \param[in] value The value that will be rounded up to the nearest integer, only floating point types are allowed (float, double, long double)

		 \return The rounded up value to the nearest integer of the \a value provided.

		 \par Example:
		 \code
		 float ceilValue = ae::Math::ceil(5.25f); // 6.f
		 \endcode

		 \sa floor()

		 \since v0.1.0
		*/
		template <typename T, typename = FLOATING_POINT_POLICY<T>>
		[[nodiscard]] inline T ceil(T value) noexcept
		{
			// Avoid the casts if the T type is already a double during compilation
			if constexpr (std::is_same_v<T, double>) {
				return ::ceil(value);
			}
			else {
				return static_cast<T>(::ceil(static_cast<double>(value)));
			}
		}

		/*!
		 \brief Retrieves the minimum value of the \a v1 and \a v2 values provided.
		 \note Only arithmetic types are allowed (float, int, etc.).

		 \param[in] v1 The first value that will be compared with the \a v2 value
		 \param[in] v2 The second value that will be compared with the \a v1 value

		 \return The minimum value between the two values provided

		 \par Example:
		 \code
		 constexpr float MIN_VALUE = ae::Math::min(5.f, 0.3f); // 0.3f
		 \endcode

		 \sa max(), clamp()

		 \since v0.1.0
		*/
		template <typename T, typename = ARITHMETIC_POLICY<T>>
		[[nodiscard]] constexpr T min(T v1, T v2) noexcept
		{
			return (v1 < v2) ? v1 : v2;
		}
		/*!
		 \brief Retrieves the maximum value of the \a v1 and \a v2 values provided.
		 \note Only arithmetic types are allowed (float, int, etc.).

		 \param[in] v1 The first value that will be compared with the \a v2 value
		 \param[in] v2 The second value that will be compared with the \a v1 value

		 \return The maximum value between the two values provided

		 \par Example:
		 \code
		 constexpr float MAX_VALUE = ae::Math::max(5.f, 0.3f); // 5.f
		 \endcode

		 \sa min(), clamp()

		 \since v0.1.0
		*/
		template <typename T, typename = ARITHMETIC_POLICY<T>>
		[[nodiscard]] constexpr T max(T v1, T v2) noexcept
		{
			return (v1 > v2) ? v1 : v2;
		}
		/*!
		 \brief Retrieves a value situated between the range [\a minValue,\a maxValue].
		 \details If the \a value is less than the \a minValue, the value retrieved will be equal to \a minValue.\n
		 If the \a value is situated between the range [\a minValue,\a maxValue], the value retrieved will be equal to \a value.\n
		 If the \a value is greater than the \a maxValue, the value retrieved will be equal to \a maxValue.
		 \note Only arithmetic types are allowed (float, int, etc.).
		 
		 \param[in] value The value to check if it's situated between the range [\a minValue,\a maxValue]
		 \param[in] minValue The minimum value allowed
		 \param[in] maxValue The maximum value allowed

		 \return A value situated between the range [\a minValue,\a maxValue]

		 \par Example:
		 \code
		 constexpr float MIN_VALUE = 0.3f;
		 constexpr float MAX_VALUE = 0.8f;
		 constexpr float VALUE = 0.2f;
		 constexpr float CLAMPED_VALUE = ae::Math::clamp(VALUE, MIN_VALUE, MAX_VALUE); // 0.3f
		 \endcode

		 \sa min(), max()

		 \since v0.1.0
		*/
		template <typename T, typename = ARITHMETIC_POLICY<T>>
		[[nodiscard]] constexpr T clamp(T value, T minValue, T maxValue) noexcept
		{
			return min(max(value, minValue), maxValue);
		}

		/*!
		 \brief Retrieves an interpolated value situated between the range [\a start,\a end] based on the \a t percentage.
		 \details Lerp or mix is the basic linear interpolation function.\n
		 If \a t is less than 0, the interpolated value will be situated before the \a start value.\n
		 If \a t is 0, the interpolated value will be equal to the \a start value.\n
		 If \a t is between 0 and 1, the interpolated value will be situated between the range [\a start,\a end].\n
		 If \a is 1, the interpolated value will be equal to the \a end value.\n
		 If \a is greater than 1, the interpolated value will be situated after the \a end value.
		 \note Only arithmetic types are allowed (float, int, etc.).

		 \param[in] start The value indicating the minimum starting value
		 \param[in] end The value indicating the maximum ending value
		 \param[in] t The unit value (between the range [0,1]) that will act as a percentage for the interpolation

		 \return An interpolated value situated between the range [\a start,\a end] based on the \a t percentage

		 \par Example:
		 \code
		 constexpr float START = 5.f;
		 constexpr float END = 20.f;
		 constexpr float LERP = ae::Math::lerp(START, END, 0.5f);
		 \endcode

		 \since v0.1.0
		*/
		template <typename T, typename = ARITHMETIC_POLICY<T>>
		[[nodiscard]] constexpr T lerp(T start, T end, T t) noexcept
		{
			return (start + t * (end - start));
		}
	}
}
#endif // Aeon_Math_Misc_H_

/*!
 \namespace ae::Math
 \ingroup math

 The namespace ae::Math provides utility functions that may prove useful during
 the development of a game, whether it be 2D or 3D.
 
 \author Filippos Gleglakos
 \version v0.1.0
 \date 2019-06-14
 \copyright MIT License
*/