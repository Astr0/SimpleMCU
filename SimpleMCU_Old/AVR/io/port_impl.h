/*
 * port_impl.h
 *
 * Created: 01.02.2013 16:33:04
 *  Author: astr0
 */ 
#pragma once


#ifndef PORT_IMPL_H_
#define PORT_IMPL_H_

#include "../../io/pin.h"

namespace smcu
{
	namespace io
	{
		namespace types
		{
			template<class DDRREG, class PORTREG, class PINREG>
			class AVRPort
			{
				public:
				constexpr AVRPort(){}
				
				static constexpr bool IsStatic(){return true;}
					
				static constexpr uint8_t Width(){return 8;}			
				static constexpr bool IsAutoUpdate(){return true;}
			
				inline static void Write(uint8_t value) {PORTREG::value() = value;}
				inline static void ClearAndSet(uint8_t clearMask, uint8_t setMask) {PORTREG::value() = PORTREG::value() & ~clearMask | setMask;}
				inline static void Set(uint8_t mask) {PORTREG::value() |= mask;}
				inline static void Clear(uint8_t mask) {PORTREG::value() &= ~mask;}
				inline static void Toggle(uint8_t mask) {PORTREG::value() ^= mask;}
				inline static uint8_t Read() {return PINREG::value();}
				inline static bool Read(uint8_t mask){return Read() & mask;}
					
				inline static void Update(uint8_t mask){}
				inline static void Refresh(uint8_t mask){}
					
				template<unsigned VNumber>
				static constexpr StaticPin<AVRPort<DDRREG, PORTREG, PINREG>, VNumber> Pin()
				{
					return StaticPin<AVRPort<DDRREG, PORTREG, PINREG>, VNumber>();
				}
			};	
		}
	}	
}

#include "port_decl.h"

#endif /* PORT_IMPL_H_ */