/*
 * vport.h
 *
 * Created: 01.02.2013 18:03:47
 *  Author: astr0
 */ 


#ifndef VPORT_H_
#define VPORT_H_

#include "port.h"
#include "../../loki/Typelist.h"
#include "../../common/util.h"
#include "../impl/pinlist.h"
#include "../impl/updatecounter.h"

namespace smcu
{
	namespace io
	{
		namespace priv
		{
			// VPort - port-like wrapper for Pins
			template<class TPinList>
			class VPort
			{
				private:
					typedef TPinList Pins;
					typedef typename Loki::TL::NoDuplicates<typename smcu::io::priv::MakePortList<Pins>::Result>::Result Ports;
					typedef typename Loki::TL::NoDuplicates<typename smcu::io::priv::MakeControllerList<Ports>::Result>::Result Controllers;
					static constexpr uint8_t PinCount = smcu::common::TLS::Length<Pins>();
					static constexpr uint8_t ControllerCount = smcu::common::TLS::Length<Controllers>();
				public:
					typedef typename smcu::common::FastDataType<smcu::common::MinSizeInBytes(PinCount)>::Result DataType;
					typedef NullPortController ControllerType;
				private:
					static constexpr DataType Mask = smcu::common::Pow<DataType>(2, PinCount) - 1;
				public:
					constexpr VPort(){}
			
					static constexpr uint8_t Width(){return PinCount;}
					static constexpr bool IsStatic(){return true;}
								
					static constexpr NullPortController* Controller(){return nullptr;}
							
					static void Write(DataType value) 
					{
						smcu::io::priv::PortListIterator<Ports, Pins, DataType>::Write(value);
						smcu::io::priv::PortControllerListIterator<Controllers, Pins, DataType>::UpdateAll();
					}
					static void ClearAndSet(DataType clearMask, DataType setMask) 
					{
						smcu::io::priv::PortListIterator<Ports, Pins, DataType>::ClearAndSet(clearMask, setMask);
						smcu::io::priv::PortControllerListIterator<Controllers, Pins, DataType>::UpdateAffected(clearMask | setMask);
					}				
					static void Set(DataType mask) 
					{
						smcu::io::priv::PortListIterator<Ports, Pins, DataType>::Set(mask);
						smcu::io::priv::PortControllerListIterator<Controllers, Pins, DataType>::UpdateAffected(mask);
					}
					static void Clear(DataType mask) 
					{
						smcu::io::priv::PortListIterator<Ports, Pins, DataType>::Clear(mask);					
						smcu::io::priv::PortControllerListIterator<Controllers, Pins, DataType>::UpdateAffected(mask);
					}
					static void Toggle(DataType mask) 
					{
						smcu::io::priv::PortListIterator<Ports, Pins, DataType>::Toggle(mask);	
						smcu::io::priv::PortControllerListIterator<Controllers, Pins, DataType>::UpdateAffected(mask);
					}
					static DataType Read() 
					{
						smcu::io::priv::PortControllerListIterator<Controllers, Pins, DataType>::RefreshAll();
						return smcu::io::priv::PortListIterator<Ports, Pins, DataType>::Read();
					}
				
					static constexpr bool HasControllers(){return ControllerCount != 0;}
					
					static void Update(DataType mask)
					{
						if (!HasControllers())
							return;
						smcu::io::UpdateCounter::Increment();
						DoUpdate(mask);
					}
					static void Refresh(DataType mask)
					{
						if (!HasControllers())
							return;
						smcu::io::UpdateCounter::Increment();
						DoRefresh(mask);
					}		
				
				
					static void DoUpdate(DataType mask)
					{
						if (!HasControllers())
							return;
						if (mask & Mask == Mask)
							smcu::io::priv::PortControllerListIterator<Controllers, Pins, DataType>::UpdateAll();
						else
							smcu::io::priv::PortControllerListIterator<Controllers, Pins, DataType>::UpdateAffected(mask);
					}					

					static void DoRefresh(DataType mask)
					{
						if (!HasControllers())
							return;
						if (mask & Mask == Mask)
							smcu::io::priv::PortControllerListIterator<Controllers, Pins, DataType>::RefreshAll();
						else
							smcu::io::priv::PortControllerListIterator<Controllers, Pins, DataType>::RefreshAffected(mask);
					}
					
					template<uint8_t PIN>
					static typename Loki::TL::TypeAt<Pins, PIN>::Result* Pin(){return nullptr;}
			};
		}
	}
}



#endif /* VPORT_H_ */