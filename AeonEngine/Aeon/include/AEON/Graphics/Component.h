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

#include <memory>

#include <AEON/Config.h>

namespace ae
{
	// Forward declaration(s)
	class Actor;

	/*!
	 \brief Abstract base class used in component-based design.
	 \note No direct instance of this class may be created.
	*/
	class AEON_API Component
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Virtual destructor.
		 \details A virtual destructor is needed as this class will be inherited.

		 \since v0.7.0
		*/
		virtual ~Component() = default;

		// Public method(s)
		/*!
		 \brief Raises/lowers the dirty flag.
		 \note This method should in most cases be used internally.

		 \param[in] flag True to raise the dirty flag, false otherwise

		 \sa isDirty()

		 \since v0.7.0
		*/
		inline void setDirty(bool flag) noexcept { mIsDirty = flag; }
		/*!
		 \brief Checks whether the component has been modified.

		 \return True if the component has been dirtied, false otherwise

		 \since v0.7.0
		*/
		[[nodiscard]] inline bool isDirty() const noexcept { return mIsDirty; }
	protected:
		// Protected constructor(s)
		/*!
		 \brief Constructs the component by providing the associated actor.
		 \note No direct instance of this class may be created.

		 \param[in] associatedActor The ae::Actor to which the component is attached

		 \since v0.7.0
		*/
		explicit Component(Actor& associatedActor) noexcept;
		/*!
		 \brief Copy constructor.

		 \since v0.7.0
		*/
		Component(const Component&) = default;
		/*!
		 \brief Move constructor.

		 \since v0.7.0
		*/
		Component(Component&&) noexcept = default;

		// Protected operator(s)
		/*!
		 \brief Assignment operator.

		 \return The caller ae::Component
		 
		 \since v0.7.0
		*/
		Component& operator=(const Component&) = default;
		/*!
		 \brief Move assignment operator.

		 \return The caller ae::Component

		 \since v0.7.0
		*/
		Component& operator=(Component&&) noexcept = default;

	protected:
		// Protected member(s)
		Actor& mAssociatedActor; //!< The actor to which the component is attached
	private:
		// Private member(s)
		bool   mIsDirty;         //!< The dirty flag used by the associated actor
	};
}

/*!
 \class ae::Component
 \ingroup graphics

 The ae::Component abstract base class in used to separate various
 functionalities such as a transformable actor, a renderabe actor, etc.
 into components which can easily be added and generally managed.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2022.01.02
 \copyright MIT License
*/