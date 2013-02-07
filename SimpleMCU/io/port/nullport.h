/*
 * nullport.h
 *
 * Created: 03.02.2013 1:37:39
 *  Author: astr0
 */ 


#ifndef NULLPORT_H_
#define NULLPORT_H_

#include "../pin.h"

namespace smcu
{
	namespace io
	{
		namespace types
		{
			class NullPort
			{
				public:
				constexpr NullPort(){}
					
				typedef NullPort PortType;
				typedef uint8_t DataType;
				
				static constexpr bool IsStatic(){return true;}
				
				static constexpr uint8_t Width(){return 8 * sizeof(DataType);}
				static constexpr bool IsAutoUpdate(){return true;}
				
				static void Write(uint8_t value) {}
				template<class TClear, class TSet>
				static void ClearAndSet(TClear clearMask, TSet setMask) {}
				template<class TMask>
				static void Set(TMask mask) {}
				template<class TMask>
				static void Clear(TMask mask) {}
				template<class TMask>
				static void Toggle(TMask mask) {}
				static constexpr uint8_t Read() {return 0;}
				template<class TMask>
				static constexpr bool Read(TMask pin){return false;}
					
				template<class TMask>
				static void Update(TMask mask){}
				template<class TMask>
				static void Refresh(TMask mask){}
					
				template<unsigned VNumber>
				static constexpr StaticPin<PortType, VNumber> Pin()
				{
					return StaticPin<PortType, VNumber>();
				}
			};
			typedef StaticPin<NullPort, 0> NullPin;
		}
		static constexpr types::NullPort NullPort;
		static constexpr auto NullPin = NullPort.Pin<0>();
	}
}



#endif /* NULLPORT_H_ */