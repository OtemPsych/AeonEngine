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

#ifndef	Aeon_Graphics_BatchRenderer2D_H_
#define Aeon_Graphics_BatchRenderer2D_H_

#include <map>

#include <AEON/Math/Matrix.h>
#include <AEON/Graphics/BlendMode.h>
#include <AEON/Graphics/RenderStates.h>
#include <AEON/Graphics/internal/Renderer2D.h>

namespace ae
{
	// Forward declaration(s)
	class Shader;
	class Texture;

	/*!
	 \brief Singleton class used as the standard 2D batch renderer.
	*/
	class AEON_API BatchRenderer2D : public Renderer2D
	{
	public:
		// Public constructor(s)
		BatchRenderer2D(const BatchRenderer2D&) = delete;
		BatchRenderer2D(BatchRenderer2D&&) = delete;

		// Public operator(s)
		BatchRenderer2D& operator=(const BatchRenderer2D&) = delete;
		BatchRenderer2D& operator=(BatchRenderer2D&&) = delete;

		// Public virtual method(s)
		virtual void endScene() override final;
		virtual void submit(const std::vector<Vertex2D>& vertices, const std::vector<uint32_t>& indices, const RenderStates& states) override final;
		virtual void submit(const Render2DComponent& component, const RenderStates& states) override final;

		// Public static method(s)
		static BatchRenderer2D& getInstance();

	private:
		// Private struct(s)
		struct SubmissionData
		{
			RenderStates                 states;
			const std::vector<Vertex2D>* vertices;
			const std::vector<uint32_t>* indices;
		};

		// Private constructor(s)
		BatchRenderer2D();

		// Private method(s)
		void flush();
		[[nodiscard]] bool isSubmissionBatchable(const RenderStates& currentStates, const RenderStates& newStates);
		void batchSubmission(const SubmissionData& submission, std::vector<Vertex2D>& vertices, std::vector<uint32_t>& indices);

	private:
		// Private member(s)
		std::vector<SubmissionData> mSubmissions;
		uint32_t                    mIndexOffset;
	};
}
#endif // Aeon_Graphics_BatchRenderer2D_H_

/*!
 \class ae::BatchRenderer2D
 \ingroup graphics

 The ae::BatchRenderer2D singleton class is the default 2D renderer which
 automatically batches similar render calls. The ae::Renderable2D instances'
 submitted will be queried for their vertices and corresponding indices which
 will be rendered as a batch, thus improving performance.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2021.07.22
 \copyright MIT License
*/