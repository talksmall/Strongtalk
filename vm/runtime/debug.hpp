/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.158 $ */
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

# include <string.h>

// Debug flags control various aspects of the VM and are
// accessible by Delta programs.


// use FlagSetting to temporarily change some debug flag

class FlagSetting {
  bool val;
  bool* flag;
 public:
  FlagSetting(bool& fl, bool newValue)			{ flag = &fl; val = fl; fl = newValue; }
  ~FlagSetting()					{ *flag = val; }
};


class debugFlags {
 public:
  static bool boolAt(char* name, int len, bool* value);
  static bool boolAt(char* name, bool* value)		{ return boolAt(name, strlen(name), value); }

  static bool boolAtPut(char* name, int len, bool* value);
  static bool boolAtPut(char* name, bool* value)	{ return boolAtPut(name, strlen(name), value); }

  static bool intAt(char* name, int len, int* value);
  static bool intAt(char* name, int* value)		{ return intAt(name, strlen(name), value); }

  static bool intAtPut(char* name, int len, int* value);
  static bool intAtPut(char* name, int* value)		{return intAtPut(name, strlen(name), value); }

  static void printFlags();
  static void print_on(outputStream* st);
  static void print_diff_on(outputStream* st);
};


// Note: develop flags do not show up in the flag table of the product version
//       product flags do show up always in the flag table

#define APPLY_TO_BOOLEAN_FLAGS(develop,product)                                                                                       \
/*        name                               default  documentation */                                                                \
                                                                                                                                      \
  develop(PrintUnwind                        , false, "Print a message every 100 unwinds"                                           ) \
  develop(FlagsInitialized                   , true,  "Not to be set directly - used to indicate that flags have been initialized"  ) \
  develop(AllowGlobalNew                     , true,  "Allow allocations on the 'C' heap - use for testing"                         ) \
  develop(ZapResourceArea                    , false, "Zap the resource area when deallocated"                                      ) \
  develop(PrintResourceAllocation            , false, "Print each resource area allocation"                                         ) \
  develop(PrintResourceChunkAllocation       , false, "Print each resource area chunk allocation"                                   ) \
  develop(PrintHeapAllocation                , false, "Print heap alloction"                                                        ) \
  develop(PrintOopAddress                    , false, "Always print the location of the oop"                                        ) \
  develop(PrintObjectID                      , true , "Always prepend object ID when printing"                                      ) \
  develop(PrintLongFrames                    , false, "Print tons of details in VM stack traces"                                    ) \
  develop(LogVMMessages                      , true , "Log all vm messages to a file"                                               ) \
  develop(AlwaysFlushVMMessages              , true , "Flush VM message log after every line"                                       ) \
  develop(VerifyBeforeScavenge               , false, "Verify system before scavenge"                                               ) \
  develop(VerifyAfterScavenge                , false, "Verify system after scavenge"                                                ) \
  product(PrintScavenge                      , false, "Print message at scavenge"                                                   ) \
  product(PrintGC                            , true , "Print message at garbage collect"                                            ) \
  develop(WizardMode                         , false, "Wizard debugging mode"                                                       ) \
  develop(VerifyBeforeGC                     , false, "Verify system before garbage collect"                                        ) \
  develop(VerifyAfterGC                      , false, "Verify system after garbage collect"                                         ) \
  develop(VerifyZoneOften                    , false, "Verify compiled-code zone often"                                             ) \
  develop(PrintVMMessages                    , true , "Print vm messages on console"                                                ) \
  develop(CompiledCodeOnly                   , false, "Use compiled code only"                                                      ) \
  product(UseRecompilation                   , true , "Automatically (re-)compile frequently-used methods"                          ) \
  develop(UseNMethodAging                    , true , "Age nmethods before recompiling them"                                        ) \
  develop(UseInlineCaching                   , true , "Use inline caching in compiled code"                                         ) \
  develop(EnableTasks                        , true , "Enable periodic tasks to be performed"                                       ) \
  develop(CompressPcDescs                    , true , "ScopeDescRecorder: Compress PcDescs by ignoring multiple entries at same offset") \
  develop(UseAccessMethods                   , true , "Use access methods"                                                          ) \
  develop(UsePredictedMethods                , true , "Use predicted methods"                                                       ) \
  develop(UsePrimitiveMethods                , false, "Use primitive methods"                                                       ) \
  develop(PrintStackAtScavenge               , false, "Print stack at Scavenge"                                                     ) \
  develop(PrintInterpreter                   , false, "Prints the generated interpreter's code"                                     ) \
  develop(PrintStubRoutines                  , false, "Prints the stub routine's code"                                              ) \
  product(UseInliningDatabase                , false, "Use the inlining database for recompilation"                                 ) \
  product(UseInliningDatabaseEagerly         , false, "Use the inlining database eagerly at lookup"                                 ) \
  develop(UseSlidingSystemAverage            , true , "Compute sliding system average on the fly"                                   ) \
  develop(UseGlobalFlatProfiling             , true , "Include all processes when flat-profiling"                                   ) \
  develop(EnableOptimizedCodeRecompilation   , true , "Enable recompilation of optimized code"                                      ) \
  develop(CountParentLinksAsOne	             , true , "Count going up to parent frame as 1 when checking MaxInterpretedSearchLength during recompilee search"                                      ) \
                                                                                                                                      \
  develop(GenerateSmalltalk                  , false, "Generate Smalltalk output for file_in"                                       ) \
  develop(GenerateHTML                       , false, "Generate HTML output for documentation"                                      ) \
                                                                                                                                      \
  develop(UseTimers                          , true , "Tells whether the VM should use timers (only used at startup)"               ) \
  develop(SweeperUseTimer                    , true , "Tells whether the sweeper should use timer interrupts or compile events"     ) \
                                                                                                                                      \
