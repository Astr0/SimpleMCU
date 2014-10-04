/*
* avrport.h
* AVR native port
* Created: 10/4/2014 4:46:30 PM
*  Author: astr0
*/


#ifndef AVRPORT_H_
#define AVRPORT_H_


namespace smcu{
	namespace io{
		namespace types{
			template<class PORTREG>
			class AVRPort
			{
				inline static volatile uint8_t& DDR(){return PORTREG::DDR();}
				inline static volatile uint8_t& PORT(){return PORTREG::PORT();}
				inline static volatile uint8_t& PIN(){return PORTREG::PIN();}
				
				public:
				typedef uint8_t DataType;
				typedef uint8_t MaskType;
				typedef uint8_t PinNumberType;
				
				constexpr AVRPort(){}
				
				inline static constexpr PinNumberType Width(){return sizeof(DataType) * 8;}
				inline static constexpr bool AutoUpdates(){return true;}
				inline static constexpr bool Readable(){return true;}
				inline static constexpr bool Writable(){return true;}
				
				inline static void Write(DataType value) {PORT() = value;}
				inline static void ClearAndSet(MaskType clearMask, MaskType setMask) {PORT() = PORT() & ~clearMask | setMask;}
				inline static void Set(MaskType mask) {PORT() |= mask;}
				inline static void Clear(MaskType mask) {PORT() &= ~mask;}
				inline static void Toggle(MaskType mask) {PORT() ^= mask;}
				inline static DataType Read() {return PIN();}
				inline static MaskType Read(MaskType mask){return Read() & mask;}
				inline static bool ReadAny(MaskType mask){return Read(mask);}
				
				inline static void Flush(MaskType mask){}
				inline static void Poll(MaskType mask){}
			};
		}
	}
}


#endif /* AVRPORT_H_ */