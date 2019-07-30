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

#ifndef Aeon_System_Time_H_
#define Aeon_System_Time_H_

#include <cstdint>

#include <AEON/Config.h>

namespace ae
{
	/*!
	 \brief The class representing a time value.
	*/
	class AEON_API Time
	{
	public:
		// Public static member(s)
		static const Time Zero; //!< The pre-defined time value of 'zero'

	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details Sets the time value to 0.0.

		 \since v0.3.0
		*/
		Time() noexcept;
	public:
		// Public operator(s)
		/*!
		 \brief Addition operator.
		 \details Performs an addition of the caller's and the \a other's time values.

		 \param[in] other The ae::Time of which its time value will be added with the caller's time value

		 \return An ae::Time containing the sum of the caller's and the \a other's time values

		 \par Example:
		 \code
		 ae::Time time1 = ae::Time::seconds(2.0);
		 ae::Time time2 = ae::Time::seconds(3.5);
		 ae::Time time3 = time1 + time2;
		 \endcode

		 \sa operator-(const Time&), operator/(const Time&)

		 \since v0.3.0
		*/
		_NODISCARD Time operator+(const Time& other) const noexcept;
		/*!
		 \brief Subtraction operator.
		 \details Performs a subtraction of the caller's and the \a other's time values.

		 \param[in] other The ae::Time of which its time value will subtract from the caller's time value

		 \return An ae::Time containing the difference of the caller's and the \a other's time values

		 \par Example:
		 \code
		 ae::Time time1 = ae::Time::seconds(2.0);
		 ae::Time time2 = ae::Time::seconds(3.5);
		 ae::Time time3 = time1 - time2;
		 \endcode

		 \sa operator+(const Time&), operator/(const Time&)

		 \since v0.3.0
		*/
		_NODISCARD Time operator-(const Time& other) const noexcept;
		/*!
		 \brief Division operator.
		 \details Performs a division of the caller's and the \a other's time values.

		 \param[in] other The ae::Time of which its time value will be the divisor of the caller's time value

		 \return An ae::Time containing the quotient of the caller's and the \a other's time values

		 \par Example:
		 \code
		 ae::Time time1 = ae::Time::seconds(2.0);
		 ae::Time time2 = ae::Time::seconds(3.5);
		 ae::Time time3 = time1 / time2;
		 \endcode

		 \sa operator+(const Time&), operator-(const Time&)

		 \since v0.3.0
		*/
		_NODISCARD Time operator/(const Time& other) const noexcept;

		/*!
		 \brief Multiplication operator.
		 \details Scales the ae::Time's time value by the scalar value provided.

		 \param[in] scale The scalar value which will scale the ae::Time's time value

		 \return An ae::Time containing the scaled time value

		 \par Example:
		 \code
		 ae::Time time = ae::Time::seconds(2.0);
		 ae::Time doubledTime = time * 2.0;
		 \endcode

		 \sa operator/(double)

		 \since v0.3.0
		*/
		_NODISCARD Time operator*(double scale) const noexcept;
		/*!
		 \brief Division operator.
		 \details Scales the ae::Time's time value by the scalar value provided.

		 \param[in] scale The scalar value which will scale the ae::Time's time value

		 \return An ae::Time containing the scaled time value

		 \par Example:
		 \code
		 ae::Time time = ae::Time::seconds(2.0);
		 ae::Time halvedTime = time / 2.0;
		 \endcode

		 \sa operator*(double)

		 \since v0.3.0
		*/
		_NODISCARD Time operator/(double scale) const;

