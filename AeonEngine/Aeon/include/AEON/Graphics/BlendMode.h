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

#ifndef Aeon_Graphics_BlendMode_H_
#define Aeon_Graphics_BlendMode_H_

#include <yvals_core.h>

#include <AEON/Config.h>

namespace ae
{
	/*!
	 \brief Structure describing the blending mode to use during OpenGL rendering.
	*/
	struct _NODISCARD AEON_API BlendMode
	{
		// Public enum(s)
		/*!
		 \brief The enumeration of the blending factors.
		*/
		enum Factor
		{
			Zero             = 0x0000, //!< (0, 0, 0, 0)
			One              = 0x0001, //!< (1, 1, 1, 1)
			SrcColor         = 0x0300, //!< (srcR, srcG, srcB, srcA)
			OneMinusSrcColor = 0x0301, //!< (1, 1, 1, 1) - (srcR, srcG, srcB, srcA)
			SrcAlpha         = 0x0302, //!< (srcA, srcA, srcA, srcA)
			OneMinusSrcAlpha = 0x0303, //!< (1, 1, 1, 1) - (srcA, srcA, srcA, srcA)
			DstAlpha         = 0x0304, //!< (dstA, dstA, dstA, dstA)
			OneMinusDstAlpha = 0x0305, //!< (1, 1, 1, 1) - (dstA, dstA, dstA, dstA)
			DstColor         = 0x0306, //!< (dstR, dstG, dstB, dstA)
			OneMinusDstColor = 0x0307  //!< (1, 1, 1, 1) - (dstR, dstG, dstB, dstA)
		};
		/*!
		 \brief The enumeration of the blending equations.
		*/
		enum Equation
		{
			Add             = 0x8006, //!< RGBA = Src * SrcFactor + Dst * DstFactor
			Subtract        = 0x800A, //!< RGBA = Src * SrcFactor - Dst * DstFactor
			ReverseSubtract = 0x800B  //!< RGBA = Dst * DstFactor - Src * SrcFactor
		};

		// Public member(s)
		Factor   colorSrcFactor; //!< The source's blending factor for the color channels
		Factor   colorDstFactor; //!< The destination's blending factor for the color channels
		Equation colorEquation;  //!< The blending equation for the color channels
		Factor   alphaSrcFactor; //!< The source's blending factor for the alpha channel
		Factor   alphaDstFactor; //!< The destination's blending factor for the alpha channel
		Equation alphaEquation;  //!< The blending equation for the alpha channel

		// Public static member(s)
		static const BlendMode BlendAlpha;    //!< Blend the source and the destination according to the destination's opacity
		static const BlendMode BlendAdd;      //!< Add the source to the destination
		static const BlendMode BlendMultiply; //!< Multiply the source with the destination
		static const BlendMode BlendNone;     //!< Replace the destination with the source

		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details The ae::BlendMode chosen is the ae::BlendMode::BlendAlpha.

		 \since v0.4.0
		*/
		BlendMode() noexcept;
		/*!
		 \brief Copy constructor.
		 
		 \param[in] copy The ae::BlendMode that will be copied

		 \since v0.5.0
		*/
		BlendMode(const BlendMode& copy) noexcept = default;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::BlendMode that will be copied

		 \since v0.5.0
		*/
		BlendMode(BlendMode&& rvalue) noexcept;
		/*!
		 \brief Constructs the ae::BlendMode by providing the blending factors and equations to use.
		 \details The same factors and the same equation will be used for both the color channels and the alpha channel.

		 \param[in] srcFactor Specifies the source ae::BlendMode::Factor for the color and alpha channels
		 \param[in] dstFactor Specifies the destination ae::BlendMode::Factor for the color and alpha channels
		 \param[in] equation Specifies the optional ae::BlendMode::Equation for the color and alpha channels, ae::BlendMode::Equation::Add by default

		 \par Example:
		 \code
		 ae::BlendMode blendMode(ae::BlendMode::Factor::SrcColor, ae::BlendMode::Factor::DstAlpha);
		 \endcode

		 \since v0.4.0
		*/
		BlendMode(Factor srcFactor, Factor dstFactor, Equation equation = Equation::Add) noexcept;
		/*!
		 \brief Constructs the ae::BlendMode by providing all the blending factors and equations to use.

		 \param[in] colorSrcFactor Specifies the source ae::BlendMode::Factor for the color channels
		 \param[in] colorDstFactor Specifies the destination ae::BlendMode::Factor for the color channels
		 \param[in] colorEquation Specifies the ae::BlendMode::Equation for the color channels
		 \param[in] alphaSrcFactor Specifies the source ae::BlendMode::Factor for the alpha channel
		 \param[in] alphaDstFactor Specifies the destination ae::BlendMode::Factor for the alpha channel
		 \param[in] alphaEquation Specifies the ae::BlendMode::Equation for the alpha channel

		 \par Example:
		 \code
		 ae::BlendMode blendMode(ae::BlendMode::Factor::SrcColor, ae::BlendMode::Factor::DstAlpha, ae::BlendMode::Equation::Add,
		                         ae::BlendMode::Factor::SrcAlpha, ae::BlendMode::Factor::OneMinusSrcAlpha, ae::BlendMode::Equation::ReverseSubtract);
		 \endcode

		 \since v0.4.0
		*/
		BlendMode(Factor colorSrcFactor, Factor colorDstFactor, Equation colorEquation,
		          Factor alphaSrcFactor, Factor alphaDstFactor, Equation alphaEquation) noexcept;

