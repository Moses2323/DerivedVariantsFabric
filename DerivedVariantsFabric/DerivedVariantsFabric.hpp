#pragma once

// THIS FILE IS A PART OF AN IMPLEMENTATION!
// IT SHOULD NOT BE INCLUDED DIRECTLY INTO THE PROJECTS!

#include "DerivedVariantsFabric.h"


// StackContainer ----------------

template<typename ParentT, typename... DerivedTypes>
constexpr ParentT& 
DerivedVariantsFabric<ParentT, DerivedTypes...>::StackContainer::get()
{
    return getImpl<0>( derivedObject_.index() );
}


template<typename ParentT, typename ... DerivedTypes>
constexpr const ParentT&
DerivedVariantsFabric<ParentT, DerivedTypes...>::StackContainer::get() const
{
    return const_cast<StackContainer&>( *this ).get();
}


template<typename ParentT, typename ... DerivedTypes>
constexpr bool DerivedVariantsFabric<ParentT, DerivedTypes...>
::StackContainer::isValid() const noexcept
{
    return !std::holds_alternative<ErrorType>( derivedObject_ );
}


template<typename ParentT, typename ... DerivedTypes>
template<size_t currIdx>
constexpr ParentT&
DerivedVariantsFabric<ParentT, DerivedTypes...>::StackContainer::getImpl(
    size_t idx
)
{
    if ( idx == currIdx )
    {
        return std::get<currIdx>( derivedObject_ );
    }
    else
    {
        constexpr auto nextIdx = currIdx + 1;
        if constexpr ( nextIdx < sizeof...( DerivedTypes ) )
        {
            return getImpl<nextIdx>( idx );
        }
        else
        {
            throw bad_derived_types_index{};
        }
    }
}


// DerivedVariantsFabric --------------------

template<typename ParentT, typename ... DerivedTypes>
template<typename... Args>
constexpr auto DerivedVariantsFabric<ParentT, DerivedTypes...>::createElement(
    size_t derivedElemIdx, Args&&... args
) -> StackContainer
{
    if ( derivedElemIdx >= NderivedTypes )
    {
        throw bad_derived_types_index{};
    }

    return searchElement<0, Args...>( derivedElemIdx, args... );
}


template<typename ParentT, typename... DerivedTypes>
template<size_t derivedElemIdx, typename... Args>
constexpr auto DerivedVariantsFabric<ParentT, DerivedTypes...>
    ::createElement( Args&&... args ) -> StackContainer
{
    static_assert( derivedElemIdx < NderivedTypes,
                   "Index of derived element >= numbers of derived classes" );

    using CreatorT = ElementByIndexCreator<DerivedTypes...>;

    return StackContainer{
        CreatorT::template create<derivedElemIdx, Args...>( 
            std::forward<Args>( args )... 
        )
    };
}


template<typename ParentT, typename... DerivedTypes>
constexpr auto DerivedVariantsFabric<ParentT, DerivedTypes...>
    ::createErrorElement() -> StackContainer
{
    return StackContainer{};
}


template<typename ParentT, typename ... DerivedTypes>
template<size_t currIdx, typename... Args>
constexpr auto DerivedVariantsFabric<ParentT, DerivedTypes...>::searchElement(
    size_t idx, Args&&... args
) -> StackContainer
{
    if ( currIdx == idx )
    {
        return createElement<currIdx>( args... );
    }
    else
    {
        if constexpr ( currIdx + 1 < NderivedTypes )
        {
            return searchElement<currIdx + 1>( idx, args... );
        }
    }

    // this should not go here ever
    throw bad_derived_types_index{};
}
