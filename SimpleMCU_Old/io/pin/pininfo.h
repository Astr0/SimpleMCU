/*
 * pininfo.h
 *
 * Created: 06.02.2013 1:10:22
 *  Author: astr0
 */ 


#ifndef PININFO_H_
#define PININFO_H_

#include "../../common/typeutils.h"

namespace smcu
{
	namespace io
	{
		namespace types
		{
			template<class TPin>
			struct PinInfo
			{
				typedef TPin PinType;
				typedef decltype(smcu::common::declval<TPin>().Port()) PortType;
				typedef decltype(smcu::common::declval<TPin>().Mask()) PinMaskType;
				typedef decltype(smcu::common::declval<TPin>().Number()) PinNumberType;
				typedef decltype(smcu::common::declval<TPin>().NotInverted()) NotInvertedType;
			};
		}
	}
}


#endif /* PININFO_H_ */