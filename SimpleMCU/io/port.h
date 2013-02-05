/*
* port.h
*
* Created: 31.01.2013 23:52:11
*  Author: astr0
*/

#pragma once


#ifndef PORT_H_
#define PORT_H_

/*
	TODO: UNIT TEST
	PORT INTERFACE:

	TYPEDEF STUFF:
		typedef ? PortType; - Port type itself
		typedef ? DataType; - Port Data Type (Read / Write) operations
		typedef ? PinMaskType; - Mask type for 1 pin
		typedef ? PinNumberType; - Pin number type
		typedef ? MaskType; - Mask type for multiple pins
		
	STATIC STUFF:
		constexpr bool IsStatic(){return ?;} - is this port static		
		
	DYNAMIC STUFF:
		constexpr PinNumberType Width(){return ?;} - number of pins in port
		constexpr bool IsAutoUpdate(){return ?;} - do this port requires calls to update / refresh?
			
		void Write(DataType value)const {}
		void ClearAndSet(MaskType / PinMaskType clearMask, MaskType / PinMaskType setMask)const {}
		void Set(MaskType / PinMaskType mask)const {}
		void Clear(MaskType / PinMaskType mask)const {}
		void Toggle(MaskType / PinMaskType mask)const {}
		DataType Read()const {return 0;}
		bool Read(PinMaskType pin)const{}

		void Update(MaskType / PinMaskType mask)const{}
		void Refresh(MaskType / PinMaskType mask)const{}
					
		constexpr PossiblyStaticPin<PORT, Number> Pin<Number>(){return StaticPin<PortType, Number>();}
		constexpr Pin<PORT> Pin(){return Pin<PORT>(*this, index);}
	
*/

#include "port/portbase.h"
#include "port/nullport.h"
#include "port/portpointer.h"
#include "port/vport.h"
#include "port/dynamicvport.h"
#include <io/port_impl.h>

namespace smcu
{
	namespace io
	{
		namespace priv
		{
			template<class... TPins>
			struct MakeVPort
			{
				typedef typename smcu::io::types::priv::MakePinList<TPins...>::Result Pins;
				
				struct MakeStatic
				{
					typedef smcu::io::types::VPort<Pins> VPortType;
					
					static constexpr VPortType Make(const TPins... pins)
					{
						return VPortType();
					}
				};		
				
				struct MakeDynamic
				{
					typedef smcu::io::types::DynamicVPort<Pins> VPortType;
					
					static constexpr VPortType Make(const TPins... pins)
					{
						return VPortType(pins...);
					}
				};				
							
				typedef typename smcu::common::TL::Where<Pins, smcu::common::TL::Not<smcu::io::types::priv::IsStaticPinPredicate>::Result>::Result DynamicPins;
				static constexpr int LengthDynamic = smcu::common::TL::Length<DynamicPins>();
				typedef typename Loki::Select<LengthDynamic == 0, MakeStatic, MakeDynamic>::Result MakeType;
				typedef typename MakeType::VPortType VPortType;
				
				static constexpr VPortType Make(const TPins... pins)
				{
					return MakeType::Make(pins...);
				}
			};
		}
		
		template<class ...TPins>
		constexpr typename priv::MakeVPort<TPins...>::VPortType MakeVPort(const TPins... pins)
		{
			return priv::MakeVPort<TPins...>::Make(pins...);
		}
	}
}	

#endif /* PORT_H_ */