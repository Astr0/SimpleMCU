/*
 * vport.h
 *
 * Created: 01.02.2013 18:03:47
 *  Author: astr0
 */ 


#ifndef STATICVPORT_H_
#define STATICVPORT_H_

#include "../../loki/Typelist.h"
#include "priv/staticpinlist.h"
#include "../../common/typelist.h"
#include "../../common/maskutils.h"
#include "../../common/typeutils.h"
#include "../updatecounter.h"
#include "../../common/bitholder.h"


namespace smcu
{
	namespace io
	{
		namespace types
		{
			// VPort - static port-like wrapper for Pins
			template<class TPinList>
			class StaticVPort
			{
				private:
					typedef TPinList Pins;
					typedef typename Loki::TL::NoDuplicates<typename priv::MakePortList<Pins>::Result>::Result Ports;
					static constexpr unsigned PinCount = smcu::common::TL::Length<Pins>();
					static constexpr unsigned ByteSize = smcu::common::MinSizeInBytes(PinCount);
					
					typedef smcu::common::BitHolder<smcu::common::MinSizeInBytes(PinCount)> DataType;
					//typedef typename smcu::common::BitMaskType<smcu::common::MinSizeInBytes(PinCount)>::Result DataType;
					typedef smcu::common::BitMaskTypes<DataType> BitMaskTypes;			
					typedef typename BitMaskTypes::OneBitMask PinMaskType;
					typedef typename BitMaskTypes::BitNumberType PinNumberType;
					typedef typename BitMaskTypes::MaskType MaskType;
				private:
					static constexpr bool AutoUpdated = smcu::common::TL::All<Ports, priv::PortAutoUpdatedPredicate>::value;
				public:
					constexpr StaticVPort(){}
			
					typedef StaticVPort<TPinList> PortType;
					
					static constexpr bool IsStatic(){return true;}
	
					static constexpr PinNumberType Width(){return PinCount;}
					static constexpr bool IsAutoUpdate(){return AutoUpdated;}
								
					static void Write(DataType value) 
					{
						priv::PortListIterator<Ports, Pins, DataType>::Write(value);
					}
					
					static void ClearAndSet(MaskType clearMask, MaskType setMask) 
					{
						priv::PortListIterator<Ports, Pins, DataType>::ClearAndSet(clearMask, setMask);
					}				
					static void Set(MaskType mask) 
					{
						priv::PortListIterator<Ports, Pins, DataType>::Set(mask);
					}
					static void Clear(MaskType mask) 
					{
						priv::PortListIterator<Ports, Pins, DataType>::Clear(mask);					
					}
					static void Toggle(MaskType mask) 
					{
						priv::PortListIterator<Ports, Pins, DataType>::Toggle(mask);	
					}
					static DataType Read() 
					{
						return priv::PortListIterator<Ports, Pins, DataType>::Read();
					}
					static bool Read(PinMaskType pin)
					{
						// TODO: Optimize this
						return Read() & pin;
					}
					
					static void Update(MaskType mask)
					{
						if (AutoUpdated)
							return;
						smcu::io::UpdateCounter.Increment();
						DoUpdate(mask);
					}
					static void Refresh(MaskType mask)
					{
						if (AutoUpdated)
							return;
						smcu::io::UpdateCounter.Increment();
						DoRefresh(mask);
					}		
				
				
					static void DoUpdate(MaskType mask)
					{
						if (AutoUpdated)
							return;
						priv::PortListIterator<Ports, Pins, DataType>::Update(mask);
					}					

					static void DoRefresh(MaskType mask)
					{
						if (AutoUpdated)
							return;
						priv::PortListIterator<Ports, Pins, DataType>::Refresh(mask);
					}
					
					template<PinNumberType VPin>
					static constexpr typename Loki::TL::TypeAt<Pins, VPin>::Result::Pin Pin(){return typename Loki::TL::TypeAt<Pins, VPin>::Result::Pin();}
						
					static constexpr DynamicNumberPin<PortType> Pin(PinNumberType number){return DynamicNumberPin<PortType>(number);}						
			};
		}
	}
}



#endif /* STATICVPORT_H_ */