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

#include <vector>

#include <AEON/Math/Vector.h>
#include <AEON/Graphics/Component.h>

namespace ae
{
	/*!
	 \brief Struct representing a 2-dimensional vertex which ae::Render2DComponent entities will use.
	*/
	struct Vertex2D
	{
		Vector3f position; //!< The vertex's position
		Vector4f color;    //!< The vertex's normalized color
		Vector2f uv;       //!< The vertex's texture coordinates
	};

	/*!
	 \brief Class representing 2D entities' component that indicates that the entity can be drawn to a framebuffer.
	*/
	class AEON_API Render2DComponent : public Component
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the component by providing the associated actor.

		 \param[in] associatedActor The ae::Actor to which the component is attached

		 \since v0.7.0
		*/
		explicit Render2DComponent(Actor& associatedActor) noexcept;
		/*!
		 \brief Copy constructor.

		 \since v0.7.0
		*/
		Render2DComponent(const Render2DComponent&) = default;
		/*!
		 \brief Move constructor.

		 \since v0.7.0
		*/
		Render2DComponent(Render2DComponent&&) noexcept = default;

		// Public operator(s)
		/*!
		 \brief Assignment operator.

		 \return The caller ae::RenderComponent2D

		 \since v0.7.0
		*/
		Render2DComponent& operator=(const Render2DComponent&) = default;
		/*!
		 \brief Move assignment operator.

		 \return The caller ae::RenderComponent2D

		 \since v0.7.0
		*/
		Render2DComponent& operator=(Render2DComponent&&) noexcept = default;

		// Public method(s)
		/*!
		 \brief Retrieves the vertices defining the shape.

		 \return The vertices to be passed on to a renderer

		 \since v0.7.0
		*/
		[[nodiscard]] inline std::vector<Vertex2D>& getVertices() noexcept { return mVertices; }
		/*!
		 \brief Retrieves the vertices defining the shape.

		 \return The vertices to be passed on to a renderer

		 \since v0.7.0
		*/
		[[nodiscard]] inline const std::vector<Vertex2D>& getVertices() const noexcept { return mVertices; }
		/*!
		 \brief Retrieves the indices reducing vertex redundancy.

		 \return The indices to be passed on to a renderer

		 \since v0.7.0
		*/
		[[nodiscard]] inline std::vector<uint32_t>& getIndices() noexcept { return mIndices; }
		/*!
		 \brief Retrieves the indices reducing vertex redundancy.

		 \return The indices to be passed on to a renderer

		 \since v0.7.0
		*/
		[[nodiscard]] inline const std::vector<uint32_t>& getIndices() const noexcept { return mIndices; }

	private:
		// Private member(s)
		std::vector<Vertex2D> mVertices; //!< The vertices to be passed on to a renderer
		std::vector<uint32_t> mIndices;  //!< The indices to be passed on to a renderer
	};
}

/*!
 \class ae::Render2DComponent
 \ingroup graphics

 The ae::Render2DComponent class is used to be attach to 2D entities that can
 be drawn to a framebuffer, meaning a ae::RenderTarget.

 The ae::Renderable2D abstract base class is used to represent entities that
 can be rendered to the screen or, more specifically, passed to a renderer to
 then be rendered to a render target. They hold a list of vertices (most often
 4) and a list of indices that will automatically be passed on to the GPU.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2022.01.02
 \copyright MIT License
*/