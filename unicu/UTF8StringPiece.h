/*
 * Copyright 2017 Facebook, Inc.
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

#pragma once

#include <boost/regex/pending/unicode_iterator.hpp>

#include "accelerator/Range.h"

namespace unicu {

template <
      class Iterator = const char*,
                class Base = acc::Range<boost::u8_to_u32_iterator<Iterator>>>
class UTF8Range : public Base {
 public:
  /* implicit */ UTF8Range(const acc::Range<Iterator> baseRange)
      : Base(boost::u8_to_u32_iterator<Iterator>(
                 baseRange.begin(), baseRange.begin(), baseRange.end()),
             boost::u8_to_u32_iterator<Iterator>(
                 baseRange.end(), baseRange.begin(), baseRange.end())) {}
  /* implicit */ UTF8Range(const std::string& baseString)
      : Base(acc::Range<Iterator>(baseString)) {}
};

using UTF8StringPiece = UTF8Range<const char*>;

} // namespace unicu
