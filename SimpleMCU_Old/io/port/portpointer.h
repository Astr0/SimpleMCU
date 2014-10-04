/*
 * portpointer.h
 *
 * Created: 05.02.2013 1:18:04
 *  Author: astr0
 */ 

#ifndef PORTPOINTER_H_
#define PORTPOINTER_H_

#include "../../common/typeutils.h"
#include "portinfo.h"

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
				PPort _port;
				static_assert(!TPort::IsStatic(), "Don't use for static type instances - copy are almost free...");
				
				typedef typename PortInfo<TPort>::DataType DataType;
				typedef typename PortInfo<TPort>::PinMaskType PinMaskType;
				typedef typename PortInfo<TPort>::PinNumberType PinNumberType;
				typedef typename PortInfo<TPort>::MaskType MaskType;

				public:
				
				PortPointer(PPort port): _port(port)
				{					
				}
					
				typedef PortPointer<PPort> PortType;
				
				static constexpr bool IsStatic(){return false;}
				
				inline constexpr PinNumberType Width(){return _port->Width();}
				inline constexpr bool IsAutoUpdate(){return _port->IsAutoUpdate();}
				
				inline void Write(const DataType value)const {_port->Write(value);}
				inline void ClearAndSet(MaskType clearMask, MaskType setMask)const {_port->ClearAndSet(clearMask, setMask);}
				inline void Set(MaskType mask)const {_port->Set(mask);}
				inline void Clear(MaskType mask)const {_port->Clear(mask);}
				inline void Toggle(MaskType mask)const {_port->Toggle(mask);}
				inline DataType Read()const {return _port->Read();}
				inline bool Read(PinMaskType pin)const{return _port->Read(pin);}
					
				inline void Update(MaskType mask)const{_port->Update(mask);}
				inline void Refresh(MaskType mask)const{_port->Refresh(mask);}
					
				template<PinNumberType VNumber>
				inline constexpr decltype(smcu::common::declval<TPort>().template Pin<VNumber>()) Pin()
				{
					return _port->template Pin<VNumber>();
				}
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
					static constexpr Result Make(PPort port)
					{
						return Result();						
					}
				};				
				struct MakeDynamic
				{
					typedef smcu::io::types::PortPointer<PPort> Result;					
					static Result Make(PPort port)
					{
						return Result(port);
					}
				};
				
				typedef typename Loki::Select<IsStatic, MakeStatic, MakeDynamic>::Result MakeType;
			};		
		}		
		template<class PPort>
		constexpr typename priv::MakePortPointer<PPort>::MakeType::Result MakePortPointer(PPort port)
		{
			return priv::MakePortPointer<PPort>::MakeType::Make(port);
		}
	}	
}


#endif /* PORTPOINTER_H_ */