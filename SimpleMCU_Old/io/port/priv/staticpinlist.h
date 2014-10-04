/*
 * staticpinlist.h
 *
 * Created: 02.02.2013 15:21:51
 *  Author: astr0
 */ 

#pragma once

#ifndef STATICPINLIST_H_
#define STATICPINLIST_H_

#include "../../../common/typelist.h"
#include "../../../common/maskutils.h"
#include "../../pin/pininfo.h"
#include "../portinfo.h"

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
					typedef typename PinInfo<typename TPinHolder::Pin>::PortType Result;
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
						static constexpr bool value = Loki::IsSameType<TPort, typename PinInfo<typename TPinHolder::Pin>::PortType>::value;
					};
				};


				//	Mask for inverted pins
		
				template <class TList, class MaskType> 
				struct GetInversionMask
				{
					template <class TPinHolder, MaskType VTail>
					struct Predicate
					{
						static constexpr MaskType value = (TPinHolder::Pin::IsInverted() ? TPinHolder::Pin::Mask() : 0) | VTail;
					};
					static constexpr MaskType Empty = MaskType();					
					static constexpr MaskType value = smcu::common::TL::Aggregate<TList, MaskType, Empty, Predicate>::value;
				};

				// Ignore inversion mask
				template <class TList, class MaskType> 
				struct FalseInversionMask
				{
					static constexpr MaskType value = MaskType();			
				};
		

				//	Mask for port pins
				template <class TList, class MaskType>
				struct GetPortMask
				{
					template <class TPinHolder, MaskType VTail>
					struct Predicate
					{
						static constexpr MaskType value = TPinHolder::Pin::Mask() | VTail;
					};					
					static constexpr MaskType Empty = MaskType();
					static constexpr MaskType value = smcu::common::TL::Aggregate<TList, MaskType, Empty, Predicate>::value;
				};


				// Value bit mask for pin list
				template <class TList, class TValueType> struct GetValueMask;

				template <class TValueType> 
				struct GetValueMask<Loki::NullType, TValueType>
				{
					typedef smcu::common::BitMaskTypes<TValueType> BM;
					typedef typename BM::MaskType MaskType;
					static constexpr MaskType value = MaskType();
				};

				template <class Head, class Tail, class TValueType>
				struct GetValueMask<Loki::Typelist<Head, Tail>, TValueType>
				{
					typedef smcu::common::BitMaskTypes<TValueType> BM;
					typedef typename BM::MaskType MaskType;					
					static constexpr MaskType value = BM::BitToMask(Head::Position) | GetValueMask<Tail, TValueType>::value;
				};
		
				// Number of sequental pins in list
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

				// Iterates through pin list to compute value to write to their port
				// TODO: Refactor mask stuff to generate normal type, use |= rather than return result |
				template <class TList, class TDataType> struct PinWriteIterator;

				template <class TDataType> 
				class PinWriteIterator<Loki::NullType, TDataType>
				{
					typedef smcu::common::BitMaskTypes<TDataType> MT;
					typedef typename MT::MaskType MaskType;
					
					public:
					
					template<class DataType, template<class, class> class InversionMask>
					static inline DataType AppendValue(DataType value, DataType result)
					{
						return result;
					}
					
					static inline TDataType AppendWriteValue(TDataType value, TDataType result)
					{
						return result;
					}

					static inline TDataType AppendMaskValue(MaskType value, MaskType result)
					{
						return result;
					}

					static inline TDataType AppendReadValue(TDataType portValue, TDataType result)
					{
						return result;
					}
					
					static void Write(MaskType mask)
					{   }
				
					static void ClearAndSet(MaskType clearMask, MaskType setMask)
					{	}

					static void Set(MaskType mask)
					{   }

					static void Clear(MaskType mask)
					{   }

					static void Toggle(MaskType mask)
					{   }			
							
					static void Read(TDataType& result)
					{
					}
					
					static void Update(MaskType mask) 
					{
					}

					static void Refresh(MaskType mask) 
					{
					}
							
				};

				template <class Head, class Tail, class TDataType>
				class PinWriteIterator<Loki::Typelist<Head, Tail>, TDataType>
				{
					typedef smcu::common::BitMaskTypes<TDataType> MT;
					typedef typename MT::MaskType MaskType;
					typedef PinWriteIterator<Tail, TDataType> NextIterator;
					static constexpr typename MT::OneBitMask ListMask = MT::BitToMask(Head::Position);
					typedef typename Head::Pin Pin;
					typedef typename PinInfo<Pin>::PortType PortType;
					typedef PinWriteIterator<Tail, TDataType> Next;
					
					public:
					template<class DataType, template<class, class> class InversionMask>
					static inline DataType AppendValue(DataType value, DataType result)
					{
						typedef Loki::Typelist<Head, Tail> CurrentList;
						typedef typename smcu::common::TL::Where<CurrentList, PinPositionMatchPredicate>::Result TransparentPins;
						typedef typename smcu::common::TL::Where<CurrentList, smcu::common::TL::Not<PinPositionMatchPredicate>::Result>::Result NotTransparentPins;
						constexpr int TransparentCount = Loki::TL::Length<TransparentPins>::value;

						if(TransparentCount > 1)
						{
							result |= (value &
									GetPortMask<TransparentPins, DataType>::value) ^
									InversionMask<TransparentPins, DataType>::value;

							return PinWriteIterator<NotTransparentPins, TDataType>::template AppendValue<DataType, InversionMask>(value, result);
						}

						constexpr int SerialLength = GetSerialCount<CurrentList>::value;

						if(SerialLength >= 2)
						{
							typedef typename smcu::common::TL::TakeFirst<CurrentList, SerialLength>::Result SerialList;
							typedef typename smcu::common::TL::SkipFirst<CurrentList, SerialLength>::Result RestList;

							result |= (smcu::common::Shifter<Head::Pin::Number() - Head::Position>::Shift(value) & GetPortMask<SerialList, DataType>::value) ^ InversionMask<SerialList, DataType>::value;

							return PinWriteIterator<RestList, TDataType>::template AppendValue<DataType, InversionMask>(value, result);
						}

						if(!(InversionMask<Loki::Typelist<Head, Loki::NullType>, DataType>::value))
						{
							if(value & ListMask)
								result |= Head::Pin::Mask();
						}
						else
						{
							if(!(value & ListMask))
								result |= Head::Pin::Mask();
						}

						return PinWriteIterator<Tail, TDataType>::template AppendValue<DataType, InversionMask>(value, result);
					}
					
					static inline TDataType AppendWriteValue(TDataType value, TDataType result)
					{
						return AppendValue<TDataType, GetInversionMask>(value, result);
					}

					static inline TDataType AppendMaskValue(MaskType value, MaskType result)
					{
						return AppendValue<MaskType, FalseInversionMask>(value, result);
					}

					static inline TDataType AppendReadValue(TDataType portValue, TDataType result)
					{
						typedef Loki::Typelist<Head, Tail> CurrentList;
						typedef typename smcu::common::TL::Where<CurrentList, PinPositionMatchPredicate>::Result TransparentPins;
						typedef typename smcu::common::TL::Where<CurrentList, smcu::common::TL::Not<PinPositionMatchPredicate>::Result>::Result NotTransparentPins;
						constexpr int TransparentCount = Loki::TL::Length<TransparentPins>::value;

						if(TransparentCount > 1)
						{
							result |= (portValue &
									GetValueMask<TransparentPins, MaskType>::value) ^
									GetInversionMask<TransparentPins, TDataType>::value;

							return PinWriteIterator<NotTransparentPins, TDataType>::AppendReadValue(portValue, result);
						}
						constexpr int SerialLength = GetSerialCount<CurrentList>::value;

						if(SerialLength >= 2)
						{
							typedef typename smcu::common::TL::TakeFirst<CurrentList, SerialLength>::Result SerialList;
							typedef typename smcu::common::TL::SkipFirst<CurrentList, SerialLength>::Result RestList;


							result |= smcu::common::Shifter<Head::Position - Head::Pin::Number()>::Shift(portValue ^ GetInversionMask<SerialList, MaskType>::value) & GetValueMask<SerialList, MaskType>::value;
							return PinWriteIterator<RestList, TDataType>::AppendReadValue(portValue, result);
						}

						if(GetInversionMask<Loki::Typelist<Head, Loki::NullType>, MaskType>::value)
						{
							if(!(portValue & Head::Pin::Mask()))
								result |= ListMask;

						}else
						{
							if(portValue & Head::Pin::Mask())
								result |= ListMask;
						}


						return PinWriteIterator<Tail, TDataType>::AppendReadValue(portValue, result);
					}
					
					static void Write(TDataType value)
					{
						Pin::Set(value & ListMask);
						Next::Write(value);
					}
					
					static void ClearAndSet(MaskType clearMask, MaskType setMask)
					{				
						if (clearMask & ListMask)
						{
							if (!(setMask & ListMask))
								Pin::Clear();
						} else if (setMask & ListMask)
							Pin::Set();
						Next::ClearAndSet(clearMask, setMask);								
					}

					static void Set(MaskType mask)					
					{
						if (mask & ListMask)
							Pin::Set();
						Next::Set(mask);			
					}

					static void Clear(MaskType mask)					
					{
						if (mask & ListMask)
							Pin::Clear();
						Next::Clear(mask);
					}

					static void Toggle(MaskType mask)					
					{
						if (mask & ListMask)
							Pin::Toggle(mask);
						Next::Toggle(mask);						
					}		
					
					static void Read(TDataType& result)
					{
						if (Pin::Read())
							result |= ListMask;
						Next::Read(result);
					}
						
					static void Update(MaskType mask) 
					{
						if (mask & ListMask)
						{
							PortType::Update(Pin::Mask());
						}
						Next::Update(mask);
					}

					static void Refresh(MaskType mask) 
					{
						if (mask & ListMask)
						{
							PortType::Refresh(Pin::Mask());
						}
						Next::Refresh(mask);
					}
				};
				
				// Iterates through port list and write value to them
				// TODO: Fix DataType, MaskType and PinMaskType!!!
				//		 Add Read(PinMaskType) - performance!		
				
				template <class TPortList, class TPinList, class TValueType>
				struct PortListIterator;						
				template <class TPortList, class TPinList, class TValueType, bool VIsComplex>
				struct PortListIteratorImpl;

				template <class TPinList, class TValueType, bool VIsComplex> 
				class PortListIteratorImpl<Loki::NullType, TPinList, TValueType, VIsComplex>
				{
					typedef smcu::common::BitMaskTypes<TValueType> BitMaskTypes;
					typedef typename BitMaskTypes::OneBitMask PinMaskType;
					typedef typename BitMaskTypes::BitNumberType PinNumberType;
					typedef typename BitMaskTypes::MaskType MaskType;
					public:					
					typedef TValueType DataType;

					static void Write(DataType value)
					{   }
				
					static void ClearAndSet(MaskType clearMask, MaskType setMask)
					{	}

					static void Set(MaskType value)
					{   }

					static void Clear(MaskType value)
					{   }

					static void Toggle(MaskType value)
					{   }

					static DataType Read()
					{
						return DataType();
					}
			
					static void Update(MaskType mask)
					{
				
					}
			
					static void Refresh(MaskType mask)
					{
				
					}
				};
				
				template <class THead, class TTail, class TPinList, class TValueType>
				class PortListIteratorImpl< Loki::Typelist<THead, TTail>, TPinList, TValueType, true>
				{
					//pins on current port
					typedef typename smcu::common::TL::Where<TPinList, PortPinsPredicate<THead>::template Result>::Result Pins;
					static constexpr int PinsLength = Loki::TL::Length<Pins>::value;
			
					typedef typename smcu::common::TL::Where<Pins, InvertedPinsPredicate>::Result InvertedPins;
					static constexpr int InvertedPinsLength = Loki::TL::Length<InvertedPins>::value;
			
					typedef typename smcu::common::TL::Where<Pins, smcu::common::TL::Not<InvertedPinsPredicate>::Result>::Result NotInvertedPins;
					static constexpr int NotInvertedPinsLength = Loki::TL::Length<NotInvertedPins>::value;

					typedef THead Port; //Head points to current port in the list.
					typedef typename PortInfo<Port>::DataType PortType;
					typedef typename PortInfo<Port>::MaskType PortMaskType;
					static constexpr PortMaskType Mask = GetPortMask<Pins, PortMaskType>::value;

					typedef typename Loki::Select<sizeof(PortType) >= sizeof(TValueType), PortType, TValueType>::Result DataType;
					
					typedef smcu::common::BitMaskTypes<DataType> BitMaskTypes;
					typedef typename BitMaskTypes::OneBitMask PinMaskType;
					typedef typename BitMaskTypes::BitNumberType PinNumberType;
					typedef typename BitMaskTypes::MaskType MaskType;
					
					typedef PinWriteIterator<Pins, DataType> PinWrite;
					typedef PortListIterator<TTail, TPinList, TValueType> Next;
					
					public:

					static void Write(DataType value)
					{
						// Apply inversion mask on value
						DataType result = PinWrite::AppendWriteValue(value, DataType(0));

						if(PinsLength == (int)Port::Width())// whole port
							Port::Write(result);
						else
						{
							Port::ClearAndSet(Mask, result);
						}

						Next::Write(value);
					}

					static void ClearAndSet(MaskType clearMask, MaskType setMask)
					{
						MaskType resultC = PinWrite::AppendMaskValue(clearMask, MaskType());
						MaskType resultS = PinWrite::AppendMaskValue(setMask, MaskType());
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
							constexpr MaskType inversionMask = GetInversionMask<Pins, MaskType>::value;
							// clear - not inverted clear + inverted set
							DataType clearMask = (resultC & ~inversionMask) | (resultS & inversionMask);
							// set - not inverted set + inverted clear
							DataType setMask = (resultS & ~inversionMask) | (resultC & inversionMask);
							Port::ClearAndSet(clearMask, setMask);					
						}

						Next::ClearAndSet(clearMask, setMask);
					}

					static void Set(MaskType value)
					{
						// Invert inverted
						//static_assert(1 == PinsLength, "All pins are normal");
						if (NotInvertedPinsLength == PinsLength)
						{
							MaskType result = PinWrite::AppendMaskValue(value, MaskType());
							Port::Set(result);					
						} else if (InvertedPinsLength == PinsLength)
						{
							MaskType result = PinWrite::AppendMaskValue(value, MaskType());
							Port::Clear(result);					
						} else
						{
							MaskType clearMask = PinWriteIterator<InvertedPins, DataType>::AppendMaskValue(value, MaskType());
							MaskType setMask = PinWriteIterator<NotInvertedPins, DataType>::AppendMaskValue(value, MaskType());
							Port::ClearAndSet(clearMask, setMask);
						}

						Next::Set(value);
					}

					static void Clear(MaskType value)
					{
						// Invert inverted
						if (NotInvertedPinsLength == PinsLength)
						{
							MaskType result = PinWrite::AppendMaskValue(value, MaskType());
							Port::Clear(result);
						} else if (InvertedPinsLength == PinsLength)
						{
							MaskType result = PinWrite::AppendMaskValue(value, MaskType());
							Port::Set(result);
						} else
						{
							MaskType clearMask = PinWriteIterator<NotInvertedPins, DataType>::AppendMaskValue(value, MaskType());
							MaskType setMask = PinWriteIterator<InvertedPins, DataType>::AppendMaskValue(value, MaskType());
							Port::ClearAndSet(clearMask, setMask);
						}

						Next::Clear(value);
					}

					static void Toggle(MaskType value)
					{
						// Ignore inverted - toggle does not care
						MaskType result = PinWrite::AppendMaskValue(value, MaskType());
						Port::Toggle(result);

						Next::Toggle(value);
					}
			
					static DataType Read()
					{
						// Apply inversion mask on value
						DataType portValue = Port::Read();
						return PinWrite::AppendReadValue(portValue, Next::Read());
					}
			
					static void Update(MaskType mask)
					{
						if (!Port::IsAutoUpdate())
						{
							MaskType portMask = PinWrite::AppendMaskValue(mask, MaskType());
							if (portMask)
								Port::Update(portMask);
						}
						Next::Update(mask);
					}

					static void Refresh(MaskType mask)
					{
						if (!Port::IsAutoUpdate())
						{
							MaskType portMask = PinWrite::AppendMaskValue(mask, MaskType());
							if (portMask)
								Port::Refresh(portMask);
						}
						Next::Refresh(mask);
					}
				};				


				template <class THead, class TTail, class TPinList, class TValueType>
				class PortListIteratorImpl< Loki::Typelist<THead, TTail>, TPinList, TValueType, false>
				{
					//pins on current port
					typedef typename smcu::common::TL::Where<TPinList, PortPinsPredicate<THead>::template Result>::Result Pins;

					typedef THead Port; //Head points to current port in the list.

					typedef TValueType DataType;
					
					typedef smcu::common::BitMaskTypes<DataType> BitMaskTypes;
					typedef typename BitMaskTypes::OneBitMask PinMaskType;
					typedef typename BitMaskTypes::BitNumberType PinNumberType;
					typedef typename BitMaskTypes::MaskType MaskType;
					typedef PinWriteIterator<Pins, DataType> PinWrite;
					typedef PortListIterator<TTail, TPinList, TValueType> Next;
					
					public:

					static void Write(DataType value)
					{
						PinWrite::Write(value);						

						Next::Write(value);
					}

					static void ClearAndSet(MaskType clearMask, MaskType setMask)
					{
						PinWrite::ClearAndSet(clearMask, setMask);

						Next::ClearAndSet(clearMask, setMask);
					}

					static void Set(MaskType value)
					{
						PinWrite::Set(value);

						Next::Set(value);
					}

					static void Clear(MaskType value)
					{
						PinWrite::Clear(value);

						Next::Clear(value);
					}

					static void Toggle(MaskType value)
					{
						PinWrite::Toggle(value);

						Next::Toggle(value);
					}
					
					static DataType Read()
					{
						DataType result;
						PinWrite::Read(result);
						return result;
					}
					
					static void Update(MaskType mask)
					{
						if (!Port::IsAutoUpdate())
							PinWrite::Update(mask);
						Next::Update(mask);
					}

					static void Refresh(MaskType mask)
					{
						if (!Port::IsAutoUpdate())
							PinWrite::Refresh(mask);
						Next::Refresh(mask);
					}
				};

				template<class PortList, class ValueType>
				struct IsComplexPortListImpl
				{
					typedef typename PortList::Head PortType;
					static constexpr bool value = !PortInfo<PortType>::OnlyPinInterface && !smcu::common::BitMaskTypes<ValueType>::OnlyBitInterface;
				};

				template<class ValueType>
				struct IsComplexPortListImpl<Loki::NullType, ValueType>
				{
					static constexpr bool value = false;
				};

				template <class PortList, class PinList, class ValueType> 
				struct PortListIterator: public PortListIteratorImpl<PortList, PinList, ValueType, IsComplexPortListImpl<PortList, ValueType>::value>
				{
				};
		

				// Creates pin list from passed pins.
				template<typename ...TPins>
				struct MakePinList
				{
					typedef typename smcu::common::TL::MakeList<0, PinPositionHolder, TPins...>::Result Result;
				};
				// Returns if all ports in list are auto-updated
				template<class TPort>
				struct PortAutoUpdatedPredicate
				{
					static constexpr bool value = TPort::IsAutoUpdate();
				};
			}
		}
	}
}

#endif /* STATICPINLIST_H_ */