/*
 * pinlist.h
 *
 * Created: 02.02.2013 15:21:51
 *  Author: astr0
 */ 

#pragma once

#ifndef PINLIST_H_
#define PINLIST_H_

namespace smcu
{
namespace io
{
	namespace priv
	{

////////////////////////////////////////////////////////////////////////////////

		template<unsigned BitsToShift>
		struct ShiftLeft
		{
			template<class T>
			static constexpr T Shift(T value)
			{
				return value << BitsToShift;
			}
		};
		
		template<>
		struct ShiftLeft<0>
		{
			template<class T>
			static constexpr T Shift(T value)
			{
				return value;
			}
		};

		template<unsigned BitsToShift>
		struct ShiftRight
		{
			template<class T>
			static constexpr T Shift(T value)
			{
				return value >> BitsToShift;
			}
		};
		
		template<>
		struct ShiftRight<0>
		{
			template<class T>
			static constexpr T Shift(T value)
			{
				return value;
			}
		};


		enum class MapDirection{ValueToPort = 1 , PortToValue = 0};

		template<unsigned First, unsigned Second, MapDirection mapDirection>
		class Shifter
		{
			typedef ShiftRight<First - Second> RightShifter1;
			typedef ShiftRight<Second - First> RightShifter2;

			typedef ShiftLeft<First - Second> LeftShifter1;
			typedef ShiftLeft<Second - First> LeftShifter2;

			static constexpr bool ShiftDirection = First > Second;
			typedef typename smcu::common::StaticIf<ShiftDirection, LeftShifter1, RightShifter2>::Result FirstShifter;
			typedef typename smcu::common::StaticIf<ShiftDirection, RightShifter1, LeftShifter2>::Result SecondShifter;

			typedef typename smcu::common::StaticIf<mapDirection == MapDirection::ValueToPort, FirstShifter, SecondShifter>::Result ActualShifter;
		public:

			template<class T>
			static constexpr T Shift(T value)
			{
				return ActualShifter::Shift(value);
			}
		};
		

////////////////////////////////////////////////////////////////////////////////
// Holds a Pin class and its bit position in value to read/write
////////////////////////////////////////////////////////////////////////////////

		template<class PIN, uint8_t POSITION>
		struct PinPositionHolder
		{
			typedef PIN Pin;
			static constexpr uint8_t Position = POSITION;
		};
		
////////////////////////////////////////////////////////////////////////////////
// class template MakePortList
// Converts list of Pin types to Port types
////////////////////////////////////////////////////////////////////////////////
		template<class TList>
		struct MakePortList;
		
		template<>
		struct MakePortList<Loki::NullType>
		{
			typedef Loki::NullType Result;
		};
		
		template<class H, class T>
		struct MakePortList<Loki::Typelist<H, T>>
		{
			typedef Loki::Typelist<typename H::Pin::PortType, typename MakePortList<T>::Result> Result;
		};
			

////////////////////////////////////////////////////////////////////////////////
// class template SelectPins
// Assume that TList is type list of PinPositionHolder types.
////////////////////////////////////////////////////////////////////////////////

		template <class TList, template <class> class Predicate>
		struct SelectPins;

		template <template <class> class Predicate>
		struct SelectPins<Loki::NullType, Predicate>
		{
			typedef Loki::NullType Result;
		};

		template <class Head, class Tail, template <class> class Predicate>
		class SelectPins<Loki::Typelist<Head, Tail>, Predicate>
		{
			 typedef typename SelectPins<Tail, Predicate>::Result NotMatch;
			 typedef Loki::Typelist<Head,
					typename SelectPins<Tail, Predicate>::Result>
				Match;
			 static constexpr bool IsMatch = Predicate<Head>::value;
		public:
			 typedef typename Loki::Select<IsMatch,
				Match,
				NotMatch>::Result Result;
		};

////////////////////////////////////////////////////////////////////////////////
//
// 			Select Predicates
//
////////////////////////////////////////////////////////////////////////////////
// class template TransparentMappedPins
// Selects pins types form pin list with port bit position equals to value bit position
// Assume that TList is type list of PinPositionHolder types.
////////////////////////////////////////////////////////////////////////////////

