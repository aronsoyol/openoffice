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



#ifndef INCLUDED_CPPUNITTESTER_PROTECTORFACTORY_HXX
#define INCLUDED_CPPUNITTESTER_PROTECTORFACTORY_HXX

#include "sal/config.h"

#include "sal/types.h"

namespace CppUnit { class Protector; }

namespace cppunittester {

// The type of CppUnit::Protector factory functions that can be plugged into
// cppunittester:
extern "C" typedef CppUnit::Protector * SAL_CALL ProtectorFactory();

}

#endif
