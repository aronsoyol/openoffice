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
#include "precompiled_ucb.hxx"
#include "com/sun/star/io/IOException.hpp"
#include "com/sun/star/uno/RuntimeException.hpp"
#include "osl/diagnose.h"
#include "filstr.hxx"
#include "shell.hxx"
#include "prov.hxx"


using namespace fileaccess;
using namespace com::sun::star;
using namespace com::sun::star::ucb;



/******************************************************************************/
/*                                                                            */
/*               XStream_impl implementation                                  */
/*                                                                            */
/******************************************************************************/


uno::Any SAL_CALL
XStream_impl::queryInterface(
	const uno::Type& rType )
	throw( uno::RuntimeException)
{
	uno::Any aRet = cppu::queryInterface( rType,
										  SAL_STATIC_CAST( lang::XTypeProvider*,this ),
										  SAL_STATIC_CAST( io::XStream*,this ),
										  SAL_STATIC_CAST( io::XInputStream*,this ),
										  SAL_STATIC_CAST( io::XOutputStream*,this ),
										  SAL_STATIC_CAST( io::XSeekable*,this ),
										  SAL_STATIC_CAST( io::XTruncate*,this ),
                                          SAL_STATIC_CAST( io::XAsyncOutputMonitor*,this ) );
	return aRet.hasValue() ? aRet : OWeakObject::queryInterface( rType );
}


void SAL_CALL
XStream_impl::acquire(
	void )
	throw()
{
	OWeakObject::acquire();
}


void SAL_CALL
XStream_impl::release(
	void )
	throw()
{
	OWeakObject::release();
}


//////////////////////////////////////////////////////////////////////////////////////////
//  XTypeProvider
//////////////////////////////////////////////////////////////////////////////////////////


XTYPEPROVIDER_IMPL_7( XStream_impl,
					  lang::XTypeProvider,
					  io::XStream,
					  io::XSeekable,
					  io::XInputStream,
					  io::XOutputStream,
					  io::XTruncate,
                      io::XAsyncOutputMonitor )



XStream_impl::XStream_impl( shell* pMyShell,const rtl::OUString& aUncPath, sal_Bool bLock )
	: m_bInputStreamCalled( false ),
	  m_bOutputStreamCalled( false ),
      m_pMyShell( pMyShell ),
	  m_xProvider( m_pMyShell->m_pProvider ),
      m_bLock( bLock ),
	  m_aFile( aUncPath ),
	  m_nErrorCode( TASKHANDLER_NO_ERROR ),
	  m_nMinorErrorCode( TASKHANDLER_NO_ERROR )
{
    sal_uInt32 nFlags = ( OpenFlag_Read | OpenFlag_Write );
    if ( !bLock )
        nFlags |= OpenFlag_NoLock;

	osl::FileBase::RC err = m_aFile.open( nFlags );
    if(  err != osl::FileBase::E_None )
	{
		m_nIsOpen = false;
		m_aFile.close();

		m_nErrorCode = TASKHANDLING_OPEN_FOR_STREAM;
		m_nMinorErrorCode = err;
	}
    else
		m_nIsOpen = true;
}


XStream_impl::~XStream_impl()
{
    try
    {
        closeStream();
    }
    catch (io::IOException const &)
    {
        OSL_ENSURE(false, "unexpected situation");
    }
    catch (uno::RuntimeException const &)
    {
        OSL_ENSURE(false, "unexpected situation");
    }
}


sal_Int32 SAL_CALL XStream_impl::CtorSuccess()
{
	return m_nErrorCode;
}



sal_Int32 SAL_CALL XStream_impl::getMinorError()
{
	return m_nMinorErrorCode;
}



uno::Reference< io::XInputStream > SAL_CALL
XStream_impl::getInputStream(  )
	throw( uno::RuntimeException)
{
	{
		osl::MutexGuard aGuard( m_aMutex );
		m_bInputStreamCalled = true;
	}
	return uno::Reference< io::XInputStream >( this );
}


uno::Reference< io::XOutputStream > SAL_CALL
XStream_impl::getOutputStream(  )
	throw( uno::RuntimeException )
{
	{
		osl::MutexGuard aGuard( m_aMutex );
		m_bOutputStreamCalled = true;
	}
	return uno::Reference< io::XOutputStream >( this );
}


void SAL_CALL XStream_impl::truncate(void)
	throw( io::IOException, uno::RuntimeException )
{
	if (osl::FileBase::E_None != m_aFile.setSize(0))
		throw io::IOException( ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( OSL_LOG_PREFIX ) ), uno::Reference< uno::XInterface >() );

    if (osl::FileBase::E_None != m_aFile.setPos(Pos_Absolut,sal_uInt64(0)))
		throw io::IOException( ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( OSL_LOG_PREFIX ) ), uno::Reference< uno::XInterface >() );
}



//===========================================================================
// XStream_impl private non interface methods
//===========================================================================

sal_Int32 SAL_CALL
XStream_impl::readBytes(
	uno::Sequence< sal_Int8 >& aData,
	sal_Int32 nBytesToRead )
	throw( io::NotConnectedException,
		   io::BufferSizeExceededException,
		   io::IOException,
		   uno::RuntimeException)
{
	if( ! m_nIsOpen )
		throw io::IOException( ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( OSL_LOG_PREFIX ) ), uno::Reference< uno::XInterface >() );

	sal_Int8 * buffer;
	try
	{
		buffer = new sal_Int8[nBytesToRead];
	}
	catch( std::bad_alloc )
	{
		if( m_nIsOpen ) m_aFile.close();
		throw io::BufferSizeExceededException( ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( OSL_LOG_PREFIX ) ), uno::Reference< uno::XInterface >() );
	}
	
	sal_uInt64 nrc(0);
	if(m_aFile.read( (void* )buffer,sal_uInt64(nBytesToRead),nrc )
       != osl::FileBase::E_None)
	{
		delete[] buffer;
        throw io::IOException( ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( OSL_LOG_PREFIX ) ), uno::Reference< uno::XInterface >() );		
	}
	aData = uno::Sequence< sal_Int8 > ( buffer, (sal_uInt32)nrc );
	delete[] buffer;
	return ( sal_Int32 ) nrc;
}


