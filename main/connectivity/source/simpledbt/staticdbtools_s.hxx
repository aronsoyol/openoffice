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



#ifndef CONNECTIVITY_STATIC_DBTOOLS_SIMPLE_HXX
#define CONNECTIVITY_STATIC_DBTOOLS_SIMPLE_HXX

#include <connectivity/virtualdbtools.hxx>
#include "refbase.hxx"

//........................................................................
namespace connectivity
{
//........................................................................

	//================================================================
	//= ODataAccessStaticTools
	//================================================================
	class ODataAccessStaticTools
			:public simple::IDataAccessTypeConversion
			,public simple::IDataAccessTools
			,public ORefBase
	{
	public:
		ODataAccessStaticTools();

		// IDataAccessTypeConversion
		// ------------------------------------------------
		virtual ::com::sun::star::util::Date getStandardDate() const;

		// ------------------------------------------------
		virtual double getValue(
			const ::com::sun::star::uno::Reference< ::com::sun::star::sdb::XColumn>& _rxVariant,
			const ::com::sun::star::util::Date& rNullDate ) const;

		// ------------------------------------------------
		virtual ::rtl::OUString getFormattedValue(
			const ::com::sun::star::uno::Reference< ::com::sun::star::sdb::XColumn >& _rxColumn,
			const ::com::sun::star::uno::Reference< ::com::sun::star::util::XNumberFormatter >& _rxFormatter,
			const ::com::sun::star::util::Date& _rNullDate,
			sal_Int32 _nKey,
			sal_Int16 _nKeyType) const;

		// ------------------------------------------------
		virtual ::rtl::OUString getFormattedValue(
			const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet>& _rxColumn,
			const ::com::sun::star::uno::Reference< ::com::sun::star::util::XNumberFormatter>& _rxFormatter,
			const ::com::sun::star::lang::Locale& _rLocale,
			const ::com::sun::star::util::Date& _rNullDate
		) const;

		// IDataAccessTools
		// ------------------------------------------------
		virtual ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection> getConnection_withFeedback(
			const ::rtl::OUString& _rDataSourceName,
			const ::rtl::OUString& _rUser,
			const ::rtl::OUString& _rPwd,
			const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory>& _rxFactory
		) const SAL_THROW ( (::com::sun::star::sdbc::SQLException) );

		// ------------------------------------------------
		virtual ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection> connectRowset(
			const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XRowSet>& _rxRowSet,
			const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory>& _rxFactory,
			sal_Bool _bSetAsActiveConnection
		) const SAL_THROW ( ( ::com::sun::star::sdbc::SQLException
                            , ::com::sun::star::lang::WrappedTargetException
                            , ::com::sun::star::uno::RuntimeException ) );

		// ------------------------------------------------
		virtual ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection> getRowSetConnection(
				const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XRowSet>& _rxRowSet) 
				const SAL_THROW ( (::com::sun::star::uno::RuntimeException) );

		// ------------------------------------------------
		::com::sun::star::uno::Reference< ::com::sun::star::util::XNumberFormatsSupplier> getNumberFormats(
			const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection>& _rxConn,
			sal_Bool _bAllowDefault
		) const;

		// ------------------------------------------------
		virtual sal_Int32  getDefaultNumberFormat(
			const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet >& _rxColumn,
			const ::com::sun::star::uno::Reference< ::com::sun::star::util::XNumberFormatTypes >& _rxTypes,
			const ::com::sun::star::lang::Locale& _rLocale
		) const;

		// ------------------------------------------------
		virtual void TransferFormComponentProperties(
			const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet>& _rxOld,
			const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet>& _rxNew,
			const ::com::sun::star::lang::Locale& _rLocale
		) const;

		// ------------------------------------------------
		virtual ::rtl::OUString quoteName(
			const ::rtl::OUString& _rQuote,
			const ::rtl::OUString& _rName
		) const;

		// ------------------------------------------------
	    virtual ::rtl::OUString composeTableNameForSelect(
            const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection >& _rxConnection,
			const ::rtl::OUString& _rCatalog,
			const ::rtl::OUString& _rSchema,
			const ::rtl::OUString& _rName
        ) const;

		// ------------------------------------------------
	    virtual ::rtl::OUString composeTableNameForSelect(
            const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection >& _rxConnection,
			const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet>& _xTable
        ) const;

		// ------------------------------------------------
		virtual ::com::sun::star::sdb::SQLContext prependContextInfo(
			::com::sun::star::sdbc::SQLException& _rException,
			const ::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface >& _rxContext,
			const ::rtl::OUString& _rContextDescription,
			const ::rtl::OUString& _rContextDetails
		) const;

		// ------------------------------------------------
		virtual ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XDataSource > getDataSource(
			const ::rtl::OUString& _rsRegisteredName,
			const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XMultiServiceFactory>& _rxFactory
		) const;

		// ------------------------------------------------
		/** check if the property "Privileges" supports ::com::sun::star::sdbcx::Privilege::INSERT
			@param		_rxCursorSet	the property set
		*/
		virtual sal_Bool canInsert(const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet>& _rxCursorSet) const;

		// ------------------------------------------------
		/** check if the property "Privileges" supports ::com::sun::star::sdbcx::Privilege::UPDATE
			@param		_rxCursorSet	the property set
		*/
		virtual sal_Bool canUpdate(const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet>& _rxCursorSet) const;

		// ------------------------------------------------
		/** check if the property "Privileges" supports ::com::sun::star::sdbcx::Privilege::DELETE
			@param		_rxCursorSet	the property set
		*/
		virtual sal_Bool canDelete(const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet>& _rxCursorSet) const;

		// ------------------------------------------------
		virtual ::com::sun::star::uno::Reference< ::com::sun::star::container::XNameAccess >
			getFieldsByCommandDescriptor(
				const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection >& _rxConnection,
				const sal_Int32 _nCommandType,
				const ::rtl::OUString& _rCommand,
				::com::sun::star::uno::Reference< ::com::sun::star::lang::XComponent >& _rxKeepFieldsAlive,
				::dbtools::SQLExceptionInfo* _pErrorInfo = NULL
			)	SAL_THROW( ( ) );

		// ------------------------------------------------
		virtual ::com::sun::star::uno::Sequence< ::rtl::OUString >
			getFieldNamesByCommandDescriptor(
				const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection >& _rxConnection,
				const sal_Int32 _nCommandType,
				const ::rtl::OUString& _rCommand,
				::dbtools::SQLExceptionInfo* _pErrorInfo = NULL
			)	SAL_THROW( ( ) );

		// ------------------------------------------------
        virtual bool isEmbeddedInDatabase(
            const ::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface >& _rxComponent,
            ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection >& _rxActualConnection
        );

        // disambiguate IReference
		// ------------------------------------------------
		virtual oslInterlockedCount SAL_CALL acquire();
		virtual oslInterlockedCount SAL_CALL release();
	};

//........................................................................
}	// namespace connectivity
//........................................................................

#endif // CONNECTIVITY_STATIC_DBTOOLS_SIMPLE_HXX

