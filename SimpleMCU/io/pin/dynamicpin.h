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
				public:
				typedef TPort PortType;
				typedef typename PortType::PinNumberType PinNumberType;
				typedef typename PortType::PinMaskType PinMaskType;
				typedef DynamicPin<TPort> PinType;
				typedef PinType NotInvertedType;

				private:
				const PinMaskType _mask;	
				const PortType _port;			

				public:
				constexpr DynamicPin(const PortType port, const PinNumberType number):
					_port(port), _mask(1 << number)
				{				
				}
			
				static constexpr bool IsStatic(){return false;}
				static constexpr bool IsInverted(){return false;}
				
				inline constexpr PortType Port()const {return _port;}
				inline constexpr PinMaskType Mask()const {return _mask;}
				inline constexpr PinNumberType Number()const {return smcu::common::MaskToBit<PinNumberType, PinMaskType>(_mask);}
				inline constexpr NotInvertedType NotInverted()const{return *this;}

				inline void Set()const
				{
					Port().Set(Mask());
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