/* transition */                                                                                                                      \
  develop(HasActivationClass                 , true,  "Transition variable: tell whether the system has an activation class"        ) \
                                                                                                                                      \
/* tracing */                                                                                                                         \
  develop(TraceOopPrims                      , false, "Trace Oop primitives"                                                        ) \
  develop(TraceDoublePrims                   , false, "Trace Double primitives"                                                     ) \
  develop(TraceByteArrayPrims                , false, "Trace ByteArray primitives"                                                  ) \
  develop(TraceDoubleByteArrayPrims          , false, "Trace DoubleByteArray primitives"                                            ) \
  develop(TraceDoubleValueArrayPrims         , false, "Trace DoubleByteArray primitives"                                            ) \
  develop(TraceObjArrayPrims                 , false, "Trace objArray primitives"                                                   ) \
  develop(TraceSmiPrims                      , false, "Trace SmallInteger primitives"                                               ) \
  develop(TraceProxyPrims                    , false, "Trace Proxy primitives"                                                      ) \
  develop(TraceBehaviorPrims                 , false, "Trace behavior primitives"                                                   ) \
  develop(TraceBlockPrims                    , false, "Trace block primitives"                                                      ) \
  develop(TraceDebugPrims                    , false, "Trace debug primitives"                                                      ) \
  develop(TraceSystemPrims                   , false, "Trace system primitives"                                                     ) \
  develop(TraceProcessPrims                  , false, "Trace process primitives"                                                    ) \
  develop(TraceVframePrims                   , false, "Trace vframe primitives"                                                     ) \
  develop(TraceCallBackPrims                 , false, "Trace callBack primitives"                                                   ) \
  develop(TraceLookup                        , false, "Trace lookups"                                                               ) \
  develop(TraceLookup2                       , false, "Trace lookups in excruciating detail"                                        ) \
  develop(TraceLookupAtMiss                  , false, "Trace lookups at lookup cache miss"                                          ) \
  develop(TraceBytecodes                     , false, "Trace byte code execution"                                                   ) \
  develop(TraceAllocation                    , false, "Trace allocation"                                                            ) \
  develop(TraceBootstrap                     , false, "Trace the bootstrap readin"                                                  ) \
  develop(TraceMethodPrims                   , false, "Trace method prims"                                                          ) \
  develop(TraceMixinPrims                    , false, "Trace mixin prims"                                                           ) \
  develop(TraceVMOperation                   , false, "Trace vm operations"                                                         ) \
  develop(TraceDLLLookup                     , false, "Trace DLL function lookup"                                                   ) \
  develop(TraceDLLCalls                      , false, "Trace DLL function calls"                                                    ) \
  product(TraceGC                            , true , "Trace Garbage Collection"                                                    ) \
  develop(TraceMessageSend                   , false, "Trace all message sends"                                                     ) \
  develop(TraceInlineCacheMiss               , false, "Trace inline cache misses"                                                     ) \
  develop(TraceProcessEvents                 , false, "Trace all process events"                                                    ) \
  develop(TraceDeoptimization                , false, "Trace deoptimizion"                                                          ) \
  develop(TraceZombieCreation                , false, "Trace nmethod zombie creation"                                               ) \
  develop(TraceResults                       , false, "Trace nmethod results"                                                       ) \
  develop(TraceApplyChange                   , false, "Trace reflective operation"                                                  ) \
  develop(TraceInliningDatabase              , false, "Trace inlining database"                                                     ) \
                                                                                                                                      \
