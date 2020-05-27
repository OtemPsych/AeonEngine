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

#ifndef Aeon_Graphics_Renderable2D_H_
#define Aeon_Graphics_Renderable2D_H_

#include <AEON/Config.h>
#include <AEON/Math/Vector2.h>
#include <AEON/Math/Vector3.h>
#include <AEON/Graphics/Color.h>
#include <AEON/Graphics/RenderStates.h>

namespace ae
{
	/*!
	 \brief Struct representing a 2-dimensional vertex which Renderable2D entities will use.
	*/
	struct Vertex2D
	{
		Vector2f position; //!< The vertex's position
		Vector2f uv;       //!< The vertex's texture coordinates
		Vector4f color;    //!< The vertex's normalized color
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
		 \details May also update the list of vertices if necessary.

		 \return The list of vertices

		 \since v0.4.0
		*/
		_NODISCARD const std::vector<Vertex2D>& getVertices();
		/*!
		 \brief Retrieves the list of indices defining the shape of the ae::Renderable2D.

		 \return The list of indices

		 \since v0.4.0
		*/
		_NODISCARD const std::vector<unsigned int>& getIndices();
		/*!
		 \brief Renders the ae::Renderable2D.
		 
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

		 \since v0.4.0
		*/
		virtual bool render(RenderStates states) = 0;
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
		 
		 \param[in] copy The ae::Renderable2D that'll be copied

		 \since v0.4.0
		*/
		Renderable2D(const Renderable2D& copy) = default;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::Renderable2D that'll be moved

		 \since v0.4.0
		*/
		Renderable2D(Renderable2D&& rvalue) = default;
	protected:
		// Protected operator(s)
		/*!
		 \brief Assignment operator.

		 \param[in] other The ae::Renderable2D that'll be copied

		 \return The caller ae::Renderable2D

		 \since v0.4.0
		*/
		Renderable2D& operator=(const Renderable2D& other) noexcept = default;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::Renderable2D that'll be moved

		 \return The caller ae::Renderable2D

		 \since v0.4.0
		*/
		Renderable2D& operator=(Renderable2D&& rvalue) noexcept = default;
	private:
		// Private virtual method(s)
		/*!
		 \brief Updates the ae::Renderable2D's stored vertices.
		 \note This method needs to be overwritten by derived classes for it to have any effect.

		 \sa updateIndices()

		 \since v0.4.0
		*/
		virtual void updateVertices();
		/*!
		 \brief Updates the ae::Renderable2D's stored indices.
		 \note This method needs to be overwritten by derived classes for it to have any effect.

		 \sa updateVertices()

		 \since v0.4.0
		*/
		virtual void updateIndices();

	protected:
		// Protected member(s)
		std::vector<Vertex2D>     mVertices;       //!< The list of vertices to be passed on to a renderer
		std::vector<unsigned int> mIndices;        //!< The list of indices to be passed on to a renderer
	private:
		// Private member(s)
		bool                      mUpdateIndices;  //!< Whether the list of indices must be updated
	};
}
#endif // Aeon_Graphics_Renderable2D_H_

/*!
 \class ae::Renderable2D
 \ingroup graphics

 The ae::Renderable2D abstract base class is used to represent entities that
 can be rendered to the screen or, more specifically, passed to a renderer to
 then be rendered to a render target. They hold a list of vertices (most often
 4) that will automatically be passed on to the GPU.

 \author Filippos Gleglakos
 \version v0.4.0
 \date 2020.05.14
 \copyright MIT License
*/