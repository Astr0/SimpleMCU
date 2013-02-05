/*
 * vport.h
 *
 * Created: 01.02.2013 18:03:47
 *  Author: astr0
 */ 


#ifndef VPORT_H_
#define VPORT_H_

#include "../../loki/Typelist.h"
#include "priv/pinlist.h"
#include "../../common/typelist.h"
#include "../../common/maskutils.h"
#include "../../common/typeutils.h"
#include "../updatecounter.h"


namespace smcu
{
	namespace io
	{
		namespace types
		{
			// VPort - static port-like wrapper for Pins
			template<class TPinList>
			class VPort
			{
				private:
					typedef TPinList Pins;
					typedef typename Loki::TL::NoDuplicates<typename priv::MakePortList<Pins>::Result>::Result Ports;
					static constexpr int PinCount = smcu::common::TL::Length<Pins>();
					
				public:
					typedef typename smcu::common::FastDataType<smcu::common::MinSizeInBytes(PinCount)>::Result DataType;
				private:
					typedef smcu::common::BitMaskTypes<DataType> BitMaskTypes;			
				public:
					typedef typename BitMaskTypes::OneBitType PinMaskType; 
					typedef typename BitMaskTypes::BitNumberType PinNumberType;
					typedef typename BitMaskTypes::MaskType MaskType;					
				private:
					static constexpr MaskType Mask = smcu::common::SetBits<MaskType>(PinCount);
					static constexpr bool AutoUpdated = smcu::common::TL::All<Ports, priv::PortAutoUpdatedPredicate>::value;
				public:
					constexpr VPort(){}
			
					typedef VPort<TPinList> PortType;
					
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
						if (mask & Mask == Mask)
							priv::PortListIterator<Ports, Pins, DataType>::UpdateAll();
						else
							priv::PortListIterator<Ports, Pins, DataType>::UpdateAffected(mask);
					}					

					static void DoRefresh(MaskType mask)
					{
						if (AutoUpdated)
							return;
						if (mask & Mask == Mask)
							priv::PortListIterator<Ports, Pins, DataType>::RefreshAll();
						else
							priv::PortListIterator<Ports, Pins, DataType>::RefreshAffected(mask);
					}
					
					template<PinNumberType VPin>
					static constexpr typename Loki::TL::TypeAt<Pins, VPin>::Result::Pin Pin(){return typename Loki::TL::TypeAt<Pins, VPin>::Result::Pin();}
						
					static constexpr DynamicNumberPin<PortType> Pin(PinNumberType number){return DynamicNumberPin<PortType>(number);}						
			};
		}
	}
}



#endif /* VPORT_H_ */