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

#pragma once

#include <AEON/Graphics/internal/Renderer2D.h>
#include <AEON/Graphics/RenderStates.h>

namespace ae
{
	/*!
	 \brief Singleton class used to directly render geometry.
	 \note The BatchRenderer2D should be used for practically all rendering by the API user.
	*/
	class AEON_API BasicRenderer2D : public Renderer2D
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Deleted copy constructor.

		 \since v0.6.0
		*/
		BasicRenderer2D(const BasicRenderer2D&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.6.0
		*/
		BasicRenderer2D(BasicRenderer2D&&) = delete;

		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.6.0
		*/
		BasicRenderer2D& operator=(const BasicRenderer2D&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.6.0
		*/
		BasicRenderer2D& operator=(BasicRenderer2D&&) = delete;

		// Public virtual method(s)
		virtual void beginScene(RenderTarget& target) override final;
		/*!
		 \brief Adds a submission to the ae::BasicRenderer2D which will be instantly rendered.
		 \note The beginScene() method must be called prior to calling this method for correct results.

		 \param[in] vertices The list of vertices to be rendered
		 \param[in] indices The list of associated indices to be rendered
		 \param[in] states The ae::RenderStates (texture, transform, blend mode, shader) to be applied to the geometry

		 \par Example:
		 \code
		 // Retrieve the ae::BasicRenderer2D's single instance
		 ae::BasicRenderer2D& renderer = ae::BasicRenderer2D::getInstance();

		 // Create the scene graph (the root node) and attach children nodes
		 auto sceneGraph = std::make_unique<ae::Actor2D>();
		 ...

		 // Indicate to the renderer to begin a new scene and to use the window as the render target
		 renderer.beginScene(mWindow);

		 // Start submitting the children nodes to the active renderer (they will be immediately rendered to the window)
		 sceneGraph->render();

		 // Indicate to the renderer to end the active scene which will allow a new scene to be commenced
		 renderer.endScene();
		 \endcode

		 \sa beginScene(), endScene()

		 \since v0.7.0
		*/
		virtual void submit(const std::vector<Vertex2D>& vertices, const std::vector<uint32_t>& indices, const RenderStates& states) override final;

		// Public static method(s)
		/*!
		 \brief Retrieves the single instance of the ae::BasicRenderer2D.

		 \return The single instance of the ae::BasicRenderer2D

		 \par Example:
		 \code
		 ae::BasicRenderer2D& renderer = ae::BasicRenderer2D::getInstance();
		 \endcode

		 \since v0.6.0
		*/
		static BasicRenderer2D& getInstance();
	private:
		// Private constructor(s)
		/*!
		 \brief Default constructor.

		 \since v0.7.0
		*/
		BasicRenderer2D() = default;
	};
}

/*!
 \class ae::BasicRenderer2D
 \ingroup graphics

 The ae::BasicRenderer2D singleton class is used to directly render any
 geometry submitted to it. This renderer won't attempt to batch similar render
 calls together to increase performance.

 It's used by Aeon to render geometry to render textures (such as text
 characters) which aren't rendered every frame.

 If the API user wishes to render the same geometry every frame, they should
 instead opt for the BatchRenderer2D which will optimize performance for
 continual rendering.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2022.01.26
 \copyright MIT License
*/