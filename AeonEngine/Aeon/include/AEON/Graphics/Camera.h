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

#ifndef Aeon_Graphics_Camera_H_
#define Aeon_Graphics_Camera_H_

#include <AEON/Math/Vector.h>
#include <AEON/Math/Matrix.h>
#include <AEON/Math/AABoxCollider.h>
#include <AEON/Math/internal/Quaternion.h>

namespace ae
{
	// Forward declaration(s)
	class RenderTarget;

	/*!
	 \brief The abstract base class representing a camera/view of a scene.
	 \note No direct instances of this class may be created.
	*/
	class AEON_API Camera
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Virtual destructor.
		 \details A virtual destructor is needed as this class will be inherited.

		 \since v0.4.0
		*/
		virtual ~Camera();
	public:
		// Public method(s)
		/*!
		 \brief Sets the position of the ae::Camera.
		 \details This method replaces the ae::Camera's current position with the \a position provided.

		 \param[in] position An ae::Vector3f containing the new position

		 \par Example:
		 \code
		 ae::Camera3D camera;
		 camera.setPosition(ae::Vector3f(0.f, 0.f, 3.f));
		 \endcode

		 \sa move(), getPosition()

		 \since v0.4.0
		*/
		void setPosition(const Vector3f& position) noexcept;
		/*!
		 \brief Sets the position of the ae::Camera.
		 \details This method replaces the ae::Camera's current position with the coordinates provided.

		 \param[in] posX The position in the X-axis
		 \param[in] posY The position in the Y-axis
		 \param[in] posZ The position in the Z-axis

		 \par Example:
		 \code
		 ae::Camera3D camera;
		 camera.setPosition(0.f, 0.f, 3.f);
		 \endcode

		 \sa move(), getPosition()

		 \since v0.6.0
		*/
		void setPosition(float posX, float posY, float posZ) noexcept;
		/*!
		 \brief Sets the rotation of the ae::Camera by providing an \a angle in radians and the \a axes of rotation.
		 \details This method replaces the ae::Camera's current rotation with the rotation provided.

		 \param[in] angle The angle of rotation in radians
		 \param[in] axes A unit ae::Vector3f (normalized) containing the affected axes of rotation

		 \par Example:
		 \code
		 ae::Camera3D camera;
		 camera.setRotation(ae::Math::toRadians(45.f), ae::Vector3f::Right);
		 \endcode

		 \sa rotate(), getRotation()

		 \since v0.4.0
		*/
		void setRotation(float angle, const Vector3f& axes) noexcept;
		/*!
		 \brief Sets the distance of the near and far planes in the Z-axis from the ae::Camera.
		 \details Vertices situated closer than the set near plane and farther from the set far plane will be ignored (not rendered).
		 \note If the near plane provided is greater than the far plane or vice versa, the values will be accepted but a warning log will be generated.

		 \param[in] nearPlane The distance from the camera to the near plane in the Z-axis, -1.f by default
		 \param[in] farPlane The distance from the camera to the far plane in the Z-axis, 1.f by default

		 \par Example:
		 \code
		 ae::Camera3D camera;
		 ...
		 camera.setFrustum(0.5f, 2500.f);
		 \endcode

		 \sa getFrustum()

		 \since v0.4.0
		*/
		void setFrustum(float nearPlane, float farPlane) noexcept;
		/*!
		 \brief Sets the target viewport.
		 \details The viewport is the rectangle into which the contents of the view are displayed.
		 The viewport coordinates are expressed as factors between 0 and 1.

		 \param[in] viewport The ae::Box2f containing the the factors of the size of the associated ae::RenderTarget

		 \par Example:
		 \code
		 ae::Camera2D camera;
		 ...
		 // Set the viewport as the left half of the render target
		 camera.setViewport(ae::Box2f(0.f, 0.f, 0.5f, 1.f));
		 \endcode

		 \sa getViewport()

		 \since v0.4.0
		*/
		void setViewport(const Box2f& viewport) noexcept;

