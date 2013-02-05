/*
 * portbase.h
 *
 * Created: 03.02.2013 17:23:31
 *  Author: astr0
 */ 

#pragma once

#ifndef PORTBASE_H_
#define PORTBASE_H_

#include "../../common/maskutils.h"

namespace smcu
{
	namespace io
	{
		namespace types
		{
			template<class TDataType>
			class PortBase
			{
				private:
				typedef smcu::common::BitMaskTypes<TDataType> BitMaskTypes;
				protected:
				constexpr PortBase(){}
			
				public:
				typedef TDataType DataType; 
				typedef typename BitMaskTypes::OneBitType PinMaskType; 
				typedef typename BitMaskTypes::BitNumberType PinNumberType;
				typedef typename BitMaskTypes::MaskType MaskType; 
	
	

				/* Not Implemented:
				constexpr PinNumberType Width(){return sizeof(DataType) * 8;} 
				constexpr bool IsAutoUpdate(){return false;}
			
				void Update(MaskType mask)const{}
				void Refresh(MaskType mask)const{}

				typedef TPort PortType;

				constexpr smcu::io::types::Pin<PortType> Pin(PinNumberType number)
				{
					return smcu::io::types::Pin<PortType>(*this, number);
				}


				void Write(MaskType value)const {}
				void ClearAndSet(MaskType clearMask, MaskType setMask)const {}
				void Set(MaskType mask)const {}
				void Clear(MaskType mask)const {}
				void Toggle(MaskType mask)const {}
				DataType Read()const {return 0;}
				bool Read(PinMaskType pin)const{return false;}
					
				*/
			};
			
		}
	}
}



#endif /* PORTBASE_H_ */