		/*!
		 \brief Addition and assignment operator.
		 \details Performs an addition of the caller's and the \a other's time values, and assigns the sum to the caller ae::Time.

		 \param[in] other The ae::Time of which its time value will be added with the caller's time value

		 \return The caller ae::Time containing the sum of the addition, these calls can be chained together one after the other

		 \par Example:
		 \code
		 ae::Time time1 = ae::Time::seconds(2.0);
		 ae::Time time2 = ae::Time::seconds(3.5);
		 ...
		 time1 += time2;
		 \endcode

		 \sa operator-=(const Time&)

		 \since v0.3.0
		*/
		Time& operator+=(const Time& other) noexcept;
		/*!
		 \brief Subtraction and assignment operator.
		 \details Performs an subtraction of the caller's and the \a other's time values, and assigns the difference to the caller ae::Time.

		 \param[in] other The ae::Time of which its time value will subtract from the caller's time value

		 \return The caller ae::Time containing the difference of the subtraction, these calls can be chained together one after the other

		 \par Example:
		 \code
		 ae::Time time1 = ae::Time::seconds(2.0);
		 ae::Time time2 = ae::Time::seconds(3.5);
		 ...
		 time1 -= time2;
		 \endcode

		 \sa operator+=(const Time&)

		 \since v0.3.0
		*/
		Time& operator-=(const Time& other) noexcept;

		/*!
		 \brief Multiplication and assignment operator.
		 \details Scales the ae::Time's time value by the scalar value provided, and assigns the product to the ae::Time.

		 \param[in] scale The scalar value which will scale the ae::Time's time value

		 \return The ae::Time containing the scaled time value, these calls can be chained together one after the other

		 \par Example:
		 \code
		 ae::Time time = ae::Time::seconds(2.0);
		 ...
		 time *= 2.0;
		 \endcode

		 \sa operator/=(double)

		 \since v0.3.0
		*/
		Time& operator*=(double scale) noexcept;
		/*!
		 \brief Division and assignment operator.
		 \details Scales the ae::Time's time value by the scalar value provided, and assigns the quotient to the ae::Time.

		 \param[in] scale The scalar value which will scale the ae::Time's time value

		 \return The ae::Time containing the scaled time value, these calls can be chained together one after the other

		 \par Example:
		 \code
		 ae::Time time = ae::Time::seconds(2.0);
		 ...
		 time /= 2.0;
		 \endcode

		 \sa operator*=(double)

		 \since v0.3.0
		*/
		Time& operator/=(double scale);

