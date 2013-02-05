/*
 * nullport.h
 *
 * Created: 03.02.2013 1:37:39
 *  Author: astr0
 */ 


#ifndef NULLPORT_H_
#define NULLPORT_H_

#include "portbase.h"
#include "../pin.h"

namespace smcu
{
	namespace io
	{
		namespace types
		{
			class NullPort:public PortBase<uint8_t>
			{
				public:
				constexpr NullPort(){}
					
				typedef NullPort PortType;
				
				static constexpr bool IsStatic(){return true;}
				
				static constexpr PinNumberType Width(){return 8 * sizeof(DataType);}
				static constexpr bool IsAutoUpdate(){return true;}
				
				static void Write(DataType value) {}
				static void ClearAndSet(MaskType clearMask, MaskType setMask) {}
				static void Set(MaskType mask) {}
				static void Clear(MaskType mask) {}
				static void Toggle(MaskType mask) {}
				static constexpr DataType Read() {return 0;}
				static constexpr bool Read(PinMaskType pin){return false;}
					
				static void Update(MaskType mask){}
				static void Refresh(MaskType mask){}
					
				template<PinNumberType VNumber>
				static constexpr StaticPin<PortType, VNumber> Pin()
				{
					return StaticPin<PortType, VNumber>();
				}
					
				static constexpr DynamicNumberPin<PortType> Pin(PinNumberType number){return DynamicNumberPin<PortType>(number);}
			};
			typedef StaticPin<NullPort, 0> NullPin;
		}
		static constexpr types::NullPort NullPort;
		static constexpr auto NullPin = NullPort.Pin<0>();
	}
}



#endif /* NULLPORT_H_ */