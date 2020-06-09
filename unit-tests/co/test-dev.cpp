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
#include <lely/co/obj.h>
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

  CHECK_EQUAL(0, co_dev_get_netid(dev));
  CHECK_EQUAL(0x01, co_dev_get_id(dev));

  CHECK_EQUAL(0, co_dev_get_idx(dev, 0, nullptr));

  POINTERS_EQUAL(nullptr, co_dev_get_name(dev));

  POINTERS_EQUAL(nullptr, co_dev_get_vendor_name(dev));
  CHECK_EQUAL(0, co_dev_get_vendor_id(dev));
  POINTERS_EQUAL(nullptr, co_dev_get_product_name(dev));
  CHECK_EQUAL(0, co_dev_get_product_code(dev));
  CHECK_EQUAL(0, co_dev_get_revision(dev));
  POINTERS_EQUAL(nullptr, co_dev_get_order_code(dev));

  CHECK_EQUAL(0, co_dev_get_baud(dev));
  CHECK_EQUAL(0, co_dev_get_rate(dev));

  CHECK_EQUAL(0, co_dev_get_lss(dev));

  CHECK_EQUAL(0, co_dev_get_dummy(dev));

  __co_dev_fini(dev);
  __co_dev_free(dev);
}

TEST(CO_DevInit, CODevInit_UnconfiguredId) {
  auto* const dev = static_cast<co_dev_t*>(__co_dev_alloc());

  CHECK(dev != nullptr);
  POINTERS_EQUAL(dev, __co_dev_init(dev, 0xff));

  co_obj_t* obj1 = co_obj_create(0x0000);
  co_obj_t* obj2 = co_obj_create(0x0001);
  co_obj_t* obj3 = co_obj_create(0xffff);
  CHECK(obj1 != nullptr);
  CHECK(obj2 != nullptr);
  CHECK(obj3 != nullptr);
  co_dev_insert_obj(dev, obj1);
  co_dev_insert_obj(dev, obj2);
  co_dev_insert_obj(dev, obj3);

  CHECK_EQUAL(0, co_dev_set_name(dev, "name"));
  CHECK_EQUAL(0, co_dev_set_vendor_name(dev, "vendor"));
  CHECK_EQUAL(0, co_dev_set_product_name(dev, "product name"));
  CHECK_EQUAL(0, co_dev_set_order_code(dev, "order code"));

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
  CHECK_EQUAL(ERRNUM_INVAL, get_errnum());

  POINTERS_EQUAL(nullptr, __co_dev_init(dev, 0xff - 1));
  CHECK_EQUAL(ERRNUM_INVAL, get_errnum());

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

  TEST_TEARDOWN() { co_dev_destroy(dev); }
};

TEST(CO_Dev, CoDevSetNetId) {
  const auto ret = co_dev_set_netid(dev, 0x3d);

  CHECK_EQUAL(0, ret);
  CHECK_EQUAL(0x3d, co_dev_get_netid(dev));
}

TEST(CO_Dev, CoDevSetNetId_Unconfigured) {
  const auto ret = co_dev_set_netid(dev, 0xff);

  CHECK_EQUAL(0, ret);
  CHECK_EQUAL(0xff, co_dev_get_netid(dev));
}

TEST(CO_Dev, CoDevSetNetId_InvalidId) {
  const auto ret1 = co_dev_set_netid(dev, CO_NUM_NETWORKS + 1);

  CHECK_EQUAL(-1, ret1);
  CHECK_EQUAL(0, co_dev_get_netid(dev));

  const auto ret2 = co_dev_set_netid(dev, 0xff - 1);

  CHECK_EQUAL(-1, ret2);
  CHECK_EQUAL(0, co_dev_get_netid(dev));
}

TEST(CO_Dev, CoDevSetId) {
  const auto ret = co_dev_set_id(dev, 0x3d);

  CHECK_EQUAL(0, ret);
  CHECK_EQUAL(0x3d, co_dev_get_id(dev));
}

TEST(CO_Dev, CoDevSetId_CheckObj) {
  co_obj_t* obj1 = co_obj_create(0x0000);
  co_obj_t* obj2 = co_obj_create(0x0001);
  co_obj_t* obj3 = co_obj_create(0xffff);
  CHECK(obj1 != nullptr);
  CHECK(obj2 != nullptr);
  CHECK(obj3 != nullptr);
  co_dev_insert_obj(dev, obj1);
  co_dev_insert_obj(dev, obj2);
  co_dev_insert_obj(dev, obj3);

  const auto ret = co_dev_set_id(dev, 0x3d);

  CHECK_EQUAL(0, ret);
  CHECK_EQUAL(0x3d, co_dev_get_id(dev));
  for(co_obj_t* obj = co_dev_first_obj(dev); obj != nullptr; obj = co_obj_next(obj)) {
    // TODO: check if sub-object values are adjusted
  }
}


TEST(CO_Dev, CoDevSetId_Unconfigured) {
  const auto ret = co_dev_set_id(dev, 0xff);

  CHECK_EQUAL(0, ret);
  CHECK_EQUAL(0xff, co_dev_get_id(dev));
}

TEST(CO_Dev, CoDevSetId_InvalidId) {
  const auto ret1 = co_dev_set_id(dev, CO_NUM_NETWORKS + 1);

  CHECK_EQUAL(-1, ret1);
  CHECK_EQUAL(0x01, co_dev_get_id(dev));

  const auto ret2 = co_dev_set_id(dev, 0xff - 1);

  CHECK_EQUAL(-1, ret2);
  CHECK_EQUAL(0x01, co_dev_get_id(dev));
}
