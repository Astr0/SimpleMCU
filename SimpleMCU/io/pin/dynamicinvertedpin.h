/*
 * dynamicinvertedpin.h
 *
 * Created: 03.02.2013 2:05:01
 *  Author: astr0
 */ 

#pragma once

#ifndef DYNAMICINVERTEDPIN_H_
#define DYNAMICINVERTEDPIN_H_


namespace smcu
{
	namespace io
	{
		namespace types
		{
			template<class TPin>
			class DynamicInvertedPin
			{
				typedef TPin Pin;
				const Pin _pin;
				static_assert(!Pin::IsInverted(), "Why do you need to invert inverted pin?");

				public:
				typedef typename Pin::PortType PortType;
				typedef typename Pin::PinNumberType PinNumberType;
				typedef typename Pin::PinMaskType PinMaskType;
				typedef DynamicInvertedPin<Pin> PinType;
				typedef Pin NotInvertedType;

				constexpr DynamicInvertedPin(const Pin pin)
					:_pin(pin)
				{				
				}
			
				static constexpr bool IsStatic(){return false;}
				static constexpr bool IsInverted(){return !Pin::IsInverted();}
				
				inline constexpr PortType Port()const {return _pin.Port();}
				inline constexpr PinMaskType Mask()const {return _pin.Mask();}
				inline constexpr PinNumberType Number()const {return _pin.Number();}
				inline constexpr NotInvertedType NotInverted()const{return _pin;}

				inline void Set() const
				{
					_pin.Clear();
				}
			
				inline void Clear() const
				{
					_pin.Set();
				}
			
				inline void Set(bool val) const
				{
					_pin.Set(!val);
				}

				inline void Toggle() const
				{
					_pin.Toggle();
				}

				inline bool Read() const
				{
					return !_pin.Read();
				}
			};			
		}
	}	
}


#endif /* DYNAMICINVERTEDPIN_H_ */