/*
 * Hyperdual type for NumPy, developed from https://github.com/numpy/numpy-dtypes  Quaternion type for NumPy  Copyright (c) 2011 Martin Ling
 * Copyright (c) 2015 Anand Pratap Singh
 * In some cases the functions are just copied from Jeffrey Fike's version to avoid possibility of bugs.
 *
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the NumPy Developers nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "hyperdual.h"
#include "math.h"

int 
hyperdual_isnonzero(hyperdual h)
{
	return h.f0 != 0;
}

int 
hyperdual_isnan(hyperdual h)
{
	return isnan(h.f0) || isnan(h.f1) || isnan(h.f2) || isnan(h.f12);
}

int
hyperdual_isinf(hyperdual h)
{
	return isinf(h.f0) || isinf(h.f1) || isinf(h.f2) || isnan(h.f12);
}

int
hyperdual_isfinite(hyperdual h)
{
	return isfinite(h.f0) && isfinite(h.f1) && isfinite(h.f2) && isfinite(h.f12);
}

hyperdual
hyperdual_absolute(hyperdual h)
{
	return hyperdual_less_scalar(h, 0.0) ? hyperdual_multiply_scalar(h, -1.0) : h;
}

hyperdual
hyperdual_add(hyperdual lhs, hyperdual rhs)
{
	return (hyperdual) {
		lhs.f0+rhs.f0,
			lhs.f1+rhs.f1,
			lhs.f2+rhs.f2,
			lhs.f12+rhs.f12,
			};
}

hyperdual
hyperdual_add_scalar(hyperdual lhs, double s)
{
	return (hyperdual) {
		lhs.f0+s,
			lhs.f1,
			lhs.f2,
			lhs.f12,
			};
}


hyperdual
hyperdual_subtract(hyperdual lhs, hyperdual rhs)
{
	return (hyperdual) {
		lhs.f0-rhs.f0,
			lhs.f1-rhs.f1,
			lhs.f2-rhs.f2,
			lhs.f12-rhs.f12,
			};
}

hyperdual
hyperdual_subtract_scalar(hyperdual lhs, double s)
{
	return (hyperdual) {
		lhs.f0-s,
			lhs.f1,
			lhs.f2,
			lhs.f12,
			};
}


hyperdual
hyperdual_multiply(hyperdual lhs, hyperdual rhs)
{
	return (hyperdual) {
		lhs.f0*rhs.f0, 
			lhs.f0*rhs.f1 + lhs.f1*rhs.f0, 
			lhs.f0*rhs.f2 + lhs.f2*rhs.f0, 
			lhs.f0*rhs.f12 + lhs.f1*rhs.f2 + lhs.f2*rhs.f1+lhs.f12*rhs.f0,
			};
}

hyperdual
hyperdual_multiply_scalar(hyperdual lhs, double s)
{
	return (hyperdual) {
		lhs.f0*s, 
		lhs.f1*s, 
		lhs.f2*s, 
		lhs.f12*s, 
		};
}


hyperdual
hyperdual_divide(hyperdual lhs, hyperdual rhs)
{
	hyperdual temp,inv;
	inv = hyperdual_power_scalar(rhs,-1);
	temp = hyperdual_multiply(lhs, inv);
	//	printf("hyperdual divide s = %f %f\n", inv.f0, lhs.f0);
	return temp;
}

hyperdual
hyperdual_divide_scalar(hyperdual lhs, double s)
{
	hyperdual temp;
	double inv;
	inv = 1.0/s;
	temp.f0 = inv*lhs.f0;
	temp.f1 = inv*lhs.f1;
	temp.f2 = inv*lhs.f2;
	temp.f12 = inv*lhs.f12;
	//	printf("scalar divide s = %f\n", s);
	return temp;
}


hyperdual
hyperdual_log(hyperdual h)
{
	double deriv1,deriv2;
	deriv1 = h.f1/h.f0;
	deriv2 = h.f2/h.f0;
	return (hyperdual) {log(h.f0), deriv1, deriv2, h.f12/h.f0 - deriv1*deriv2};
}

hyperdual
hyperdual_exp(hyperdual h)
{
	double e = exp(h.f0);
	return (hyperdual) {e, e*h.f1, e*h.f2, e*(h.f12 + h.f1*h.f2)};
}

hyperdual
hyperdual_power(hyperdual h, hyperdual p)
{
	return hyperdual_exp(hyperdual_multiply(hyperdual_log(h), p));
}

hyperdual
hyperdual_power_scalar(hyperdual h, double p)
{
	hyperdual temp;
	double deriv,hval,tol;
	hval = h.f0;
	tol = 1e-15;
	if (fabs(hval) < tol)
		{
			if (hval >= 0)
				hval = tol;
			if (hval < 0)
				hval = -tol;
		}
	deriv = p*pow(hval,(p-1));
	//temp.f0 = pow(xval,a);
	temp.f0 = pow(h.f0,p);  //Use actual x value, only use tol for derivs
	temp.f1 = h.f1*deriv;
	temp.f2 = h.f2*deriv;
	temp.f12 = h.f12*deriv + p*(p-1)*h.f1*h.f2*pow(hval,(p-2));
	return temp;
}

hyperdual
hyperdual_negative(hyperdual h)
{
	return (hyperdual) {-h.f0, -h.f1, -h.f2, -h.f12};
}

hyperdual
hyperdual_conjugate(hyperdual h)
{
	return (hyperdual) {-h.f0, -h.f1, -h.f2, -h.f12};
}

hyperdual
hyperdual_copysign(hyperdual lhs, hyperdual rhs)
{
	return (hyperdual) {
		copysign(lhs.f0, rhs.f0),
			copysign(lhs.f1, rhs.f1),
			copysign(lhs.f2, rhs.f2),
			copysign(lhs.f12, rhs.f12)
			};
}

int
hyperdual_equal(hyperdual lhs, hyperdual rhs)
{
	return 
		!hyperdual_isnan(lhs) &&
		!hyperdual_isnan(rhs) &&
		lhs.f0 == rhs.f0;
}

int
hyperdual_equal_scalar(hyperdual lhs, double s)
{
	return lhs.f0 == s;
}

int
hyperdual_not_equal(hyperdual lhs, hyperdual rhs)
{
	return 
		!hyperdual_isnan(lhs) &&
		!hyperdual_isnan(rhs) &&
		lhs.f0 != rhs.f0;
}

int
hyperdual_not_equal_scalar(hyperdual lhs, double s)
{
	return lhs.f0 != s;
}


int
hyperdual_less(hyperdual lhs, hyperdual rhs)
{
	return
		(!hyperdual_isnan(lhs) &&
		 !hyperdual_isnan(rhs)) && (lhs.f0 < rhs.f0);
}

int
hyperdual_less_scalar(hyperdual lhs, double s)
{
	return lhs.f0 < s;
}

int
hyperdual_less_equal(hyperdual lhs, hyperdual rhs)
{

	return
		(!hyperdual_isnan(lhs) &&
		 !hyperdual_isnan(rhs)) && (lhs.f0 <= rhs.f0);
}

int
hyperdual_less_equal_scalar(hyperdual lhs, double s)
{
	    
	return lhs.f0 <= s;
}




int
hyperdual_greater(hyperdual lhs, hyperdual rhs)
{
	    
	return
		(!hyperdual_isnan(lhs) &&
		 !hyperdual_isnan(rhs)) && (lhs.f0 > rhs.f0);
}

int
hyperdual_greater_scalar(hyperdual lhs, double s)
{

	return lhs.f0 > s;
}

int
hyperdual_greater_equal(hyperdual lhs, hyperdual rhs)
{

	return
		(!hyperdual_isnan(lhs) &&
		 !hyperdual_isnan(rhs)) && (lhs.f0 >= rhs.f0);
}

int
hyperdual_greater_equal_scalar(hyperdual lhs, double s)
{

	return lhs.f0 >= s;
}





hyperdual
hyperdual_sin(hyperdual h)
{
	hyperdual temp;
	double funval,deriv;
	funval = sin(h.f0);
	deriv = cos(h.f0);
	temp.f0 = funval;
	temp.f1 = deriv*h.f1;
	temp.f2 = deriv*h.f2;
	temp.f12 = deriv*h.f12 - funval*h.f1*h.f2;
	return temp;
}

hyperdual
hyperdual_cos(hyperdual h)
{
	hyperdual temp;
	double funval,deriv;
	funval = cos(h.f0);
	deriv = -sin(h.f0);
	temp.f0 = funval;
	temp.f1 = deriv*h.f1;
	temp.f2 = deriv*h.f2;
	temp.f12 = deriv*h.f12 - funval*h.f1*h.f2;
	return temp;
}


hyperdual
hyperdual_tan(hyperdual h)
{
	hyperdual temp;
	double funval,deriv;
	funval = tan(h.f0);
	deriv  = funval*funval + 1.0;
	temp.f0 = funval;
	temp.f1 = deriv*h.f1;
	temp.f2 = deriv*h.f2;
	temp.f12 = deriv*h.f12 + h.f1*h.f2*(2*funval*deriv);
	return temp;
}

hyperdual
hyperdual_arcsin(hyperdual h)
{
	hyperdual temp;
	double funval,deriv1,deriv;
	funval = asin(h.f0);
	deriv1 = 1.0-h.f0*h.f0;
	deriv = 1.0/sqrt(deriv1);
	temp.f0 = funval;
	temp.f1 = deriv*h.f1;
	temp.f2 = deriv*h.f2;
	temp.f12 = deriv*h.f12 + h.f1*h.f2*(h.f0*pow(deriv1,-1.5));
	return temp;
}

hyperdual
hyperdual_arccos(hyperdual h)
{
	hyperdual temp;
	double funval,deriv1,deriv;
	funval = acos(h.f0);
	deriv1 = 1.0-h.f0*h.f0;
	deriv = -1.0/sqrt(deriv1);
	temp.f0 = funval;
	temp.f1 = deriv*h.f1;
	temp.f2 = deriv*h.f2;
	temp.f12 = deriv*h.f12 + h.f1*h.f2*(-h.f0*pow(deriv1,-1.5));
	return temp;
}

hyperdual
hyperdual_arctan(hyperdual h)
{
	hyperdual temp;
	double funval,deriv1,deriv;
	funval = atan(h.f0);
	deriv1 = 1.0+h.f0*h.f0;
	deriv = 1.0/deriv1;
	temp.f0 = funval;
	temp.f1 = deriv*h.f1;
	temp.f2 = deriv*h.f2;
	temp.f12 = deriv*h.f12 + h.f1*h.f2*(-2*h.f0/(deriv1*deriv1));
	return temp;
}

hyperdual
hyperdual_sqrt(hyperdual h)
{
	return hyperdual_power_scalar(h,0.5);
}
