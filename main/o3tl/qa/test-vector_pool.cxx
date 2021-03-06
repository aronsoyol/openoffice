/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/

// autogenerated file with codegen.pl

#include "preextstl.h"
#include "cppunit/TestAssert.h"
#include "cppunit/TestFixture.h"
#include "cppunit/extensions/HelperMacros.h"
#include "postextstl.h"

#include <o3tl/vector_pool.hxx>

using namespace ::o3tl;

class vector_pool_test : public CppUnit::TestFixture
{
public:
    void testPoolBasics()
    {
        vector_pool<int> aPool;

        std::ptrdiff_t nIdx1 = aPool.alloc();
        std::ptrdiff_t nIdx2 = aPool.alloc();
        std::ptrdiff_t nIdx3 = aPool.alloc();

        CPPUNIT_ASSERT_MESSAGE("allocator idx order 1", nIdx1 < nIdx2 );
        CPPUNIT_ASSERT_MESSAGE("allocator idx order 2", nIdx2 < nIdx3 );

        aPool.free(nIdx2);
        aPool.free(nIdx3);

        nIdx2 = aPool.alloc();
        nIdx3 = aPool.alloc();

        CPPUNIT_ASSERT_MESSAGE("allocator idx order 1 after fragmentation", nIdx1 < nIdx3 );
        CPPUNIT_ASSERT_MESSAGE("allocator idx order 2 after fragmentation", nIdx3 < nIdx2 );
    }

    void testPoolValueSemantics()
    {
        vector_pool<int> aPool;

        std::ptrdiff_t nIdx1 = aPool.store(0);
        CPPUNIT_ASSERT_MESSAGE("allocator value semantics 1", aPool.get(nIdx1) == 0 );

        std::ptrdiff_t nIdx2 = aPool.store(1);
        CPPUNIT_ASSERT_MESSAGE("allocator value semantics 2", aPool.get(nIdx2) == 1 );

        std::ptrdiff_t nIdx3 = aPool.store(2);
        CPPUNIT_ASSERT_MESSAGE("allocator value semantics 3", aPool.get(nIdx3) == 2 );

        aPool.free(nIdx2);
        aPool.free(nIdx3);

        nIdx2 = aPool.store(1);
        CPPUNIT_ASSERT_MESSAGE("allocator value semantics 2 after fragmentation", aPool.get(nIdx2) == 1 );

        nIdx3 = aPool.store(2);
        CPPUNIT_ASSERT_MESSAGE("allocator value semantics 3 after fragmentation", aPool.get(nIdx3) == 2 );
    }

    // Change the following lines only, if you add, remove or rename 
    // member functions of the current class, 
    // because these macros are need by auto register mechanism.

    CPPUNIT_TEST_SUITE(vector_pool_test);
    CPPUNIT_TEST(testPoolBasics);
    CPPUNIT_TEST(testPoolValueSemantics);
    CPPUNIT_TEST_SUITE_END();
};

// -----------------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION(vector_pool_test);
