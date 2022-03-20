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

#include <cmath>
#include <cassert>

#include <AEON/Config.h>

namespace ae
{
	/*!
	 \brief The namespace providing various mathematical utility functions.
	*/
	namespace Math
	{
		/*!
		 \brief Retrieves the value of pi to the accuracy of the type provided.
		 \details If a type isn't explicitly stated, the accuracy of a float is retrieved by default.
		 \note Only floating point types are allowed: float, double, long double.

		 \return The value of pi to the accuracy of the type provided

		 \par Example:
		 \code
		 float pi = ae::Math::pi();                // float accuracy by default
		 float piFloat = ae::Math::pi<float>();    // explicit float accuracy
		 double piDouble = ae::Math::pi<double>(); // explicit double accuracy
		 \endcode

		 \since v0.7.0
		*/
		template <typename T = float>
		[[nodiscard]] AEON_API constexpr T pi()
		{
			static_assert(std::is_floating_point_v<T>, "'pi()' only accepts floating point types");
			return static_cast<T>(3.14159265358979323846264338327950288L);
		}

		/*!
		 \brief Converts the angle provided in \a degrees into radians.
		 \note Only floating point types are allowed: float, double, long double.
		 
		 \param[in] degrees The angle to convert to radians

		 \return The angle provided in \a degrees converted to radians.

		 \par Example:
		 \code
		 double radiansDbl = ae::Math::radians(60.0); // explicit double accuracy
		 float radiansFloat = ae::Math::radians(60);  // implicit float accuracy
		 \endcode
		 
		 \sa degrees()

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T radians(T degrees)
		{
			static_assert(std::is_floating_point_v<T>, "'radians' only accepts floating point types");
			return degrees * (pi<T>() / static_cast<T>(180.f));
		}

		/*!
		 \brief Converts the angle provided in \a radians into degrees.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] radians The angle to convert to degrees

		 \return The angle provided in \a radians converted to degrees.

		 \par Example:
		 \code
		 float degrees = ae::Math::degrees(1.0472f);
		 \endcode

		 \sa radians()

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T degrees(T radians)
		{
			static_assert(std::is_floating_point_v<T>, "'degrees' only accepts floating point types");
			return radians * (static_cast<T>(180.0) / pi<T>());
		}

		/*!
		 \brief Retrieves a random value in the range [min ; max[.
		 \note A seed must be set using srand for a different series of numbers at each execution.\n
		 Only arithmetic types are allowed: short, int, float, double, etc.

		 \param[in] min The minimum value (inclusive)
		 \param[in] max The maximum value (non-inclusive)

		 \return A random value in the range [min ; max[

		 \par Example:
		 \code
		 float randomFloat = ae::Math::random(2.f, 5.f);
		 int randomInt = ae::Math::random(2, 5);
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API T random(T min, T max)
		{
			static_assert(std::is_arithmetic_v<T>, "'random' only accepts arithmetic types");
			assert(min < max);

			// Return a random number in the range [min ; max[
			const auto DIVIDEND = static_cast<float>(RAND_MAX) / (max - min);
			return min + static_cast<T>(rand() / DIVIDEND);
		}

		/*!
		 \brief Calculates the cosine of the \a value provided.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] value The value of which its cosine will be calculated

		 \return The cosine of the \a value after casting it to its initial type

		 \par Example:
		 \code
		 float cosValue = ae::Math::cos(1.0472f);
		 \endcode

		 \sa sin(), tan(), acos()

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API inline T cos(T value)
		{
			static_assert(std::is_floating_point_v<T>, "'cos' only accepts floating point types");
			return static_cast<T>(::cos(static_cast<double>(value)));
		}

		/*!
		 \brief Calculates the sine of the \a value provided.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] value The value of which its sine will be calculated

		 \return The sine of the \a value after casting it to its initial type

		 \par Example:
		 \code
		 float sinValue = ae::Math::sin(1.0472f);
		 \endcode

		 \sa cos(), tan(), asin()

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API inline T sin(T value)
		{
			static_assert(std::is_floating_point_v<T>, "'sin' only accepts floating point types");
			return static_cast<T>(::sin(static_cast<double>(value)));
		}

		/*!
		 \brief Calculates the tangent of the \a value provided.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] value The value of which its tangent will be calculated

		 \return The tangent of the \a value after casting it to its initial type

		 \par Example:
		 \code
		 float tanValue = ae::Math::tan(1.0472f);
		 \endcode

		 \sa cos(), sin(), atan(), atan2()

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API inline T tan(T value)
		{
			static_assert(std::is_floating_point_v<T>, "'tan' only accepts floating point types");
			return static_cast<T>(::tan(static_cast<double>(value)));
		}

		/*!
		 \brief Calculates the inverse cosine of the \a value provided.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] value The value of which its inverse cosine will be calculated

		 \return The inverse cosine of the \a value after casting it to its initial type

		 \par Example:
		 \code
		 float acosValue = ae::Math::acos(0.5f);
		 \endcode

		 \sa asin(), atan(), cos()

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API inline T acos(T value)
		{
			static_assert(std::is_floating_point_v<T>, "'acos' only accepts floating point types");
			return static_cast<T>(::acos(static_cast<double>(value)));
		}

		/*!
		 \brief Calculates the inverse sine of the \a value provided.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] value The value of which its inverse sine will be calculated

		 \return The inverse sine of the \a value after casting it to its initial type

		 \par Example:
		 \code
		 float asinValue = ae::Math::asin(0.5f);
		 \endcode

		 \sa acos(), atan(), sin()

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API inline T asin(T value)
		{
			static_assert(std::is_floating_point_v<T>, "'asin' only accepts floating point types");
			return static_cast<T>(::asin(static_cast<double>(value)));
		}

		/*!
		 \brief Calculates the inverse tangent of the \a value provided.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] value The value of which its inverse tangent will be calculated

		 \return The inverse tangent of the \a value after casting it to its initial type

		 \par Example:
		 \code
		 float atanValue = ae::Math::atan(0.5f);
		 \endcode

		 \sa acos(), asin(), atan2(), tan()

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API inline T atan(T value)
		{
			static_assert(std::is_floating_point_v<T>, "'atan' only accepts floating point types");
			return static_cast<T>(::atan(static_cast<double>(value)));
		}

		/*!
		 \brief Calculates the inverse tangent of the \a v1 and \a v2 values.
		 \details This functions permits the calculation of a signed angle in radians (possibility to have a negative angle) and protection against division by 0.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] v1 The first value of which its inverse tangent will be calculated
		 \param[in] v2 The second value of which its inverse tangent will be calculated

		 \return The inverse tangent of the \a v1 and \a v2 values after casting it to the values' initial type

		 \par Example:
		 \code
		 float angle = ae::Math::atan2(mouseY - objectY, mouseX - objectX);
		 \endcode

		 \sa atan()

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API inline T atan2(T v1, T v2)
		{
			static_assert(std::is_floating_point_v<T>, "'atan2' only accepts floating point types");
			return static_cast<T>(::atan2(static_cast<double>(v1), static_cast<double>(v2)));
		}

		/*!
		 \brief Calculates the value of the \a base to the \power provided.
		 \note Only arithmetic types are allowed: short, int, float, double, etc.

		 \param[in] base The value that will be the base of the operation
		 \param[in] power The value that will be the power of the operation

		 \return The \a base to the \a power provided after casting it to the values' initial type

		 \par Example:
		 \code
		 float squared = ae::Math::pow(5, 2);
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API inline T pow(T base, T power)
		{
			static_assert(std::is_arithmetic_v<T>, "'pow' only accepts arithmetic types");
			return static_cast<T>(::pow(static_cast<double>(base), static_cast<double>(power)));
		}

		/*!
		 \brief Calculates the square root of the \a value provided.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] value The value of which its square root will be calculated

		 \return The square root of the \a value after casting it to its initial type

		 \par Example:
		 \code
		 float sqrtValue = ae::Math::sqrt(5.f);
		 \endcode

		 \sa rsqrt()

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API inline T sqrt(T value)
		{
			static_assert(std::is_floating_point_v<T>, "'sqrt' only accepts floating point types");
			return static_cast<T>(::sqrt(static_cast<double>(value)));
		}

		/*!
		 \brief Calculates the inverse square root of the \a value provided after casting it to a double (if necessary).
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] value The value of which its inverse square root will be calculated

		 \return The inverse square root of the \a value after casting it to its initial type

		 \par Example:
		 \code
		 float rsqrtValue = ae::Math::rsqrt(5.f);
		 \endcode

		 \sa sqrt()

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API inline T rsqrt(T value)
		{
			static_assert(std::is_floating_point_v<T>, "'rsqrt' only accepts floating point types");
			assert(value != static_cast<T>(0));

			return static_cast<T>(1.0 / ::sqrt(static_cast<double>(value)));
		}

		/*!
		 \brief Retrieves the rounded down value to the nearest integer of the \a value provided.
		 \note Only floating point types are allowed: float, double, long double.
		 
		 \param[in] value The value that will be rounded down to the nearest integer

		 \return The rounded down value to the nearest integer of the \a value provided.

		 \par Example:
		 \code
		 float floorValue = ae::Math::floor(5.25f); // 5.f
		 \endcode

		 \sa ceil()

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API inline T floor(T value)
		{
			static_assert(std::is_floating_point_v<T>, "'floor' only accepts floating point types");
			return static_cast<T>(::floor(static_cast<double>(value)));
		}

		/*!
		 \brief Retrieves the rounded up value to the nearest integer of the \a value provided.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] value The value that will be rounded up to the nearest integer

		 \return The rounded up value to the nearest integer of the \a value provided.

		 \par Example:
		 \code
		 float ceilValue = ae::Math::ceil(5.25f); // 6.f
		 \endcode

		 \sa floor()

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API inline T ceil(T value)
		{
			static_assert(std::is_floating_point_v<T>, "'ceil' only accepts floating point types");
			return static_cast<T>(::ceil(static_cast<double>(value)));
		}

		/*!
		 \brief Retrieves the minimum value of the values provided.
		 \note Only arithmetic types are allowed: int, float, etc.

		 \param[in] a The first value to compare
		 \param[in] b The second value to compare

		 \return The minimum value

		 \par Example:
		 \code
		 constexpr float MIN_VALUE = ae::Math::min(5.f, 0.3f); // 0.3f
		 \endcode

		 \sa max(), clamp()

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T min(T a, T b) noexcept
		{
			static_assert(std::is_arithmetic_v<T>, "'min' only accepts arithmetic types");
			return (a < b) ? a : b;
		}

		/*!
		 \brief Retrieves the maximum value of the values provided.
		 \note Only arithmetic types are allowed: int, float, etc.

		 \param[in] a The first value to compare
		 \param[in] b The second value to compare

		 \return The maximum value

		 \par Example:
		 \code
		 constexpr float MAX_VALUE = ae::Math::max(5.f, 0.3f); // 5.f
		 \endcode

		 \sa min(), clamp()

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T max(T a, T b) noexcept
		{
			static_assert(std::is_arithmetic_v<T>, "'max' only accepts arithmetic types");
			return (a > b) ? a : b;
		}

		/*!
		 \brief Retrieves a value clamped in the range [minValue ; maxValue].
		 \details If the \a value is less than \a minValue, the value retrieved will be \a minValue.\n
		 If the \a value is in the range [\a minValue ; \a maxValue], the value retrieved will be \a value.\n
		 If the \a value is greater than \a maxValue, the value retrieved will be \a maxValue.
		 \note Only arithmetic types are allowed: int, float, etc.

		 \param[in] value The value to clamp in the range [minValue ; maxValue]
		 \param[in] minValue The minimum value allowed
		 \param[in] maxValue The maximum value allowed

		 \return A value in the range [minValue ; maxValue]

		 \par Example:
		 \code
		 constexpr float MIN_VALUE = 0.3f;
		 constexpr float MAX_VALUE = 0.8f;
		 constexpr float VALUE = 0.2f;
		 constexpr float CLAMPED_VALUE = ae::Math::clamp(VALUE, MIN_VALUE, MAX_VALUE); // 0.3f
		 \endcode

		 \sa min(), max()

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T clamp(T value, T minValue, T maxValue) noexcept
		{
			static_assert(std::is_arithmetic_v<T>, "'clamp' only accepts arithmetic types");
			return min(max(value, minValue), maxValue);
		}

		/*!
		 \brief Retrieves the interpolated value in the range [start ; end] based on the percentage provided.
		 \details Lerp (also known as mix) is the basic linear interpolation function.\n
		 If \a t is less than 0, the interpolated value will be situated before the \a start value.\n
		 If \a t is 0, the interpolated value will be \a start.\n
		 If \a t is between 0 and 1, the interpolated value will be in the range [\a start ; \a end].\n
		 If \a t is 1, the interpolate value will be \a end.\n
		 If \a t is greater than 1, the interpolated value will be situated after the \a end value.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] start The value indicating the minimum starting value
		 \param[in] end The value indicating the maximum ending value
		 \param[in] t The value in the range [0 ; 1] that will act as a percentage for the interpolation

		 \return An interpolated value in the range [start ; end] based on the percentage

		 \par Example:
		 \code
		 constexpr float START = 5.f;
		 constexpr float END = 20.f;
		 constexpr float LERP = ae::Math::lerp(START, END, 0.5f);
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T lerp(T start, T end, T t) noexcept
		{
			static_assert(std::is_floating_point_v<T>, "'lerp' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			return start + t * (end - start);
		}

		/*!
		 \brief Smoothstep is a technique that will interpolate between two edges based on the \a value provided.
		 \details It applies a curve function so the movement starts out slow, speeds up in the middle, then slows down again.\n
		 The value retrieved will represent the interpolation factor in the range [0;1].
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] edge1 The first edge (the minimum point of interpolation)
		 \param[in] edge2 The second ege (the maximum point of interpolation)

		 \return An interpolation factor of the \a value based on the edges

		 \par Example:
		 \code
		 float edge1 = 0.5f;
		 float edge2 = 0.65f;
		 float value = 0.5f;
		 float interpolationFactor = ae::smoothstep(edge1, edge2, value);

		 // In order to retrieve the interpolated value between the edges:
		 float interpolatedValue = edge1 + interpolationFactor * (edge2 - edge1);
		 // or
		 float interpolatedValue = ae::lerp(edge1, edge2, interpolationFactor);
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T smoothstep(T edge1, T edge2, T value)
		{
			static_assert(std::is_floating_point_v<T>, "'smoothstep' only accepts floating point types");
			assert(edge1 <= edge2);
			
			const T ERP = clamp((value - edge1) / (edge2 - edge1), static_cast<T>(0), static_cast<T>(1));
			return ERP * ERP * (static_cast<T>(3) - static_cast<T>(2) * ERP);
		}

		/*!
		 \brief Retrieves the eased value using an InSine function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeInSine((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API inline T easeInSine(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeInSine' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			return static_cast<T>(1) - cos(t * pi<T>() / static_cast<T>(2));
		}
		/*!
		 \brief Retrieves the eased value using an OutSine function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeOutSine((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API inline T easeOutSine(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeOutSine' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			return sin(t * pi<T>() / static_cast<T>(2));
		}
		/*!
		 \brief Retrieves the eased value using an InOutSine function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeInOutSine((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API inline T easeInOutSine(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeInOutSine' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			return -(cos(pi<T>() * t) - static_cast<T>(1)) / static_cast<T>(2);
		}
		/*!
		 \brief Retrieves the eased value using an InQuad function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeInQuad((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T easeInQuad(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeInQuad' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			return t * t;
		}
		/*!
		 \brief Retrieves the eased value using an OutQuad function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeOutQuad((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T easeOutQuad(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeOutQuad' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T ONE = static_cast<T>(1);

			const T A = ONE - t;
			return ONE - A * A;
		}
		/*!
		 \brief Retrieves the eased value using an InOutQuad function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeInOutQuad((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T easeInOutQuad(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeInOutQuad' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T TWO = static_cast<T>(2);

			const T A = -TWO * t + TWO;
			return (t < static_cast<T>(0.5)) ? (TWO * t * t) : (static_cast<T>(1) - A * A / TWO);
		}
		/*!
		 \brief Retrieves the eased value using an InCubic function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeInCubic((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T easeInCubic(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeInCubic' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			return t * t * t;
		}
		/*!
		 \brief Retrieves the eased value using an OutCubic function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeOutCubic((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T easeOutCubic(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeOutCubic' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T ONE = static_cast<T>(1);

			const T A = ONE - t;
			return ONE - A * A * A;
		}
		/*!
		 \brief Retrieves the eased value using an InOutCubic function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeInOutCubic((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T easeInOutCubic(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeInOutCubic' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T TWO = static_cast<T>(2);

			const T A = -TWO * t + TWO;
			return (t < static_cast<T>(0.5)) ? (static_cast<T>(4) * t * t * t) : (static_cast<T>(1) - A * A * A / TWO);
		}
		/*!
		 \brief Retrieves the eased value using an InQuart function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeInQuart((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T easeInQuart(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeInQuart' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			return (t *= t) * t;
		}
		/*!
		 \brief Retrieves the eased value using an OutQuart function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeOutQuart((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T easeOutQuart(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeOutQuart' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T ONE = static_cast<T>(1);

			T a = ONE - t;
			return ONE - (a *= a) * a;
		}
		/*!
		 \brief Retrieves the eased value using an InOutQuart function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeInOutQuart((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T easeInOutQuart(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeInOutQuart' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T TWO = static_cast<T>(2);

			const T A = t * t;
			T b = -TWO * t + TWO;
			return (t < static_cast<T>(0.5)) ? (static_cast<T>(8) * A * A) : (static_cast<T>(1) - (b *= b) * b / TWO);
		}
		/*!
		 \brief Retrieves the eased value using an InQuint function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeInQuint((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T easeInQuint(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeInQuint' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			const T A = t * t;
			return A * A * t;
		}
		/*!
		 \brief Retrieves the eased value using an OutQuint function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeOutQuint((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T easeOutQuint(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeOutQuint' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T ONE = static_cast<T>(1);

			const T A = ONE - t;
			const T B = A * A;
			return ONE - B * B * A;
		}
		/*!
		 \brief Retrieves the eased value using an InOutQuint function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeInOutQuint((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T easeInOutQuint(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeInOutQuint' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T TWO = static_cast<T>(2);

			const T A = t * t;
			const T B = -TWO * t + TWO;
			const T C = B * B;
			return (t < static_cast<T>(0.5)) ? (static_cast<T>(16) * A * A * t) : (static_cast<T>(1) - C * C * B / TWO);
		}
		/*!
		 \brief Retrieves the eased value using an InExpo function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeInExpo((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API inline T easeInExpo(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeInExpo' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T ZERO = static_cast<T>(0);
			constexpr T TEN = static_cast<T>(10);

			return (t == ZERO) ? ZERO : (pow(static_cast<T>(2), TEN * t - TEN));
		}
		/*!
		 \brief Retrieves the eased value using an OutExpo function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeOutExpo((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API inline T easeOutExpo(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeOutExpo' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T ONE = static_cast<T>(1);

			return (t == ONE) ? ONE : (ONE - pow(static_cast<T>(2), -static_cast<T>(10) * t));
		}
		/*!
		 \brief Retrieves the eased value using an InOutExpo function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeInOutExpo((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API T easeInOutExpo(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeInOutExpo' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T ZERO = static_cast<T>(0);
			constexpr T ONE = static_cast<T>(1);
			constexpr T TWO = static_cast<T>(2);
			constexpr T TEN = static_cast<T>(10);
			constexpr T TWENTY = static_cast<T>(20);

			if (t == ZERO) {
				return ZERO;
			}
			if (t == ONE) {
				return ONE;
			}
			if (t < static_cast<T>(0.5)) {
				return pow(TWO, TWENTY * t - TEN) / TWO;
			}

			return (TWO - pow(TWO, -TWENTY * t + TEN)) / TWO;
		}
		/*!
		 \brief Retrieves the eased value using an InCirc function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeInCirc((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API inline T easeInCirc(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeInCirc' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T ONE = static_cast<T>(1);

			return ONE - sqrt(ONE - t * t);
		}
		/*!
		 \brief Retrieves the eased value using an OutCirc function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeOutCirc((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API inline T easeOutCirc(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeOutCirc' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T ONE = static_cast<T>(1);

			const T A = t - ONE;
			return sqrt(ONE - A * A);
		}
		/*!
		 \brief Retrieves the eased value using an InOutCirc function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeInOutCirc((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API inline T easeInOutCirc(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeInOutCirc' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T ONE = static_cast<T>(1);
			constexpr T TWO = static_cast<T>(2);

			const T A = TWO * t;
			const T B = -TWO * t + TWO;
			return ((t < static_cast<T>(0.5)) ? (ONE - sqrt(ONE - A * A)) : (sqrt(ONE - B * B) + ONE)) / TWO;
		}
		/*!
		 \brief Retrieves the eased value using an InBack function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeInBack((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T easeInBack(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeInBack' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			return t * t * (static_cast<T>(2.70158) * t - static_cast<T>(1.70158));
		}
		/*!
		 \brief Retrieves the eased value using an OutBack function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeOutBack((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T easeOutBack(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeOutBack' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T ONE = static_cast<T>(1);

			const T A = t - ONE;
			const T B = A * A;
			return ONE + B * (static_cast<T>(2.70158) * A + static_cast<T>(1.70158));
		}
		/*!
		 \brief Retrieves the eased value using an InOutBack function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeInOutBack((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T easeInOutBack(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeInOutBack' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T TWO = static_cast<T>(2);
			constexpr T VAL1 = static_cast<T>(3.5949095);
			constexpr T VAL2 = static_cast<T>(2.5949095);

			const T A = TWO * t;
			const T B = A - TWO;
			return ((t < static_cast<T>(0.5)) ? (A * A * (VAL1 * A - VAL2)) : (B * B * (VAL1 * B + VAL2) + TWO)) / TWO;
		}
		/*!
		 \brief Retrieves the eased value using an InElastic function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeInElastic((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API T easeInElastic(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeInElastic' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T ZERO = static_cast<T>(0);
			constexpr T ONE = static_cast<T>(1);
			constexpr T TWO = static_cast<T>(2);
			constexpr T TEN = static_cast<T>(10);

			if (t == ZERO) {
				return ZERO;
			}
			if (t == ONE) {
				return ONE;
			}

			const T A = TEN * t;
			return -pow(TWO, A - TEN) * sin((A - static_cast<T>(10.75)) * TWO * pi<T>() / static_cast<T>(3));
		}
		/*!
		 \brief Retrieves the eased value using an OutElastic function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeOutElastic((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API T easeOutElastic(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeOutElastic' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T ZERO = static_cast<T>(0);
			constexpr T ONE = static_cast<T>(1);
			constexpr T TWO = static_cast<T>(2);

			if (t == ZERO) {
				return ZERO;
			}
			if (t == ONE) {
				return ONE;
			}

			const T A = static_cast<T>(10) * t;
			return pow(TWO, -A) * sin((A - static_cast<T>(0.75)) * TWO * pi<T>() / static_cast<T>(3)) + ONE;
		}
		/*!
		 \brief Retrieves the eased value using an InOutElastic function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeInOutElastic((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API T easeInOutElastic(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeInOutElastic' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T ZERO = static_cast<T>(0);
			constexpr T HALF = static_cast<T>(0.5);
			constexpr T ONE = static_cast<T>(1);
			constexpr T TWO = static_cast<T>(2);
			constexpr T TEN = static_cast<T>(10);

			if (t == ZERO) {
				return ZERO;
			}
			if (t == ONE) {
				return ONE;
			}

			const T A = static_cast<T>(20) * t;
			const T B = sin((A - static_cast<T>(11.125)) * TWO * pi<T>() / static_cast<T>(4.5)) * HALF;

			if (t < HALF) {
				return -pow(TWO, A - TEN) * B;
			}

			return pow(TWO, -A + TEN) * B + ONE;
		}
		/*!
		 \brief Retrieves the eased value using an OutBounce function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeOutBounce((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T easeOutBounce(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeOutBounce' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T VAL1 = static_cast<T>(2.75);
			constexpr T VAL2 = static_cast<T>(7.5625);

			if (t < static_cast<T>(1) / VAL1) {
				return VAL2 * t * t;
			}
			if (t < static_cast<T>(2) / VAL1) {
				return VAL2 * (t -= static_cast<T>(1.5) / VAL1) * t + static_cast<T>(0.75);
			}
			if (t < static_cast<T>(2.5) / VAL1) {
				return VAL2 * (t -= static_cast<T>(2.25) / VAL1) * t + static_cast<T>(0.9375);
			}

			return VAL2 * (t -= static_cast<T>(2.625) / VAL1) * t + static_cast<T>(0.984375);
		}
		/*!
		 \brief Retrieves the eased value using an InBounce function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeInBounce((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T easeInBounce(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeInBounce' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T ONE = static_cast<T>(1);

			return ONE - easeOutBounce(ONE - t);
		}
		/*!
		 \brief Retrieves the eased value using an InOutBounce function.
		 \note Only floating point types are allowed: float, double, long double.

		 \param[in] t A value in the range [0;1]

		 \return The rate of change over time based on the \a t provided

		 \par Example:
		 \code
		 double factorOverTime = ae::Math::easeInOutBounce((elapsedTime / duration).asSeconds());
		 \endcode

		 \since v0.7.0
		*/
		template <typename T>
		[[nodiscard]] AEON_API constexpr T easeInOutBounce(T t)
		{
			static_assert(std::is_floating_point_v<T>, "'easeInOutBounce' only accepts floating point types");
			assert(t >= static_cast<T>(0));
			assert(t <= static_cast<T>(1));

			constexpr T HALF = static_cast<T>(0.5);
			constexpr T ONE = static_cast<T>(1);

			const T A = static_cast<T>(2) * t;

			if (t < HALF) {
				return (ONE - easeOutBounce(ONE - A)) * HALF;
			}

			return (ONE + easeOutBounce(A - ONE)) * HALF;
		}
	}
}

/*!
 \namespace ae::Math
 \ingroup math

 The namespace ae::Math provides utility functions that may prove useful during
 the development of a game, whether it be 2D or 3D.
 
 \author Filippos Gleglakos
 \version v0.7.0
 \date 2022.03.20
 \copyright MIT License
*/