/*
 * variadic.h
 *
 * Created: 03.02.2013 23:20:06
 *  Author: astr0
 */ 

#pragma once

#ifndef VARIADIC_H_
#define VARIADIC_H_

#include "../loki/NullType.h"

namespace smcu
{
	namespace common
	{
		namespace variadic
		{
			namespace priv
			{
				// returns length of variadic parameters
				template<class... T>
				struct Length;
				
				template<class T1, class...T>
				struct Length<T1, T...>
				{
					static constexpr int value = 1 + Length<T...>::value;
				};
				
				template<>
				struct Length<>
				{
					static constexpr int value = 0;					
				};
				
				// retuns list of variadic parameters				
				// POSITION, template<POSITION, TElement, TTail> ListElementType, Types>
				template<int POS, template<int, class, class> class ListElement, class... T>
				struct MakeList;
			
				template<int POS, template<int, class, class> class ListElement, class THead, class... T>
				struct MakeList<POS, ListElement, THead, T...>
				{
					private:
					typedef MakeList<POS+1, ListElement, T...> TailMakeList;
					typedef typename TailMakeList::Result TailType;
					public:
					typedef ListElement<POS, THead, TailType> Result;
				
					static constexpr Result Create(const THead head, const T... other)
					{
						return Result(head, TailMakeList::Create(other...));
					}
				};
			
				template<int POS, template<int, class, class> class ListElement>
				struct MakeList<POS, ListElement>
				{
					typedef ListElement<POS, Loki::NullType, Loki::NullType> Result;
					static constexpr Result Create()
					{
						return Result();
					}
				};				
			}
			
			// length of variadic parameters
			template<class... T>			
			constexpr int Length()
			{
				return priv::Length<T...>::value;
			}		
			
			// variadic list base element
			template<int POS, class TElement, class TTail>
			struct ListElement
			{
				static constexpr int Position = POS;
				const TElement Element;
				const TTail Tail;
				
				constexpr ListElement(const TElement element, const TTail tail):
					Element(element), Tail(tail)
				{
					
				}
			};
			
			template<int POS>
			struct ListElement<POS, Loki::NullType, Loki::NullType>
			{
				constexpr ListElement()
				{
					
				}				
			};
			
			// make variadic list
			template<template<int, class, class> class ListElement>
			struct MakeList
			{
				template<class...T>
				struct List
				{
					private:
					typedef priv::MakeList<0, ListElement, T...> ML;
					public:
					typedef typename ML::Result Result;
					static constexpr Result Create(const T... val)
					{
						return ML::Create(val...);
					}				
				};
				
				template<class...T>
				static constexpr typename List<T...>::Result Create(const T... val)
				{
					return List<T...>::Create(val...);
				}
			};
			
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
}

#endif /* VARIADIC_H_ */