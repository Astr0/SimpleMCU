/*
 * staticinvertedpin.h
 *
 * Created: 03.02.2013 1:54:31
 *  Author: astr0
 */ 
#pragma once

#ifndef STATICINVERTEDPIN_H_
#define STATICINVERTEDPIN_H_

namespace smcu
{
	namespace io
	{
		namespace priv
		{
			template<class TPIN>
			class StaticInvertedPin
			{
				private:
				typedef TPIN Pin;
			
				public:
				constexpr StaticInvertedPin()
				{				
				}
			
				typedef typename Pin::PortType PortType;
				static constexpr PortType* Port() {return nullptr;}
				static constexpr uint8_t Number() {return Pin::Number();}
				static constexpr bool IsStatic(){return true;}
				static constexpr bool Inverted(){return !Pin::Inverted();}
				static constexpr typename PortType::DataType Mask() { return Pin::Mask();}

				static void Set()
				{
					Pin::Clear();
				}
			
				static void Clear()
				{
					Pin::Set();
				}
			
				static void Set(bool val)
				{
					Pin::Set(!val);
				}

				static void Toggle()
				{
					Pin::Toggle();
				}

				static bool Read()
				{
					return !Pin::Read();
				}
			
				static void Refresh()
				{
					Pin::Refresh();
				}
			
				static void Update()
				{
					Pin::Update();
				}
			};			
		}
	}	
}




#endif /* STATICINVERTEDPIN_H_ */