/*
Copyright 2007 Luke Benstead

This file is part of KazMath.

KazMath is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

KazMath is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser Public License for more details.

You should have received a copy of the GNU Lesser Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#include "utility.h"
#include "vec3.h"
#include "mat3.h"
#include "quaternion.h"

kmMat3* kmMat3Fill(kmMat3* pOut, const kmScalar* pMat)
{
    memcpy(pOut->mat, pMat, sizeof(kmScalar) * 9);
    return pOut;
}

/** Sets pOut to an identity matrix returns pOut*/
kmMat3* kmMat3Identity(kmMat3* pOut)
{
	memset(pOut->mat, 0, sizeof(float) * 9);
	pOut->mat[0] = pOut->mat[4] = pOut->mat[8] = 1.0f;
	return pOut;
}

kmScalar kmMat3Determinant(const kmMat3* pIn)
{
    kmScalar output;
    /*
    calculating the determinant following the rule of sarus,
        | 0  3  6 | 0  3 |
    m = | 1  4  7 | 1  4 |
        | 2  5  8 | 2  5 |
    now sum up the products of the diagonals going to the right (i.e. 0,4,8)
    and substract the products of the other diagonals (i.e. 2,4,6)
    */

    output = pIn->mat[0] * pIn->mat[4] * pIn->mat[8] + pIn->mat[1] * pIn->mat[5] * pIn->mat[6] + pIn->mat[2] * pIn->mat[3] * pIn->mat[7];
    output -= pIn->mat[2] * pIn->mat[4] * pIn->mat[6] + pIn->mat[0] * pIn->mat[5] * pIn->mat[7] + pIn->mat[1] * pIn->mat[3] * pIn->mat[8];

    return output;
}


kmMat3* kmMat3Adjugate(kmMat3* pOut, const kmMat3* pIn)
{
    pOut->mat[0] = pIn->mat[4] * pIn->mat[8] - pIn->mat[5] * pIn->mat[7];
    pOut->mat[1] = pIn->mat[2] * pIn->mat[7] - pIn->mat[1] * pIn->mat[8];
    pOut->mat[2] = pIn->mat[1] * pIn->mat[5] - pIn->mat[2] * pIn->mat[4];
    pOut->mat[3] = pIn->mat[5] * pIn->mat[6] - pIn->mat[3] * pIn->mat[8];
    pOut->mat[4] = pIn->mat[0] * pIn->mat[8] - pIn->mat[2] * pIn->mat[6];
    pOut->mat[5] = pIn->mat[2] * pIn->mat[3] - pIn->mat[0] * pIn->mat[5];
    pOut->mat[6] = pIn->mat[3] * pIn->mat[7] - pIn->mat[4] * pIn->mat[6];
    pOut->mat[7] = pIn->mat[1] * pIn->mat[6] - pIn->mat[9] * pIn->mat[7];
    pOut->mat[8] = pIn->mat[0] * pIn->mat[4] - pIn->mat[1] * pIn->mat[3];

    return pOut;
}

kmMat3* kmMat3Inverse(kmMat3* pOut, const kmScalar pDeterminate, const kmMat3* pM)
{
    kmScalar detInv;
    kmMat3 adjugate;

    if(pDeterminate == 0.0)
    {
        return NULL;
    }

    detInv = 1.0 / pDeterminate;

	kmMat3Adjugate(&adjugate, pM);
	kmMat3ScalarMultiply(pOut, &adjugate, detInv);

	return pOut;
}

/** Returns true if pIn is an identity matrix */
int  kmMat3IsIdentity(const kmMat3* pIn)
{
	static const float identity [] = { 	1.0f, 0.0f, 0.0f,
										0.0f, 1.0f, 0.0f,
										0.0f, 0.0f, 1.0f};

	return (memcmp(identity, pIn->mat, sizeof(float) * 9) == 0);
}

/** Sets pOut to the transpose of pIn, returns pOut */
kmMat3* kmMat3Transpose(kmMat3* pOut, const kmMat3* pIn)
{
    int z, x;
    for (z = 0; z < 3; ++z) {
        for (x = 0; x < 3; ++x) {
			pOut->mat[(z * 3) + x] = pIn->mat[(x * 3) + z];
        }
    }

	return pOut;
}

