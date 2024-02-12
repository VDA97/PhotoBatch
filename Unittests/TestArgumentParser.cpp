
#include "../ArgumentParser.h"
#include <array>
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

TEST(TestArgumentParser, RegisterOption) {
  ArgumentParser argParser;
  argParser.RegisterOption("MyOption");
  EXPECT_TRUE(argParser.IsOptionRegistered("MyOption"));
}

TEST(TestArgumentParser, RegisterOption_WhiteSpace) {
  ArgumentParser argParser;
  argParser.RegisterOption("My Option");
  EXPECT_FALSE(argParser.IsOptionRegistered("My Option"));
}

TEST(TestArgumentParser, Parse) {
  ArgumentParser argParser;
  // Check if Parse will ignore "TestArgumentParser" which is neither option nor flag.
  std::array<const char *, 5> args = {
      "TestArgumentParser",
      "--flag",
      "--string=AaBbCcDd",
      "--number=42",
      "--float=4.2"};
  argParser.RegisterFlag("flag");
  argParser.RegisterOption("string");
  argParser.RegisterOption("number");
  argParser.RegisterOption("float");

  argParser.Parse(static_cast<int>(args.size()), args.data());
  EXPECT_FALSE(argParser.GetFlag("")); // It was used GetFlag instead IsFlagRegistered method because GetFlag is used by Parse method.
  EXPECT_FALSE(argParser.GetFlag("TestArgumentParser"));
  EXPECT_TRUE(argParser.GetFlag("flag"));
  EXPECT_EQ(argParser.GetOption("string"), "aabbccdd"); // Note that ToLower will be used A->a
  int i{0};                                             // Refactor this.
  EXPECT_EQ(argParser.GetOptionAs(i, "number"), 42);
  float j{0}; // Refactor this.
  EXPECT_EQ(argParser.GetOptionAs(j, "float"), 4.2f);
}

TEST(TestArgumentParser, Parse_InvalidArguments) {
  ArgumentParser argParser;
  argParser.Parse(0, nullptr);  // It is ok because we are verifying if argc > 1.
  argParser.Parse(42, nullptr); // Check how to debug using Visual Studio Code.
}