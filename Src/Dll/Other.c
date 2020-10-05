/*--------------------------------------------------------------------------------- */
/* Other.c - Glide functions with unknown functionality exported                    */
/*                                                                                  */
/* Copyright (C) 2004 Dege                                                          */
/*                                                                                  */
/* This library is free software; you can redistribute it and/or                    */
/* modify it under the terms of the GNU Lesser General Public                       */
/* License as published by the Free Software Foundation; either                     */
/* version 2.1 of the License, or (at your option) any later version.               */
/*                                                                                  */
/* This library is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of                   */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                */
/* Lesser General Public License for more details.                                  */
/*                                                                                  */
/* You should have received a copy of the GNU Lesser General Public                 */
/* License along with this library; if not, write to the Free Software              */
/* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA   */
/*--------------------------------------------------------------------------------- */


/*------------------------------------------------------------------------------------------*/
/* dgVoodoo: Other.c																		*/
/*			 Ismeretlen funkciójú függvények												*/
/*------------------------------------------------------------------------------------------*/
#include "dgVoodooBase.h"
#include <glide.h>
#include <windows.h>
#include "Debug.h"


FxBool EXPORT grSstDetectResources()	{
	return(FXTRUE);
}


void EXPORT guMovieStart()	{
	DEBUGLOG(0,"\n   Error: guMovieStart is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a guMovieStart függvényt a dgVoodoo nem támogatja");
}


void EXPORT guMovieStop()	{
	DEBUGLOG(0,"\n   Error: guMovieStop is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a guMovieStop függvényt a dgVoodoo nem támogatja");
}


void EXPORT guMovieSetName(int x1)	{
	DEBUGLOG(0,"\n   Error: guMovieSetName is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a guMovieSetName függvényt a dgVoodoo nem támogatja");
}


/* Ismeretlen funkciójú és paraméterezésû függvények (a lényeg az, hogy az exorttáblában szerepeljenek) */
void EXPORT grSstConfigPipeLine(int unknown0, int unknown1, int unknown2)	{
	DEBUGLOG(0,"\n   Error: grSstConfigPipeLine is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a grSstConfigPipeLine függvényt a dgVoodoo nem támogatja");
}


void EXPORT pciOpen()	{
	DEBUGLOG(0,"\n   Error: pciOpen is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciOpen függvényt a dgVoodoo nem támogatja");
}


void EXPORT pciOpenEx()	{
	DEBUGLOG(0,"\n   Error: pciOpenEx is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciOpenEx függvényt a dgVoodoo nem támogatja");
}


void EXPORT pciClose()	{
	DEBUGLOG(0,"\n   Error: pciClose is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciClose függvényt a dgVoodoo nem támogatja");
}


void EXPORT pciDeviceExists(int unknown)	{
	DEBUGLOG(0,"\n   Error: pciDeviceExists is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciDeviceExists függvényt a dgVoodoo nem támogatja");
}


void EXPORT pciFindCard(int unknown0, int unknown1, int unknown2)	{
	DEBUGLOG(0,"\n   Error: pciFindCard is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciFindCard függvényt a dgVoodoo nem támogatja");
}


void EXPORT pciFindCardMulti(int unknown0, int unknown1, int unknown2, int unknown3)	{
	DEBUGLOG(0,"\n   Error: pciFindCardMulti is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciFindCardMulti függvényt a dgVoodoo nem támogatja");
}


void EXPORT pciFindFreeMTRR(int unknown0)	{
	DEBUGLOG(0,"\n   Error: pciFindFreeMTRR is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciFindFreeMTRR függvényt a dgVoodoo nem támogatja");
}


void EXPORT pciFindMTRRMatch(int unknown0, int unknown1, int unknown2, int unknown3)	{
	DEBUGLOG(0,"\n   Error: pciFindMTRRMatch is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciFindMTRRMatch függvényt a dgVoodoo nem támogatja");
}


void EXPORT pciFindFreeMTRRMatch(int unknown0, int unknown1, int unknown2, int unknown3)	{
	DEBUGLOG(0,"\n   Error: pciFindFreeMTRRMatch is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciFindFreeMTRRMatch függvényt a dgVoodoo nem támogatja");
}


void EXPORT pciGetConfigData(int unknown0, int unknown1, int unknown2, int unknown3, int unknown4)	{
	DEBUGLOG(0,"\n   Error: pciGetConfigData is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciGetConfigData függvényt a dgVoodoo nem támogatja");
}


void EXPORT pciSetConfigData(int unknown0, int unknown1, int unknown2, int unknown3, int unknown4)	{
	DEBUGLOG(0,"\n   Error: pciSetConfigData is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciSetConfigData függvényt a dgVoodoo nem támogatja");
}


void EXPORT pciGetErrorCode()	{
	DEBUGLOG(0,"\n   Error: pciGetErrorCode is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciGetErrorCode függvényt a dgVoodoo nem támogatja");
}


