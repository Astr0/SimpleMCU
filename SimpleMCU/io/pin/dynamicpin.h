/*
 * dynamicpin.h
 *
 * Created: 04.02.2013 2:48:38
 *  Author: astr0
 */ 


#ifndef DYNAMICPIN_H_
#define DYNAMICPIN_H_

#include "../../common/maskutils.h"

namespace smcu
{
	namespace io
	{
		namespace types
		{
			template<class TPort>
			class DynamicPin
			{
				static_assert(!TPort::IsStatic(), "Use DynamicNumberPin for max performance!");
				typedef PortInfo<TPort> PI;
				const typename PI::PinMaskType _mask;	
				const TPort _port;			

				public:
				constexpr DynamicPin(const TPort port, const typename PI::PinNumberType number):
					_port(port), _mask(PI::GetPinMask(number))
				{				
				}
			
				static constexpr bool IsStatic(){return false;}
				static constexpr bool IsInverted(){return false;}
				
				inline constexpr TPort Port()const {return _port;}
				inline constexpr typename PI::PinMaskType Mask()const {return _mask;}
				inline constexpr typename PI::PinNumberType Number()const {return PI::GetPinNumber(_mask);}
				inline constexpr DynamicPin<TPort> NotInverted()const{return *this;}

				inline void Set()const
				{
					_port.Set(_mask);
				}
			
				inline void Clear()const
				{
					Port().Clear(Mask());
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
					Port().Toggle(Mask());
				}

				inline bool Read()const
				{
					return Port().Read(Mask());
				}
			};				
		}
	}
}




#endif /* DYNAMICPIN_H_ */