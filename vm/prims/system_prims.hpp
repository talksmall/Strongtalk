/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.42 $ */
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


// Misc. system primitives

class systemPrimitives : AllStatic {
 private: 
  static void inc_calls() { number_of_calls++; }
 public:
  static int number_of_calls;

  // REFLECTIVE OPERATIONS
  
  //%prim
  // <NoReceiver> primitiveCreateInvocationOf: mixin      <Mixin>
  //                                    named: name       <Symbol>
  //                      isPrimaryInvocation: primary    <Boolean>
  //                               superclass: superclass <Behavior>
  //                                   format: format     <Symbol>
  //                                   ifFail: failBlock  <PrimFailBlock> ^<GlobalAssociation> =
  //   Internal { doc   = 'format: #Oops, #ExternalProxy #Process'
  //              doc   = '        #IndexedInstanceVariables #IndexedByteInstanceVariables'
  //              doc   = '        #IndexedDoubleByteInstanceVariables #IndexedNextOfKinInstanceVariables'
  //              error = #(WrongFormat) 
  //              name  = 'systemPrimitives::createNamedInvocation' }
  //%
  static PRIM_DECL_5(createNamedInvocation, oop mixin, oop name, oop primary, oop superclass, oop format);

  //%prim
  // <NoReceiver> primitiveCreateInvocationOf: mixin      <Mixin>
  //                               superclass: superclass <Behavior>
  //                                   format: format     <Symbol>
  //                                   ifFail: failBlock  <PrimFailBlock> ^<GlobalAssociation> =
  //   Internal { doc   = 'format: #Oops, #ExternalProxy #Process'
  //              doc   = '        #IndexedInstanceVariables #IndexedByteInstanceVariables'
  //              doc   = '        #IndexedDoubleByteInstanceVariables #IndexedNextOfKinInstanceVariables'
  //              error = #(WrongFormat) 
  //              name  = 'systemPrimitives::createInvocation' }
  //%
  static PRIM_DECL_3(createInvocation, oop mixin, oop superclass, oop format);

  //%prim
  // <NoReceiver> primitiveApplyChange: change        <IndexedInstanceVariables>
  //                            ifFail: failBlock     <PrimFailBlock> ^<Object> =
  //   Internal { doc  = 'Apply change to a mixin and their invocations.'
  //              doc  = 'The change is a <IndexedInstanceVariables> with the following structure:'
  //              doc  = '  [1]     = new-mixin   <Mixin>'
  //              doc  = '  [2]     = old-mixin   <Mixin>'
  //              doc  = '  [3 - n] = invocations <IndexedInstanceVariables>'
  //              doc  = 'Where the format for the a incovation is:'
  //              doc  = '  [1]     = incovation  <Class>'
  //              doc  = '  [2]     = format      <Symbol>'
  //              doc  = '  [3]     = superClass  <Class>'
  //              doc  = '  [4 - m] = {subclass, format}*  <IndexedInstanceVariables>, <Symbol>'
  //              doc  = 'The list of classes are sub classes of the invocation (topological sorted).'
  //              name = 'systemPrimitives::applyChange' }
  //%
  static PRIM_DECL_1(applyChange, oop change);

  //%prim
  // <Object> primitiveScavenge ^<Self> =
  //   Internal { name  = 'systemPrimitives::scavenge' }
  //%
  static PRIM_DECL_1(scavenge, oop receiver);

  //%prim
  // <Object> primitiveGarbageCollect ^<Self> =
  //   Internal { name  = 'systemPrimitives::garbageGollect' }
  //%
  static PRIM_DECL_1(garbageGollect, oop receiver);

  //%prim
  // <NoReceiver> primitiveExpandMemory: size <SmallInteger> ^<Object> =
  //   Internal { name  = 'systemPrimitives::expandMemory' }
  //%
  static PRIM_DECL_1(expandMemory, oop size);

  //%prim
  // <NoReceiver> primitiveShrinkMemory: size <SmallInteger> ^<Object> =
  //   Internal { name  = 'systemPrimitives::shrinkMemory' }
  //%
  static PRIM_DECL_1(shrinkMemory, oop size);

  //%prim
  // <NoReceiver> primitiveSizeOfOop ^<SmallInteger> =
  //   Internal { name  = 'systemPrimitives::oopSize' }
  //%
  static PRIM_DECL_0(oopSize);

