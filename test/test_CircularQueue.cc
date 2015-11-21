// test_CircularQueue.h

#define CIRCULAR_QUEUE_DEBUG 1

#include "CircularQueue.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "gtest/gtest.h"

namespace
{

class NoMove
{
public:
  NoMove () // default constructor required
    : m_idx (-1)
  { }

  NoMove (int idx)
    : m_idx (idx)
  { }

  ~NoMove () = default;

  // allow copy
  NoMove (const NoMove&) = default;
  NoMove& operator= (const NoMove&) = default;

  // disallow move
  NoMove (NoMove&&) = delete;
  NoMove& operator= (NoMove&&) = delete;

  int
  idx ()
    const
  { return m_idx; }

private:
  int m_idx;
};

std::ostream&
operator<< (std::ostream& strm, const NoMove& ndc)
{
  return strm << "NoMove.idx=" << ndc.idx ();
}

} // namespace


TEST(Int,FailModeConstructor)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::FailOnWrite);
}

TEST(Int,BlockModeConstructor)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::BlockOnWrite);
}

TEST(Int,NonBlockingModeConstructor)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::NonBlockingWrite);
}

TEST(Int,InitValFailModeConstructor)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::FailOnWrite, 7);
}

TEST(Int,InitValBlockModeConstructor)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::BlockOnWrite, 21);
}

TEST(Int,InitValNonBlockingModeConstructor)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::NonBlockingWrite, 18);
}

TEST(Int,Empty)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::BlockOnWrite);
  EXPECT_EQ (cq.isEmpty (), true);
}

TEST(Int,Size)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::BlockOnWrite);
  EXPECT_EQ (cq.size (), 0UL);
}

TEST(Int,Max)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::BlockOnWrite);
  EXPECT_EQ (cq.max (), 5UL);
}

TEST(Int,IsShutdown)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::BlockOnWrite);
  EXPECT_EQ (cq.isShutdown (), false);
}

TEST(Int,CoupleEntriesEmpty)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::BlockOnWrite);
  cq.push (42);
  cq.push (24);
  EXPECT_EQ (cq.isEmpty (), false);
}

TEST(Int,CoupleEntriesSize)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::BlockOnWrite);
  cq.push (42);
  cq.push (24);
  EXPECT_EQ (cq.size (), 2UL);
}

TEST(Int,Full)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::BlockOnWrite);
  
  for (int i=0; i < 5; ++i)
  {
    cq.push (51 + i);
  }
  
  cq.dump (std::cout);
  EXPECT_EQ (cq.size (), 5UL);
}

TEST(Int,Wraparound)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::BlockOnWrite);
  std::cout << "filling buffer..." << std::endl;
  for (int i=0; i < 5; ++i)
  {
    cq.push (69 + i);
  }
  std::cout << "filled buffer..." << std::endl;
  
  cq.dump (std::cout);
  
  std::cout << "read: " << cq.pop () << std::endl;
  std::cout << "read: " << cq.pop () << std::endl;
  
  cq.push (74);
  
  cq.dump (std::cout);
  
  for (int i=0; i < 2; ++i)
  {
    std::cout << "read: " << cq.pop () << std::endl;
    cq.push (75 + i);
  }
  
  cq.dump (std::cout);
  EXPECT_EQ (cq.size (), 4UL);
}

TEST(Int,ReadWriteSameAmount)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::BlockOnWrite);
  
  cq.push (77);
  
  for (int i=0; i < 3; ++i)
  {
    cq.push (78 + i);
    std::cout << "read: " << cq.pop () << std::endl;
  }
  
  std::cout << "read: " << cq.pop () << std::endl;
  
  cq.dump (std::cout);
  EXPECT_EQ (cq.isEmpty (), true);
}

TEST(Int,WraparoundReadWriteSameAmount)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::BlockOnWrite);
  
  for (int i=0; i < 8; ++i)
  {
    cq.push (44 + i);
    std::cout << "read: " << cq.pop () << std::endl;
  }
  
  cq.dump (std::cout);
  EXPECT_EQ (cq.size (), 0UL);
}