		/*!
		 \brief Moves the ae::Camera according to the \a offset provided.
		 \details This method adds the \a offset to the ae::Camera's current position.

		 \param[in] offset An ae::Vector3f containing the displacement

		 \par Example:
		 \code
		 ae::Camera3D camera;
		 camera.move(ae::Vector3f(1.f, 0.f, 0.f));
		 \endcode

		 \sa setPosition(), getPosition()

		 \since v0.4.0
		*/
		void move(const Vector3f& offset) noexcept;
		/*!
		 \brief Moves the ae::Camera according to the offset coordinates provided.
		 \details This method adds the offset coordinates to the ae::Camera's current position.

		 \param[in] offsetX The offset in the X-axis
		 \param[in] offsetY The offset in the Y-axis
		 \param[in] offsetZ The offset in the Z-axis

		 \par Example:
		 \code
		 ae::Camera3D camera;
		 camera.move(1.f, 0.f, 0.f);
		 \endcode

		 \sa setPosition(), getPosition()

		 \since v0.6.0
		*/
		void move(float offsetX, float offsetY, float offsetZ) noexcept;
		/*!
		 \brief Rotates the ae::Camera based on the \a angle in radians and the \a axes of rotation provided.
		 \details This method applies a rotation to the ae::Camera's current rotation.

		 \param[in] angle The angle of rotation in radians
		 \param[in] axes A unit ae::Vector3f (normalized) containing the affected axes of rotation

		 \par Example:
		 \code
		 ae::Camera3D camera;
		 camera.rotate(ae::Math::toRadians(45.f), ae::Vector3f::Up);
		 \endcode

		 \sa setRotation(), getRotation()

		 \since v0.4.0
		*/
		void rotate(float angle, const Vector3f& axes) noexcept;
		/*!
		 \brief Rotates the ae::Camera so that it faces the point of \a focus provided.
		 \details This method replaces the ae::Camera's current rotation.

		 \param[in] focus An ae::Vector3f containing the position of the point of focus

		 \par Example:
		 \code
		 ae::Camera3D camera;
		 camera.lookAt(ae::Vector3f(5.f, -2.f, -8.f));
		 \endcode

		 \since v0.5.0
		*/
		void lookAt(const Vector3f& focus) noexcept;
		/*!
		 \brief Sets the ae::Camera's associated ae::RenderTarget.
		 \note This method is called automatically when a camera is set for the ae::RenderTarget.

		 \param[in] target The ae::RenderTarget associated with this ae::Camera

		 \par Example:
		 \code
		 ae::Camera2D camera;
		 mWindow.setCamera(camera);
		 \endcode

		 \since v0.4.0
		*/
		void setTarget(const RenderTarget* const target) noexcept;
		/*!
		 \brief Retrieves the ae::Camera's local directional vector pointing rightwards.
		 \details This directional vector differs from the global vector ae::Vector3f::Right as it depends on the position and the rotation of the ae::Camera instance.

		 \return An ae::Vector3f containing the local directional vector pointing rightwards

		 \par Example:
		 \code
		 ae::Camera3D camera;
		 ...
		 // The 'dt' variable is found in the overloaded 'update()' method of classes deriving from ae::State
		 float speed = 50.f;
		 camera.move(camera.getLocalRight() * speed * dt.asSeconds()); // the camera moves rightwards (along the X-axis)
		 \endcode

		 \sa getLocalUp(), getLocalForward()

		 \since v0.4.0
		*/
		_NODISCARD Vector3f getLocalRight();
		/*!
		 \brief Retrieves the ae::Camera's local directional vector pointing upwards.
		 \details This directional vector differs from the global vector ae::Vector3f::Up as it depends on the position and the rotation of the ae::Camera instance.

		 \return An ae::Vector3f containing the local directional vector pointing upwards

		 \par Example:
		 \code
		 ae::Camera3D camera;
		 ...
		 // The 'dt' variable is found in the overloaded 'update()' method of classes deriving from ae::State
		 float speed = 50.f;
		 camera.move(camera.getLocalUp() * speed * dt.asSeconds()); // the camera moves upwards (along the Y-axis)
		 \endcode

		 \sa getLocalRight(), getLocalForward()

		 \since v0.4.0
		*/
		_NODISCARD Vector3f getLocalUp();
		/*!
		 \brief Retrieves the ae::Camera's local directional vector pointing forwards.
		 \details This directional vector differs from the global vector ae::Vector3f::Forward as it depends on the position and the rotation of the ae::Camera instance.

		 \return An ae::Vector3f containing the local directional vector pointing forwards

		 \par Example:
		 \code
		 ae::Camera3D camera;
		 ...
		 // The 'dt' variable is found in the overloaded 'update()' method of classes deriving from ae::State
		 float speed = 50.f;
		 camera.move(camera.getLocalForward() * speed * dt.asSeconds()); // the camera moves forwards (along the Z-axis)
		 \endcode

		 \sa getLocalRight(), getLocalUp()

		 \since v0.4.0
		*/
		_NODISCARD Vector3f getLocalForward();
		/*!
		 \brief Retrieves the ae::Camera's position.

		 \return An ae::Vector3f containing the current position

		 \par Example:
		 \code
		 ae::Camera3D camera;
		 ...
		 const ae::Vector3f& cameraPosition = camera.getPosition();
		 \endcode

		 \sa setPosition()

		 \since v0.4.0
		*/
		_NODISCARD const Vector3f& getPosition() const noexcept;
		/*!
		 \brief Retrieves the ae::Camera's distances to the near and the far planes.
		 \details The references to the parameters provided will be overwritten with the near and far plane distance values.

		 \param[in,out] nearPlane The reference to a float that will be overwritten with the ae::Camera's distance to the near plane
		 \param[in,out] farPlane The reference to a float that will be overwritten with the ae::Camera's distance to the far plane

		 \par Example:
		 \code
		 ae::Camera3D camera;
		 ...
		 float nearPlane, farPlane;
		 camera.getFrustum(nearPlane, farPlane); // nearPlane and farPlane now contain the camera's distances to the two planes
		 \endcode

		 \sa setFrustum()

		 \since v0.4.0
		*/
		void getFrustum(float& nearPlane, float& farPlane) const noexcept;
		/*!
		 \brief Retrieves the target viewport.
		 \details The viewport is the rectangle into which the contents of the view are displayed.
		 The viewport coordinates are expressed as factors between 0 and 1.

		 \return The ae::Box2f containing the the factors of the size of the associated ae::RenderTarget

		 \par Example:
		 \code
		 ae::Camera2D camera;
		 ...
		 // Set the viewport as the left half of the render target
		 camera.setViewport(ae::Box2f(0.f, 0.f, 0.5f, 1.f));
		 ...
		 const ae::Box2f& viewport = camera.getViewport();
		 \endcode

		 \sa setViewport()

		 \since v0.4.0
		*/
		_NODISCARD const Box2f& getViewport() const noexcept;
		/*!
		 \brief Retrieves the ae::Camera's inverse view matrix.
		 \details The inverse view matrix will be updated (if necessary) before being retrieved.

		 \return An ae::Matrix4f containing the inverse view matrix

		 \par Example:
		 \code
		 ae::Camera3D camera;
		 ...
		 const ae::Matrix4f& invViewMatrix = camera.getInverseViewMatrix();
		 \endcode

		 \sa getViewMatrix()

		 \since v0.4.0
		*/
		_NODISCARD const Matrix4f& getInverseViewMatrix();
		/*!
		 \brief Retrieves the ae::Camera's inverse projection matrix.
		 \details The inverse projection matrix will be updated (if necessary) before being retrieved.

		 \return An ae::Matrix4f containing the inverse projection matrix

		 \par Example:
		 \code
		 ae::Camera3D camera;
		 ...
		 const ae::Matrix4f& invProjectionMatrix = camera.getInverseProjectionMatrix();
		 \endcode

		 \sa getProjectionMatrix()

		 \since v0.5.0
		*/
		_NODISCARD const Matrix4f& getInverseProjectionMatrix();

