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
#include "precompiled_vcl.hxx"

#include <tools/ref.hxx>
#include <tools/debug.hxx>
#include <tools/poly.hxx>

#include <vcl/metaact.hxx>
#include <vcl/gdimtf.hxx>
#include <vcl/outdev.hxx>
#include <vcl/virdev.hxx>

#include <salgdi.hxx>
#include <svdata.hxx>
#include <outdata.hxx>
#include <outdev.h>

// =======================================================================

DBG_NAMEEX( OutputDevice )

// =======================================================================

void OutputDevice::DrawRect( const Rectangle& rRect,
							 sal_uLong nHorzRound, sal_uLong nVertRound )
{
	DBG_TRACE( "OutputDevice::DrawRoundRect()" );
	DBG_CHKTHIS( OutputDevice, ImplDbgCheckOutputDevice );

	if ( mpMetaFile )
		mpMetaFile->AddAction( new MetaRoundRectAction( rRect, nHorzRound, nVertRound ) );

	if ( !IsDeviceOutputNecessary() || (!mbLineColor && !mbFillColor) || ImplIsRecordLayout() )
		return;

	const Rectangle aRect( ImplLogicToDevicePixel( rRect ) );

	if ( aRect.IsEmpty() )
		return;

	nHorzRound = ImplLogicWidthToDevicePixel( nHorzRound );
	nVertRound = ImplLogicHeightToDevicePixel( nVertRound );

	// we need a graphics
	if ( !mpGraphics )
	{
		if ( !ImplGetGraphics() )
			return;
	}

	if ( mbInitClipRegion )
		ImplInitClipRegion();
	if ( mbOutputClipped )
		return;

	if ( mbInitLineColor )
		ImplInitLineColor();
	if ( mbInitFillColor )
		ImplInitFillColor();

	if ( !nHorzRound && !nVertRound )
		mpGraphics->DrawRect( aRect.Left(), aRect.Top(), aRect.GetWidth(), aRect.GetHeight(), this );
	else
	{
		const Polygon aRoundRectPoly( aRect, nHorzRound, nVertRound );

		if ( aRoundRectPoly.GetSize() >= 2 )
		{
			const SalPoint* pPtAry = (const SalPoint*) aRoundRectPoly.GetConstPointAry();
			
			if ( !mbFillColor )
				mpGraphics->DrawPolyLine( aRoundRectPoly.GetSize(), pPtAry, this );
			else
				mpGraphics->DrawPolygon( aRoundRectPoly.GetSize(), pPtAry, this );
		}
	}

    if( mpAlphaVDev )
        mpAlphaVDev->DrawRect( rRect, nHorzRound, nVertRound );
}

// -----------------------------------------------------------------------

void OutputDevice::DrawEllipse( const Rectangle& rRect )
{
	DBG_TRACE( "OutputDevice::DrawEllipse()" );
	DBG_CHKTHIS( OutputDevice, ImplDbgCheckOutputDevice );

	if ( mpMetaFile )
		mpMetaFile->AddAction( new MetaEllipseAction( rRect ) );

	if	( !IsDeviceOutputNecessary() || (!mbLineColor && !mbFillColor) || ImplIsRecordLayout() )
		return;

	Rectangle aRect( ImplLogicToDevicePixel( rRect ) );
	if ( aRect.IsEmpty() )
		return;

	// we need a graphics
	if ( !mpGraphics )
	{
		if ( !ImplGetGraphics() )
			return;
	}

	if ( mbInitClipRegion )
		ImplInitClipRegion();
	if ( mbOutputClipped )
		return;

	if ( mbInitLineColor )
		ImplInitLineColor();

	Polygon aRectPoly( aRect.Center(), aRect.GetWidth() >> 1, aRect.GetHeight() >> 1 );
	if ( aRectPoly.GetSize() >= 2 )
	{
		const SalPoint* pPtAry = (const SalPoint*)aRectPoly.GetConstPointAry();
		if ( !mbFillColor )
			mpGraphics->DrawPolyLine( aRectPoly.GetSize(), pPtAry, this );
		else
		{
			if ( mbInitFillColor )
				ImplInitFillColor();
			mpGraphics->DrawPolygon( aRectPoly.GetSize(), pPtAry, this );
		}
	}

    if( mpAlphaVDev )
        mpAlphaVDev->DrawEllipse( rRect );
}

// -----------------------------------------------------------------------

