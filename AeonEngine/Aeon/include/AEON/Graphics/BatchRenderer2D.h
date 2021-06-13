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

#ifndef	Aeon_Graphics_BatchRenderer2D_H_
#define Aeon_Graphics_BatchRenderer2D_H_

#include <map>

#include <AEON/Config.h>
#include <AEON/Math/Matrix.h>
#include <AEON/Graphics/internal/Renderer2D.h>

namespace ae
{
	// Forward declaration(s)
	struct BlendMode;
	class Shader;
	class Texture;

	/*!
	 \brief Singleton class used as the standard 2D batch renderer.
	*/
	class AEON_API BatchRenderer2D : public Renderer2D
	{
	private:
		// Private struct(s)
		/*!
		 \brief The internal struct used to cache each submission's metadata.
		*/
		struct SubmissionData {
			Matrix4f                         transform;   //!< The transform that needs to be applied
			const std::vector<Vertex2D>*     vertexList;  //!< The list of vertices
			const std::vector<unsigned int>* indexList;   //!< The list of indices
			bool                             resubmitted; //!< Whether the submission should remain cached
			bool                             dirty;       //!< Whether the submission's render data have been modified
		};
		/*!
		 \brief The internal struct representing a batch.
		*/
		struct RenderData {
			std::vector<Vertex2D>                                   vertices;    //!< The list of vertices of all submissions in the batch
			std::vector<unsigned int>                               indices;     //!< The list of indices of all submissions in the batch
			std::vector<SubmissionData>                             submissions; //!< The list of submissions in the batch
			unsigned int                                            indexOffset; //!< The offset used to increment each individual submission's indices

			std::map<const std::vector<Vertex2D>*, SubmissionData*> lookup;      //!< The hashmap of submissions and their corresponding metadata (used to check resubmissions faster)
		};
	private:
		// Private typedef(s)
		using TexturePasses = std::map<const Texture*, RenderData>;
		using BlendPasses = std::map<BlendMode, TexturePasses>;
		using ShaderPasses = std::map<const Shader*, BlendPasses>;

	public:
		// Public constructor(s)
		/*!
		 \brief Deleted copy constructor.

		 \since v0.6.0
		*/
		BatchRenderer2D(const BatchRenderer2D&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.6.0
		*/
		BatchRenderer2D(BatchRenderer2D&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.6.0
		*/
		BatchRenderer2D& operator=(const BatchRenderer2D&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.6.0
		*/
		BatchRenderer2D& operator=(BatchRenderer2D&&) = delete;
	public:
		// Public virtual method(s)
		/*!
		 \brief Indicates to the ae::BatchRenderer2D to batch together all submissions received and render them.
		 \details The submissions received will be rendered onto the ae::RenderTarget provided when the scene began.
		 \note The beginScene() and submit() methods must be called prior to calling this method for correct results.

		 \par Example:
		 \code
		 // Retrieve the ae::BatchRenderer2D's single instance
		 ae::BatchRenderer2D& renderer = ae::BatchRenderer2D::getInstance();

		 // Create the scene graph (the root node) and attach children nodes
		 auto sceneGraph = std::make_unique<ae::Actor2D>();
		 ...

		 // Indicate to the renderer to begin a new scene and to use the window as the render target
		 renderer.beginScene(mWindow);

		 // Start submitting the children nodes to the active renderer
		 sceneGraph->render(ae::BlendMode::BlendAlpha);

		 // The children nodes' render properties will be batched together and rendered simultaneously
		 renderer.endScene();
		 \endcode

		 \sa submit()

		 \since v0.6.0
		*/
		virtual void endScene() override final;
		/*!
		 \brief Adds a submission to the ae::BatchRenderer2D to be placed into a batch and rendered once the scene ends.
		 \note This method is automatically called by Aeon's derived classes of ae::Renderable2D.\n
		 The beginScene() method must be called prior to calling this method for correct results.

		 \param[in] vertices The list of vertices to be rendered
		 \param[in] indices The list of associated indices to be rendered
		 \param[in] states The ae::RenderStates (texture, transform, blend mode, shader) to be applied to the geometry

		 \sa endScene()

		 \since v0.6.0
		*/
		virtual void submit(const std::vector<Vertex2D>& vertices, const std::vector<unsigned int>& indices, const RenderStates& states) override final;

		// Public static method(s)
		/*!
		 \brief Retrieves the single instance of the ae::BatchRenderer2D.

		 \return The single instance of the ae::Renderer2D

		 \par Example:
		 \code
		 ae::BatchRenderer2D& renderer = ae::BatchRenderer2D::getInstance();
		 \endcode

		 \since v0.6.0
		*/
		static BatchRenderer2D& getInstance();
	private:
		// Private constructor(s)
		/*!
		 \brief Default constructor.

		 \since v0.6.0
		*/
		BatchRenderer2D();
	private:
		// Private method(s)
		/*!
		 \brief Renders the batches of renderables.
		 \details Performs multiple shader passes, blend passes and texture passes.

		 \param[in] drawcalls The list containing the opaque or the transparent renderables
		 \param[in] frontToBack Whether to render the renderables front-to-back or back-to-front

		 \since v0.6.0
		*/
		void flush(ShaderPasses& drawcalls, bool frontToBack);
		/*!
		 \brief Sorts the submissions received based on their Z-index and the rendering order.
		 \details The submissions will also be stored in a single vertex and index list to be drawn as a batch.

		 \param[in] data The data containing the submissions to be batched together
		 \param[in] frontToBack Whether to sort the renderables front-to-back or back-to-front

		 \sa resetSubmissions()

		 \since v0.6.0
		*/
		void sortSubmissions(RenderData& data, bool frontToBack);
		/*!
		 \brief Deletes the renderables that weren't resubmitted and resets the remaining renderables' resubmission flags.

		 \param[in] data The data containing the submissions

		 \sa sortSubmissions()

		 \since v0.6.0
		*/
		void resetSubmissions(RenderData& data);

	private:
		// Private member(s)
		ShaderPasses mOpaqueCalls;      //!< The list of all drawcalls for opaque renderables
		ShaderPasses mTransparentCalls; //!< The list of all drawcalls for transparent renderables
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

 The ae::Renderable2D instances submitted are additionally cached, meaning that
 the batches won't be recreated every frame if they haven't been modified.

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2020.08.31
 \copyright MIT License
*/