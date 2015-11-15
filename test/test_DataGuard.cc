// gtest_unittest.cc

#include "DataGuard.h"

#include <condition_variable>
#include <future>
#include <thread>

#include "gtest/gtest.h"


namespace
{

struct NoCopyHolder
{
  NoCopyHolder (const std::vector<int>& v)
    : iv (v)
  { }

  ~NoCopyHolder () = default;

  // disallow copy
  NoCopyHolder (const NoCopyHolder&) = delete;
  NoCopyHolder& operator= (const NoCopyHolder&) = delete;

  // allow move
  NoCopyHolder (NoCopyHolder&& rhs) = default;
  NoCopyHolder& operator= (NoCopyHolder&&) = default;

private: 
  std::vector<int> iv;
};

struct NoMoveHolder
{
  NoMoveHolder (const std::vector<int>& v)
    : iv (v)
  { }

  ~NoMoveHolder () = default;

  // allow copy
  NoMoveHolder (const NoMoveHolder&) = default;
  NoMoveHolder& operator= (const NoMoveHolder&) = default;

  // disallow move
  NoMoveHolder (NoMoveHolder&& rhs) = delete;
  NoMoveHolder& operator= (NoMoveHolder&&) = delete;

private: 
  std::vector<int> iv;
};

} // namespace

TEST(Int,DefaultConstructor)
{
  cdn::thread::DataGuard<int> dg;
}

TEST(Int,CopyConstructor)
{
  const int v = 5;
  cdn::thread::DataGuard<int> dg (v);
}

TEST(Int,MoveConstructor)
{
  int v = 23;
  cdn::thread::DataGuard<int> dg (std::move (v));
}

TEST(NCHolder,TemplateParamPack)
{
  std::vector<int> v { 12, 34, 56, 78, 90 };
  cdn::thread::DataGuard<NoCopyHolder> dg (v);
}

TEST(NMHolder,TemplateParamPack)
{
  std::vector<int> v { 12, 34, 56, 78, 90 };
  cdn::thread::DataGuard<NoMoveHolder> dg (v);
}

TEST(Int,OperatorReadAccess)
{
  cdn::thread::DataGuard<int> dg (1998);
  auto lock (cdn::thread::lockDataGuard (dg));
  EXPECT_EQ (1998, dg (lock));
}

TEST(Int,OperatorWriteAccess)
{
  cdn::thread::DataGuard<int> dg (12);
  auto lock (cdn::thread::lockDataGuard (dg));
  EXPECT_EQ (12, dg (lock));
  dg (lock) += 5;
  EXPECT_EQ (17, dg (lock));
}

TEST(Int,ConstOperatorAccess)
{
  const cdn::thread::DataGuard<int> dg (21);
  auto lock (cdn::thread::lockDataGuard (dg));
  EXPECT_EQ (21, dg (lock));
}

TEST(Int,GetReadAccess)
{
  cdn::thread::DataGuard<int> dg (2000);
  auto lock (cdn::thread::lockDataGuard (dg));
  EXPECT_EQ (2000, dg.get (lock));
}

TEST(Int,GetWriteAccess)
{
  cdn::thread::DataGuard<int> dg (24);
  auto lock (cdn::thread::lockDataGuard (dg));
  EXPECT_EQ (24, dg.get (lock));
  dg.get (lock) += 18;
  EXPECT_EQ (42, dg.get (lock));
}

TEST(Int,ConstGetAccess)
{
  const cdn::thread::DataGuard<int> dg (77);
  auto lock (cdn::thread::lockDataGuard (dg));
  EXPECT_EQ (77, dg.get (lock));
}

// since we should NEVER call lock/unlock explictly don't test them
// *unique_lock calls them for us anyway*

TEST(Int,InvalidLock)
{
  cdn::thread::DataGuard<int> good (4096);
  cdn::thread::DataGuard<int> bad  (4000);

  auto goodLock = cdn::thread::lockDataGuard (good);
  auto badLock  = cdn::thread::lockDataGuard (bad);

  EXPECT_EQ (4096, good (goodLock));
  EXPECT_THROW (good (badLock), std::system_error);
}

TEST(Int,CondVarAny)
{
  cdn::thread::DataGuard<int> dg (1975);
  std::condition_variable_any cond;

  auto fut = std::async (std::launch::async,
                         [&] 
                         { 
                           std::cout << "Notify thread delaying..." << std::endl;
                           std::this_thread::sleep_for (std::chrono::milliseconds (850));
                           std::cout << "Notify thread calling notify_one..." << std::endl;
                           cond.notify_one ();
                         });
  
  auto lock = cdn::thread::lockDataGuard (dg);
  std::cout << "Waiting..." << std::endl;
  // A proper wait would use a predicate to ignore spurious signals
  cond.wait (dg);
  std::cout << "Signaled!" << std::endl;
  
  fut.get ();
}
