/* Copyright (c) Colorado School of Mines, 2002.*/
/* All rights reserved.                       */

/*********************** self documentation **********************/
/********************************************************************
IBMFLT_ - Convert IBM tape format to floats and back

	IBMFLT  -  convert IBM tape format to floats and back

int IBMFLT(float *in, float *out, int *nwds, int *idirec);
*********************************************************************
Input:
idirec		=1  IBM to T3E ,  =-1  T3E to IBM
in		pointer to input
out		pointer to output

*********************************************************************
Author: Stew Levin (translated from fortran by f2c)
********************************************************************/
/**************** end self doc ********************************/
/* See if this fits the bill for your needs - Stew */

/* ibmflt.f -- translated by f2c (version 19951025).
*/

#include <stdlib.h>

typedef int integer;
typedef float real;
typedef int logical;

/* Table of constant values */

static integer c__1 = 1;
static integer c__23 = 23;
static integer c__2 = 2;

#define shiftl_(a,b) (((a)[0])<<((b)[0]))
#define shiftr_(a,b) (((a)[0])>>((b)[0]))


/* idirec=1  IBM-->T3E */
/* idirec=-1 T3E-->IBM */

/* Author:  Stewart A. Levin    MTC/SRC     salevin@dal.mobil.com */

/* Subroutine */
int IBMFLT(real *in, real *out, integer *nwds, integer *idirec)
{
    /* Initialized data */

    static logical first = 1;

    /* System generated locals */
    integer i__1;
    static real equiv_0[1];

    /* Local variables */
    static integer eibm, itab, emod, i__, mhibm, m1[1024]	/* was [512][
	    2] */;
    static real r1[1024]	/* was [512][2] */;
#define ii ((integer *)equiv_0)
    static integer jj;
#define rr (equiv_0)
/*    extern integer shiftl_(integer *, integer *), shiftr_(integer *, integer *
	    ); */
    static real r_infinity__;
    static integer et3e;

    /* Parameter adjustments */
    --out;
    --in;

    /* Function Body */

    if (first) {
	first = ! first;
	*ii = 2139095039;
	r_infinity__ = *rr;
	for (i__ = 0; i__ <= 511; ++i__) {
	    i__1 = i__ & 255;
	    eibm = shiftr_(&i__1, &c__1);
	    mhibm = i__ & 1;
	    et3e = (eibm << 2) - 130;
	    if (et3e > 0 && et3e <= 255) {
		i__1 = et3e ^ i__ & 255;
		m1[i__] = shiftl_(&i__1, &c__23);
		if (mhibm == 1) {
		    r1[i__] = 0.f;
		} else {
		    i__1 = et3e | i__ & 256;
		    *ii = shiftl_(&i__1, &c__23);
		    r1[i__] = -(*rr);
		}
	    } else if (et3e <= 0) {
		m1[i__] = shiftl_(&i__, &c__23);
		r1[i__] = 0.f;
	    } else {
		m1[i__] = shiftl_(&i__, &c__23);
		if (i__ < 256) {
		    r1[i__] = r_infinity__;
		} else {
		    r1[i__] = -r_infinity__;
		}
	    }
	    if (i__ == 0) {
		m1[i__ + 512] = 0;
		r1[i__ + 512] = 0.f;
	    } else {
		et3e = i__ & 255;
		i__1 = et3e + 133;
		eibm = shiftr_(&i__1, &c__2);
		emod = (eibm << 2) - et3e - 130;
		if (emod == 0) {
		    i__1 = et3e ^ (eibm << 1) + 1;
		    m1[i__ + 512] = shiftl_(&i__1, &c__23);
		    r1[i__ + 512] = 0.f;
		} else {
		    i__1 = et3e ^ (eibm << 1) - emod;
		    m1[i__ + 512] = shiftl_(&i__1, &c__23);
		    i__1 = (256 & i__) + (eibm << 1);
		    *ii = shiftl_(&i__1, &c__23);
		    r1[i__ + 512] = *rr;
		}
	    }
/* L10: */
	}
    }

    if (*idirec >= 0) {
	itab = 1;
    } else {
	itab = 2;
    }
    i__1 = *nwds;
    for (i__ = 1; i__ <= i__1; ++i__) {
	*rr = in[i__];
/* use 9 high bits for table lookup */
	jj = 511 & shiftr_(ii, &c__23);
/* fix up exponent */
	*ii = m1[jj + (itab << 9) - 512] ^ *ii;
/* fix up mantissa */
	out[i__] = *rr + r1[jj + (itab << 9) - 512];
/* L20: */
    }

    return EXIT_SUCCESS;
} /* ibmflt_ */

#undef rr
#undef ii

