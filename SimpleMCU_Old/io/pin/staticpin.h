/*
 * pin.h
 *
 * Created: 01.02.2013 0:11:32
 *  Author: astr0
 */ 

#pragma once

#ifndef STATICPIN_H_
#define STATICPIN_H_

#include "../port/portinfo.h"

namespace smcu
{
	namespace io
	{
		namespace types
		{
			template<class TPort, unsigned VNumber>
			class StaticPin
			{
				static_assert(VNumber < TPort::Width(), "Pin number out of range");
				typedef PortInfo<TPort> PI;
				public:

				constexpr StaticPin()
				{
				}

				static constexpr bool IsStatic(){return true;}
				static constexpr bool IsInverted(){return false;}
				
				static constexpr TPort Port() {return TPort();}
				static constexpr typename PI::PinMaskType Mask() {return PI::GetPinMask(VNumber);}
				static constexpr typename PI::PinNumberType Number() {return VNumber;}
				static constexpr StaticPin<TPort, VNumber> NotInverted(){return StaticPin<TPort, VNumber>();}

				static void Set()
				{
					TPort::Set(Mask());
				}
			
				static void Clear()
				{
					TPort::Clear(Mask());
				}
			
				static void Set(bool val)
				{
					if (val)
						Set();
					else
						Clear();
				}

				static void Toggle()
				{
					TPort::Toggle(Mask());
				}

				static bool Read()
				{
					return TPort::Read(Mask());
				}
			};
		}
	}	
}



#endif /* STATICPIN_H_ */