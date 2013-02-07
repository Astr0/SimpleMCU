/*
 * portinfo.h
 *
 * Created: 06.02.2013 0:58:06
 *  Author: astr0
 */ 


#ifndef PORTINFO_H_
#define PORTINFO_H_

#include "../../common/maskutils.h"
#include "../../common/typeutils.h"

namespace smcu
{
	namespace io
	{
		namespace types
		{
			template<class TPort>
			struct PortInfo
			{
				typedef TPort PortType;
				typedef decltype(smcu::common::declval<TPort>().Read()) DataType;
				private:
				typedef smcu::common::BitMaskTypes<DataType> MT;
				public:
				typedef typename MT::OneBitMask PinMaskType;
				typedef typename MT::BitNumberType PinNumberType;
				typedef typename MT::MaskType MaskType;	
				static constexpr bool OnlyPinInterface = MT::OnlyBitInterface;
				static constexpr PinMaskType GetPinMask(PinNumberType number){return MT::BitToMask(number);}						
				static constexpr PinNumberType GetPinNumber(PinMaskType pinMask){return MT::MaskToBit(pinMask);}
			};
		}
	}
}



#endif /* PORTINFO_H_ */