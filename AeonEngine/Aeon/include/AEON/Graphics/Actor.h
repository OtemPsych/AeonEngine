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

#ifndef Aeon_Graphics_Actor_H_
#define Aeon_Graphics_Actor_H_

#include <vector>
#include <memory>
#include <unordered_map>
#include <queue>

#include <AEON/Math/Vector.h>
#include <AEON/System/Time.h>
#include <AEON/Window/Event.h>
#include <AEON/Graphics/internal/ComponentManager.h>
#include <AEON/Graphics/RenderStates.h>

namespace ae
{
	// Forward declaration(s)
	class EasingAnimation;

	/*!
	 \brief Base class used in scene-graph architecture.
	 \details The majority of user-defined classes will derive from this class or one of its derivatives.
	*/
	class AEON_API Actor
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
			Render      = 1 << 2,
			AllFunc     = EventHandle | Update | Render
		};
		/*!
		 \brief Enum used to (de)activate event handling, updating and/or rendering for a specific target.
		 \details The enum values can be paired together to (de)activate functionalities for both of them in the same call.
		*/
		enum Target {
			Self      = 1 << 0,
			Children  = 1 << 1,
			AllTarget = Self | Children
		};

	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details Event handling, updating and rendering for all targets are activated by default.

		 \since v0.7.0
		*/
		Actor();
		/*!
		 \brief Copy constructor.
		 \note The parent, the children nodes and the components won't be copied.

		 \param[in] copy The ae::Actor that will be copied

		 \since v0.7.0
		*/
		Actor(const Actor& copy);
		/*!
		 \brief Move constructor.

		 \since v0.7.0
		*/
		Actor(Actor&&) noexcept = default;
		/*!
		 \brief Virtual destructor.
		 \details A virtual destructor is needed as this class will be inherited.

		 \since v0.7.0
		*/
		virtual ~Actor() = default;
	public:
		// Public operator(s)
		/*!
		 \brief Assignment operator.
		 \note The parent, the children nodes and the components won't be copied.

		 \param[in] other The ae::Actor that will be copied

		 \return The caller ae::Actor

		 \since v0.7.0
		*/
		Actor& operator=(const Actor& other);
		/*!
		 \brief Move assignment operator.

		 \return The caller ae::Actor

		 \since v0.7.0
		*/
		Actor& operator=(Actor&&) noexcept = default;
	public:
		// Public method(s)
		/*!
		 \brief Attaches a child node.
		 \details The parent's transform will also be applied to the child.

		 \param[in] child An ae::Actor (or derived class) created before being attached

		 \par Example:
		 \code
		 std::unique_ptr<ae::Actor> parent;
		 std::unique_ptr<ae::Actor> child;

		 parent->attachChild(std::move(child));
		 \endcode

		 \sa detachChild()

		 \since v0.7.0
		*/
		void attachChild(std::unique_ptr<Actor> child);
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

		 \since v0.7.0
		*/
		std::unique_ptr<Actor> detachChild(const Actor& child);
		/*!
		 \brief Sends the event received to the current node and all of its children nodes for processing.
		 \details The children handle the event before the current node.

		 \param[in] event The polled input event

		 \sa handleEventSelf()

		 \since v0.7.0
		*/
		void handleEvent(Event* const event);
		/*!
		 \brief Updates the current node and all of its children nodes.
		 \details The current node is updated before its children.

		 \param[in] dt The delta time between the current frame and the previous one

		 \sa updateSelf()

		 \since v0.7.0
		*/
		void update(const Time& dt);
		/*!
		 \brief Submits the current node and its children to the active renderer.

		 \param[in] states The ae::RenderStates associated (texture, transform, blend mode, shader)

		 \sa renderSelf()

		 \since v0.7.0
		*/
		void render(RenderStates states);
		/*!
		 \brief (De)Activates event handling, updating and/or rendering for the current node and/or its children.
		 \details The affected functionalities/targets are only the ones specified regardless if they're activated or deactivated.

		 \param[in] func The ae::Actor::Func to (de)activate
		 \param[in] target The ae::Actor::Target to (de)activate
		 \param[in] flag True to activate the selected functionalities for the selected targets, false to deactivate them

		 \par Example:
		 \code
		 // Deactivate event handling and updating for the actor's chilren
		 auto actor = std::make_unique<ae::Actor>();
		 actor->activateFunctionality(ae::Actor::Func::EventHandle | ae::Actor::Func::Update, ae::Actor::Target::Children, false);
		 \endcode

		 \since v0.7.0
		*/
		void activateFunctionality(uint32_t func, uint32_t target, bool flag);
		bool isFunctionalityActive(uint32_t func, uint32_t target);
		/*!
		 \brief Calculates and retrieves the global transform by multiplying every transform of the actor until the root node is reached.

		 \return The global transform

		 \since v0.7.0
		*/
		_NODISCARD inline const Matrix4f& getGlobalTransform() { return mGlobalTransform; }
		/*!
		 \brief Retrieves the current node's parent.

		 \return The ae::Actor that's the node's parent

		 \since v0.7.0
		*/
		inline Actor* const getParent() const noexcept { return mParent; }
		/*!
		 \brief Adds the component specified by the template typename.
		 \details The component is only added if the same type isn't already within the component list.

		 \par Example:
		 \code
		 sprite->addComponent<ae::Collider2DComponent>();
		 \endcode

		 \sa getComponent, hasComponent()

		 \since v0.7.0
		*/
		template <class T>
		std::enable_if_t<std::is_base_of_v<Component, T>, void> addComponent()
		{
			ComponentManager::Type type = ComponentManager::getType<T>();
			if (!mComponents.try_emplace(type, std::make_unique<T>(*this)).second) {
				AEON_LOG_ERROR("Failed to add component", "A component of the same type already exists.");
			}
		}
		/*!
		 \brief Retrieves the component specified by the template typename.

		 \return The component associated or nullptr if it wasn't found

		 \par Example:
		 \code
		 auto collider = sprite->getComponent<ae::Collider2DComponent>();
		 \endcode

		 \sa addComponent(), hasComponent()

		 \since v0.7.0
		*/
		template <class T>
		std::enable_if_t<std::is_base_of_v<Component, T>, T* const> getComponent()
		{
			auto component = mComponents.find(ComponentManager::getType<T>());
			return (component != mComponents.end()) ? dynamic_cast<T* const>(component->second.get()) : nullptr;
		}
		/*!
		 \brief Retrieves the component specified by the template typename.

		 \return The component associated or nullptr if it wasn't found

		 \par Example:
		 \code
		 auto collider = sprite->getComponent<ae::Collider2DComponent>();
		 \endcode

		 \sa addComponent(), hasComponent()

		 \since v0.7.0
		*/
		template <class T>
		std::enable_if_t<std::is_base_of_v<Component, T>, const T* const> getComponent() const
		{
			const auto component = mComponents.find(ComponentManager::getType<T>());
			return (component != mComponents.end()) ? dynamic_cast<const T* const>(component->second.get()) : nullptr;
		}
		/*!
		 \brief Attempts to find the component specified by the template typename.

		 \return True if the associated component has been added, false otherwise

		 \par Example:
		 \code
		 if (sprite->hasComponent<ae::Collider2DComponent>()) {
			...
		 }
		 \endcode

		 \sa addComponent(), getComponent()

		 \since v0.7.0
		*/
		template <class T>
		std::enable_if_t<std::is_base_of_v<Component, T>, bool> hasComponent() const
		{
			return mComponents.find(ComponentManager::getType<T>()) != mComponents.end();
		}

		// Public virtual method(s)
		/*!
		 \brief Retrieves the appropriate transform of the ae::Actor.

		 \return The actor's transform

		 \since v0.7.0
		*/
		virtual Matrix4f getTransform();
	protected:
		// Protected method(s)
		/*!
		 \brief Updates the z-index ordering of the current node and its children to achieve correct depth ordering.
		 \details The z-index provided will be assigned to the current node whereas the children will be assigned a superior z-index.

		 \param[in] zIndex The depth index to assign to the current node

		 \since v0.7.0
		*/
		void updateZOrdering(int zIndex);

		// Protected virtual method(s)
		/*!
		 \brief Process the polled input event.
		 \details This method's behaviour is defined by the derived class.

		 \param[in] event The polled input event

		 \sa handleEvent()

		 \since v0.7.0
		*/
		virtual void handleEventSelf(Event* const event);
		/*!
		 \brief Updates the actor.
		 \details Updates the ae::Transform2DComponent's properties by default (origin flags and relative alignment).

		 \param[in] dt The time difference between the previous frame and the current one

		 \sa update()

		 \since v0.7.0
		*/
		virtual void updateSelf(const Time& dt);
		/*!
		 \brief Submits the current node to the active renderer.
		 \details This method's behaviour is defined by the derived class.

		 \param[in] states The ae::RenderStates defining the OpenGL state

		 \sa render()

		 \since v0.7.0
		*/
		virtual void renderSelf(RenderStates states) const;
	private:
		// Private method(s)
		void cleanupFinishedAnimations();

	protected:
		// Protected member(s)
		Actor*                                                                 mParent;          //!< The node's parent node
	private:
		// Private member(s)		 
		std::vector<std::unique_ptr<Actor>>                                    mChildren;        //!< The attached children nodes
		std::queue<EasingAnimation*>                                           mAnimations;
		std::unordered_map<ComponentManager::Type, std::unique_ptr<Component>> mComponents;      //!< The attached components of the actor
		std::unordered_map<Func, std::unordered_map<Target, bool>>             mFuncs;           //!< The functionalities that are available for all targets
		Matrix4f                                                               mGlobalTransform; //!< The global transform of the node
	};
}
#endif // Aeon_Graphics_Actor_H_

/*!
 \class ae::Actor
 \ingroup graphics

 The ae::Actor base class represents all game objects in a scene that form a
 scene tree to better organize and manage different entities. It provides
 functionality for attaching/detaching children nodes, event handling, updating
 and drawing the caller node and its children.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2021.08.21
 \copyright MIT License
*/