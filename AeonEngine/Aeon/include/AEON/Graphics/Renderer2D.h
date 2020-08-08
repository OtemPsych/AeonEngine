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

#ifndef	Aeon_Graphics_Renderer2D_H_
#define Aeon_Graphics_Renderer2D_H_

#include <map>

#include <AEON/Config.h>

#include <AEON/Graphics/internal/VertexArray.h>
#include <AEON/Graphics/internal/VertexBuffer.h>
#include <AEON/Graphics/internal/IndexBuffer.h>
#include <AEON/Graphics/internal/UniformBuffer.h>
#include <AEON/Graphics/GLResourceFactory.h>
#include <AEON/Graphics/Sprite.h>
#include <AEON/Graphics/Renderable2D.h>
#include <AEON/Graphics/internal/RenderTarget.h>

namespace ae
{
	class AEON_API Renderer2D
	{
	private:
		struct SubmissionData {
			size_t vertexOffset;
			size_t indexCount;
			bool   resubmitted;
		};
		struct RenderData {
			std::vector<Vertex2D>     vertices;
			std::vector<unsigned int> indices;
			Matrix4f                  transform;
			unsigned int              indexOffset;

			std::map<const std::vector<Vertex2D>*, SubmissionData> subData;
		};

	public:
		// Public constructor(s)
		/*!
		 \brief Deleted copy constructor.

		 \since v0.4.0
		*/
		Renderer2D(const Renderer2D&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.4.0
		*/
		Renderer2D(Renderer2D&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.4.0
		*/
		Renderer2D& operator=(const Renderer2D&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.4.0
		*/
		Renderer2D& operator=(Renderer2D&&) = delete;
	public:
		void beginScene(RenderTarget& target);
		void endScene();
		void submit(const Renderable2D& renderable, const RenderStates& states, bool dirty);
		void submit(const std::vector<Vertex2D>& vertices, const std::vector<unsigned int>& indices, const RenderStates& states, bool dirty);

		// Public static method(s)
		/*!
		 \brief Retrieves the single instance of the ae::Renderer2D.

		 \return The single instance of the ae::Renderer2D

		 \par Example:
		 \code
		 ae::Renderer2D& renderer2D = ae::Renderer2D::getInstance();
		 \endcode

		 \since v0.4.0
		*/
		_NODISCARD static Renderer2D& getInstance();
	private:
		// Private constructor(s)
		/*!
		 \brief Default constructor.
		 \details Setups the VAOs, VBOs, IBOs and the UBOs.

		 \since v0.4.0
		*/
		Renderer2D();
	private:
		void deleteSubmissions(RenderData& renderData);

	private:
		using TexturePass = std::map<const Texture*, RenderData>;
		using BlendPass = std::map<BlendMode, TexturePass>;
		using ShaderPass = std::map<const Shader*, BlendPass>;
	private:
		std::shared_ptr<UniformBuffer> mTransformUBO;
		std::shared_ptr<VertexArray>   mSpriteVAO;
		RenderTarget*                  mRenderTarget;
		std::shared_ptr<Texture2D>     mWhiteTexture;

		ShaderPass                     mDrawcalls;
	};
}
#endif // Aeon_Graphics_Renderer2D_H_