	template<class Item>
	struct TransparentMappedPins
	{
		static constexpr bool value = Item::Position == Item::Pin::Number();
	};

 	template<class Item>
	struct NotTransparentMappedPins
	{
		static constexpr bool value = Item::Position != Item::Pin::Number();
	};
	
////////////////////////////////////////////////////////////////////////////////
// class template TransparentMappedPins
// Selects pins types form pin list with port bit position equals to value bit position
// Assume that TList is type list of PinPositionHolder types.
////////////////////////////////////////////////////////////////////////////////

	template<class Item>
	struct InvertedPins
	{
		static constexpr bool value = Item::Pin::Inverted();
	};

	template<class Item>
	struct NotInvertedPins
	{
		static constexpr bool value = !Item::Pin::Inverted();
	};

////////////////////////////////////////////////////////////////////////////////
// class template PinsWithPort
// Selects with specified port
// Assume that TList is type list of PinPositionHolder types.
////////////////////////////////////////////////////////////////////////////////
	template<class Port>
	struct PinsWithPort
	{
		template<class Item>
		struct Result
		{
			static constexpr bool value = Loki::IsSameType<Port, typename Item::Pin::PortType>::value;
		};
	};


////////////////////////////////////////////////////////////////////////////////
//	Mask for inverted pins
////////////////////////////////////////////////////////////////////////////////

		template <class TList> struct GetInversionMask;

		template <> struct GetInversionMask<Loki::NullType>
		{
			static constexpr int value = 0;
		};

		template <class Head, class Tail>
		struct GetInversionMask< Loki::Typelist<Head, Tail> >
		{
			static constexpr int value = (Head::Pin::Inverted() ? (1 << Head::Pin::Number()) : 0) | GetInversionMask<Tail>::value;
		};

		template <class TList> struct TrueInversionMask;

		template <> struct TrueInversionMask<Loki::NullType>
		{
			static constexpr int value = 0;
		};

		template <class Head, class Tail>
		struct TrueInversionMask< Loki::Typelist<Head, Tail> >
		{
			static constexpr int value = (1 << Head::Pin::Number()) | GetInversionMask<Tail>::value;
		};
		
		template <class TList> struct FalseInversionMask
		{
			static constexpr int value = 0;			
		};
		

////////////////////////////////////////////////////////////////////////////////
// class template GetPortMask
// Computes port bit mask for pin list
// Assume that TList is type list of PinPositionHolder types.
////////////////////////////////////////////////////////////////////////////////

		template <class TList> struct GetPortMask;

		template <> struct GetPortMask<Loki::NullType>
		{
			static constexpr int value = 0;
		};

		template <class Head, class Tail>
		struct GetPortMask< Loki::Typelist<Head, Tail> >
		{
			static constexpr int value = (1 << Head::Pin::Number()) | GetPortMask<Tail>::value;
		};

		//// Calculate bit mask for port
		//template <class PortPins>
		//constexpr uint8_t PortBitMask()
		//{
			//return (1 << typename PortPins::Head::Pin::Number()) | PortBitMask<typename PortPins::Tail>();
		//}
		//
		//template <>
		//constexpr uint8_t PortBitMask<NullType>()
		//{
			//return 0;
		//}


////////////////////////////////////////////////////////////////////////////////
// class template GetValueMask
// Computes value bit mask for pin list
// Assume that TList is type list of PinPositionHolder types.
////////////////////////////////////////////////////////////////////////////////

		template <class TList> struct GetValueMask;

		template <> struct GetValueMask<Loki::NullType>
		{
			static constexpr int value = 0;
		};

		template <class Head, class Tail>
		struct GetValueMask< Loki::Typelist<Head, Tail> >
		{
			static constexpr int value = (1 << Head::Position) | GetValueMask<Tail>::value;
		};
////////////////////////////////////////////////////////////////////////////////
// class template GetSerialCount
// Computes number of seqental pins in list
// Assume that TList is type list of PinPositionHolder types.
////////////////////////////////////////////////////////////////////////////////

