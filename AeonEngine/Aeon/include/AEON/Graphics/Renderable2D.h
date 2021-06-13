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

#ifndef Aeon_Graphics_Renderable2D_H_
#define Aeon_Graphics_Renderable2D_H_

#include <vector>

#include <AEON/Config.h>
#include <AEON/Math/Vector.h>
#include <AEON/Graphics/Color.h>
#include <AEON/Graphics/RenderStates.h>

namespace ae
{
	// Forward declaration(s)
	class Renderer2D;

	/*!
	 \brief Struct representing a 2-dimensional vertex which Renderable2D entities will use.
	*/
	struct Vertex2D
	{
		Vector3f position; //!< The vertex's position
		Vector4f color;    //!< The vertex's normalized color
		Vector2f uv;       //!< The vertex's texture coordinates
	};

	/*!
	 \brief Abstract base class representing entities which can be rendered / passed to a renderer.
	 \note No direct instances of this class may be created.
	*/
	class AEON_API Renderable2D
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Virtual destructor.
		 \details A virtual destructor is needed as this class will be inherited.

		 \since v0.4.0
		*/
		virtual ~Renderable2D();
	public:
		// Public method(s)
		/*!
		 \brief Retrieves the list of vertices defining the shape of the ae::Renderable2D.

		 \return The list of vertices

		 \since v0.4.0
		*/
		_NODISCARD const std::vector<Vertex2D>& getVertices() const noexcept;
		/*!
		 \brief Retrieves the list of indices defining the shape of the ae::Renderable2D.

		 \return The list of indices

		 \since v0.4.0
		*/
		_NODISCARD const std::vector<unsigned int>& getIndices() const noexcept;
		// Public virtual method(s)
		/*!
		 \brief Renders the ae::Renderable2D.

		 \param[in] states The ae::RenderStates associated (texture, transform, blend mode, shader)
		 
		 \par Example:
		 \code
		 // Retrieve the renderer instance to which the renderable objects will be passed on
		 ae::Renderer2D renderer = ae::Renderer2D::getInstance();

		 // Sets the camera to be used and prepares the renderer to receive renderables
		 renderer.beginScene(camera);

		 // Submits the sprites and their children with their associated render states to the active renderer (doesn't render them immediately)
		 ae::RenderStates renderStates;
		 for (ae::Sprite& sprite : sprites) {
			sprite.render(renderStates);
		 }

		 // Sends all the renderable objects' vertices to the GPU and renders them
		 renderer.endScene();
		 \endcode

		 \since v0.5.0
		*/
		virtual void render(RenderStates states = RenderStates()) = 0;
	protected:
		// Protected constructor(s)
		/*!
		 \brief Default constructor.
		 \details No direct instance of this class may be created.

		 \since v0.4.0
		*/
		Renderable2D() noexcept;
		/*!
		 \brief Copy constructor.

		 \param[in] copy The ae::Renderable2D that will be copied

		 \since v0.6.0
		*/
		Renderable2D(const Renderable2D& copy) = default;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::Renderable2D that'll be moved

		 \since v0.5.0
		*/
		Renderable2D(Renderable2D&& rvalue) noexcept;
	protected:
		// Protected operator(s)
		/*!
		 \brief Assignment operator.

		 \param[in] other The ae::Renderable2D that will be copied

		 \return The caller ae::Renderable2D

		 \since v0.6.0
		*/
		Renderable2D& operator=(const Renderable2D& other) = default;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::Renderable2D that'll be moved

		 \return The caller ae::Renderable2D

		 \since v0.5.0
		*/
		Renderable2D& operator=(Renderable2D&& rvalue) noexcept;
	protected:
		// Protected method(s)
		/*!
		 \brief Raises/Drops the ae::Renderable2D's dirty render flag.
		 \details The dirty render flag is used to inform the renderer to update its cached properties.

		 \param[in] True to raise the dirty render flag, false to drop it

		 \sa isDirty()

		 \since v0.6.0
		*/
		void setDirty(bool flag) const noexcept;
		/*!
		 \brief Whether or not the ae::Renderable2D's render properties need to be updated.
		 \details This flag needs to be passed on to a renderer to update its cached properties.

		 \return True if the render properties are dirty, false otherwise

		 \sa setDirty()

		 \since v0.5.0
		*/
		_NODISCARD bool isDirty() const noexcept;
		/*!
		 \brief Retrieves the list of vertices defining the shape of the ae::Renderable2D.

		 \return The list of vertices

		 \since v0.5.0
		*/
		_NODISCARD std::vector<Vertex2D>& getVertices() noexcept;
		/*!
		 \brief Retrieves the list of indices defining the shape of the ae::Renderable2D.

		 \return The list of indices

		 \since v0.5.0
		*/
		_NODISCARD std::vector<unsigned int>& getIndices() noexcept;

	private:
		// Private member(s)
		std::vector<Vertex2D>     mVertices; //!< The list of vertices to be passed on to a renderer
		std::vector<unsigned int> mIndices;  //!< The list of indices to be passed on to a renderer
		mutable bool              mDirty;    //!< Whether the render properties need to be updated
	};
}
#endif // Aeon_Graphics_Renderable2D_H_

/*!
 \class ae::Renderable2D
 \ingroup graphics

 The ae::Renderable2D abstract base class is used to represent entities that
 can be rendered to the screen or, more specifically, passed to a renderer to
 then be rendered to a render target. They hold a list of vertices (most often
 4) and a list of indices that will automatically be passed on to the GPU.

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2020.08.31
 \copyright MIT License
*/