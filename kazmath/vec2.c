/*
Copyright (c) 2008, Luke Benstead.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <assert.h>
#include <stdlib.h>

#include "mat3.h"
#include "vec2.h"
#include "utility.h"

kmVec2* kmVec2Fill(kmVec2* pOut, kmScalar x, kmScalar y)
{
    pOut->x = x;
    pOut->y = y;
    return pOut;
}

kmScalar kmVec2Length(const kmVec2* pIn)
{
    return sqrtf(kmSQR(pIn->x) + kmSQR(pIn->y));
}

kmScalar kmVec2LengthSq(const kmVec2* pIn)
{
    return kmSQR(pIn->x) + kmSQR(pIn->y);
}

kmVec2* kmVec2Normalize(kmVec2* pOut, const kmVec2* pIn)
{
	kmScalar l = 1.0f / kmVec2Length(pIn);

	kmVec2 v;
	v.x = pIn->x * l;
	v.y = pIn->y * l;
    
	pOut->x = v.x;
	pOut->y = v.y;

	return pOut;
}

kmVec2* kmVec2Add(kmVec2* pOut, const kmVec2* pV1, const kmVec2* pV2)
{
	pOut->x = pV1->x + pV2->x;
	pOut->y = pV1->y + pV2->y;

	return pOut;
}

kmScalar kmVec2Dot(const kmVec2* pV1, const kmVec2* pV2)
{
    return pV1->x * pV2->x + pV1->y * pV2->y;
}

kmVec2* kmVec2Subtract(kmVec2* pOut, const kmVec2* pV1, const kmVec2* pV2)
{
	pOut->x = pV1->x - pV2->x;
	pOut->y = pV1->y - pV2->y;

	return pOut;
}

kmVec2* kmVec2Transform(kmVec2* pOut, const kmVec2* pV, const kmMat3* pM)
{
    kmVec2 v;

    v.x = pV->x * pM->mat[0] + pV->y * pM->mat[3] + pM->mat[6];
    v.y = pV->x * pM->mat[1] + pV->y * pM->mat[4] + pM->mat[7];

    pOut->x = v.x;
    pOut->y = v.y;

    return pOut;
}

kmVec2* kmVec2TransformCoord(kmVec2* pOut, const kmVec2* pV, const kmMat3* pM)
{
	assert(0);
    return NULL;
}

kmVec2* kmVec2Scale(kmVec2* pOut, const kmVec2* pIn, const kmScalar s)
{
	pOut->x = pIn->x * s;
	pOut->y = pIn->y * s;

	return pOut;
}

int kmVec2AreEqual(const kmVec2* p1, const kmVec2* p2)
{
	return (
				(p1->x < p2->x + kmEpsilon && p1->x > p2->x - kmEpsilon) &&
				(p1->y < p2->y + kmEpsilon && p1->y > p2->y - kmEpsilon)
			);
}

/**
 * Assigns pIn to pOut. Returns pOut. If pIn and pOut are the same
 * then nothing happens but pOut is still returned
 */
kmVec2* kmVec2Assign(kmVec2* pOut, const kmVec2* pIn) {
	if (pOut == pIn) {
		return pOut;
	}

	pOut->x = pIn->x;
	pOut->y = pIn->y;

	return pOut;
}

/**
 * Rotates the point anticlockwise around a center
 * by an amount of degrees.
 *
 * Code ported from Irrlicht: http://irrlicht.sourceforge.net/
 */
kmVec2* kmVec2RotateBy(kmVec2* pOut, const kmVec2* pIn,
      const kmScalar degrees, const kmVec2* center)
{
   kmScalar x, y;
   const kmScalar radians = kmDegreesToRadians(degrees);
   const kmScalar cs = cos(radians), sn = sin(radians);

   pOut->x = pIn->x - center->x;
   pOut->y = pIn->y - center->y;

   x = pOut->x * cs - pOut->y * sn;
   y = pOut->x * sn + pOut->y * cs;

   pOut->x = x + center->x;
   pOut->y = y + center->y;

   return pOut;
}

/**
 * 	Returns the angle in degrees between the two vectors
 */
kmScalar kmVec2DegreesBetween(const kmVec2* v1, const kmVec2* v2) {
	if(kmVec2AreEqual(v1, v2)) {
		return 0.0;
	}
	
	return kmRadiansToDegrees(acos(kmVec2Dot(v1, v2)));
}

/**
 * Returns the distance between the two points
 */
kmScalar kmVec2DistanceBetween(const kmVec2* v1, const kmVec2* v2) {
	kmVec2 diff;
	kmVec2Subtract(&diff, v2, v1);
	return fabs(kmVec2Length(&diff));
}
/**
 * Returns the point mid-way between two others
 */
kmVec2* kmVec2MidPointBetween(kmVec2* pOut, const kmVec2* v1, const kmVec2* v2) {
	kmVec2 diff;
	kmVec2Subtract(&diff, v2, v1);
	kmVec2Normalize(&diff, &diff);
	kmVec2Scale(&diff, &diff, kmVec2DistanceBetween(v1, v2) * 0.5);
	kmVec2Add(pOut, v1, &diff);
	return pOut;
}