void OutputDevice::DrawArc( const Rectangle& rRect,
							const Point& rStartPt, const Point& rEndPt )
{
	DBG_TRACE( "OutputDevice::DrawArc()" );
	DBG_CHKTHIS( OutputDevice, ImplDbgCheckOutputDevice );

	if ( mpMetaFile )
		mpMetaFile->AddAction( new MetaArcAction( rRect, rStartPt, rEndPt ) );

	if ( !IsDeviceOutputNecessary() || !mbLineColor || ImplIsRecordLayout() )
		return;

	Rectangle aRect( ImplLogicToDevicePixel( rRect ) );
	if ( aRect.IsEmpty() )
		return;

	// we need a graphics
	if ( !mpGraphics )
	{
		if ( !ImplGetGraphics() )
			return;
	}

	if ( mbInitClipRegion )
		ImplInitClipRegion();
	if ( mbOutputClipped )
		return;

	if ( mbInitLineColor )
		ImplInitLineColor();

	const Point 	aStart( ImplLogicToDevicePixel( rStartPt ) );
	const Point 	aEnd( ImplLogicToDevicePixel( rEndPt ) );
	Polygon 		aArcPoly( aRect, aStart, aEnd, POLY_ARC );

	if ( aArcPoly.GetSize() >= 2 )
	{
		const SalPoint* pPtAry = (const SalPoint*)aArcPoly.GetConstPointAry();
		mpGraphics->DrawPolyLine( aArcPoly.GetSize(), pPtAry, this );
	}

    if( mpAlphaVDev )
        mpAlphaVDev->DrawArc( rRect, rStartPt, rEndPt );
}

// -----------------------------------------------------------------------

void OutputDevice::DrawPie( const Rectangle& rRect,
							const Point& rStartPt, const Point& rEndPt )
{
	DBG_TRACE( "OutputDevice::DrawPie()" );
	DBG_CHKTHIS( OutputDevice, ImplDbgCheckOutputDevice );

	if ( mpMetaFile )
		mpMetaFile->AddAction( new MetaPieAction( rRect, rStartPt, rEndPt ) );

	if ( !IsDeviceOutputNecessary() || (!mbLineColor && !mbFillColor) || ImplIsRecordLayout() )
		return;

	Rectangle aRect( ImplLogicToDevicePixel( rRect ) );
	if ( aRect.IsEmpty() )
		return;

	// we need a graphics
	if ( !mpGraphics )
	{
		if ( !ImplGetGraphics() )
			return;
	}

	if ( mbInitClipRegion )
		ImplInitClipRegion();
	if ( mbOutputClipped )
		return;

	if ( mbInitLineColor )
		ImplInitLineColor();

	const Point 	aStart( ImplLogicToDevicePixel( rStartPt ) );
	const Point 	aEnd( ImplLogicToDevicePixel( rEndPt ) );
	Polygon 		aPiePoly( aRect, aStart, aEnd, POLY_PIE );

	if ( aPiePoly.GetSize() >= 2 )
	{
		const SalPoint* pPtAry = (const SalPoint*)aPiePoly.GetConstPointAry();
		if ( !mbFillColor )
			mpGraphics->DrawPolyLine( aPiePoly.GetSize(), pPtAry, this );
		else
		{
			if ( mbInitFillColor )
				ImplInitFillColor();
			mpGraphics->DrawPolygon( aPiePoly.GetSize(), pPtAry, this );
		}
	}

    if( mpAlphaVDev )
        mpAlphaVDev->DrawPie( rRect, rStartPt, rEndPt );
}

// -----------------------------------------------------------------------

void OutputDevice::DrawChord( const Rectangle& rRect,
							  const Point& rStartPt, const Point& rEndPt )
{
	DBG_TRACE( "OutputDevice::DrawChord()" );
	DBG_CHKTHIS( OutputDevice, ImplDbgCheckOutputDevice );

	if ( mpMetaFile )
		mpMetaFile->AddAction( new MetaChordAction( rRect, rStartPt, rEndPt ) );

	if ( !IsDeviceOutputNecessary() || (!mbLineColor && !mbFillColor) || ImplIsRecordLayout() )
		return;

	Rectangle aRect( ImplLogicToDevicePixel( rRect ) );
	if ( aRect.IsEmpty() )
		return;

	// we need a graphics
	if ( !mpGraphics )
	{
		if ( !ImplGetGraphics() )
			return;
	}

	if ( mbInitClipRegion )
		ImplInitClipRegion();
	if ( mbOutputClipped )
		return;

	if ( mbInitLineColor )
		ImplInitLineColor();

	const Point 	aStart( ImplLogicToDevicePixel( rStartPt ) );
	const Point 	aEnd( ImplLogicToDevicePixel( rEndPt ) );
	Polygon 		aChordPoly( aRect, aStart, aEnd, POLY_CHORD );

	if ( aChordPoly.GetSize() >= 2 )
	{
		const SalPoint* pPtAry = (const SalPoint*)aChordPoly.GetConstPointAry();
		if ( !mbFillColor )
			mpGraphics->DrawPolyLine( aChordPoly.GetSize(), pPtAry, this );
		else
		{
			if ( mbInitFillColor )
				ImplInitFillColor();
			mpGraphics->DrawPolygon( aChordPoly.GetSize(), pPtAry, this );
		}
	}

    if( mpAlphaVDev )
        mpAlphaVDev->DrawChord( rRect, rStartPt, rEndPt );
}
