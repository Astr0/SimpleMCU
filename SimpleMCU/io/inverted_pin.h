/*
 * inverted_pin.h
 *
 * Created: 10/4/2014 6:07:18 PM
 *  Author: astr0
 */ 


#ifndef INVERTED_PIN_H_
#define INVERTED_PIN_H_


namespace smcu
{
	namespace io
	{
		namespace types
		{
			template<class TPin>
			class InvertedPin
			{
				public:
				// Port type
				typedef typename TPin::PortType PortType;
				// Mask type
				typedef typename TPin::MaskType MaskType;
				// Number type
				typedef typename TPin::NumberType NumberType;
				// Inverted pin type
				typedef TPin InvertedPinType;
				
				constexpr InvertedPin()
				{
				}

				// Port
				inline static constexpr PortType& Port() {return TPin::Port();}
				// Number
				inline static constexpr NumberType Number() {return TPin::Number();}
				// Is Inverted?
				inline static constexpr bool Inverted(){return !TPin::Inverted();}
				// Make inverted pin
				inline static constexpr InvertedPinType MakeInverted(){return TPin();}

				// Set pin
				inline static void Set()
				{
					TPin::Clear();
				}
				
				// Clear pin
				inline static void Clear()
				{
					TPin::Set();
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
					TPin::Toggle();
				}

				// Read pin
				inline static bool Read()
				{
					return !TPin::Read();
				}
			};
		}
	}
}



#endif /* INVERTED_PIN_H_ */