  //%prim
  // <NoReceiver> primitiveFreeSpace ^<SmallInteger> =
  //   Internal { name  = 'systemPrimitives::freeSpace' 
  //              doc   = 'Returns the number of unused bytes in the old generation' }
  //%
  static PRIM_DECL_0(freeSpace);

  //%prim
  // <NoReceiver> primitiveNurseryFreeSpace ^<SmallInteger> =
  //   Internal { name  = 'systemPrimitives::nurseryFreeSpace' 
  //              doc   = 'Returns the number of unused bytes in the new generation' }
  //%
  static PRIM_DECL_0(nurseryFreeSpace);

  //%prim
  // <NoReceiver> primitiveExpansions ^<SmallInteger> =
  //   Internal { name  = 'systemPrimitives::expansions' 
  //              doc   = 'Returns the number of expansions of the old generation' }
  //%
  static PRIM_DECL_0(expansions);

  //%prim
  // <NoReceiver> primitiveBreakpoint ^<Object> =
  //   Internal { name  = 'systemPrimitives::breakpoint' }
  //%
  static PRIM_DECL_0(breakpoint);

  //%prim
  // <NoReceiver> primitiveVMBreakpoint ^<Object> =
  //   Internal { name  = 'systemPrimitives::vmbreakpoint' }
  //%
  static PRIM_DECL_0(vmbreakpoint);

  //%prim
  // <NoReceiver> primitiveHalt ^<Object> =
  //   Internal { name  = 'systemPrimitives::halt' }
  //%
  static PRIM_DECL_0(halt);

  //%prim
  // <NoReceiver> primitiveUserTime ^<Float> =
  //   Internal { name  = 'systemPrimitives::userTime' }
  //%
  static PRIM_DECL_0(userTime);

  //%prim
  // <NoReceiver> primitiveSystemTime ^<Float> =
  //   Internal { name  = 'systemPrimitives::systemTime' }
  //%
  static PRIM_DECL_0(systemTime);

  //%prim
  // <NoReceiver> primitiveElapsedTime ^<Float> = 
  //   Internal { name = 'systemPrimitives::elapsedTime' }
  //%
  static PRIM_DECL_0(elapsedTime);
  
  //%prim
  // <NoReceiver> primitiveWriteSnapshot: fileName <String> ^<Object> =
  //   Internal { name  = 'systemPrimitives::writeSnapshot' }
  //%
  static PRIM_DECL_1(writeSnapshot, oop fileName);

  //%prim
  // <NoReceiver> primitiveQuit ^<BottomType> =
  //   Internal { name  = 'systemPrimitives::quit' }
  //%
  static PRIM_DECL_0(quit);

  // GLOBAL ASSOCIATION

  //%prim
  // <GlobalAssociation> primitiveGlobalAssociationKey ^<Symbol> =
  //   Internal { name  = 'systemPrimitives::globalAssociationKey' }
  //%
  static PRIM_DECL_1(globalAssociationKey, oop receiver);

  //%prim
  // <GlobalAssociation> primitiveGlobalAssociationSetKey: key <Symbol> ^<Object> =
  //   Internal { name  = 'systemPrimitives::globalAssociationSetKey' }
  //%
  static PRIM_DECL_2(globalAssociationSetKey, oop receiver, oop key);

  //%prim
  // <GlobalAssociation> primitiveGlobalAssociationValue ^<Object> =
  //   Internal { name  = 'systemPrimitives::globalAssociationValue' }
  //%
  static PRIM_DECL_1(globalAssociationValue, oop receiver);

  //%prim
  // <GlobalAssociation> primitiveGlobalAssociationSetValue: value <Object> ^<Object> =
  //   Internal { name  = 'systemPrimitives::globalAssociationSetValue' }
  //%
  static PRIM_DECL_2(globalAssociationSetValue, oop receiver, oop value);

  //%prim
  // <GlobalAssociation> primitiveGlobalAssociationIsConstant ^<Boolean> =
  //   Internal { name  = 'systemPrimitives::globalAssociationIsConstant' }
  //%
  static PRIM_DECL_1(globalAssociationIsConstant, oop receiver);

