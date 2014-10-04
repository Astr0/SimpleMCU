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
*/

#include "port/portinfo.h"
#include "port/nullport.h"
#include "port/portpointer.h"
#include "port/vport.h"
#include <io/port_impl.h>


#endif /* PORT_H_ */