		/*!
		 \brief Equality operator.
		 \details Checks if the caller's and the \a other's time values are equal.

		 \param[in] other The ae::Time that will be compared with the caller

		 \return True if the caller's and the \a other's time values are equal, false otherwise

		 \par Example:
		 \code
		 ae::Time time1 = ae::Time::seconds(2.0);
		 ae::Time time2 = ae::Time::milliseconds(2000);
		 if (time1 == time2) {
			...
		 }
		 \endcode

		 \sa operator!=()

		 \since v0.3.0
		*/
		_NODISCARD bool operator==(const Time& other) const noexcept;
		/*!
		 \brief Inequality operator.
		 \details Checks if the caller's and the \a other's time values are inequal.

		 \param[in] other The ae::Time that will be compared with the caller

		 \return True if the caller's and the \a other's time values are inequal, false otherwise

		 \par Example:
		 \code
		 ae::Time time1 = ae::Time::seconds(2.0);
		 ae::Time time2 = ae::Time::milliseconds(2500);
		 if (time1 != time2) {
			...
		 }
		 \endcode

		 \sa operator==()

		 \since v0.3.0
		*/
		_NODISCARD bool operator!=(const Time& other) const noexcept;
		/*!
		 \brief Less than operator.
		 \details Checks if the caller's time value is less than the \a other's time value.

		 \param[in] other The ae::Time that will be compared with the caller

		 \return True if the caller ae::Time's time value is less than the \a other's time value, false otherwise

		 \par Example:
		 \code
		 ae::Time time1 = ae::Time::seconds(2.0);
		 ae::Time time2 = ae::Time::seconds(2.5);
		 if (time1 < time2) {
			...
		 }
		 \endcode

		 \sa operator<=(), operator>(), operator>=()

		 \since v0.3.0
		*/
		_NODISCARD bool operator<(const Time& other) const noexcept;
		/*!
		 \brief Less than or equal to operator.
		 \details Checks if the caller's time value is less than or equal to the \a other's time value.

		 \param[in] other The ae::Time that will be compared with the caller

		 \return True if the caller ae::Time's time value is less than or equal to the \a other's time value, false otherwise

		 \par Example:
		 \code
		 ae::Time time1 = ae::Time::seconds(2.0);
		 ae::Time time2 = ae::Time::seconds(2.5);
		 if (time1 <= time2) {
			...
		 }
		 \endcode

		 \sa operator<(), operator>=(), operator>()

		 \since v0.3.0
		*/
		_NODISCARD bool operator<=(const Time& other) const noexcept;
		/*!
		 \brief Greater than operator.
		 \details Checks if the caller's time value is greater than the \a other's time value.

		 \param[in] other The ae::Time that will be compared with the caller

		 \return True if the caller ae::Time's time value is greater than the \a other's time value, false otherwise

		 \par Example:
		 \code
		 ae::Time time1 = ae::Time::seconds(2.5);
		 ae::Time time2 = ae::Time::seconds(2.0);
		 if (time1 > time2) {
			...
		 }
		 \endcode

		 \sa operator>=(), operator<(), operator<=()

		 \since v0.3.0
		*/
		_NODISCARD bool operator>(const Time& other) const noexcept;
		/*!
		 \brief Greater than or equal to operator.
		 \details Checks if the caller's time value is greater than or equal to the \a other's time value.

		 \param[in] other The ae::Time that will be compared with the caller

		 \return True if the caller ae::Time's time value is greater than or equal to the \a other's time value, false otherwise

		 \par Example:
		 \code
		 ae::Time time1 = ae::Time::seconds(2.0);
		 ae::Time time2 = ae::Time::seconds(2.5);
		 if (time1 >= time2) {
			...
		 }
		 \endcode

		 \sa operator>(), operator<=(), operator<()

		 \since v0.3.0
		*/
		_NODISCARD bool operator>=(const Time& other) const noexcept;

		// Friend operator(s)
		/*!
		 \brief Multiplication operator.
		 \details Scales the ae::Time's time value by the scalar value provided.

		 \param[in] scale The scalar value which will scale the ae::Time's time value
		 \param[in] time The ae::Time which will be scaled

		 \return An ae::Time containing the scaled time value

		 \par Example:
		 \code
		 ae::Time time = ae::Time::seconds(2.0);
		 ae::Time doubledTime = 2.0 * time;
		 \endcode

		 \sa operator/(double, const Time&)

		 \since v0.3.0
		*/
		friend AEON_API Time operator*(double scale, const Time& time) noexcept;

		/*!
		 \brief Unary negation operator.
		 \details Performs a negation on the \a time's value in which its sign will be inversed (positive to negative and vice-versa).

		 \param[in] time The ae::Time on which the operation will be performed

		 \return An ae::Time containing a time value with an opposite sign than the \a time provided

		 \par Example:
		 \code
		 ae::Time time = ae::Time::seconds(2.0);
		 ae::Time negativeTime = -time; // -2.0
		 \endcode

		 \since v0.3.0
		*/
		friend AEON_API Time operator-(const Time& time) noexcept;
	public:
		// Public method(s)
		/*!
		 \brief Retrieves the ae::Time's time value in milliseconds.

		 \return The time value in milliseconds

		 \par Example:
		 \code
		 ae::Time seconds = ae::Time::seconds(2.0);
		 int_fast32_t milliseconds = seconds.asMilliseconds(); // 2000 milliseconds
		 \endcode

		 \sa asSeconds(), asMicroseconds()

		 \since v0.3.0
		*/
		_NODISCARD int_fast32_t asMilliseconds() const noexcept;
		/*!
		 \brief Retrieves the ae::Time's time value in microseconds.

		 \return The time value in microseconds

		 \par Example:
		 \code
		 ae::Time seconds = ae::Time::seconds(2.0);
		 int_fast64_t microseconds = seconds.asMicroseconds(); // 2'000'000 microseconds
		 \endcode

		 \sa asSeconds(), asMilliseconds()

		 \since v0.3.0
		*/
		_NODISCARD int_fast64_t asMicroseconds() const noexcept;
		/*!
		 \brief Retrieves the ae::Time's time value in seconds.

		 \return The time value in seconds

		 \par Example:
		 \code
		 ae::Time milliseconds = ae::Time::milliseconds(2000);
		 double seconds = milliseconds.asSeconds(); // 2 seconds
		 \endcode

		 \sa asMilliseconds(), asMicroseconds()

		 \since v0.3.0
		*/
		_NODISCARD double asSeconds() const noexcept;

