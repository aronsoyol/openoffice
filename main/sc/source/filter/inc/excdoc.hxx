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



#ifndef SC_EXCDOC_HXX
#define SC_EXCDOC_HXX

#include <tools/solar.h>
#include "excrecds.hxx"
#include "xeroot.hxx"
#include "root.hxx"

//------------------------------------------------------------------ Forwards -

class SvStream;
class ScBaseCell;
class ScHorizontalCellIterator;
class ScDocument;
class ScProgress;

class NameBuffer;

class XclExpChangeTrack;


//------------------------------------------------------------ class ExcTable -

class XclExpCellTable;

class ExcTable : public XclExpRecordBase, public XclExpRoot
{
private:
    typedef XclExpRecordList< ExcBundlesheetBase >  ExcBoundsheetList;
    typedef ScfRef< XclExpCellTable >               XclExpCellTableRef;

    XclExpRecordList<>          aRecList;
    XclExpCellTableRef          mxCellTable;

    SCTAB                       mnScTab;    // table number SC document
	sal_uInt16						nExcTab;	// table number Excel document
	sal_uInt16						nAktRow;	// fuer'n Iterator
	sal_uInt16						nAktCol;

    NameBuffer*                 pTabNames;

	// pRec mit new anlegen und vergessen, delete macht ExcTable selber!
    void                        Add( XclExpRecordBase* pRec );

    void                        FillAsXmlTable( SCTAB nCodeNameIdx );

public:
                                ExcTable( const XclExpRoot& rRoot );
                                ExcTable( const XclExpRoot& rRoot, SCTAB nScTab );
								~ExcTable();

    void                        FillAsHeader( ExcBoundsheetList& rBoundsheetList );
    void                        FillAsTable( SCTAB nCodeNameIdx );
    void                        FillAsEmptyTable( SCTAB nCodeNameIdx );

	void						Write( XclExpStream& );
	void						WriteXml( XclExpXmlStream& );
};


//--------------------------------------------------------- class ExcDocument -

class ExcDocument : protected XclExpRoot
{
friend class ExcTable;

private:
    typedef XclExpRecordList< ExcTable >            ExcTableList;
    typedef ExcTableList::RecordRefType             ExcTableRef;
    typedef XclExpRecordList< ExcBundlesheetBase >  ExcBoundsheetList;
    typedef ExcBoundsheetList::RecordRefType        ExcBoundsheetRef;

	ExcTable			aHeader;

    ExcTableList        maTableList;
    ExcBoundsheetList   maBoundsheetList;

	XclExpChangeTrack*	pExpChangeTrack;

public:
    explicit                    ExcDocument( const XclExpRoot& rRoot );
    virtual                     ~ExcDocument();

	void				ReadDoc( void );
	void				Write( SvStream& rSvStrm );
	void				WriteXml( SvStream& rSvStrm );
};




#endif

