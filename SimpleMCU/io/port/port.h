/*
* port.h
*
* Created: 31.01.2013 23:52:11
*  Author: astr0
*/

#pragma once


#ifndef PORT_H_
#define PORT_H_

#include "nullport.h"
#include "vport.h"
#include "dynamicvport.h"
#include <io/port_impl.h>
#include "../../loki/Typelist.h"

namespace smcu
{
	namespace io
	{
		namespace priv
		{
			template<class... PINS>
			struct MakeVPort
			{
				typedef typename smcu::io::priv::MakePinList<0, PINS...>::Result Pins;
				typedef typename smcu::io::priv::SelectPins<Pins, smcu::io::priv::IsDynamicPinPredicate>::Result DynamicPins;
				static constexpr uint8_t LengthDynamic = Loki::TL::Length<DynamicPins>::value;
				typedef typename smcu::common::StaticIf<LengthDynamic == 0, smcu::io::priv::VPort<Pins>, smcu::io::priv::DynamicVPort<Pins>>::Result VPortType;
				
				static constexpr VPortType Make(const PINS... pins)
				{
					return LengthDynamic == 0 ? VPortType() : VPortType(pins...);
				}
			};			
		}		
				
		template<class ...PINS>
		constexpr typename priv::MakeVPort<PINS...>::VPortType MakeVPort(const PINS... pins)
		{
			return priv::MakeVPort<PINS...>::Make(pins...);
		}
		
	}
}

#endif /* PORT_H_ */