/* Flags for displaying activations */                                                                                                \
  develop(ActivationShowExpressionStack      , false, "Show expression stack for activation"                                        ) \
  develop(ActivationShowBCI                  , false, "Show current bci for activation"                                             ) \
  develop(ActivationShowFrame                , false, "Show frame for activation"                                                   ) \
  develop(ActivationShowContext              , false, "Show context if any for activation"                                          ) \
  develop(ActivationShowCode                 , false, "Show pretty printed code for activation"                                     ) \
  develop(ActivationShowNameDescs            , false, "Show name desc in the printed code"                                          ) \
                                                                                                                                      \
/* error handling */                                                                                                                  \
  develop(ShowMessageBoxOnError              , false, "Show a message box on error"                                                 ) \
  develop(BreakAtWarning                     , false, "Interrupt execution at warning?"                                             ) \
  develop(PrintCompilerWarnings              , true , "Print compiler warning?"                                                     ) \
  develop(PrintInliningDatabaseCompilation   , false, "Print inlining database compilions?"                                         ) \
                                                                                                                                      \
/* statistics */                                                                                                                      \
  develop(CountBytecodes                     , false, "Count nof. bytecodes executed"                                               ) \
                                                                                                                                      \
/* profiler */                                                                                                                        \
  develop(ProfilerShowMethodHolder           , true , "Show method holder for method"                                               ) \
                                                                                                                                      \
/* compiled code */                                                                                                                   \
  develop(UseMICs                            , true , "Use megamorphic PICs (MICs)"                                                 ) \
  develop(UseLRUInterrupts                   , true , "User timers for zone LRU info"                                               ) \
  develop(UseNewBackend                      , false, "Use new backend"                                                             ) \
  develop(TryNewBackend                      , false, "Use new backend & set additional flags as needed for compilation"            ) \
  develop(UseFPUStack                        , false, "Use FPU stack for floats (unsafe)"                                           ) \
  develop(ReorderBBs                         , true , "Reorder BBs"                                                                 ) \
  product(CodeForP6                          , false, "Minimize use of byte registers in code generation for P6"                    ) \
  develop(PrintInlineCacheInvalidation       , false, "Print ic invalidation"                                                       ) \
  develop(PrintCodeReclamation               , false, "Print code reclamation"                                                      ) \
  develop(PrintCodeSweep                     , false, "Print sweeps through zone/methods"                                           ) \
  develop(PrintCodeCompaction                , false, "Print code compaction"                                                       ) \
  develop(PrintMethodFlushing                , false, "Print method flushing"                                                       ) \
  develop(MakeBlockMethodZombies             , false, "Make block nmethod zombies if needed"                                        ) \
                                                                                                                                      \
