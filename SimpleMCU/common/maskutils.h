/*
 * maskutils.h
 *
 * Created: 03.02.2013 20:58:09
 *  Author: astr0
 */ 
#pragma once

#ifndef MASKUTILS_H_
#define MASKUTILS_H_

namespace smcu
{
	namespace common
	{
		template<class NumberType, class MaskType>
		constexpr NumberType MaskToBit(MaskType value)
		{
			return (value & 1) ? 0 : (1 + MaskToBit(value >> 1));
		}

		constexpr int MinSizeInBytes(int sizeInBits)
		{
			return sizeInBits <= 8 ? 1 : (1 + MinSizeInBytes(sizeInBits - 8));
		}
		
		template<class MaskType>
		constexpr MaskType SetBits(int count)
		{
			return count == 0 ? 0 : ((1 << count) | SetBits<MaskType>(count - 1));
		}
		
		template<class DataType>
		struct BitMaskTypes
		{
			typedef DataType OneBitType;
			typedef DataType MaskType;
			typedef uint_fast8_t BitNumberType;
		};
		
		template<int VBytesToLeft>
		struct Shifter
		{
			template<class T>
			static constexpr T Shift(T value)
			{
				return VBytesToLeft == 0 ? value :
				VBytesToLeft > 0 ? value << VBytesToLeft : value >> -VBytesToLeft;
			}
		};

	}
}



#endif /* MASKUTILS_H_ */