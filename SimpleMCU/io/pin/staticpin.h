/*
 * pin.h
 *
 * Created: 01.02.2013 0:11:32
 *  Author: astr0
 */ 

#pragma once

#ifndef STATICPIN_H_
#define STATICPIN_H_

namespace smcu
{
	namespace io
	{
		namespace priv
		{
			template<class PORT, uint8_t PIN>
			class StaticPin;
		
			template<class PORT, uint8_t PIN>
			class StaticPin
			{
				static_assert(PIN < PORT::Width(), "Pin number out of range");
				public:
				constexpr StaticPin()
				{				
				}
			
				typedef PORT PortType;
				static constexpr PortType* Port() {return  nullptr;}
				static constexpr uint8_t Number() {return PIN;}
				static constexpr bool IsStatic(){return true;}
				static constexpr bool Inverted(){return false;}
				static constexpr typename PortType::DataType Mask() {return 1 << PIN;}

				static void Set()
				{
					Port()->Set(Mask());
				}
			
				static void Clear()
				{
					Port()->Clear(Mask());
				}
			
				static void Set(bool val)
				{
					if (val)
						Set();
					else
						Clear();
				}

				static void Toggle()
				{
					Port()->Toggle(Mask());
				}

				static bool Read()
				{
					return (Port()->Read() && Mask());
				}
			
				static void Refresh()
				{
					Port() -> Refresh(Mask());				
				}
			
				static void Update()
				{
					Port() -> Update(Mask());
				}
			};
		}
	}	
}



#endif /* STATICPIN_H_ */