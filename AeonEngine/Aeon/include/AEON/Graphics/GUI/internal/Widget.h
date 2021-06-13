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

#ifndef Aeon_Graphics_GUI_Widget_H_
#define Aeon_Graphics_GUI_Widget_H_

#include <AEON/Window/Application.h>
#include <AEON/Graphics/Actor2D.h>
#include <AEON/Graphics/Camera2D.h>

namespace ae
{
	// Forward declaration(s)
	class Shape;
	class RenderTarget;

	/*!
	 \brief Abstract base class used to provide basic GUI functionalities.
	 \note The typename T needs to be derived of the ae::Actor2D class and be default-constructible.
	*/
	template <typename T, typename = std::enable_if_t<std::is_base_of_v<Actor2D, T> && std::is_default_constructible_v<T>>>
	class Widget : public Actor2D
	{
	public:
		// Public enum(s)
		/*!
		 \brief The different states which a GUI widget may be in.
		*/
		enum State {
			Disabled,  //!< The widget won't receive any input
			Idle,      //!< The widget's idle state, awaiting input
			Click,     //!< The widget was clicked
			Focus,     //!< The widget in being focused
			Hover,     //!< The widget is currently being hovered over
			StateCount //!< The number of states available
		};

	public:
		// Public constructor(s)
		/*!
		 \brief Virtual destructor.
		 \details A virtual destructor is needed as this class will be inherited.

		 \since v0.5.0
		*/
		virtual ~Widget()
		{
		}
	public:
		// Public method(s)
		/*!
		 \brief Enables/Disables the ae::Widget (sets the active state to Idle or Disabled).
		 
		 \param[in] flag True to enable, false to disable

		 \par Example:
		 \code
		 // Set the button to its disabled state
		 auto button = std::make_unique<ae::Button>();
		 button->enable(false);
		 \endcode

		 \since v0.5.0
		*/
		void enable(bool flag)
		{
			if (mActiveState == State::Disabled && flag) {
				enableState(State::Idle);
			}
			else if (mActiveState != State::Disabled && !flag) {
				enableState(State::Disabled);
			}
		}
		/*!
		 \brief Retrieves the ae::Widget's active state.

		 \return The ae::Widget::State indicating the current state of the widget

		 \par Example:
		 \code
		 auto button = std::make_unique<ae::Button>();
		 ...

		 ae::Widget::State buttonState = button->getActiveState();
		 \endcode

		 \sa getState()

		 \since v0.5.0
		*/
		_NODISCARD State getActiveState() const noexcept
		{
			return mActiveState;
		}
		void setRenderTarget(RenderTarget& target) noexcept
		{
			mTarget = &target;
		}
		/*!
		 \brief Retrieves the ae::Widget's state associated to the ae::Widget::State provided.

		 \param[in] state The ae::Widget::State associated to the appropriate ae::Shape

		 \return The ae::Shape associated to the ae::Widget::State provided

		 \par Example:
		 \code
		 auto button = std::make_unique<ae::Button>();
		 ...
		 ae::RectangleShape& idleState = button->getState(ae::Button::State::Idle);
		 \endcode

		 \sa getActiveState()

		 \since v0.5.0
		*/
		_NODISCARD T& getState(State state) noexcept
		{
			return *mStates[state];
		}

		// Public virtual method(s)
		/*!
		 \brief Retrieves the active state's model bounding box of the ae::Widget.

		 \return The active state's model bounding box

		 \par Example:
		 \code
		 auto button = std::make_unique<ae::Button>();
		 ...

		 Box2f idleModelBounds = button->getModelBounds();
		 \endcode

		 \since v0.5.0
		*/
		_NODISCARD virtual Box2f getModelBounds() const override
		{
			return mStates[mActiveState]->getModelBounds();
		}
	protected:
		// Protected constructor(s)
		/*!
		 \brief Default constructor.
		 \details The different states will be instantiated and attached to the ae::Widget, and the idle state will be enabled.

		 \since v0.5.0
		*/
		Widget()
			: Actor2D()
			, mTarget(&Application::getInstance().getWindow())
			, mStates()
			, mActiveState(State::Idle)
		{
			// Instantiate and attach the children states
			for (size_t i = 0; i < State::StateCount; ++i) {
				auto state = std::make_unique<T>();
				mStates[i] = state.get();
				attachChild(std::move(state));
			}
			enableState(mActiveState);
		}
		/*!
		 \brief Deleted copy constructor.

		 \since v0.5.0
		*/
		Widget(const Widget<T>&) = delete;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::Widget that will be moved