/* compiler debugging */                                                                                                              \
  develop(CompilerDebug                      , false, "Make compiler debugging easier"                                              ) \
  develop(EnableInt3                         , true , "Enables/disables code generation for int3 instructions"                      ) \
  develop(VerifyCode                         , false, "Generates verification code in compiled code"                                ) \
  develop(VerifyDebugInfo                    , false, "Verify compiled-code debug info at each call (very slow)"                    ) \
  develop(GenTraceCalls                      , false, "Generate code for TraceCalls"                                                ) \
  develop(TraceCalls                         , false, "Print non-inlined calls/returns (must compile with VerifyCode or GenTraceCalls)") \
  develop(MaterializeEliminatedBlocks        , true , "Create fake blocks for eliminated blocks when printing stack"                ) \
  develop(Inline                             , true , "Inline message sends"                                                        ) \
  develop(InlinePrims                        , true , "Inline some primitive calls"                                                 ) \
  develop(ConstantFoldPrims                  , true , "Constant-fold primitive calls"                                               ) \
  develop(TypePredict                        , true , "Predict smi/bool/array message sends"                                        ) \
  develop(TypePredictArrays                  , false, "Predict at:/at:Put: message sends"                                           ) \
  develop(TypeFeedback                       , true , "use type feedback data"                                                      ) \
  develop(CodeSizeImpactsInlining            , true , "code size is used as parameter to guide inlining"                            ) \
  develop(OptimizeIntegerLoops               , true,  "optimize integer loops"                                                      ) \
  develop(OptimizeLoops                      , true,  "optimize loops (hoist type tests"                                            ) \
  develop(EliminateJumpsToJumps              , true , "Eliminate jumps to jumps"                                                    ) \
  develop(EliminateContexts                  , true , "Eliminate context allocations"                                               ) \
  develop(LocalCopyPropagate                 , true , "Perform local copy propagation"                                              ) \
  develop(GlobalCopyPropagate                , true , "Perform global copy propagation"                                             ) \
  develop(BruteForcePropagate                , false, "Perform brute-force global copy propagation (UNSAFE  -Urs 5/3/96)"           ) \
  develop(Splitting                          , true , "Perform message splitting"                                                   ) \
  develop(EliminateUnneededNodes             , true , "Eliminate dead code"                                                         ) \
  develop(DeferUncommonBranches              , true , "Don't generate code for uncommon cases"                                      ) \
  develop(MemoizeBlocks                      , true , "memoize (delay creation of) blocks"                                          ) \
  develop(DebugPerformance                   , false, "Print info useful for performance debugging"                                 ) \
  develop(PrintInlining                      , false, "Print info about inlining"                                                   ) \
  develop(PrintSplitting                     , false, "Print info about boolean splitting"                                          ) \
  develop(PrintLocalAllocation               , false, "Print info about local register allocation"                                  ) \
  develop(PrintGlobalAllocation              , false, "Print info about global register allocation"                                 ) \
  develop(PrintEliminateContexts             , false, "Print info about eliminating context allocations"                            ) \
  product(PrintCompilation                   , false, "Print each compilation"                                                      ) \
  develop(PrintRecompilation                 , false, "Print each recompilation"                                                    ) \
  develop(PrintRecompilation2                , false, "Print details about each recompilation"                                      ) \
  develop(PrintCode                          , false, "Print intermediate code"                                                     ) \
  develop(PrintAssemblyCode                  , false, "Print assembly code"                                                         ) \
  develop(PrintJumpElimination               , false, "Print eliminated jumps"                                                      ) \
  develop(PrintRegAlloc                      , false, "Print register allocation"                                                   ) \
  develop(PrintCopyPropagation               , false, "Print info about copy propagation"                                           ) \
  develop(PrintUncommonBranches              , false, "Print message upon encountering uncommon case"                               ) \
  develop(PrintRegTargeting                  , false, "Print info about register targeting"                                         ) \
  develop(PrintExposed                       , false, "Print info about exposed-block analysis"                                     ) \
  develop(PrintEliminateUnnededNodes         , false, "Print info about dead code elimination"                                      ) \
  develop(PrintHexAddresses                  , true , "Print hex addresses in print outs (otherwise print 0)"                       ) \
  develop(GenerateLiteScopeDescs             , false ,"generate lite scope descs"                                                   ) \
  develop(PrintRScopes                       , false, "Print info about RScopes (type feedback sources)"                            ) \
  develop(PrintLoopOpts                      , false, "Print info about loop optimizations"                                         ) \
  develop(PrintStackAfterUnpacking           , false, "Print stack after unpacking deoptimized frames"                              ) \
  develop(PrintDebugInfo                     , false, "Print debugging info of ScopeDescs generated for nmethod"                    ) \
  develop(PrintDebugInfoGeneration           , false, "Print debugging info generation"                                             ) \
                                                                                                                                      \
