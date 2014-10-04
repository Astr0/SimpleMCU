/*
 * vport.h
 *
 * Created: 05.02.2013 18:57:13
 *  Author: astr0
 */ 


#ifndef VPORT_H_
#define VPORT_H_

#include "staticvport.h"
#include "dynamicvport.h"

namespace smcu
{
	namespace io
	{
		namespace priv
		{
			template<class... TPins>
			struct MakeVPort
			{
				typedef typename smcu::io::types::priv::MakePinList<TPins...>::Result Pins;
				
				struct MakeStatic
				{
					typedef smcu::io::types::StaticVPort<Pins> VPortType;
					
					static constexpr VPortType Make(const TPins... pins)
					{
						return VPortType();
					}
				};
				
				struct MakeDynamic
				{
					typedef smcu::io::types::DynamicVPort<Pins> VPortType;
					
					static constexpr VPortType Make(const TPins... pins)
					{
						return VPortType(pins...);
					}
				};
				
				typedef typename smcu::common::TL::Where<Pins, smcu::common::TL::Not<smcu::io::types::priv::IsStaticPinPredicate>::Result>::Result DynamicPins;
				static constexpr int LengthDynamic = smcu::common::TL::Length<DynamicPins>();
				typedef typename Loki::Select<LengthDynamic == 0, MakeStatic, MakeDynamic>::Result MakeType;
				typedef typename MakeType::VPortType VPortType;
				
				static constexpr VPortType Make(const TPins... pins)
				{
					return MakeType::Make(pins...);
				}
			};
		}
		
		template<class ...TPins>
		constexpr typename priv::MakeVPort<TPins...>::VPortType MakeVPort(const TPins... pins)
		{
			return priv::MakeVPort<TPins...>::Make(pins...);
		}
	}
}


#endif /* VPORT_H_ */