  //%prim
  // <GlobalAssociation> primitiveGlobalAssociationSetConstant: value <Boolean> ^<Boolean> =
  //   Internal { name  = 'systemPrimitives::globalAssociationSetConstant' }
  //%
  static PRIM_DECL_2(globalAssociationSetConstant, oop receiver, oop value);

  // THE SMALLTALK ARRAY

  //%prim
  // <NoReceiver> primitiveSmalltalkAt: index <SmallInteger>
  //                            ifFail: failBlock <PrimFailBlock>  ^<GlobalAssociation> =
  //   Internal { doc   = 'Returns the global association at index'
  //              error = #(OutOfBounds)
  //              name  = 'systemPrimitives::smalltalk_at' }
  //%
  static PRIM_DECL_1(smalltalk_at, oop index);

  //%prim
  // <NoReceiver> primitiveSmalltalkAt: key       <Symbol>
  //                               Put: value     <Object>
  //                            ifFail: failBlock <PrimFailBlock>  ^<GlobalAssociation> =
  //   Internal { doc   = 'Add a new non-constant global association'
  //              name  = 'systemPrimitives::smalltalk_at_put' }
  //%
  static PRIM_DECL_2(smalltalk_at_put, oop key, oop value);

  //%prim
  // <NoReceiver> primitiveSmalltalkRemoveAt: index <SmallInteger>
  //                                  ifFail: failBlock <PrimFailBlock>  ^<GlobalAssociation> =
  //   Internal { doc   = 'Removes the association at index, and returns the removed element'
  //              name  = 'systemPrimitives::smalltalk_remove_at' }
  //%
  static PRIM_DECL_1(smalltalk_remove_at, oop index);

  //%prim
  // <NoReceiver> primitiveSmalltalkSize ^<SmallInteger> =
  //   Internal { name  = 'systemPrimitives::smalltalk_size' }
  //%
  static PRIM_DECL_0(smalltalk_size);
  
  //%prim
  // <NoReceiver> primitiveSmalltalkArray ^<IndexedInstanceVariables> =
  //   Internal { name  = 'systemPrimitives::smalltalk_array' }
  //%
  static PRIM_DECL_0(smalltalk_array);

  //%prim
  // <NoReceiver> primitivePrintPrimitiveTable ^<Object> =
  //   Internal { name  = 'systemPrimitives::printPrimitiveTable' }
  //%
  static PRIM_DECL_0(printPrimitiveTable);

  //%prim
  // <NoReceiver> primitivePrintMemory ^<Object> =
  //   Internal { name  = 'systemPrimitives::print_memory' }
  //%
  static PRIM_DECL_0(print_memory);

  //%prim
  // <NoReceiver> primitivePrintZone ^<Object> =
  //   Internal { name  = 'systemPrimitives::print_zone' }
  //%
  static PRIM_DECL_0(print_zone);

  // Windows specific primitives

  //%prim
  // <NoReceiver> primitiveDefWindowProc: resultProxy <Proxy>
  //                              ifFail: failBlock <PrimFailBlock>  ^<Proxy> =
  //   Internal { name  = 'systemPrimitives::defWindowProc' }
  //%
  static PRIM_DECL_1(defWindowProc, oop resultProxy);


  //%prim
  // <NoReceiver> primitiveWindowsHInstance: resultProxy <Proxy>
  //                                 ifFail: failBlock <PrimFailBlock>  ^<Proxy> =
  //   Internal { name  = 'systemPrimitives::windowsHInstance' }
  //%
  static PRIM_DECL_1(windowsHInstance, oop resultProxy);

  //%prim
  // <NoReceiver> primitiveWindowsHPrevInstance: resultProxy <Proxy>
  //                                     ifFail: failBlock <PrimFailBlock>  ^<Proxy> =
  //   Internal { name  = 'systemPrimitives::windowsHPrevInstance' }
  //%
  static PRIM_DECL_1(windowsHPrevInstance, oop resultProxy);

  //%prim
  // <NoReceiver> primitiveWindowsNCmdShow ^<Object> =
  //   Internal { name  = 'systemPrimitives::windowsNCmdShow' }
  //%
  static PRIM_DECL_0(windowsNCmdShow);

