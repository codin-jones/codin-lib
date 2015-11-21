// ScopedWith.h
//
#ifndef CDN_SCOPED_WITH_INCLUDED
#define CDN_SCOPED_WITH_INCLUDED

//! The main namespace for the codin-lib
namespace cdn
{
//! misc related classes and utilities
namespace misc
{

//! \brief ScopedWith is a helper type for dealing with deeply nested structures
//!
//! The ScopedWith class is a simple utility class I came up with after having
//! to deal with some structures that were nested many levels deep. (Hopefully
//! you will never have to deal with this type of structure layout). I realize
//! it is potentially only useful in very few use cases, but it made the syntax
//! simpler for the accesses I need to perform.
//! 
//! Use the #makeScopedWith function for creation of ScopedWith objects, really
//! the best way is to use the #CDN_WITH(tt) or #CDN_WITH_AS(tt,nm) macros.
//!
template <typename TypeT>
class ScopedWith
{
public:

  //! Save the reference
  explicit
  ScopedWith (TypeT& tt)
    noexcept;

  //! = default
  ~ScopedWith () = default;

  //! = delete
  ScopedWith (const ScopedWith&) = delete;
  //! = delete
  ScopedWith& operator= (const ScopedWith&) = delete;

  //! = default
  //! This is the method that the assignment in the if command uses
  ScopedWith (ScopedWith&&) = default;
  //! = delete
  ScopedWith& operator= (ScopedWith&&) = delete;

  //! Return a pointer to the wrapped type
  TypeT*
  operator-> () 
    noexcept;
  
  //! operator bool() is required so the evaluation of the ScopedWith object
  //! within the if statement will return true after the assignment and the
  //! block will be executed
  operator bool() const
    noexcept;

private:
  TypeT& m_tt;
};

//! Helper make function to auto-detect TypeT
template <typename TypeT>
ScopedWith<TypeT>
makeScopedWith (TypeT& tt);

} // namespace misc
} // namespace cdn


//! Open a 'with' block that has a with accessor name of the nm argument
//! \param tt The type to create the with block on
//! \param nm The accessor variable name to be used within the with block
#define CDN_WITH_AS(tt,nm) if (auto nm = cdn::misc::makeScopedWith (tt))

//! Open a 'with' block that has a with accessor name of 'T'
//! \param tt The type to create the with block on
#define CDN_WITH(tt) CDN_WITH_AS(tt,T)       

#include "ScopedWith.icc"

#endif // #ifndef CDN_SCOPED_WITH_INCLUDED
