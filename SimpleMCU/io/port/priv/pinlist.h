/*
 * pinlist.h
 *
 * Created: 02.02.2013 15:21:51
 *  Author: astr0
 */ 

#pragma once

#ifndef PINLIST_H_
#define PINLIST_H_

#include "../../../common/typelist.h"
#include "../../../common/maskutils.h"

namespace smcu
{
	namespace io
	{
		namespace types
		{
			namespace priv
			{
				// Holds pin and position in PinList
				template<int VPosition, class TPin>
				struct PinPositionHolder
				{
					typedef TPin Pin;
					static constexpr int Position = VPosition;
				};
		
				// Select Ports from pin list
				template<class TPinHolder>
				struct SelectPinPort
				{
					typedef typename TPinHolder::Pin::PortType Result;
				};
		
				template<class TPinList>
				struct MakePortList
				{
					typedef typename smcu::common::TL::Select<TPinList, SelectPinPort>::Result Result;
				};
		

				// Predicate<PinPositionHolder> - pins that match 1 to 1 with port pins
				template<class TPinHolder>
				struct PinPositionMatchPredicate
				{
					static constexpr bool value = TPinHolder::Position == TPinHolder::Pin::Number();
				};

				// Predicate<PinPositionHolder> - inverted pins
				template<class TPinHolder>
				struct InvertedPinsPredicate
				{
					static constexpr bool value = TPinHolder::Pin::IsInverted();
				};

				// Result - Predicate<PinPositionHolder> - pins that belongs to TPort Port
				template<class TPort>
				struct PortPinsPredicate
				{
					template<class TPinHolder>
					struct Result
					{
						static constexpr bool value = Loki::IsSameType<TPort, typename TPinHolder::Pin::PortType>::value;
					};
				};


				//	Mask for inverted pins
				template <class TPinHolder, int VTail>
				struct InversionMaskPredicate
				{
					static constexpr int value = (TPinHolder::Pin::IsInverted() ? (1 << TPinHolder::Pin::Number()) : 0) | VTail;			
				};
		
				template <class TList> 
				struct GetInversionMask
				{
					static constexpr int value = smcu::common::TL::Aggregate<TList, int, 0, InversionMaskPredicate>::value;
				};

				// Ignore inversion mask
				template <class TList> 
				struct FalseInversionMask
				{
					static constexpr int value = 0;			
				};
		

				//	Mask for port pins
				template <class TPinHolder, int VTail>
				struct PortMaskPredicate
				{
					static constexpr int value =  (1 << TPinHolder::Pin::Number()) | VTail;
				};
				
				template <class TList>
				struct GetPortMask
				{
					static constexpr int value = smcu::common::TL::Aggregate<TList, int, 0, PortMaskPredicate>::value;
				};


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
						typedef typename smcu::common::TL::Where<CurrentList, PinPositionMatchPredicate>::Result TransparentPins;
						typedef typename smcu::common::TL::Where<CurrentList, smcu::common::TL::Not<PinPositionMatchPredicate>::Result>::Result NotTransparentPins;
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
							typedef typename smcu::common::TL::TakeFirst<CurrentList, SerialLength>::Result SerialList;
							typedef typename smcu::common::TL::SkipFirst<CurrentList, SerialLength>::Result RestList;

							result |= (smcu::common::Shifter<Head::Pin::Number() - Head::Position>::Shift(value) & GetPortMask<SerialList>::value) ^ InversionMask<SerialList>::value;

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
						typedef typename smcu::common::TL::Where<CurrentList, PinPositionMatchPredicate>::Result TransparentPins;
						typedef typename smcu::common::TL::Where<CurrentList, smcu::common::TL::Not<PinPositionMatchPredicate>::Result>::Result NotTransparentPins;
						constexpr int TransparentCount = Loki::TL::Length<TransparentPins>::value;

						if(TransparentCount > 1)
						{
							result |= (portValue &
									GetValueMask<TransparentPins>::value) ^
									InversionMask<TransparentPins>::value;

							return PinWriteIterator<NotTransparentPins, InversionMask>::AppendReadValue(portValue, result);
						}
						constexpr int SerialLength = GetSerialCount<CurrentList>::value;

