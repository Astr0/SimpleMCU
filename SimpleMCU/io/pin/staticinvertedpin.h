/*
 * staticinvertedpin.h
 *
 * Created: 03.02.2013 1:54:31
 *  Author: astr0
 */ 
#pragma once

#ifndef STATICINVERTEDPIN_H_
#define STATICINVERTEDPIN_H_

#include "pininfo.h"

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
				typedef PinInfo<TPin> PI;
				public:

				constexpr StaticInvertedPin()
				{
				}

				static constexpr bool IsStatic(){return true;}
				static constexpr bool IsInverted(){return !TPin::IsInverted();}
				
				static constexpr typename PI::PortType Port() {return TPin::Port();}
				static constexpr typename PI::PinMaskType Mask() {return TPin::Mask();}
				static constexpr typename PI::PinNumberType Number() {return TPin::Number();}
				static constexpr typename PI::NotInvertedType NotInverted(){return TPin();}

				static void Set()
				{
					TPin::Clear();
				}
			
				static void Clear()
				{
					TPin::Set();
				}
			
				static void Set(bool val)
				{
					TPin::Set(!val);
				}

				static void Toggle()
				{
					TPin::Toggle();
				}

				static bool Read()
				{
					return !TPin::Read();
				}
			};			
		}
	}	
}




#endif /* STATICINVERTEDPIN_H_ */