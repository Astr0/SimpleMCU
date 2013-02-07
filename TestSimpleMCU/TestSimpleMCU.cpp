/*
* TestSimpleMCU.cpp
*
* Created: 31.01.2013 20:52:28
*  Author: astr0
*/


#include <avr/io.h>
#include <smcu/io.h>
#include "common/bitholder.h"


using namespace smcu::io;

template<class TDataType>
class TestPort
{
	private:
		typedef smcu::common::BitMaskTypes<TDataType> BitMaskTypes;
					
		static TDataType _data;
			
	public:
		typedef TDataType DataType;
		typedef typename BitMaskTypes::OneBitMask PinMaskType;
		typedef typename BitMaskTypes::BitNumberType PinNumberType;
		typedef typename BitMaskTypes::MaskType MaskType;			

		constexpr TestPort(){}
				
		typedef TestPort<TDataType> PortType;
								
		static constexpr bool IsStatic(){return true;}
					
		static constexpr PinNumberType Width(){return 8 * sizeof(DataType);}			
		static constexpr bool IsAutoUpdate(){return false;}
			
		inline static void Write(DataType value) {_data = value;}
						
		inline static void ClearAndSet(MaskType clearMask, MaskType setMask) 
		{
			Clear(clearMask);
			Set(setMask);
		}
		inline static void Set(MaskType mask) {_data |= mask;}
		inline static void Clear(MaskType mask) {_data &= ~mask;}
		inline static void Toggle(MaskType mask) {_data ^= mask;}
		inline static DataType Read() {return _data;}
		inline static bool Read(PinMaskType mask){return _data & mask;}
					
		inline static void Update(MaskType mask)
		{
			uint8_t* d = (uint8_t*)(&_data);
			for (PinNumberType i = 0; i < sizeof(DataType); ++i)
			{
				PORTB = d[i];
			}
		}
		inline static void Refresh(MaskType mask){}
					
		template<PinNumberType VNumber>
		static constexpr smcu::io::types::StaticPin<PortType, VNumber> Pin()
		{
			return smcu::io::types::StaticPin<PortType, VNumber>();
		}
};	
template<class TDataType>
TDataType TestPort<TDataType>::_data;

typedef TestPort<smcu::common::BitHolder<5>> TPort;
static constexpr TPort p;
static constexpr auto p0 = MakePin(p, 0);
static constexpr auto vp = MakeVPort(PinB0
/*p.Pin<0>(), 
p.Pin<1>(),
p.Pin<2>(),
p.Pin<3>(),
p.Pin<4>(),
p.Pin<5>(),
p.Pin<6>(),
p.Pin<7>(),
p.Pin<8>(),
p.Pin<9>(),
p.Pin<10>(),
p.Pin<11>(),
p.Pin<12>(),
p.Pin<13>(),
p.Pin<14>(),
p.Pin<15>(),
p.Pin<16>(),
p.Pin<17>(),
p.Pin<18>(),
p.Pin<19>(),
p.Pin<20>(),
p.Pin<21>(),
p.Pin<22>(),
p.Pin<23>(),
p.Pin<24>(),
p.Pin<25>(),
p.Pin<26>(),
p.Pin<27>(),
p.Pin<28>(),
p.Pin<29>(),
p.Pin<30>(),
p.Pin<31>(),
p.Pin<32>(),
p.Pin<33>(),
p.Pin<34>(),
p.Pin<35>(),
p.Pin<36>(),
p.Pin<37>()*/
);

int main(void)
{
	if (!vp.Read())
		PORTB = 1;
	//p.Update(0);
	//PortB.Set(3);
	
	while(1)
    {
        //TODO:: Please write your application code 
    }
}