  //%prim
  // <NoReceiver> primitiveCharacterFor: value     <SmallInteger>
  //                             ifFail: failBlock <PrimFailBlock>  ^<Proxy> =
  //   Internal { error = #(OutOfBounds)
  //              name  = 'systemPrimitives::characterFor' }
  //%
  static PRIM_DECL_1(characterFor, oop value);

  //%prim
  // <NoReceiver> primitiveTraceStack ^<Object> =
  //   Internal { name  = 'systemPrimitives::traceStack' }
  //%
  static PRIM_DECL_0(traceStack);

  // FLAT PROFILER

  //%prim
  // <NoReceiver> primitiveFlatProfilerReset ^<Object> = 
  //   Internal { doc  = 'Resets the flat profiler to initial state.'
  //              name = 'systemPrimitives::flat_profiler_reset' }
  //%
  static PRIM_DECL_0(flat_profiler_reset);

  //%prim
  // <NoReceiver> primitiveFlatProfilerProcess ^<Process|nil> = 
  //   Internal { doc  = 'Returns the process beeing profiler, nil otherwise.'
  //              name = 'systemPrimitives::flat_profiler_process' }
  //%
  static PRIM_DECL_0(flat_profiler_process);

  //%prim
  // <NoReceiver> primitiveFlatProfilerEngage: process <Process>
  //                                   ifFail: failBlock <PrimFailBlock> ^<Process> = 
  //   Internal { doc  = 'Starts profiling process.'
  //              name = 'systemPrimitives::flat_profiler_engage' }
  //%
  static PRIM_DECL_1(flat_profiler_engage, oop process);

  //%prim
  // <NoReceiver> primitiveFlatProfilerDisengage ^<Process|nil> = 
  //   Internal { doc  = 'Stops profiling.'
  //              name = 'systemPrimitives::flat_profiler_disengage' }
  //%
  static PRIM_DECL_0(flat_profiler_disengage);

  //%prim
  // <NoReceiver> primitiveFlatProfilerPrint ^<Object> =
  //    Internal { doc   = 'Prints the collected profile information.'
  //               name  = 'systemPrimitives::flat_profiler_print' }
  //%
  static PRIM_DECL_0(flat_profiler_print);


  // SUPPORT FOR WEAK ARRAY NOTIFICATION
  
  //%prim
  // <NoReceiver> primitiveNotificationQueueGetIfFail: failBlock <PrimFailBlock>  ^<Object> =
  //   Internal { doc   = 'Returns the first element in the notification queue (FIFO).'
  //              error = #(EmptyQueue)
  //              name  = 'systemPrimitives::notificationQueueGet' }
  //%
  static PRIM_DECL_0(notificationQueueGet);
  
  //%prim
  // <NoReceiver> primitiveNotificationQueuePut: value <Object> ^<Object> =
  //   Internal { doc   = 'Appends the argument to the notification queue (FIFO).'
  //              name  = 'systemPrimitives::notificationQueuePut' }
  //%
  static PRIM_DECL_1(notificationQueuePut, oop value);
  
  //%prim
  // <NoReceiver> primitiveHadNearDeathExperience: value <Object> ^<Boolean> =
  //   Internal { doc  = 'Tells whether the receiver had a near death experience.'
  //              name = 'systemPrimitives::hadNearDeathExperience' }
  //%
  static PRIM_DECL_1(hadNearDeathExperience, oop value);

  // DLL support

  //%prim
  // <NoReceiver> primitiveDLLSetupLookup: receiver <Object>
  //                             selector: selector <Symbol>
  //                               ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { doc    = 'Setup call back for DLL lookup. Fails if selector does not have right number of arguments'
  //              errors = #(failed)
  //              name   = 'systemPrimitives::dll_setup' }
  //%
  static PRIM_DECL_2(dll_setup, oop receiver, oop selector);

  //%prim
  // <NoReceiver> primitiveDLLLookup: name      <Symbol> 
  //                              in: library   <Proxy>
  //                          result: entry     <Proxy>
  //                          ifFail: failBlock <PrimFailBlock> ^<Proxy> =
  //   Internal { doc    = 'Lookup name in library'
  //              errors = #(NotFound)
  //              name   = 'systemPrimitives::dll_lookup' }
  //%
  static PRIM_DECL_3(dll_lookup, oop name, oop library, oop result);

