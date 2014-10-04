/*
 * utils.h
 *
 * Created: 03.02.2013 22:11:13
 *  Author: astr0
 */ 

#pragma once


#ifndef UTILS_H_
#define UTILS_H_

#define SMCU_WRAPVARIABLE(CLASSNAME, VARNAME)\
struct CLASSNAME\
{\
static decltype(VARNAME)& value(){return VARNAME;}\
};

namespace smcu
{
	namespace common
	{
		template<class T>
		constexpr T Max(T v1, T v2){return v1 > v2 ? v1 : v2;}		

		template<class T>
		constexpr T Min(T v1, T v2){return v1 < v2 ? v1 : v2;}
	}
}


#endif /* UTILS_H_ */