/*
 * Copyright 2017 Yeolar
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>

#include "unicu/Unicode.h"

using namespace unicu;

TEST(unicode, isWide) {
  for (char32_t c = 0; c < 0xff; c++) {
    if (isprint(c)) {
      EXPECT_TRUE(!unicode::isWide(c));
    }
  }
  const char* s = "ＲＤ文档";
  for (auto it = Utf8CharIterator(s), end = Utf8CharIterator(s + strlen(s));
       it != end; it++) {
    EXPECT_TRUE(unicode::isWide(*it));
  }
}

TEST(Util, cutText) {
  static const char* const text =
    "和大多数Web框架一样，Tornado的一个重要目标就是帮助你更快地编写程序，"
    "尽可能整洁地复用更多的代码。";
  size_t i = 0;
  EXPECT_STREQ(cutText(text, i, 0, 10).c_str(), "和大多数Web...");
  EXPECT_STREQ(cutText(text, i, 0, 11).c_str(), "和大多数Web...");
  EXPECT_STREQ(cutText(text, i, 0, 12).c_str(), "和大多数Web框...");
  EXPECT_STREQ(cutText(text, i, 0, 13).c_str(), "和大多数Web框...");
  EXPECT_STREQ(cutText(text, i, 0, 14).c_str(), "和大多数Web框架...");
  EXPECT_STREQ(cutText(text, i, 1, 10).c_str(), "和大多数Web...");
  EXPECT_STREQ(cutText(text, i, 2, 10).c_str(), "和大多数Web...");

  i = acc::StringPiece(text).find("Tornado");
  EXPECT_STREQ(cutText(text, i, 0, 10).c_str(), "...Tornado的一...");
  EXPECT_STREQ(cutText(text, i, 1, 10).c_str(), "...，Tornado的...");
  EXPECT_STREQ(cutText(text, i, 2, 10).c_str(), "...，Tornado的...");
  EXPECT_STREQ(cutText(text, i, 3, 10).c_str(), "...样，Tornado...");
  EXPECT_STREQ(cutText(text, i, 4, 10).c_str(), "...样，Tornado...");

  i = acc::StringPiece(text).find("一个");
  EXPECT_STREQ(cutText(text, i, 0, 10).c_str(), "...一个重要目...");
  EXPECT_STREQ(cutText(text, i, 1, 10).c_str(), "...的一个重要...");
  EXPECT_STREQ(cutText(text, i, 2, 10).c_str(), "...的一个重要...");
  EXPECT_STREQ(cutText(text, i, 3, 10).c_str(), "...Tornado的一...");
  EXPECT_STREQ(cutText(text, i, 4, 10).c_str(), "...Tornado的一...");
}