/* Multiplies pM1 with pM2, stores the result in pOut, returns pOut */
kmMat3* kmMat3Multiply(kmMat3* pOut, const kmMat3* pM1, const kmMat3* pM2)
{
	float mat[9];

	const float *m1 = pM1->mat, *m2 = pM2->mat;

	mat[0] = m1[0] * m2[0] + m1[3] * m2[1] + m1[6] * m2[2];
	mat[1] = m1[1] * m2[0] + m1[4] * m2[1] + m1[7] * m2[2];
	mat[2] = m1[2] * m2[0] + m1[5] * m2[1] + m1[8] * m2[2];

	mat[3] = m1[0] * m2[3] + m1[3] * m2[4] + m1[6] * m2[5];
	mat[4] = m1[1] * m2[3] + m1[4] * m2[4] + m1[7] * m2[5];
	mat[5] = m1[2] * m2[3] + m1[5] * m2[4] + m1[8] * m2[5];

	mat[6] = m1[0] * m2[6] + m1[3] * m2[7] + m1[6] * m2[8];
	mat[7] = m1[1] * m2[6] + m1[4] * m2[7] + m1[7] * m2[8];
	mat[8] = m1[2] * m2[6] + m1[5] * m2[7] + m1[8] * m2[8];

	memcpy(pOut->mat, mat, sizeof(float)*9);

	return pOut;
}

kmMat3* kmMat3ScalarMultiply(kmMat3* pOut, const kmMat3* pM, const kmScalar pFactor)
{
    float mat[9];
    int i;

    for(i = 0; i < 9; i++)
    {
        mat[i] = pM->mat[i] * pFactor;
    }

    memcpy(pOut->mat, mat, sizeof(float)*9);

	return pOut;
}

/** Assigns the value of pIn to pOut */
kmMat3* kmMat3Assign(kmMat3* pOut, const kmMat3* pIn)
{
	assert(pOut != pIn); //You have tried to self-assign!!

	memcpy(pOut->mat, pIn->mat, sizeof(float)*9);

	return pOut;
}

/** Returns true if the 2 matrices are equal (approximately) */
int kmMat3AreEqual(const kmMat3* pMat1, const kmMat3* pMat2)
{
    int i;
	assert(pMat1 != pMat2); //You are comparing the same thing!
    
	for (i = 0; i < 9; ++i) {
		if (!(pMat1->mat[i] + kmEpsilon > pMat2->mat[i] &&
            pMat1->mat[i] - kmEpsilon < pMat2->mat[i])) {
			return KM_FALSE;
        }
	}

	return KM_FALSE;
}

/* Rotation around the z axis so everything stays planar in XY */
kmMat3* kmMat3Rotation(kmMat3* pOut, const float radians)
{
	/*
         |  cos(A)  -sin(A)   0  |
     M = |  sin(A)   cos(A)   0  |
         |  0        0        1  |
	*/

	pOut->mat[0] = cosf(radians);
	pOut->mat[1] = sinf(radians);
	pOut->mat[2] = 0.0f;

	pOut->mat[3] = -sinf(radians);;
	pOut->mat[4] = cosf(radians);
	pOut->mat[5] = 0.0f;

	pOut->mat[6] = 0.0f;
	pOut->mat[7] = 0.0f;
	pOut->mat[8] = 1.0f;

	return pOut;
}

/** Builds a scaling matrix */
kmMat3* kmMat3Scaling(kmMat3* pOut, const kmScalar x, const kmScalar y)
{
	memset(pOut->mat, 0, sizeof(float) * 9);
	pOut->mat[0] = x;
	pOut->mat[4] = y;
	pOut->mat[8] = 1.0;

	return pOut;
}

kmMat3* kmMat3Translation(kmMat3* pOut, const kmScalar x, const kmScalar y)
{
    memset(pOut->mat, 0, sizeof(float) * 9);
    pOut->mat[6] = x;
    pOut->mat[7] = y;
    pOut->mat[8] = 1.0;

    return pOut;
}