		 \since v0.5.0
		*/
		Widget(Widget<T>&& rvalue) noexcept
			: Actor2D(std::move(rvalue))
			, mTarget(rvalue.mTarget)
			, mStates(rvalue.mStates)
			, mActiveState(rvalue.mActiveState)
		{
		}
	protected:
		// Protected operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.5.0
		*/
		Widget<T>& operator=(const Widget<T>&) = delete;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::Widget that will be moved

		 \return The caller ae::Widget

		 \since v0.5.0
		*/
		Widget<T>& operator=(Widget<T>&& rvalue) noexcept
		{
			// Copy the rvalue's trivial data and move the rest
			Actor2D::operator=(std::move(rvalue));
			mTarget = rvalue.mTarget;
			mStates = rvalue.mStates;
			mActiveState = rvalue.mActiveState;

			return *this;
		}
	protected:
		// Protected method(s)
		/*!
		 \brief Enables event handling, updating and rendering for the active state, and disables it for the others.

		 \param[in] state The ae::Widget::State that will be enabled

		 \since v0.5.0
		*/
		virtual void enableState(State state)
		{
			mActiveState = state;
			for (size_t i = 0; i < State::StateCount; ++i) {
				const bool FLAG = static_cast<State>(i) == mActiveState;
				mStates[i]->activateFunctionality(Func::EventHandle | Func::Render, Target::Self | Target::Children, FLAG);
			}
		}

		// Protected virtual method(s)
		/*!
		 \brief Checks if the position provided is situated within the ae::Widget's bounds.
		 \note This method is best suited for rectangular widgets.

		 \param[in] mousePos The current position of the mouse cursor

		 \since v0.5.0
		*/
		_NODISCARD virtual bool isHoveredOver(const Vector2d& mousePos)
		{
			Window& appWindow = Application::getInstance().getWindow();
			if (&appWindow != mTarget) {
				const Matrix4f GLOBAL_TRANSFORM = mTarget->getCamera()->getViewMatrix() * getGlobalTransform();
				const Box2f MODEL_BOUNDS = getModelBounds();
				const Box2f GLOBAL_BOUNDS(Vector2f(GLOBAL_TRANSFORM * Vector3f(MODEL_BOUNDS.min)), Vector2f(GLOBAL_TRANSFORM * Vector3f(MODEL_BOUNDS.max)));
				return GLOBAL_BOUNDS.contains(appWindow.mapPixelToCoords(mousePos));
			}
			else {
				return getGlobalBounds().contains(mTarget->mapPixelToCoords(mousePos));
			}
		}
	protected:
		// Protected virtual method(s)
		/*!
		 \brief Corrects the ae::Widget's and its childrens' properties.

		 \param[in] dt The time difference between the previous frame and the current frame

		 \sa handleEventSelf()

		 \since v0.5.0
		*/
		virtual void updateSelf(const Time& dt) override
		{
			correctProperties();
		}

	protected:
		// Protected member(s)
		RenderTarget*               mTarget;      //!< The widget's render target
	private:
		// Private member(s)
		std::array<T*, State::StateCount> mStates;      //!< The different widgets based on the active state
		State                             mActiveState; //!< The widget's active state
	};
}
#endif // Aeon_Graphics_GUI_Widget_H_

/*!
 \class ae::Widget
 \ingroup graphics

 The ae::Widget abstract template base class provides the most basic functionalities for
 all GUI widgets available.

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2020.08.17
 \copyright MIT License
*/