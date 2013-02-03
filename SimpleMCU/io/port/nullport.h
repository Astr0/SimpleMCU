/*
 * nullport.h
 *
 * Created: 03.02.2013 1:37:39
 *  Author: astr0
 */ 


#ifndef NULLPORT_H_
#define NULLPORT_H_

#include "../pin/pin.h"
#include "../impl/portcontroller.h"

namespace smcu
{
	namespace io
	{
		namespace priv
		{
			class NullPort
			{
				public:
				constexpr NullPort(){}
				
				typedef uint8_t DataType;
				typedef NullPortController ControllerType;
				
				static constexpr uint8_t Width(){return 8 * sizeof(DataType);}
				static constexpr bool IsStatic(){return true;}
			
				static constexpr NullPortController* Controller(){return nullptr;}
			
				static void Write(DataType value) {}
				static void ClearAndSet(DataType clearMask, DataType setMask) {}
				static void Set(DataType mask) {}
				static void Clear(DataType mask) {}
				static void Toggle(DataType mask) {}
				static DataType Read() {return 0;}
					
				static void Update(DataType mask){}
				static void Refresh(DataType mask){}
					
				template<uint8_t PIN>
				static constexpr StaticPin<NullPort, 0> Pin(){return StaticPin<NullPort, 0>();}
			};
			typedef StaticPin<NullPort, 0> NullPin;
		}
		static constexpr priv::NullPort NullPort;
		static constexpr priv::NullPin NullPin;
	}
}



#endif /* NULLPORT_H_ */