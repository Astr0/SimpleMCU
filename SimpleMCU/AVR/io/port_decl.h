/*
 * port_decl.h
 *
 * Created: 01.02.2013 17:01:04
 *  Author: astr0
 */ 

#pragma once

#ifndef PORT_DECL_H_
#define PORT_DECL_H_

#include "../../common/utils.h"


#define SMCU_DECLAREPORT(CODE)\
namespace priv\
{\
	SMCU_WRAPVARIABLE(PortReg ## CODE, PORT ## CODE)\
	SMCU_WRAPVARIABLE(PinReg ## CODE, PIN ## CODE)\
	SMCU_WRAPVARIABLE(DdrReg ## CODE, DDR ## CODE)\
}\
static constexpr types::AVRPort<priv::DdrReg ## CODE, priv::PortReg ## CODE, priv::PinReg ## CODE> Port ## CODE;\
static constexpr auto Pin ## CODE ## 0 = Port ## CODE.Pin<0>();\
static constexpr auto Pin ## CODE ## 1 = Port ## CODE.Pin<1>();\
static constexpr auto Pin ## CODE ## 2 = Port ## CODE.Pin<2>();\
static constexpr auto Pin ## CODE ## 3 = Port ## CODE.Pin<3>();\
static constexpr auto Pin ## CODE ## 4 = Port ## CODE.Pin<4>();\
static constexpr auto Pin ## CODE ## 5 = Port ## CODE.Pin<5>();\
static constexpr auto Pin ## CODE ## 6 = Port ## CODE.Pin<6>();\
static constexpr auto Pin ## CODE ## 7 = Port ## CODE.Pin<7>();\
static constexpr auto Pin ## CODE ## 0Inv = MakeInvertedPin( Pin ## CODE ## 0);\
static constexpr auto Pin ## CODE ## 1Inv = MakeInvertedPin( Pin ## CODE ## 1);\
static constexpr auto Pin ## CODE ## 2Inv = MakeInvertedPin( Pin ## CODE ## 2);\
static constexpr auto Pin ## CODE ## 3Inv = MakeInvertedPin( Pin ## CODE ## 3);\
static constexpr auto Pin ## CODE ## 4Inv = MakeInvertedPin( Pin ## CODE ## 4);\
static constexpr auto Pin ## CODE ## 5Inv = MakeInvertedPin( Pin ## CODE ## 5);\
static constexpr auto Pin ## CODE ## 6Inv = MakeInvertedPin( Pin ## CODE ## 6);\
static constexpr auto Pin ## CODE ## 7Inv = MakeInvertedPin( Pin ## CODE ## 7);


namespace smcu
{
	namespace io
	{
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

#undef SMCU_DECLAREPORT

#endif /* PORT_DECL_H_ */