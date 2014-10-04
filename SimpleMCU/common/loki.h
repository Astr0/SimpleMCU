/*
* typelist.h
*
* Created: 10/4/2014 6:40:11 PM
*  Author: astr0
*/


#ifndef TYPELIST_H_
#define TYPELIST_H_

#include "../loki/Typelist.h"

namespace Loki{
	namespace TL{
		// Convert one type list to another TSelector<THead>::Result
		template <class TList, template<class> class TSelector>
		struct Select;
		
		template<template<class> class TSelector>
		struct Select<Loki::NullType, TSelector>
		{
			typedef Loki::NullType Result;
		};
		
		template<class THead, class TTail, template<class> class TSelector>
		struct Select<Loki::Typelist<THead, TTail>, TSelector>
		{
			typedef Loki::Typelist<typename TSelector<THead>::Result, typename Select<TTail, TSelector>::Result> Result;
		};
		
		// true if all elements match predicate
		template<class TList, template<class> class TPredicate>
		struct All;
			
		template <template<class> class TPredicate>
		struct All<Loki::NullType, TPredicate>
		{
			// empty matches, right?
			static constexpr bool value = true;
		};
			
		template <class THead, class TTail, template<class> class TPredicate>
		struct All<Loki::Typelist<THead, TTail>, TPredicate>
		{
			static constexpr bool value = TPredicate<THead>::value & All<TTail, TPredicate>::value;
		};
		
		// true if any element match predicate
		template<class TList, template<class> class TPredicate>
		struct Any;
				
		template <template<class> class TPredicate>
		struct Any<Loki::NullType, TPredicate>
		{
			// empty don't matches, right?
			static constexpr bool value = false;
		};
				
		template <class THead, class TTail, template<class> class TPredicate>
		struct Any<Loki::Typelist<THead, TTail>, TPredicate>
		{
			static constexpr bool value = TPredicate<THead>::value | Any<TTail, TPredicate>::value;
		};
		
		template<class TList, template<class> class TPredicate>
		constexpr bool GetAll(){
			return All<TList, TPredicate>::value;
		}
		
		template<class TList, template<class> class TPredicate>
		constexpr bool GetAny(){
			return Any<TList, TPredicate>::value;
		}
		
		// returns list length
		template <class TList>
		constexpr int GetLength()
		{
			return Loki::TL::Length<TList>::value;
		}
	}	
}



#endif /* TYPELIST_H_ */