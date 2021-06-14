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

#ifndef Aeon_Graphics_Component_H_
#define Aeon_Graphics_Component_H_

#include <AEON/Config.h>

namespace ae
{
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
	protected:
		// Protected constructor(s)
		/*!
		 \brief Default constructor.
		 \note No direct instance of this class may be created.

		 \since v0.7.0
		*/
		Component() noexcept = default;
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
	protected:
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
	};
}
#endif // Aeon_Graphics_Component_H_

/*!
 \class ae::Component
 \ingroup graphics

 The ae::Component abstract base class in used to separate various
 functionalities such as a transformable actor, a renderabe actor, etc.
 into components which can easily be added, removed and generally managed.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2021.06.15
 \copyright MIT License
*/