		// Public virtual method(s)
		/*!
		 \brief Retrieves the ae::Camera's current rotation.

		 \return An ae::Quaternion containing the current rotation

		 \par Example:
		 \code
		 ae::Camera3D camera;
		 ...
		 const ae::Quaternion& cameraRotation = camera.getRotation();
		 \endcode

		 \sa setRotation()

		 \since v0.4.0
		*/
		_NODISCARD virtual const Quaternion& getRotation();
		/*!
		 \brief Retrieves the ae::Camera's view matrix.
		 \details The view matrix will be updated (if necessary) before being retrieved.

		 \par Example:
		 \code
		 ae::Camera3D camera;
		 ...
		 const ae::Matrix4f& viewMatrix = camera.getViewMatrix();
		 \endcode

		 \sa getInverseViewMatrix(), getProjectionMatrix()

		 \since v0.5.0
		*/
		_NODISCARD virtual const Matrix4f& getViewMatrix();
		/*!
		 \brief Retrieves the ae::Camera's projection matrix.
		 \note All derived classes are required to override this pure virtual method.

		 \return An ae::Matrix4f containing the projection matrix

		 \par Example:
		 \code
		 ae::Camera3D camera;
		 ...
		 const ae::Matrix4f& projectionMatrix = camera.getProjectionMatrix();
		 \endcode

		 \sa getViewMatrix()

		 \since v0.4.0
		*/
		_NODISCARD virtual const Matrix4f& getProjectionMatrix() = 0;
	protected:
		// Protected constructor(s)
		/*!
		 \brief Constructs the ae::Camera by providing the distances to the Z-axis planes.
		 \note No direct instance of this class may be created.

		 \param[in] nearPlane The distance from the camera to the near plane in the Z-axis
		 \param[in] farPlane The distance from the camera to the far plane in the Z-axis
		 
		 \since v0.4.0
		*/
		Camera(float nearPlane, float farPlane);
		/*!
		 \brief Copy constructor.

		 \param[in] copy The ae::Camera that will be copied

		 \since v0.6.0
		*/
		Camera(const Camera& copy) = default;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::Camera rvalue that will be moved