		// Public static method(s)
		/*!
		 \brief Constructs the ae::Time by providing a time value in seconds.

		 \param[in] seconds The time value in seconds

		 \return An ae::Time containing the time value provided

		 \par Example:
		 \code
		 ae::Time time = ae::Time::seconds(2.0); // 2 seconds
		 \endcode

		 \sa milliseconds(), microseconds()

		 \since v0.3.0
		*/
		_NODISCARD static Time seconds(double seconds) noexcept;
		/*!
		 \brief Constructs the ae::Time by providing a time value in milliseconds.

		 \param[in] milliseconds The time value in milliseconds

		 \return An ae::Time containing the time value provided

		 \par Example:
		 \code
		 ae::Time time = ae::Time::milliseconds(2000); // 2 seconds
		 \endcode

		 \sa seconds(), microseconds()

		 \since v0.3.0
		*/
		_NODISCARD static Time milliseconds(int_fast32_t milliseconds) noexcept;
		/*!
		 \brief Constructs the ae::Time by providing a time value in microseconds.

		 \param[in] microseconds The time value in microseconds

		 \return An ae::Time containing the time value provided

		 \par Example:
		 \code
		 ae::Time time = ae::Time::microseconds(2'000'000); // 2 seconds
		 \endcode

		 \sa seconds(), milliseconds()

		 \since v0.3.0
		*/
		_NODISCARD static Time microseconds(int_fast64_t microseconds) noexcept;
	private:
		// Private constructor(s)
		/*!
		 \brief Constructs the ae::Time by providing a time value in seconds.
		 \details The API user can't use this constructor, but he can use the available static methods.

		 \param[in] seconds The time value in seconds

		 \since v0.3.0
		*/
		explicit Time(double seconds) noexcept;

	private:
		// Private member(s)
		double mSeconds; //!< The time value in seconds
	};
}
#endif // Aeon_System_Time_H_

/*!
 \class ae::Time
 \ingroup system

 The ae::Time class is used to encapsulate a time value in a flexible manner.
 It permits the definition of a time value in seconds, in milliseconds or in
 microseconds. This also works the other way around: the API user can read a
 time value in seconds, in microseconds and in microseconds.

 The ae::Time objects support the standard mathematical operations, such as:
 adding and subtracting ae::Time objects, multiplying or dividing (essentially
 scaling the time value) by a scalar value, and comparing two ae::Time objects.

 Usage example:
 \code
 ae::Time time1 = ae::Time::seconds(0.1);
 int_fast32_t milli = time1.asMilliseconds(); // 100 milliseconds

 ae::Time time2 = ae::Time::milliseconds(30);
 int_fast64_t micro = time2.asMicroseconds(); // 30'000 microseconds

 ae::Time time3 = ae::Time::microseconds(-800'000);
 double sec = time3.asSeconds(); // -0.8 seconds
 \endcode

 \sa ae::Clock

 \author Filippos Gleglakos
 \version v0.3.0
 \date 2019.07.13
 \copyright MIT License
*/