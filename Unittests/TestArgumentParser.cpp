
#include "../ArgumentParser.h"
#include <gtest/gtest.h>

TEST(TestArgumentParser, RegisterFlag) {

  ArgumentParser argParser;
  EXPECT_FALSE(argParser.IsFlagRegistered("MyFlag"));
  argParser.RegisterFlag("MyFlag");
  EXPECT_TRUE(argParser.IsFlagRegistered("MyFlag"));
}

TEST(TestArgumentParser, RegisterFlag_WhiteSpace) {

  ArgumentParser argParser;
  argParser.RegisterFlag("My Flag");
  EXPECT_FALSE(argParser.IsFlagRegistered("My Flag"));
}