TEST(Int,Emplace)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::BlockOnWrite);
  
  int i1 = 99;
  int i2 = 88;
  int i3 = 77;
  
  cq.emplace (i1);
  cq.emplace (i2);
  cq.emplace (i3);
  
  cq.dump (std::cout);
  EXPECT_EQ (cq.size (), 3UL);
}

TEST(Int,FailOnFull)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::FailOnWrite);
      
  for (int i=0; i < 5; ++i)
  {
    cq.push (i + 13);
  }
  
  cq.dump (std::cout);

  EXPECT_THROW (cq.push (99), cdn::container::CircularQueueError);

  cq.dump (std::cout);
}

TEST(Int,NonBlocking)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::NonBlockingWrite);
  
  for (int i=0; i < 5; ++i)
  {
    cq.push (i + 23);
  }

  cq.dump (std::cout);

  cq.push (101);

  // if we get here we are successful
  EXPECT_EQ (cq.isEmpty (), false);

  cq.dump (std::cout);

  cq.push (102);

  // ditto push in one more
  EXPECT_EQ (cq.isEmpty (), false);

  cq.dump (std::cout);
}

TEST(Int,BlockOnWrite)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::BlockOnWrite);
  
  for (int i=0; i < 5; ++i)
  {
    cq.push (i + 98);
  }
  
  cq.dump (std::cout);
  
  std::cout << "read 2 - " << cq.pop () << " " << cq.pop () << std::endl;
  
  cq.dump (std::cout);
  
  std::cout << "write 2..." << std::endl;
  
  cq.push (222);
  cq.push (223);
  
  cq.dump (std::cout);
  
  std::cout << "size=" << cq.size () << std::endl;
  
  // kick off a thread that will read to allow the writer to finish
  std::thread t ([&] 
                 {
                   std::cout << "thread delaying..." << std::endl;
                   std::this_thread::sleep_for (std::chrono::milliseconds (1650));
                   std::cout << "thread read=" << cq.pop () << std::endl;
                 });
  
  std::cout << "about to push '224' which will wait for room..." << std::endl;
  
  cq.push (224);
  
  t.join ();
  
  cq.dump (std::cout);
}

TEST(Int,PopTimeout)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::BlockOnWrite);
  
  boost::optional<const int> v = cq.pop (std::chrono::milliseconds (1800));
  
  EXPECT_TRUE (! v);
}

TEST(Int,BlockedRead)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::BlockOnWrite);
  
  // kick off a thread that will read to allow the writer to finish
  std::thread t ([&] 
                 {
                   std::cout << "thread delaying..." << std::endl;
                   std::this_thread::sleep_for (std::chrono::milliseconds (2250));
                   std::cout << "thread pushing value..." << std::endl;
                   cq.push (1118);
                 });
  
  boost::optional<const int> v = cq.pop (std::chrono::seconds (3));
  
  EXPECT_FALSE (! v);

  std::cout << "Expected result pop value=" << v.get () << std::endl;
  
  t.join ();
}

TEST(Int,PopShutdown)
{
  cdn::container::CircularQueue<int, 5> cq (cdn::container::CircularQueueMode::BlockOnWrite);
  
  // kick off a thread that will shutdown after a delay
  std::thread t ([&] 
                 {
                   std::cout << "thread delaying..." << std::endl;
                   std::this_thread::sleep_for (std::chrono::milliseconds (850));
                   std::cout << "shutting down queue..." << std::endl;
                   cq.shutdown ();
                 });
  
  EXPECT_THROW (cq.pop (), cdn::container::CircularQueueShutdown);

  t.join ();
}

TEST(NoMove,PushPop)
{
  NoMove initVal (1001);
  cdn::container::CircularQueue<NoMove, 7> cq (cdn::container::CircularQueueMode::BlockOnWrite,
                                     initVal);
  
  cq.dump (std::cout);
  
  cq.push (NoMove (41));
  cq.push (NoMove (42));
  cq.push (NoMove (43));
  
  cq.dump (std::cout);
  
  NoMove v = cq.pop ();
  
  std::cout << "popped value=" << v.idx () << std::endl;
  
  cq.dump (std::cout);
}
