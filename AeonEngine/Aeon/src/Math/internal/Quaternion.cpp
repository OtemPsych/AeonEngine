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

#include <AEON/Math/internal/Quaternion.h>

namespace ae
{
	// Public constructor(s)
	Quaternion::Quaternion() noexcept
		: w(1.f)
		, v(0.f)
	{
	}

	Quaternion::Quaternion(float w, float x, float y, float z) noexcept
		: w(w)
		, v(x, y, z)
	{
	}

	Quaternion::Quaternion(float w, const Vector3f& axes) noexcept
		: w(w)
		, v(axes)
	{
	}

	Quaternion::Quaternion(const Vector4f& rotation) noexcept
		: w(rotation.w)
		, v(rotation)
	{
	}

	Quaternion::Quaternion(const Quaternion& copy) noexcept
		: w(copy.w)
		, v(copy.v)
	{
	}

	Quaternion::Quaternion(Quaternion&& rvalue) noexcept
		: w(std::move(rvalue.w))
		, v(std::move(rvalue.v))
	{
	}

	// Public operator(s)
	Quaternion& Quaternion::operator=(const Quaternion& other) noexcept
	{
		// Check that the caller object won't be assigned to itself (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (this == &other) {
				AEON_LOG_ERROR("Invalid assignment", "Attempt to assign an object to itself.\nAborting operation.");
				return *this;
			}
		}

		w = other.w;
		v = other.v;

		return *this;
	}

	Quaternion& Quaternion::operator=(Quaternion&& rvalue) noexcept
	{
		w = std::move(rvalue.w);
		v = std::move(rvalue.v);

		return *this;
	}

	Quaternion Quaternion::operator+(const Quaternion& other) const noexcept
	{
		return Quaternion(w + other.w, v + other.v);
	}

	Quaternion Quaternion::operator-(const Quaternion& other) const noexcept
	{
		return Quaternion(w - other.w, v - other.v);
	}

	Quaternion Quaternion::operator*(const Quaternion& other) const noexcept
	{
		return Quaternion(
			((w * other.w) - (x * other.x) - (y * other.y) - (z * other.z)),
			((w * other.x) + (x * other.w) + (y * other.z) - (z * other.y)),
			((w * other.y) - (x * other.z) + (y * other.w) + (z * other.x)),
			((w * other.z) + (x * other.y) - (y * other.x) + (z * other.w)));
	}

	Quaternion Quaternion::operator*(float scalar) const noexcept
	{
		return Quaternion(w * scalar, v * scalar);
	}

	Quaternion Quaternion::operator/(float scalar) const
	{
		// Check if scalar is equal to 0 (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (scalar == 0.f) {
				AEON_LOG_ERROR("Division by zero", "The scalar value provided is equal to 0.\nReturning caller quaternion.");
				return *this;
			}
		}

		return Quaternion(w / scalar, v / scalar);
	}

	Vector3f Quaternion::operator*(const Vector3f& vec) const noexcept
	{
		return rotate(vec);
	}

	Quaternion& Quaternion::operator+=(const Quaternion& other) noexcept
	{
		w += other.w;
		v += other.v;

		return *this;
	}

	Quaternion& Quaternion::operator-=(const Quaternion& other) noexcept
	{
		w -= other.w;
		v -= other.v;

		return *this;
	}

	Quaternion& Quaternion::operator*=(const Quaternion& other) noexcept
	{
		return (*this = *this * other);
	}

	Quaternion& Quaternion::operator*=(float scalar) noexcept
	{
		w *= scalar;
		v *= scalar;

		return *this;
	}

	Quaternion& Quaternion::operator/=(float scalar)
	{
		// Check if scalar is equal to 0 (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (scalar == 0.f) {
				AEON_LOG_ERROR("Division by zero", "The scalar value provided is equal to 0.\nReturning caller quaternion.");
				return *this;
			}
		}

		w /= scalar;
		v /= scalar;

		return *this;
	}

	bool Quaternion::operator==(const Quaternion& other) const noexcept
	{
		return (w == other.w && v == other.v);
	}

	bool Quaternion::operator!=(const Quaternion& other) const noexcept
	{
		return !(*this == other);
	}

	// Friend operator(s)
	Quaternion operator-(const Quaternion& quat) noexcept
	{
		return Quaternion(-quat.w, -quat.v);
	}

	// Public method(s)
	Vector3f Quaternion::toEulerAngles() const noexcept
	{
		const float YY = y * y;
		const Vector2f PITCH(2.f * (w * x + y * z), 1.f - 2.f * (x * x + YY));

		return Vector3f(
			((PITCH == Vector2f(0.f)) ? 2.f * Math::atan2(x, w) : Math::atan2(PITCH.x, PITCH.y)),
			Math::asin(Math::clamp(2.f * (w * y - z * x), -1.f, 1.f)),
			Math::atan2(2.f * (w * z + x * y), 1.f - 2.f * (YY + z * z))
		);
	}

	Vector3f Quaternion::rotate(const Vector3f& vec) const noexcept
	{
		const Vector3f UV = cross(v, vec);
		return vec + 2.f * (UV * w + cross(v, UV));
	}

	Quaternion Quaternion::conjugate() const noexcept
	{
		return Quaternion(w, -v);
	}

	float Quaternion::magnitude() const noexcept
	{
		return Math::sqrt(x * x + y * y + z * z + w * w);
	}

	Quaternion Quaternion::normalize() const
	{
		const float MAGNITUDE = magnitude();
		return (MAGNITUDE <= 0.f) ? Quaternion() : (*this / MAGNITUDE);
	}

	float Quaternion::getAngle() const noexcept
	{
		return w;
	}

	const Vector3f& Quaternion::getAxes() const noexcept
	{
		return v;
	}

	// Public static method(s)
	Quaternion Quaternion::rotation(const Vector3f& unitVec0, const Vector3f& unitVec1)
	{
		const float M = Math::sqrt(2.f * (1.f + ae::dot(unitVec0, unitVec1)));
		return Quaternion(0.5f * M, 1.f / M * cross(unitVec0, unitVec1));
	}

	Quaternion Quaternion::rotation(float angle, const Vector3f& axes) noexcept
	{
		Vector3f unitAxes = axes;

		// Normalize axes if necessary
		const float MAGNITUDE = unitAxes.magnitude();
		if (abs(MAGNITUDE - 1.f) > 0.001f) {
			unitAxes *= 1.f / MAGNITUDE;
		}

		angle *= 0.5f;
		return Quaternion(Math::cos(angle), unitAxes * Math::sin(angle));
	}

	Quaternion Quaternion::rotationX(float angle) noexcept
	{
		angle *= 0.5f;
		return Quaternion(Math::cos(angle), Vector3f::Right * Math::sin(angle));
	}

	Quaternion Quaternion::rotationY(float angle) noexcept
	{
		angle *= 0.5f;
		return Quaternion(Math::cos(angle), Vector3f::Up * Math::sin(angle));
	}

	Quaternion Quaternion::rotationZ(float angle) noexcept
	{
		angle *= 0.5f;
		return Quaternion(Math::cos(angle), Vector3f::Backward * Math::sin(angle));
	}

	float Quaternion::dot(const Quaternion& q0, const Quaternion& q1) noexcept
	{
		return (q0.w * q1.w + q0.x * q1.x + q0.y * q1.y + q0.z * q1.z);
	}
}