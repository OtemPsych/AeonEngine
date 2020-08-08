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

#ifndef Aeon_Graphics_Actor2D_H_
#define Aeon_Graphics_Actor2D_H_

#include <AEON/System/Time.h>
#include <AEON/Window/Event.h>
#include <AEON/Graphics/Transformable2D.h>
#include <AEON/Graphics/Renderable2D.h>

namespace ae
{
	/*!
	 \brief Base class used in scene graph architecture.
	 \details The majority of user-defined classes will derive from this class or one of its derivatives.
	*/
	class _NODISCARD AEON_API Actor2D : public Transformable2D, public Renderable2D
	{
	public:
		// Public enum(s)
		/*!
		 \brief Enum used to (de)activate event handling, updating and/or rendering.
		 \details All of the enum values can be paired together to (de)activate several in the same call.
		*/
		enum Func {
			EventHandle = 1 << 0,
			Update      = 1 << 1,
			Render      = 1 << 2
		};
		/*!
		 \brief Enum used to (de)activate event handling, updating and/or rendering for a specific target.
		 \details The enum values can be paired together to (de)activate functionalities for both of them in the same call.
		*/
		enum Target {
			Self     = 1 << 0,
			Children = 1 << 1
		};

	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details Event handling, updating and rendering are activated by default.

		 \since v0.4.0
		*/
		Actor2D();
		/*!
		 \brief Deleted copy constructor.

		 \since v0.5.0
		*/
		Actor2D(const Actor2D&) = delete;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::Actor2D that will be moved

		 \since v0.5.0
		*/
		Actor2D(Actor2D&& rvalue) noexcept;
		/*!
		 \brief Virtual destructor.
		 \details A virtual destructor is needed as this class will be inherited.

		 \since v0.4.0
		*/
		virtual ~Actor2D();
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.5.0
		*/
		Actor2D& operator=(const Actor2D&) = delete;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::Actor2D that will be moved

		 \return The caller ae::Actor2D

