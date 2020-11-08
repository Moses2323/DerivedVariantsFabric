#pragma once

#include <cstdint>
#include "ExampleAclass.h"
#include "DerivedVariantsFabric.h"


//! Special fabric to create derived classes on stack using std::variant<>
//! specifically for the derived classes from A
struct StackFabricA : private DerivedVariantsFabric<A, BA, CA>
{
    constexpr StackFabricA() noexcept = default;

    using StackContainerA = StackContainer;

    enum class Type : uint8_t
    {
        BA = 0,
        CA,
        Size
    };

    StackContainerA create( Type tp, int v1, int v2 ) const noexcept;
};


struct StackFabricAwithoutArgs
    : private DerivedVariantsFabric<AwithoutArgs, BAwithoutArgs, CAwithoutArgs>
{
    constexpr StackFabricAwithoutArgs() noexcept = default;

    using StackContainerA = StackContainer;

    enum class Type : uint8_t
    {
        BA = 0,
        CA,
        Size
    };

    StackContainerA create( Type tp ) const noexcept;
};


// implementation -------------------------------------------------------------

inline StackFabricA::StackContainerA StackFabricA::create( 
    Type tp, int v1, int v2 
) const noexcept
{
    // this check prevents throwing an exception
    if ( static_cast<size_t>(tp) >= static_cast<size_t>( Type::Size ) )
    {
        return createErrorElement();
    }

    return createElement( static_cast<size_t>(tp), v1, v2 );
}


inline StackFabricAwithoutArgs::StackContainerA
StackFabricAwithoutArgs::create( Type tp ) const noexcept
{
    // this check prevents throwing an exception
    if ( static_cast<size_t>( tp ) >= static_cast<size_t>( Type::Size ) )
    {
        return createErrorElement();
    }

    return createElement( static_cast<size_t>( tp ) );
}
