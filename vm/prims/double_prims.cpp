/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.27 $ */
/* Copyright (c) 2006, Sun Microsystems, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
	  disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of Sun Microsystems nor the names of its contributors may be used to endorse or promote products derived 
	  from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT 
NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE


*/

# include "incls/_precompiled.incl"
# include "incls/_double_prims.cpp.incl"
# include <math.h>
# include <float.h>

TRACE_FUNC(TraceDoublePrims, "double")

int doubleOopPrimitives::number_of_calls;

inline static oop new_double(double value) {
  doubleOop d = as_doubleOop(Universe::allocate(sizeof(doubleOopDesc)/oopSize));
  d->init_untagged_contents_mark();
  d->set_klass_field(doubleKlassObj);
  d->set_value(value);
  return d;
}

# define ASSERT_RECEIVER assert(receiver->is_double(), "receiver must be double")

# define DOUBLE_RELATIONAL_OP(op)                                     \
  ASSERT_RECEIVER;                                                    \
  if (!argument->is_double())                                         \
    return markSymbol(vmSymbols::first_argument_has_wrong_type());    \
  return doubleOop(receiver)->value() op doubleOop(argument)->value() \
         ? trueObj : falseObj

# define DOUBLE_ARITH_OP(op)                                          \
  ASSERT_RECEIVER;                                                    \
  if (!argument->is_double())                                         \
    return markSymbol(vmSymbols::first_argument_has_wrong_type());    \
  return new_double(doubleOop(receiver)->value() op doubleOop(argument)->value())


PRIM_DECL_2(doubleOopPrimitives::lessThan, oop receiver, oop argument) {
  PROLOGUE_2("lessThan", receiver, argument); 
  DOUBLE_RELATIONAL_OP(<);
}

PRIM_DECL_2(doubleOopPrimitives::greaterThan, oop receiver, oop argument) {
  PROLOGUE_2("greaterThan", receiver, argument);
  DOUBLE_RELATIONAL_OP(>);
}

PRIM_DECL_2(doubleOopPrimitives::lessThanOrEqual, oop receiver, oop argument) {
  PROLOGUE_2("lessThanOrEqual", receiver, argument);
  DOUBLE_RELATIONAL_OP(<=);
}

PRIM_DECL_2(doubleOopPrimitives::greaterThanOrEqual, oop receiver, oop argument) {
  PROLOGUE_2("greaterThanOrEqual", receiver, argument);
  DOUBLE_RELATIONAL_OP(>=);
}

PRIM_DECL_2(doubleOopPrimitives::equal, oop receiver, oop argument) {
  PROLOGUE_2("equal", receiver, argument);
  DOUBLE_RELATIONAL_OP(==);
}

PRIM_DECL_2(doubleOopPrimitives::notEqual, oop receiver, oop argument) {
  PROLOGUE_2("notEqual", receiver, argument);
  DOUBLE_RELATIONAL_OP(!=);
}

/*
PRIM_DECL_2(doubleOopPrimitives::add, oop receiver, oop argument) {
  PROLOGUE_2("add", receiver, argument);
  DOUBLE_ARITH_OP(+);
}

PRIM_DECL_2(doubleOopPrimitives::subtract, oop receiver, oop argument) {
  PROLOGUE_2("subtract", receiver, argument);
  DOUBLE_ARITH_OP(-);
}


PRIM_DECL_2(doubleOopPrimitives::multiply, oop receiver, oop argument) {
  PROLOGUE_2("multiply", receiver, argument);
  DOUBLE_ARITH_OP(*);
}

PRIM_DECL_2(doubleOopPrimitives::divide, oop receiver, oop argument) {
  PROLOGUE_2("divide", receiver, argument);
  DOUBLE_ARITH_OP(/);
}
*/

PRIM_DECL_2(doubleOopPrimitives::mod, oop receiver, oop argument) {
  PROLOGUE_2("mod", receiver, argument);
  ASSERT_RECEIVER;
  if (!argument->is_double())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  double result = fmod(doubleOop(receiver)->value(), doubleOop(argument)->value());
  return new_double(result);
}

PRIM_DECL_1(doubleOopPrimitives::cosine, oop receiver) {
  PROLOGUE_1("cosine", receiver);
  ASSERT_RECEIVER;
  return new_double(cos(doubleOop(receiver)->value()));
}

