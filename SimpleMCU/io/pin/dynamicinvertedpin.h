/*
 * dynamicinvertedpin.h
 *
 * Created: 03.02.2013 2:05:01
 *  Author: astr0
 */ 

#pragma once

#ifndef DYNAMICINVERTEDPIN_H_
#define DYNAMICINVERTEDPIN_H_



namespace smcu
{
	namespace io
	{
		namespace priv
		{
			template<class TPIN>
			class DynamicInvertedPin
			{
				private:
				typedef TPIN Pin;
				const Pin _pin;
			
				public:
				constexpr DynamicInvertedPin(const Pin pin)
					:_pin(pin)
				{				
				}
			
				typedef typename Pin::PortType PortType;
				constexpr PortType* Port() {return _pin.Port();}
				constexpr uint8_t Number() {return _pin.Number();}
				static constexpr bool IsStatic(){return false;}
				constexpr bool Inverted(){return !_pin.Inverted();}
				constexpr typename PortType::DataType Mask() { return _pin.Mask();}

				void Set() const
				{
					_pin.Clear();
				}
			
				void Clear() const
				{
					_pin.Set();
				}
			
				void Set(bool val) const
				{
					_pin.Set(!val);
				}

				void Toggle() const
				{
					_pin.Toggle();
				}

				bool Read() const
				{
					return !_pin.Read();
				}
			
				void Refresh() const
				{
					_pin.Refresh();
				}
			
				 void Update() const
				{
					_pin.Update();
				}
			};
			
		}
	}	
}


#endif /* DYNAMICINVERTEDPIN_H_ */