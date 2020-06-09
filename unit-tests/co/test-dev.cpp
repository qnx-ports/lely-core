/**@file
 * This file is part of the CANopen Library Unit Test Suite.
 *
 * @copyright 2020 N7 Space Sp. z o.o.
 *
 * Unit Test Suite was developed under a programme of,
 * and funded by, the European Space Agency.
 *
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

#include <CppUTest/TestHarness.h>

#include <config.h>
#include <lely/co/dev.h>
#include <lely/util/errnum.h>

TEST_GROUP(CO_DevInit){};

TEST(CO_DevInit, CODevAllocFree) {
  void* const ptr = __co_dev_alloc();

  CHECK(ptr != nullptr);

  __co_dev_free(ptr);
}

TEST(CO_DevInit, CODevInit) {
  auto* const dev = static_cast<co_dev_t*>(__co_dev_alloc());

  CHECK(dev != nullptr);
  POINTERS_EQUAL(dev, __co_dev_init(dev, 0x01));

  __co_dev_fini(dev);
  __co_dev_free(dev);
}

TEST(CO_DevInit, CODevInit_MaxId) {
  auto* const dev = static_cast<co_dev_t*>(__co_dev_alloc());

  CHECK(dev != nullptr);
  POINTERS_EQUAL(dev, __co_dev_init(dev, 0xff));

  __co_dev_fini(dev);
  __co_dev_free(dev);
}

TEST(CO_DevInit, CODevInit_ZeroId) {
  auto* const dev = static_cast<co_dev_t*>(__co_dev_alloc());

  CHECK(dev != nullptr);
  POINTERS_EQUAL(nullptr, __co_dev_init(dev, 0x00));

  __co_dev_free(dev);
}

TEST(CO_DevInit, CODevInit_InvalidId) {
  auto* const dev = static_cast<co_dev_t*>(__co_dev_alloc());
  CHECK(dev != nullptr);

  POINTERS_EQUAL(nullptr, __co_dev_init(dev, CO_NUM_NODES + 1));
  CHECK_EQUAL(EINVAL, get_errc());

  POINTERS_EQUAL(nullptr, __co_dev_init(dev, 0xff - 1));
  CHECK_EQUAL(EINVAL, get_errc());

  __co_dev_free(dev);
}

TEST(CO_DevInit, CODevFini) {
  auto* const dev = static_cast<co_dev_t*>(__co_dev_alloc());

  CHECK(dev != nullptr);
  POINTERS_EQUAL(dev, __co_dev_init(dev, 0x01));

  __co_dev_fini(dev);
  __co_dev_free(dev);
}

TEST(CO_DevInit, CODevDestroy_Null) { co_dev_destroy(nullptr); }

TEST_GROUP(CO_Dev) {
  co_dev_t* dev = nullptr;

  TEST_SETUP() {
    dev = co_dev_create(0x01);
    CHECK(dev != nullptr);
  }
};
