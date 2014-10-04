/*
 * pin.h
 *
 * Created: 03.02.2013 1:39:43
 *  Author: astr0
 */ 

#ifndef PIN_H_
#define PIN_H_

/*
	TODO: UNIT TEST
	PIN INTERFACE:

	STATIC STUFF:
		static constexpr bool IsStatic(){return ?;} - is this pin static		
		static constexpr bool IsInverted(){return ?;} // is this pin inverted
		
	DYNAMIC STUFF:
		inline constexpr PortType Port() {return ?;} // return port copy
		inline constexpr PinMaskType Mask() {return ?;} // return this pin's mask
		inline constexpr NumberType Number(){return ?;} // return this pin's number
		inline constexpr NotInvertedType NotInverted() {return ?;}// for not-inverted returns copy, for inverted returns underlying pin / inverted again pin (if underlying is lost somehow)
			
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
*/

#include "pin/pininfo.h"
#include "pin/staticpin.h"
#include "pin/dynamicnumberpin.h"
#include "pin/dynamicpin.h"

#include "pin/staticinvertedpin.h"
#include "pin/dynamicinvertedpin.h"

#include "../loki/TypeManip.h"

namespace smcu
{
	namespace io
	{		
		namespace priv
		{
			template<class TPin>
			struct MakeInvertedPin
			{
				struct MakeNotInverted
				{
					typedef typename smcu::io::types::PinInfo<TPin>::NotInvertedType InvType;
					static constexpr InvType Make(const TPin pin)
					{
						return pin.NotInverted();
					}
				};
				
				struct MakeStatic
				{
					typedef smcu::io::types::StaticInvertedPin<TPin> InvType;
					static constexpr InvType Make(const TPin pin)
					{
						return InvType();
					}
				};

				struct MakeDynamic
				{
					typedef smcu::io::types::DynamicInvertedPin<TPin> InvType;
					static constexpr InvType Make(const TPin pin)
					{
						return InvType(pin);
					}
				};
				
				typedef typename Loki::Select<TPin::IsInverted(), MakeNotInverted, 
					typename Loki::Select<TPin::IsStatic(), MakeStatic, MakeDynamic>::Result>::Result Result;
			};
			
			template<class TPort>
			struct MakePin
			{
				typedef typename smcu::io::types::PortInfo<TPort>::PinNumberType PinNumberType;
				struct MakeStatic
				{
					typedef smcu::io::types::DynamicNumberPin<TPort> PinType;
					static constexpr PinType Make(const TPort port, const PinNumberType number)
					{
						return PinType(number);
					}
				};

				struct MakeDynamic
				{
					typedef smcu::io::types::DynamicPin<TPort> PinType;
					static constexpr PinType Make(const TPort port, const PinNumberType number)
					{
						return PinType(port, number);
					}
				};
				
				typedef typename Loki::Select<TPort::IsStatic(), MakeStatic, MakeDynamic>::Result Result;
			};
		}
		
		template<class TPin>
		constexpr typename priv::MakeInvertedPin<TPin>::Result::InvType MakeInvertedPin(const TPin pin)
		{
			return priv::MakeInvertedPin<TPin>::Result::Make(pin);
		}	
		
		template<class TPort>
		constexpr typename priv::MakePin<TPort>::Result::PinType MakePin(const TPort port, const typename priv::MakePin<TPort>::PinNumberType number)
		{
			return priv::MakePin<TPort>::Result::Make(port, number);
		}		
	}
}

#endif /* PIN_H_ */