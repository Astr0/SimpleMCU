/*
 * ports.h
 *
 * Created: 10/4/2014 4:19:29 PM
 *  Author: astr0
 */ 


#ifndef PORTS_H_
#define PORTS_H_

/*Defines port interfaces
 class Port
 {
		public:
		// Port data type
		typedef uint8_t DataType;
		// Port mask type
		typedef uint8_t MaskType;
		// Pin number type
		typedef uint8_t PinNumberType;		
		
		// Number of pins in port
		static constexpr PinNumberType Width(){return 8;}
		// Is this port auto-updated?
		static constexpr bool AutoUpdates(){return true;}
		// Is this port readable?
		static constexpr bool Readable(){return true;}
		// Is this port writable?
		static constexpr bool Writable(){return true;}
		
		// Write the whole port
		inline static void Write(DataType value) {PORTREG::value() = value;}
	    // clear mask and set mask
		inline static void ClearAndSet(MaskType clearMask, MaskType setMask) {PORTREG::value() = PORTREG::value() & ~clearMask | setMask;}
	    // set mask
		inline static void Set(MaskType mask) {PORTREG::value() |= mask;}
	    // clear mask
		inline static void Clear(MaskType mask) {PORTREG::value() &= ~mask;}
		// toggle mask
		inline static void Toggle(MaskType mask) {PORTREG::value() ^= mask;}
		// read the whole port
		inline static DataType Read() {return PINREG::value();}
		// read mask, returns what is set
		inline static MaskType Read(MaskType mask){return Read() & mask;}
		// read mask, returns true if anything set
		inline static bool ReadAny(MaskType mask){return ReadMask(mask);}
		
		// Commits mask
		inline static void Flush(MaskType mask){}
		// Polls for mask
		inline static void Poll(MaskType mask){}
			
		// 
 */


// include MCU's native ports
#include <io/native_ports.h>
#include "pinport.h"


#endif /* PORTS_H_ */