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

#ifndef Aeon_Graphics_Camera3D_H_
#define Aeon_Graphics_Camera3D_H_

#include <AEON/Math/Vector.h>
#include <AEON/Graphics/Camera.h>

namespace ae
{
	/*!
	 \brief The class used to represent a camera for a 3D scene.
	 \details This camera is typically the standard camera for a 3D scene.
	*/
	class _NODISCARD AEON_API Camera3D : public Camera
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::Camera3D by providing the optional distances to the Z-axis planes and the field of view.

		 \param[in] nearPlane The distance from the camera to the near plane in the Z-axis, 0.1f by default
		 \param[in] farPlane The distance from the camera to the far plane in the Z-axis, 1000.f by default
		 \param[in] fov The camera's field of view, 60.f by default

		 \par Example:
		 \code
		 ae::Camera3D camera(0.1f, 5000.f, 90.f);
		 \endcode

		 \since v0.4.0
		*/
		explicit Camera3D(float nearPlane = 0.1f, float farPlane = 1000.f, float fov = 60.f);
		/*!
		 \brief Virtual destructor.
		 \details A virtual destructor is needed as this class will be inherited.

		 \since v0.4.0
		*/
		virtual ~Camera3D();
	public:
		// Public method(s)
		/*!
		 \brief Sets the ae::Camera3D's field of view.
		 \details The field of view is normally situated in the range [60,110] but no warning log will be generated if the one provided isn't within that range.

		 \param[in] fov The new field of view, 60.f by default

		 \par Example:
		 \code
		 ae::Camera3D camera;
		 ...
		 camera.setFOV(90.f);
		 \endcode

		 \sa getFOV()

		 \since v0.4.0
		*/
		void setFOV(float fov) noexcept;
		/*!
		 \brief Retrieves the ae::Camera3D's field of view.

		 \return The camera's current field of view

		 \par Example:
		 \code
		 ae::Camera3D camera(0.1f, 5000.f, 90.f);
		 ...
		 float cameraFOV = camera.getFOV();
		 \endcode

		 \sa setFOV()

		 \since v0.4.0
		*/
		_NODISCARD float getFOV() const noexcept;

		// Public virtual method(s)
		/*!
		 \brief Retrieves the ae::Camera3D's perspective projection matrix.
		 \details This matrix transforms view-space coordinates into clip-space coordinates.

		 \return An ae::Matrix4f containing the perspective projection matrix

		 \par Example:
		 \code
		 ae::Camera3D camera;

		 const ae::Matrix4f& projectionMatrix = camera.getProjectionMatrix();
		 \endcode

		 \since v0.5.0
		*/
		_NODISCARD virtual const Matrix4f& getProjectionMatrix() override final;
	protected:
		// Protected method(s)
		/*!
		 \brief Updates and retrieves the render target's current framebuffer size.
		 \details The internally-saved framebuffer size is overwritten and the projection matrix update flag is raised automatically.

		 \return The render target's current framebuffer size

		 \since v0.4.0
		*/
		const Vector2f& updateInternalFrameSize() noexcept;

	private:
		// Private member(s)
		Vector2f mTargetSize; //!< The size of the render target's framebuffer size
		float    mFOV;        //!< The field of view
	};
}
#endif // Aeon_Graphics_Camera3D_H_

/*!
 \class ae::Camera3D
 \ingroup graphics

 The ae::Camera3D class is used to represent a 3D scene and determines how it's
 displayed to the application user. The camera/view concept is incredibly
 powerful as it can be used to move and rotate the entire scene without
 modifying how all the scene objects are rendered.

 This class offers the standard camera used to represent a 3-dimensional world,
 the alternative is the ae::CameraFPS class which is a "free" camera meaning
 that its rotation is affected by the mouse cursor's movement (typically used
 in FPS (First-Person Shooter) games).

 \author Filippos Gleglakos
 \version v0.5.0
 \date 2020.08.03
 \copyright MIT License
*/