/* new backend debugging */                                                                                                           \
  develop(PrintCodeGeneration                , false, "Print code generation with new backend"                                      ) \
  develop(PrintPRegMapping                   , false, "Print PRegMapping during code generation"                                    ) \
  develop(PrintMakeConformantCode            , false, "Print code generated by makeConformant"                                      ) \
  develop(CreateScopeDescInfo                , true , "Create ScopeDesc info for new backend code"                                  ) \
  develop(GenerateFullDebugInfo              , false, "Generate debugging info for each byte code and not only for sends/traps"     ) \
  develop(UseNewMakeConformant               , true , "Use new makeConformant function"                                             ) \


#define APPLY_TO_INTEGER_FLAGS(develop,product)                                                                                       \
/*        name                               default  documentation */                                                                \
                                                                                                                                      \
  develop(EventLogLength                     ,  1000, "Length of internal event log"                                                ) \
  develop(StackPrintLimit                    ,    20, "Number of stack frames to print in VM-level stack dump"                      ) \
  develop(MaxElementPrintSize                ,    10, "Maximum number of elements to print"                                         ) \
                                                                                                                                      \
/* allocation parameters */                                                                                                           \
  product(ReservedHeapSize                   ,  50*K, "Maximum size for object heap in Kbytes"                                      ) \
  product(ObjectHeapExpandSize               ,   512, "Chunk size (in Kbytes) by which the object heap grows"                       ) \
  product(EdenSize                           ,   512, "size of eden (in Kbytes)"                                                    ) \
  product(SurvivorSize                       ,    64, "size of survivor spaces (in Kbytes)"                                         ) \
  product(OldSize                            ,   3*K, "initial size of oldspace (in Kbytes)"                                        ) \
  product(ReservedCodeSize                   ,  10*K, "Maximum size of code cache (in Kbytes)"                                      ) \
  product(CodeSize                           ,  20*K, "size of code cache (in Kbytes)"                                              ) \
  product(ReservedPICSize                    ,   4*K, "Maximum size of PIC cache (in Kbytes)"                                       ) \
  product(PICSize                            ,   128, "size of PIC cache (in Kbytes)"                                               ) \
  product(JumpTableSize                      ,   8*K, "size of jump table"                                                          ) \
                                                                                                                                      \
/* compiler parameters */                                                                                                             \
  product(CompilerInstrsSize                 ,  50*K, "max. size of nmethod instrs"                                                 ) \
  product(CompilerScopesSize                 ,  50*K, "max. size of debugging info per nmethod"                                     ) \
  product(CompilerPCsSize                    ,  15*K, "max. size of relocation info info per nmethod"                               ) \
                                                                                                                                      \
