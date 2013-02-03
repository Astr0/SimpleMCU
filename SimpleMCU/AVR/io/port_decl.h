/*
 * port_decl.h
 *
 * Created: 01.02.2013 17:01:04
 *  Author: astr0
 */ 

#pragma once

#ifndef PORT_DECL_H_
#define PORT_DECL_H_

#include "../../common/util.h"

/*#define SMCU_DECLAREPORT(CODE)\
namespace priv\
{\
	SMCU_WRAPVARIABLE(PortReg ## CODE, PORT ## CODE)\
	SMCU_WRAPVARIABLE(PinReg ## CODE, PIN ## CODE)\
	SMCU_WRAPVARIABLE(DdrReg ## CODE, DDR ## CODE)\
	typedef AVRPort<DdrReg ## CODE, PortReg ## CODE, PinReg ## CODE> Port ## CODE ## Type;\
	typedef Pin<Port ## CODE ## Type, 0> Pin ## CODE ## 0Type;\
	typedef Pin<Port ## CODE ## Type, 1> Pin ## CODE ## 1Type;\
	typedef Pin<Port ## CODE ## Type, 2> Pin ## CODE ## 2Type;\
	typedef Pin<Port ## CODE ## Type, 3> Pin ## CODE ## 3Type;\
	typedef Pin<Port ## CODE ## Type, 4> Pin ## CODE ## 4Type;\
	typedef Pin<Port ## CODE ## Type, 5> Pin ## CODE ## 5Type;\
	typedef Pin<Port ## CODE ## Type, 6> Pin ## CODE ## 6Type;\
	typedef Pin<Port ## CODE ## Type, 7> Pin ## CODE ## 7Type;\
	typedef Pin<Port ## CODE ## Type, 0, true> Pin ## CODE ## 0InvType;\
	typedef Pin<Port ## CODE ## Type, 1, true> Pin ## CODE ## 1InvType;\
	typedef Pin<Port ## CODE ## Type, 2, true> Pin ## CODE ## 2InvType;\
	typedef Pin<Port ## CODE ## Type, 3, true> Pin ## CODE ## 3InvType;\
	typedef Pin<Port ## CODE ## Type, 4, true> Pin ## CODE ## 4InvType;\
	typedef Pin<Port ## CODE ## Type, 5, true> Pin ## CODE ## 5InvType;\
	typedef Pin<Port ## CODE ## Type, 6, true> Pin ## CODE ## 6InvType;\
	typedef Pin<Port ## CODE ## Type, 7, true> Pin ## CODE ## 7InvType;\
}\
static constexpr priv::Port ## CODE ## Type Port ## CODE;\
static constexpr priv::Pin ## CODE ## 0Type Pin ## CODE ## 0;\
static constexpr priv::Pin ## CODE ## 1Type Pin ## CODE ## 1;\
static constexpr priv::Pin ## CODE ## 2Type Pin ## CODE ## 2;\
static constexpr priv::Pin ## CODE ## 3Type Pin ## CODE ## 3;\
static constexpr priv::Pin ## CODE ## 4Type Pin ## CODE ## 4;\
static constexpr priv::Pin ## CODE ## 5Type Pin ## CODE ## 5;\
static constexpr priv::Pin ## CODE ## 6Type Pin ## CODE ## 6;\
static constexpr priv::Pin ## CODE ## 7Type Pin ## CODE ## 7;\
static constexpr priv::Pin ## CODE ## 0InvType Pin ## CODE ## 0Inv;\
static constexpr priv::Pin ## CODE ## 1InvType Pin ## CODE ## 1Inv;\
static constexpr priv::Pin ## CODE ## 2InvType Pin ## CODE ## 2Inv;\
static constexpr priv::Pin ## CODE ## 3InvType Pin ## CODE ## 3Inv;\
static constexpr priv::Pin ## CODE ## 4InvType Pin ## CODE ## 4Inv;\
static constexpr priv::Pin ## CODE ## 5InvType Pin ## CODE ## 5Inv;\
static constexpr priv::Pin ## CODE ## 6InvType Pin ## CODE ## 6Inv;\
static constexpr priv::Pin ## CODE ## 7InvType Pin ## CODE ## 7Inv;
*/

#define SMCU_DECLAREPORT(CODE)\
namespace priv\
{\
	SMCU_WRAPVARIABLE(PortReg ## CODE, PORT ## CODE)\
	SMCU_WRAPVARIABLE(PinReg ## CODE, PIN ## CODE)\
	SMCU_WRAPVARIABLE(DdrReg ## CODE, DDR ## CODE)\
	typedef AVRPort<DdrReg ## CODE, PortReg ## CODE, PinReg ## CODE> Port ## CODE ## Type;\
}\
static constexpr priv::Port ## CODE ## Type Port ## CODE;\
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