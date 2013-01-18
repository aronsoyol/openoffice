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



#ifndef _B3D_MATRIL3D_HXX
#define _B3D_MATRIL3D_HXX

#include <bf_svtools/bf_solar.h>


#ifndef _B3D_BUCKET_HXX
#include "bucket.hxx"
#endif

#ifndef _TOOLS_COLOR_HXX
#include <tools/color.hxx>
#endif

#ifndef _STREAM_HXX
#include <tools/stream.hxx>
#endif

namespace binfilter {

/*************************************************************************
|*
|* Moegliche MaterialModes fuer Polygone
|*
\************************************************************************/

enum Base3DMaterialMode
{
	Base3DMaterialFront = 0,
	Base3DMaterialBack,
	Base3DMaterialFrontAndBack
};

/*************************************************************************
|*
|* Moegliche MaterialValues fuer Polygone
|*
\************************************************************************/

enum Base3DMaterialValue
{
	Base3DMaterialAmbient = 0,
	Base3DMaterialDiffuse,
	Base3DMaterialSpecular,
	Base3DMaterialEmission
};

/*************************************************************************
|*
|* Basisklasse fuer Materialparameter
|*
\************************************************************************/

class B3dMaterial
{
private:
	Color					aAmbient;
	Color					aDiffuse;
	Color					aSpecular;
	Color					aEmission;
	UINT16					nExponent;

public:
	B3dMaterial();

	// Laden/Speichern in StarView
	virtual void WriteData(SvStream& rOut) const;
	virtual void ReadData(SvStream& rIn);

	// Vergleichsoperator
	BOOL operator==(const B3dMaterial&);
	BOOL operator!=(const B3dMaterial& rMat) { return (!((*this) == rMat)); }
protected:
};

}//end of namespace binfilter

#endif          // _B3D_MATRIL3D_HXX