		template <class TList> struct GetSerialCount;

		template <> struct GetSerialCount<Loki::NullType>
		{
			static constexpr int value = 0;
			static constexpr int PinNumber = -1;
			static constexpr int BitPosition = -1;
		};
		template <class Head, class Tail>

		struct GetSerialCount< Loki::Typelist<Head, Tail> >
		{
			typedef GetSerialCount<Tail> I;
			static constexpr int PinNumber = Head::Pin::Number();
			static constexpr int BitPosition = Head::Position;
			static constexpr int value =
				((PinNumber == I::PinNumber - 1 &&
				BitPosition == I::BitPosition - 1) ?
				I::value + 1 : 1);
		};
////////////////////////////////////////////////////////////////////////////////
// Returns first Num elements from Typelist
////////////////////////////////////////////////////////////////////////////////
		template <class TList, uint8_t Num> struct TakeFirst;

		template <>
		struct TakeFirst<Loki::NullType, 0>
		{
			typedef Loki::NullType Result;
		};

		template <class Head, class Tail>
		struct TakeFirst<Loki::Typelist<Head, Tail>, 0>
		{
			typedef Loki::NullType Result;
		};

		template <class Head, class Tail, uint8_t Num>
		struct TakeFirst<Loki::Typelist<Head, Tail>, Num>
		{
			typedef Loki::Typelist<Head, typename TakeFirst<Tail, Num-1>::Result>Result;
		};
////////////////////////////////////////////////////////////////////////////////
// Skip Num first elements from tipelist
////////////////////////////////////////////////////////////////////////////////
		template <class TList, uint8_t Num> struct SkipFirst;

		template <>
		struct SkipFirst<Loki::NullType, 0>
		{
			typedef Loki::NullType Result;
		};

		template <class Head, class Tail>
		struct SkipFirst<Loki::Typelist<Head, Tail>, 0>
		{
			typedef Loki::Typelist<Head, Tail> Result;
		};

		template <class Head, class Tail, uint8_t Num>
		struct SkipFirst<Loki::Typelist<Head, Tail>, Num>
		{
			typedef typename SkipFirst<Tail, Num-1>::Result Result;
		};

////////////////////////////////////////////////////////////////////////////////
// class template PinWriteIterator
// Iterates througth pin list to compute value to write to their port
// Assume that TList is type list of PinPositionHolder types.
////////////////////////////////////////////////////////////////////////////////

		template <class TList, template <class> class InversionMask> struct PinWriteIterator;

		template <template <class> class InversionMask> struct PinWriteIterator<Loki::NullType, InversionMask>
		{
			template<class DataType>
			static inline DataType AppendValue(DataType value, DataType result)
			{
				return result;
			}

			template<class DataType>
			static inline DataType AppendReadValue(DataType portValue, DataType result)
			{
				return result;
			}
		};

		template <class Head, class Tail, template <class> class InversionMask>
		struct PinWriteIterator< Loki::Typelist<Head, Tail>, InversionMask >
		{
			template<class DataType>
			static inline DataType AppendValue(DataType value, DataType result)
			{
				typedef Loki::Typelist<Head, Tail> CurrentList;
				typedef typename SelectPins<CurrentList, TransparentMappedPins>::Result TransparentPins;
				typedef typename SelectPins<CurrentList, NotTransparentMappedPins>::Result NotTransparentPins;
				constexpr int TransparentCount = Loki::TL::Length<TransparentPins>::value;

				if(TransparentCount > 1)
				{
					result |= (value &
							GetPortMask<TransparentPins>::value) ^
							InversionMask<TransparentPins>::value;

					return PinWriteIterator<NotTransparentPins, InversionMask>::AppendValue(value, result);
				}

				constexpr int SerialLength = GetSerialCount<CurrentList>::value;

				if(SerialLength >= 2)
				{
					typedef typename TakeFirst<CurrentList, SerialLength>::Result SerialList;
					typedef typename SkipFirst<CurrentList, SerialLength>::Result RestList;

					result |= (Shifter<
							Head::Pin::Number(), 	//bit position in port
							Head::Position, 	//bit position in value
							MapDirection::ValueToPort>::Shift(value) &
							GetPortMask<SerialList>::value) ^
							InversionMask<SerialList>::value;

					return PinWriteIterator<RestList, InversionMask>::AppendValue(value, result);
				}

				if(!(InversionMask<Loki::Typelist<Head, Loki::NullType>>::value))
				{
					if(value & (1 << Head::Position))
						result |= (1 << Head::Pin::Number());
				}
				else
				{
					if(!(value & (1 << Head::Position)))
						result |= (1 << Head::Pin::Number());
				}

				return PinWriteIterator<Tail, InversionMask>::AppendValue(value, result);
			}

