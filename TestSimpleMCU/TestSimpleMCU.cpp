/*
* TestSimpleMCU.cpp
*
* Created: 10/4/2014 3:56:28 PM
*  Author: astr0
*/


#include <avr/io.h>
#include <smcu/io.h>

using namespace smcu::io;

int main(void)
{
	constexpr auto pin1 = PinB1::MakeInverted();
	constexpr auto pin2 = PinB2Inv();
	pin1.Set();
	while(1)
	{
		pin2.Set();
		//TODO:: Please write your application code
	}
}