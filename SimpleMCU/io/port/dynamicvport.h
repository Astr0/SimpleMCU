/*
 * dynamicvport.h
 *
 * Created: 02.02.2013 15:21:51
 *  Author: astr0
 */ 

#pragma once

#ifndef DYNAMICVPORT_H_
#define DYNAMICVPORT_H_

#include "vport.h"
#include "../impl/dynamicpinlist.h"

namespace smcu
{
	namespace io
	{
		namespace priv
		{
			template <class TPinList>
			class DynamicVPort
			{
				private:
					typedef TPinList Pins;	
					typedef typename smcu::io::priv::MakeStaticPinList<Pins>::Result StaticPins;
					typedef typename smcu::io::priv::VPort<StaticPins> StaticPort;
				public:
					typedef typename StaticPort::DataType DataType;
				private:			
					typedef typename smcu::io::priv::SelectPins<Pins, smcu::io::priv::IsDynamicPinPredicate>::Result DynamicPins;
					typedef typename smcu::io::priv::DynamicPinWrapper<DataType, DynamicPins> DynamicHeadType;
					//static_assert(Loki::TL::Length<DynamicPins>::value == 0, "Omg Dynamic pin!");
				
					const DynamicHeadType _dynamicPortHead;
				public:
					template<class ...PINS>
					constexpr DynamicVPort(const PINS... pins): _dynamicPortHead(pins...)
					{					
					}	
				
					static constexpr uint8_t Width(){return StaticPort::Width();}
					static constexpr bool IsStatic(){return false;}			
					static constexpr NullPortController* Controller(){return nullptr;}
		
					inline void Write(DataType value) const
					{
						StaticPort::Write(value);
						_dynamicPortHead.Write(value);
					}
					inline void ClearAndSet(DataType clearMask, DataType setMask)const 
					{
						StaticPort::ClearAndSet(clearMask, setMask);
						_dynamicPortHead.ClearAndSet(clearMask, setMask);
					}
					inline void Set(DataType mask)const 
					{
						StaticPort::Set(mask);
						_dynamicPortHead.Set(mask);
					}
					inline void Clear(DataType mask)const
					{
						StaticPort::Clear(mask);
						_dynamicPortHead.Clear(mask);
					}
					inline void Toggle(DataType mask) const
					{
						StaticPort::Toggle(mask);
						_dynamicPortHead.Toggle(mask);
					}
					inline DataType Read()const 
					{
						return StaticPort::Read() | _dynamicPortHead.Read();
					}

					void Update(DataType mask) const
					{
						if (!StaticPort::HasControllers() && !DynamicHeadType::HasControllers())
							return;
						smcu::io::UpdateCounter::Increment();
					
						StaticPort::DoUpdate(mask);
						_dynamicPortHead.DoUpdate(mask);
					}
					void Refresh(DataType mask) const
					{
						if (!StaticPort::HasControllers() && !DynamicHeadType::HasControllers())
							return;
						smcu::io::UpdateCounter::Increment();
					
						StaticPort::DoRefresh(mask);
						_dynamicPortHead.DoRefresh(mask);
					}
					
					template<uint8_t PIN>
					static typename Loki::TL::TypeAt<Pins, PIN>::Result* Pin(){return nullptr;}
			};		
		}
	}	
}

#endif /* DYNAMICVPORT_H_ */