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
		namespace types
		{
			template<class TPort, typename TPort::PinNumberType VNumber>
			class StaticPin
			{
				static_assert(VNumber < TPort::Width(), "Pin number out of range");
				public:
				typedef TPort PortType;
				typedef typename PortType::PinNumberType PinNumberType;
				typedef typename PortType::PinMaskType PinMaskType;
				typedef StaticPin<TPort, VNumber> PinType;
				typedef PinType NotInvertedType;

				constexpr StaticPin()
				{
				}

				static constexpr bool IsStatic(){return true;}
				static constexpr bool IsInverted(){return false;}
				
				static constexpr PortType Port() {return PortType();}
				static constexpr PinMaskType Mask() {return 1 << VNumber;}
				static constexpr PinNumberType Number() {return VNumber;}
				static constexpr NotInvertedType NotInverted(){return PinType();}

				static void Set()
				{
					PortType::Set(Mask());
				}
			
				static void Clear()
				{
					PortType::Clear(Mask());
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
					PortType::Toggle(Mask());
				}

				static bool Read()
				{
					return PortType::Read(Mask());
				}
			};
		}
	}	
}



#endif /* STATICPIN_H_ */