		// Public operator(s)
		/*!
		 \brief Assignment operator.
		 \details Checks if the caller is being assigned to itself.

		 \param[in] other The ae::BlendMode that will be copied

		 \return The caller ae::BlendMode

		 \since v0.5.0
		*/
		BlendMode& operator=(const BlendMode& other);
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::BlendMode that will be copied

		 \return The caller ae::BlendMode

		 \since v0.5.0
		*/
		BlendMode& operator=(BlendMode&& rvalue) noexcept;
		/*!
		 \brief Equality operator.
		 \details Checks if the caller's and the \a other's members are respectively equal.

		 \param[in] other The ae::BlendMode that will be compared with the caller

		 \return True if the caller's and the \a other's members are respectively equal, false otherwise

		 \par Example:
		 \code
		 ae::BlendMode blendMode1 = ae::BlendMode::BlendAlpha;
		 ae::BlendMode blendMode2(ae::BlendMode::Factor::SrcColor, ae::BlendMode::Factor::DstAlpha);
		 if (blendMode1 == blendMode2) {
			...
		 }
		 \endcode

		 \sa operator!=()

		 \since v0.4.0
		*/
		_NODISCARD bool operator==(const BlendMode& other) const noexcept;
		/*!
		 \brief Inequality operator.
		 \details Checks if the caller's and the \a other's members are respectively inequal.

		 \param[in] other The ae::BlendMode that will be compared with the caller

		 \return True if at least one of the caller's and the \a other's members is inequal, false otherwise

		 \par Example:
		 \code
		 ae::BlendMode blendMode1 = ae::BlendMode::BlendAlpha;
		 ae::BlendMode blendMode2(ae::BlendMode::Factor::SrcColor, ae::BlendMode::Factor::DstAlpha);
		 if (blendMode1 != blendMode2) {
			...
		 }
		 \endcode

		 \sa operator==()

		 \since v0.4.0
		*/
		_NODISCARD bool operator!=(const BlendMode& other) const noexcept;
		/*!
		 \brief Less than operator.
		 \details This method is simply used so that ae::BlendMode can be used as a key in a hashtable.

		 \param[in] other The ae::BlendMode that will be compared with the caller

		 \since v0.4.0
		*/
		_NODISCARD bool operator<(const BlendMode& other) const noexcept;
	};
}
#endif // Aeon_Graphics_BlendMode_H_

/*!
 \struct ae::BlendMode
 \ingroup graphics

 The ae::BlendMode struct is used to represent a blending function. A blend
 mode determines how the colors of an entity are mixed (blended) with the
 pre-rendered colors in the color buffer.

 The struct is composed of 6 components:
 \li Color source factor
 \li Color destination factor
 \li Color blend equation
 \li Alpha (transparence) source factor
 \li Alpha (transparence) destination factor
 \li Alpha (transparence) blend equation

 The source factor specifies the pixel's contribution to the final color. The
 destination factor specifies how the previously rendered pixel contributes to
 the final color.

 The RGB (red, green, blue; simply called color) color channels and A (alpha;
 transparence) can be treated separately. This separation can prove useful for
 certain blending modes, but most often, we won't have need of this separation
 and will simply treat the color as a single entity.

 The blend factors and equations correspond to their OpenGL equivalents.
 Generally, the resulting pixel's color is calculated using the following
 formula ('src' is the source pixel's color, 'dst' is the destination pixel's
 color, the remaining variables correspond to public attributs with the
 equations being the + and - operators):
 \code
 dst.rgb = colorSrcFactor * src.rgb (color equation operator) colorDstFactor * dst.rgb
 dst.a = alphaSrcFactor * src.a (alpha equation operator) alphaDstFactor * dst.a
 \endcode

 All factors and colors are represented as floating points between the values 0
 and 1. If they're not normalized (within the range [0,1]), the result is
 adjusted so that they're placed within that range.

 The most common blend modes are defined as constants:
 \code
 ae::BlendMode alphaBlending = ae::BlendMode::BlendAlpha;
 ae::BlendMode additiveBlending = ae::BlendMode::BlendAdd;
 ae::BlendMode multiplicativeBlending = ae::BlendMode::BlendMultiply;
 ae::BlendMode noBlending = ae::BlendMode::BlendNone;
 \endcode

 \author Filippos Gleglakos
 \version v0.5.0
 \date 2020.06.09
 \copyright MIT License
*/