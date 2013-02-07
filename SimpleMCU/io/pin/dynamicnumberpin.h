/*
 * dynamicnumberpin.h
 *
 * Created: 02.02.2013 17:32:23
 *  Author: astr0
 */ 

#pragma once

#ifndef DYNAMICNUMBERPIN_H_
#define DYNAMICNUMBERPIN_H_

#include "../port/portinfo.h"

namespace smcu
{
	namespace io
	{
		namespace types
		{
			template<class TPort>
			class DynamicNumberPin
			{
				typedef PortInfo<TPort> PI;
				const typename PI::PinMaskType _mask;				
				
				public:
				constexpr DynamicNumberPin(const typename PI::PinNumberType number): _mask(PI::GetPinMask(number))
				{				
				}
			
				static constexpr bool IsStatic(){return false;}
				static constexpr bool IsInverted(){return false;}
				
				static constexpr TPort Port(){return TPort();}
				inline constexpr typename PI::PinMaskType Mask()const {return _mask;}
				inline constexpr typename PI::PinNumberType Number()const {return PI::GetPinNumber(Mask());}
				inline constexpr DynamicNumberPin<TPort> NotInverted()const{return *this;}

				inline void Set()const
				{
					TPort::Set(Mask());
				}
			
				inline void Clear()const
				{
					TPort::Clear(Mask());
				}
			
				inline void Set(bool val)const
				{
					if (val)
						Set();
					else
						Clear();
				}

				inline void Toggle()const
				{
					TPort::Toggle(Mask());
				}

				inline bool Read()const
				{
					return TPort::Read(Mask());
				}
			};				
		}
	}
}




#endif /* DYNAMICNUMBERPIN_H_ */