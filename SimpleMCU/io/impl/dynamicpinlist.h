/*
 * dynamicpinlist.h
 *
 * Created: 02.02.2013 15:25:36
 *  Author: astr0
 */ 

#pragma once

#ifndef DYNAMICPINLIST_H_
#define DYNAMICPINLIST_H_

#include "../../loki/Typelist.h"
#include "../../common/util.h"
#include "pinlist.h"

namespace smcu
{
	namespace io
	{
		namespace priv
		{
			// returns true for Pin<PORT, POS, INV> (static pin)
			template<class Pin>
			struct IsStaticPin
			{
				static constexpr bool value = Pin::IsStatic();
			};
			
			// true if not static
			template<class PinHolder>
			struct IsDynamicPinPredicate
			{
				static constexpr bool value = !(IsStaticPin<typename PinHolder::Pin>::value);
			};

			
			// converts pin list to include only static pins or NullPin
			template<class TList>
			struct MakeStaticPinList;
			
			template<>
			struct MakeStaticPinList<Loki::NullType>
			{
				typedef Loki::NullType Result;
			};
			
			template<class H, class T>
			struct MakeStaticPinList<Loki::Typelist<H, T>>
			{
				private:
				static constexpr bool IsStatic = IsStaticPin<typename H::Pin>::value;
				
				typedef typename smcu::common::StaticIf<IsStatic, H, PinPositionHolder<NullPin, H::Position>>::Result Holder;
				public:				
				typedef Loki::Typelist<Holder, typename MakeStaticPinList<T>::Result> Result;
			};	
			
			
			// pin pointer list
			
			
			// dynamic pin wrapper
			template<class ValueType, class TList>
			class DynamicPinWrapper;
			
			template<class ValueType>
			class DynamicPinWrapper<ValueType, Loki::NullType>
			{
				private:
				typedef ValueType DataType;
				public:
					template<class ...PINS>
					constexpr DynamicPinWrapper(const PINS... pins)
					{
					}
					
					inline void Write(DataType value) const
					{
					}
					inline void ClearAndSet(DataType clearMask, DataType setMask)const
					{
					}
					inline void Set(DataType mask)const
					{
					}
					inline void Clear(DataType mask)const
					{
					}
					inline void Toggle(DataType mask) const
					{
					}
					inline DataType Read()const
					{
						return 0;
					}
					
					static constexpr bool HasControllers() { return false; }
					
					inline void DoUpdate(DataType mask) const
					{				
					}
					
					inline void DoRefresh(DataType mask) const
					{
					}
			};
			
			template<class ValueType, class Head, class Tail>
			class DynamicPinWrapper<ValueType, Loki::Typelist<Head, Tail>>
			{
				private:
					typedef ValueType DataType;
					typedef typename Head::Pin PinType;
					typedef typename PinType::PortType PortType;
					typedef typename PortType::ControllerType ControllerType;
					
					typedef DynamicPinWrapper<ValueType, Tail> NextWrapper;
					
					static constexpr uint8_t Position = Head::Position;
					static constexpr DataType ValueMask = 1 << Position;
					const PinType _pin;
					const NextWrapper Next;
				public:
					template<class ...PINS>
					constexpr DynamicPinWrapper(const PINS... pins): 
						_pin(smcu::common::GetValueAt<Position, const PinType, const PINS...>::Get(pins...)),
						Next(pins...)
					{
					}
					
					inline void Write(DataType value) const
					{
						if (value & ValueMask)
							_pin.Set();
						else 
							_pin.Clear();
						Next.Write(value);
					}
					inline void ClearAndSet(DataType clearMask, DataType setMask)const
					{
						if (clearMask & ValueMask)
						{
							if (!(setMask & ValueMask))
								_pin.Clear();
						} else if (setMask & ValueMask)
							_pin.Set();						
						Next.ClearAndSet(clearMask, setMask);
					}
					inline void Set(DataType mask)const
					{
						if (mask & ValueMask)
							_pin.Set();
						Next.Set(mask);
					}
					inline void Clear(DataType mask)const
					{
						if (mask & ValueMask)
							_pin.Clear();
						Next.Clear(mask);
					}
					inline void Toggle(DataType mask) const
					{
						if (mask & ValueMask)
							_pin.Toggle(mask);
						Next.Toggle(mask);
					}
					inline DataType Read()const
					{
						return (_pin.Read() << Position) | Next.Read();
					}
										
					static constexpr bool HasControllers() { return ControllerType::NeedUpdate() || NextWrapper::HasControllers();}
						
					inline void DoUpdate(DataType mask) const
					{
						if (ControllerType::NeedUpdate())
						{
							if (mask & ValueMask)
								_pin->Port()->Controller()->Update();
						}							
						Next.DoUpdate(mask);
					}

					inline void DoRefresh(DataType mask) const
					{
						if (ControllerType::NeedUpdate())
						{
							if (mask & ValueMask)
								_pin->Port()->Controller()->Refresh();
						}							
						Next.DoRefresh(mask);
					}
			};
		}
	}
}


#endif /* DYNAMICPINLIST_H_ */