			template<class DataType>
			static inline DataType AppendReadValue(DataType portValue, DataType result)
			{
				typedef Loki::Typelist<Head, Tail> CurrentList;
				typedef typename SelectPins<CurrentList, TransparentMappedPins>::Result TransparentPins;
				typedef typename SelectPins<CurrentList, NotTransparentMappedPins>::Result NotTransparentPins;
				constexpr int TransparentCount = Loki::TL::Length<TransparentPins>::value;

				if(TransparentCount > 1)
				{
					result |= (portValue &
							GetValueMask<TransparentPins>::value) ^
							InversionMask<TransparentPins>::value;

					return PinWriteIterator<NotTransparentPins, InversionMask>::AppendReadValue(portValue, result);
				}
				constexpr int SerialLength = GetSerialCount<CurrentList>::value;

				typedef Shifter<
					Head::Pin::Number(),//bit position in port
					Head::Position, 	//bit position in value
					MapDirection::PortToValue> ActualShifter;

				if(SerialLength >= 2)
				{
					typedef typename TakeFirst<CurrentList, SerialLength>::Result SerialList;
					typedef typename SkipFirst<CurrentList, SerialLength>::Result RestList;


					result |= ActualShifter::Shift(portValue ^ InversionMask<SerialList>::value) & GetValueMask<SerialList>::value;
					return PinWriteIterator<RestList, InversionMask>::AppendReadValue(portValue, result);
				}

				if(InversionMask<Loki::Typelist<Head, Loki::NullType>>::value)
				{
					if(!(portValue & (1 << Head::Pin::Number())))
						result |= (1 << Head::Position);

				}else
				{
					if(portValue & (1 << Head::Pin::Number()))
						result |= (1 << Head::Position);
				}


				return PinWriteIterator<Tail, InversionMask>::AppendReadValue(portValue, result);
			}
		};

////////////////////////////////////////////////////////////////////////////////
// class template PortListIterator
// Iterates througth port list and write value to them
// Assume that PinList is type list of PinPositionHolder types.
// And PortList is type list of port types.
////////////////////////////////////////////////////////////////////////////////

		template <class PortList, class PinList, class ValueType> 
		struct PortListIterator;

		template <class PinList, class ValueType> 
		struct PortListIterator<Loki::NullType, PinList, ValueType>
		{
			typedef ValueType DataType;

			static void Write(DataType value)
			{   }
				
			static void ClearAndSet(DataType clearMask, DataType setMask)
			{	}

			static void Set(DataType value)
			{   }

			static void Clear(DataType value)
			{   }

			static void Toggle(DataType value)
			{   }

			static DataType Read()
			{
				return 0;
			}
        };

        template <class Head, class Tail, class PinList, class ValueType>
        struct PortListIterator< Loki::Typelist<Head, Tail>, PinList, ValueType>
        {
			//pins on current port
			typedef typename SelectPins<PinList, PinsWithPort<Head>::template Result>::Result Pins;
			static constexpr int PinsLength = Loki::TL::Length<Pins>::value;
			
			typedef typename SelectPins<Pins, InvertedPins>::Result InvertedPins;
			static constexpr int InvertedPinsLength = Loki::TL::Length<InvertedPins>::value;
			
			typedef typename SelectPins<Pins, NotInvertedPins>::Result NotInvertedPins;
			static constexpr int NotInvertedPinsLength = Loki::TL::Length<NotInvertedPins>::value;

