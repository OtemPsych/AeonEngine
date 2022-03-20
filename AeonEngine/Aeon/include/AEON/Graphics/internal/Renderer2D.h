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

#ifndef	Aeon_Graphics_Renderer2D_H_
#define Aeon_Graphics_Renderer2D_H_

#include <vector>
#include <memory>

#include <AEON/Config.h>

namespace ae
{
	// Forward declaration(s)
	struct RenderStates;
	struct Vertex2D;
	class RenderTarget;
	class Render2DComponent;
	class Texture2D;
	class UniformBuffer;
	class VertexArray;

	/*!
	 \brief Abstract base class representing a 2D renderer.
	*/
	class AEON_API Renderer2D
	{
	private:
		// Private static member(s)
		static Renderer2D* activeInstance; //!< The currently active renderer instance

	public:
		// Public constructor(s)
		/*!
		 \brief Deleted copy constructor.

		 \since v0.6.0
		*/
		Renderer2D(const Renderer2D&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.6.0
		*/
		Renderer2D(Renderer2D&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.6.0
		*/
		Renderer2D& operator=(const Renderer2D&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.6.0
		*/
		Renderer2D& operator=(Renderer2D&&) = delete;
	public:
		// Public method(s)
		/*!
		 \brief Adds a submission to the ae::Renderer2D to be rendered.
		 \note This method is automatically called by Aeon's derived classes of ae::Renderable2D.\n
		 The beginScene() method must be called prior to calling this method for correct results.

		 \param[in] renderable The ae::Renderable2D which will be rendered
		 \param[in] states The ae::RenderStates (texture, transform, blend mode, shader) to be applied to the ae::Renderable2D

		 \sa beginScene(), endScene()

		 \since v0.6.0
		*/
		virtual void submit(const Render2DComponent& renderComponent, const RenderStates& states);

		// Public virtual method(s)
		/*!
		 \brief Setups the ae::Renderer2D for rendering to the \a target provided.
		 \details Uploads the ae::RenderTarget's camera's properties to the global transform UBO.

		 \param[in] target The ae::RenderTarget which will act as the scene's framebuffer

		 \sa submit(), endScene()

		 \since v0.6.0
		*/
		virtual void beginScene(RenderTarget& target);
		/*!
		 \brief Indicates to the ae::Renderer2D to perform any modifications prior to terminating rendering for the scene.
		 \details Unbinds the VAO used for the drawcalls, disables depth-testing and invalidates the pointer to the render target used in the scene.
		 \note The beginScene() and submit() methods must be called prior to calling this method for correct results.

		 \sa beginScene(), submit()

		 \since v0.6.0
		*/
		virtual void endScene();		
		/*!
		 \brief Adds a submission to the ae::Renderer2D to be rendered.
		 \note This method is automatically called by Aeon's derived classes of ae::Renderable2D.\n
		 The beginScene() method must be called prior to calling this method for correct results.

		 \param[in] vertices The list of vertices to be rendered
		 \param[in] indices The list of associated indices to be rendered
		 \param[in] states The ae::RenderStates (texture, transform, blend mode, shader) to be applied to the geometry

		 \sa beginScene(), endScene()

		 \since v0.6.0
		*/
		virtual void submit(const std::vector<Vertex2D>& vertices, const std::vector<uint32_t>& indices, const RenderStates& states) = 0;

		// Public static method(s)
		/*!
		 \brief Retrieves the currently-active ae::Renderer2D instance.
		 \details A renderer is set as active once its beginScene() method is called and deactivated with its endScene() method.
		 \note This static method is primarily of use to derived classes of ae::Renderable2D.

		 \return A pointer to the active ae::Renderer2D instance.

		 \since v0.6.0
		*/
		_NODISCARD static Renderer2D* const getActiveInstance() noexcept;
	protected:
		// Protected constructor(s)
		/*!
		 \brief Default constructor.
		 \details Retrieves the necessary OpenGL resources (global UBO, white texture, VAO).

		 \since v0.6.0
		*/
		Renderer2D();

	protected:
		// Protected member(s)
		std::shared_ptr<Texture2D>     mWhiteTexture; //!< A 1x1 white texture for untextured renderables
		std::shared_ptr<VertexArray>   mVAO;          //!< The VAO used for all drawcalls
		RenderTarget*                  mRenderTarget; //!< The scene's active render target
	private:
		// Private member(s)
		std::shared_ptr<UniformBuffer> mTransformUBO; //!< The global transform UBO
	};
}
#endif // Aeon_Graphics_Renderer2D_H_

/*!
 \class ae::Renderer2D
 \ingroup graphics

 The ae::Renderer2D abstract base class provides the basic functionality which
 all 2D renderers will have to adhere to. This class won't be of any use to the
 API user.

 \sa BatchRenderer2D, BasicRenderer2D

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2020.08.31
 \copyright MIT License
*/