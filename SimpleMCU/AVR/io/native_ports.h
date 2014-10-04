/*
* native_ports.h
*
* Created: 10/4/2014 4:27:02 PM
*  Author: astr0
*/


#ifndef NATIVE_PORTS_H_
#define NATIVE_PORTS_H_

#include "avrport.h"

#define SMCU_PORTREG(CODE)\
namespace types{\
	struct PortReg ## CODE{\
		inline static volatile uint8_t& DDR(){return DDR ## CODE;}\
		inline static volatile uint8_t& PORT(){return PORT ## CODE;}\
		inline static volatile uint8_t& PIN(){return PIN ## CODE;}\
	};\
}

#define SMCU_DECLAREPORT(CODE)\
SMCU_PORTREG(CODE)\
typedef types::AVRPort<types::PortReg ## CODE> Port ## CODE;\
typedef types::Pin<Port ## CODE, 0> Pin ## CODE ## 0;\
typedef types::Pin<Port ## CODE, 1> Pin ## CODE ## 1;\
typedef types::Pin<Port ## CODE, 2> Pin ## CODE ## 2;\
typedef types::Pin<Port ## CODE, 3> Pin ## CODE ## 3;\
typedef types::Pin<Port ## CODE, 4> Pin ## CODE ## 4;\
typedef types::Pin<Port ## CODE, 5> Pin ## CODE ## 5;\
typedef types::Pin<Port ## CODE, 6> Pin ## CODE ## 6;\
typedef types::Pin<Port ## CODE, 7> Pin ## CODE ## 7;\
typedef types::InvertedPin<Pin ## CODE ## 0> Pin ## CODE ## 0Inv;\
typedef types::InvertedPin<Pin ## CODE ## 1> Pin ## CODE ## 1Inv;\
typedef types::InvertedPin<Pin ## CODE ## 2> Pin ## CODE ## 2Inv;\
typedef types::InvertedPin<Pin ## CODE ## 3> Pin ## CODE ## 3Inv;\
typedef types::InvertedPin<Pin ## CODE ## 4> Pin ## CODE ## 4Inv;\
typedef types::InvertedPin<Pin ## CODE ## 5> Pin ## CODE ## 5Inv;\
typedef types::InvertedPin<Pin ## CODE ## 6> Pin ## CODE ## 6Inv;\
typedef types::InvertedPin<Pin ## CODE ## 7> Pin ## CODE ## 7Inv;

namespace smcu{
	namespace io{
		#ifdef PORTA
		SMCU_DECLAREPORT(A)
		#endif
		#ifdef PORTB
		SMCU_DECLAREPORT(B)
		#endif
		#ifdef PORTC
		SMCU_DECLAREPORT(C)
		#endif
		#ifdef PORTD
		SMCU_DECLAREPORT(D)
		#endif
		#ifdef PORTE
		SMCU_DECLAREPORT(E)
		#endif
		#ifdef PORTF
		SMCU_DECLAREPORT(F)
		#endif
		#ifdef PORTG
		SMCU_DECLAREPORT(G)
		#endif
		#ifdef PORTH
		SMCU_DECLAREPORT(H)
		#endif
	}
}

#undef SMCU_PORTREG
#undef SMCU_DECLAREPORT

#endif /* NATIVE_PORTS_H_ */