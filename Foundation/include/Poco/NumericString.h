//
// NumericString.h
//
// $Id: //poco/1.4/Foundation/include/Poco/NumericString.h#1 $
//
// Library: Foundation
// Package: Core
// Module:  NumericString
//
// Numeric string utility functions.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Foundation_NumericString_INCLUDED
#define Foundation_NumericString_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/FPEnvironment.h"
#ifdef min
	#undef min
#endif
#ifdef max
	#undef max
#endif
#include <limits>
#include <cmath>


namespace Poco {


template <typename I>
bool strToInt(const char* pStr, I& result, short base = -1)
	/// Converts zero-terminated array to integer number;
	/// If base is equal to -1, this functin will try to determine
	/// the numeric base (using '0' prefix for octal, '0x' for 
	/// hexadecimal and no prefix for decimal).
	/// Thousand separators are recognized for base10 and the locale
	/// and silently skipped but not verified for correct positioning.
	/// Returns true if succesful. If parsing was unsuccesful,
	/// the return value is false with result value undetermined.
{
	if (!std::numeric_limits<I>::is_integer) return false;

	if (!pStr || (pStr && *pStr == '\0')) return false;
	while ((*pStr != '\0') && (*pStr == ' ')) ++pStr;
	if (*pStr == '\0') return false;
	
	char sign = 1;

	if (*pStr == '-')
	{
		++pStr;
		sign = -1;
	}
	else if (*pStr == '+') ++pStr;
	if (*pStr == '\0') return false;

	result = 0;

	if (*pStr == '0')
	{
		while ((*pStr != '\0') && (*pStr == '0')) ++pStr; 
		if (*pStr == '\0') 
		{
			result = 0;
			return true;
		}
		
		if ((*pStr == 'x') || (*pStr == 'X'))
		{
			base = 0x10;
			++pStr;
			if (*pStr == '\0') return false;
		}
		else if (base == -1) base = 010;
	}
	else if (base == -1) base = 10;

	while ((*pStr == '0'))
	{
		if (*pStr != '\0')
		{
			result = 0;
			return true;
		}
		++pStr;
	}
	if (*pStr == '\0') return false;

	const char thSep = thousandSeparator();
	bool allowDigits = true;
	for (; *pStr != '\0'; ++pStr)
	{
		switch (*pStr)
		{
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7':
			if (allowDigits)
			{
				if (result > (std::numeric_limits<I>::max() / base)) return false;
				result = result * base + (*pStr - '0');
			}
			else return false;
			break;
		
		case '8': case '9':
			if (allowDigits && (base == 10 || base == 16))
			{
				if (result > (std::numeric_limits<I>::max() / base)) return false;
				result = result * base + (*pStr - '0');
			}
			else return false;
			break;

		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
			if (allowDigits && base == 16)
			{
				if (result > (std::numeric_limits<I>::max() / base)) return false;
				result = result * base + (10 + *pStr - 'a');
			}
			else return false;
			break;

		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
			if (allowDigits && base == 16)
			{
				if (result > (std::numeric_limits<I>::max() / base)) return false;
				result = result * base + (10 + *pStr - 'A');
			}
			else
				return false;
			break;

		case 'U':
		case 'u':
		case 'L':
		case 'l':
			allowDigits = false;
			break;

		case '.':
			if ((base == 10) && (thSep == '.')) break;
			else return false;

		case ',':
			if ((base == 10) && (thSep == ',')) break;
			else return false;

		case ' ':
			if (base == 10) break;
			else return false;

		default:
			return false;
		}
	}

	if ((base == 10) && (std::numeric_limits<I>::is_signed))
		result *= sign;

	return true;
}


template <typename I>
bool strToInt(const std::string& str, I& result, short base = -1)
	/// Converts string to integer number;
	/// This is a wrapper function, for details see see the
	/// bool strToInt(const char*, I&, short&) implementation.
{
	return strToInt(str.c_str(), result, base);
}


inline char decimalSeparator()
	/// Returns decimal separator from global locale or
	/// default '.' for platforms where locale is unavailable.
{
#if !defined(POCO_NO_LOCALE)
	return std::use_facet<std::numpunct<char> >(std::locale()).decimal_point();
#else
	return '.';
#endif
}


inline char thousandSeparator()
	/// Returns thousand separator from global locale or
	/// default ',' for platforms where locale is unavailable.
{
#if !defined(POCO_NO_LOCALE)
	return std::use_facet<std::numpunct<char> >(std::locale()).thousands_sep();
#else
	return ',';
#endif
}


#ifndef POCO_NO_FPENVIRONMENT

namespace {

static char DUMMY_EXP_UNDERFLOW = 0; // dummy default val

}

template <typename F>
bool strToFloat (const char* pStr, F& result, char& eu = DUMMY_EXP_UNDERFLOW)
	/// Converts zero-terminated array to floating-point number;
	/// Returns true if succesful. Exponent underflow (i.e. loss of precision)
	/// is signalled in eu. Thousand separators are recognized for the locale
	/// and silently skipped but not verified for correct positioning.
	///
	/// If parsing was unsuccesful, the return value is false with
	/// result and eu values undetermined.
{
	if (!pStr || (pStr && *pStr == '\0')) return false;

	// parser states:
	const char STATE_LEADING_SPACES = 0;
	const char STATE_DIGITS_BEFORE_DEC_POINT = 1;
	const char STATE_DIGITS_AFTER_DEC_POINT = 2;
	const char STATE_EXP_CHAR = 3;
	const char STATE_EXP_DIGITS = 4;
	const char STATE_SUFFIX = 5; // 'f' suffix

	const char decSep = decimalSeparator();
	char numSign = 1, expSign = 1;
	char state = STATE_LEADING_SPACES;
	F mantissa = 0.0, exponent = 0.0;
	F pow10 = 1.;
	result = 0.0;
	eu = 0;
	for (; *pStr != '\0'; ++pStr)
	{
		switch (*pStr)
		{
		case '.':
			if (decSep == '.')
			{
				if (state > STATE_DIGITS_BEFORE_DEC_POINT) return false;
				state = STATE_DIGITS_AFTER_DEC_POINT;
			}
			break;

		case ',':
			if (decSep == ',')
			{
				if (state > STATE_DIGITS_BEFORE_DEC_POINT) return false;
				state = STATE_DIGITS_AFTER_DEC_POINT;
			}
			break;

		case ' ':
			if ((state > STATE_LEADING_SPACES) && (state < STATE_DIGITS_AFTER_DEC_POINT))
				return false;
			break;

		case '-':
			if (state == STATE_LEADING_SPACES)
				numSign = -1;
			else if (state == STATE_EXP_CHAR) // exponential char
				expSign = -1;
			else return false;
		case '+':
			break;

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (state >= STATE_SUFFIX) return false; // constant suffix
			if (state <= STATE_DIGITS_BEFORE_DEC_POINT) // integral part digits
			{
				result = result * 10 + (*pStr - '0');
				state = STATE_DIGITS_BEFORE_DEC_POINT;
			}
			else if (state <= STATE_DIGITS_AFTER_DEC_POINT) // fractional part digits
			{
				mantissa += (*pStr - '0') / (pow10 *= 10.);
				state = STATE_DIGITS_AFTER_DEC_POINT;
			}
			else if (state <= STATE_EXP_DIGITS) // exponent digits
			{
				exponent = exponent * 10 + (*pStr - '0');
				state = STATE_EXP_DIGITS;
			}
			else return false;
			break;

		case 'E':
		case 'e':
			if (state > STATE_DIGITS_AFTER_DEC_POINT) return false;
			state = STATE_EXP_CHAR;
			break;

		case 'F':
		case 'f':
			state = STATE_SUFFIX;
			break;

		default:
			return false;
		}
	}

	if (exponent > std::numeric_limits<F>::max_exponent10)
	{
		eu = expSign;
		exponent = std::numeric_limits<F>::max_exponent10;
	}

	result += mantissa;
	if (numSign != 1) result *= numSign;
	if (exponent > 1.0)
	{
		F scale = std::pow(10., exponent);
		result = (expSign < 0) ? (result / scale) : (result * scale);
	}

	return (state != STATE_LEADING_SPACES) && // empty/zero-length string
		!FPEnvironment::isInfinite(result) &&
		!FPEnvironment::isNaN(result);
}


template <typename F>
bool strToFloat (const std::string& s, F& result, char& eu = DUMMY_EXP_UNDERFLOW)
	/// Converts string to floating-point number;
	/// This is a wrapper function, for details see see the
	/// bool strToFloat(const char*, F&, char&) implementation.
{
	return strToFloat(s.c_str(), result, eu); 
}


#endif // POCO_NO_FPENVIRONMENT


} // namespace Poco


#endif // Foundation_NumericString_INCLUDED
