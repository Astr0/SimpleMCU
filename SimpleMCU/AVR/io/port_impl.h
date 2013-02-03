/*
 * port_impl.h
 *
 * Created: 01.02.2013 16:33:04
 *  Author: astr0
 */ 
#pragma once


#ifndef PORT_IMPL_H_
#define PORT_IMPL_H_

namespace smcu
{
	namespace io
	{
		namespace priv
		{
			template<class DDRREG, class PORTREG, class PINREG>
			class AVRPort
			{
				public:
				constexpr AVRPort(){}
				
				typedef uint8_t DataType;
				typedef NullPortController ControllerType;
								
				static constexpr uint8_t Width(){return 8 * sizeof(DataType);}
				static constexpr bool IsStatic(){return true;}
			
				static constexpr NullPortController* Controller(){return nullptr;}
			
				inline static void Write(DataType value) {PORTREG::value() = value;}
				inline static void ClearAndSet(DataType clearMask, DataType setMask) {PORTREG::value() = PORTREG::value() & ~clearMask | setMask;}
				inline static void Set(DataType mask) {PORTREG::value() |= mask;}
				inline static void Clear(DataType mask) {PORTREG::value() &= ~mask;}
				inline static void Toggle(DataType mask) {PORTREG::value() ^= mask;}
				inline static DataType Read() {return PINREG::value();}
					
				inline static void Update(DataType mask){}
				inline static void Refresh(DataType mask){}
					
				template<uint8_t PIN>
				static constexpr StaticPin<AVRPort<DDRREG, PORTREG, PINREG>, PIN> Pin()
				{
					return StaticPin<AVRPort<DDRREG, PORTREG, PINREG>, PIN>();
				}
			};	
		}
	}	
}

#include "port_decl.h"

#endif /* PORT_IMPL_H_ */