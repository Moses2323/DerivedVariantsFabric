#include <iostream>

#include "StackFabricA.h"


//! Checks that you can use const& with the stack element from a fabric
void useConst( const StackFabricA::StackContainerA& constContainer )
{
    const auto& ref = constContainer.get();
    ref.print();
}

//! Test derived classes with arguments in the constructor
void testWithArgs()
{
    // read type
    int i = 0;
    std::cout << "Input 0 for BA and 1 for CA" << std::endl;
    std::cin >> i;

    const auto type = static_cast<StackFabricA::Type>( i );

    // create fabric and the stack object derived from A
    const StackFabricA fabric;
    auto obj = fabric.create( type, 1, 2 );

    // do smth with the object
    std::cout << "isValid = " << obj.isValid() << std::endl;

    if ( obj.isValid() )
    {
        A& ref = obj.get();

        ref.print();
        useConst( obj );
    }
}

//! Test derived classes without arguments in the constructor
void testWithoutArgs()
{
    // read type
    int i = 0;
    std::cout << "Input 0 for BA and 1 for CA without args" << std::endl;
    std::cin >> i;

    const auto type = static_cast<StackFabricAwithoutArgs::Type>( i );

    // create fabric and the stack object derived from A
    const StackFabricAwithoutArgs fabric;
    auto obj = fabric.create( type );

    // do smth with the object
    std::cout << "isValid = " << obj.isValid() << std::endl;

    if ( obj.isValid() )
    {
        AwithoutArgs& ref = obj.get();

        ref.print();
    }
}


int main()
{
    try
    {
        testWithArgs();
        std::cout << std::endl;
        testWithoutArgs();
    }
    catch ( ... )
    {
        std::cout << "ERROR: exception" << std::endl;
    }

    return 0;
}
