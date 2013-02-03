/*
 * UpdateCounter.h
 *
 * Created: 02.02.2013 23:39:38
 *  Author: astr0
 */ 

#pragma once

#ifndef UPDATECOUNTER_H_
#define UPDATECOUNTER_H_

#ifndef SMCU_UPDATECOUNTER
#define SMCU_UPDATECOUNTER uint8_t
#endif

namespace smcu
{
	namespace io
	{
		namespace priv
		{
			template<class UPDATECOUNTER>
			class UpdateCheck;

			
			template<class COUNTER>
			class UpdateCounter
			{
				private:
				typedef COUNTER CounterType;
				static CounterType _counter;
				static CounterType Counter(){return _counter;}
				friend class UpdateCheck<UpdateCounter<COUNTER>>;
				public:
				static void Increment()
				{
					_counter++;
					if (_counter == 0)
						_counter = 1;
				}
			};
			template<class COUNTER>
			COUNTER UpdateCounter<COUNTER>::_counter = 0;
			
			template<>
			class UpdateCounter<void>
			{
				public:
				static void Increment()
				{
				}
			};
			
			template<class UPDATECOUNTER>
			class UpdateCheck
			{
				private:
				typedef UPDATECOUNTER UpdateCounter;
				typename UpdateCounter::CounterType _counter;
				public:
				bool Validate()
				{
					if (_counter == UpdateCounter::Counter())
						return true;
					_counter = UpdateCounter::Counter();
					return false;
				}
			};
			template<>
			class UpdateCheck<void>
			{
				public:
				static constexpr bool Validate()
				{
					return true;
				}
			};
		}
		typedef smcu::io::priv::UpdateCounter<SMCU_UPDATECOUNTER> UpdateCounter;
		typedef smcu::io::priv::UpdateCheck<UpdateCounter> UpdateCheck;
	}
}

#endif /* UPDATECOUNTER_H_ */