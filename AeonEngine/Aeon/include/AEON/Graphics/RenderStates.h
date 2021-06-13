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

#ifndef Aeon_Graphics_RenderStates_H_
#define Aeon_Graphics_RenderStates_H_

#include <AEON/Math/Matrix.h>
#include <AEON/Graphics/BlendMode.h>

namespace ae
{
	// Forward declaration(s)
	class Texture;
	class Shader;

	/*!
	 \brief Struct representing the render states to apply prior to rendering.
	*/
	struct _NODISCARD AEON_API RenderStates
	{
		// Public member(s)
		BlendMode      blendMode; //!< The blend mode to apply
		Matrix4f       transform; //!< The transform that will be applied to the vertices
		const Texture* texture;   //!< The texture to apply
		const Shader*  shader;    //!< The shader used to display the vertices
		bool           dirty;     //!< Whether the corresponding renderable is marked as dirty

		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details Creates a default set of render states wherein: BlendAlpha, identity transform, null texture and null shader are used.

		 \since v0.6.0
		*/
		RenderStates() noexcept;
		/*!
		 \brief Constructs a default set of render states with a custom blend mode.

		 \param[in] blendMode The custom ae::BlendMode to use

		 \since v0.6.0
		*/
		RenderStates(const BlendMode& blendMode) noexcept;
		/*!
		 \brief Constructs a default set of render states with a custom transform.

		 \param[in] transform The custom ae::Matrix4f transform to use

		 \since v0.6.0
		*/
		RenderStates(const Matrix4f& transform) noexcept;
		/*!
		 \brief Constructs a default set of render states with a custom texture.

		 \param[in] texture The custom ae::Texture to use

		 \since v0.6.0
		*/
		RenderStates(const Texture& texture) noexcept;
		/*!
		 \brief Constructs a default set of render states with a custom shader.

		 \param[in] shader The custom ae::Shader to use

		 \since v0.6.0
		*/
		RenderStates(const Shader& shader) noexcept;
		/*!
		 \brief Constructs a custom set of render states.

		 \param[in] blendMode The custom ae::BlendMode to use
		 \param[in] transform The custom ae::Matrix4f transform to use
		 \param[in] texture The custom ae::Texture to use
		 \param[in] shader The custom ae::Shader to use

		 \since v0.6.0
		*/
		RenderStates(const BlendMode& blendMode, const Matrix4f& transform, const Texture& texture, const Shader& shader) noexcept;
		/*!
		 \brief Copy constructor.

		 \param[in] copy The ae::RenderStates that will be copied

		 \since v0.6.0
		*/
		RenderStates(const RenderStates& copy) = default;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::RenderStates that will be moved
		 
		 \since v0.6.0
		*/
		RenderStates(RenderStates&& rvalue) noexcept;

		// Public operator(s)
		/*!
		 \brief Assignment operator.

		 \param[in] other The ae::RenderStates that will be copied

		 \return The caller ae::RenderStates

		 \since v0.6.0
		*/
		RenderStates& operator=(const RenderStates& other) = default;
		/*!
		 \brief Move assignment operator.
		 
		 \param[in] rvalue The ae::RenderStates that will be moved

		 \return The caller ae::RenderStates

		 \since v0.6.0
		*/
		RenderStates& operator=(RenderStates&& rvalue) noexcept;
	};
}
#endif // Aeon_Graphics_RenderStates_H_

/*!
 \struct ae::RenderStates
 \ingroup graphics

 The ae::RenderStates struct is used to define the states for rendering.
 High-level objects such as sprites will automatically fill these states if
 they haven't been manually filled (the texture will always be modified).

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2020.08.29
 \copyright MIT License
*/