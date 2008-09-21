/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.50 $ */
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

#define VMSYMBOLS(template) \
  template(smi_overflow,                   "SmallIntegerOverflow")       \
  template(division_by_zero,               "DivisionByZero")             \
  template(receiver_has_wrong_type,        "ReceiverHasWrongType")       \
  template(division_not_exact,             "DivisionNotExact")           \
  template(first_argument_has_wrong_type,  "FirstArgumentHasWrongType")  \
  template(second_argument_has_wrong_type, "SecondArgumentHasWrongType") \
  template(third_argument_has_wrong_type,  "ThirdArgumentHasWrongType")  \
  template(fourth_argument_has_wrong_type, "FourthArgumentHasWrongType") \
  template(fifth_argument_has_wrong_type,  "FifthArgumentHasWrongType")  \
  template(sixth_argument_has_wrong_type,  "SixthArgumentHasWrongType")  \
  template(seventh_argument_has_wrong_type,"SixthArgumentHasWrongType")  \
  template(eighth_argument_has_wrong_type, "SeventhArgumentHasWrongType")\
  template(ninth_argument_has_wrong_type,  "EighthArgumentHasWrongType") \
  template(tenth_argument_has_wrong_type,  "NinthArgumentHasWrongType")  \
  template(argument_has_wrong_type,  	   "ArgumentHasWrongType")       \
  template(argument_is_invalid,            "ArgumentIsInvalid")          \
  template(conversion_failed,		"ConversionFailed")		\
  template(not_found,			"NotFound")			\
  template(plus,                         "+")                      	\
  template(minus,                        "-")                      	\
  template(multiply,                     "*")                      	\
  template(divide,                       "//")                     	\
  template(mod,                          "\\\\")                   	\
  template(point_at,                     "@")                      	\
  template(equal,                        "=")                      	\
  template(not_equal,                    "~=")                     	\
  template(less_than,                    "<")                      	\
  template(less_than_or_equal,           "<=")                     	\
  template(greater_than,                 ">")                      	\
  template(greater_than_or_equal,        ">=")                     	\
  template(bitAnd_,                      "bitAnd:")                	\
  template(bitOr_,                       "bitOr:")                 	\
  template(bitXor_,                      "bitXor:")                	\
  template(bitShift_,                    "bitShift:")              	\
  template(bitInvert,                    "bitInvert")              	\
  template(and_,  	                 "and:")       			\
  template(or_,				 "or:")  	           	\
  template(_and,  	                 "&&")        			\
  template(_or,				 "||")    	         	\
  template(and1,  	                 "&")        			\
  template(or1,				 "|")  	 	          	\
  template(_not,  	                 "not")        			\
  template(xor_,  	                 "xor:")       			\
  template(eqv_,  	                 "eqv:")       			\
  template(at,                           "at:")                    	\
  template(at_put,                       "at:put:")                	\
  template(size  ,                       "size")                   	\
  template(double_equal,                 "==")                     	\
  template(double_tilde,                 "~~")                     	\
  template(out_of_bounds,                "OutOfBounds")                       \
  template(index_not_valid,              "IndexNotValid")                     \
  template(not_yet_implemented,          "NotYetImplemented")                 \
  template(indexable,                    "Indexable")                         \
  template(not_oops,                     "ReceiverNotOops")                   \
  template(not_clonable,                 "ReceiverNotClonable")               \
  template(value_out_of_range,           "ValueOutOfRange")                   \
  template(method_construction_failed,   "MethodConstructionFailed")          \
  template(primitive_not_defined,        "PrimitiveNotDefined")               \
  template(smi_conversion_failed,        "SmallIntegerConversionFailed")      \
  template(not_klass,                    "NotAClass")                         \
  template(null_proxy_access,            "NullProxyAccess")                   \
  template(selector_for_blockMethod,     "block")                             \
  template(not_indexable,                "NotIndexable")                      \
  template(to_by_do,                     "to:by:do:")                         \
  template(value,                        "value")                             \
  template(value_,                       "value:")                            \
  template(if_true,			 "ifTrue:")                           \
  template(if_true_false,                "ifTrue:ifFalse:")                   \
  template(if_false,                     "ifFalse:")                          \
  template(if_false_true,                "ifFalse:ifTrue:")                   \
  template(while_true,			 "whileTrue")                         \
  template(while_true_,                  "whileTrue:")                        \
  template(while_false,                  "whileFalse")                        \
  template(while_false_,                 "whileFalse:")                       \
  template(empty_queue,                  "EmptyQueue:")                       \
  template(is_absent,                    "IsAbsent")                          \
  template(in_scheduler,                 "InScheduler")                       \
  template(in_async_dll,                 "InAsyncDLL")                        \
  template(not_in_scheduler,             "NotInScheduler")                    \
  template(initialized,                  "Initialized")                       \
  template(yielded,                      "Yielded")                           \
  template(running,                      "Running")                           \
  template(stopped,                      "Stopped")                           \
  template(preempted,                    "Preempted")                         \
  template(completed,                    "Completed")                         \
  template(dead,                         "Dead")                              \
  template(aborted,                      "Aborted")                           \
  template(boolean_error,                "BooleanError")                      \
  template(lookup_error,                 "LookupError")                       \
  template(primitive_lookup_error,       "PrimitiveLookupError")              \
  template(DLL_lookup_error,             "DLLLookupError")                    \
  template(NLR_error,                    "NLRError")                          \
  template(float_error,                  "FloatError")                        \
  template(stack_overflow,               "StackOverflow")                     \
  template(is_installed,                 "IsInstalled")                       \
  template(failed,                       "Failed")                            \
  template(abs,                          "abs")                               \
  template(negated,                      "negated")                           \
  template(squared,                      "squared")                           \
  template(sqrt,                         "sqrt")                              \
  template(sin,                          "sin")                               \
  template(cos,                          "cos")                               \
  template(tan,                          "tan")                               \
  template(exp,                          "exp")                               \
  template(ln,                           "ln")                                \
  template(as_float,                     "asFloat")                           \
  template(as_float_value,               "asFloatValue")                      \
  template(negative_size,                "NegativeSize")                      \
  template(not_active,                   "NotActive")                         \
  template(primitive_disabled_in_product,"PrimitiveDisabledInProduct")        \
  template(activation_is_invalid,        "ActivationIsInvalid")               \
  template(external_activation,          "ExternalActivation")                \
  template(mem_klass,                    "Oops")                              \
  template(association_klass,            "GlobalAssociation")                 \
  template(blockClosure_klass,           "Block")                             \
  template(byteArray_klass,              "ByteArray")                         \
  template(symbol_klass,                 "Symbol")                            \
  template(context_klass,                "Context")                           \
  template(doubleByteArray_klass,        "DoubleByteArray")                   \
  template(doubleValueArray_klass,       "FloatValueArray")                   \
  template(double_klass,                 "Float")                             \
  template(klass_klass,                  "Class")                             \
  template(method_klass,                 "Method")                            \
  template(mixin_klass,                  "Mixin")                             \
  template(objArray_klass,               "Array")                             \
  template(weakArray_klass,              "WeakArray")                         \
  template(process_klass,                "Process")                           \
  template(vframe_klass,                 "Activation")                        \
  template(proxy_klass,                  "Proxy")                             \
  template(smi_klass,                    "SmallInteger")                      \
  template(failed_allocation,            "FailedAllocation")                  \
								              \
  /* for compiler's type prediction */					      \
  template(error,      "error")        					      \
  template(error_,     "error:")       					      \
  template(subclassResponsibility,      "subclassResponsibility")	      \


