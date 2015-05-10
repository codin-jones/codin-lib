// DataGuard.h
//
#ifndef CDN_DATA_GUARD_INCLUDED
#define CDN_DATA_GUARD_INCLUDED

#include <mutex>
#include <system_error>

//! The main namespace for the codin-lib
namespace cdn
{
//! Thread related classes and utilities
namespace thread
{

//!
//! The DataGuard class associates a mutex with the data it is providing
//! synchronized access to. Additionally the DataGuard requires that the mutex
//! be locked to gain access to the wrapped data. DataGuard does not support 
//! std::lock_guard<> because there is no method to validate the mutex instance 
//! that the std::lock_guard<> has locked.
//!
//! All the standard Mutex types adhere to the Mutex Concept
//! (http://en.cppreference.com/w/cpp/concept/Mutex) which means the Mutex type
//! is not copyable or movable therefore DataGuard will not be copyable or movable.
//! 
//! If a custom Mutex type is used it must meet the BasicLockable concept 
//! requirements (http://en.cppreference.com/w/cpp/concept/BasicLockable)
//!
template <typename Datum, typename Mutex = std::mutex>
class DataGuard final
{
public:
  //! Typedef for wrapped type 
  typedef Datum value_type;
  //! Typedef for mutex type  
  typedef Mutex mutex_type;


  //! The Datum default constructor will be executed
  DataGuard ();

  //! The Datum copy constructor will be executed
  DataGuard (const Datum& datum);

  //! The Datum move constructor will be executed
  DataGuard (Datum&& datum);

  //! Construct the Datum with the supplied template parameter pack
  template <typename... Args> 
  DataGuard (Args&&... args);

  ~DataGuard () = default;

  // disallow copy
  DataGuard (const DataGuard&) = delete;
  DataGuard& operator= (const DataGuard&) = delete;

  // disallow move
  DataGuard (DataGuard&&) = delete;
  DataGuard& operator= (DataGuard&&) = delete;


  //
  // Datum accessors.
  // The accessors operator() and get() will only throw std::system_error
  // if the lock supplied is not associated with this DataGuard
  //

  const Datum&
  operator () (const std::unique_lock<const DataGuard<Datum, Mutex>>& lock)
    const
    throw (std::system_error);

  Datum&
  operator () (const std::unique_lock<DataGuard<Datum, Mutex>>& lock)
    throw (std::system_error);

  const Datum&
  get (const std::unique_lock<const DataGuard<Datum, Mutex>>& lock)
    const
    throw (std::system_error);

  Datum&
  get (const std::unique_lock<DataGuard<Datum, Mutex>>& lock)
    throw (std::system_error);


  //! Required method to meet BasicLockable concept
  //! (http://en.cppreference.com/w/cpp/concept/BasicLockable)
  //!
  //! lock is usually not called directly, use std::unique_lock to manage
  //! exclusive scope based locking. 
  void
  lock ()
    const
    throw (std::system_error);
  
  //! Required method to meet BasicLockable concept
  //! (http://en.cppreference.com/w/cpp/concept/BasicLockable)
  //!
  //! unlock is usually not called directly, use std::unique_lock to manage
  //! exclusive scope based locking. 
  void
  unlock ()
    const
    noexcept;

private:

  Mutex m_mutex;
  Datum m_datum;
};


//
// Simple helper temlate functions to allow for cleaner syntax to lock the
// DataGuard object
//

//! \page cdn_thread_lockDataGuardExample Example usage of the lockDataGuard method
//! Locking DataGuard without using the lockDataGuard method:
//! \code
//! cdn::thread::DataGuard<int> dg;
//! std::unique_lock<cdn::thread::DataGuard<int>> lock (dg);
//! \endcode
//! 
//! Locking DataGuard using the lockDataGuard method:
//! \code
//! cdn::thread::DataGuard<int> dg;
//! auto lock (std::thread::lockDataGuard (dg));
//! \endcode
//! 

//! lock helper function for simple locking (const variant)
//! \ref cdn_thread_lockDataGuardExample
template <typename Datum, typename Mutex = std::mutex>
inline
std::unique_lock <const DataGuard<Datum,Mutex>>
lockDataGuard (const DataGuard<Datum,Mutex>& dataGuard);

//! lock helper function for simple locking (non-const variant)
//! \ref cdn_thread_lockDataGuardExample
template <typename Datum, typename Mutex = std::mutex>
inline
std::unique_lock <DataGuard<Datum,Mutex>>
lockDataGuard (DataGuard<Datum,Mutex>& dataGuard);


} // namespace thread
} // namespace cdn

#include "DataGuard.icc"

//! template typedef for std::recursive_mutex
template <typename Datum>
using RecursiveDataGuard = DataGuard<Datum, std::recursive_mutex>; 

#endif // #ifndef CDN_DATA_GUARD_INCLUDED