void EXPORT pciGetErrorString()	{
	DEBUGLOG(0,"\n   Error: pciGetErrorString is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciGetErrorString függvényt a dgVoodoo nem támogatja");
}


void EXPORT pciMapCard(int unknown0, int unknown1, int unknown2, int unknown3, int unknown4)	{
	DEBUGLOG(0,"\n   Error: pciMapCard is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciMapCard függvényt a dgVoodoo nem támogatja");
}


void EXPORT pciMapCardMulti(int unknown0, int unknown1, int unknown2, int unknown3, int unknown4, int unknown5)	{
	DEBUGLOG(0,"\n   Error: pciMapCardMulti is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciMapCardMulti függvényt a dgVoodoo nem támogatja");
}


void EXPORT pciMapPhysicalToLinear(int unknown0, int unknown1, int unknown2)	{
	DEBUGLOG(0,"\n   Error: pciMapPhysicalToLinear is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciMapPhysicalToLinear függvényt a dgVoodoo nem támogatja");
}


void EXPORT pciSetMTRR(int unknown0, int unknown1, int unknown2, int unknown3)	{
	DEBUGLOG(0,"\n   Error: pciSetMTRR is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciSetMTRR függvényt a dgVoodoo nem támogatja");
}


void EXPORT pciUnmapPhysical(int unknown0, int unknown1)	{
	DEBUGLOG(0,"\n   Error: pciUnmapPhysical is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciUnmapPhysical függvényt a dgVoodoo nem támogatja");
}


void EXPORT pioInByte(int unknown0)	{
	DEBUGLOG(0,"\n   Error: pioInByte is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pioInByte függvényt a dgVoodoo nem támogatja");
}


void EXPORT pioInWord(int unknown0)	{
	DEBUGLOG(0,"\n   Error: pioInWord is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pioInWord függvényt a dgVoodoo nem támogatja");
}


void EXPORT pioInLong(int unknown0)	{
	DEBUGLOG(0,"\n   Error: pioInLong is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pioInLong függvényt a dgVoodoo nem támogatja");
}


void EXPORT pioOutByte(int unknown0, int unknown1)	{
	DEBUGLOG(0,"\n   Error: pioOutByte is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pioOutByte függvényt a dgVoodoo nem támogatja");
}


void EXPORT pioOutWord(int unknown0, int unknown1)	{
	DEBUGLOG(0,"\n   Error: pioOutWord is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pioOutWord függvényt a dgVoodoo nem támogatja");
}


void EXPORT pioOutLong(int unknown0, int unknown1)	{
	DEBUGLOG(0,"\n   Error: pioOutLong is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pioOutLong függvényt a dgVoodoo nem támogatja");
}


void EXPORT grSstConfigPipeline(int x1, int x2, int x3)        {
	DEBUGLOG(0,"\n   Error: grSstConfigPipeline is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a grSstConfigPipeline függvényt a dgVoodoo nem támogatja");
}


void EXPORT grSstVidMode(int x1, int x2) {
	DEBUGLOG(0,"\n   Error: grSstVidMode is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a grSstVidMode függvényt a dgVoodoo nem támogatja");
}

/*void EXPORT guFBReadRegion(int x1, int x2, int x3, int x4, int x5, int x6)       {

}

void EXPORT guFBWriteRegion(int x1, int x2, int x3, int x4, int x5, int x6)       {

}*/


void EXPORT guMPInit()  {
	DEBUGLOG(0,"\n   Error: guMPInit is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a guMPInit függvényt a dgVoodoo nem támogatja");
}


void EXPORT guMPDrawTriangle(int x1, int x2, int x3) {
	DEBUGLOG(0,"\n   Error: guMPDrawTriangle is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a guMPDrawTriangle függvényt a dgVoodoo nem támogatja");
}


void EXPORT guMPTexCombineFunction(int x)       {
	DEBUGLOG(0,"\n   Error: guMPTexCombineFunction is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a guMPTexCombineFunction függvényt a dgVoodoo nem támogatja");
}


void EXPORT guMPTexSource(int x1, int x2)       {
	DEBUGLOG(0,"\n   Error: guMPTexSource is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a guMPTexSource függvényt a dgVoodoo nem támogatja");
}


void EXPORT ConvertAndDownLoadRLE(GrChipID_t        tmu,
                                FxU32             startAddress,
                                GrLOD_t           thisLod,
                                GrLOD_t           largeLod,
                                GrAspectRatio_t   aspectRatio,
                                GrTextureFormat_t format,
                                FxU32             evenOdd,
                                FxU8              *bm_data,
                                long              bm_h,
                                FxU32             u0,
                                FxU32             v0,
                                FxU32             width,
                                FxU32             height,
                                FxU32             dest_width,
                                FxU32             dest_height,
                                FxU16             *tlut) {

	DEBUGLOG(0,"\n   Error: ConvertAndDownLoadRLE is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a ConvertAndDownLoadRLE függvényt a dgVoodoo nem támogatja");
}
