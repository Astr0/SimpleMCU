/*
 * pin.h
 *
 * Created: 03.02.2013 1:39:43
 *  Author: astr0
 */ 


#ifndef PIN_H_
#define PIN_H_

#include "staticpin.h"
#include "dynamicpin.h"
#include "staticinvertedpin.h"
#include "dynamicinvertedpin.h"
#include "../../common/util.h"

namespace smcu
{
	namespace io
	{
		namespace priv
		{
			template<class PIN>
			struct MakeInvertedPin
			{
				struct MakeStatic
				{
					static constexpr StaticInvertedPin<PIN> Make(const PIN pin)
					{
						return StaticInvertedPin<PIN>();
					}					
				};

				struct MakeDynamic
				{
					static constexpr DynamicInvertedPin<PIN> Make(const PIN pin)
					{
						return DynamicInvertedPin<PIN>(pin);
					}
				};
								
				typedef typename smcu::common::StaticIf<PIN::IsStatic(), StaticInvertedPin<PIN>, DynamicInvertedPin<PIN>>::Result InvertedType;
				typedef typename smcu::common::StaticIf<PIN::IsStatic(), MakeStatic, MakeDynamic>::Result MakeType;
			};
		}
		
		template<class PORT>
		constexpr priv::DynamicPin<PORT> MakeDynamicPin(const PORT* port, uint8_t number)
		{
			return priv::DynamicPin<PORT>(port, number);
		}
		
		template<class PIN>
		constexpr typename priv::MakeInvertedPin<PIN>::InvertedType MakeInvertedPin(const PIN pin)
		{
			return priv::MakeInvertedPin<PIN>::MakeType::Make(pin);
		}
		
	}
}



#endif /* PIN_H_ */