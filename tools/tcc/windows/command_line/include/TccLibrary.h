/*  ===========================================================================
*
*   This file is part of HISE.
*   Copyright 2016 Christoph Hart
*
*   HISE is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   HISE is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with HISE.  If not, see <http://www.gnu.org/licenses/>.
*
*   Commercial licences for using HISE in an closed source project are
*   available on request. Please visit the project's website to get more
*   information about commercial licencing:
*
*   http://www.hartinstruments.net/hise/
*
*   HISE is based on the JUCE library,
*   which also must be licenced for commercial applications:
*
*   http://www.juce.com
*
*   ===========================================================================
*/

#ifndef TCCLIBRARY_H_INCLUDED
#define TCCLIBRARY_H_INCLUDED

#define TCC_LIBRARY_VERSION 0x0007

#ifndef TCC_INCLUDE
#define TCC_INCLUDE 1
#endif

#if TCC_INCLUDE
#define TCCLIBDEF extern

#include <tcclib.h>

#if TCC_COMMAND_LINE
#include <kiss_fft/kiss_fft.h>
#include <kiss_fft/kiss_fftr.h>
#endif

#else
#define TCCLIBDEF static
#endif

#if !TCC_INCLUDE
static void addFunctionsToContext(TccContext* context);
#endif

/***************************************************************************************
*																					   *
*	VECTOR OPERATIONS																   *
*																					   *
***************************************************************************************/

/** dst *= src */
TCCLIBDEF void vMultiply(float* dst, const float* src, int numSamples);

/** dst *= scalar */
TCCLIBDEF void vMultiplyScalar(float* dst, float scalar, int numSamples);

/** dst += src. */
TCCLIBDEF void vAdd(float* dst, const float* src, int numSamples);

/** dst += a */
TCCLIBDEF void vAddScalar(float* dst, float a, int numSamples);

/** dst -= s */
TCCLIBDEF void vSub(float* dst, const float* s, int numSamples);

/** dst = value. */
TCCLIBDEF void vFill(float* dst, float value, int numSamples);

/** dst = src. */
TCCLIBDEF void vCopy(float* dst, const float* src, int numSamples);

/** return =  min(data). */
TCCLIBDEF float vMinimum(const float* data, int numSamples);

/** return max(data). */
TCCLIBDEF float vMaximum(const float* data, int numSamples);

/** data = min(min, max(max, d)) */
TCCLIBDEF void vLimit(float* data, float minimum, float maximum, int numSamples);

/***************************************************************************************
*																					   *
*	DEBUG / PRINT FUNCTIONS															   *
*																					   *
***************************************************************************************/

TCCLIBDEF void printString(const char* message);
TCCLIBDEF void printInt(int i);
TCCLIBDEF void printDouble(double d);
TCCLIBDEF void printFloat(float f);

/***************************************************************************************
*																					   *
*	MATH ROUTINES																	   *
*																					   *
***************************************************************************************/

TCCLIBDEF void windowFunctionBlackman(float* d, int size);
TCCLIBDEF void windowFunctionRectangle(float* d, int size);
TCCLIBDEF void windowFunctionHann(float* d, int size);
TCCLIBDEF void windowFunctionBlackmanHarris(float* d, int size);

TCCLIBDEF void* createFFTState(int size, bool isReal, bool isInverse);
TCCLIBDEF void destroyFFTState(void* state);

TCCLIBDEF void complexFFT(void* FFTState, float* in, float* out, int fftSize);
TCCLIBDEF void complexFFTInverse(void* FFTState, float* in, float* out, int fftSize);
TCCLIBDEF void complexFFTInplace(void* FFTState, float* data, int fftSize);
TCCLIBDEF void complexFFTInverseInplace(void* FFTState, float* data, int fftSize);

TCCLIBDEF void realFFT(void* FFTState, float* in, float* out, int fftSize);
TCCLIBDEF void realFFTInverse(void* FFTState, float* in, float* out, int fftSize);

/** Converts real values to complex value. sizeof(c) must be 2*r. */
TCCLIBDEF void realToComplex(float* c, float* r, int size);
TCCLIBDEF void complexMultiply(float* d, float* r, int size);
TCCLIBDEF void zeroPadVector(float* dst, const float* src, int size, int paddingSize);
TCCLIBDEF int nextPowerOfTwo(int size);

TCCLIBDEF void writeFloatArray(float* data, int numSamples);
TCCLIBDEF void writeFrequencySpectrum(float* data, int numSamples);

#endif  // TCCLIBRARY_H_INCLUDED
