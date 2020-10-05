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
/*			 Ismeretlen funkci�j� f�ggv�nyek												*/
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
	DEBUGLOG(1,"\n   Hiba: a guMovieStart f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT guMovieStop()	{
	DEBUGLOG(0,"\n   Error: guMovieStop is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a guMovieStop f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT guMovieSetName(int x1)	{
	DEBUGLOG(0,"\n   Error: guMovieSetName is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a guMovieSetName f�ggv�nyt a dgVoodoo nem t�mogatja");
}


/* Ismeretlen funkci�j� �s param�terez�s� f�ggv�nyek (a l�nyeg az, hogy az exortt�bl�ban szerepeljenek) */
void EXPORT grSstConfigPipeLine(int unknown0, int unknown1, int unknown2)	{
	DEBUGLOG(0,"\n   Error: grSstConfigPipeLine is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a grSstConfigPipeLine f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pciOpen()	{
	DEBUGLOG(0,"\n   Error: pciOpen is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciOpen f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pciOpenEx()	{
	DEBUGLOG(0,"\n   Error: pciOpenEx is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciOpenEx f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pciClose()	{
	DEBUGLOG(0,"\n   Error: pciClose is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciClose f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pciDeviceExists(int unknown)	{
	DEBUGLOG(0,"\n   Error: pciDeviceExists is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciDeviceExists f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pciFindCard(int unknown0, int unknown1, int unknown2)	{
	DEBUGLOG(0,"\n   Error: pciFindCard is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciFindCard f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pciFindCardMulti(int unknown0, int unknown1, int unknown2, int unknown3)	{
	DEBUGLOG(0,"\n   Error: pciFindCardMulti is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciFindCardMulti f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pciFindFreeMTRR(int unknown0)	{
	DEBUGLOG(0,"\n   Error: pciFindFreeMTRR is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciFindFreeMTRR f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pciFindMTRRMatch(int unknown0, int unknown1, int unknown2, int unknown3)	{
	DEBUGLOG(0,"\n   Error: pciFindMTRRMatch is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciFindMTRRMatch f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pciFindFreeMTRRMatch(int unknown0, int unknown1, int unknown2, int unknown3)	{
	DEBUGLOG(0,"\n   Error: pciFindFreeMTRRMatch is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciFindFreeMTRRMatch f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pciGetConfigData(int unknown0, int unknown1, int unknown2, int unknown3, int unknown4)	{
	DEBUGLOG(0,"\n   Error: pciGetConfigData is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciGetConfigData f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pciSetConfigData(int unknown0, int unknown1, int unknown2, int unknown3, int unknown4)	{
	DEBUGLOG(0,"\n   Error: pciSetConfigData is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciSetConfigData f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pciGetErrorCode()	{
	DEBUGLOG(0,"\n   Error: pciGetErrorCode is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciGetErrorCode f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pciGetErrorString()	{
	DEBUGLOG(0,"\n   Error: pciGetErrorString is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciGetErrorString f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pciMapCard(int unknown0, int unknown1, int unknown2, int unknown3, int unknown4)	{
	DEBUGLOG(0,"\n   Error: pciMapCard is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciMapCard f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pciMapCardMulti(int unknown0, int unknown1, int unknown2, int unknown3, int unknown4, int unknown5)	{
	DEBUGLOG(0,"\n   Error: pciMapCardMulti is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciMapCardMulti f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pciMapPhysicalToLinear(int unknown0, int unknown1, int unknown2)	{
	DEBUGLOG(0,"\n   Error: pciMapPhysicalToLinear is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciMapPhysicalToLinear f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pciSetMTRR(int unknown0, int unknown1, int unknown2, int unknown3)	{
	DEBUGLOG(0,"\n   Error: pciSetMTRR is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciSetMTRR f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pciUnmapPhysical(int unknown0, int unknown1)	{
	DEBUGLOG(0,"\n   Error: pciUnmapPhysical is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pciUnmapPhysical f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pioInByte(int unknown0)	{
	DEBUGLOG(0,"\n   Error: pioInByte is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pioInByte f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pioInWord(int unknown0)	{
	DEBUGLOG(0,"\n   Error: pioInWord is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pioInWord f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pioInLong(int unknown0)	{
	DEBUGLOG(0,"\n   Error: pioInLong is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pioInLong f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pioOutByte(int unknown0, int unknown1)	{
	DEBUGLOG(0,"\n   Error: pioOutByte is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pioOutByte f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pioOutWord(int unknown0, int unknown1)	{
	DEBUGLOG(0,"\n   Error: pioOutWord is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pioOutWord f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT pioOutLong(int unknown0, int unknown1)	{
	DEBUGLOG(0,"\n   Error: pioOutLong is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a pioOutLong f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT grSstConfigPipeline(int x1, int x2, int x3)        {
	DEBUGLOG(0,"\n   Error: grSstConfigPipeline is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a grSstConfigPipeline f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT grSstVidMode(int x1, int x2) {
	DEBUGLOG(0,"\n   Error: grSstVidMode is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a grSstVidMode f�ggv�nyt a dgVoodoo nem t�mogatja");
}

/*void EXPORT guFBReadRegion(int x1, int x2, int x3, int x4, int x5, int x6)       {

}

void EXPORT guFBWriteRegion(int x1, int x2, int x3, int x4, int x5, int x6)       {

}*/


void EXPORT guMPInit()  {
	DEBUGLOG(0,"\n   Error: guMPInit is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a guMPInit f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT guMPDrawTriangle(int x1, int x2, int x3) {
	DEBUGLOG(0,"\n   Error: guMPDrawTriangle is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a guMPDrawTriangle f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT guMPTexCombineFunction(int x)       {
	DEBUGLOG(0,"\n   Error: guMPTexCombineFunction is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a guMPTexCombineFunction f�ggv�nyt a dgVoodoo nem t�mogatja");
}


void EXPORT guMPTexSource(int x1, int x2)       {
	DEBUGLOG(0,"\n   Error: guMPTexSource is not supported by dgVoodoo");
	DEBUGLOG(1,"\n   Hiba: a guMPTexSource f�ggv�nyt a dgVoodoo nem t�mogatja");
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
	DEBUGLOG(1,"\n   Hiba: a ConvertAndDownLoadRLE f�ggv�nyt a dgVoodoo nem t�mogatja");
}
