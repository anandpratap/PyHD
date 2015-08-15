/*
 * Hyperdual type for NumPy, developed from https://github.com/numpy/numpy-dtypes  Quaternion type for NumPy  Copyright (c) 2011 Martin Ling
 * Copyright (c) 2015 Anand Pratap Singh
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
#ifndef __HYPERDUAL_H__
#define __HYPERDUAL_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	double f0;
	double f1;
	double f2;
	double f12;
} hyperdual;

int hyperdual_isnonzero(hyperdual q);
int hyperdual_isnan(hyperdual q);
int hyperdual_isinf(hyperdual q);
int hyperdual_isfinite(hyperdual q);
hyperdual hyperdual_absolute(hyperdual q);
hyperdual hyperdual_add(hyperdual lhs, hyperdual rhs);
hyperdual hyperdual_add_scalar(hyperdual lhs, double s);
hyperdual hyperdual_subtract(hyperdual lhs, hyperdual rhs);
hyperdual hyperdual_subtract_scalar(hyperdual lhs, double s);
hyperdual hyperdual_multiply(hyperdual lhs, hyperdual rhs);
hyperdual hyperdual_divide(hyperdual lhs, hyperdual rhs);
hyperdual hyperdual_multiply_scalar(hyperdual q, double s);
hyperdual hyperdual_divide_scalar(hyperdual q, double s);
hyperdual hyperdual_log(hyperdual q);
hyperdual hyperdual_exp(hyperdual q);
hyperdual hyperdual_power(hyperdual q, hyperdual p);
hyperdual hyperdual_power_scalar(hyperdual q, double p);
hyperdual hyperdual_negative(hyperdual q);
hyperdual hyperdual_conjugate(hyperdual q);
hyperdual hyperdual_copysign(hyperdual lhs, hyperdual rhs);
int hyperdual_equal(hyperdual lhs, hyperdual rhs);
int hyperdual_not_equal(hyperdual lhs, hyperdual rhs);
int hyperdual_less(hyperdual lhs, hyperdual rhs);
int hyperdual_less_equal(hyperdual lhs, hyperdual rhs);

	int hyperdual_equal_scalar(hyperdual lhs, double s);
	int hyperdual_not_equal_scalar(hyperdual lhs, double s);


	int hyperdual_less_scalar(hyperdual lhs, double s);
	int hyperdual_less_equal_scalar(hyperdual lhs, double s);

	int hyperdual_greater(hyperdual lhs, hyperdual rhs);
	int hyperdual_greater_equal(hyperdual lhs, hyperdual rhs);

	int hyperdual_greater_scalar(hyperdual lhs, double s);
	int hyperdual_greater_equal_scalar(hyperdual lhs, double s);

	

	hyperdual hyperdual_sin(hyperdual h);
	hyperdual hyperdual_cos(hyperdual h);
	hyperdual hyperdual_tan(hyperdual h);
	hyperdual hyperdual_arcsin(hyperdual h);
	hyperdual hyperdual_arccos(hyperdual h);
	hyperdual hyperdual_arctan(hyperdual h);
	hyperdual hyperdual_sqrt(hyperdual h);

#ifdef __cplusplus
}
#endif

#endif