			typedef Head Port; //Head points to current port in the list.
			typedef typename Port::DataType PortType;
			static constexpr PortType Mask = GetPortMask<Pins>::value;

			typedef typename smcu::common::StaticIf
			  		<
					  sizeof(PortType) >= sizeof(ValueType),
					  PortType,
					  ValueType
					 >::Result DataType;

			static void Write(DataType value)
			{
				// Apply inversion mask on value
				DataType result = PinWriteIterator<Pins, GetInversionMask>::AppendValue(value, DataType(0));

				if(PinsLength == (int)Port::Width())// whole port
					Port::Write(result);
				else
				{
					Port::ClearAndSet(Mask, result);
				}

				PortListIterator<Tail, PinList, ValueType>::Write(value);
			}

			static void ClearAndSet(DataType clearMask, DataType setMask)
			{
				DataType resultC = PinWriteIterator<Pins, FalseInversionMask>::AppendValue(clearMask, DataType(0));
				DataType resultS = PinWriteIterator<Pins, FalseInversionMask>::AppendValue(setMask, DataType(0));
				if (NotInvertedPinsLength == PinsLength)
				{
					// All not inverted
					Port::ClearAndSet(resultC, resultS);
				} else if (InvertedPinsLength == PinsLength)
				{
					// All inverted
					Port::ClearAndSet(resultS, resultC);				
				} else
				{
					// Mix... calculate set and clear masks...
					constexpr int inversionMask = GetInversionMask<Pins>::value;
					// clear - not inverted clear + inverted set
					DataType clearMask = (resultC & ~inversionMask) | (resultS & inversionMask);
					// set - not inverted set + inverted clear
					DataType setMask = (resultS & ~inversionMask) | (resultC & inversionMask);
					Port::ClearAndSet(clearMask, setMask);					
				}

				PortListIterator<Tail, PinList, ValueType>::ClearAndSet(clearMask, setMask);
			}

			static void Set(DataType value)
			{
				// Invert inverted
				//static_assert(1 == PinsLength, "All pins are normal");
				if (NotInvertedPinsLength == PinsLength)
				{
					DataType result = PinWriteIterator<Pins, FalseInversionMask>::AppendValue(value, DataType(0));
					Port::Set(result);					
				} else if (InvertedPinsLength == PinsLength)
				{
					DataType result = PinWriteIterator<Pins, FalseInversionMask>::AppendValue(value, DataType(0));
					Port::Clear(result);					
				} else
				{
					DataType clearMask = PinWriteIterator<InvertedPins, FalseInversionMask>::AppendValue(value, DataType(0));
					DataType setMask = PinWriteIterator<NotInvertedPins, FalseInversionMask>::AppendValue(value, DataType(0));
					Port::ClearAndSet(clearMask, setMask);
				}

				PortListIterator<Tail, PinList, ValueType>::Set(value);
			}

			static void Clear(DataType value)
			{
				// Invert inverted
				if (NotInvertedPinsLength == PinsLength)
				{
					DataType result = PinWriteIterator<Pins, FalseInversionMask>::AppendValue(value, DataType(0));
					Port::Clear(result);
				} else if (InvertedPinsLength == PinsLength)
				{
					DataType result = PinWriteIterator<Pins, FalseInversionMask>::AppendValue(value, DataType(0));
					Port::Set(result);
				} else
				{
					DataType clearMask = PinWriteIterator<NotInvertedPins, FalseInversionMask>::AppendValue(value, DataType(0));
					DataType setMask = PinWriteIterator<InvertedPins, FalseInversionMask>::AppendValue(value, DataType(0));
					Port::ClearAndSet(clearMask, setMask);
				}

				PortListIterator<Tail, PinList, ValueType>::Clear(value);
			}

			static void Toggle(DataType value)
			{
				// Ignore inverted - toggle does not care
				DataType result = PinWriteIterator<Pins, FalseInversionMask>::AppendValue(value, DataType(0));
				Port::Toggle(result);

				PortListIterator<Tail, PinList, ValueType>::Toggle(value);
			}
			