#define VMSYMBOL_POSTFIX  _enum
#define VMSYMBOL_ENUM_NAME(name) name##VMSYMBOL_POSTFIX

#define VMSYMBOL_ENUM(name,string) VMSYMBOL_ENUM_NAME(name),

enum {
  VMSYMBOLS(VMSYMBOL_ENUM)
  terminating_enum
};

#define VMSYMBOL_DECL(name, string) \
  static symbolOop name () { return vm_symbols[VMSYMBOL_ENUM_NAME(name)]; }

extern "C" symbolOop vm_symbols[];

class vmSymbols : AllStatic {
  friend void vmSymbols_init();
 public:
  static void initialize();
  VMSYMBOLS(VMSYMBOL_DECL)

  // operations for memory management.
  static void follow_contents();
  static void switch_pointers(oop from, oop to);
  static void relocate();
  static void verify();
};

#undef VMSYMBOL_DECL
#undef VMSYMBOL_ENUM

// for primitive failures
inline oop markSymbol(symbolOop sym) {
  assert(sym->is_mem(), "must have mem tag");
  return oop((char*) sym - Mem_Tag + Mark_Tag);
}

inline symbolOop unmarkSymbol(oop sym) {
  assert(sym->is_mark(), "must have mark tag");
  oop res = oop((char*) sym - Mark_Tag + Mem_Tag);
  assert(res->is_symbol(), "must be symbol");
  return symbolOop(res);
}