  //%prim
  // <NoReceiver> primitiveDLLLoad: name      <Symbol> 
  //                        result: library   <Proxy>
  //                        ifFail: failBlock <PrimFailBlock> ^<Proxy> =
  //   Internal { doc    = 'Load library'
  //              errors = #(NotFound)
  //              name   = 'systemPrimitives::dll_load' }
  //%
  static PRIM_DECL_2(dll_load, oop name, oop library);

  //%prim
  // <NoReceiver> primitiveDLLUnload: library <Proxy> 
  //                          ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { doc    = 'Unload the library'
  //              errors = #(Failed)
  //              name   = 'systemPrimitives::dll_unload' }
  //%
  static PRIM_DECL_1(dll_unload, oop library);

  // Inlining database

  //%prim
  // <NoReceiver> primitiveInliningDatabaseDirectory ^<Symbol> =
  //   Internal { doc    = 'Returns the directory for the external inlining database.'
  //              errors = #(Failed)
  //              name   = 'systemPrimitives::inlining_database_directory' }
  //%
  static PRIM_DECL_0(inlining_database_directory);

  //%prim
  // <NoReceiver> primitiveInliningDatabaseSetDirectory: name      <Symbol>
  //                                             ifFail: failBlock <PrimFailBlock> ^<Symbol> =
  //   Internal { doc    = 'Sets the directory for external inlining database.'
  //              doc    = 'Returns the old directory.'
  //              errors = #(Failed)
  //              name   = 'systemPrimitives::inlining_database_set_directory' }
  //%
  static PRIM_DECL_1(inlining_database_set_directory, oop name);


  //%prim
  // <NoReceiver>  primitiveInliningDatabaseFileOutClass: receiverClass <Behavior>
  //                                              ifFail: failBlock     <PrimFailBlock> ^<SmallInteger> =
  //   Internal { doc    = 'Adds inlining information to the external database for all compiled methods with the specific receiver class.'
  //              doc    = 'Returns the number of filed out structures.'
  //              errors = #(Failed)
  //              name   = 'systemPrimitives::inlining_database_file_out_class' }
  //%
  static PRIM_DECL_1(inlining_database_file_out_class, oop receiver_class);

  //%prim
  // <NoReceiver>  primitiveInliningDatabaseFileOutAllIfFail: failBlock ^<SmallInteger> =
  //   Internal { doc    = 'Adds inlining information to the external database for all compiled methods.'
  //              doc    = 'Returns the number of filed out structures.'
  //              errors = #(Failed)
  //              name   = 'systemPrimitives::inlining_database_file_out_all' }
  //%
  static PRIM_DECL_0(inlining_database_file_out_all);

  //%prim
  // <NoReceiver>  primitiveInliningDatabaseCompile: fileName  <String>
  //                                         ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { doc    = 'Compiles a method described in fileName.'
  //              errors = #(Failed)
  //              name   = 'systemPrimitives::inlining_database_compile' }
  //%
  static PRIM_DECL_1(inlining_database_compile, oop file_name);

  //%prim
  // <NoReceiver>  primitiveInliningDatabaseCompile ^<Boolean> =
  //   Internal { doc    = 'Compiles a method for the inlining database (for background compilation).'
  //              name   = 'systemPrimitives::inlining_database_compile_next' }
  //%
  static PRIM_DECL_0(inlining_database_compile_next);

  //%prim
  // <NoReceiver>  primitiveInliningDatabaseMangle: name      <String>
  //                                        ifFail: failBlock <PrimFailBlock> ^<IndexedByteInstanceVariables> =
  //   Internal { doc    = 'Returns the mangled name'
  //              name   = 'systemPrimitives::inlining_database_mangle' }
  //%
  static PRIM_DECL_1(inlining_database_mangle, oop name);

  //%prim
  // <NoReceiver>  primitiveInliningDatabaseCompileDemangled: name <String>
  //                                                  ifFail: failBlock <PrimFailBlock> ^<IndexedByteInstanceVariables> =
  //   Internal { doc    = 'Returns the demangled name'
  //              name   = 'systemPrimitives::inlining_database_demangle' }
  //%
  static PRIM_DECL_1(inlining_database_demangle, oop name);

