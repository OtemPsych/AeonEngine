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

#ifndef Aeon_Graphics_CameraFPS_H_
#define Aeon_Graphics_CameraFPS_H_

#include <AEON/Graphics/Camera3D.h>

namespace ae
{
	/*!
	 \brief The class used to represent an FPS (First-Person Shooter) camera for a 3D scene.
	 \details This class is typically the standard camera for FPS games.
	*/
	class _NODISCARD AEON_API CameraFPS : public Camera3D
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::CameraFPS by providing the optional distances to the Z-axis planes, the field of view and the rotational sensitivity. 
		 \details The mouse cursor will automatically be hidden and locked to the active window upon construction.

		 \param[in] nearPlane The distance from the camera to the near plane in the Z-axis, 0.1f by default
		 \param[in] farPlane The distance from the camera to the far plane in the Z-axis, 1000.f by default
		 \param[in] fov The camera's field of view, 60.f by default
		 \param[in] sensitivity The camera's rotational sensitivity factor based on the mouse cursor's movement, 0.002f by default

		 \par Example:
		 \code
		 ae::CameraFPS camera(0.1f, 5000.f, 110.f, 0.006f);
		 \endcode

		 \since v0.4.0
		*/
		explicit CameraFPS(float nearPlane = 0.1f, float farPlane = 1000.f, float fov = 60.f, float sensitivity = 0.002f);
	public:
		// Public method(s)
		/*!
		 \brief Sets the ae::CameraFPS's sensitivity to the mouse cursor's movements.
		 \details The movement sensitivity determines the speed of the camera's rotation.

		 \param[in] sensitivity The new sensitivity to the mouse cursor's movement, 0.002f by default

		 \par Example:
		 \code
		 // Sensitivity: 0.002f
		 ae::CameraFPS camera;
		 ...
		 camera.setSensitivity(0.006f);
		 \endcode

		 \sa getSensitivity()

		 \since v0.4.0
		*/
		void setSensitivity(float sensitivity) noexcept;
		/*!
		 \brief Retrieves the ae::CameraFPS's sensitivity to the mouse cursor's movements.
		 \details The movement sensitivity determines the speed of the camera's rotation.

		 \return The ae::CameraFPS's current sensitivity to the mouse cursor's movement

		 \par Example:
		 \code
		 ae::CameraFPS camera;
		 float cameraSensitivity = camera.getSensitivity();
		 \endcode

		 \sa setSensitivity()

		 \since v0.4.0
		*/
		_NODISCARD float getSensitivity() const noexcept;

		// Public virtual method(s)
		/*!
		 \brief Retrieves the ae::CameraFPS's current rotation.

		 \return An ae::Quaternion containing the current rotation

		 \par Example:
		 \code
		 // Near plane: 0.1, Far plane: 5000, FOV: 110, Rotational sensitivity: 0.008
		 ae::Camera3D camera(0.1f, 5000.f, 110.f, 0.008f);
		 ...
		 const ae::Quaternion& cameraRotation = camera.getRotation();
		 \endcode

		 \since v0.4.0
		*/
		_NODISCARD virtual const Quaternion& getRotation() override final;
		/*!
		 \brief Retrieves the ae::CameraFPS's view matrix.
		 \details The view matrix will be updated (if necessary) before being retrieved.

		 \par Example:
		 \code
		 // Near plane: 0.1, Far plane: 5000, FOV: 110, Rotational sensitivity: 0.008
		 ae::Camera3D camera(0.1f, 5000.f, 110.f, 0.008f);
		 ...
		 const ae::Matrix4f& viewMatrix = camera.getViewMatrix();
		 \endcode

		 \since v0.5.0
		*/
		_NODISCARD virtual const Matrix4f& getViewMatrix() override final;

	private:
		// Private member(s)
		Vector2f mLastMousePos; //!< The last recorded mouse position
		float    mSensitivity;  //!< The rotational sensitivity of the camera
	};
}
#endif // Aeon_Graphics_CameraFPS_H_

/*!
 \class ae::CameraFPS
 \ingroup graphics

 The ae::CameraFPS class is used to represent a 3D scene wherein the camera is
 "free", meaning that its rotation is affected by the mouse cursor's movement
 (typically used in FPS (First-Person Shooter) games).

 Usage example:
 \code
 class GameLayer : public ae::Layer
 {
 public:
	GameLayer()
		: Layer()
		, mCamera()
		, mCameraSpeed(10.f)
		, mMovingLeft(false)
		, mMovingRight(false)
		, mMovingForward(false)
		, mMovingBackward(false)
	{
		// Configure the camera
		mCamera.setFOV(90.f);
		mCamera.setMouseSensitivity(0.004f);
	}
 public:
	virtual bool handleEvent(ae::Event* const event) override final
	{
		// Check if the event received concerns a key
		if (event->type == ae::Event::Type::KeyPressed || event->type == ae::Event::Type::KeyReleased) {
			bool pressed = event->type == ae::Event::Type::KeyPressed; // the flag to see if the key was pressed or released
			auto keyEvent = event->as<ae::KeyEvent>();                 // convert the Event pointer to the actual KeyEvent
			ae::Keyboard::Key key = keyEvent->key;                     // the key in question

			// Check if one of the right keys was pressed/released
			switch (key)
			{
			case ae::Keyboard::Key::W:
				mMovingForward = pressed;
				break;
			case ae::Keyboard::Key::S:
				mMovingBackward = pressed;
				break;
			case ae::Keyboard::Key::A:
				mMovingLeft = pressed;
				break;
			case ae::Keyboard::Key::D:
				mMovingRight = pressed;
			}
		}

		return true;
	}

	virtual bool update(const ae::Time& dt) override final
	{
		// The camera's movement speed multiplied by the fixed time-step to ensure consistent speed
		const float CAMERA_SPEED = mCameraSpeed * dt.asSeconds();

		// Apply the displacement based on the keys pressed down
		ae::Vector3f movement;
		if (mMovingLeft) {
			movement -= mCamera.getLocalRight() * CAMERA_SPEED; // go leftwards
		}
		if (mMovingRight) {
			movement += mCamera.getLocalRight() * CAMERA_SPEED; // go rightwards
		}

		if (mMovingForward) {
			movement += mCamera.getLocalForward() * CAMERA_SPEED; // go forwards
		}
		if (mMovingBackward) {
			movement -= mCamera.getLocalForward() * CAMERA_SPEED; // go backwards
		}

		mCamera.move(movement);
		return true;
	}

 private:
	ae::CameraFPS mCamera;         // The camera instance
	float         mCameraSpeed;    // The camera's movement speed
	bool          mMovingLeft;     // Whether we're moving leftwards
	bool          mMovingRight;    // Whether we're moving rightwards
	bool          mMovingForward;  // Whether we're moving forward
	bool          mMovingBackward; // Whether we're moving backward
 };
 \endcode

 \author Filippos Gleglakos
 \version v0.5.0
 \date 2020.08.03
 \copyright MIT License
*/