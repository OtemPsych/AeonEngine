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

#include <AEON/System/Time.h>

#include <ctime>
#include <cassert>

namespace ae
{
	// Public static member(s)
	const Time Time::Zero(0.0);

	// Public constructor(s)
	Time::Time() noexcept
		: mSeconds(0.0)
	{
	}

	// Public operator(s)
	Time Time::operator+(const Time& other) const noexcept
	{
		return Time(mSeconds + other.mSeconds);
	}

	Time Time::operator-(const Time& other) const noexcept
	{
		return Time(mSeconds - other.mSeconds);
	}

	Time Time::operator/(const Time& other) const noexcept
	{
		return Time(mSeconds / other.mSeconds);
	}

	Time Time::operator*(double scale) const noexcept
	{
		return Time(mSeconds * scale);
	}

	Time Time::operator/(double scale) const
	{
		assert(scale != 0.0);
		return Time(mSeconds / scale);
	}

	Time& Time::operator+=(const Time& other) noexcept
	{
		mSeconds += other.mSeconds;
		return *this;
	}

	Time& Time::operator-=(const Time& other) noexcept
	{
		mSeconds -= other.mSeconds;
		return *this;
	}

	Time& Time::operator*=(double scale) noexcept
	{
		mSeconds *= scale;
		return *this;
	}

	Time& Time::operator/=(double scale)
	{
		assert(scale != 0.0);

		mSeconds /= scale;
		return *this;
	}

	bool Time::operator==(const Time& other) const noexcept
	{
		return mSeconds == other.mSeconds;
	}

	bool Time::operator!=(const Time& other) const noexcept
	{
		return mSeconds != other.mSeconds;
	}

	bool Time::operator<(const Time& other) const noexcept
	{
		return mSeconds < other.mSeconds;
	}

	bool Time::operator<=(const Time& other) const noexcept
	{
		return mSeconds <= other.mSeconds;
	}

	bool Time::operator>(const Time& other) const noexcept
	{
		return mSeconds > other.mSeconds;
	}

	bool Time::operator>=(const Time& other) const noexcept
	{
		return mSeconds >= other.mSeconds;
	}

	// Friend operator(s)
	Time operator*(double scale, const Time& time) noexcept
	{
		return Time(scale * time.mSeconds);
	}

	Time operator-(const Time& time) noexcept
	{
		return Time(-time.mSeconds);
	}

	// Public method(s)
	int32_t Time::asMilliseconds() const noexcept
	{
		return static_cast<int32_t>(mSeconds * 1000.0);
	}

	int64_t Time::asMicroseconds() const noexcept
	{
		return static_cast<int64_t>(mSeconds * 1'000'000.0);
	}

	// Public static method(s)
	Time Time::seconds(double seconds) noexcept
	{
		return Time(seconds);
	}

	Time Time::milliseconds(int32_t milliseconds) noexcept
	{
		return Time(static_cast<double>(milliseconds) / 1000.0);
	}

	Time Time::microseconds(int64_t microseconds) noexcept
	{
		return Time(static_cast<double>(microseconds) / 1'000'000.0);
	}

	std::string Time::getSystemDate()
	{
		// Retrieve system time
		time_t currentTime = time(0);
		tm date;
		localtime_s(&date, &currentTime);

		// Setup leading zeros for the date
		char day[3], month[3], year[5];
		snprintf(day, sizeof(day), "%02d", date.tm_mday);
		snprintf(month, sizeof(month), "%02d", 1 + date.tm_mon);
		snprintf(year, sizeof(year), "%d", 1900 + date.tm_year);

		char dateStr[11];
		snprintf(dateStr, sizeof(dateStr), "%s.%s.%s", year, month, day);

		return std::string(dateStr);
	}

	std::string Time::getSystemTime()
	{
		// Retrieve system time
		time_t currentTime = time(0);
		tm date;
		localtime_s(&date, &currentTime);

		// Setup leading zeros for the time
		char hours[3], minutes[3], seconds[3];
		snprintf(hours, 3, "%02d", date.tm_hour);
		snprintf(minutes, 3, "%02d", date.tm_min);
		snprintf(seconds, 3, "%02d", date.tm_sec);

		char timeStr[9];
		snprintf(timeStr, sizeof(timeStr), "%s:%s:%s", hours, minutes, seconds);

		return std::string(timeStr);
	}

	// Private constructor(s)
	Time::Time(double seconds) noexcept
		: mSeconds(seconds)
	{
	}
}