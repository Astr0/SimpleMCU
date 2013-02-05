/*
* TestSimpleMCU.cpp
*
* Created: 31.01.2013 20:52:28
*  Author: astr0
*/


#include <avr/io.h>
#include <smcu/io.h>

using namespace smcu::io;

template<int VPos, class TType>
struct Holder
{
	typedef TType Type;
};


static constexpr auto p = MakeVPort(PinB0, PortB.Pin(0));
static constexpr auto pin = p.Pin<1>();

static constexpr auto pp = MakePortPointer(&p);
//static constexpr auto ppin = pp.Pin<0>();


int main(void)
{
	//Test(char());
	//auto p = MakeVPort(NullPort.Pin(0));	
	auto p1 = p.Pin<1>();
	p1.Set();
	//pp.Set(7);
	/*int x = 2;
	decltype(*smcu::common::declval<int*>()) px = x;*/
	//auto p0 = p.Pin(0);
	//p0.Set();
	
	while(1)
    {
        //TODO:: Please write your application code 
    }
}