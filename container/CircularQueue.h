// CircularQueue.h
// 
#ifndef CDN_CIRCULAR_QUEUE_INCLUDED
#define CDN_CIRCULAR_QUEUE_INCLUDED

#include <array>
#include <condition_variable>
#include <functional>
#include <ostream>
#include <string>

// TODO: dependency on boost
#include "boost/optional.hpp"

// TODO: dependency on DataGuard
#include "DataGuard.h"


//! The main namespace for the codin-lib
namespace cdn
{
//! Container related classes and utilities
namespace container
{

//! CircularQueue encounterd a fatal error, likely caused by a mutex error
class CircularQueueError
  : public std::runtime_error 
{ 
public:
  CircularQueueError (const std::string&);
};

//! CircularQueue has been shutdown
class CircularQueueShutdown
  : public std::runtime_error
{
public:
  CircularQueueShutdown ();
};

//! The CircularQueueMode enum controls the behavior of the CircularQueue when
//! it reaches a queue full state.
enum class CircularQueueMode 
{
  FailOnWrite,     /*!< Writer will not overwrite an element that has not been
                        read yet. If the queue is full and in this mode, the 
                        exception CircularQueueError will be raised
                   */
 
  BlockOnWrite,    /*!< Writer will not overwrite an element that has not been
                        read yet. If the queue is full and in this mode the 
                        push or emplace call will block and wait until there
                        is room for the element
                   */ 

  NonBlockingWrite /*!< Writer will overwrite an element that has not been read
                        yet
                   */
};

//! \brief The CircularQueue class provides a thread-safe queue based upon std::array<>
//!
//! This queue orders elements FIFO (first-in-first-out). The front/head of the
//! queue is that element that has been on the queue the longest time. The 
//! back/tail of the queue is that element that has been on the queue the 
//! shortest time. New elements are inserted via push/emplace at the tail of the
//! queue, and the queue retrieval operations via pop obtain elements from the 
//! head of the queue. 
//!
//! At a minimum T must meet the requirements of 
//! <a href="http://en.cppreference.com/w/cpp/concept/DefaultConstructible">DefaultConstructible</a>, 
//! <a href="http://en.cppreference.com/w/cpp/concept/CopyConstructible">CopyConstructible</a> and 
//! <a href="http://en.cppreference.com/w/cpp/concept/CopyAssignable">CopyAssignable</a>.
//! The individual methods that require additional concepts are documented on those methods.
//!              
template <typename T, std::size_t N>
class CircularQueue
{
public:

  //! Default initialize all the elements in the array
  CircularQueue (const CircularQueueMode&)
    throw (CircularQueueError);

  //! Initialize all the elements in the array with a copy of initialValue
  explicit 
  CircularQueue (const CircularQueueMode&,
                 const T& initialValue)
    throw (CircularQueueError);

  //! = default
  ~CircularQueue () = default;

  //! = delete
  CircularQueue (const CircularQueue&) = delete;
  //! = delete
  CircularQueue& operator= (const CircularQueue&) = delete;

  //! = delete
  CircularQueue (CircularQueue&&) = delete;
  //! = delete
  CircularQueue& operator= (CircularQueue&&) = delete;

  //! Return true if there are no elements available to be popped
  //!
  //! \throw CircularQueueError Raise CircularQueueError on mutex error
  bool
  isEmpty ()
    const
    throw (CircularQueueError);

  //! Number of elements available to be popped from the queue
  //!
  //! \throw CircularQueueError Raise CircularQueueError on mutex error
  std::size_t
  size ()
    const
    throw (CircularQueueError);

  //! The std::size_t that the Queue was allocated with, which is the 
  //! maximum number of element the Queue can hold
  //!
  //! noexcept
  std::size_t
  max ()
    const
    noexcept;

  //! Tell the queue to shutdown, this will force any blocking pops to return
  //!
  //! \throw CircularQueueError Raise CircularQueueError on mutex error
  void
  shutdown ()
    throw (CircularQueueError);

