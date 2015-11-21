// test_ScopedWith.cc

#include "ScopedWith.h"

#include "gtest/gtest.h"

namespace
{
  struct level_three
  {
    int index;
  };

  struct level_two
  {
    float offset;
    level_three lvlThree;
  };

  struct level_one
  {
    std::string name;
    level_two lvlTwo;
  };

  struct outer
  {
    long count;
    level_one lvlOne;
  };
} // namespace

// Test case showing access where structs are only nested two levels deep
TEST(Canonical,With)
{
  level_two lt = { 42.24, { 77 } };

  std::cout << "level_three.index=" << lt.lvlThree.index << std::endl;

  CDN_WITH(lt.lvlThree)
  {
    std::cout << "T->index=" << T->index << std::endl;
    EXPECT_EQ (T->index, lt.lvlThree.index);
  }
}

// Test case showing access where structs are multi levels deep
// Also showing how to get unique names instead of default 'T'
TEST(Canonical,WithAs)
{
  outer otr { 99, { "test-string", { 42.24, { 77 } } } };

  CDN_WITH_AS(otr, o)
  {
    std::cout << "outer.count=" << o->count << std::endl;

    CDN_WITH_AS (o->lvlOne, one)
    {
      CDN_WITH_AS (one->lvlTwo.lvlThree, three)
      {
        std::cout << "level_three.index=" << three->index << std::endl;
      }
    }

    EXPECT_EQ (o->count, 99);
  }
}



