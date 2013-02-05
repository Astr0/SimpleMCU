/*
 * staticinvertedpin.h
 *
 * Created: 03.02.2013 1:54:31
 *  Author: astr0
 */ 
#pragma once

#ifndef STATICINVERTEDPIN_H_
#define STATICINVERTEDPIN_H_

namespace smcu
{
	namespace io
	{
		namespace types
		{
			template<class TPin>
			class StaticInvertedPin
			{
				static_assert(!TPin::IsInverted(), "Don't invert static inverted pins... This kills compilation time...");
				typedef TPin Pin;
			
				public:
				typedef typename Pin::PortType PortType;
				typedef typename Pin::PinNumberType PinNumberType;
				typedef typename Pin::PinMaskType PinMaskType;
				typedef StaticInvertedPin<Pin> PinType;
				typedef Pin NotInvertedType;

				constexpr StaticInvertedPin()
				{
				}

				static constexpr bool IsStatic(){return true;}
				static constexpr bool IsInverted(){return !Pin::IsInverted();}
				
				static constexpr PortType Port() {return Pin::Port();}
				static constexpr PinMaskType Mask() {return Pin::Mask();}
				static constexpr PinNumberType Number() {return Pin::Number();}
				static constexpr NotInvertedType NotInverted(){return Pin();}

				static void Set()
				{
					Pin::Clear();
				}
			
				static void Clear()
				{
					Pin::Set();
				}
			
				static void Set(bool val)
				{
					Pin::Set(!val);
				}

				static void Toggle()
				{
					Pin::Toggle();
				}

				static bool Read()
				{
					return !Pin::Read();
				}
			};			
		}
	}	
}




#endif /* STATICINVERTEDPIN_H_ */