  //! Return true if the queue has been shutdown
  //!
  //! \throw CircularQueueError Raise CircularQueueError on mutex error
  bool
  isShutdown ()
    const
    throw (CircularQueueError);

  // TODO: clear method
	
  // TODO: test class that supports move assign but not move const

  //! Move the element onto the queue, potentially waiting for space based upon
  //! the mode
  //!
  //! T must support
  //! <a href="http://en.cppreference.com/w/cpp/concept/MoveAssignable">MoveAssignable</a> 
  //!
  //! \throw CircularQueueError Raise CircularQueueError on mutex error or if 
  //! the T move assignment operator throws
  //! \throw CircularQueueShutdown Raise CircularQueueShutdown if the queue has
  //! been shutdown
  template <typename... Args>
  void
  emplace (Args&&... args)
    throw (CircularQueueError, CircularQueueShutdown);

  //! Copy the element onto the queue, potentially waiting for space based upon
  //! the mode.
  //!
  //! \throw CircularQueueError Raise CircularQueueError on mutex error or if 
  //! the T copy assignment operator throws
  //! \throw CircularQueueShutdown Raise CircularQueueShutdown if the queue has
  //! been shutdown
  void
  push (const T&)
    throw (CircularQueueError, CircularQueueShutdown);

  //! Attempt to pop the front of the queue and return a copy of it, waiting
  //! forever if the queue contains no elements
  //!
  //! \throw CircularQueueError Raise CircularQueueError on mutex error or if 
  //! the T copy constructor throws
  //! \throw CircularQueueShutdown Raise CircularQueueShutdown if the queue has
  //! been shutdown
  const T
  pop ()
    throw (CircularQueueError, CircularQueueShutdown);

  //! Attempt to pop the front of the queue and return a copy of it, if there
  //! are no available elements before the timeout expires an 'empty' 
  //! optional<T> will be returned.
  //!
  //! \throw CircularQueueError Raise CircularQueueError on mutex error or if 
  //! the T copy constructor throws
  //! \throw CircularQueueShutdown Raise CircularQueueShutdown if the queue has
  //! been shutdown
  template <typename Rep, typename Period>
  boost::optional<const T>
  pop (const std::chrono::duration<Rep, Period>& rel_time)
    throw (CircularQueueError, CircularQueueShutdown);


  //! For debug purposes only
  //! The container T type must have a stream insertion operator defined in the
  //! global namespace to use this function
  void dump (std::ostream&);

private:

  struct Bookkeeping;
  typedef thread::RecursiveDataGuard<Bookkeeping> Guard;

  void 
  insert (std::function<void(std::unique_lock<Guard>&, std::size_t)>)
    throw (CircularQueueError, CircularQueueShutdown);

  boost::optional<const T>
  popImpl (std::function<bool(std::unique_lock<Guard>&)>)
    throw (CircularQueueError, CircularQueueShutdown);

  std::size_t 
  nextIndex (std::size_t)
    noexcept;

  //! \brief Internal type for the state data
  struct Bookkeeping
  {
    Bookkeeping (const CircularQueueMode& mode_)
      noexcept
      : mode (mode_),
        isShutdown (false), 
        nextReadIndex (0),
        nextWriteIndex (0),
        isEmpty (true)
    { }

    ~Bookkeeping () = default;

    Bookkeeping (const Bookkeeping&) = default;
    Bookkeeping& operator= (const Bookkeeping&) = default;

    Bookkeeping (Bookkeeping&&) = default;
    Bookkeeping& operator= (Bookkeeping&&) = default;


    CircularQueueMode mode;
    bool              isShutdown;
    std::size_t       nextReadIndex;
    std::size_t       nextWriteIndex;
    // The isEmpty flag is required in addition to the read/write indicies due to 
    // the fact they could be euqal but the queue could be either empty or full
    bool              isEmpty;
    std::array<T,N>   m_buffer;
  };

  mutable Guard               m_bookkeeping;
  std::condition_variable_any m_cond;
};

} // namespace container
} // namespace cdn

#include "CircularQueue.icc"

#endif // #ifndef CDN_CIRCULAR_QUEUE_INCLUDED
