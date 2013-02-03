/*
 * util.h
 *
 * Created: 01.02.2013 16:59:49
 *  Author: astr0
 */ 

#pragma once
#ifndef UTIL_H_
#define UTIL_H_

#include "stdint.h"
#include "../loki/Typelist.h"


#define SMCU_WRAPVARIABLE(CLASSNAME, VARNAME)\
struct CLASSNAME\
{\
static decltype(VARNAME)& value(){return VARNAME;}\
};

namespace smcu
{
	namespace common
	{
		// Min Size in bytes
		constexpr uint8_t MinSizeInBytes(uint8_t sizeInBits)
		{
			return sizeInBits <= 8 ? 1 : (1 + MinSizeInBytes(sizeInBits - 8));
		}
		
		template<class ValueType>
		constexpr uint8_t MaskToBit(ValueType value)
		{
			return value & 1 ? 0 : (1 + MaskToBit(value >> 1));
		}
		
		// Fast Data Type
		template<uint8_t bytes> 
		struct FastDataType;
		
		template<>
		struct FastDataType<1>
		{
			typedef uint_fast8_t Result;		
		};
		template<>
		struct FastDataType<2>
		{
			typedef uint_fast16_t Result;
		};
		template<>
		struct FastDataType<3>
		{
			typedef uint_fast32_t Result;
		};
		template<>
		struct FastDataType<4>
		{
			typedef uint_fast8_t Result;
		};
		
		// Constexpr Power function
		template<class DataType>
		constexpr DataType Pow(DataType base, DataType pow)
		{
			return pow == 0 ? 1 : base * Pow(base, pow - 1);
		}
		
		// TypeList stuff
		namespace TLS
		{
	        template <class TL>
	        constexpr uint8_t Length()
	        {
		        return 1 + Length<typename TL::Tail>();
	        }
			
			template <> 
			constexpr uint8_t Length<Loki::NullType>()
			{
				return 0;
			}
		}
		
		// Template metaprogramming stuff
		
		// Static IF statement
		template<bool condition, class TypeIfTrue, class TypeIfFale>
		struct StaticIf
		{
			typedef TypeIfTrue Result;
		};

		template<class TypeIfTrue, class TypeIfFale>
		struct StaticIf<false, TypeIfTrue, TypeIfFale>
		{
			typedef TypeIfFale Result;
		};		
		
		// Removes pointer from a type
		template<class T> struct remove_pointer                    {typedef T type;};
		template<class T> struct remove_pointer<T*>                {typedef T type;};
		template<class T> struct remove_pointer<T* const>          {typedef T type;};
		template<class T> struct remove_pointer<T* volatile>       {typedef T type;};
		template<class T> struct remove_pointer<T* const volatile> {typedef T type;};
			
		//Returns value at X position
		template<unsigned POS, class TResult, class... T>
		struct GetValueAt;

		template<class TResult, class...T>
		struct GetValueAt<0, TResult, TResult, T...>
		{
			static constexpr TResult Get(TResult res, T... val)
			{
				return res;
			}
		};

		template<unsigned POS, class TResult, class TVal, class...T>
		struct GetValueAt<POS, TResult, TVal, T...>
		{
			static constexpr TResult Get(TVal res, T... val)
			{
				return GetValueAt<POS-1, TResult, T...>::Get(val...);
			}
		};		
		
	}
}

#endif /* UTIL_H_ */