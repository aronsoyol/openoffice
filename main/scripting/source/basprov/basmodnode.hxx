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



#ifndef SCRIPTING_BASMODNODE_HXX
#define SCRIPTING_BASMODNODE_HXX

#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/script/browse/XBrowseNode.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>
#include <cppuhelper/implbase1.hxx>

class SbModule;


//.........................................................................
namespace basprov
{
//.........................................................................

    //	----------------------------------------------------
    //	class BasicModuleNodeImpl
    //	----------------------------------------------------

    typedef ::cppu::WeakImplHelper1<    
        ::com::sun::star::script::browse::XBrowseNode > BasicModuleNodeImpl_BASE;


    class BasicModuleNodeImpl : public BasicModuleNodeImpl_BASE 
    {
    private:
        ::com::sun::star::uno::Reference< ::com::sun::star::uno::XComponentContext >    m_xContext;
	::rtl::OUString m_sScriptingContext;
        SbModule* m_pModule;
        bool m_bIsAppScript;

    public:
        BasicModuleNodeImpl( const ::com::sun::star::uno::Reference< ::com::sun::star::uno::XComponentContext >& rxContext,
            const ::rtl::OUString& sScriptingContext,
            SbModule* pModule, bool isAppScript = true );
        virtual ~BasicModuleNodeImpl();

        // XBrowseNode
        virtual ::rtl::OUString SAL_CALL getName(  ) 
            throw (::com::sun::star::uno::RuntimeException);
        virtual ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Reference< ::com::sun::star::script::browse::XBrowseNode > > SAL_CALL getChildNodes(  )
            throw (::com::sun::star::uno::RuntimeException);
        virtual sal_Bool SAL_CALL hasChildNodes(  ) 
            throw (::com::sun::star::uno::RuntimeException);
        virtual sal_Int16 SAL_CALL getType(  ) 
            throw (::com::sun::star::uno::RuntimeException);
    };

//.........................................................................
}	// namespace basprov
//.........................................................................

#endif // SCRIPTING_BASMODNODE_HXX