PRIM_DECL_1(doubleOopPrimitives::sine, oop receiver) {
  PROLOGUE_1("sine", receiver);
  ASSERT_RECEIVER;
  return new_double(sin(doubleOop(receiver)->value()));
}

PRIM_DECL_1(doubleOopPrimitives::tangent, oop receiver) {
  PROLOGUE_1("tangent", receiver);
  ASSERT_RECEIVER;
  return new_double(tan(doubleOop(receiver)->value()));
}


PRIM_DECL_1(doubleOopPrimitives::arcCosine, oop receiver) {
  PROLOGUE_1("arcCosine", receiver);
  ASSERT_RECEIVER;
  return new_double(acos(doubleOop(receiver)->value()));
}

PRIM_DECL_1(doubleOopPrimitives::arcSine, oop receiver) {
  PROLOGUE_1("arcSine", receiver);
  ASSERT_RECEIVER;
  return new_double(asin(doubleOop(receiver)->value()));
}

PRIM_DECL_1(doubleOopPrimitives::arcTangent, oop receiver) {
  PROLOGUE_1("arcTangent", receiver);
  ASSERT_RECEIVER;
  return new_double(atan(doubleOop(receiver)->value()));
}

PRIM_DECL_1(doubleOopPrimitives::hyperbolicCosine, oop receiver) {
  PROLOGUE_1("hyperbolicCosine", receiver);
  ASSERT_RECEIVER;
  return new_double(cosh(doubleOop(receiver)->value()));
}

PRIM_DECL_1(doubleOopPrimitives::hyperbolicSine, oop receiver) {
  PROLOGUE_1("hyperbolicSine", receiver);
  ASSERT_RECEIVER;
  return new_double(sinh(doubleOop(receiver)->value()));
}

PRIM_DECL_1(doubleOopPrimitives::hyperbolicTangent, oop receiver) {
  PROLOGUE_1("hyperbolicTangent", receiver);
  ASSERT_RECEIVER;
  return new_double(tanh(doubleOop(receiver)->value()));
}

PRIM_DECL_1(doubleOopPrimitives::sqrt, oop receiver) {
  PROLOGUE_1("sqrt", receiver);
  ASSERT_RECEIVER;
  return new_double(::sqrt(doubleOop(receiver)->value()));
}

PRIM_DECL_1(doubleOopPrimitives::squared, oop receiver) {
  PROLOGUE_1("squared", receiver);
  ASSERT_RECEIVER;
  return new_double(doubleOop(receiver)->value() * 2);
}

PRIM_DECL_1(doubleOopPrimitives::ln, oop receiver) {
  PROLOGUE_1("ln", receiver);
  ASSERT_RECEIVER;
  return new_double(log(doubleOop(receiver)->value()));
}

PRIM_DECL_1(doubleOopPrimitives::exp, oop receiver) {
  PROLOGUE_1("exp", receiver);
  ASSERT_RECEIVER;
  return new_double(::exp(doubleOop(receiver)->value()));
}

PRIM_DECL_1(doubleOopPrimitives::log10, oop receiver) {
  PROLOGUE_1("log10", receiver);
  ASSERT_RECEIVER;
  return new_double(::log10(doubleOop(receiver)->value()));
}

PRIM_DECL_1(doubleOopPrimitives::isNan, oop receiver) {
  PROLOGUE_1("isNan", receiver);
  ASSERT_RECEIVER;
  return _isnan(doubleOop(receiver)->value()) ? trueObj : falseObj;
}

PRIM_DECL_1(doubleOopPrimitives::isFinite, oop receiver) {
  PROLOGUE_1("isFinite", receiver);
  ASSERT_RECEIVER;
  return _finite(doubleOop(receiver)->value()) ? trueObj : falseObj;
}

PRIM_DECL_1(doubleOopPrimitives::floor, oop receiver) {
  PROLOGUE_1("floor", receiver);
  ASSERT_RECEIVER;
  double result = ::floor(doubleOop(receiver)->value());
  return new_double(result);
}

PRIM_DECL_1(doubleOopPrimitives::smi_floor, oop receiver) {
  PROLOGUE_1("smi_floor", receiver);
  ASSERT_RECEIVER;
  double result = ::floor(doubleOop(receiver)->value());
  if (result < 0.0) {
    if (result > smi_min) return as_smiOop((int) result);
  } else {
    if (result < smi_max) return as_smiOop((int) result);
  }
  return markSymbol(vmSymbols::conversion_failed());

}