sal_Int32 SAL_CALL
XStream_impl::readSomeBytes(
	uno::Sequence< sal_Int8 >& aData,
	sal_Int32 nMaxBytesToRead )
	throw( io::NotConnectedException,
		   io::BufferSizeExceededException,
		   io::IOException,
		   uno::RuntimeException)
{
	return readBytes( aData,nMaxBytesToRead );
}


void SAL_CALL
XStream_impl::skipBytes(
	sal_Int32 nBytesToSkip )
	throw( io::NotConnectedException,
		   io::BufferSizeExceededException,
		   io::IOException,
		   uno::RuntimeException )
{
	m_aFile.setPos( osl_Pos_Current, sal_uInt64( nBytesToSkip ) );
}


sal_Int32 SAL_CALL
XStream_impl::available(
	void )
	throw( io::NotConnectedException,
		   io::IOException,
		   uno::RuntimeException)
{
	return 0;
}


void SAL_CALL
XStream_impl::writeBytes( const uno::Sequence< sal_Int8 >& aData )
	throw( io::NotConnectedException,
		   io::BufferSizeExceededException,
		   io::IOException,
		   uno::RuntimeException)
{
	sal_uInt32 length = aData.getLength();
	if(length)
	{
		sal_uInt64 nWrittenBytes(0);
		const sal_Int8* p = aData.getConstArray();
		if(osl::FileBase::E_None != m_aFile.write(((void*)(p)),sal_uInt64(length),nWrittenBytes) ||
		   nWrittenBytes != length )
			throw io::IOException( ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( OSL_LOG_PREFIX ) ), uno::Reference< uno::XInterface >() );
	}
}


void SAL_CALL
XStream_impl::closeStream(
	void )
	throw( io::NotConnectedException,
		   io::IOException,
		   uno::RuntimeException )
{
	if( m_nIsOpen )
	{
		osl::FileBase::RC err = m_aFile.close();
        
        if( err != osl::FileBase::E_None ) {
            io::IOException ex;
            ex.Message = rtl::OUString::createFromAscii(
                "could not close file");
			throw ex;
        }
		
		m_nIsOpen = false;
	}
}

void SAL_CALL
XStream_impl::closeInput(
	void )
	throw( io::NotConnectedException,
		   io::IOException,
		   uno::RuntimeException )
{
	osl::MutexGuard aGuard( m_aMutex );
	m_bInputStreamCalled = false;
	
	if( ! m_bOutputStreamCalled )
		closeStream();
}


void SAL_CALL
XStream_impl::closeOutput(
	void )
	throw( io::NotConnectedException,
		   io::IOException,
		   uno::RuntimeException )
{
	osl::MutexGuard aGuard( m_aMutex );
	m_bOutputStreamCalled = false;
	
	if( ! m_bInputStreamCalled )
		closeStream();
}


void SAL_CALL
XStream_impl::seek(
	sal_Int64 location )
	throw( lang::IllegalArgumentException,
		   io::IOException,
		   uno::RuntimeException )
{
	if( location < 0 )
		throw lang::IllegalArgumentException( ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( OSL_LOG_PREFIX ) ), uno::Reference< uno::XInterface >(), 0 );
	if( osl::FileBase::E_None != m_aFile.setPos( Pos_Absolut, sal_uInt64( location ) ) )
		throw io::IOException( ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( OSL_LOG_PREFIX ) ), uno::Reference< uno::XInterface >() );
}


sal_Int64 SAL_CALL
XStream_impl::getPosition(
	void )
	throw( io::IOException,
		   uno::RuntimeException )
{
	sal_uInt64 uPos;
	if( osl::FileBase::E_None != m_aFile.getPos( uPos ) )
		throw io::IOException( ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( OSL_LOG_PREFIX ) ), uno::Reference< uno::XInterface >() );
	return sal_Int64( uPos );
}

sal_Int64 SAL_CALL
XStream_impl::getLength(
	void )
	throw( io::IOException,
		   uno::RuntimeException )
{
        sal_uInt64 uEndPos;
        if ( m_aFile.getSize(uEndPos) != osl::FileBase::E_None )
                throw io::IOException( ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( OSL_LOG_PREFIX ) ), uno::Reference< uno::XInterface >() );
        else
                return sal_Int64( uEndPos );
}

void SAL_CALL
XStream_impl::flush()
	throw( io::NotConnectedException,
		   io::BufferSizeExceededException,
		   io::IOException,
		   uno::RuntimeException )
{}

void XStream_impl::waitForCompletion()
    throw (io::IOException, uno::RuntimeException)
{
    // At least on UNIX, to reliably learn about any errors encountered by
    // asynchronous NFS write operations, without closing the file directly
    // afterwards, there appears to be no cheaper way than to call fsync:
    if (m_nIsOpen && m_aFile.sync() != osl::FileBase::E_None) {
        throw io::IOException(
            rtl::OUString(
                RTL_CONSTASCII_USTRINGPARAM(
                    "could not synchronize file to disc")),
            static_cast< OWeakObject * >(this));
    }
}