						if(SerialLength >= 2)
						{
							typedef typename smcu::common::TL::TakeFirst<CurrentList, SerialLength>::Result SerialList;
							typedef typename smcu::common::TL::SkipFirst<CurrentList, SerialLength>::Result RestList;


							result |= smcu::common::Shifter<Head::Position - Head::Pin::Number()>::Shift(portValue ^ InversionMask<SerialList>::value) & GetValueMask<SerialList>::value;
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
		// TODO: Fix DataType, MaskType and PinMaskType!!!
		//		 Add Read(PinMaskType) - performance!
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
			
					static void UpdateAll()
					{
				
					}
			
					static void UpdateAffected(ValueType mask)
					{
				
					}
			
					static void RefreshAll()
					{
					}

					static void RefreshAffected(ValueType mask)
					{
				
					}
				};

				template <class Head, class Tail, class PinList, class ValueType>
				struct PortListIterator< Loki::Typelist<Head, Tail>, PinList, ValueType>
				{
					//pins on current port
					typedef typename smcu::common::TL::Where<PinList, PortPinsPredicate<Head>::template Result>::Result Pins;
					static constexpr int PinsLength = Loki::TL::Length<Pins>::value;
			
					typedef typename smcu::common::TL::Where<Pins, InvertedPinsPredicate>::Result InvertedPins;
					static constexpr int InvertedPinsLength = Loki::TL::Length<InvertedPins>::value;
			
					typedef typename smcu::common::TL::Where<Pins, smcu::common::TL::Not<InvertedPinsPredicate>::Result>::Result NotInvertedPins;
					static constexpr int NotInvertedPinsLength = Loki::TL::Length<NotInvertedPins>::value;

					typedef Head Port; //Head points to current port in the list.
					typedef typename Port::DataType PortType;
					static constexpr PortType Mask = GetPortMask<Pins>::value;

					typedef typename Loki::Select<sizeof(PortType) >= sizeof(ValueType), PortType, ValueType>::Result DataType;

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
			
					static void UpdateAll()
					{
						if (!Port::IsAutoUpdate())
							Port::Update(Mask);
						PortListIterator<Tail, PinList, ValueType>::UpdateAll();
					}
			
					static void UpdateAffected(ValueType mask)
					{
						if (!Port::IsAutoUpdate())
						{
							DataType portMask = PinWriteIterator<Pins, FalseInversionMask>::AppendValue(mask, DataType(0));
							if (portMask)
								Port::Update(portMask);
						}
						PortListIterator<Tail, PinList, ValueType>::UpdateAffected(mask);
					}

					static void RefreshAll()
					{
						if (!Port::IsAutoUpdate())
							Port::Refresh(Mask);
						PortListIterator<Tail, PinList, ValueType>::RefreshAll();
					}

					static void RefreshAffected(ValueType mask)
					{
						if (!Port::IsAutoUpdate())
						{
							DataType portMask = PinWriteIterator<Pins, FalseInversionMask>::AppendValue(mask, DataType(0));
							if (portMask)
								Port::Refresh(portMask);
						}
						PortListIterator<Tail, PinList, ValueType>::RefreshAffected(mask);
					}
				};
		

		////////////////////////////////////////////////////////////////////////////////
		// class template MakePinList
		// Creates pin list from passed pins.
		////////////////////////////////////////////////////////////////////////////////
				template<typename ...TPins>
				struct MakePinList
				{
					typedef typename smcu::common::TL::MakeList<0, PinPositionHolder, TPins...>::Result Result;
				};
		////////////////////////////////////////////////////////////////////////////////
		// All autoupdate
		// returns if all ports in list are auto-updated
		////////////////////////////////////////////////////////////////////////////////
				template<class TPort>
				struct PortAutoUpdatedPredicate
				{
					static constexpr bool value = TPort::IsAutoUpdate();
				};
			}
		}
	}
}

#endif /* PINLIST_H_ */