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

#ifndef Aeon_Graphics_Camera2D_H_
#define Aeon_Graphics_Camera2D_H_

#include <AEON/Math/Vector2.h>
#include <AEON/Graphics/Camera.h>

namespace ae
{
	/*!
	 \brief The class used to represent a camera for a 2D scene.
	 \details This camera can be used for a 3D scene's user interface (UI) or for a 2D scene.
	*/
	class _NODISCARD AEON_API Camera2D : public Camera
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::Camera2D by providing the optional distances to the Z-axis planes.

		 \param[in] nearPlane The distance from the camera to the near plane in the Z-axis, -1.f by default
		 \param[in] farPlane The distance from the camera to the far plane in the Z-axis, 1.f by default

		 \par Example:
		 \code
		 ae::Camera2D camera(-1.f, 2000.f);
		 \endcode

		 \since v0.4.0
		*/
		explicit Camera2D(float nearPlane = -1.f, float farPlane = 1.f);
	public:
		// Public method(s)
		/*!
		 \brief The zoom factor simulates a zoom by scaling the render target's framebuffer size.
		 \details Performs the zoom with the ae::Camera2D's position as the zoom base.\n
		 A zoom factor of 1.f is the unscaled size.\n
		 A zoom factor of < 1.f simulates a zoomed-in scene.\n
		 A zoom factor of > 1.f simulates a zoomed-out scene.

		 \param[in] factor The zoom factor by which to scale the framebuffer size

		 \par Example:
		 \code
		 ae::Camera2D camera;
		 camera.zoom(0.5f);
		 \endcode

		 \sa zoomAt()

		 \since v0.4.0
		*/
		void zoom(float factor);
		/*!
		 \brief Zooms in at a specific point in screen coordinates.
		 \details The zoom factor simulates a zoom by scaling the render target's framebuffer size.\n
		 A zoom factor of 1.f is the unscaled size.\n
		 A zoom factor of < 1.f simulates a zoomed-in scene.\n
		 A zoom factor of > 1.f simulates a zoomed-out scene.
		 \note An ae::RenderTarget must be associated with this ae::Camera2D.

		 \param[in] pixel The 2-dimensional ae::Vector representing the pixel to serve as the zoom base
		 \param[in] factor The zoom factor by which to scale the framebuffer size

		 \par Example:
		 \code
		 ae::Camera2D* windowCamera = mWindow.getCamera();
		 windowCamera->zoomAt(ae::Mouse::getPosition(), 0.5f);
		 \endcode

		 \sa zoom()

		 \since v0.4.0
		*/
		void zoomAt(const Vector2f& pixel, float factor);

		// Public virtual method(s)
		/*!
		 \brief Retrieves the ae::Camera2D's orthographic projection matrix.
		 \details This matrix transforms view-space coordinates into clip-space coordinates.
		 
		 \return An ae::Matrix4f containing the orthographic projection matrix

		 \par Example:
		 \code
		 ae::Camera2D camera;

		 const ae::Matrix4f& projectionMatrix = camera.getProjectionMatrix();
		 \endcode

		 \since v0.4.0
		*/
		_NODISCARD virtual const Matrix4f& getProjectionMatrix() override final;

	private:
		// Private member(s)
		float mZoomFactor; //!< The camera's zoom factor applied to the framebuffer size
	};
}
#endif // Aeon_Graphics_Camera2D_H_

/*!
 \class ae::Camera2D
 \ingroup graphics

 The ae::Camera2D class is used to represent a 2D scene and determines how it's
 displayed to the application user. The camera/view concept is incredibly
 powerful as it can be used to move and rotate the entire scene without
 modifying how all the scene objects are rendered.

 While this camera is primarily used for a 2D scene, it remains a 3-dimensional
 entity as the Z axis can still serve a major role; that being the zoom
 functionality.

 \author Filippos Gleglakos
 \version v0.4.0
 \date 2019.10.13
 \copyright MIT License
*/