  //%prim
  // <NoReceiver>  primitiveInliningDatabaseAddLookupEntryClass: class      <Behavior>
  //                                                   selector: selector   <Symbol>
  //                                                     ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { name   = 'systemPrimitives::inlining_database_add_entry' }
  //%
  static PRIM_DECL_2(inlining_database_add_entry, oop receiver_class, oop method_selector);


  //%prim
  // <NoReceiver>  primitiveSlidingSystemAverageIfFail: failBlock <PrimFailBlock> ^<IndexedInstanceVariables> =
  //   Internal { errors = #(NotActive)
  //              name   = 'systemPrimitives::sliding_system_average' }
  //%
  static PRIM_DECL_0(sliding_system_average);

  // Enumeration primitives

  //%prim
  // <NoReceiver> primitiveInstancesOf: class <Class>
  //                             limit: limit <SmallInteger>
  //                            ifFail: failBlock <PrimFailBlock> ^<IndexedInstanceVariables> =
  //   Internal { doc    = 'Returns an array with instances of class.'
  //              doc    = 'limit specifies the maximum number of elements.'
  //              errors = #(OutOfMemory)
  //              name   = 'systemPrimitives::instances_of' }
  //%
  static PRIM_DECL_2(instances_of, oop klass, oop limit);

  //%prim
  // <NoReceiver> primitiveReferencesTo: obj <Object>
  //                              limit: limit <SmallInteger>
  //                             ifFail: failBlock <PrimFailBlock> ^<IndexedInstanceVariables> =
  //   Internal { doc    = 'Returns an array with all objects referring obj.'
  //              doc    = 'limit specifies the maximum number of elements.'
  //              errors = #(OutOfMemory)
  //              name   = 'systemPrimitives::references_to' }
  //%
  static PRIM_DECL_2(references_to, oop obj, oop limit);

  //%prim
  // <NoReceiver> primitiveReferencesToInstancesOf: class <Class>
  //                                         limit: limit <SmallInteger>
  //                                        ifFail: failBlock <PrimFailBlock> ^<IndexedInstanceVariables> =
  //   Internal { doc    = 'Returns an array with all objects referring instances of class.'
  //              doc    = 'limit specifies the maximum number of elements.'
  //              errors = #(OutOfMemory)
  //              name   = 'systemPrimitives::references_to_instances_of' }
  //%
  static PRIM_DECL_2(references_to_instances_of, oop klass, oop limit);

  //%prim
  // <NoReceiver> primitiveAllObjectsLimit: limit <SmallInteger>
  //                                ifFail: failBlock <PrimFailBlock> ^<IndexedInstanceVariables> =
  //   Internal { doc    = 'Returns an array containing all objects.'
  //              doc    = 'limit specifies the maximum number of elements.'
  //              errors = #(OutOfMemory)
  //              name   = 'systemPrimitives::all_objects' }
  //%
  static PRIM_DECL_1(all_objects, oop limit);

  //%prim
  // <NoReceiver> primitiveFlushCodeCache ^<Object> =
  //   Internal { doc    = 'Flushes all compiled code.'
  //              name   = 'systemPrimitives::flush_code_cache' }
  //%
  static PRIM_DECL_0(flush_code_cache);

  //%prim
  // <NoReceiver> primitiveFlushDeadCode ^<Object> =
  //   Internal { doc    = 'Flushes all invalidate compiled code.'
  //              name   = 'systemPrimitives::flush_dead_code' }
  //%
  static PRIM_DECL_0(flush_dead_code);

  //%prim
  // <NoReceiver> primitiveCommandLineArgs ^<Array[String]> =
  //   Internal { doc    = 'Retrieves the command line arguments as an array of strings.'
  //              name   = 'systemPrimitives::command_line_args' }
  //%
  static PRIM_DECL_0(command_line_args);

  //%prim
  // <NoReceiver> primitiveCurrentThreadId ^<SmallInteger> =
  //   Internal { doc    = 'Retrieves an identifier for the currently executing thread.'
  //              name   = 'systemPrimitives::current_thread_id' }
  //%
  static PRIM_DECL_0(current_thread_id);

  //%prim
  // <NoReceiver> primitiveObjectMemorySize ^<Float> =
  //   Internal { doc    = 'Retrieves the current size of old space in bytes.'
  //              name   = 'systemPrimitives::object_memory_size' }
  //%
  static PRIM_DECL_0(object_memory_size);
};
