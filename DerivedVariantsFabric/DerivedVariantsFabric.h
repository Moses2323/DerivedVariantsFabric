#pragma once

#include <variant>

/*! Creates an element on the position of the input index using arguments.
 *
 *  ElementByIndexCreator<int, double, bool>
 *  here 0 - int, 1 - double, 2 - bool
 */
template<typename... Types>
struct ElementByIndexCreator
{
    template<size_t i, typename... Args>
    static decltype(auto) create( Args&&... args )
    {
        throw std::exception{ "Can\'t create element from the empty set" };
    }
};

template<typename T, typename... OtherTypes>
struct ElementByIndexCreator<T, OtherTypes...>
{
    template<size_t idx, typename... Args>
    static decltype(auto) create( Args&&... args )
    {
        if constexpr ( idx == 0 )
        {
            return T{ std::forward<Args>( args )... };
        }
        else
        {
            using next = ElementByIndexCreator<OtherTypes...>;
            return next::template create<idx - 1, Args...>(
                std::forward<Args>( args )... 
            );
        }
    }
};


/*! A carcass for the other fabrics that want to create a derived object
 *  on the stack instead of using a heap memory.
 *
 *  Fabric can be used with classes that have several arguments in the
 *  constructor. However, number of arguments in the constructor should be
 *  the same for all used derived classes.
 *
 *  How to use:
 *  Create your own fabric class and derive from this, or just use it inside.
 *
 *  \code
 *  struct A
 *  {
 *      virtual void doSmth() = 0;
 *  };
 *
 *  struct B : A
 *  {
 *      virtual void doSmth() override { // do smth B }
 *  };
 *
 *  struct C : A
 *  {
 *      virtual void doSmth() override { // do smth C }
 *  };
 *
 *  // fabric
 *  struct MyFabric : private DerivedVariantsFabric<A, B, C> 
 *  {
 *      using StackA = StackContainer;
 *
 *      enum class Type : uint8_t
 *      {
 *          B = 0, C
 *      };
 *
 *      StackA create( Type tp ) const noexcept
 *      {
 *          const size_t idx = static_cast<size_t>( tp );
 *          if ( idx < NderivedTypes )
 *          {
 *              return createElement( idx );
 *          }
 *
 *          // if you don't like exceptions.
 *          // Otherwise, you can just use createElement(idx) without check
 *          return createErrorElement();
 *      }
 *  };
 *
 *  void testExample()
 *  {
 *      // dynamic 
 *      int i = 0;
 *      std::cout << "0 for B, 1 for C" << std::endl;
 *      std::cin >> i;
 *
 *      // creating an element derived from A
 *      MyFabric fab;
 *      auto obj = fab.create( static_cast<MyFabric::Type>( i ) );
 *
 *      // using the element
 *      if ( obj.isValid() )
 *      {
 *          A& ref = obj.get();
 *          ref.doSmth();
 *      }
 *  }
 *
 *  \endcode
 */
template<typename ParentT, typename... DerivedTypes>
class DerivedVariantsFabric
{
public:
    //! Exception for bad index (>= than sizeof...(DerivedTypes))
    struct bad_derived_types_index : std::exception {};

protected:
    //! Number of derived types mentioned in this class
    static constexpr size_t NderivedTypes = sizeof...( DerivedTypes );

    static_assert( NderivedTypes > 0,
                   "at least one derived type should be presented" );

    class StackContainer
    {
        //! Type that is used for indicating an error
        struct ErrorType
        {
            template<typename... Args>
            constexpr ErrorType( Args&&... args ) noexcept {}
        };

        //! Type of the variant that is used as a stack holder of the object
        using InternalContainerT = std::variant<DerivedTypes..., ErrorType>;

    public:
        /*! Returns the reference to the parent type from the object in the
         *  variant.
         *
         *  \details if it is an error type, throw the exception.
         */
        constexpr ParentT& get();

        constexpr const ParentT& get() const;

        //! Checks if this variant contains error-type or derived types
        constexpr bool isValid() const noexcept;


        /*! Can be used by user, but not recommended.
         *  Use it only if this interface is not enough for you
         */
        constexpr const InternalContainerT& getVariant() const
            { return derivedObject_; }

        constexpr InternalContainerT& getVariant() { return derivedObject_; }

    private:
        constexpr explicit StackContainer( InternalContainerT&& obj )
            noexcept( std::is_nothrow_move_constructible_v<InternalContainerT> )
            : derivedObject_( std::move( obj ) ) {}

        constexpr explicit StackContainer( const InternalContainerT& obj )
            noexcept( std::is_nothrow_copy_constructible_v<InternalContainerT> )
            : derivedObject_( obj ) {}

        constexpr StackContainer() noexcept : derivedObject_( ErrorType{} ) {}

        template<size_t currIdx>
        constexpr ParentT& getImpl( size_t idx );

        //! Internal stack container
        InternalContainerT derivedObject_;

        friend DerivedVariantsFabric<ParentT, DerivedTypes...>;
    };


    /*! Creates a new derived object (using input index) in the variant
     *  (on the stack memory (well, not always, but you understood what I mean)
     *
     * \details throw an exception if the index >= NderivedTypes
     */
    template<typename... Args>
    constexpr static StackContainer createElement( size_t derivedElemIdx, 
                                                   Args&&... args );

    /*! Creates a new derived object (using input index) in the variant
     *  (on the stack memory (well, not always, but you understood what I mean)
     */
    template<size_t derivedElemIdx, typename... Args>
    constexpr static StackContainer createElement( Args&&... args );

    /*! Creates an ErrorType in the variant.
     *  For this type StackContainer.isValid() gives you false.
     */
    constexpr static StackContainer createErrorElement();

private:
    //! This class is only for checking that ParentT is really the
    //! parent for DerivedTypes
    template<typename... Args>
    struct DerivedChecker
    {
        static constexpr bool value = true;
    };

    template<typename T, typename... Args>
    struct DerivedChecker<T, Args...>
    {
        static constexpr bool value = std::is_base_of_v<ParentT, T> &&
            DerivedChecker<Args...>::value;
    };

    static_assert( DerivedChecker<ParentT, DerivedTypes...>::value,
                   "First type (ParentT) should be the base for other "
                   "derived types (DerivedTypes)" );

    template<size_t currIdx, typename... Args>
    constexpr static StackContainer searchElement( size_t idx, 
                                                   Args&&... args );
};


// implementation -------------------------------------------------------------
#include "DerivedVariantsFabric.hpp"
