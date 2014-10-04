/*
 * pins.h
 *
 * Created: 10/4/2014 6:05:47 PM
 *  Author: astr0
 */ 


#ifndef PINS_H_
#define PINS_H_

/*
	Defines pin interface
	
	class Pin
	{
		// Port type
		typedef TPort PortType;
		// Mask type
		typedef typename TPort::MaskType MaskType;
		// Number type
		typedef typename TPort::PinNumberType NumberType;
		// Inverted type
		typedef InvertedPin<Pin> InvertedPinType;
		
		// Port
		inline static constexpr TPort& Port() {return TPort();}
		// Number
		inline static constexpr NumberType Number() {return VNumber;}
		// Is Inverted?
		inline static constexpr bool Inverted(){return !TPin::Inverted();}
		// Make inverted pin
		inline static constexpr InvertedPinType MakeInverted(){return InvertedPinType();}

		// Set pin
		inline static void Set()
		{
			TPort::Set(Mask());
		}
		
		// Clear pin
		inline static void Clear()
		{
			TPort::Clear(Mask());
		}
		
		// Set pin value
		inline static void Set(bool val)
		{
			if (val)
			Set();
			else
			Clear();
		}

		// Toggle pin
		inline static void Toggle()
		{
			TPort::Toggle(Mask());
		}

		// Read pin
		inline static bool Read()
		{
			return TPort::ReadAny(Mask());
		}
	};
*/

#include "pin.h"
#include "inverted_pin.h"


#endif /* PINS_H_ */