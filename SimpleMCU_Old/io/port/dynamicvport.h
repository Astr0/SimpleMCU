/*
 * dynamicvport.h
 *
 * Created: 02.02.2013 15:21:51
 *  Author: astr0
 */ 

#pragma once

#ifndef DYNAMICVPORT_H_
#define DYNAMICVPORT_H_

#include "staticvport.h"
#include "priv/dynamicpinlist.h"
#include "../updatecounter.h"
#include "portinfo.h"
//#include "portpointer.h"

namespace smcu
{
	namespace io
	{
		namespace types
		{
			template <class TPinList>
			class DynamicVPort
			{
					typedef TPinList Pins;	
					typedef typename smcu::common::TL::Select<Pins, priv::SelectStaticPinList>::Result StaticPins;
					typedef StaticVPort<StaticPins> StaticPort;
				private:			
					typedef typename PortInfo<StaticPort>::DataType DataType;
					typedef typename PortInfo<StaticPort>::PinMaskType PinMaskType;
					typedef typename PortInfo<StaticPort>::PinNumberType PinNumberType;
					typedef typename PortInfo<StaticPort>::MaskType MaskType;

					typedef typename smcu::common::TL::Where<Pins, smcu::common::TL::Not<priv::IsStaticPinPredicate>::Result>::Result DynamicPins;
					typedef priv::DynamicPinWrapper<DataType, DynamicPins> DynamicHeadType;
					static_assert(Loki::TL::Length<DynamicPins>::value > 0, "Use static VPort for performance!");
				
					const DynamicHeadType _dynamicPortHead;
					const bool _autoUpdated;
				protected:
				public:
					template<class ...PINS>
					constexpr DynamicVPort(const PINS... pins): _dynamicPortHead(pins...), _autoUpdated(priv::IsDynamicAutoUpdate(pins...))
					{					
					}						
					
					static constexpr bool IsStatic(){return false;}
					static constexpr PinNumberType Width(){return StaticPort::Width();}
					constexpr bool IsAutoUpdate(){return StaticPort::IsAutoUpdate() && _autoUpdated;}
		
					inline void Write(MaskType value) const
					{
						StaticPort::Write(value);
						_dynamicPortHead.Write(value);
					}
					inline void ClearAndSet(MaskType clearMask, MaskType setMask)const 
					{
						StaticPort::ClearAndSet(clearMask, setMask);
						_dynamicPortHead.ClearAndSet(clearMask, setMask);
					}
					inline void Set(MaskType mask)const 
					{
						StaticPort::Set(mask);
						_dynamicPortHead.Set(mask);
					}
					inline void Clear(MaskType mask)const
					{
						StaticPort::Clear(mask);
						_dynamicPortHead.Clear(mask);
					}
					inline void Toggle(MaskType mask) const
					{
						StaticPort::Toggle(mask);
						_dynamicPortHead.Toggle(mask);
					}
					inline DataType Read()const 
					{
						DataType result = StaticPort::Read();
						_dynamicPortHead.Read(result);
						return result;
					}
					inline bool Read(PinMaskType pin) const
					{
						// TODO: Optimize this
						return Read() & pin;
					}

					void Update(MaskType mask) const
					{
						if (StaticPort::IsAutoUpdate() && _autoUpdated)
							return;
						smcu::io::UpdateCounter.Increment();
					
						StaticPort::DoUpdate(mask);
						if (!_autoUpdated)
							_dynamicPortHead.DoUpdate(mask);
					}
					void Refresh(MaskType mask) const
					{
						if (StaticPort::IsAutoUpdate() && _autoUpdated)
							return;
						smcu::io::UpdateCounter.Increment();
					
						StaticPort::DoRefresh(mask);
						if (!_autoUpdated)
							_dynamicPortHead.DoRefresh(mask);
					}
					
					private:
					template<PinNumberType VPin>
					struct PinType
					{
						typedef typename Loki::TL::TypeAt<Pins, VPin>::Result::Pin Result;
						static constexpr bool IsStatic = Result::IsStatic();
						
						typedef typename Loki::Select<IsStatic, bool, char>::Result OverloadType;
						static constexpr OverloadType Overload = OverloadType();
					};
					
					// static 
					template<PinNumberType VPin>
					static constexpr typename PinType<VPin>::Result MakePin(bool)
					{
						return typename PinType<VPin>::Result();
					}				
					
					// dynamic
					template<PinNumberType VPin>
					constexpr typename PinType<VPin>::Result MakePin(char) const
					{
						return _dynamicPortHead.template Pin<VPin>();
					}							
								
					public:
					template<PinNumberType VPin>
					constexpr typename PinType<VPin>::Result Pin()
					{
						return MakePin<VPin>( PinType<VPin>::Overload);
					}
			};		
		}
	}	
}

#endif /* DYNAMICVPORT_H_ */