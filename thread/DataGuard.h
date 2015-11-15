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

  //! deleted
  DataGuard (const DataGuard&) = delete;
  //! deleted
  DataGuard& operator= (const DataGuard&) = delete;

  //! deleted
  DataGuard (DataGuard&&) = delete;
  //! deleted
  DataGuard& operator= (DataGuard&&) = delete;


  //
  // Datum accessors.
  // The accessors operator() and get() will only throw std::system_error
  // if the lock supplied is not associated with this DataGuard
  //

  //! const accessor
  const Datum&
  operator () (const std::unique_lock<const DataGuard<Datum, Mutex>>& lock)
    const
    throw (std::system_error);

  //! non-const accessor
  Datum&
  operator () (const std::unique_lock<DataGuard<Datum, Mutex>>& lock)
    throw (std::system_error);

  //! const accessor
  const Datum&
  get (const std::unique_lock<const DataGuard<Datum, Mutex>>& lock)
    const
    throw (std::system_error);

  //! non-const accessor
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

//! \page cdn_thread_DataGuard Example usage of the DataGuard
//! Simple DataGuard usage:
//! \code
//!
//! cdn::thread::DataGuard<int> dg;
//!
//! // Lock the DataGuard object using the helper function
//! // The lock is of type std::unique_lock
//! auto lock (std::thread::lockDataGuard (dg));
//!
//! // You can access the int wrapped by the DataGuard by using the operator()
//! // or the get() method, both are overloaded for read/write access of Datum
//!
//! int v = dg (lock);
//! 
//! int v2 = dg.get (lock);
//! 
//! \endcode

//! \page cdn_thread_DataGuard_cond Example usage of std::condition_variable_any and DataGuard
//! You can use a std::condition_variable::any with DataGuard:
//! \code
//!
//! cdn::thread::DataGuard<int> dg (1975);
//! std::condition_variable_any cond;
//! 
//! auto fut = std::async (std::launch::async,
//!                        [&] 
//!                        { 
//!                          std::cout << "Notify thread delaying..." << std::endl;
//!                          std::this_thread::sleep_for (std::chrono::milliseconds (850));
//!                          std::cout << "Notify thread calling notify_one..." << std::endl;
//!                          cond.notify_one ();
//!                        });
//! 
//! auto lock = cdn::thread::lockDataGuard (dg);
//! std::cout << "Waiting..." << std::endl;
//! // A proper wait would use a predicate to ignore spurious signals
//! cond.wait (dg);
//! std::cout << "Signaled!" << std::endl;
//! 
//! fut.get ();
//! 
//! \endcode

//
// Simple helper template functions to allow for simpler/cleaner syntax to lock
// the DataGuard object
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


//! template typedef for std::recursive_mutex
template <typename Datum>
using RecursiveDataGuard = DataGuard<Datum, std::recursive_mutex>; 

} // namespace thread
} // namespace cdn

#include "DataGuard.icc"

#endif // #ifndef CDN_DATA_GUARD_INCLUDED
