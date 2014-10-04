/*
 * dynamicpinlist.h
 *
 * Created: 02.02.2013 15:25:36
 *  Author: astr0
 */ 

#pragma once

#ifndef DYNAMICPINLIST_H_
#define DYNAMICPINLIST_H_

#include "../../../loki/Typelist.h"
#include "../../../common/typelist.h"
#include "../../../common/variadic.h"
#include "staticpinlist.h"
#include "../../pin/pininfo.h"

namespace smcu
{
	namespace io
	{
		namespace types
		{
			namespace priv
			{
				// returns true for static pin
				template<class TPinHolder>
				struct IsStaticPinPredicate
				{
					static constexpr bool value = TPinHolder::Pin::IsStatic();
				};
			
				// predicate: creates typelist where dynamic pins are replaced with NullPin
				template<class TPinHolder>
				struct SelectStaticPinList
				{
					private:
					static constexpr bool IsStatic = IsStaticPinPredicate<TPinHolder>::value;
				
					public:
					typedef typename Loki::Select<IsStatic, TPinHolder, PinPositionHolder<TPinHolder::Position, smcu::io::types::NullPin>>::Result Result;
				};	

				constexpr bool IsDynamicAutoUpdate()
				{
					return true;
				}
				
				template<class TPin, class ...TPins>
				constexpr bool IsDynamicAutoUpdate(const TPin pin, const TPins... pins)
				{
					return (TPin::IsStatic() || pin.Port().IsAutoUpdate()) && IsDynamicAutoUpdate(pins...);
				}
			
			
				// dynamic pin wrapper
				template<class ValueType, class TList>
				class DynamicPinWrapper;
			
				template<class ValueType>
				class DynamicPinWrapper<ValueType, Loki::NullType>
				{
					private:
					typedef ValueType DataType;
					typedef smcu::common::BitMaskTypes<DataType> BitMaskTypes;
					typedef typename BitMaskTypes::OneBitMask PinMaskType;
					typedef typename BitMaskTypes::BitNumberType PinNumberType;
					typedef typename BitMaskTypes::MaskType MaskType;
				
					public:
						template<class ...PINS>
						constexpr DynamicPinWrapper(const PINS... pins)
						{
						}
					
						inline void Write(DataType value) const
						{
						}
						inline void ClearAndSet(MaskType clearMask, MaskType setMask)const
						{
						}
						inline void Set(MaskType mask)const
						{
						}
						inline void Clear(MaskType mask)const
						{
						}
						inline void Toggle(MaskType mask) const
						{
						}
						inline void Read(DataType& result)const
						{
						}
					
						inline void DoUpdate(MaskType mask) const
						{				
						}
					
						inline void DoRefresh(MaskType mask) const
						{
						}
						
						template<PinNumberType VPin>
						constexpr Loki::NullType Pin()
						{
							//static_assert(false, "Should not be called.");
							return Loki::NullType();
						}						
				};
			
				template<class ValueType, class Head, class Tail>
				class DynamicPinWrapper<ValueType, Loki::Typelist<Head, Tail>>
				{
					private:
						typedef ValueType DataType;
						typedef smcu::common::BitMaskTypes<DataType> BitMaskTypes;
						typedef typename BitMaskTypes::OneBitMask PinMaskType;
						typedef typename BitMaskTypes::BitNumberType PinNumberType;
						typedef typename BitMaskTypes::MaskType MaskType;
					
					
						typedef typename Head::Pin PinType;
						typedef typename PinInfo<PinType>::PortType PortType;
					
						typedef DynamicPinWrapper<ValueType, Tail> NextWrapper;
					
						static constexpr PinNumberType Position = Head::Position;
						static constexpr PinMaskType ValueMask = BitMaskTypes::BitToMask(Position);
						const PinType _pin;
						const NextWrapper Next;
					public:
						template<class ...PINS>
						constexpr DynamicPinWrapper(const PINS... pins): 
							_pin(smcu::common::variadic::GetValueAt<Position, const PinType, const PINS...>::Get(pins...)),
							Next(pins...)
						{
						}
					
						inline void Write(DataType value) const
						{
							_pin.Set(value & ValueMask);
							Next.Write(value);
						}
						inline void ClearAndSet(MaskType clearMask, MaskType setMask)const
						{
							if (clearMask & ValueMask)
							{
								if (!(setMask & ValueMask))
									_pin.Clear();
							} else if (setMask & ValueMask)
								_pin.Set();						
							Next.ClearAndSet(clearMask, setMask);
						}
						inline void Set(MaskType mask)const
						{
							if (mask & ValueMask)
								_pin.Set();
							Next.Set(mask);
						}
						inline void Clear(MaskType mask)const
						{
							if (mask & ValueMask)
								_pin.Clear();
							Next.Clear(mask);
						}
						inline void Toggle(MaskType mask) const
						{
							if (mask & ValueMask)
								_pin.Toggle(mask);
							Next.Toggle(mask);
						}
						inline void Read(DataType& result)const
						{
							if (_pin.Read())
								result |= ValueMask;
							Next.Read(result);
						}
										
						inline void DoUpdate(MaskType mask) const
						{
							if (!_pin.Port().IsAutoUpdate() && (mask & ValueMask))
							{
								_pin.Port().Update(_pin.Mask());							
							}
							Next.DoUpdate(mask);
						}

						inline void DoRefresh(MaskType mask) const
						{
							if (!_pin.Port().IsAutoUpdate() && (mask & ValueMask))
							{
								_pin.Port().Refresh(_pin.Mask());
							}
							Next.DoRefresh(mask);
						}
						
						private:
						template<PinNumberType VPin>
						struct MakePinType
						{
							typedef decltype(smcu::common::declval<NextWrapper>().template Pin<VPin>()) NextType;
							typedef typename Loki::Select<VPin == Position, PinType, NextType>::Result	Result;
							typedef typename Loki::Select<VPin == Position, bool, char>::Result	OverloadType;
							static constexpr OverloadType Overload = OverloadType();
						};
						
						// this pin
						template<PinNumberType VPin>
						constexpr typename MakePinType<VPin>::Result MakePin(bool)
						{
							return _pin;
						}
						
						// next pin
						template<PinNumberType VPin>
						constexpr typename MakePinType<VPin>::Result MakePin(char)
						{
							return Next.template Pin<VPin>();
						}
						
						public:
						template<PinNumberType VPin>
						constexpr typename MakePinType<VPin>::Result Pin()
						{
							return MakePin<VPin>(MakePinType<VPin>::Overload);
						}
				};
			}
		}
	}
}


#endif /* DYNAMICPINLIST_H_ */