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



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_sal.hxx"
//------------------------------------------------------------------------
// include files
//------------------------------------------------------------------------
#include <sal/types.h>

#ifndef _RTL_USTRING_HXX_
#include <rtl/string.hxx>
#endif

#ifndef _OSL_THREAD_HXX
#include <osl/thread.hxx>
#endif
#include <osl/time.h>

#include <rtl/instance.hxx>

#include <testshl/simpleheader.hxx>

// -----------------------------------------------------------------------------
#define CONST_TEST_STRING "gregorian"

namespace {
struct Gregorian : public rtl::StaticWithInit<const ::rtl::OUString, Gregorian> {
    const ::rtl::OUString operator () () {
        return ::rtl::OUString(RTL_CONSTASCII_USTRINGPARAM( CONST_TEST_STRING ));
    }
};
}

inline void printOUString( ::rtl::OUString const & _suStr )
{
    rtl::OString aString;

    t_print( "OUString: " );
    aString = ::rtl::OUStringToOString( _suStr, RTL_TEXTENCODING_ASCII_US );
    t_print( "'%s'\n", aString.getStr( ) );
}

// -----------------------------------------------------------------------------
namespace ThreadHelper
{
    // typedef enum {
    //     QUIET=1,
    //     VERBOSE
    // } eSleepVerboseMode;

    void thread_sleep_tenth_sec(sal_Int32 _nTenthSec/*, eSleepVerboseMode nVerbose = VERBOSE*/)
    {
        // if (nVerbose == VERBOSE)
        // {
        //     t_print("wait %d tenth seconds. ", _nTenthSec );
        //     fflush(stdout);
        // }
#ifdef WNT      //Windows
        Sleep(_nTenthSec * 100 );
#endif
#if ( defined UNX ) || ( defined OS2 )  //Unix
        TimeValue nTV;
        nTV.Seconds = static_cast<sal_uInt32>( _nTenthSec/10 );
        nTV.Nanosec = ( (_nTenthSec%10 ) * 100000000 );
        osl_waitThread(&nTV);
#endif
        // if (nVerbose == VERBOSE)
        // {
        //     t_print("done\n");
        // }
    }
}

// -----------------------------------------------------------------------------

/** Simple thread for testing Thread-create.
 * Just add 1 of value 0, and after running, result is 1.
 */
class OGetThread : public osl::Thread
{
    sal_Int32 m_nOK;
    sal_Int32 m_nFails;

    rtl::OUString m_sConstStr;
public:
    OGetThread()
            :m_nOK(0),
             m_nFails(0)
        {
            m_sConstStr = rtl::OUString(RTL_CONSTASCII_USTRINGPARAM( CONST_TEST_STRING ));
        }

    sal_Int32 getOK() { return m_nOK; }
    sal_Int32 getFails() {return m_nFails;}

protected:

    /** guarded value which initialized 0

        @see ThreadSafeValue
    */
    void SAL_CALL run()
        {
            while(schedule())
            {
                rtl::OUString aStr = Gregorian::get();
                // printOUString(aStr);
                // printOUString(m_sConstStr);
                if (aStr.equals(m_sConstStr))
                {
                    m_nOK++;
                }
                else
                {
                    m_nFails++;
                }
                ThreadHelper::thread_sleep_tenth_sec(1);
            }
        }

public:

    virtual void SAL_CALL suspend()
        {
            ::osl::Thread::suspend();
        }

    ~OGetThread()
        {
            if (isRunning())
            {
                t_print("error: not terminated.\n");
            }
        }
};

// -----------------------------------------------------------------------------
namespace rtl_DoubleLocking
{

/** Test of the osl::Thread::create method
 */

    class getValue : public CppUnit::TestFixture
    {
    public:

        // initialise your test code values here.
        void setUp()
            {
            }

        void tearDown()
            {
            }


        void getValue_001()
            {
                rtl::OUString aStr = Gregorian::get();
                printOUString(aStr);

                CPPUNIT_ASSERT_MESSAGE(
                    "Gregorian::get() failed, wrong value expected.",
                    aStr.getLength() != 0
                    );
            }

        /** check 2 threads.

            ALGORITHM:
            Here the function should show, that 2 different threads,
            which only increase a value, should run at the same time with same prio.
            The test fails, if the difference between the two values is more than 5%
            but IMHO this isn't a failure, it's only a feature of the OS.
        */

        void getValue_002()
            {
                // initial 5 threads with different priorities
                OGetThread* pThread = new OGetThread();
                OGetThread* p2Thread = new OGetThread();

                //Create them and start running at the same time
                pThread->create();
                p2Thread->create();

                ThreadHelper::thread_sleep_tenth_sec(50);

                pThread->terminate();
                p2Thread->terminate();

                sal_Int32 nValueOK = 0;
                nValueOK = pThread->getOK();

                sal_Int32 nValueOK2 = 0;
                nValueOK2 = p2Thread->getOK();

                t_print("Value in Thread #1 is %d\n", nValueOK);
                t_print("Value in Thread #2 is %d\n", nValueOK2);

                sal_Int32 nValueFails = 0;
                nValueFails = pThread->getFails();

                sal_Int32 nValueFails2 = 0;
                nValueFails2 = p2Thread->getFails();

                t_print("Fails in Thread #1 is %d\n", nValueFails);
                t_print("Fails in Thread #2 is %d\n", nValueFails2);

                // ThreadHelper::thread_sleep_tenth_sec(1);
                pThread->join();
                p2Thread->join();

                delete pThread;
                delete p2Thread;

                CPPUNIT_ASSERT_MESSAGE(
                    "getValue() failed, wrong value expected.",
                    nValueOK != 0 && nValueFails == 0 && nValueFails2 == 0
                    );
            }

        CPPUNIT_TEST_SUITE(getValue);
        CPPUNIT_TEST(getValue_001);
        CPPUNIT_TEST(getValue_002);
        CPPUNIT_TEST_SUITE_END();
    }; // class create
// -----------------------------------------------------------------------------
    CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(rtl_DoubleLocking::getValue, "rtl_DoubleLocking");
} // namespace rtl_DoubleLocking

// this macro creates an empty function, which will called by the RegisterAllFunctions()
// to let the user the possibility to also register some functions by hand.
NOADDITIONAL;
