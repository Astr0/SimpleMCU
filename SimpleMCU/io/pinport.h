/*
 * pinport.h
 * TODO: Dynamic MaskType and PinNumber type
 * Created: 10/4/2014 6:34:32 PM
 *  Author: astr0
 */ 


#ifndef PINPORT_H_
#define PINPORT_H_

#include "../common/utils.h"

namespace smcu{
	namespace io{
		namespace types{
			namespace priv{
				template<class TPin>
				struct SelectPinPort
				{
					typedef typename TPin::PortType Result;
				};
				
				template<class TPort>
				struct PortAutoUpdates
				{
					static constexpr bool value = TPort::AutoUpdates();
				};
				
				template<class TPort>
				struct PortWritable
				{
					static constexpr bool value = TPort::Writable();
				};
				
				template<class TPort>
				struct PortReadable
				{
					static constexpr bool value = TPort::Readable();
				};
			}
			
			template<class TPinList>
			class PinPort
			{
				private:				
				
				// Pin ports in list
				typedef typename Loki::TL::Select<TPinList, priv::SelectPinPort>::Result _PinPorts;
				// distinct ports
				typedef typename Loki::TL::NoDuplicates<_PinPorts> Ports;
				
				static constexpr unsigned PinCount = Loki::TL::GetLength<TPinList>();
				static constexpr unsigned PinCountBytes = smcu::common::GetByteSize(PinCount);
												
				static constexpr bool _autoUpdates = Loki::TL::GetAny<Ports, priv::PortAutoUpdates>();
				static constexpr bool _readable = Loki::TL::GetAll<Ports, priv::PortReadable>();
				static constexpr bool _writable = Loki::TL::GetAll<Ports, priv::PortWritable>();
				//typedef typename smcu::common::BitMaskType<smcu::common::MinSizeInBytes(PinCount)>::Result DataType;
				/*typedef smcu::common::BitMaskTypes<DataType> BitMaskTypes;
				typedef typename BitMaskTypes::OneBitMask PinMaskType;
				typedef typename BitMaskTypes::BitNumberType PinNumberType;
				typedef typename BitMaskTypes::MaskType MaskType;*/
				public:
				typedef typename smcu::common::DataType<PinCountBytes>::Result DataType;
				typedef typename smcu::common::MaskType<PinCountBytes>::Result MaskType;
				typedef typename smcu::common::NumberType<PinCount>::Result PinNumberType;
				
				private:
				static constexpr PinNumberType _width = PinNumberType(PinCount);
				
				public:				
				static constexpr PinNumberType Width(){return _width;}
				static constexpr bool AutoUpdates(){return _autoUpdates;}
				static constexpr bool Readable(){return _readable;}
				static constexpr bool Writable(){return _writable;}
				/*
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
				
				static constexpr DynamicNumberPin<PortType> Pin(PinNumberType number){return DynamicNumberPin<PortType>(number);}*/
			};
		}
	}
}


#endif /* PINPORT_H_ */