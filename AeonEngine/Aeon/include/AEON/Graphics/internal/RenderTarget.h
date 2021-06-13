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

#ifndef Aeon_Graphics_RenderTarget_H_
#define Aeon_Graphics_RenderTarget_H_

#include <AEON/Config.h>
#include <AEON/Math/Vector.h>
#include <AEON/Math/AABoxCollider.h>
#include <AEON/Graphics/Camera.h>

namespace ae
{
	// Forward declaration(s)
	class Color;

	/*!
	 \brief Abstract base class for all render targets (window, texture).
	*/
	class AEON_API RenderTarget
	{
	private:
		// Private static member(s)
		static const RenderTarget* activeTarget; //!< The currently active render target
		static float               depthValue;   //!< The depth buffer clear value

	public:
		// Public constructor(s)
		/*!
		 \brief Deleted copy constructor.

		 \since v0.4.0
		*/
		RenderTarget(const RenderTarget&) = delete;
		/*!
		 \brief Virtual destructor.
		 \details A virtual destructor is needed as this class will be inherited.

		 \since v0.4.0
		*/
		virtual ~RenderTarget();
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.4.0
		*/
		RenderTarget& operator=(const RenderTarget&) = delete;
	public:
		// Public method(s)
		/*!
		 \brief Clears the active ae::RenderTarget's color and depth buffers.
		 \note This method should only be used internally, its use by the API user isn't necessary.
		 
		 \sa setClearColor()

		 \since v0.4.0
		*/
		void clear();
		/*!
		 \brief Activates the ae::RenderTarget for rendering.
		 \details This method binds the framebuffer, sets the appropriate viewport and clears the color and depth buffers.
		 \note This method should only be used internally, its use by the API user isn't necessary.

		 \since v0.4.0
		*/
		void activate();
		/*!
		 \brief Sets the ae::RenderTarget's clear color used for the color buffer.

		 \param[in] color The ae::Color used to clear the previously rendered content

		 \sa clear()

		 \since v0.4.0
		*/
		void setClearColor(const Color& color);
		/*!
		 \brief Converts a point from target coordinates to world coordinates.
		 \details This method calculates the 2D position that matches the given pixel of the ae::RenderTarget.
		 \note This method uses the assigned camera for its calculations.

		 \param[in] pixel The 2-dimensional ae::Vector representing the pixel to convert

		 \return A 2-dimensional ae::Vector containing the converted pixel in world coordinates

		 \par Example:
		 \code
		 ae::Vector2f mouseCoord = window.mapPixelToCoords(ae::Mouse::getPosition());
		 \endcode

		 \sa mapCoordsToPixel()

		 \since v0.4.0
		*/
		_NODISCARD Vector2f mapPixelToCoords(const Vector2f& pixel) const;
		/*!
		 \brief Converts a point from world coordinates to target coordinates.
		 \details This method calculates the pixel of the ae::RenderTarget that matches the point provided.
		 \note This method uses the assigned camera for its calculations.

		 \param[in] point The 2-dimensional ae::Vector representing the point to convert

		 \return A 2-dimensional ae::Vector containing the converted point in target coordinates

		 \sa mapPixelToCoords()

		 \since v0.4.0
		*/
		_NODISCARD Vector2f mapCoordsToPixel(const Vector2f& point) const;
		/*!
		 \brief Retrieves the viewport of the assigned camera in relation to the caller ae::RenderTarget.
		 \details The camera's viewport is defined as a ratio, whereas this method applies said ratio to the current dimensions of the ae::RenderTarget.

		 \return An ae::Box2f containing the viewport rectangle in pixels

		 \since v0.4.0
		*/
		_NODISCARD Box2f getViewport() const;
		/*!
		 \brief Retrieves the base class pointer of the camera currently used by the ae::RenderTarget.
		 \note The base pointer must be converted to the derived pointer in order to use its full functionalities.

		 \return The base class pointer of the camera in use

		 \sa setCamera()

		 \since v0.4.0
		*/
		_NODISCARD Camera* getCamera();
		/*!
		 \brief Retrieves the ae::RenderTarget's framebuffer size which is in pixels.
		 \details There's no gurantee that a window's size which is in screen coordinates will map 1:1 with the framebuffer size which is in pixels.
		 It's therefore recommended to use the framebuffer size for pixel-based OpenGL operations, such as setting the viewport.

		 \return The 2-dimensional ae::Vector containing the framebuffer's size

		 \since v0.4.0
		*/
		_NODISCARD const Vector2i& getFramebufferSize() const noexcept;
		/*!
		 \brief Sets a new camera to be used by the ae::RenderTarget.
		 \details The \a camera provided will be copied, so it doesn't need to be kept alive.
		 \note The \a camera has to be a derived class of the ae::Camera base class.

		 \param[in] camera The new camera to be used

		 \sa getCamera()

		 \since v0.4.0
		*/
		template <class T, typename = std::enable_if_t<std::is_base_of_v<Camera, T>>>
		void setCamera(const T& camera)
		{
			mCamera = std::make_unique<T>(camera);
			mCamera->setTarget(this);
		}

		// Public virtual method(s)
		/*!
		 \brief Retrieves the ae::RenderTarget's internal framebuffer handle.
		 \details Must be overloaded by derived classes as it selects the window's back buffer by default.

		 \return The internal OpenGL framebuffer handle

		 \since v0.4.0
		*/
		_NODISCARD virtual unsigned int getFramebufferHandle() const noexcept;
	protected:
		// Protected constructor(s)
		/*!
		 \brief Default constructor.
		 \details Sets the clear color to an opaque black, depth clear value to 1.f and the FBO handle to 0.

		 \since v0.4.0
		*/
		RenderTarget() noexcept;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::RenderTarget that will be moved

		 \since v0.5.0
		*/
		RenderTarget(RenderTarget&& rvalue) noexcept;
	protected:
		// Protected operator(s)
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::RenderTarget that will be moved

		 \return The caller ae::RenderTarget

		 \since v0.5.0
		*/
		RenderTarget& operator=(RenderTarget&& rvalue) noexcept;

	protected:
		// Protected member(s)
		Vector2i                mFramebufferSize; //!< The render target's framebuffer size
	private:
		// Private member(s)
		Vector4f                mClearColor;      //!< The normalized color used to clear the target's color buffer
		std::unique_ptr<Camera> mCamera;          //!< The render target's camera
	};
}
#endif // Aeon_Graphics_RenderTarget_H_

/*!
 \class ae::RenderTarget
 \ingroup graphics

 The ae::RenderTarget abstract base class defines the common behaviour of all
 the render targets. They also possess a camera object that decides what is
 shown on the window / render texture.

 \author Filippos Gleglakos
 \version v0.5.0
 \date 2020.06.09
 \copyright MIT License
*/