		 \since v0.5.0
		*/
		Actor2D& operator=(Actor2D&& rvalue) noexcept;
	public:
		// Public method(s)
		/*!
		 \brief Attaches an ae::Actor2D child node to the caller ae::Actor2D.
		 \details The caller ae::Actor2D's transform will also be applied to the child.

		 \param[in] child An ae::Actor2D (or derivative) unique_ptr created before being moved

		 \par Example:
		 \code
		 std::unique_ptr<ae::Actor2D> parent;
		 std::unique_ptr<ae::Actor2D> child;

		 parent->attachChild(std::move(child));
		 \endcode

		 \sa detachChild()

		 \since v0.4.0
		*/
		void attachChild(std::unique_ptr<Actor2D> child);
		/*!
		 \brief Detaches a child node and retrieves it.
		 \details The detached node will be destroyed if not subsequently stored.

		 \param[in] child The stored child to detach

		 \return The detached child node

		 \par Example:
		 \code
		 std::unique_ptr<ae::Actor2D> parent;
		 std::unique_ptr<ae::Actor2D> child;

		 ae::Actor2D* childPtr = child.get();
		 parent->attachChild(std::move(child));
		 
		 ...

		 parent->detachChild(*childPtr);
		 \endcode

		 \sa attachChild()

		 \since v0.4.0
		*/
		std::unique_ptr<Actor2D> detachChild(const Actor2D& child);
		/*!
		 \brief Relatively aligns the caller ae::Transformable2D to another based on the flags provided.
		 \details Can be used to easily center text inside a rectangle, placing elements below/above/next to other elements.
		 \note This method has no effect if the caller doesn't have a parent.\n
		 Padding isn't taken into account if the alignment flags center the caller.

		 \param[in] alignmentFlags The ae::Transformable2D::OriginFlag to set using the OR bit operator
		 \param[in] padding The padding (spacing) from the edge of the alignment \a target (not taken into account if the caller is centered)

		 \par Example:
		 \code
		 // Create the parent and child sprites
		 auto parentSprite = std::make_unique<ae::Sprite>(someTexture);
		 auto childSprite = std::make_unique<ae::Sprite>(otherTexture);

		 ae::Sprite* childSpritePtr = childSprite.get();
		 parentSprite->attachChild(std::move(childSprite));

		 // Set the origin of the child sprite to its center and align it to the center of the parent
		 childSpritePtr->setOriginFlags(ae::Sprite::OriginFlag::Center);
		 childSpritePtr->setRelativeAlignment(ae::Sprite::OriginFlag::Center);
		 \endcode

		 \since v0.5.0
		*/
		void setRelativeAlignment(uint32_t alignmentFlags, float padding = 0.f);
		/*!
		 \brief Sends the event received to the current node and all of its children nodes for processing.

		 \param[in] event The polled input event

		 \sa handleEventSelf(), handleEventChildren()

		 \since v0.5.0
		*/
		void handleEvent(Event* const event);
		/*!
		 \brief Updates the current nodes and all of its children nodes.

		 \param[in] dt The delta time between the current frame and the previous one

		 \sa updateSelf(), updateChildren()

		 \since v0.5.0
		*/
		void update(const Time& dt);
		/*!
		 \brief (De)Activates event handling, updating and/or rendering for the current node and/or its children.
		 \details Several functionalities and targets may be selected simultaneously.

		 \param[in] func The ae::Actor2D::Func to (de)activate
		 \param[in] target The ae::Actor2D::Target to (de)activate
		 \param[in] flag True to activate selected functionalities for the selected targets, false to deactivate

		 \par Example:
		 \code
		 // Deactivate event handling and updating for the object's children
		 std::unique_ptr<ae::Actor2D> actor;
		 actor->activateFunctionality(ae::Actor2D::Func::EventHandle | ae::Actor2D::Func::Update, ae::Actor2D::Target::Children, false);
		 \endcode

		 \since v0.4.0
		*/
		void activateFunctionality(uint32_t func, uint32_t target, bool flag);
		/*!
		 \brief Calculates and retrieves the global transform by multiplying every parent's transform until the root node is reached.

		 \return The ae::Actor2D's global transform

		 \sa getGlobalPosition(), getGlobalBounds()

		 \since v0.5.0
		*/
		_NODISCARD Matrix4f getGlobalTransform();
		/*!
		 \brief Calculates and retrieves the global axis-aligned bounding box of the ae::Actor2D.

		 \return The ae::Actor2D's global bounding box

		 \sa getModelBounds(), getGlobalTransform(), getGlobalPosition()

		 \since v0.4.0
		*/
		_NODISCARD Box2f getGlobalBounds();
		/*!
		 \brief Calculates and retrieves the ae::Actor2D's global position.
		 \details This is particularly useful if the caller ae::Actor2D is a child of another node causing it to have a position relative to its parent node.

		 \sa getGlobalTransform(), getGlobalBounds()

		 \since v0.4.0
		*/
		_NODISCARD Vector2f getGlobalPosition();