PRIM_DECL_1(doubleOopPrimitives::ceiling, oop receiver) {
  PROLOGUE_1("ceiling", receiver);
  ASSERT_RECEIVER;
  double result = ceil(doubleOop(receiver)->value());
  return new_double(result);
}

PRIM_DECL_1(doubleOopPrimitives::exponent, oop receiver) {
  PROLOGUE_1("exponent", receiver);
  ASSERT_RECEIVER;
  int result;
  (void) frexp(doubleOop(receiver)->value(), &result);
  return as_smiOop(result);
}

PRIM_DECL_1(doubleOopPrimitives::mantissa, oop receiver) {
  PROLOGUE_1("mantissa", receiver);
  ASSERT_RECEIVER;
  int exp;
  return new_double(frexp(doubleOop(receiver)->value(), &exp));
}

PRIM_DECL_1(doubleOopPrimitives::truncated, oop receiver) {
  PROLOGUE_1("truncated", receiver);
  ASSERT_RECEIVER;
  double value = doubleOop(receiver)->value();
  return new_double(value > 0.0 ? ::floor(value) : ::ceil(value));
}

PRIM_DECL_2(doubleOopPrimitives::timesTwoPower, oop receiver, oop argument) {
  PROLOGUE_2("timesTwoPower", receiver, argument)
  ASSERT_RECEIVER;
  if (!argument->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  return new_double(ldexp(doubleOop(receiver)->value(), smiOop(argument)->value()));
}

PRIM_DECL_1(doubleOopPrimitives::roundedAsSmallInteger, oop receiver) {
  PROLOGUE_1("roundedAsSmallInteger", receiver);
  ASSERT_RECEIVER;
  if (doubleOop(receiver)->value() < 0.0) {
    double result = ::ceil(doubleOop(receiver)->value() - 0.5);
    if (result > smi_min)
    	return as_smiOop((int) result);
  } else {
    double result = ::floor(doubleOop(receiver)->value() + 0.5);
    if (result < smi_max)
    	 return as_smiOop((int) result);
  }
  return markSymbol(vmSymbols::smi_conversion_failed());
}

PRIM_DECL_1(doubleOopPrimitives::asSmallInteger, oop receiver) {
  PROLOGUE_1("asSmallInteger", receiver);
  ASSERT_RECEIVER;
  double value = doubleOop(receiver)->value();
  if (value != ::floor(value)) 
    return markSymbol(vmSymbols::smi_conversion_failed());
  if (value < 0.0) {
    if (value > smi_min)
    	return as_smiOop((int) value);
  } else {
    if (value < smi_max)
    	 return as_smiOop((int) value);
  }
  return markSymbol(vmSymbols::smi_conversion_failed());
}


PRIM_DECL_2(doubleOopPrimitives::printFormat, oop receiver, oop argument) {
  PROLOGUE_2("printFormat", receiver, argument);
  ASSERT_RECEIVER;
  const int size = 100;
  char format[size];
  
  if (argument->is_byteArray()) {
    byteArrayOop(argument)->copy_null_terminated(format, size);
  } else if (argument->is_doubleByteArray()) {
    doubleByteArrayOop(argument)->copy_null_terminated(format, size);
  } else {
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  }
  lprintf(format, doubleOop(receiver)->value());
  return receiver;
}

static int doubleToPrintString(char* buf, double x){
#ifdef WIN32

#define IS_NAN(x)		(x == _FPCLASS_SNAN || x == _FPCLASS_QNAN)
#define IS_NEGATIVE(x)	(x == _FPCLASS_NINF || x == _FPCLASS_NN || x == _FPCLASS_ND || x == _FPCLASS_NZ)
#define IS_INFINITY(x)	(x == _FPCLASS_NINF || x == _FPCLASS_PINF)
#define IS_ZERO(x)		(x == _FPCLASS_NZ	|| x == _FPCLASS_PZ)

  (void) _set_output_format(_TWO_DIGIT_EXPONENT);

  int i, fpclass;

  i = 0;
  fpclass = _fpclass(x);
  if (IS_NAN(fpclass)) {
    i += sprintf(buf+i, "%s", "NaN");
  } else {
    if (IS_NEGATIVE(fpclass)) {
	  i += sprintf(buf+i, "%c", '-');
    }
	if (IS_INFINITY(fpclass)){
      i += sprintf(buf+i, "Infinity");
    } else if (IS_ZERO(fpclass)){
      i += sprintf(buf+i, "%s", "0.0");
    } else {
      double x_abs;
      x_abs = fabs(x);
      if (x_abs >= 0.001 && x_abs < 10000000) {
        i += sprintf(buf+i, "%f", x_abs);
      } else {
        i += sprintf(buf+i, "%e", x_abs);
      }
    }
  }
  (void) _set_output_format(0);

  return i;

#undef IS_NAN
#undef IS_NEGATIVE
#undef IS_INFINITY
#undef IS_ZERO

#else
  int i, fpclass;

  i = 0;
  fpclass = fpclassify(x);
  if (fpclass == FP_NAN) {
    i += sprintf(buf+i, "%s", "NaN");
  } else {
    if (signbit(x)) {
      i += sprintf(buf+i, "%c", '-');
    }
    if (fpclass == FP_INFINITE) {
      i += sprintf(buf+i, "Infinity");
    } else if (fpclass == FP_ZERO) {
      i += sprintf(buf+i, "%s", "0.0");
    } else {
      double x_abs;
      x_abs = fabs(x);
      if (x_abs >= 0.001 && x_abs < 10000000) {
        i += sprintf(buf+i, "%f", x_abs);
      } else {
        i += sprintf(buf+i, "%e", x_abs);
      }
    }
  }
  return i;
#endif
}

PRIM_DECL_1(doubleOopPrimitives::printString, oop receiver) {
  PROLOGUE_1("printString", receiver);
  ASSERT_RECEIVER;
  ResourceMark rm;
  char* result = NEW_RESOURCE_ARRAY(char, 4*K);

  int len = doubleToPrintString(result, doubleOop(receiver)->value());
#if 0
  int len = sprintf(result, "%1.6f", doubleOop(receiver)->value());
#endif
  while (len > 1 && result[len-1] == '0' && result[len-2] != '.') len--;
  result[len] = '\0';

  BlockScavenge bs;
  byteArrayOop ba = oopFactory::new_byteArray(len);
  for (int i = 0; i < len; i++) {
    ba->byte_at_put(i+1, result[i]);
  }
  return ba;
}

PRIM_DECL_0(doubleOopPrimitives::max_value) {
  PROLOGUE_0("max_value");
  return new_double(DBL_MAX);
}

PRIM_DECL_0(doubleOopPrimitives::min_positive_value) {
  PROLOGUE_0("min_positive_value");
  return new_double(DBL_MIN);
}

PRIM_DECL_1(doubleOopPrimitives::store_string, oop receiver) {
  PROLOGUE_1("printFormat", receiver);
  ASSERT_RECEIVER;
  BlockScavenge bs;

  double value = doubleOop(receiver)->value();
  u_char* addr = (u_char*) &value;

  byteArrayOop result = oopFactory::new_byteArray(8);
  for (int index = 0; index < 8; index++) {
    result->byte_at_put(index+1, addr[index]); 
  }
  return result;
}

PRIM_DECL_3(doubleOopPrimitives::mandelbrot, oop re, oop im, oop n) {
  PROLOGUE_3("mandelbrot", re, im, n);

  if (!re->is_double()) return markSymbol(vmSymbols::first_argument_has_wrong_type ());
  if (!im->is_double()) return markSymbol(vmSymbols::second_argument_has_wrong_type());
  if (!n ->is_smi   ()) return markSymbol(vmSymbols::third_argument_has_wrong_type ());
  
  double c_re = doubleOop(re)->value();
  double c_im = doubleOop(im)->value();
  double z_re = c_re;
  double z_im = c_im;
  double d_re = z_re * z_re;
  double d_im = z_im * z_im;

  int i    = 0;
  int imax = smiOop(n)->value() - 1;
  
  while (i < imax && d_re + d_im <= 4.0) {
    z_im = 2.0 * z_re * z_im + c_im;
    z_re = d_re - d_im + c_re;
    d_re = z_re * z_re;
    d_im = z_im * z_im;
    i++;
  }

  return as_smiOop(i);
}

static void trap() { assert(false, "This primitive should be patched"); };

extern "C" oop PRIM_API double_subtract(oop receiver, oop argument) 	{ trap(); return markSymbol(vmSymbols::primitive_trap()); };
extern "C" oop PRIM_API double_divide(oop receiver, oop argument) 	{ trap(); return markSymbol(vmSymbols::primitive_trap()); };
extern "C" oop PRIM_API double_add(oop receiver, oop argument) 		{ trap(); return markSymbol(vmSymbols::primitive_trap()); };
extern "C" oop PRIM_API double_multiply(oop receiver, oop argument) 	{ trap(); return markSymbol(vmSymbols::primitive_trap()); };
extern "C" oop PRIM_API double_from_smi(oop receiver) 			{ trap(); return markSymbol(vmSymbols::primitive_trap()); };

PRIM_DECL_1(doubleOopPrimitives::isInfinity, oop receiver) {
  PROLOGUE_1("isInfinity", receiver);
  ASSERT_RECEIVER;
#ifdef WIN32
  int status = _fpclass(doubleOop(receiver)->value());
  bool isInfinite = (status == _FPCLASS_NINF || status == _FPCLASS_PINF);
#else
  bool isInfinite = isinf(doubleOop(receiver)->value()) != 0;
#endif
  return isInfinite ? trueObj : falseObj;
}

PRIM_DECL_1(doubleOopPrimitives::isNormal, oop receiver){
  PROLOGUE_1("isFinite", receiver);
  ASSERT_RECEIVER;
#ifdef WIN32
  int status = _fpclass(doubleOop(receiver)->value());
  bool isNormal = (status == _FPCLASS_NN || status == _FPCLASS_PN);
#else
  bool isNormal = isnormal(doubleOop(receiver)->value()) != 0;
#endif
  return isNormal ? trueObj : falseObj;
}


PRIM_DECL_1(doubleOopPrimitives::sign, oop receiver) {
  PROLOGUE_1("sign", receiver);
  ASSERT_RECEIVER;
#ifdef WIN32
  double signum = 0.0;
  double value = doubleOop(receiver)->value();
  int status = _fpclass(value);
  if (status == _FPCLASS_SNAN || status == _FPCLASS_QNAN) { // sign(NaN) => NaN
    signum = value;
  } else {
	if (status == _FPCLASS_NZ || status == _FPCLASS_PZ) {
	  signum = value;                                       // sign(+0) => +0.0  and	sign(-0) => -0.0
	} else {
	  signum = value < 0 ? -1.0 : 1.0;                      // sign(<0) => -1.0  and	sign(>0) => 1.0
	}
  }
#else
  double signum = 0.0;
  double value = doubleOop(receiver)->value();
  int status = fpclassify(value);
  if (status == FP_NAN) {                                   // sign(NaN) => NaN
    signum = value;
  } else {
    if (status == FP_ZERO) {
	  signum = value;                                       // sign(+0) => +0.0  and	sign(-0) => -0.0
    } else {
	  signum = value < 0 ? -1.0 : 1.0;                      // sign(<0) => -1.0  and	sign(>0) => 1.0
    }
  }
#endif
  return new_double(signum);
}

PRIM_DECL_2(doubleOopPrimitives::copySign, oop receiver, oop argument) {
  PROLOGUE_2("copySign", receiver, argument);
  ASSERT_RECEIVER;
  if (!argument->is_double()) return markSymbol(vmSymbols::first_argument_has_wrong_type());

  double x = doubleOop(receiver)->value();
  double y = doubleOop(argument)->value();
#ifdef WIN32
  return new_double(_copysign(x, y));
#else
  return new_double(copysign(x, y));
#endif
}

PRIM_DECL_2(doubleOopPrimitives::nextAfter, oop receiver, oop argument) {
  PROLOGUE_2("nextAfter", receiver, argument);
  ASSERT_RECEIVER;
  if (!argument->is_double()) return markSymbol(vmSymbols::first_argument_has_wrong_type());

  double x = doubleOop(receiver)->value();
  double y = doubleOop(argument)->value();
#ifdef WIN32
  return new_double(_nextafter(x, y));
#else
  return new_double(nextafter(x, y));
#endif
}