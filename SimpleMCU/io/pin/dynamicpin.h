/*
 * dynamicpin.h
 *
 * Created: 02.02.2013 17:32:23
 *  Author: astr0
 */ 

#pragma once

#ifndef DYNAMICPIN_H_
#define DYNAMICPIN_H_

#include "../../common/util.h"

namespace smcu
{
	namespace io
	{
		namespace priv
		{
			template<class PORT>
			class DynamicPin
			{
				private:
				struct PinHolder
				{
					constexpr PinHolder(const PORT* port, uint8_t number): 
						_mask(1 << number)
					{
					
					}
				
					const typename PORT::DataType _mask;
					inline static PORT* Port(){return nullptr;}
				};
				struct PinPortHolder: public PinHolder
				{
					constexpr PinPortHolder(const PORT* port, uint8_t number): 
						PinHolder(port, number),
						_port(port)
					{
					
					}
					const PORT* _port;
					inline PORT* Port()const{return _port;}
				};
				typedef typename smcu::common::StaticIf<PORT::IsStatic(), PinHolder, PinPortHolder>::Result PinPortHolderType;
				static_assert(PORT::IsStatic(), "not Static port!");
				const PinPortHolderType Holder;
				public:
				typedef PORT PortType;
			
				constexpr DynamicPin(const PORT* port, uint8_t number):
					Holder(port, number)
				{
					
				}
			
				inline constexpr PortType* Port() {return Holder.Port();}
				inline constexpr uint8_t Number(){return smcu::common::MaskToBit(Mask());}
				
				static constexpr bool IsStatic(){return false;}				
				static constexpr bool Inverted(){return false;}				
				
				inline constexpr typename PortType::DataType Mask() {return Holder._mask;}

				inline void Set()const
				{
					Port()->Set(Mask());
				}
			
				inline void Clear()const
				{
					Port()->Clear(Mask());
				}
			
				inline void Set(bool val)const
				{
					if (val)
						Set();
					else
						Clear();
				}

				inline void Toggle()const
				{
					Port()->Toggle(Mask());
				}

				inline bool Read()const
				{
					return (Port()->Read() && Mask());
				}
			
				inline void Refresh()const
				{
					Port() -> Refresh(Mask());				
				}
			
				inline void Update()const
				{
					Port() -> Update(Mask());
				}
			};	
			
		}
	}
}




#endif /* DYNAMICPIN_H_ */