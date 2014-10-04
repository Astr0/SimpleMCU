/*
 * utils.h
 *
 * Created: 10/4/2014 6:50:33 PM
 *  Author: astr0
 */ 


#ifndef UTILS_H_
#define UTILS_H_

#include "loki.h"

namespace smcu{
	namespace common{
		constexpr unsigned GetByteSize(unsigned bitSize)
		{
			return bitSize == 0 ? 1 : ((bitSize / 8)  + ((bitSize % 8) ? 1 : 0));
		}	
		
		template<unsigned MaxValue>
		struct NumberType
		{
			typedef typename Loki::Select<(MaxValue <= 256), 
				uint_fast8_t, 
				typename Loki::Select<(MaxValue <= 65536), 
					uint_fast16_t, 
					typename Loki::Select<(MaxValue <= 4294967296), 
						uint_fast32_t, 
						void>::Result
					>::Result
				>::Result Result;
		};
		
		template<unsigned bytes>
		struct MaskType;
		
		template<>
		struct MaskType<1>
		{
			typedef uint_fast8_t Result;
		};
		template<>
		struct MaskType<2>
		{
			typedef uint_fast16_t Result;
		};
		template<>
		struct MaskType<3>
		{
			typedef uint_fast32_t Result;
		};
		template<>
		struct MaskType<4>
		{
			typedef uint_fast32_t Result;
		};	
		
		template<unsigned bytes>
		struct DataType{
			typedef typename MaskType<bytes>::Result Result;	
		};
	}	
}



#endif /* UTILS_H_ */