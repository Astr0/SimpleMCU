/*
* portcontroller.h
*
* Created: 01.02.2013 14:59:53
*  Author: astr0
*/
#pragma once

#ifndef PORTCONTROLLER_H_
#define PORTCONTROLLER_H_

namespace smcu
{
	namespace io
	{
		class NullPortController
		{
			public:
				static constexpr bool NeedUpdate() {return false;}
				static void SetDirty()
				{					
				}
				static constexpr bool IsDirty()
				{
					return false;					
				}				
				static void Update()
				{
				}	
				static void Refresh()
				{
				}
		};
	}
}



#endif /* PORTCONTROLLER_H_ */