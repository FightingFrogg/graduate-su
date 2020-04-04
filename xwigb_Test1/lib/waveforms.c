/* Copyright (c) Colorado School of Mines, 2002.*/
/* All rights reserved.                       */


/*********************** self documentation **********************/
/******************************************************************************
WAVEFORMS	Subroutines to define some wavelets for modeling of seimic
		data.
	
ricker1_wavelet		Compute the time response of a source function as
			a Ricker wavelet with peak frequency "fpeak" Hz.	

ricker2_wavelet		Compute a Ricke wavelet with a given period, amplitude
			and distorsion factor

akb_wavelet 		Compute the time response of a source function as
			a wavelet based on a wavelet used by Alford, Kelly, 
			and Boore.

spike_wavelet		Compute the time response of a source function as
			a spike.	

unit_wavelet 		Compute the time response of a source function as
			a constant unit shift.	

zero_wavelet		Compute the time response of a source function as
			zero everywhere.	
*******************************************************************************
Function Prototypes:
void ricker1_wavelet (int nt, float dt, float fpeak, float *wavelet);
void ricker2_wavelet (int hlw, float dt, float period, float ampl, 
	float distort, float *wavelet);
void akb_wavelet (int nt, float dt, float fpeak, float *wavelet);
void spike_wavelet (int nt, int tindex, float *wavelet);
void unit_wavelet (int nt, float *wavelet);
void zero_wavelet (int nt, float *wavelet);
******************************************************************************
Authors: Tong Fei, Ken Larner 
******************************************************************************/

/**************** end self doc ********************************/


#include "cwp.h"

/******************************************************************************
ricker_wavelet -- Compute the	time response of a source function as
	a Ricker wavelet with peak frequency "fpeak" Hz.	
*******************************************************************************
Input: 
	int nt		number samples in output wavelet
	float dt 	time step
	float fpeak	peak frequency of the Ricker wavelet
	
*******************************************************************************
Output: 
	float wavelet 	array[nt] of computed wavelet

*******************************************************************************
Author: Tong Fei, 1993, Colorado School of Mines.
******************************************************************************/
void ricker1_wavelet (int nt, float dt, float fpeak, float *wavelet)
{
	int	it;
	float	t1, t0; 

	t0=1.0/fpeak;

	for (it=0; it<nt; it++) {
		t1=it*dt;
		wavelet[it] = exp(-PI*PI*fpeak*fpeak*(t1-t0)*(t1-t0))*
			(1.0-2.*PI*PI*fpeak*fpeak*(t1-t0)*(t1-t0));
	}
}


/*****************************************************************************/
void ricker2_wavelet (int hlw, float dt, float period, float ampl, 
	float distort, float *wavelet)

/******************************************************************************
Compute Ricker wavelet
*******************************************************************************
Input:
hlw		half-length of the wavelet including center (samples)
dt		sampling interval (s)
period		wavelet period (s)
ampl		wavelet amplitude
distort		wavelet distortion factor

Output:
wavelet		Ricker wavelet
*******************************************************************************
Notes:
Creates a sampled version of the Ricker wavelet #2?? in (Ricker, N., 195?).
*******************************************************************************
Author:  Ken Larner, Colorado School of Mines, 02/26/90
Modified:
******************************************************************************/
{	
	float t=0., tnorm, xsq, wsym;
	int j;
		
	for (j=0; j<hlw; j++)   
	{	
		tnorm    = t/period;
		xsq      = (2.5*tnorm)*(2.5*tnorm);
		wsym     = ampl*(1.0 - 2.0*xsq)*exp(-xsq);
		wavelet[hlw+j-1] = wsym*(1.0 - 2.0*distort*tnorm);
		wavelet[hlw-j-1] = wsym*(1.0 + 2.0*distort*tnorm);
		t        = t + dt;	
	}
}

/******************************************************************************
* akb_wavelet -- Compute the time response of a source function as
*	a wavelet based on a wavelet used by Alford, Kelly, and Boore.
*******************************************************************************
* Input: 
*	int nt		number of samples in output wavelet
*	float dt 	time step
*	float fpeak	peak frequency of the wavelet
*	
*******************************************************************************
* Output: 
*	float wavelet 	array[nt] of computed wavelet
*
*******************************************************************************
* Reference:
*	Alford, R., Kelly, K., and Boore, D., 1974,
*	Accuracy of finite-difference modeling of the acoustic wave
*	 equation: Geophysics, vol. 39, p. 834-842.
*
* The waveform here differs from the one in Alford, Kelly, and Boore in
* that there is a time shift and an arbitrary amplitude scaling factor of 60.
*******************************************************************************
* Author: Tong Fei, 1993, Colorado School of Mines.
******************************************************************************/
void akb_wavelet (int nt, float dt, float fpeak, float *wavelet)
{
	int	it;
	float	t1; 
	float 	t0=1.8/fpeak;

	for (it=0; it<nt; it++) {
		t1=it*dt;
		wavelet[it] = -60.0*(t1-t0)*exp(-2.0*fpeak*fpeak
			*(t1-t0)*(t1-t0));
	}
}

/******************************************************************************
spike_wavelet -- Compute the time response of a source function as
	a spike.	
*******************************************************************************
Input: 
	int nt		number of time step
	int tindex	time index to locate the spike
	
Output: 
 	float wavelet	array[nt] of computed wavelet

*******************************************************************************
 Author: Tong Fei, 1993, Colorado School of Mines.
******************************************************************************/
void spike_wavelet (int nt, int tindex, float *wavelet)
{
	int	it;

	for (it=0; it<nt; it++) {
		wavelet[it] = 0.0;
	}

	wavelet[tindex]=1.0;
}


/******************************************************************************
unit_wavelet -- Compute the	time response of a source function as
	a constant unit shift.	
*************I*****************************************************************
Input: 
	int nt		number of samples in output wavelet
	
*******************************************************************************
Output: 
	float wavelet	array[nt] of computed wavelet

*******************************************************************************
 Author: Tong Fei, 1993, Colorado School of Mines.
******************************************************************************/
void unit_wavelet (int nt, float *wavelet)
{
	int	it;

	for (it=0; it<nt; it++) {
		wavelet[it] = 1.0;
	}

}



/******************************************************************************
zero_wavelet -- Compute the time response of a source function as
	zero everywhere.	
*******************************************************************************
Input: 
	int nt		number of samples in output wavelet
	
*******************************************************************************
Output: 
	float *wavelet	array[nt] of computed wavelet

*******************************************************************************
Author: Tong Fei, 1993, Colorado School of Mines.
******************************************************************************/
void zero_wavelet (int nt, float *wavelet)
{
	int	it;

	for (it=0; it<nt; it++) {
		wavelet[it] = 0.0;
	}

}
