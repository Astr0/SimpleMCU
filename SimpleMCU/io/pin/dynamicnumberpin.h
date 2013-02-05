/*
 * dynamicnumberpin.h
 *
 * Created: 02.02.2013 17:32:23
 *  Author: astr0
 */ 

#pragma once

#ifndef DYNAMICNUMBERPIN_H_
#define DYNAMICNUMBERPIN_H_

#include "../../common/maskutils.h"

namespace smcu
{
	namespace io
	{
		namespace types
		{
			template<class TPort>
			class DynamicNumberPin
			{
				public:
				typedef TPort PortType;
				typedef typename PortType::PinNumberType PinNumberType;
				typedef typename PortType::PinMaskType PinMaskType;
				typedef DynamicNumberPin<TPort> PinType;
				typedef PinType NotInvertedType;

				private:
				const PinMaskType _mask;				
				
				public:
				constexpr DynamicNumberPin(const PinNumberType number): _mask(1 << number)
				{				
				}
			
				static constexpr bool IsStatic(){return false;}
				static constexpr bool IsInverted(){return false;}
				
				static constexpr PortType Port(){return PortType();}
				inline constexpr PinMaskType Mask()const {return _mask;}
				inline constexpr PinNumberType Number()const {return smcu::common::MaskToBit<PinNumberType, PinMaskType>(_mask);}
				inline constexpr NotInvertedType NotInverted()const{return *this;}

				inline void Set()const
				{
					PortType::Set(Mask());
				}
			
				inline void Clear()const
				{
					PortType::Clear(Mask());
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
					PortType::Toggle(Mask());
				}

				inline bool Read()const
				{
					return PortType::Read(Mask());
				}
			};				
		}
	}
}




#endif /* DYNAMICNUMBERPIN_H_ */