/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.16 $ */
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

// Primitives for methods

class methodOopPrimitives : AllStatic {
 private: 
  static void inc_calls() { number_of_calls++; }
 public:
  static int number_of_calls;

  //%prim
  // <Method> primitiveMethodSelector ^<Symbol> =
  //   Internal { name = 'methodOopPrimitives::selector' }
  //%
  static PRIM_DECL_1(selector, oop receiver);

  //%prim
  // <Method> primitiveMethodSelector: name      <Symbol>
  //                           ifFail: failBlock <PrimFailBlock> ^<Symbol> =
  //   Internal { name = 'methodOopPrimitives::setSelector' }
  //%
  static PRIM_DECL_2(setSelector, oop receiver, oop name);

  //%prim
  // <Method> primitiveMethodNumberOfArguments ^<SmallInteger> =
  //   Internal { name = 'methodOopPrimitives::numberOfArguments' }
  //%
  static PRIM_DECL_1(numberOfArguments, oop receiver);
 
  //%prim
  // <Method> primitiveMethodOuterIfFail: failBlock <PrimFailBlock> ^<Method> =
  //   Internal { error = #(ReceiverNotBlockMethod)
  //              name  = 'methodOopPrimitives::outer' }
  //%
  static PRIM_DECL_1(outer, oop receiver);

  //%prim
  // <Method> primitiveMethodOuter: method    <Method>
  //                        ifFail: failBlock <PrimFailBlock> ^<Symbol> =
  //   Internal { name = 'methodOopPrimitives::setOuter' }
  //%
  static PRIM_DECL_2(setOuter, oop receiver, oop method);

  //%prim
  // <Method> primitiveMethodReferencedInstVarNamesMixin: mixin <Mixin>
  //                                              ifFail: failBlock <PrimFailBlock> ^<IndexedInstanceVariables> =
  //   Internal { name = 'methodOopPrimitives::referenced_instance_variable_names' }
  //%
  static PRIM_DECL_2(referenced_instance_variable_names, oop receiver, oop mixin);

  //%prim
  // <Method> primitiveMethodReferencedClassVarNames ^<IndexedInstanceVariables> =
  //   Internal { name = 'methodOopPrimitives::referenced_class_variable_names' }
  //%
  static PRIM_DECL_1(referenced_class_variable_names, oop receiver);

  //%prim
  // <Method> primitiveMethodReferencedGlobalNames ^<IndexedInstanceVariables> =
  //   Internal { name = 'methodOopPrimitives::referenced_global_names' }
  //%
  static PRIM_DECL_1(referenced_global_names, oop receiver);
 
  //%prim
  // <Method> primitiveMethodSenders ^<IndexedInstanceVariables> =
  //   Internal { name = 'methodOopPrimitives::senders' }
  //%
  static PRIM_DECL_1(senders, oop receiver);

  //%prim
  // <Method> primitiveMethodPrettyPrintKlass: klass     <Object>
  //                                   ifFail: failBlock <PrimFailBlock> ^<Method> =
  //   Internal { name = 'methodOopPrimitives::prettyPrint' }
  //%
  static PRIM_DECL_2(prettyPrint, oop receiver, oop klass);

  //%prim
  // <Method> primitiveMethodPrettyPrintSourceKlass: klass     <Object>
  //                                         ifFail: failBlock <PrimFailBlock> ^<ByteIndexedInstanceVariables> =
  //   Internal { name = 'methodOopPrimitives::prettyPrintSource' }
  //%
  static PRIM_DECL_2(prettyPrintSource, oop receiver, oop klass);

  //%prim
  // <Method> primitiveMethodPrintCodes ^<Symbol> =
  //   Internal { name = 'methodOopPrimitives::printCodes' }
  //%
  static PRIM_DECL_1(printCodes, oop receiver);

  //%prim
  // <Method> primitiveMethodDebugInfo ^<Object> =
  //   Internal { name = 'methodOopPrimitives::debug_info' }
  //%
  static PRIM_DECL_1(debug_info, oop receiver);

  //%prim
  // <Method> primitiveMethodSizeAndFlags ^<Object> =
  //   Internal { name = 'methodOopPrimitives::size_and_flags' }
  //%
  static PRIM_DECL_1(size_and_flags, oop receiver);

  //%prim
  // <Method> primitiveMethodBody ^<Object> =
  //   Internal { name = 'methodOopPrimitives::fileout_body' }
  //%
  static PRIM_DECL_1(fileout_body, oop receiver);

  //%prim
  // <NoReceiver> primitiveConstructMethod: selector_or_method <Object>
  //                                 flags: flags              <SmallInteger>
  //                               nofArgs: nofArgs            <SmallInteger>
  //                             debugInfo: debugInfo          <Array>
  //                                 bytes: bytes              <ByteArray>
  //                                  oops: oops               <Array>
  //                                ifFail: failBlock          <PrimFailBlock> ^<Method> =
  //   Internal { name = 'methodOopPrimitives::constructMethod' }
  //%
  static PRIM_DECL_6(constructMethod, oop selector_or_method, oop flags, oop nofArgs, oop debugInfo, oop bytes, oop oops);

  //%prim
  // <Method> primitiveMethodAllocateBlockIfFail: failBlock <PrimFailBlock> ^<Block> =
  //   Internal {
  //     name  = 'methodOopPrimitives::allocate_block' }
  //%
  static PRIM_DECL_1(allocate_block, oop receiver);

  //%prim
  // <Method> primitiveMethodAllocateBlock: receiver <Object> ifFail: failBlock <PrimFailBlock> ^<Block> =
  //   Internal {
  //     name  = 'methodOopPrimitives::allocate_block_self' }
  //%
  static PRIM_DECL_2(allocate_block_self, oop receiver, oop self);

  //%prim
  // <Method> primitiveMethodSetInliningInfo: info      <Symbol>
  //                                  ifFail: failBlock <PrimFailBlock> ^<Symbol> =
  //   Internal { doc  = 'Sets the inlining info for the method (#Normal, #Never, or #Always)'
  //              error = #(ArgumentIsInvalid)
  //              name = 'methodOopPrimitives::set_inlining_info' }
  //%
  static PRIM_DECL_2(set_inlining_info, oop receiver, oop info);


  //%prim
  // <Method> primitiveMethodInliningInfo ^<Symbol> =
  //   Internal { doc  = 'Returns #Normal, #Never, or #Always'
  //              name = 'methodOopPrimitives::inlining_info' }
  //%
  static PRIM_DECL_1(inlining_info, oop receiver);
};
