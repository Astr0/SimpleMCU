/*
 * bitholder.h
 *
 * Created: 06.02.2013 0:38:31
 *  Author: astr0
 */ 


#ifndef BITHOLDER_H_
#define BITHOLDER_H_

#include "maskutils.h"

namespace smcu
{
	namespace common
	{
		template<class TNumber>
		class NotBitNumber;

		template<class TNumber>
		class BitNumber
		{	
			private:
				const TNumber _number;
			public:
				constexpr BitNumber():_number()
				{
			
				}
				constexpr BitNumber(TNumber number):_number(number)
				{
				}
		
				constexpr TNumber Number(){return _number;}
		
				template<class T>
				constexpr BitNumber<TNumber> operator<<(T val){return BitNumber<TNumber>(Number() + val);}
				template<class T>
				constexpr BitNumber<TNumber> operator>>(T val){return BitNumber<TNumber>(Number() - val);}
		
				template<class T>
				BitNumber<TNumber>& operator<<=(T val){_number += val; return *this;}
				template<class T>
				BitNumber<TNumber>& operator>>=(T val){_number -= val; return *this;}
		
				NotBitNumber<TNumber> operator~(){ return NotBitNumber<TNumber>(_number);}
		};

		template<class TNumber>
		class NotBitNumber: public BitNumber<TNumber>
		{
			private:
				const TNumber _number;
			public:
				constexpr NotBitNumber():_number()
				{
			
				}
				constexpr NotBitNumber(TNumber number):_number(number)
				{
				}
		
				constexpr TNumber Number(){return _number;}
		
				template<class T>
				constexpr NotBitNumber<TNumber> operator<<(T val){return NotBitNumber<TNumber>(Number() + val);}
				template<class T>
				constexpr NotBitNumber<TNumber> operator>>(T val){return NotBitNumber<TNumber>(Number() - val);}
		
				template<class T>
				NotBitNumber<TNumber>& operator<<=(T val){_number += val; return *this;}
				template<class T>
				NotBitNumber<TNumber>& operator>>=(T val){_number -= val; return *this;}
		
				BitNumber<TNumber> operator~(){ return BitNumber<TNumber>(_number);}
		};	

		template<unsigned N>
		class BitHolder
		{
			private:
				static constexpr unsigned MaxBitIndex = N * 8;
				static constexpr unsigned ByteSize = N;
			public:
				uint8_t _buf[ByteSize];
		
				typedef typename smcu::common::NumberType<N>::Result ByteNumberType;
			
				constexpr uint8_t operator[](ByteNumberType number){return _buf[number];}
				uint8_t& operator[](ByteNumberType number){return _buf[number];}
		
			public:
				typedef typename smcu::common::NumberType<MaxBitIndex>::Result BitNumberType;
				typedef BitHolder<N> MaskType;
				typedef BitNumber<BitNumberType> BitMaskType;


				template<class Num>
				inline BitHolder<N>& operator|=(BitNumber<Num> num)
				{
					_buf[num.Number() / 8] |= 1 << (num.Number() % 8);
					return *this;
				}

				template<class Num>
				inline BitHolder<N>& operator^=(BitNumber<Num> num)
				{
					_buf[num.Number() / 8] ^= 1 << (num.Number() % 8);
					return *this;
				}

				template<class Num>
				inline BitHolder<N>& operator&=(NotBitNumber<Num> num)
				{
			
					_buf[num.Number() / 8] &= ~(1 << (num.Number() % 8));
					return *this;
				}
		
				template<class Num>
				constexpr inline bool operator&(BitNumber<Num> num)const
				{
					return _buf[num.Number() / 8] & (1 << (num.Number() % 8));
				}
				
			private:
				bool IsTruthy()const
				{
					for (ByteNumberType i = 0; i < N; ++i)
						if (_buf[i])
							return true;
					return false;					
				}
			public:
				
				constexpr operator bool() const
				{
					return IsTruthy();
				}
				constexpr bool operator !()const
				{
					return !IsTruthy();
				}				
		};
		
		template<unsigned bytes>
		struct BitMaskType
		{
			typedef BitHolder<bytes> Result;
		};
				


		template<unsigned N>
		struct BitMaskTypes<BitHolder<N>>
		{
			typedef typename BitHolder<N>::BitMaskType OneBitMask;
			typedef OneBitMask MaskType;
			typedef typename BitHolder<N>::BitNumberType BitNumberType;
			static constexpr bool OnlyBitInterface = true;
			static constexpr OneBitMask BitToMask(BitNumberType num){return OneBitMask() << num; }
			static constexpr BitNumberType MaskToBit(OneBitMask value)
			{
				return value.Number();
			}					
		};
	}
}



#endif /* BITHOLDER_H_ */