		// Public virtual method(s)
		/*!
		 \brief Informs the API user if the ae::Actor2D must be removed from the scene.
		 \details ae::Actor2D objects that are marked for removal are automatically removed from the scene.
		 \note The removal condition is defined by the API user.

		 \return True if the ae::Actor2D will be removed, false otherwise

		 \sa isDestroyed()

		 \since v0.4.0
		*/
		_NODISCARD virtual bool isMarkedForRemoval() const;
		/*!
		 \brief Informs the API user if the ae::Actor2D is destroyed.
		 \details This can be useful when, for example, a spaceship was destroyed but an explosion animation still needs to be played out before removing the ae::Actor2D from the scene.
		 \note The destruction condition is defined by the API user.

		 \return True if the ae::Actor2D is considered destroyed, false otherwise

		 \sa isMarkedForRemoval()

		 \since v0.4.0
		*/
		_NODISCARD virtual bool isDestroyed() const;
		/*!
		 \brief Recalculates the correct origin based on the origin flags, and realigns the actor relative to its parent.

		 \since v0.5.0
		*/
		virtual void correctProperties() override final;
		/*!
		 \brief Renders the current ae::Actor2D and its children.

		 \param[in] states The ae::RenderStates associated (texture, transform, blend mode, shader)

		 \sa renderSelf(), renderChilren()

		 \since v0.5.0
		*/
		virtual void render(RenderStates states) override final;
		/*!
		 \brief Retrieves the ae::Actor2D's model bounding box.
		 \note Derived classes should override this method to set the appropriate bounding box.

		 \return The ae::Actor2D's model bounding box, empty box by default

		 \sa getGlobalBounds()

		 \since v0.4.0
		*/
		_NODISCARD virtual Box2f getModelBounds() const override;
	protected:
		// Protected method(s)
		/*!
		 \brief Updates the z-index ordering of the caller and its children to achieve correct depth ordering.
		 \details The z-index provided will be assigned to the caller and its children will be assigned to a superior z-index.

		 \param[in] zIndex The depth index to assign to the caller

		 \since v0.5.0
		*/
		void updateZOrdering(int zIndex);
	private:
		// Private method(s)
		/*!
		 \brief Removes all children nodes that have been marked for removal.
		 \details This method is called during the updating.

		 \sa isMarkedForRemoval()

		 \since v0.4.0
		*/
		void removeChildrenMarkedForRemoval();
		/*!
		 \brief Sends the polled input \a event to the ae::Actor2D's attached children nodes.

		 \param[in] event The polled input ae::Event

		 \sa handleEventSelf(), handleEvent()

		 \since v0.5.0
		*/
		void handleEventChildren(Event* const event);
		/*!
		 \brief Sends the command to the ae::Actor2D's children to update themselves and their own children.
		 
		 \param[in] dt The time difference between the previous frame and the current frame

		 \sa updateSelf(), update()

		 \since v0.5.0
		*/
		void updateChildren(const Time& dt);
		/*!
		 \brief Sends the command to the ae::Actor2D's children to render themselves and their own children.

		 \param[in] states The ae::RenderStates defining the OpenGL state

		 \sa renderSelf(), render()

		 \since v0.5.0
		*/
		void renderChildren(RenderStates states) const;

		// Private virtual method(s)
		/*!
		 \brief The ae::Actor2D processes the polled input event.
		 \note The method's behaviour is defined by the derived class.

		 \param[in] event The polled input ae::Event

		 \sa handleEventChildren(), handleEvent()

		 \since v0.5.0
		*/
		virtual void handleEventSelf(Event* const event);
		/*!
		 \brief Updates the ae::Actor2D.
		 \note The method's behaviour is defined by the derived class.

		 \param[in] dt The time difference between the previous frame and the current frame

		 \sa updateChildren(), update()

		 \since v0.5.0
		*/
		virtual void updateSelf(const Time& dt);
		/*!
		 \brief Renders the ae::Actor2D.
		 \note The method's behaviour is defined by the derived class.

		 \param[in] states The ae::RenderStates defining the OpenGL state

		 \sa renderChildren(), render()

		 \since v0.5.0
		*/
		virtual void renderSelf(RenderStates states) const;

	protected:
		// Protected member(s)
		Actor2D*                                    mParent;    //!< The node's parent node
	private:
		// Private member(s)
		std::vector<std::unique_ptr<Actor2D>>       mChildren;  //!< The list of attached children nodes
		std::map<Func, std::map<Target, bool>>      mFuncs;     //!< The active functionalities
		std::pair<bool, std::pair<uint32_t, float>> mAlignment; //!< The relative alignment to the parent node
	};
}
#endif // Aeon_Graphics_Actor2D_H_

/*!
 \class ae::Actor2D
 \ingroup graphics

 The ae::Actor2D base class represents all game objects in a scene that form a
 scene tree to better organize and manage different entities. It provides
 functionality for attaching/detaching children nodes, event handling, updating
 and drawing the caller node and its children.

 \author Filippos Gleglakos
 \version v0.5.0
 \date 2020.08.07
 \copyright MIT License
*/