/* inlining parameters */                                                                                                             \
  develop(MaxFnInlineCost                    ,    40, "max. cost of normal inlined method"                                          ) \
  develop(MaxBlockInlineCost                 ,    70, "max. cost of block method"                                                   ) \
  develop(MinBlockCostFraction               ,    50, "(in %) inline block if makes up more than this fraction of parent's cost"    ) \
  develop(BlockArgAdditionalAllowedInlineCost,    35, "additional allowed cost for each block arg"                                  ) \
                                                                                                                                      \
  product(InvocationCounterLimit             , 10000, "max. number of method invocations before (re-)compiling"                     ) \
  product(LoopCounterLimit                   , 10000, "max. number of loop iterations before (re-)compiling"                        ) \
                                                                                                                                      \
  product(MaxNmInstrSize                     , 12000, "max. desired size (in instr bytes) of an method"                             ) \
  product(MinSendsBeforeRecompile            ,  2000, "min # sends a method must have performed before being recompiled"            ) \
  product(MaxFnInstrSize                     ,   300, "max. inline size (in instr bytes) of normal method"                          ) \
  product(BlockArgAdditionalInstrSize        ,   150, "extra allowance (in instr bytes) for each block arg"                         ) \
  product(MaxBlockInstrSize                  ,   450, "max. inline size (in instr bytes) of block method"                           ) \
  product(MaxRecursionUnroll                 ,     2, "max. unrolling depth of recursive methods"                                   ) \
  product(MaxTypeCaseSize                    ,     3, "max. number of types in typecase-based inlining"                             ) \
  product(UncommonRecompileLimit             ,     5, "min. number of uncommon traps before recompiling"                            ) \
  product(UncommonInvocationLimit            , 10000, "min. #invocations of uncommon nmethod before recompiling it again"     ) \
  product(UncommonAgeBackoffFactor           ,     4, "for exponential back-off of UncommonAgeLimit based on nmethod version"       ) \
  product(MinInvocationsBeforeTrust          ,   100, "min. # invocations required before trusting nmethod's PICs"		    ) \
  product(NMethodAgeLimit                    ,     2, "min. #  sweeps before nmethod becomes old"                                   ) \
  product(MaxRecompilationSearchLength       ,    10, "max. # of real stack frames to traverse searching for recompilee"            ) \
  product(MaxInterpretedSearchLength         ,    10, "max. # of intrepreted stack frames to traverse searching for recompilee"     ) \
  product(CounterHalfLifeTime                ,    30, "time (in seconds) in which invocation counters decay by half"                ) \
  product(MaxCustomization                   ,    10, "max. # customized method copies to create"                                   ) \
                                                                                                                                      \
/* interpreter debugging */                                                                                                           \
  develop(StopInterpreterAt                  ,     0, "Stops interpreter execution at specified bytecode number"                    ) \
  develop(TraceInterpreterFramesAt           ,     0, "Trace interpreter frames at specified bytecode number"                       ) \
                                                                                                                                      \
/* interpreter statistics */                                                                                                          \
  develop(NumberOfContextAllocations         ,     0, "Number of allocated block contexts"                                          ) \
  develop(NumberOfBlockAllocations           ,     0, "Number of allocated blocks"                                                  ) \
  develop(NumberOfBytecodesExecuted          ,     0, "Number of bytecodes executed by interpreter (if tracing)"                    ) \
                                                                                                                                      \
/* profiler variables */                                                                                                              \
  develop(ProfilerNumberOfInterpreterMethods ,    10, "Max. number of interpreter methods to print"                                 ) \
  develop(ProfilerNumberOfCompiledMethods    ,    10, "Max. number of compiled methods to print"                                    ) \
                                                                                                                                      \
/* miscellaneous */                                                                                                                   \
  develop(HeapSweeperInterval                ,   120, "Time interval (sec) between starting heap sweep"                             ) \
  develop(PrintProgress                      ,     0, "No. of compilations that cause a . to be printed out (0 means turned off)"   ) \
                                                                                                                                      \
/* inlining database variables */                                                                                                     \
  develop(InliningDatabasePruningLimit       ,     3, "Min. number of nodes in inlining structure to qualify for database"          ) \
                                                                                                                                      \

// declaration of boolean flags

#define DECLARE_BOOLEAN_FLAG(name,value,doc) \
  extern "C" bool name; 

APPLY_TO_BOOLEAN_FLAGS(DECLARE_BOOLEAN_FLAG,DECLARE_BOOLEAN_FLAG)


// declaration of integer flags

#define DECLARE_INTEGER_FLAG(name,value,doc) \
  extern "C" int name; 

APPLY_TO_INTEGER_FLAGS(DECLARE_INTEGER_FLAG,DECLARE_INTEGER_FLAG)


extern "C" char* vmDate;

// debug() is intended as a "start debugging" hook to be called from the
// C++ debugger.It sets up everything for debugging.
extern "C" void debug();