			static DataType Read()
			{
				// Apply inversion mask on value
				DataType portValue = Port::Read();
				return PinWriteIterator<Pins, GetInversionMask>::template AppendReadValue<DataType>(
							portValue,
							PortListIterator<Tail, PinList, ValueType>::Read());
			}
        };
		

////////////////////////////////////////////////////////////////////////////////
// class template MakeControllerList
// Converts list of Port types to Controller types
////////////////////////////////////////////////////////////////////////////////
		template<class TList>
		struct MakeControllerList;

		template<>
		struct MakeControllerList<Loki::NullType>
		{
			typedef Loki::NullType Result;
		};

		template<class H, class T>
		struct MakeControllerList<Loki::Typelist<H, T>>
		{
			private:
			typedef typename H::ControllerType ControllerType;
			public:
			typedef typename smcu::common::StaticIf<
				ControllerType::NeedUpdate(),
				Loki::Typelist<ControllerType, typename MakeControllerList<T>::Result>,
				typename MakeControllerList<T>::Result>::Result Result;
		};

////////////////////////////////////////////////////////////////////////////////
// class template GetControllerMask
// Returns Input value mask for Pins and Controller
////////////////////////////////////////////////////////////////////////////////
	template <class TPinList, class TController>
	struct GetControllerMask;

	template <class TController> struct GetControllerMask<Loki::NullType, TController>
	{
		static constexpr int value = 0;
	};

	template <class Head, class Tail, class TController>
	struct GetControllerMask< Loki::Typelist<Head, Tail>, TController >
	{
		static constexpr int value = (Loki::IsSameType<typename Head::Pin::PortType::ControllerType, TController>::value ? (1 << Head::Position) : 0) | GetControllerMask<Tail, TController>::value;
	};

////////////////////////////////////////////////////////////////////////////////
// class template PortControllerListIterator
// Iterates througth controllers list and updates them if needed
// Assume that PinList is type list of PinPositionHolder types.
// And ControllerList is type list of controller types.
////////////////////////////////////////////////////////////////////////////////

		template <class ControllerList, class PinList, class ValueType>
		struct PortControllerListIterator;

		template <class PinList, class ValueType>
		struct PortControllerListIterator<Loki::NullType, PinList, ValueType>
		{
			static void UpdateAll()
			{
				
			}
			
			static void UpdateAffected(ValueType mask)
			{
				
			}
			
			static void RefreshAffected(ValueType mask)
			{
				
			}
			
			static void RefreshAll()
			{				
			}
		};

		template <class Head, class Tail, class PinList, class ValueType>
		struct PortControllerListIterator<Loki::Typelist<Head, Tail>, PinList, ValueType>
		{
			static void UpdateAll()
			{
				Head::Update();
				PortControllerListIterator<Tail, PinList, ValueType>::UpdateAll();
			}
			
			static void UpdateAffected(ValueType mask)
			{
				if (mask & GetControllerMask<PinList, Head>::value)
					Head::Update();
				PortControllerListIterator<Tail, PinList, ValueType>::UpdateAffected(mask);
			}

			static void RefreshAffected(ValueType mask)
			{
				if (mask & GetControllerMask<PinList, Head>::value)
					Head::Refresh();
				PortControllerListIterator<Tail, PinList, ValueType>::RefreshAffected(mask);
			}
			
			static void RefreshAll()
			{
				Head::Refresh();
				PortControllerListIterator<Tail, PinList, ValueType>::RefreshAll();
			}
		};
	

////////////////////////////////////////////////////////////////////////////////
// class template MakePinList
// Creates pin list from passed pins.
////////////////////////////////////////////////////////////////////////////////
		template<uint8_t POSITION, typename ...PINS>
		struct MakePinList;
		
		template<uint8_t POSITION>
		struct MakePinList<POSITION>
		{
			typedef Loki::NullType Result;
		};
		
		template<uint8_t POSITION, typename PIN, typename ...PINS>
		struct MakePinList<POSITION, PIN, PINS...>
		{
			typedef Loki::Typelist<PinPositionHolder<PIN, POSITION>, typename MakePinList<POSITION + 1, PINS...>::Result> Result;
		};
	}
}
}

#endif /* PINLIST_H_ */