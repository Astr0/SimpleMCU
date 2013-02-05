/*
 * portpointer.h
 *
 * Created: 05.02.2013 1:18:04
 *  Author: astr0
 */ 

#ifndef PORTPOINTER_H_
#define PORTPOINTER_H_

#include "../../common/typeutils.h"

namespace smcu
{
	namespace io
	{
		namespace types
		{
			template<class TPortPointer>
			class PortPointer
			{
				typedef TPortPointer PPort;
				typedef decltype(*smcu::common::declval<PPort>()) RPort; // port reference
				typedef typename smcu::common::remove_reference<RPort>::type TPort; // port type itself, too bad we don't have std here :(
				const PPort _port;
				static_assert(!TPort::IsStatic(), "Don't use for static type instances - copy are almost free...");
				public:
				typedef typename TPort::DataType DataType;
				typedef typename TPort::PinMaskType PinMaskType;
				typedef typename TPort::PinNumberType PinNumberType;
				typedef typename TPort::MaskType MaskType;
				
				constexpr PortPointer(const PPort port): _port(port)
				{					
				}
					
				typedef PortPointer<PPort> PortType;
				
				static constexpr bool IsStatic(){return false;}
				
				constexpr PinNumberType Width(){return _port->Width();}
				constexpr bool IsAutoUpdate(){return _port->IsAutoUpdate();}
				
				void Write(DataType value)const {_port->Write(value);}
				void ClearAndSet(MaskType clearMask, MaskType setMask)const {_port->ClearAndSet(clearMask, setMask);}
				void Set(MaskType mask)const {_port->Set(mask);}
				void Clear(MaskType mask)const {_port->Clear(mask);}
				void Toggle(MaskType mask)const {_port->Toggle(mask);}
				constexpr DataType Read()const {return _port->Read();}
				constexpr bool Read(PinMaskType pin)const{return _port->Read(pin);}
					
				void Update(MaskType mask)const{_port->Update(mask);}
				void Refresh(MaskType mask)const{_port->Refresh(mask);}
					
				template<PinNumberType VNumber>
				constexpr decltype(smcu::common::declval<TPort>().template Pin<VNumber>()) Pin()
				{
					return _port->template Pin<VNumber>();
				}
					
				// TODO
				/*constexpr DynamicNumberPin<PortType> Pin(PinNumberType number)const{return DynamicNumberPin<PortType>(number);}	*/
			};
		}
		// TODO: MakePortPointer(Port)
		namespace priv
		{
			template<class PPort>
			struct MakePortPointer
			{
				typedef decltype(*smcu::common::declval<PPort>()) RPort; // port reference
				typedef typename smcu::common::remove_reference<RPort>::type TPort; // port type itself, too bad we don't have std here :(
				static constexpr bool IsStatic = TPort::IsStatic();

				struct MakeStatic
				{
					typedef TPort Result;
					static constexpr Result Make(const PPort port)
					{
						return Result();						
					}
				};				
				struct MakeDynamic
				{
					typedef smcu::io::types::PortPointer<PPort> Result;					
					static constexpr Result Make(const PPort port)
					{
						return Result(port);
					}
				};
				
				typedef typename Loki::Select<IsStatic, MakeStatic, MakeDynamic>::Result MakeType;
			};		
		}		
		template<class PPort>
		constexpr typename priv::MakePortPointer<PPort>::MakeType::Result MakePortPointer(const PPort port)
		{
			return priv::MakePortPointer<PPort>::MakeType::Make(port);
		}
	}	
}


#endif /* PORTPOINTER_H_ */