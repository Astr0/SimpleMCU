/*
* pin.h
*
* Created: 10/4/2014 5:29:37 PM
*  Author: astr0
*/


#ifndef PIN_H_
#define PIN_H_


namespace smcu
{
	namespace io
	{
		namespace types
		{
			template<class TPin>
			class InvertedPin;
			
			template<class TPort, unsigned VNumber>
			class Pin
			{
				public:
				// Port type
				typedef TPort PortType;
				// Mask type
				typedef typename TPort::MaskType MaskType;
				// Number type
				typedef typename TPort::PinNumberType NumberType;
				// Inverted pin type
				typedef InvertedPin<Pin> InvertedPinType;
				
				private:
				static_assert(VNumber < TPort::Width(), "Pin number out of range");
				static constexpr MaskType _mask = MaskType(1) << VNumber;
				
				public:
				constexpr Pin()
				{
				}

				// Port
				inline static constexpr TPort& Port() {return TPort();}
				// Number
				inline static constexpr NumberType Number() {return VNumber;}
				inline static constexpr bool Inverted(){return false;}
				// Make inverted pin
				inline static constexpr InvertedPinType MakeInverted(){return InvertedPin<Pin>();}

				// Set pin
				inline static void Set()
				{
					TPort::Set(_mask);
				}
				
				// Clear pin
				inline static void Clear()
				{
					TPort::Clear(_mask);
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
					TPort::Toggle(_mask);
				}

				// Read pin
				inline static bool Read()
				{
					return TPort::ReadAny(_mask);
				}
			};
		}
	}
}

#endif /* PIN_H_ */