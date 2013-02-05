/*
 * port_impl.h
 *
 * Created: 01.02.2013 16:33:04
 *  Author: astr0
 */ 
#pragma once


#ifndef PORT_IMPL_H_
#define PORT_IMPL_H_

#include "../../io/port/portbase.h"
#include "../../io/pin.h"

namespace smcu
{
	namespace io
	{
		namespace types
		{
			template<class DDRREG, class PORTREG, class PINREG>
			class AVRPort: public PortBase<uint8_t>
			{
				public:
				constexpr AVRPort(){}
				
				typedef AVRPort<DDRREG, PORTREG, PINREG> PortType;
								
				static constexpr bool IsStatic(){return true;}
					
				static constexpr PinNumberType Width(){return 8 * sizeof(DataType);}			
				static constexpr bool IsAutoUpdate(){return true;}
			
				inline static void Write(DataType value) {PORTREG::value() = value;}
				inline static void ClearAndSet(MaskType clearMask, MaskType setMask) {PORTREG::value() = PORTREG::value() & ~clearMask | setMask;}
				inline static void Set(MaskType mask) {PORTREG::value() |= mask;}
				inline static void Clear(MaskType mask) {PORTREG::value() &= ~mask;}
				inline static void Toggle(MaskType mask) {PORTREG::value() ^= mask;}
				inline static DataType Read() {return PINREG::value();}
				inline static bool Read(PinMaskType mask){return Read() & mask;}
					
				inline static void Update(MaskType mask){}
				inline static void Refresh(MaskType mask){}
					
				template<PinNumberType VNumber>
				static constexpr StaticPin<PortType, VNumber> Pin()
				{
					return StaticPin<PortType, VNumber>();
				}
				
				static constexpr DynamicNumberPin<PortType> Pin(PinNumberType number){return DynamicNumberPin<PortType>(number);}
			};	
		}
	}	
}

#include "port_decl.h"

#endif /* PORT_IMPL_H_ */