		 \since v0.5.0
		*/
		Camera(Camera&& rvalue) noexcept;
	protected:
		// Protected operator(s)
		/*!
		 \brief Assignment operator.

		 \param[in] other The ae::Camera that will be copied

		 \return The caller ae::Camera

		 \since v0.6.0
		*/
		Camera& operator=(const Camera& other) = default;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::Camera that will be moved

		 \return The caller ae::Camera

		 \since v0.5.0
		*/
		Camera& operator=(Camera&& rvalue) noexcept;

	protected:
		// Protected member(s)
		Matrix4f            mViewMatrix;                //!< The view matrix
		Matrix4f            mProjectionMatrix;          //!< The projection matrix
		Quaternion          mRotation;                  //!< The camera's rotation
		bool                mUpdateViewMatrix;          //!< Whether or not the view matrix must be updated
		bool                mUpdateInvViewMatrix;       //!< Whether or not the inverse view matrix must be updated
		bool                mUpdateProjectionMatrix;    //!< Whether or not the projection matrix must be updated
		bool                mUpdateInvProjectionMatrix; //!< Whether or not the inverse projection matrix must be updated
		const RenderTarget* mTarget;                    //!< The associated render target
	private:
		// Private member(s)
		Matrix4f            mInvViewMatrix;             //!< The inverse view matrix
		Matrix4f            mInvProjectionMatrix;       //!< The inverse projection matrix
		Box2f               mViewport;                  //!< The viewport rectangle
		Vector3f            mPosition;                  //!< The camera's position
		float               mNearPlane;                 //!< The distance to the near plane in the Z-axis
		float               mFarPlane;                  //!< The distance to the far plane in the Z-axis
	};
}
#endif // Aeon_Graphics_Camera_H_

/*!
 \class ae::Camera
 \ingroup graphics
 
 The ae::Camera abstract base class is used to represent how a scene is
 displayed to the application user. Using one of the derived classes, the
 entire scene display can be moved and rotated without modifying how all the
 scene objects are rendered.

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2020.08.27
 \copyright MIT License
*/