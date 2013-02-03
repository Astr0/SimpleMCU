/*
* TestSimpleMCU.cpp
*
* Created: 31.01.2013 20:52:28
*  Author: astr0
*/


#include <avr/io.h>
#include <smcu/io.h>

using namespace smcu::io;



template<class Pin>
struct TestPin
{
	private:
		const Pin _pin;
	public:
	
		constexpr TestPin(const Pin pin): _pin(pin)
		{

		}
		inline void TestSet() const
		{
			if (PORTC & 1)
				_pin.Set();
			else
				_pin.Clear();
		}
};

//static auto pb = MakeDynamicPin(&PortB, 0);

int main(void)
{
	constexpr auto tp = MakeInvertedPin(PinB0);
	tp.Set();
	PinC0Inv.Clear();

	/*constexpr TestPin<decltype(PinB0)> t(PinB0);
	t.TestSet();*/
	
	//constexpr smcu::io::priv::DynamicPin<decltype<PortB>> pb = MakePin(&PortB, 0);
	/*constexpr auto p = MakeVPort(&PinB0, &PinB1, &PinB2);
	p.Write(PORTC);*/
	
	//smcu::common::GetValueAt<Position, const PinType*, PINS...>::Get(pins...) 
	/*bool omg = smcu::io::priv::IsStaticPin<decltype(PinB0)>::value;
	if (omg)
		PORTB = 1;*/

	/*PORTC = 1;
	//PinB0.Set();
	constexpr auto Pins = MakeVPort(&PinB0, &PinB1, &PinC0);
	Pins.Write(123);
	Pins.Write(234);*/
	
	/*typedef VPort<priv::PinB1, priv::PinC0Inv, priv::PinC1Inv> MyPins;
	constexpr MyPins Pins;
	Pins.Write(123);
	PinB0::Set();
	//typedef PinList<priv::PinB0Inv, priv::PinB1Inv, priv::PinB2Inv> MyPins;
	//typedef PinList<priv::PinB0, priv::PinB1, priv::PinB2Inv> MyPins;
	PORTC = Pins.Read(); // ClearAndSet(4, 3*/
	//PORTB = Length<MyPins>();
	while(1)
    {
        //TODO:: Please write your application code 
    }
}