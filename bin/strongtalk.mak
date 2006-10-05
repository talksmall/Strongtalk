# Microsoft Developer Studio Generated NMAKE File, Format Version 40001
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=Delta - Win32 Product
!MESSAGE No configuration specified.  Defaulting to Delta - Win32 Product.
!ENDIF 

!IF "$(CFG)" != "Delta - Win32 Debug" && "$(CFG)" != "Delta - Win32 Fast" &&\
 "$(CFG)" != "Delta - Win32 Product"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "strongtalk.mak" CFG="Delta - Win32 Product"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Delta - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Delta - Win32 Fast" (based on "Win32 (x86) Console Application")
!MESSAGE "Delta - Win32 Product" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "Delta - Win32 Fast"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Delta - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Delta___"
# PROP BASE Intermediate_Dir "Delta___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug"
# PROP Intermediate_Dir "debug"
# PROP Target_Dir ""
OUTDIR=.\debug
INTDIR=.\debug

ALL : "$(OUTDIR)\strongtalk_debug.exe" "$(OUTDIR)\bin\debug\strongtalk.bsc"

CLEAN : 
	-@erase ".\debug\vc40.pdb"
	-@erase ".\debug\strongtalk.pch"
	-@erase ".\debug\vc40.idb"
	-@erase ".\debug\strongtalk.bsc"
	-@erase ".\debug\arguments.sbr"
	-@erase ".\debug\vmSymbols.sbr"
	-@erase ".\debug\bitVector.sbr"
	-@erase ".\debug\associationKlass.sbr"
	-@erase ".\debug\eventlog.sbr"
	-@erase ".\debug\mapConformance.sbr"
	-@erase ".\debug\memOop.sbr"
	-@erase ".\debug\rSet.sbr"
	-@erase ".\debug\codeTable.sbr"
	-@erase ".\debug\objectIDTable.sbr"
	-@erase ".\debug\objArray_prims.sbr"
	-@erase ".\debug\preg.sbr"
	-@erase ".\debug\registerMask.sbr"
	-@erase ".\debug\inliner.sbr"
	-@erase ".\debug\util.sbr"
	-@erase ".\debug\floats.sbr"
	-@erase ".\debug\scopeDescRecorder.sbr"
	-@erase ".\debug\memOopKlass.sbr"
	-@erase ".\debug\vframe.sbr"
	-@erase ".\debug\lookupTypes.sbr"
	-@erase ".\debug\loopOpt.sbr"
	-@erase ".\debug\pcDesc.sbr"
	-@erase ".\debug\iterator.sbr"
	-@erase ".\debug\symbolOop.sbr"
	-@erase ".\debug\debug_prims.sbr"
	-@erase ".\debug\generation.sbr"
	-@erase ".\debug\smi_prims.sbr"
	-@erase ".\debug\dispatchTable.sbr"
	-@erase ".\debug\codeBuffer.sbr"
	-@erase ".\debug\methodIterator.sbr"
	-@erase ".\debug\block_prims.sbr"
	-@erase ".\debug\slist.sbr"
	-@erase ".\debug\oldCodeGenerator.sbr"
	-@erase ".\debug\delta.sbr"
	-@erase ".\debug\dByteArrayOop.sbr"
	-@erase ".\debug\debugPrintable.sbr"
	-@erase ".\debug\markSweep.sbr"
	-@erase ".\debug\byteArray_prims.sbr"
	-@erase ".\debug\ageTable.sbr"
	-@erase ".\debug\associationOop.sbr"
	-@erase ".\debug\shell.sbr"
	-@erase ".\debug\klassKlass.sbr"
	-@erase ".\debug\rframe.sbr"
	-@erase ".\debug\compiler.sbr"
	-@erase ".\debug\runtime.sbr"
	-@erase ".\debug\dValueArrayOop.sbr"
	-@erase ".\debug\universe_more.sbr"
	-@erase ".\debug\locations.sbr"
	-@erase ".\debug\location.sbr"
	-@erase ".\debug\zone.sbr"
	-@erase ".\debug\proxyKlass.sbr"
	-@erase ".\debug\methodOop.sbr"
	-@erase ".\debug\recompile.sbr"
	-@erase ".\debug\tempDecoder.sbr"
	-@erase ".\debug\stackChunk.sbr"
	-@erase ".\debug\methodPrinter.sbr"
	-@erase ".\debug\double_prims.sbr"
	-@erase ".\debug\uncommonBranch.sbr"
	-@erase ".\debug\sweeper.sbr"
	-@erase ".\debug\callBack.sbr"
	-@erase ".\debug\mixinOop.sbr"
	-@erase ".\debug\scope.sbr"
	-@erase ".\debug\dll.sbr"
	-@erase ".\debug\systemAverage.sbr"
	-@erase ".\debug\nativeInstruction.sbr"
	-@erase ".\debug\compiledIC.sbr"
	-@erase ".\debug\rscope.sbr"
	-@erase ".\debug\processOop.sbr"
	-@erase ".\debug\oop_prims.sbr"
	-@erase ".\debug\prettyPrinter.sbr"
	-@erase ".\debug\nmethodScopes.sbr"
	-@erase ".\debug\oopFactory.sbr"
	-@erase ".\debug\snapshot.sbr"
	-@erase ".\debug\primInliner.sbr"
	-@erase ".\debug\nameDesc.sbr"
	-@erase ".\debug\dValueArray_prims.sbr"
	-@erase ".\debug\dByteArrayKlass.sbr"
	-@erase ".\debug\codeGenerator.sbr"
	-@erase ".\debug\bootstrap.sbr"
	-@erase ".\debug\zoneHeap.sbr"
	-@erase ".\debug\bytecodes.sbr"
	-@erase ".\debug\intervalInfo.sbr"
	-@erase ".\debug\processKlass.sbr"
	-@erase ".\debug\printlayout.sbr"
	-@erase ".\debug\expr.sbr"
	-@erase ".\debug\longInt.sbr"
	-@erase ".\debug\objArrayKlass.sbr"
	-@erase ".\debug\symbolTable.sbr"
	-@erase ".\debug\compiledPIC.sbr"
	-@erase ".\debug\fprofiler.sbr"
	-@erase ".\debug\scopeDesc.sbr"
	-@erase ".\debug\growableArray.sbr"
	-@erase ".\debug\basicBlock.sbr"
	-@erase ".\debug\ic_iterator.sbr"
	-@erase ".\debug\iCache.sbr"
	-@erase ".\debug\relocInfo.sbr"
	-@erase ".\debug\allocation.sbr"
	-@erase ".\debug\ncode.sbr"
	-@erase ".\debug\vframeOop.sbr"
	-@erase ".\debug\evaluator.sbr"
	-@erase ".\debug\timer.sbr"
	-@erase ".\debug\byteArrayKlass.sbr"
	-@erase ".\debug\ostream.sbr"
	-@erase ".\debug\jumpTable.sbr"
	-@erase ".\debug\spaceSize.sbr"
	-@erase ".\debug\debug.sbr"
	-@erase ".\debug\dByteArray_prims.sbr"
	-@erase ".\debug\key.sbr"
	-@erase ".\debug\interpretedIC.sbr"
	-@erase ".\debug\dValueArrayKlass.sbr"
	-@erase ".\debug\prim.sbr"
	-@erase ".\debug\frame.sbr"
	-@erase ".\debug\byteArrayOop.sbr"
	-@erase ".\debug\handle.sbr"
	-@erase ".\debug\error.sbr"
	-@erase ".\debug\proxyOop.sbr"
	-@erase ".\debug\klass.sbr"
	-@erase ".\debug\interpreter.sbr"
	-@erase ".\debug\space.sbr"
	-@erase ".\debug\smiKlass.sbr"
	-@erase ".\debug\regAlloc.sbr"
	-@erase ".\debug\stubRoutines.sbr"
	-@erase ".\debug\os.sbr"
	-@erase ".\debug\init.sbr"
	-@erase ".\debug\disassembler.sbr"
	-@erase ".\debug\nodeBuilder.sbr"
	-@erase ".\debug\lookupCache.sbr"
	-@erase ".\debug\codeIterator.sbr"
	-@erase ".\debug\costModel.sbr"
	-@erase ".\debug\klassOop.sbr"
	-@erase ".\debug\vframeKlass.sbr"
	-@erase ".\debug\universe.sbr"
	-@erase ".\debug\bits.sbr"
	-@erase ".\debug\recompiler.sbr"
	-@erase ".\debug\blockOop.sbr"
	-@erase ".\debug\task.sbr"
	-@erase ".\debug\symbolKlass.sbr"
	-@erase ".\debug\oop.sbr"
	-@erase ".\debug\compileTimeClosure.sbr"
	-@erase ".\debug\method_prims.sbr"
	-@erase ".\debug\mixinKlass.sbr"
	-@erase ".\debug\callBack_prims.sbr"
	-@erase ".\debug\opcode.sbr"
	-@erase ".\debug\weakArrayKlass.sbr"
	-@erase ".\debug\methodKlass.sbr"
	-@erase ".\debug\lprintf.sbr"
	-@erase ".\debug\objArrayOop.sbr"
	-@erase ".\debug\integerOps.sbr"
	-@erase ".\debug\vmOperations.sbr"
	-@erase ".\debug\assembler.sbr"
	-@erase ".\debug\doubleOop.sbr"
	-@erase ".\debug\process_prims.sbr"
	-@erase ".\debug\reflection.sbr"
	-@erase ".\debug\disclaimer.sbr"
	-@erase ".\debug\markOop.sbr"
	-@erase ".\debug\pregMapping.sbr"
	-@erase ".\debug\proxy_prims.sbr"
	-@erase ".\debug\mixin_prims.sbr"
	-@erase ".\debug\x86_mapping.sbr"
	-@erase ".\debug\virtualspace.sbr"
	-@erase ".\debug\behavior_prims.sbr"
	-@erase ".\debug\process.sbr"
	-@erase ".\debug\smiOop.sbr"
	-@erase ".\debug\blockKlass.sbr"
	-@erase ".\debug\defUse.sbr"
	-@erase ".\debug\vframe_prims.sbr"
	-@erase ".\debug\savedRegisters.sbr"
	-@erase ".\debug\system_prims.sbr"
	-@erase ".\debug\inliningdb.sbr"
	-@erase ".\debug\node.sbr"
	-@erase ".\debug\abort.sbr"
	-@erase ".\debug\nmethod.sbr"
	-@erase ".\debug\compUtils.sbr"
	-@erase ".\debug\interpreterStatistics.sbr"
	-@erase ".\debug\doubleKlass.sbr"
	-@erase "..\strongtalk_debug.exe"
	-@erase ".\debug\task.obj"
	-@erase ".\debug\symbolKlass.obj"
	-@erase ".\debug\oop.obj"
	-@erase ".\debug\compileTimeClosure.obj"
	-@erase ".\debug\method_prims.obj"
	-@erase ".\debug\mixinKlass.obj"
	-@erase ".\debug\callBack_prims.obj"
	-@erase ".\debug\opcode.obj"
	-@erase ".\debug\weakArrayKlass.obj"
	-@erase ".\debug\methodKlass.obj"
	-@erase ".\debug\lprintf.obj"
	-@erase ".\debug\objArrayOop.obj"
	-@erase ".\debug\integerOps.obj"
	-@erase ".\debug\vmOperations.obj"
	-@erase ".\debug\assembler.obj"
	-@erase ".\debug\doubleOop.obj"
	-@erase ".\debug\process_prims.obj"
	-@erase ".\debug\reflection.obj"
	-@erase ".\debug\disclaimer.obj"
	-@erase ".\debug\markOop.obj"
	-@erase ".\debug\pregMapping.obj"
	-@erase ".\debug\proxy_prims.obj"
	-@erase ".\debug\mixin_prims.obj"
	-@erase ".\debug\x86_mapping.obj"
	-@erase ".\debug\virtualspace.obj"
	-@erase ".\debug\behavior_prims.obj"
	-@erase ".\debug\process.obj"
	-@erase ".\debug\smiOop.obj"
	-@erase ".\debug\blockKlass.obj"
	-@erase ".\debug\defUse.obj"
	-@erase ".\debug\vframe_prims.obj"
	-@erase ".\debug\savedRegisters.obj"
	-@erase ".\debug\system_prims.obj"
	-@erase ".\debug\inliningdb.obj"
	-@erase ".\debug\node.obj"
	-@erase ".\debug\abort.obj"
	-@erase ".\debug\nmethod.obj"
	-@erase ".\debug\compUtils.obj"
	-@erase ".\debug\interpreterStatistics.obj"
	-@erase ".\debug\doubleKlass.obj"
	-@erase ".\debug\arguments.obj"
	-@erase ".\debug\vmSymbols.obj"
	-@erase ".\debug\bitVector.obj"
	-@erase ".\debug\associationKlass.obj"
	-@erase ".\debug\eventlog.obj"
	-@erase ".\debug\mapConformance.obj"
	-@erase ".\debug\memOop.obj"
	-@erase ".\debug\rSet.obj"
	-@erase ".\debug\codeTable.obj"
	-@erase ".\debug\objectIDTable.obj"
	-@erase ".\debug\objArray_prims.obj"
	-@erase ".\debug\preg.obj"
	-@erase ".\debug\registerMask.obj"
	-@erase ".\debug\inliner.obj"
	-@erase ".\debug\util.obj"
	-@erase ".\debug\floats.obj"
	-@erase ".\debug\scopeDescRecorder.obj"
	-@erase ".\debug\memOopKlass.obj"
	-@erase ".\debug\vframe.obj"
	-@erase ".\debug\lookupTypes.obj"
	-@erase ".\debug\loopOpt.obj"
	-@erase ".\debug\pcDesc.obj"
	-@erase ".\debug\iterator.obj"
	-@erase ".\debug\symbolOop.obj"
	-@erase ".\debug\debug_prims.obj"
	-@erase ".\debug\generation.obj"
	-@erase ".\debug\smi_prims.obj"
	-@erase ".\debug\dispatchTable.obj"
	-@erase ".\debug\codeBuffer.obj"
	-@erase ".\debug\methodIterator.obj"
	-@erase ".\debug\block_prims.obj"
	-@erase ".\debug\slist.obj"
	-@erase ".\debug\oldCodeGenerator.obj"
	-@erase ".\debug\delta.obj"
	-@erase ".\debug\dByteArrayOop.obj"
	-@erase ".\debug\debugPrintable.obj"
	-@erase ".\debug\markSweep.obj"
	-@erase ".\debug\byteArray_prims.obj"
	-@erase ".\debug\ageTable.obj"
	-@erase ".\debug\associationOop.obj"
	-@erase ".\debug\shell.obj"
	-@erase ".\debug\klassKlass.obj"
	-@erase ".\debug\rframe.obj"
	-@erase ".\debug\compiler.obj"
	-@erase ".\debug\runtime.obj"
	-@erase ".\debug\dValueArrayOop.obj"
	-@erase ".\debug\universe_more.obj"
	-@erase ".\debug\locations.obj"
	-@erase ".\debug\location.obj"
	-@erase ".\debug\zone.obj"
	-@erase ".\debug\proxyKlass.obj"
	-@erase ".\debug\methodOop.obj"
	-@erase ".\debug\recompile.obj"
	-@erase ".\debug\tempDecoder.obj"
	-@erase ".\debug\stackChunk.obj"
	-@erase ".\debug\methodPrinter.obj"
	-@erase ".\debug\double_prims.obj"
	-@erase ".\debug\uncommonBranch.obj"
	-@erase ".\debug\sweeper.obj"
	-@erase ".\debug\callBack.obj"
	-@erase ".\debug\mixinOop.obj"
	-@erase ".\debug\scope.obj"
	-@erase ".\debug\dll.obj"
	-@erase ".\debug\systemAverage.obj"
	-@erase ".\debug\nativeInstruction.obj"
	-@erase ".\debug\compiledIC.obj"
	-@erase ".\debug\rscope.obj"
	-@erase ".\debug\processOop.obj"
	-@erase ".\debug\oop_prims.obj"
	-@erase ".\debug\prettyPrinter.obj"
	-@erase ".\debug\nmethodScopes.obj"
	-@erase ".\debug\oopFactory.obj"
	-@erase ".\debug\snapshot.obj"
	-@erase ".\debug\primInliner.obj"
	-@erase ".\debug\nameDesc.obj"
	-@erase ".\debug\dValueArray_prims.obj"
	-@erase ".\debug\dByteArrayKlass.obj"
	-@erase ".\debug\codeGenerator.obj"
	-@erase ".\debug\bootstrap.obj"
	-@erase ".\debug\zoneHeap.obj"
	-@erase ".\debug\bytecodes.obj"
	-@erase ".\debug\intervalInfo.obj"
	-@erase ".\debug\processKlass.obj"
	-@erase ".\debug\printlayout.obj"
	-@erase ".\debug\expr.obj"
	-@erase ".\debug\longInt.obj"
	-@erase ".\debug\objArrayKlass.obj"
	-@erase ".\debug\symbolTable.obj"
	-@erase ".\debug\compiledPIC.obj"
	-@erase ".\debug\fprofiler.obj"
	-@erase ".\debug\scopeDesc.obj"
	-@erase ".\debug\growableArray.obj"
	-@erase ".\debug\basicBlock.obj"
	-@erase ".\debug\ic_iterator.obj"
	-@erase ".\debug\iCache.obj"
	-@erase ".\debug\relocInfo.obj"
	-@erase ".\debug\allocation.obj"
	-@erase ".\debug\ncode.obj"
	-@erase ".\debug\vframeOop.obj"
	-@erase ".\debug\evaluator.obj"
	-@erase ".\debug\timer.obj"
	-@erase ".\debug\byteArrayKlass.obj"
	-@erase ".\debug\ostream.obj"
	-@erase ".\debug\jumpTable.obj"
	-@erase ".\debug\spaceSize.obj"
	-@erase ".\debug\debug.obj"
	-@erase ".\debug\dByteArray_prims.obj"
	-@erase ".\debug\key.obj"
	-@erase ".\debug\interpretedIC.obj"
	-@erase ".\debug\dValueArrayKlass.obj"
	-@erase ".\debug\prim.obj"
	-@erase ".\debug\frame.obj"
	-@erase ".\debug\byteArrayOop.obj"
	-@erase ".\debug\handle.obj"
	-@erase ".\debug\error.obj"
	-@erase ".\debug\proxyOop.obj"
	-@erase ".\debug\klass.obj"
	-@erase ".\debug\interpreter.obj"
	-@erase ".\debug\space.obj"
	-@erase ".\debug\smiKlass.obj"
	-@erase ".\debug\regAlloc.obj"
	-@erase ".\debug\stubRoutines.obj"
	-@erase ".\debug\os.obj"
	-@erase ".\debug\init.obj"
	-@erase ".\debug\disassembler.obj"
	-@erase ".\debug\nodeBuilder.obj"
	-@erase ".\debug\lookupCache.obj"
	-@erase ".\debug\codeIterator.obj"
	-@erase ".\debug\costModel.obj"
	-@erase ".\debug\klassOop.obj"
	-@erase ".\debug\vframeKlass.obj"
	-@erase ".\debug\universe.obj"
	-@erase ".\debug\bits.obj"
	-@erase ".\debug\recompiler.obj"
	-@erase ".\debug\blockOop.obj"
	-@erase ".\debug\interpreter_asm.obj"
	-@erase ".\debug\smi_prims_asm.obj"
	-@erase ".\debug\callBackStubs.obj"
	-@erase ".\debug\block_prims_asm.obj"
	-@erase ".\debug\double_prims_asm.obj"
	-@erase ".\debug\util_asm.obj"
	-@erase ".\debug\behavior_prims_asm.obj"
	-@erase "..\strongtalk_debug.ilk"
	-@erase ".\debug\strongtalk_debug.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I "." /I "incls" /I "..\vm\asm" /I "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I "..\vm\interpreter" /I "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I "..\vm\prims" /I "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes" /I "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I "C:/Program Files/MSDEV/INCL" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "MICROSOFT" /D "ASSERT" /D "DEBUG" /D "DELTA_COMPILER" /FR /Yu"incls/_precompiled.incl" /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /I "." /I "incls" /I "..\vm\asm" /I\
 "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I "..\vm\interpreter" /I\
 "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I "..\vm\prims" /I\
 "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes" /I\
 "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I\
 "C:/Program Files/MSDEV/INCL" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D\
 "MICROSOFT" /D "ASSERT" /D "DEBUG" /D "DELTA_COMPILER" /FR"$(INTDIR)/"\
 /Fp"$(INTDIR)/strongtalk.pch" /Yu"incls/_precompiled.incl" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\debug/
CPP_SBRS=.\debug/
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/strongtalk.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/arguments.sbr" \
	"$(INTDIR)/vmSymbols.sbr" \
	"$(INTDIR)/bitVector.sbr" \
	"$(INTDIR)/associationKlass.sbr" \
	"$(INTDIR)/eventlog.sbr" \
	"$(INTDIR)/mapConformance.sbr" \
	"$(INTDIR)/memOop.sbr" \
	"$(INTDIR)/rSet.sbr" \
	"$(INTDIR)/codeTable.sbr" \
	"$(INTDIR)/objectIDTable.sbr" \
	"$(INTDIR)/objArray_prims.sbr" \
	"$(INTDIR)/preg.sbr" \
	"$(INTDIR)/registerMask.sbr" \
	"$(INTDIR)/inliner.sbr" \
	"$(INTDIR)/util.sbr" \
	"$(INTDIR)/floats.sbr" \
	"$(INTDIR)/scopeDescRecorder.sbr" \
	"$(INTDIR)/memOopKlass.sbr" \
	"$(INTDIR)/vframe.sbr" \
	"$(INTDIR)/lookupTypes.sbr" \
	"$(INTDIR)/loopOpt.sbr" \
	"$(INTDIR)/pcDesc.sbr" \
	"$(INTDIR)/iterator.sbr" \
	"$(INTDIR)/symbolOop.sbr" \
	"$(INTDIR)/debug_prims.sbr" \
	"$(INTDIR)/generation.sbr" \
	"$(INTDIR)/smi_prims.sbr" \
	"$(INTDIR)/dispatchTable.sbr" \
	"$(INTDIR)/codeBuffer.sbr" \
	"$(INTDIR)/methodIterator.sbr" \
	"$(INTDIR)/block_prims.sbr" \
	"$(INTDIR)/slist.sbr" \
	"$(INTDIR)/oldCodeGenerator.sbr" \
	"$(INTDIR)/delta.sbr" \
	"$(INTDIR)/dByteArrayOop.sbr" \
	"$(INTDIR)/debugPrintable.sbr" \
	"$(INTDIR)/markSweep.sbr" \
	"$(INTDIR)/byteArray_prims.sbr" \
	"$(INTDIR)/ageTable.sbr" \
	"$(INTDIR)/associationOop.sbr" \
	"$(INTDIR)/shell.sbr" \
	"$(INTDIR)/klassKlass.sbr" \
	"$(INTDIR)/rframe.sbr" \
	"$(INTDIR)/compiler.sbr" \
	"$(INTDIR)/runtime.sbr" \
	"$(INTDIR)/dValueArrayOop.sbr" \
	"$(INTDIR)/universe_more.sbr" \
	"$(INTDIR)/locations.sbr" \
	"$(INTDIR)/location.sbr" \
	"$(INTDIR)/zone.sbr" \
	"$(INTDIR)/proxyKlass.sbr" \
	"$(INTDIR)/methodOop.sbr" \
	"$(INTDIR)/recompile.sbr" \
	"$(INTDIR)/tempDecoder.sbr" \
	"$(INTDIR)/stackChunk.sbr" \
	"$(INTDIR)/methodPrinter.sbr" \
	"$(INTDIR)/double_prims.sbr" \
	"$(INTDIR)/uncommonBranch.sbr" \
	"$(INTDIR)/sweeper.sbr" \
	"$(INTDIR)/callBack.sbr" \
	"$(INTDIR)/mixinOop.sbr" \
	"$(INTDIR)/scope.sbr" \
	"$(INTDIR)/dll.sbr" \
	"$(INTDIR)/systemAverage.sbr" \
	"$(INTDIR)/nativeInstruction.sbr" \
	"$(INTDIR)/compiledIC.sbr" \
	"$(INTDIR)/rscope.sbr" \
	"$(INTDIR)/processOop.sbr" \
	"$(INTDIR)/oop_prims.sbr" \
	"$(INTDIR)/prettyPrinter.sbr" \
	"$(INTDIR)/nmethodScopes.sbr" \
	"$(INTDIR)/oopFactory.sbr" \
	"$(INTDIR)/snapshot.sbr" \
	"$(INTDIR)/primInliner.sbr" \
	"$(INTDIR)/nameDesc.sbr" \
	"$(INTDIR)/dValueArray_prims.sbr" \
	"$(INTDIR)/dByteArrayKlass.sbr" \
	"$(INTDIR)/codeGenerator.sbr" \
	"$(INTDIR)/bootstrap.sbr" \
	"$(INTDIR)/zoneHeap.sbr" \
	"$(INTDIR)/bytecodes.sbr" \
	"$(INTDIR)/intervalInfo.sbr" \
	"$(INTDIR)/processKlass.sbr" \
	"$(INTDIR)/printlayout.sbr" \
	"$(INTDIR)/expr.sbr" \
	"$(INTDIR)/longInt.sbr" \
	"$(INTDIR)/objArrayKlass.sbr" \
	"$(INTDIR)/symbolTable.sbr" \
	"$(INTDIR)/compiledPIC.sbr" \
	"$(INTDIR)/fprofiler.sbr" \
	"$(INTDIR)/scopeDesc.sbr" \
	"$(INTDIR)/growableArray.sbr" \
	"$(INTDIR)/basicBlock.sbr" \
	"$(INTDIR)/ic_iterator.sbr" \
	"$(INTDIR)/iCache.sbr" \
	"$(INTDIR)/relocInfo.sbr" \
	"$(INTDIR)/allocation.sbr" \
	"$(INTDIR)/ncode.sbr" \
	"$(INTDIR)/vframeOop.sbr" \
	"$(INTDIR)/evaluator.sbr" \
	"$(INTDIR)/timer.sbr" \
	"$(INTDIR)/byteArrayKlass.sbr" \
	"$(INTDIR)/ostream.sbr" \
	"$(INTDIR)/jumpTable.sbr" \
	"$(INTDIR)/spaceSize.sbr" \
	"$(INTDIR)/debug.sbr" \
	"$(INTDIR)/dByteArray_prims.sbr" \
	"$(INTDIR)/key.sbr" \
	"$(INTDIR)/interpretedIC.sbr" \
	"$(INTDIR)/dValueArrayKlass.sbr" \
	"$(INTDIR)/prim.sbr" \
	"$(INTDIR)/frame.sbr" \
	"$(INTDIR)/byteArrayOop.sbr" \
	"$(INTDIR)/handle.sbr" \
	"$(INTDIR)/error.sbr" \
	"$(INTDIR)/proxyOop.sbr" \
	"$(INTDIR)/klass.sbr" \
	"$(INTDIR)/interpreter.sbr" \
	"$(INTDIR)/space.sbr" \
	"$(INTDIR)/smiKlass.sbr" \
	"$(INTDIR)/regAlloc.sbr" \
	"$(INTDIR)/stubRoutines.sbr" \
	"$(INTDIR)/os.sbr" \
	"$(INTDIR)/init.sbr" \
	"$(INTDIR)/disassembler.sbr" \
	"$(INTDIR)/nodeBuilder.sbr" \
	"$(INTDIR)/lookupCache.sbr" \
	"$(INTDIR)/codeIterator.sbr" \
	"$(INTDIR)/costModel.sbr" \
	"$(INTDIR)/klassOop.sbr" \
	"$(INTDIR)/vframeKlass.sbr" \
	"$(INTDIR)/universe.sbr" \
	"$(INTDIR)/bits.sbr" \
	"$(INTDIR)/recompiler.sbr" \
	"$(INTDIR)/blockOop.sbr" \
	"$(INTDIR)/task.sbr" \
	"$(INTDIR)/symbolKlass.sbr" \
	"$(INTDIR)/oop.sbr" \
	"$(INTDIR)/compileTimeClosure.sbr" \
	"$(INTDIR)/method_prims.sbr" \
	"$(INTDIR)/mixinKlass.sbr" \
	"$(INTDIR)/callBack_prims.sbr" \
	"$(INTDIR)/opcode.sbr" \
	"$(INTDIR)/weakArrayKlass.sbr" \
	"$(INTDIR)/methodKlass.sbr" \
	"$(INTDIR)/lprintf.sbr" \
	"$(INTDIR)/objArrayOop.sbr" \
	"$(INTDIR)/integerOps.sbr" \
	"$(INTDIR)/vmOperations.sbr" \
	"$(INTDIR)/assembler.sbr" \
	"$(INTDIR)/doubleOop.sbr" \
	"$(INTDIR)/process_prims.sbr" \
	"$(INTDIR)/reflection.sbr" \
	"$(INTDIR)/disclaimer.sbr" \
	"$(INTDIR)/markOop.sbr" \
	"$(INTDIR)/pregMapping.sbr" \
	"$(INTDIR)/proxy_prims.sbr" \
	"$(INTDIR)/mixin_prims.sbr" \
	"$(INTDIR)/x86_mapping.sbr" \
	"$(INTDIR)/virtualspace.sbr" \
	"$(INTDIR)/behavior_prims.sbr" \
	"$(INTDIR)/process.sbr" \
	"$(INTDIR)/smiOop.sbr" \
	"$(INTDIR)/blockKlass.sbr" \
	"$(INTDIR)/defUse.sbr" \
	"$(INTDIR)/vframe_prims.sbr" \
	"$(INTDIR)/savedRegisters.sbr" \
	"$(INTDIR)/system_prims.sbr" \
	"$(INTDIR)/inliningdb.sbr" \
	"$(INTDIR)/node.sbr" \
	"$(INTDIR)/abort.sbr" \
	"$(INTDIR)/nmethod.sbr" \
	"$(INTDIR)/compUtils.sbr" \
	"$(INTDIR)/interpreterStatistics.sbr" \
	"$(INTDIR)/doubleKlass.sbr"

"$(OUTDIR)\bin\debug\strongtalk.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 /nologo /subsystem:console /debug /machine:I386 /out:"../strongtalk_debug.exe"
# SUBTRACT LINK32 /profile
LINK32_FLAGS=/nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/strongtalk_debug.pdb" /debug /machine:I386\
 /out:"../strongtalk_debug.exe" 
LINK32_OBJS= \
	"$(INTDIR)/task.obj" \
	"$(INTDIR)/symbolKlass.obj" \
	"$(INTDIR)/oop.obj" \
	"$(INTDIR)/compileTimeClosure.obj" \
	"$(INTDIR)/method_prims.obj" \
	"$(INTDIR)/mixinKlass.obj" \
	"$(INTDIR)/callBack_prims.obj" \
	"$(INTDIR)/opcode.obj" \
	"$(INTDIR)/weakArrayKlass.obj" \
	"$(INTDIR)/methodKlass.obj" \
	"$(INTDIR)/lprintf.obj" \
	"$(INTDIR)/objArrayOop.obj" \
	"$(INTDIR)/integerOps.obj" \
	"$(INTDIR)/vmOperations.obj" \
	"$(INTDIR)/assembler.obj" \
	"$(INTDIR)/doubleOop.obj" \
	"$(INTDIR)/process_prims.obj" \
	"$(INTDIR)/reflection.obj" \
	"$(INTDIR)/disclaimer.obj" \
	"$(INTDIR)/markOop.obj" \
	"$(INTDIR)/pregMapping.obj" \
	"$(INTDIR)/proxy_prims.obj" \
	"$(INTDIR)/mixin_prims.obj" \
	"$(INTDIR)/x86_mapping.obj" \
	"$(INTDIR)/virtualspace.obj" \
	"$(INTDIR)/behavior_prims.obj" \
	"$(INTDIR)/process.obj" \
	"$(INTDIR)/smiOop.obj" \
	"$(INTDIR)/blockKlass.obj" \
	"$(INTDIR)/defUse.obj" \
	"$(INTDIR)/vframe_prims.obj" \
	"$(INTDIR)/savedRegisters.obj" \
	"$(INTDIR)/system_prims.obj" \
	"$(INTDIR)/inliningdb.obj" \
	"$(INTDIR)/node.obj" \
	"$(INTDIR)/abort.obj" \
	"$(INTDIR)/nmethod.obj" \
	"$(INTDIR)/compUtils.obj" \
	"$(INTDIR)/interpreterStatistics.obj" \
	"$(INTDIR)/doubleKlass.obj" \
	"$(INTDIR)/arguments.obj" \
	"$(INTDIR)/vmSymbols.obj" \
	"$(INTDIR)/bitVector.obj" \
	"$(INTDIR)/associationKlass.obj" \
	"$(INTDIR)/eventlog.obj" \
	"$(INTDIR)/mapConformance.obj" \
	"$(INTDIR)/memOop.obj" \
	"$(INTDIR)/rSet.obj" \
	"$(INTDIR)/codeTable.obj" \
	"$(INTDIR)/objectIDTable.obj" \
	"$(INTDIR)/objArray_prims.obj" \
	"$(INTDIR)/preg.obj" \
	"$(INTDIR)/registerMask.obj" \
	"$(INTDIR)/inliner.obj" \
	"$(INTDIR)/util.obj" \
	"$(INTDIR)/floats.obj" \
	"$(INTDIR)/scopeDescRecorder.obj" \
	"$(INTDIR)/memOopKlass.obj" \
	"$(INTDIR)/vframe.obj" \
	"$(INTDIR)/lookupTypes.obj" \
	"$(INTDIR)/loopOpt.obj" \
	"$(INTDIR)/pcDesc.obj" \
	"$(INTDIR)/iterator.obj" \
	"$(INTDIR)/symbolOop.obj" \
	"$(INTDIR)/debug_prims.obj" \
	"$(INTDIR)/generation.obj" \
	"$(INTDIR)/smi_prims.obj" \
	"$(INTDIR)/dispatchTable.obj" \
	"$(INTDIR)/codeBuffer.obj" \
	"$(INTDIR)/methodIterator.obj" \
	"$(INTDIR)/block_prims.obj" \
	"$(INTDIR)/slist.obj" \
	"$(INTDIR)/oldCodeGenerator.obj" \
	"$(INTDIR)/delta.obj" \
	"$(INTDIR)/dByteArrayOop.obj" \
	"$(INTDIR)/debugPrintable.obj" \
	"$(INTDIR)/markSweep.obj" \
	"$(INTDIR)/byteArray_prims.obj" \
	"$(INTDIR)/ageTable.obj" \
	"$(INTDIR)/associationOop.obj" \
	"$(INTDIR)/shell.obj" \
	"$(INTDIR)/klassKlass.obj" \
	"$(INTDIR)/rframe.obj" \
	"$(INTDIR)/compiler.obj" \
	"$(INTDIR)/runtime.obj" \
	"$(INTDIR)/dValueArrayOop.obj" \
	"$(INTDIR)/universe_more.obj" \
	"$(INTDIR)/locations.obj" \
	"$(INTDIR)/location.obj" \
	"$(INTDIR)/zone.obj" \
	"$(INTDIR)/proxyKlass.obj" \
	"$(INTDIR)/methodOop.obj" \
	"$(INTDIR)/recompile.obj" \
	"$(INTDIR)/tempDecoder.obj" \
	"$(INTDIR)/stackChunk.obj" \
	"$(INTDIR)/methodPrinter.obj" \
	"$(INTDIR)/double_prims.obj" \
	"$(INTDIR)/uncommonBranch.obj" \
	"$(INTDIR)/sweeper.obj" \
	"$(INTDIR)/callBack.obj" \
	"$(INTDIR)/mixinOop.obj" \
	"$(INTDIR)/scope.obj" \
	"$(INTDIR)/dll.obj" \
	"$(INTDIR)/systemAverage.obj" \
	"$(INTDIR)/nativeInstruction.obj" \
	"$(INTDIR)/compiledIC.obj" \
	"$(INTDIR)/rscope.obj" \
	"$(INTDIR)/processOop.obj" \
	"$(INTDIR)/oop_prims.obj" \
	"$(INTDIR)/prettyPrinter.obj" \
	"$(INTDIR)/nmethodScopes.obj" \
	"$(INTDIR)/oopFactory.obj" \
	"$(INTDIR)/snapshot.obj" \
	"$(INTDIR)/primInliner.obj" \
	"$(INTDIR)/nameDesc.obj" \
	"$(INTDIR)/dValueArray_prims.obj" \
	"$(INTDIR)/dByteArrayKlass.obj" \
	"$(INTDIR)/codeGenerator.obj" \
	"$(INTDIR)/bootstrap.obj" \
	"$(INTDIR)/zoneHeap.obj" \
	"$(INTDIR)/bytecodes.obj" \
	"$(INTDIR)/intervalInfo.obj" \
	"$(INTDIR)/processKlass.obj" \
	"$(INTDIR)/printlayout.obj" \
	"$(INTDIR)/expr.obj" \
	"$(INTDIR)/longInt.obj" \
	"$(INTDIR)/objArrayKlass.obj" \
	"$(INTDIR)/symbolTable.obj" \
	"$(INTDIR)/compiledPIC.obj" \
	"$(INTDIR)/fprofiler.obj" \
	"$(INTDIR)/scopeDesc.obj" \
	"$(INTDIR)/growableArray.obj" \
	"$(INTDIR)/basicBlock.obj" \
	"$(INTDIR)/ic_iterator.obj" \
	"$(INTDIR)/iCache.obj" \
	"$(INTDIR)/relocInfo.obj" \
	"$(INTDIR)/allocation.obj" \
	"$(INTDIR)/ncode.obj" \
	"$(INTDIR)/vframeOop.obj" \
	"$(INTDIR)/evaluator.obj" \
	"$(INTDIR)/timer.obj" \
	"$(INTDIR)/byteArrayKlass.obj" \
	"$(INTDIR)/ostream.obj" \
	"$(INTDIR)/jumpTable.obj" \
	"$(INTDIR)/spaceSize.obj" \
	"$(INTDIR)/debug.obj" \
	"$(INTDIR)/dByteArray_prims.obj" \
	"$(INTDIR)/key.obj" \
	"$(INTDIR)/interpretedIC.obj" \
	"$(INTDIR)/dValueArrayKlass.obj" \
	"$(INTDIR)/prim.obj" \
	"$(INTDIR)/frame.obj" \
	"$(INTDIR)/byteArrayOop.obj" \
	"$(INTDIR)/handle.obj" \
	"$(INTDIR)/error.obj" \
	"$(INTDIR)/proxyOop.obj" \
	"$(INTDIR)/klass.obj" \
	"$(INTDIR)/interpreter.obj" \
	"$(INTDIR)/space.obj" \
	"$(INTDIR)/smiKlass.obj" \
	"$(INTDIR)/regAlloc.obj" \
	"$(INTDIR)/stubRoutines.obj" \
	"$(INTDIR)/os.obj" \
	"$(INTDIR)/init.obj" \
	"$(INTDIR)/disassembler.obj" \
	"$(INTDIR)/nodeBuilder.obj" \
	"$(INTDIR)/lookupCache.obj" \
	"$(INTDIR)/codeIterator.obj" \
	"$(INTDIR)/costModel.obj" \
	"$(INTDIR)/klassOop.obj" \
	"$(INTDIR)/vframeKlass.obj" \
	"$(INTDIR)/universe.obj" \
	"$(INTDIR)/bits.obj" \
	"$(INTDIR)/recompiler.obj" \
	"$(INTDIR)/blockOop.obj" \
	"$(INTDIR)/interpreter_asm.obj" \
	"$(INTDIR)/smi_prims_asm.obj" \
	"$(INTDIR)/callBackStubs.obj" \
	"$(INTDIR)/block_prims_asm.obj" \
	"$(INTDIR)/double_prims_asm.obj" \
	"$(INTDIR)/util_asm.obj" \
	"$(INTDIR)/behavior_prims_asm.obj"

"$(OUTDIR)\strongtalk_debug.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Delta___"
# PROP BASE Intermediate_Dir "Delta___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "fast"
# PROP Intermediate_Dir "fast"
# PROP Target_Dir ""
OUTDIR=.\fast
INTDIR=.\fast

ALL : "$(OUTDIR)\strongtalk_fast.exe"

CLEAN : 
	-@erase ".\fast\vc40.pdb"
	-@erase ".\fast\strongtalk.pch"
	-@erase ".\fast\vc40.idb"
	-@erase "..\strongtalk_fast.exe"
	-@erase ".\fast\bytecodes.obj"
	-@erase ".\fast\dByteArrayOop.obj"
	-@erase ".\fast\proxyKlass.obj"
	-@erase ".\fast\smiKlass.obj"
	-@erase ".\fast\vmSymbols.obj"
	-@erase ".\fast\abort.obj"
	-@erase ".\fast\opcode.obj"
	-@erase ".\fast\tempDecoder.obj"
	-@erase ".\fast\stackChunk.obj"
	-@erase ".\fast\objArrayOop.obj"
	-@erase ".\fast\debugPrintable.obj"
	-@erase ".\fast\mixinOop.obj"
	-@erase ".\fast\fprofiler.obj"
	-@erase ".\fast\oop.obj"
	-@erase ".\fast\compiledIC.obj"
	-@erase ".\fast\registerMask.obj"
	-@erase ".\fast\runtime.obj"
	-@erase ".\fast\processOop.obj"
	-@erase ".\fast\universe.obj"
	-@erase ".\fast\universe_more.obj"
	-@erase ".\fast\smiOop.obj"
	-@erase ".\fast\blockOop.obj"
	-@erase ".\fast\defUse.obj"
	-@erase ".\fast\oopFactory.obj"
	-@erase ".\fast\vframeOop.obj"
	-@erase ".\fast\dValueArrayOop.obj"
	-@erase ".\fast\symbolOop.obj"
	-@erase ".\fast\compileTimeClosure.obj"
	-@erase ".\fast\dll.obj"
	-@erase ".\fast\spaceSize.obj"
	-@erase ".\fast\snapshot.obj"
	-@erase ".\fast\primInliner.obj"
	-@erase ".\fast\dByteArrayKlass.obj"
	-@erase ".\fast\methodPrinter.obj"
	-@erase ".\fast\sweeper.obj"
	-@erase ".\fast\ncode.obj"
	-@erase ".\fast\systemAverage.obj"
	-@erase ".\fast\weakArrayKlass.obj"
	-@erase ".\fast\uncommonBranch.obj"
	-@erase ".\fast\zoneHeap.obj"
	-@erase ".\fast\printlayout.obj"
	-@erase ".\fast\prettyPrinter.obj"
	-@erase ".\fast\nmethodScopes.obj"
	-@erase ".\fast\delta.obj"
	-@erase ".\fast\memOop.obj"
	-@erase ".\fast\symbolTable.obj"
	-@erase ".\fast\behavior_prims.obj"
	-@erase ".\fast\prim.obj"
	-@erase ".\fast\frame.obj"
	-@erase ".\fast\basicBlock.obj"
	-@erase ".\fast\floats.obj"
	-@erase ".\fast\error.obj"
	-@erase ".\fast\vframe.obj"
	-@erase ".\fast\ic_iterator.obj"
	-@erase ".\fast\recompile.obj"
	-@erase ".\fast\klass.obj"
	-@erase ".\fast\codeGenerator.obj"
	-@erase ".\fast\allocation.obj"
	-@erase ".\fast\double_prims.obj"
	-@erase ".\fast\dByteArray_prims.obj"
	-@erase ".\fast\space.obj"
	-@erase ".\fast\debug_prims.obj"
	-@erase ".\fast\dValueArrayKlass.obj"
	-@erase ".\fast\block_prims.obj"
	-@erase ".\fast\byteArray_prims.obj"
	-@erase ".\fast\init.obj"
	-@erase ".\fast\objArrayKlass.obj"
	-@erase ".\fast\oop_prims.obj"
	-@erase ".\fast\longInt.obj"
	-@erase ".\fast\eventlog.obj"
	-@erase ".\fast\growableArray.obj"
	-@erase ".\fast\scope.obj"
	-@erase ".\fast\bits.obj"
	-@erase ".\fast\rframe.obj"
	-@erase ".\fast\nativeInstruction.obj"
	-@erase ".\fast\task.obj"
	-@erase ".\fast\interpreter.obj"
	-@erase ".\fast\proxyOop.obj"
	-@erase ".\fast\assembler.obj"
	-@erase ".\fast\ostream.obj"
	-@erase ".\fast\bootstrap.obj"
	-@erase ".\fast\key.obj"
	-@erase ".\fast\iterator.obj"
	-@erase ".\fast\intervalInfo.obj"
	-@erase ".\fast\processKlass.obj"
	-@erase ".\fast\regAlloc.obj"
	-@erase ".\fast\methodIterator.obj"
	-@erase ".\fast\interpretedIC.obj"
	-@erase ".\fast\dValueArray_prims.obj"
	-@erase ".\fast\ageTable.obj"
	-@erase ".\fast\nodeBuilder.obj"
	-@erase ".\fast\lookupCache.obj"
	-@erase ".\fast\rscope.obj"
	-@erase ".\fast\associationOop.obj"
	-@erase ".\fast\vframeKlass.obj"
	-@erase ".\fast\scopeDesc.obj"
	-@erase ".\fast\recompiler.obj"
	-@erase ".\fast\klassOop.obj"
	-@erase ".\fast\expr.obj"
	-@erase ".\fast\compUtils.obj"
	-@erase ".\fast\compiler.obj"
	-@erase ".\fast\relocInfo.obj"
	-@erase ".\fast\symbolKlass.obj"
	-@erase ".\fast\evaluator.obj"
	-@erase ".\fast\arguments.obj"
	-@erase ".\fast\node.obj"
	-@erase ".\fast\jumpTable.obj"
	-@erase ".\fast\bitVector.obj"
	-@erase ".\fast\location.obj"
	-@erase ".\fast\interpreterStatistics.obj"
	-@erase ".\fast\mixinKlass.obj"
	-@erase ".\fast\methodKlass.obj"
	-@erase ".\fast\scopeDescRecorder.obj"
	-@erase ".\fast\integerOps.obj"
	-@erase ".\fast\os.obj"
	-@erase ".\fast\timer.obj"
	-@erase ".\fast\codeTable.obj"
	-@erase ".\fast\callBack.obj"
	-@erase ".\fast\reflection.obj"
	-@erase ".\fast\byteArrayOop.obj"
	-@erase ".\fast\disclaimer.obj"
	-@erase ".\fast\pregMapping.obj"
	-@erase ".\fast\proxy_prims.obj"
	-@erase ".\fast\mixin_prims.obj"
	-@erase ".\fast\x86_mapping.obj"
	-@erase ".\fast\blockKlass.obj"
	-@erase ".\fast\rSet.obj"
	-@erase ".\fast\debug.obj"
	-@erase ".\fast\smi_prims.obj"
	-@erase ".\fast\compiledPIC.obj"
	-@erase ".\fast\preg.obj"
	-@erase ".\fast\inliningdb.obj"
	-@erase ".\fast\stubRoutines.obj"
	-@erase ".\fast\util.obj"
	-@erase ".\fast\associationKlass.obj"
	-@erase ".\fast\iCache.obj"
	-@erase ".\fast\lprintf.obj"
	-@erase ".\fast\nameDesc.obj"
	-@erase ".\fast\markSweep.obj"
	-@erase ".\fast\callBack_prims.obj"
	-@erase ".\fast\disassembler.obj"
	-@erase ".\fast\process_prims.obj"
	-@erase ".\fast\pcDesc.obj"
	-@erase ".\fast\doubleKlass.obj"
	-@erase ".\fast\markOop.obj"
	-@erase ".\fast\codeIterator.obj"
	-@erase ".\fast\slist.obj"
	-@erase ".\fast\costModel.obj"
	-@erase ".\fast\process.obj"
	-@erase ".\fast\shell.obj"
	-@erase ".\fast\locations.obj"
	-@erase ".\fast\method_prims.obj"
	-@erase ".\fast\savedRegisters.obj"
	-@erase ".\fast\handle.obj"
	-@erase ".\fast\nmethod.obj"
	-@erase ".\fast\methodOop.obj"
	-@erase ".\fast\oldCodeGenerator.obj"
	-@erase ".\fast\memOopKlass.obj"
	-@erase ".\fast\lookupTypes.obj"
	-@erase ".\fast\vmOperations.obj"
	-@erase ".\fast\generation.obj"
	-@erase ".\fast\zone.obj"
	-@erase ".\fast\doubleOop.obj"
	-@erase ".\fast\codeBuffer.obj"
	-@erase ".\fast\byteArrayKlass.obj"
	-@erase ".\fast\virtualspace.obj"
	-@erase ".\fast\vframe_prims.obj"
	-@erase ".\fast\objectIDTable.obj"
	-@erase ".\fast\klassKlass.obj"
	-@erase ".\fast\mapConformance.obj"
	-@erase ".\fast\system_prims.obj"
	-@erase ".\fast\inliner.obj"
	-@erase ".\fast\loopOpt.obj"
	-@erase ".\fast\objArray_prims.obj"
	-@erase ".\fast\dispatchTable.obj"
	-@erase ".\fast\interpreter_asm.obj"
	-@erase ".\fast\smi_prims_asm.obj"
	-@erase ".\fast\callBackStubs.obj"
	-@erase ".\fast\block_prims_asm.obj"
	-@erase ".\fast\double_prims_asm.obj"
	-@erase ".\fast\util_asm.obj"
	-@erase ".\fast\behavior_prims_asm.obj"
	-@erase ".\fast\strongtalk_fast.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /I ".." /I "..\asm" /I "..\code" /I "..\compiler" /I "..\disasm" /I "..\interpreter" /I "..\lookup" /I "..\memory" /I "..\oops" /I "..\prims" /I "..\recompiler" /I "..\runtime" /I "..\topIncludes" /I "..\utilities" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "MICROSOFT" /D "ASSERT" /D "DEBUG" /D "DELTA_COMPILER" /FR /Yu"incls/_precompiled.incl" /c
# ADD CPP /nologo /W3 /Gm /GX /Zi  /Oi /Ot  /Ob2 /Oy /Gs /GF /Gy /I "." /I "incls" /I "..\vm\asm" /I "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I "..\vm\interpreter" /I "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I "..\vm\prims" /I "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes" /I "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I "C:/Program Files/MSDEV/INCL" /D "WIN32" /D "_CONSOLE" /D "MICROSOFT" /D "DELTA_COMPILER" /D "DEBUG" /Yu"incls/_precompiled.incl" /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi  /Oi /Ot  /Ob2 /Oy /Gs /GF /Gy /I "." /I "incls" /I "..\vm\asm" /I\
 "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I "..\vm\interpreter" /I\
 "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I "..\vm\prims" /I\
 "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes" /I\
 "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I\
 "C:/Program Files/MSDEV/INCL" /D "WIN32" /D "_CONSOLE" /D "MICROSOFT" /D\
 "DELTA_COMPILER" /D "DEBUG" /Fp"$(INTDIR)/strongtalk.pch"\
 /Yu"incls/_precompiled.incl" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\fast/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/strongtalk.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"../strongtalk_fast.exe"
# SUBTRACT LINK32 /profile
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/strongtalk_fast.pdb" /debug\
 /machine:I386 /out:"../strongtalk_fast.exe" 
LINK32_OBJS= \
	"$(INTDIR)/bytecodes.obj" \
	"$(INTDIR)/dByteArrayOop.obj" \
	"$(INTDIR)/proxyKlass.obj" \
	"$(INTDIR)/smiKlass.obj" \
	"$(INTDIR)/vmSymbols.obj" \
	"$(INTDIR)/abort.obj" \
	"$(INTDIR)/opcode.obj" \
	"$(INTDIR)/tempDecoder.obj" \
	"$(INTDIR)/stackChunk.obj" \
	"$(INTDIR)/objArrayOop.obj" \
	"$(INTDIR)/debugPrintable.obj" \
	"$(INTDIR)/mixinOop.obj" \
	"$(INTDIR)/fprofiler.obj" \
	"$(INTDIR)/oop.obj" \
	"$(INTDIR)/compiledIC.obj" \
	"$(INTDIR)/registerMask.obj" \
	"$(INTDIR)/runtime.obj" \
	"$(INTDIR)/processOop.obj" \
	"$(INTDIR)/universe.obj" \
	"$(INTDIR)/universe_more.obj" \
	"$(INTDIR)/smiOop.obj" \
	"$(INTDIR)/blockOop.obj" \
	"$(INTDIR)/defUse.obj" \
	"$(INTDIR)/oopFactory.obj" \
	"$(INTDIR)/vframeOop.obj" \
	"$(INTDIR)/dValueArrayOop.obj" \
	"$(INTDIR)/symbolOop.obj" \
	"$(INTDIR)/compileTimeClosure.obj" \
	"$(INTDIR)/dll.obj" \
	"$(INTDIR)/spaceSize.obj" \
	"$(INTDIR)/snapshot.obj" \
	"$(INTDIR)/primInliner.obj" \
	"$(INTDIR)/dByteArrayKlass.obj" \
	"$(INTDIR)/methodPrinter.obj" \
	"$(INTDIR)/sweeper.obj" \
	"$(INTDIR)/ncode.obj" \
	"$(INTDIR)/systemAverage.obj" \
	"$(INTDIR)/weakArrayKlass.obj" \
	"$(INTDIR)/uncommonBranch.obj" \
	"$(INTDIR)/zoneHeap.obj" \
	"$(INTDIR)/printlayout.obj" \
	"$(INTDIR)/prettyPrinter.obj" \
	"$(INTDIR)/nmethodScopes.obj" \
	"$(INTDIR)/delta.obj" \
	"$(INTDIR)/memOop.obj" \
	"$(INTDIR)/symbolTable.obj" \
	"$(INTDIR)/behavior_prims.obj" \
	"$(INTDIR)/prim.obj" \
	"$(INTDIR)/frame.obj" \
	"$(INTDIR)/basicBlock.obj" \
	"$(INTDIR)/floats.obj" \
	"$(INTDIR)/error.obj" \
	"$(INTDIR)/vframe.obj" \
	"$(INTDIR)/ic_iterator.obj" \
	"$(INTDIR)/recompile.obj" \
	"$(INTDIR)/klass.obj" \
	"$(INTDIR)/codeGenerator.obj" \
	"$(INTDIR)/allocation.obj" \
	"$(INTDIR)/double_prims.obj" \
	"$(INTDIR)/dByteArray_prims.obj" \
	"$(INTDIR)/space.obj" \
	"$(INTDIR)/debug_prims.obj" \
	"$(INTDIR)/dValueArrayKlass.obj" \
	"$(INTDIR)/block_prims.obj" \
	"$(INTDIR)/byteArray_prims.obj" \
	"$(INTDIR)/init.obj" \
	"$(INTDIR)/objArrayKlass.obj" \
	"$(INTDIR)/oop_prims.obj" \
	"$(INTDIR)/longInt.obj" \
	"$(INTDIR)/eventlog.obj" \
	"$(INTDIR)/growableArray.obj" \
	"$(INTDIR)/scope.obj" \
	"$(INTDIR)/bits.obj" \
	"$(INTDIR)/rframe.obj" \
	"$(INTDIR)/nativeInstruction.obj" \
	"$(INTDIR)/task.obj" \
	"$(INTDIR)/interpreter.obj" \
	"$(INTDIR)/proxyOop.obj" \
	"$(INTDIR)/assembler.obj" \
	"$(INTDIR)/ostream.obj" \
	"$(INTDIR)/bootstrap.obj" \
	"$(INTDIR)/key.obj" \
	"$(INTDIR)/iterator.obj" \
	"$(INTDIR)/intervalInfo.obj" \
	"$(INTDIR)/processKlass.obj" \
	"$(INTDIR)/regAlloc.obj" \
	"$(INTDIR)/methodIterator.obj" \
	"$(INTDIR)/interpretedIC.obj" \
	"$(INTDIR)/dValueArray_prims.obj" \
	"$(INTDIR)/ageTable.obj" \
	"$(INTDIR)/nodeBuilder.obj" \
	"$(INTDIR)/lookupCache.obj" \
	"$(INTDIR)/rscope.obj" \
	"$(INTDIR)/associationOop.obj" \
	"$(INTDIR)/vframeKlass.obj" \
	"$(INTDIR)/scopeDesc.obj" \
	"$(INTDIR)/recompiler.obj" \
	"$(INTDIR)/klassOop.obj" \
	"$(INTDIR)/expr.obj" \
	"$(INTDIR)/compUtils.obj" \
	"$(INTDIR)/compiler.obj" \
	"$(INTDIR)/relocInfo.obj" \
	"$(INTDIR)/symbolKlass.obj" \
	"$(INTDIR)/evaluator.obj" \
	"$(INTDIR)/arguments.obj" \
	"$(INTDIR)/node.obj" \
	"$(INTDIR)/jumpTable.obj" \
	"$(INTDIR)/bitVector.obj" \
	"$(INTDIR)/location.obj" \
	"$(INTDIR)/interpreterStatistics.obj" \
	"$(INTDIR)/mixinKlass.obj" \
	"$(INTDIR)/methodKlass.obj" \
	"$(INTDIR)/scopeDescRecorder.obj" \
	"$(INTDIR)/integerOps.obj" \
	"$(INTDIR)/os.obj" \
	"$(INTDIR)/timer.obj" \
	"$(INTDIR)/codeTable.obj" \
	"$(INTDIR)/callBack.obj" \
	"$(INTDIR)/reflection.obj" \
	"$(INTDIR)/byteArrayOop.obj" \
	"$(INTDIR)/disclaimer.obj" \
	"$(INTDIR)/pregMapping.obj" \
	"$(INTDIR)/proxy_prims.obj" \
	"$(INTDIR)/mixin_prims.obj" \
	"$(INTDIR)/x86_mapping.obj" \
	"$(INTDIR)/blockKlass.obj" \
	"$(INTDIR)/rSet.obj" \
	"$(INTDIR)/debug.obj" \
	"$(INTDIR)/smi_prims.obj" \
	"$(INTDIR)/compiledPIC.obj" \
	"$(INTDIR)/preg.obj" \
	"$(INTDIR)/inliningdb.obj" \
	"$(INTDIR)/stubRoutines.obj" \
	"$(INTDIR)/util.obj" \
	"$(INTDIR)/associationKlass.obj" \
	"$(INTDIR)/iCache.obj" \
	"$(INTDIR)/lprintf.obj" \
	"$(INTDIR)/nameDesc.obj" \
	"$(INTDIR)/markSweep.obj" \
	"$(INTDIR)/callBack_prims.obj" \
	"$(INTDIR)/disassembler.obj" \
	"$(INTDIR)/process_prims.obj" \
	"$(INTDIR)/pcDesc.obj" \
	"$(INTDIR)/doubleKlass.obj" \
	"$(INTDIR)/markOop.obj" \
	"$(INTDIR)/codeIterator.obj" \
	"$(INTDIR)/slist.obj" \
	"$(INTDIR)/costModel.obj" \
	"$(INTDIR)/process.obj" \
	"$(INTDIR)/shell.obj" \
	"$(INTDIR)/locations.obj" \
	"$(INTDIR)/method_prims.obj" \
	"$(INTDIR)/savedRegisters.obj" \
	"$(INTDIR)/handle.obj" \
	"$(INTDIR)/nmethod.obj" \
	"$(INTDIR)/methodOop.obj" \
	"$(INTDIR)/oldCodeGenerator.obj" \
	"$(INTDIR)/memOopKlass.obj" \
	"$(INTDIR)/lookupTypes.obj" \
	"$(INTDIR)/vmOperations.obj" \
	"$(INTDIR)/generation.obj" \
	"$(INTDIR)/zone.obj" \
	"$(INTDIR)/doubleOop.obj" \
	"$(INTDIR)/codeBuffer.obj" \
	"$(INTDIR)/byteArrayKlass.obj" \
	"$(INTDIR)/virtualspace.obj" \
	"$(INTDIR)/vframe_prims.obj" \
	"$(INTDIR)/objectIDTable.obj" \
	"$(INTDIR)/klassKlass.obj" \
	"$(INTDIR)/mapConformance.obj" \
	"$(INTDIR)/system_prims.obj" \
	"$(INTDIR)/inliner.obj" \
	"$(INTDIR)/loopOpt.obj" \
	"$(INTDIR)/objArray_prims.obj" \
	"$(INTDIR)/dispatchTable.obj" \
	"$(INTDIR)/interpreter_asm.obj" \
	"$(INTDIR)/smi_prims_asm.obj" \
	"$(INTDIR)/callBackStubs.obj" \
	"$(INTDIR)/block_prims_asm.obj" \
	"$(INTDIR)/double_prims_asm.obj" \
	"$(INTDIR)/util_asm.obj" \
	"$(INTDIR)/behavior_prims_asm.obj"

"$(OUTDIR)\strongtalk_fast.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Delta___"
# PROP BASE Intermediate_Dir "Delta___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "product"
# PROP Intermediate_Dir "product"
# PROP Target_Dir ""
OUTDIR=.\product
INTDIR=.\product

ALL : "$(OUTDIR)\strongtalk.exe"

CLEAN : 
	-@erase "..\strongtalk.exe"
	-@erase ".\product\callBack.obj"
	-@erase ".\product\strongtalk.pch"
	-@erase ".\product\generation.obj"
	-@erase ".\product\registerMask.obj"
	-@erase ".\product\node.obj"
	-@erase ".\product\codeBuffer.obj"
	-@erase ".\product\growableArray.obj"
	-@erase ".\product\fprofiler.obj"
	-@erase ".\product\scopeDesc.obj"
	-@erase ".\product\doubleKlass.obj"
	-@erase ".\product\nativeInstruction.obj"
	-@erase ".\product\relocInfo.obj"
	-@erase ".\product\callBack_prims.obj"
	-@erase ".\product\klassKlass.obj"
	-@erase ".\product\vframeOop.obj"
	-@erase ".\product\nameDesc.obj"
	-@erase ".\product\evaluator.obj"
	-@erase ".\product\jumpTable.obj"
	-@erase ".\product\spaceSize.obj"
	-@erase ".\product\rSet.obj"
	-@erase ".\product\dValueArray_prims.obj"
	-@erase ".\product\memOopKlass.obj"
	-@erase ".\product\preg.obj"
	-@erase ".\product\lookupTypes.obj"
	-@erase ".\product\util.obj"
	-@erase ".\product\proxyKlass.obj"
	-@erase ".\product\stackChunk.obj"
	-@erase ".\product\debug_prims.obj"
	-@erase ".\product\smiOop.obj"
	-@erase ".\product\block_prims.obj"
	-@erase ".\product\defUse.obj"
	-@erase ".\product\dispatchTable.obj"
	-@erase ".\product\ncode.obj"
	-@erase ".\product\timer.obj"
	-@erase ".\product\compiledIC.obj"
	-@erase ".\product\double_prims.obj"
	-@erase ".\product\lprintf.obj"
	-@erase ".\product\dByteArrayOop.obj"
	-@erase ".\product\processOop.obj"
	-@erase ".\product\iterator.obj"
	-@erase ".\product\byteArrayKlass.obj"
	-@erase ".\product\oopFactory.obj"
	-@erase ".\product\markOop.obj"
	-@erase ".\product\debug.obj"
	-@erase ".\product\costModel.obj"
	-@erase ".\product\process.obj"
	-@erase ".\product\frame.obj"
	-@erase ".\product\associationKlass.obj"
	-@erase ".\product\error.obj"
	-@erase ".\product\klass.obj"
	-@erase ".\product\memOop.obj"
	-@erase ".\product\method_prims.obj"
	-@erase ".\product\tempDecoder.obj"
	-@erase ".\product\space.obj"
	-@erase ".\product\nmethod.obj"
	-@erase ".\product\dByteArrayKlass.obj"
	-@erase ".\product\vframe.obj"
	-@erase ".\product\smiKlass.obj"
	-@erase ".\product\intervalInfo.obj"
	-@erase ".\product\assembler.obj"
	-@erase ".\product\pcDesc.obj"
	-@erase ".\product\processKlass.obj"
	-@erase ".\product\systemAverage.obj"
	-@erase ".\product\doubleOop.obj"
	-@erase ".\product\bits.obj"
	-@erase ".\product\debugPrintable.obj"
	-@erase ".\product\basicBlock.obj"
	-@erase ".\product\longInt.obj"
	-@erase ".\product\task.obj"
	-@erase ".\product\oldCodeGenerator.obj"
	-@erase ".\product\mixinOop.obj"
	-@erase ".\product\prettyPrinter.obj"
	-@erase ".\product\allocation.obj"
	-@erase ".\product\dByteArray_prims.obj"
	-@erase ".\product\universe.obj"
	-@erase ".\product\primInliner.obj"
	-@erase ".\product\blockOop.obj"
	-@erase ".\product\dValueArrayKlass.obj"
	-@erase ".\product\inliner.obj"
	-@erase ".\product\loopOpt.obj"
	-@erase ".\product\rframe.obj"
	-@erase ".\product\compUtils.obj"
	-@erase ".\product\snapshot.obj"
	-@erase ".\product\codeGenerator.obj"
	-@erase ".\product\oop.obj"
	-@erase ".\product\printlayout.obj"
	-@erase ".\product\arguments.obj"
	-@erase ".\product\weakArrayKlass.obj"
	-@erase ".\product\uncommonBranch.obj"
	-@erase ".\product\vmSymbols.obj"
	-@erase ".\product\bitVector.obj"
	-@erase ".\product\expr.obj"
	-@erase ".\product\symbolTable.obj"
	-@erase ".\product\compiledPIC.obj"
	-@erase ".\product\byteArray_prims.obj"
	-@erase ".\product\zoneHeap.obj"
	-@erase ".\product\objArrayKlass.obj"
	-@erase ".\product\byteArrayOop.obj"
	-@erase ".\product\dll.obj"
	-@erase ".\product\os.obj"
	-@erase ".\product\objectIDTable.obj"
	-@erase ".\product\behavior_prims.obj"
	-@erase ".\product\ic_iterator.obj"
	-@erase ".\product\codeTable.obj"
	-@erase ".\product\runtime.obj"
	-@erase ".\product\rscope.obj"
	-@erase ".\product\savedRegisters.obj"
	-@erase ".\product\abort.obj"
	-@erase ".\product\stubRoutines.obj"
	-@erase ".\product\symbolOop.obj"
	-@erase ".\product\smi_prims.obj"
	-@erase ".\product\recompiler.obj"
	-@erase ".\product\compileTimeClosure.obj"
	-@erase ".\product\disassembler.obj"
	-@erase ".\product\opcode.obj"
	-@erase ".\product\sweeper.obj"
	-@erase ".\product\markSweep.obj"
	-@erase ".\product\codeIterator.obj"
	-@erase ".\product\interpretedIC.obj"
	-@erase ".\product\eventlog.obj"
	-@erase ".\product\prim.obj"
	-@erase ".\product\mapConformance.obj"
	-@erase ".\product\interpreter.obj"
	-@erase ".\product\mixinKlass.obj"
	-@erase ".\product\interpreterStatistics.obj"
	-@erase ".\product\objArray_prims.obj"
	-@erase ".\product\universe_more.obj"
	-@erase ".\product\integerOps.obj"
	-@erase ".\product\locations.obj"
	-@erase ".\product\reflection.obj"
	-@erase ".\product\proxyOop.obj"
	-@erase ".\product\disclaimer.obj"
	-@erase ".\product\slist.obj"
	-@erase ".\product\methodOop.obj"
	-@erase ".\product\vmOperations.obj"
	-@erase ".\product\recompile.obj"
	-@erase ".\product\init.obj"
	-@erase ".\product\methodPrinter.obj"
	-@erase ".\product\nodeBuilder.obj"
	-@erase ".\product\lookupCache.obj"
	-@erase ".\product\iCache.obj"
	-@erase ".\product\blockKlass.obj"
	-@erase ".\product\delta.obj"
	-@erase ".\product\methodIterator.obj"
	-@erase ".\product\regAlloc.obj"
	-@erase ".\product\vframeKlass.obj"
	-@erase ".\product\scopeDescRecorder.obj"
	-@erase ".\product\shell.obj"
	-@erase ".\product\virtualspace.obj"
	-@erase ".\product\inliningdb.obj"
	-@erase ".\product\ageTable.obj"
	-@erase ".\product\associationOop.obj"
	-@erase ".\product\vframe_prims.obj"
	-@erase ".\product\oop_prims.obj"
	-@erase ".\product\symbolKlass.obj"
	-@erase ".\product\nmethodScopes.obj"
	-@erase ".\product\system_prims.obj"
	-@erase ".\product\key.obj"
	-@erase ".\product\klassOop.obj"
	-@erase ".\product\compiler.obj"
	-@erase ".\product\zone.obj"
	-@erase ".\product\dValueArrayOop.obj"
	-@erase ".\product\methodKlass.obj"
	-@erase ".\product\objArrayOop.obj"
	-@erase ".\product\handle.obj"
	-@erase ".\product\floats.obj"
	-@erase ".\product\location.obj"
	-@erase ".\product\ostream.obj"
	-@erase ".\product\pregMapping.obj"
	-@erase ".\product\bootstrap.obj"
	-@erase ".\product\process_prims.obj"
	-@erase ".\product\proxy_prims.obj"
	-@erase ".\product\mixin_prims.obj"
	-@erase ".\product\x86_mapping.obj"
	-@erase ".\product\bytecodes.obj"
	-@erase ".\product\scope.obj"
	-@erase ".\product\interpreter_asm.obj"
	-@erase ".\product\smi_prims_asm.obj"
	-@erase ".\product\callBackStubs.obj"
	-@erase ".\product\block_prims_asm.obj"
	-@erase ".\product\double_prims_asm.obj"
	-@erase ".\product\util_asm.obj"
	-@erase ".\product\behavior_prims_asm.obj"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi  /Oi /Ot  /Ob2 /Oy /Gs /GF /Gy /I "." /I "incls" /I "..\vm\asm" /I "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I "..\vm\interpreter" /I "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I "..\vm\prims" /I "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes" /I "..\vm\utilities" /D "WIN32" /D "_CONSOLE" /D "MICROSOFT" /D "DELTA_COMPILER" /D "DEBUG" /Yu"incls/_precompiled.incl" /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /W3 /GX  /Oi /Ot  /Ob2 /Oy /Gs /GF /Gy /I "." /I "incls" /I "..\vm\asm" /I "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I "..\vm\interpreter" /I "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I "..\vm\prims" /I "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes" /I "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I "C:/Program Files/MSDEV/INCL" /D "PRODUCT" /D "WIN32" /D "_CONSOLE" /D "MICROSOFT" /D "DELTA_COMPILER" /Yu"incls/_precompiled.incl" /c
CPP_PROJ=/nologo /MLd /W3 /GX  /Oi /Ot  /Ob2 /Oy /Gs /GF /Gy /I "." /I "incls" /I "..\vm\asm" /I\
 "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I "..\vm\interpreter" /I\
 "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I "..\vm\prims" /I\
 "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes" /I\
 "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I\
 "C:/Program Files/MSDEV/INCL" /D "PRODUCT" /D "WIN32" /D "_CONSOLE" /D\
 "MICROSOFT" /D "DELTA_COMPILER" /Fp"$(INTDIR)/strongtalk.pch"\
 /Yu"incls/_precompiled.incl" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\product/
CPP_SBRS=
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/strongtalk.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"../strongtalk.exe"
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /pdb:none /machine:I386 /out:"../strongtalk.exe"
# SUBTRACT LINK32 /profile /debug
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /pdb:none /machine:I386 /out:"../strongtalk.exe" 
LINK32_OBJS= \
	"$(INTDIR)/callBack.obj" \
	"$(INTDIR)/generation.obj" \
	"$(INTDIR)/registerMask.obj" \
	"$(INTDIR)/node.obj" \
	"$(INTDIR)/codeBuffer.obj" \
	"$(INTDIR)/growableArray.obj" \
	"$(INTDIR)/fprofiler.obj" \
	"$(INTDIR)/scopeDesc.obj" \
	"$(INTDIR)/doubleKlass.obj" \
	"$(INTDIR)/nativeInstruction.obj" \
	"$(INTDIR)/relocInfo.obj" \
	"$(INTDIR)/callBack_prims.obj" \
	"$(INTDIR)/klassKlass.obj" \
	"$(INTDIR)/vframeOop.obj" \
	"$(INTDIR)/nameDesc.obj" \
	"$(INTDIR)/evaluator.obj" \
	"$(INTDIR)/jumpTable.obj" \
	"$(INTDIR)/spaceSize.obj" \
	"$(INTDIR)/rSet.obj" \
	"$(INTDIR)/dValueArray_prims.obj" \
	"$(INTDIR)/memOopKlass.obj" \
	"$(INTDIR)/preg.obj" \
	"$(INTDIR)/lookupTypes.obj" \
	"$(INTDIR)/util.obj" \
	"$(INTDIR)/proxyKlass.obj" \
	"$(INTDIR)/stackChunk.obj" \
	"$(INTDIR)/debug_prims.obj" \
	"$(INTDIR)/smiOop.obj" \
	"$(INTDIR)/block_prims.obj" \
	"$(INTDIR)/defUse.obj" \
	"$(INTDIR)/dispatchTable.obj" \
	"$(INTDIR)/ncode.obj" \
	"$(INTDIR)/timer.obj" \
	"$(INTDIR)/compiledIC.obj" \
	"$(INTDIR)/double_prims.obj" \
	"$(INTDIR)/lprintf.obj" \
	"$(INTDIR)/dByteArrayOop.obj" \
	"$(INTDIR)/processOop.obj" \
	"$(INTDIR)/iterator.obj" \
	"$(INTDIR)/byteArrayKlass.obj" \
	"$(INTDIR)/oopFactory.obj" \
	"$(INTDIR)/markOop.obj" \
	"$(INTDIR)/debug.obj" \
	"$(INTDIR)/costModel.obj" \
	"$(INTDIR)/process.obj" \
	"$(INTDIR)/frame.obj" \
	"$(INTDIR)/associationKlass.obj" \
	"$(INTDIR)/error.obj" \
	"$(INTDIR)/klass.obj" \
	"$(INTDIR)/memOop.obj" \
	"$(INTDIR)/method_prims.obj" \
	"$(INTDIR)/tempDecoder.obj" \
	"$(INTDIR)/space.obj" \
	"$(INTDIR)/nmethod.obj" \
	"$(INTDIR)/dByteArrayKlass.obj" \
	"$(INTDIR)/vframe.obj" \
	"$(INTDIR)/smiKlass.obj" \
	"$(INTDIR)/intervalInfo.obj" \
	"$(INTDIR)/assembler.obj" \
	"$(INTDIR)/pcDesc.obj" \
	"$(INTDIR)/processKlass.obj" \
	"$(INTDIR)/systemAverage.obj" \
	"$(INTDIR)/doubleOop.obj" \
	"$(INTDIR)/bits.obj" \
	"$(INTDIR)/debugPrintable.obj" \
	"$(INTDIR)/basicBlock.obj" \
	"$(INTDIR)/longInt.obj" \
	"$(INTDIR)/task.obj" \
	"$(INTDIR)/oldCodeGenerator.obj" \
	"$(INTDIR)/mixinOop.obj" \
	"$(INTDIR)/prettyPrinter.obj" \
	"$(INTDIR)/allocation.obj" \
	"$(INTDIR)/dByteArray_prims.obj" \
	"$(INTDIR)/universe.obj" \
	"$(INTDIR)/primInliner.obj" \
	"$(INTDIR)/blockOop.obj" \
	"$(INTDIR)/dValueArrayKlass.obj" \
	"$(INTDIR)/inliner.obj" \
	"$(INTDIR)/loopOpt.obj" \
	"$(INTDIR)/rframe.obj" \
	"$(INTDIR)/compUtils.obj" \
	"$(INTDIR)/snapshot.obj" \
	"$(INTDIR)/codeGenerator.obj" \
	"$(INTDIR)/oop.obj" \
	"$(INTDIR)/printlayout.obj" \
	"$(INTDIR)/arguments.obj" \
	"$(INTDIR)/weakArrayKlass.obj" \
	"$(INTDIR)/uncommonBranch.obj" \
	"$(INTDIR)/vmSymbols.obj" \
	"$(INTDIR)/bitVector.obj" \
	"$(INTDIR)/expr.obj" \
	"$(INTDIR)/symbolTable.obj" \
	"$(INTDIR)/compiledPIC.obj" \
	"$(INTDIR)/byteArray_prims.obj" \
	"$(INTDIR)/zoneHeap.obj" \
	"$(INTDIR)/objArrayKlass.obj" \
	"$(INTDIR)/byteArrayOop.obj" \
	"$(INTDIR)/dll.obj" \
	"$(INTDIR)/os.obj" \
	"$(INTDIR)/objectIDTable.obj" \
	"$(INTDIR)/behavior_prims.obj" \
	"$(INTDIR)/ic_iterator.obj" \
	"$(INTDIR)/codeTable.obj" \
	"$(INTDIR)/runtime.obj" \
	"$(INTDIR)/rscope.obj" \
	"$(INTDIR)/savedRegisters.obj" \
	"$(INTDIR)/abort.obj" \
	"$(INTDIR)/stubRoutines.obj" \
	"$(INTDIR)/symbolOop.obj" \
	"$(INTDIR)/smi_prims.obj" \
	"$(INTDIR)/recompiler.obj" \
	"$(INTDIR)/compileTimeClosure.obj" \
	"$(INTDIR)/disassembler.obj" \
	"$(INTDIR)/opcode.obj" \
	"$(INTDIR)/sweeper.obj" \
	"$(INTDIR)/markSweep.obj" \
	"$(INTDIR)/codeIterator.obj" \
	"$(INTDIR)/interpretedIC.obj" \
	"$(INTDIR)/eventlog.obj" \
	"$(INTDIR)/prim.obj" \
	"$(INTDIR)/mapConformance.obj" \
	"$(INTDIR)/interpreter.obj" \
	"$(INTDIR)/mixinKlass.obj" \
	"$(INTDIR)/interpreterStatistics.obj" \
	"$(INTDIR)/objArray_prims.obj" \
	"$(INTDIR)/universe_more.obj" \
	"$(INTDIR)/integerOps.obj" \
	"$(INTDIR)/locations.obj" \
	"$(INTDIR)/reflection.obj" \
	"$(INTDIR)/proxyOop.obj" \
	"$(INTDIR)/disclaimer.obj" \
	"$(INTDIR)/slist.obj" \
	"$(INTDIR)/methodOop.obj" \
	"$(INTDIR)/vmOperations.obj" \
	"$(INTDIR)/recompile.obj" \
	"$(INTDIR)/init.obj" \
	"$(INTDIR)/methodPrinter.obj" \
	"$(INTDIR)/nodeBuilder.obj" \
	"$(INTDIR)/lookupCache.obj" \
	"$(INTDIR)/iCache.obj" \
	"$(INTDIR)/blockKlass.obj" \
	"$(INTDIR)/delta.obj" \
	"$(INTDIR)/methodIterator.obj" \
	"$(INTDIR)/regAlloc.obj" \
	"$(INTDIR)/vframeKlass.obj" \
	"$(INTDIR)/scopeDescRecorder.obj" \
	"$(INTDIR)/shell.obj" \
	"$(INTDIR)/virtualspace.obj" \
	"$(INTDIR)/inliningdb.obj" \
	"$(INTDIR)/ageTable.obj" \
	"$(INTDIR)/associationOop.obj" \
	"$(INTDIR)/vframe_prims.obj" \
	"$(INTDIR)/oop_prims.obj" \
	"$(INTDIR)/symbolKlass.obj" \
	"$(INTDIR)/nmethodScopes.obj" \
	"$(INTDIR)/system_prims.obj" \
	"$(INTDIR)/key.obj" \
	"$(INTDIR)/klassOop.obj" \
	"$(INTDIR)/compiler.obj" \
	"$(INTDIR)/zone.obj" \
	"$(INTDIR)/dValueArrayOop.obj" \
	"$(INTDIR)/methodKlass.obj" \
	"$(INTDIR)/objArrayOop.obj" \
	"$(INTDIR)/handle.obj" \
	"$(INTDIR)/floats.obj" \
	"$(INTDIR)/location.obj" \
	"$(INTDIR)/ostream.obj" \
	"$(INTDIR)/pregMapping.obj" \
	"$(INTDIR)/bootstrap.obj" \
	"$(INTDIR)/process_prims.obj" \
	"$(INTDIR)/proxy_prims.obj" \
	"$(INTDIR)/mixin_prims.obj" \
	"$(INTDIR)/x86_mapping.obj" \
	"$(INTDIR)/bytecodes.obj" \
	"$(INTDIR)/scope.obj" \
	"$(INTDIR)/interpreter_asm.obj" \
	"$(INTDIR)/smi_prims_asm.obj" \
	"$(INTDIR)/callBackStubs.obj" \
	"$(INTDIR)/block_prims_asm.obj" \
	"$(INTDIR)/double_prims_asm.obj" \
	"$(INTDIR)/util_asm.obj" \
	"$(INTDIR)/behavior_prims_asm.obj"

"$(OUTDIR)\strongtalk.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "Delta - Win32 Debug"
# Name "Delta - Win32 Fast"
# Name "Delta - Win32 Product"

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=..\vm\asm\x86_mapping.cpp
NODEP_CPP_X86_M=\
	".\..\vm\asm\incls\_precompiled.incl"\
	".\..\vm\asm\incls\_x86_mapping.cpp.incl"\
	

!IF  "$(CFG)" == "Delta - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\x86_mapping.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\x86_mapping.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"


"$(INTDIR)\x86_mapping.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"


"$(INTDIR)\x86_mapping.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\asm\location.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_LOCAT=\
	".\..\vm\asm\incls\_precompiled.incl"\
	".\..\vm\asm\incls\_location.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\location.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\location.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_LOCAT=\
	".\..\vm\asm\incls\_precompiled.incl"\
	".\..\vm\asm\incls\_location.cpp.incl"\
	

"$(INTDIR)\location.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_LOCAT=\
	".\incls\_precompiled.incl"\
	".\incls\_location.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\location.obj" : $(SOURCE) $(DEP_CPP_LOCAT) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\asm\x86_mapping.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\asm\location.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\zoneHeap.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_ZONEH=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_zoneHeap.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\zoneHeap.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\zoneHeap.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_ZONEH=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_zoneHeap.cpp.incl"\
	

"$(INTDIR)\zoneHeap.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_ZONEH=\
	".\incls\_precompiled.incl"\
	".\incls\_zoneHeap.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\zoneHeap.obj" : $(SOURCE) $(DEP_CPP_ZONEH) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\zone.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_ZONE_=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_zone.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\zone.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\zone.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_ZONE_=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_zone.cpp.incl"\
	

"$(INTDIR)\zone.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_ZONE_=\
	".\incls\_precompiled.incl"\
	".\incls\_zone.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\code\compiledPIC.hpp"\
	".\..\vm\runtime\delta.hpp"\
	".\..\vm\runtime\timer.hpp"\
	

"$(INTDIR)\zone.obj" : $(SOURCE) $(DEP_CPP_ZONE_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\scopeDescRecorder.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_SCOPE=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_scopeDescRecorder.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\scopeDescRecorder.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\scopeDescRecorder.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_SCOPE=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_scopeDescRecorder.cpp.incl"\
	

"$(INTDIR)\scopeDescRecorder.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_SCOPE=\
	".\incls\_precompiled.incl"\
	".\incls\_scopeDescRecorder.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\code\pcDesc.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	

"$(INTDIR)\scopeDescRecorder.obj" : $(SOURCE) $(DEP_CPP_SCOPE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\scopeDesc.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_SCOPED=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_scopeDesc.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\scopeDesc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\scopeDesc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_SCOPED=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_scopeDesc.cpp.incl"\
	

"$(INTDIR)\scopeDesc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_SCOPED=\
	".\incls\_precompiled.incl"\
	".\incls\_scopeDesc.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\methodKlass.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\expr.hpp"\
	

"$(INTDIR)\scopeDesc.obj" : $(SOURCE) $(DEP_CPP_SCOPED) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\relocInfo.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_RELOC=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_relocInfo.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\relocInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\relocInfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_RELOC=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_relocInfo.cpp.incl"\
	

"$(INTDIR)\relocInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_RELOC=\
	".\incls\_precompiled.incl"\
	".\incls\_relocInfo.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\code\compiledPIC.hpp"\
	".\..\vm\code\stubRoutines.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\runtime\uncommonBranch.hpp"\
	

"$(INTDIR)\relocInfo.obj" : $(SOURCE) $(DEP_CPP_RELOC) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\registerMask.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_REGIS=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_registerMask.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\registerMask.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\registerMask.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_REGIS=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_registerMask.cpp.incl"\
	

"$(INTDIR)\registerMask.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_REGIS=\
	".\incls\_precompiled.incl"\
	".\incls\_registerMask.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	

"$(INTDIR)\registerMask.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\pcDesc.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_PCDES=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_pcDesc.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\pcDesc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\pcDesc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_PCDES=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_pcDesc.cpp.incl"\
	

"$(INTDIR)\pcDesc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_PCDES=\
	".\incls\_precompiled.incl"\
	".\incls\_pcDesc.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\code\pcDesc.hpp"\
	

"$(INTDIR)\pcDesc.obj" : $(SOURCE) $(DEP_CPP_PCDES) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\nmethodScopes.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_NMETH=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_nmethodScopes.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\nmethodScopes.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\nmethodScopes.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_NMETH=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_nmethodScopes.cpp.incl"\
	

"$(INTDIR)\nmethodScopes.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_NMETH=\
	".\incls\_precompiled.incl"\
	".\incls\_nmethodScopes.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\code\compiledPIC.hpp"\
	".\..\vm\oops\methodKlass.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\recompiler\recompile.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	

"$(INTDIR)\nmethodScopes.obj" : $(SOURCE) $(DEP_CPP_NMETH) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\nmethod.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_NMETHO=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_nmethod.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\nmethod.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\nmethod.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_NMETHO=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_nmethod.cpp.incl"\
	

"$(INTDIR)\nmethod.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_NMETHO=\
	".\incls\_precompiled.incl"\
	".\incls\_nmethod.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\compiler\basicBlock.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	".\..\vm\compiler\node.hpp"\
	".\..\vm\code\locations.hpp"\
	".\..\vm\code\pregMapping.hpp"\
	".\..\vm\compiler\codeGenerator.hpp"\
	".\..\vm\disasm\disassembler.hpp"\
	".\..\vm\code\inliningdb.hpp"\
	".\..\vm\oops\methodKlass.hpp"\
	".\..\vm\code\pcDesc.hpp"\
	".\..\vm\code\compiledPIC.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\recompiler\recompile.hpp"\
	".\..\vm\compiler\rscope.hpp"\
	".\..\vm\code\stubRoutines.hpp"\
	

"$(INTDIR)\nmethod.obj" : $(SOURCE) $(DEP_CPP_NMETHO) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\ncode.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_NCODE=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_ncode.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\ncode.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\ncode.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_NCODE=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_ncode.cpp.incl"\
	

"$(INTDIR)\ncode.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_NCODE=\
	".\incls\_precompiled.incl"\
	".\incls\_ncode.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\code\compiledPIC.hpp"\
	

"$(INTDIR)\ncode.obj" : $(SOURCE) $(DEP_CPP_NCODE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\nameDesc.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_NAMED=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_nameDesc.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\nameDesc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\nameDesc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_NAMED=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_nameDesc.cpp.incl"\
	

"$(INTDIR)\nameDesc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_NAMED=\
	".\incls\_precompiled.incl"\
	".\incls\_nameDesc.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\stackChunk.hpp"\
	

"$(INTDIR)\nameDesc.obj" : $(SOURCE) $(DEP_CPP_NAMED) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\jumpTable.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_JUMPT=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_jumpTable.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\jumpTable.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\jumpTable.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_JUMPT=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_jumpTable.cpp.incl"\
	

"$(INTDIR)\jumpTable.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_JUMPT=\
	".\incls\_precompiled.incl"\
	".\incls\_jumpTable.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\disasm\disassembler.hpp"\
	".\..\vm\code\stubRoutines.hpp"\
	

"$(INTDIR)\jumpTable.obj" : $(SOURCE) $(DEP_CPP_JUMPT) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\iCache.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_ICACH=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_iCache.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\iCache.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\iCache.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_ICACH=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_iCache.cpp.incl"\
	

"$(INTDIR)\iCache.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_ICACH=\
	".\incls\_precompiled.incl"\
	".\incls\_iCache.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\iCache.obj" : $(SOURCE) $(DEP_CPP_ICACH) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\compiledPIC.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_COMPI=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_compiledPIC.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\compiledPIC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\compiledPIC.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_COMPI=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_compiledPIC.cpp.incl"\
	

"$(INTDIR)\compiledPIC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_COMPI=\
	".\incls\_precompiled.incl"\
	".\incls\_compiledPIC.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\disasm\disassembler.hpp"\
	".\..\vm\code\compiledPIC.hpp"\
	".\..\vm\code\stubRoutines.hpp"\
	

"$(INTDIR)\compiledPIC.obj" : $(SOURCE) $(DEP_CPP_COMPI) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\compiledIC.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_COMPIL=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_compiledIC.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\compiledIC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\compiledIC.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_COMPIL=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_compiledIC.cpp.incl"\
	

"$(INTDIR)\compiledIC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_COMPIL=\
	".\incls\_precompiled.incl"\
	".\incls\_compiledIC.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\evaluator.hpp"\
	".\..\vm\runtime\delta.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\interpreter\dispatchTable.hpp"\
	".\..\vm\interpreter\interpreter.hpp"\
	".\..\vm\code\pcDesc.hpp"\
	".\..\vm\code\compiledPIC.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\recompiler\recompile.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\code\stubRoutines.hpp"\
	

"$(INTDIR)\compiledIC.obj" : $(SOURCE) $(DEP_CPP_COMPIL) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\codeTable.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_CODET=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_codeTable.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\codeTable.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\codeTable.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_CODET=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_codeTable.cpp.incl"\
	

"$(INTDIR)\codeTable.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_CODET=\
	".\incls\_precompiled.incl"\
	".\incls\_codeTable.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\codeTable.obj" : $(SOURCE) $(DEP_CPP_CODET) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\zoneHeap.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\zone.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\scopeDescRecorder.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\scopeDesc.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\relocInfo.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\registerMask.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\pcDesc.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\nmethodScopes.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\nmethod.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\ncode.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\nameDesc.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\jumpTable.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\iCache.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\compiledPIC.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\compiledIC.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\codeTable.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\slist.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_SLIST=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_slist.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\slist.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\slist.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_SLIST=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_slist.cpp.incl"\
	

"$(INTDIR)\slist.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_SLIST=\
	".\incls\_precompiled.incl"\
	".\incls\_slist.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\slist.obj" : $(SOURCE) $(DEP_CPP_SLIST) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\scope.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_SCOPE_=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_scope.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\scope.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\scope.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_SCOPE_=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_scope.cpp.incl"\
	

"$(INTDIR)\scope.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_SCOPE_=\
	".\incls\_precompiled.incl"\
	".\incls\_scope.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\compiler\basicBlock.hpp"\
	".\..\vm\compiler\loopOpt.hpp"\
	".\..\vm\oops\methodKlass.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	".\..\vm\compiler\node.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\recompiler\recompile.hpp"\
	".\..\vm\compiler\regAlloc.hpp"\
	".\..\vm\code\pcDesc.hpp"\
	".\..\vm\compiler\rscope.hpp"\
	

"$(INTDIR)\scope.obj" : $(SOURCE) $(DEP_CPP_SCOPE_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\rscope.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_RSCOP=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_rscope.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\rscope.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\rscope.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_RSCOP=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_rscope.cpp.incl"\
	

"$(INTDIR)\rscope.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_RSCOP=\
	".\incls\_precompiled.incl"\
	".\incls\_rscope.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\code\compiledPIC.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\oops\methodKlass.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\recompiler\recompile.hpp"\
	".\..\vm\code\pcDesc.hpp"\
	".\..\vm\compiler\rscope.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\runtime\uncommonBranch.hpp"\
	

"$(INTDIR)\rscope.obj" : $(SOURCE) $(DEP_CPP_RSCOP) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\regAlloc.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_REGAL=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_regAlloc.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\regAlloc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\regAlloc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_REGAL=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_regAlloc.cpp.incl"\
	

"$(INTDIR)\regAlloc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_REGAL=\
	".\incls\_precompiled.incl"\
	".\incls\_regAlloc.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\compiler\basicBlock.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	".\..\vm\compiler\node.hpp"\
	".\..\vm\compiler\regAlloc.hpp"\
	

"$(INTDIR)\regAlloc.obj" : $(SOURCE) $(DEP_CPP_REGAL) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\primInliner.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_PRIMI=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_primInliner.cpp.incl"\
	

BuildCmds= \
	$(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /I "." /I "incls" /I "..\vm\asm" /I\
 "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I "..\vm\interpreter" /I\
 "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I "..\vm\prims" /I\
 "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes" /I\
 "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I\
 "C:/Program Files/MSDEV/INCL" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D\
 "MICROSOFT" /D "ASSERT" /D "DEBUG" /D "DELTA_COMPILER" /FR"$(INTDIR)/"\
 /Fp"$(INTDIR)/strongtalk.pch" /Yu"incls/_precompiled.incl" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\primInliner.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\primInliner.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_PRIMI=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_primInliner.cpp.incl"\
	
# ADD CPP /Ot  /Oi /Op 
# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\primInliner.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Ot  /Oi /Op  /I "." /I "incls" /I\
 "..\vm\asm" /I "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I\
 "..\vm\interpreter" /I "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I\
 "..\vm\prims" /I "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes"\
 /I "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I\
 "C:/Program Files/MSDEV/INCL" /D "WIN32" /D "_CONSOLE" /D "MICROSOFT" /D\
 "DELTA_COMPILER" /D "DEBUG" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_PRIMI=\
	".\incls\_precompiled.incl"\
	".\incls\_primInliner.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\behavior_prims.hpp"\
	".\..\vm\oops\byteArrayKlass.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\compiler\basicBlock.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	".\..\vm\compiler\node.hpp"\
	".\..\vm\oops\objArrayKlass.hpp"\
	".\..\vm\compiler\primInliner.hpp"\
	".\..\vm\code\pcDesc.hpp"\
	".\..\vm\compiler\rscope.hpp"\
	
# ADD BASE CPP /Ot  /Oi /Op 
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /Ot  /Oi /Op 
# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\primInliner.obj" : $(SOURCE) $(DEP_CPP_PRIMI) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /GX /Ot  /Oi /Op  /I "." /I "incls" /I "..\vm\asm"\
 /I "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I "..\vm\interpreter" /I\
 "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I "..\vm\prims" /I\
 "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes" /I\
 "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I\
 "C:/Program Files/MSDEV/INCL" /D "PRODUCT" /D "WIN32" /D "_CONSOLE" /D\
 "MICROSOFT" /D "DELTA_COMPILER" /Fo"$(INTDIR)/" /c $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\preg.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_PREG_=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_preg.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\preg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\preg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_PREG_=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_preg.cpp.incl"\
	

"$(INTDIR)\preg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_PREG_=\
	".\incls\_precompiled.incl"\
	".\incls\_preg.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\compiler\basicBlock.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	".\..\vm\compiler\node.hpp"\
	".\..\vm\compiler\regAlloc.hpp"\
	".\..\vm\compiler\split.hpp"\
	

"$(INTDIR)\preg.obj" : $(SOURCE) $(DEP_CPP_PREG_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\opcode.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_OPCOD=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_opcode.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\opcode.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\opcode.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_OPCOD=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_opcode.cpp.incl"\
	

"$(INTDIR)\opcode.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_OPCOD=\
	".\incls\_precompiled.incl"\
	".\incls\_opcode.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\opcode.obj" : $(SOURCE) $(DEP_CPP_OPCOD) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\nodeBuilder.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_NODEB=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_nodeBuilder.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\nodeBuilder.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\nodeBuilder.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_NODEB=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_nodeBuilder.cpp.incl"\
	

"$(INTDIR)\nodeBuilder.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_NODEB=\
	".\incls\_precompiled.incl"\
	".\incls\_nodeBuilder.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\compiler\loopOpt.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\compiler\basicBlock.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	".\..\vm\compiler\node.hpp"\
	".\..\vm\compiler\primInliner.hpp"\
	".\..\vm\oops\proxyOop.hpp"\
	

"$(INTDIR)\nodeBuilder.obj" : $(SOURCE) $(DEP_CPP_NODEB) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\node.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_NODE_=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_node.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\node.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\node.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_NODE_=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_node.cpp.incl"\
	

"$(INTDIR)\node.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_NODE_=\
	".\incls\_precompiled.incl"\
	".\incls\_node.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\compiler\basicBlock.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\behavior_prims.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\compiler\loopOpt.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	".\..\vm\compiler\node.hpp"\
	".\..\vm\compiler\node.inline.hpp"\
	".\..\vm\code\locations.hpp"\
	".\..\vm\code\pregMapping.hpp"\
	".\..\vm\oops\processOop.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\recompiler\recompile.hpp"\
	".\..\vm\code\pcDesc.hpp"\
	".\..\vm\compiler\rscope.hpp"\
	".\..\vm\prims\smi_prims.hpp"\
	".\..\vm\compiler\split.hpp"\
	

"$(INTDIR)\node.obj" : $(SOURCE) $(DEP_CPP_NODE_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\inliner.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_INLIN=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_inliner.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\inliner.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\inliner.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_INLIN=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_inliner.cpp.incl"\
	

"$(INTDIR)\inliner.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_INLIN=\
	".\incls\_precompiled.incl"\
	".\incls\_inliner.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\methodKlass.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\compiler\basicBlock.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	".\..\vm\compiler\node.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\recompiler\recompile.hpp"\
	".\..\vm\code\pcDesc.hpp"\
	".\..\vm\compiler\rscope.hpp"\
	".\..\vm\recompiler\rframe.hpp"\
	

"$(INTDIR)\inliner.obj" : $(SOURCE) $(DEP_CPP_INLIN) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\expr.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_EXPR_=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_expr.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\expr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\expr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_EXPR_=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_expr.cpp.incl"\
	

"$(INTDIR)\expr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_EXPR_=\
	".\incls\_precompiled.incl"\
	".\incls\_expr.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\compiler\basicBlock.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	".\..\vm\compiler\node.hpp"\
	

"$(INTDIR)\expr.obj" : $(SOURCE) $(DEP_CPP_EXPR_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\defUse.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_DEFUS=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_defUse.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\defUse.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\defUse.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_DEFUS=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_defUse.cpp.incl"\
	

"$(INTDIR)\defUse.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_DEFUS=\
	".\incls\_precompiled.incl"\
	".\incls\_defUse.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\compiler\basicBlock.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	".\..\vm\compiler\node.hpp"\
	

"$(INTDIR)\defUse.obj" : $(SOURCE) $(DEP_CPP_DEFUS) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\compUtils.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_COMPU=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_compUtils.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\compUtils.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\compUtils.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_COMPU=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_compUtils.cpp.incl"\
	

"$(INTDIR)\compUtils.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_COMPU=\
	".\incls\_precompiled.incl"\
	".\incls\_compUtils.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\compiler\basicBlock.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	".\..\vm\compiler\node.hpp"\
	

"$(INTDIR)\compUtils.obj" : $(SOURCE) $(DEP_CPP_COMPU) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\compileTimeClosure.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_COMPILE=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_compileTimeClosure.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\compileTimeClosure.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\compileTimeClosure.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_COMPILE=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_compileTimeClosure.cpp.incl"\
	

"$(INTDIR)\compileTimeClosure.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_COMPILE=\
	".\incls\_precompiled.incl"\
	".\incls\_compileTimeClosure.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	

"$(INTDIR)\compileTimeClosure.obj" : $(SOURCE) $(DEP_CPP_COMPILE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\compiler.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_COMPILER=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_compiler.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\compiler.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\compiler.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_COMPILER=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_compiler.cpp.incl"\
	

"$(INTDIR)\compiler.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_COMPILER=\
	".\incls\_precompiled.incl"\
	".\incls\_compiler.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\compiler\basicBlock.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	".\..\vm\compiler\node.hpp"\
	".\..\vm\code\locations.hpp"\
	".\..\vm\code\pregMapping.hpp"\
	".\..\vm\compiler\codeGenerator.hpp"\
	".\..\vm\disasm\disassembler.hpp"\
	".\..\vm\code\inliningdb.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\interpreter\dispatchTable.hpp"\
	".\..\vm\interpreter\interpreter.hpp"\
	".\..\vm\compiler\oldCodeGenerator.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\recompiler\recompile.hpp"\
	".\..\vm\recompiler\recompiler.hpp"\
	".\..\vm\compiler\regAlloc.hpp"\
	".\..\vm\code\pcDesc.hpp"\
	".\..\vm\compiler\rscope.hpp"\
	".\..\vm\runtime\timer.hpp"\
	

"$(INTDIR)\compiler.obj" : $(SOURCE) $(DEP_CPP_COMPILER) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\codeGenerator.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_CODEG=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_codeGenerator.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\codeGenerator.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\codeGenerator.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_CODEG=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_codeGenerator.cpp.incl"\
	

"$(INTDIR)\codeGenerator.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_CODEG=\
	".\incls\_precompiled.incl"\
	".\incls\_codeGenerator.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\block_prims.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\compiler\basicBlock.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	".\..\vm\compiler\node.hpp"\
	".\..\vm\code\locations.hpp"\
	".\..\vm\code\pregMapping.hpp"\
	".\..\vm\compiler\codeGenerator.hpp"\
	".\..\vm\disasm\disassembler.hpp"\
	".\..\vm\compiler\loopOpt.hpp"\
	".\..\vm\oops\proxyOop.hpp"\
	".\..\vm\recompiler\recompiler.hpp"\
	".\..\vm\compiler\regAlloc.hpp"\
	".\..\vm\code\stubRoutines.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\runtime\uncommonBranch.hpp"\
	

"$(INTDIR)\codeGenerator.obj" : $(SOURCE) $(DEP_CPP_CODEG) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\bitVector.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_BITVE=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_bitVector.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\bitVector.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\bitVector.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_BITVE=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_bitVector.cpp.incl"\
	

"$(INTDIR)\bitVector.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_BITVE=\
	".\incls\_precompiled.incl"\
	".\incls\_bitVector.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	

"$(INTDIR)\bitVector.obj" : $(SOURCE) $(DEP_CPP_BITVE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\basicBlock.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_BASIC=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_basicBlock.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\basicBlock.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\basicBlock.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_BASIC=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_basicBlock.cpp.incl"\
	

"$(INTDIR)\basicBlock.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_BASIC=\
	".\incls\_precompiled.incl"\
	".\incls\_basicBlock.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\compiler\basicBlock.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	".\..\vm\compiler\node.hpp"\
	".\..\vm\compiler\regAlloc.hpp"\
	

"$(INTDIR)\basicBlock.obj" : $(SOURCE) $(DEP_CPP_BASIC) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\slist.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\scope.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\rscope.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\regAlloc.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\primInliner.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\preg.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\opcode.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\nodeBuilder.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\node.inline.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\node.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\inliner.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\expr.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\defUse.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\compUtils.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\compileTimeClosure.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\compiler.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\codeGenerator.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\bitVector.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\basicBlock.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\disasm\disassembler.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\disasm\disassembler.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_DISAS=\
	".\..\vm\disasm\incls\_precompiled.incl"\
	".\..\vm\disasm\incls\_disassembler.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\disassembler.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\disassembler.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_DISAS=\
	".\..\vm\disasm\incls\_precompiled.incl"\
	".\..\vm\disasm\incls\_disassembler.cpp.incl"\
	

"$(INTDIR)\disassembler.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_DISAS=\
	".\incls\_precompiled.incl"\
	".\incls\_disassembler.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\disasm\disassembler.hpp"\
	".\..\vm\interpreter\prettyPrinter.hpp"\
	".\..\vm\interpreter\methodPrinter.hpp"\
	".\..\vm\code\pcDesc.hpp"\
	".\..\vm\runtime\tempDecoder.hpp"\
	

"$(INTDIR)\disassembler.obj" : $(SOURCE) $(DEP_CPP_DISAS) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\interpreter\prettyPrinter.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\interpreter\prettyPrinter.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_PRETT=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_prettyPrinter.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\prettyPrinter.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\prettyPrinter.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_PRETT=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_prettyPrinter.cpp.incl"\
	

"$(INTDIR)\prettyPrinter.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_PRETT=\
	".\incls\_precompiled.incl"\
	".\incls\_prettyPrinter.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\interpreter\prettyPrinter.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\runtime\tempDecoder.hpp"\
	".\..\vm\oops\dByteArrayOop.hpp"\
	

"$(INTDIR)\prettyPrinter.obj" : $(SOURCE) $(DEP_CPP_PRETT) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\interpreter\methodPrinter.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\interpreter\methodPrinter.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_METHO=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_methodPrinter.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\methodPrinter.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\methodPrinter.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_METHO=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_methodPrinter.cpp.incl"\
	

"$(INTDIR)\methodPrinter.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_METHO=\
	".\incls\_precompiled.incl"\
	".\incls\_methodPrinter.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\interpreter\methodPrinter.hpp"\
	

"$(INTDIR)\methodPrinter.obj" : $(SOURCE) $(DEP_CPP_METHO) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\interpreter\methodIterator.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\interpreter\methodIterator.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_METHOD=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_methodIterator.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\methodIterator.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\methodIterator.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_METHOD=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_methodIterator.cpp.incl"\
	

"$(INTDIR)\methodIterator.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_METHOD=\
	".\incls\_precompiled.incl"\
	".\incls\_methodIterator.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	

"$(INTDIR)\methodIterator.obj" : $(SOURCE) $(DEP_CPP_METHOD) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\interpreter\interpreterStatistics.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\interpreter\interpreterStatistics.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_INTER=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_interpreterStatistics.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\interpreterStatistics.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\interpreterStatistics.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_INTER=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_interpreterStatistics.cpp.incl"\
	

"$(INTDIR)\interpreterStatistics.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_INTER=\
	".\incls\_precompiled.incl"\
	".\incls\_interpreterStatistics.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\interpreter\interpreterStatistics.hpp"\
	

"$(INTDIR)\interpreterStatistics.obj" : $(SOURCE) $(DEP_CPP_INTER) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\interpreter\interpreter_asm.asm

!IF  "$(CFG)" == "Delta - Win32 Debug"

# Begin Custom Build
OutDir=.\debug
InputPath=..\vm\interpreter\interpreter_asm.asm
InputName=interpreter_asm

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

# Begin Custom Build
OutDir=.\fast
InputPath=..\vm\interpreter\interpreter_asm.asm
InputName=interpreter_asm

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

# Begin Custom Build
OutDir=.\product
InputPath=..\vm\interpreter\interpreter_asm.asm
InputName=interpreter_asm

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\interpreter\interpreter.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\interpreter\interpreter.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_INTERP=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_interpreter.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\interpreter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\interpreter.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_INTERP=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_interpreter.cpp.incl"\
	

"$(INTDIR)\interpreter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_INTERP=\
	".\incls\_precompiled.incl"\
	".\incls\_interpreter.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\disasm\disassembler.hpp"\
	".\..\vm\runtime\disclaimer.hpp"\
	".\..\vm\interpreter\dispatchTable.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\interpreter\interpreter.hpp"\
	".\..\vm\oops\proxyOop.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\recompiler\recompile.hpp"\
	".\..\vm\code\stubRoutines.hpp"\
	

"$(INTDIR)\interpreter.obj" : $(SOURCE) $(DEP_CPP_INTERP) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\interpreter\ic_iterator.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\interpreter\ic_iterator.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_IC_IT=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_ic_iterator.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\ic_iterator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\ic_iterator.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_IC_IT=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_ic_iterator.cpp.incl"\
	

"$(INTDIR)\ic_iterator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_IC_IT=\
	".\incls\_precompiled.incl"\
	".\incls\_ic_iterator.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\code\compiledPIC.hpp"\
	

"$(INTDIR)\ic_iterator.obj" : $(SOURCE) $(DEP_CPP_IC_IT) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\interpreter\dispatchTable.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\interpreter\dispatchTable.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_DISPA=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_dispatchTable.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dispatchTable.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\dispatchTable.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_DISPA=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_dispatchTable.cpp.incl"\
	

"$(INTDIR)\dispatchTable.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_DISPA=\
	".\incls\_precompiled.incl"\
	".\incls\_dispatchTable.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\interpreter\dispatchTable.hpp"\
	

"$(INTDIR)\dispatchTable.obj" : $(SOURCE) $(DEP_CPP_DISPA) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\interpreter\codeIterator.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\interpreter\codeIterator.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_CODEI=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_codeIterator.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\codeIterator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\codeIterator.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_CODEI=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_codeIterator.cpp.incl"\
	

"$(INTDIR)\codeIterator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_CODEI=\
	".\incls\_precompiled.incl"\
	".\incls\_codeIterator.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\interpreter\dispatchTable.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	

"$(INTDIR)\codeIterator.obj" : $(SOURCE) $(DEP_CPP_CODEI) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\lookup\lookupTypes.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\lookup\lookupTypes.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_LOOKU=\
	".\..\vm\lookup\incls\_precompiled.incl"\
	".\..\vm\lookup\incls\_lookupTypes.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\lookupTypes.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\lookupTypes.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_LOOKU=\
	".\..\vm\lookup\incls\_precompiled.incl"\
	".\..\vm\lookup\incls\_lookupTypes.cpp.incl"\
	

"$(INTDIR)\lookupTypes.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_LOOKU=\
	".\incls\_precompiled.incl"\
	".\incls\_lookupTypes.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\lookup\lookupTypes.hpp"\
	

"$(INTDIR)\lookupTypes.obj" : $(SOURCE) $(DEP_CPP_LOOKU) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\lookup\lookupCache.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\lookup\lookupCache.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_LOOKUP=\
	".\..\vm\lookup\incls\_precompiled.incl"\
	".\..\vm\lookup\incls\_lookupCache.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\lookupCache.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\lookupCache.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_LOOKUP=\
	".\..\vm\lookup\incls\_precompiled.incl"\
	".\..\vm\lookup\incls\_lookupCache.cpp.incl"\
	

"$(INTDIR)\lookupCache.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_LOOKUP=\
	".\incls\_precompiled.incl"\
	".\incls\_lookupCache.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\code\inliningdb.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\code\pcDesc.hpp"\
	".\..\vm\compiler\rscope.hpp"\
	".\..\vm\runtime\timer.hpp"\
	".\..\vm\runtime\sweeper.hpp"\
	

"$(INTDIR)\lookupCache.obj" : $(SOURCE) $(DEP_CPP_LOOKUP) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\lookup\key.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\lookup\key.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_KEY_C=\
	".\..\vm\lookup\incls\_precompiled.incl"\
	".\..\vm\lookup\incls\_key.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\key.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\key.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_KEY_C=\
	".\..\vm\lookup\incls\_precompiled.incl"\
	".\..\vm\lookup\incls\_key.cpp.incl"\
	

"$(INTDIR)\key.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_KEY_C=\
	".\incls\_precompiled.incl"\
	".\incls\_key.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\key.obj" : $(SOURCE) $(DEP_CPP_KEY_C) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\vmSymbols.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_VMSYM=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_vmSymbols.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\vmSymbols.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\vmSymbols.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_VMSYM=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_vmSymbols.cpp.incl"\
	

"$(INTDIR)\vmSymbols.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_VMSYM=\
	".\incls\_precompiled.incl"\
	".\incls\_vmSymbols.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\vmSymbols.obj" : $(SOURCE) $(DEP_CPP_VMSYM) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\util.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_UTIL_=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_util.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\util.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\util.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_UTIL_=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_util.cpp.incl"\
	

"$(INTDIR)\util.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_UTIL_=\
	".\incls\_precompiled.incl"\
	".\incls\_util.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\util.obj" : $(SOURCE) $(DEP_CPP_UTIL_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\universe_more.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_UNIVE=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_universe.more.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\universe_more.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\universe_more.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_UNIVE=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_universe.more.cpp.incl"\
	

"$(INTDIR)\universe_more.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_UNIVE=\
	".\incls\_precompiled.incl"\
	".\incls\_universe.more.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\oops\byteArrayKlass.hpp"\
	".\..\vm\oops\doubleKlass.hpp"\
	".\..\vm\oops\klassKlass.hpp"\
	".\..\vm\oops\objArrayKlass.hpp"\
	".\..\vm\oops\processOop.hpp"\
	".\..\vm\oops\processKlass.hpp"\
	".\..\vm\memory\rSet.inline.hpp"\
	".\..\vm\oops\smiKlass.hpp"\
	".\..\vm\memory\symbolTable.hpp"\
	".\..\vm\oops\symbolKlass.hpp"\
	".\..\vm\runtime\timer.hpp"\
	".\..\vm\oops\weakArrayKlass.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\memory\handle.hpp"\
	

"$(INTDIR)\universe_more.obj" : $(SOURCE) $(DEP_CPP_UNIVE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\universe.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_UNIVER=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_universe.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\universe.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\universe.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_UNIVER=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_universe.cpp.incl"\
	

"$(INTDIR)\universe.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_UNIVER=\
	".\incls\_precompiled.incl"\
	".\incls\_universe.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\oops\byteArrayKlass.hpp"\
	".\..\vm\runtime\disclaimer.hpp"\
	".\..\vm\oops\doubleKlass.hpp"\
	".\..\vm\oops\klassKlass.hpp"\
	".\..\vm\oops\objArrayKlass.hpp"\
	".\..\vm\oops\processOop.hpp"\
	".\..\vm\oops\processKlass.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\interpreter\dispatchTable.hpp"\
	".\..\vm\interpreter\interpreter.hpp"\
	".\..\vm\code\inliningdb.hpp"\
	".\..\vm\runtime\shell.hpp"\
	".\..\vm\oops\smiKlass.hpp"\
	".\..\vm\memory\symbolTable.hpp"\
	".\..\vm\oops\symbolKlass.hpp"\
	".\..\vm\utilities\objectIDTable.hpp"\
	".\..\vm\memory\printlayout.hpp"\
	".\..\vm\interpreter\prettyPrinter.hpp"\
	".\..\vm\oops\mixinOop.hpp"\
	".\..\vm\runtime\delta.hpp"\
	".\..\vm\memory\handle.hpp"\
	

"$(INTDIR)\universe.obj" : $(SOURCE) $(DEP_CPP_UNIVER) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\symbolTable.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_SYMBO=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_symbolTable.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\symbolTable.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\symbolTable.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_SYMBO=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_symbolTable.cpp.incl"\
	

"$(INTDIR)\symbolTable.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_SYMBO=\
	".\incls\_precompiled.incl"\
	".\incls\_symbolTable.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\byteArrayKlass.hpp"\
	".\..\vm\memory\symbolTable.hpp"\
	".\..\vm\oops\symbolKlass.hpp"\
	

"$(INTDIR)\symbolTable.obj" : $(SOURCE) $(DEP_CPP_SYMBO) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\spaceSize.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_SPACE=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_spaceSize.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\spaceSize.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\spaceSize.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_SPACE=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_spaceSize.cpp.incl"\
	

"$(INTDIR)\spaceSize.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_SPACE=\
	".\incls\_precompiled.incl"\
	".\incls\_spaceSize.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\byteArrayKlass.hpp"\
	".\..\vm\memory\symbolTable.hpp"\
	".\..\vm\oops\symbolKlass.hpp"\
	

"$(INTDIR)\spaceSize.obj" : $(SOURCE) $(DEP_CPP_SPACE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\space.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_SPACE_=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_space.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\space.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\space.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_SPACE_=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_space.cpp.incl"\
	

"$(INTDIR)\space.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_SPACE_=\
	".\incls\_precompiled.incl"\
	".\incls\_space.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\processOop.hpp"\
	".\..\vm\memory\rSet.inline.hpp"\
	

"$(INTDIR)\space.obj" : $(SOURCE) $(DEP_CPP_SPACE_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\snapshot.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_SNAPS=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_snapshot.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\snapshot.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\snapshot.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_SNAPS=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_snapshot.cpp.incl"\
	

"$(INTDIR)\snapshot.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_SNAPS=\
	".\incls\_precompiled.incl"\
	".\incls\_snapshot.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\memory\snapshot.hpp"\
	

"$(INTDIR)\snapshot.obj" : $(SOURCE) $(DEP_CPP_SNAPS) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\rSet.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_RSET_=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_rSet.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\rSet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\rSet.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_RSET_=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_rSet.cpp.incl"\
	

"$(INTDIR)\rSet.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_RSET_=\
	".\incls\_precompiled.incl"\
	".\incls\_rSet.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\memory\rSet.inline.hpp"\
	

"$(INTDIR)\rSet.obj" : $(SOURCE) $(DEP_CPP_RSET_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\printlayout.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_PRINT=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_printlayout.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\printlayout.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\printlayout.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_PRINT=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_printlayout.cpp.incl"\
	

"$(INTDIR)\printlayout.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_PRINT=\
	".\incls\_precompiled.incl"\
	".\incls\_printlayout.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\memory\printlayout.hpp"\
	

"$(INTDIR)\printlayout.obj" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\oopFactory.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_OOPFA=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_oopFactory.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\oopFactory.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\oopFactory.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_OOPFA=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_oopFactory.cpp.incl"\
	

"$(INTDIR)\oopFactory.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_OOPFA=\
	".\incls\_precompiled.incl"\
	".\incls\_oopFactory.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\oops\associationKlass.hpp"\
	".\..\vm\oops\byteArrayKlass.hpp"\
	".\..\vm\oops\objArrayKlass.hpp"\
	".\..\vm\memory\symbolTable.hpp"\
	".\..\vm\oops\doubleKlass.hpp"\
	".\..\vm\oops\vframeOop.hpp"\
	".\..\vm\oops\vframeKlass.hpp"\
	".\..\vm\oops\processOop.hpp"\
	

"$(INTDIR)\oopFactory.obj" : $(SOURCE) $(DEP_CPP_OOPFA) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\markSweep.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_MARKS=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_markSweep.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\markSweep.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\markSweep.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_MARKS=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_markSweep.cpp.incl"\
	

"$(INTDIR)\markSweep.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_MARKS=\
	".\incls\_precompiled.incl"\
	".\incls\_markSweep.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\delta.hpp"\
	".\..\vm\runtime\timer.hpp"\
	".\..\vm\oops\objArrayKlass.hpp"\
	".\..\vm\oops\weakArrayKlass.hpp"\
	".\..\vm\memory\rSet.inline.hpp"\
	".\..\vm\memory\symbolTable.hpp"\
	

"$(INTDIR)\markSweep.obj" : $(SOURCE) $(DEP_CPP_MARKS) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\iterator.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_ITERA=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_iterator.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\iterator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\iterator.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_ITERA=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_iterator.cpp.incl"\
	

"$(INTDIR)\iterator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_ITERA=\
	".\incls\_precompiled.incl"\
	".\incls\_iterator.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\iterator.obj" : $(SOURCE) $(DEP_CPP_ITERA) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\generation.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_GENER=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_generation.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\generation.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\generation.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_GENER=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_generation.cpp.incl"\
	

"$(INTDIR)\generation.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_GENER=\
	".\incls\_precompiled.incl"\
	".\incls\_generation.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\abort.hpp"\
	

"$(INTDIR)\generation.obj" : $(SOURCE) $(DEP_CPP_GENER) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\debugPrintable.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_DEBUG=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_debugPrintable.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\debugPrintable.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\debugPrintable.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_DEBUG=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_debugPrintable.cpp.incl"\
	

"$(INTDIR)\debugPrintable.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_DEBUG=\
	".\incls\_precompiled.incl"\
	".\incls\_debugPrintable.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\memory\debugPrintable.hpp"\
	".\..\vm\utilities\objectIDTable.hpp"\
	".\..\vm\interpreter\prettyPrinter.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	

"$(INTDIR)\debugPrintable.obj" : $(SOURCE) $(DEP_CPP_DEBUG) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\allocation.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_ALLOC=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_allocation.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\allocation.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\allocation.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_ALLOC=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_allocation.cpp.incl"\
	

"$(INTDIR)\allocation.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_ALLOC=\
	".\incls\_precompiled.incl"\
	".\incls\_allocation.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\abort.hpp"\
	

"$(INTDIR)\allocation.obj" : $(SOURCE) $(DEP_CPP_ALLOC) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\ageTable.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_AGETA=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_ageTable.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\ageTable.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\ageTable.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_AGETA=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_ageTable.cpp.incl"\
	

"$(INTDIR)\ageTable.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_AGETA=\
	".\incls\_precompiled.incl"\
	".\incls\_ageTable.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\ageTable.obj" : $(SOURCE) $(DEP_CPP_AGETA) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\vmSymbols.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\util.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\universe.store.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\universe.inline.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\universe.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\symbolTable.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\spaceSize.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\space.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\snapshot.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\rSet.inline.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\rSet.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\printlayout.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\oopFactory.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\markSweep.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\iterator.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\generation.inline.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\generation.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\error.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\debugPrintable.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\allocation.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\ageTable.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\weakArrayKlass.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_WEAKA=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_weakArrayKlass.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\weakArrayKlass.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\weakArrayKlass.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_WEAKA=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_weakArrayKlass.cpp.incl"\
	

"$(INTDIR)\weakArrayKlass.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_WEAKA=\
	".\incls\_precompiled.incl"\
	".\incls\_weakArrayKlass.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\objArrayKlass.hpp"\
	".\..\vm\oops\weakArrayKlass.hpp"\
	

"$(INTDIR)\weakArrayKlass.obj" : $(SOURCE) $(DEP_CPP_WEAKA) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\symbolOop.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_SYMBOL=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_symbolOop.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\symbolOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\symbolOop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_SYMBOL=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_symbolOop.cpp.incl"\
	

"$(INTDIR)\symbolOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_SYMBOL=\
	".\incls\_precompiled.incl"\
	".\incls\_symbolOop.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\runtime\shell.hpp"\
	".\..\vm\oops\byteArrayKlass.hpp"\
	".\..\vm\memory\symbolTable.hpp"\
	".\..\vm\oops\symbolKlass.hpp"\
	

"$(INTDIR)\symbolOop.obj" : $(SOURCE) $(DEP_CPP_SYMBOL) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\symbolKlass.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_SYMBOLK=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_symbolKlass.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\symbolKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\symbolKlass.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_SYMBOLK=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_symbolKlass.cpp.incl"\
	

"$(INTDIR)\symbolKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_SYMBOLK=\
	".\incls\_precompiled.incl"\
	".\incls\_symbolKlass.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\byteArrayKlass.hpp"\
	".\..\vm\memory\symbolTable.hpp"\
	".\..\vm\oops\symbolKlass.hpp"\
	

"$(INTDIR)\symbolKlass.obj" : $(SOURCE) $(DEP_CPP_SYMBOLK) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\smiOop.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_SMIOO=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_smiOop.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\smiOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\smiOop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_SMIOO=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_smiOop.cpp.incl"\
	

"$(INTDIR)\smiOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_SMIOO=\
	".\incls\_precompiled.incl"\
	".\incls\_smiOop.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\smiOop.obj" : $(SOURCE) $(DEP_CPP_SMIOO) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\smiKlass.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_SMIKL=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_smiKlass.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\smiKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\smiKlass.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_SMIKL=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_smiKlass.cpp.incl"\
	

"$(INTDIR)\smiKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_SMIKL=\
	".\incls\_precompiled.incl"\
	".\incls\_smiKlass.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\smiKlass.hpp"\
	

"$(INTDIR)\smiKlass.obj" : $(SOURCE) $(DEP_CPP_SMIKL) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\proxyOop.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_PROXY=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_proxyOop.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\proxyOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\proxyOop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_PROXY=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_proxyOop.cpp.incl"\
	

"$(INTDIR)\proxyOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_PROXY=\
	".\incls\_precompiled.incl"\
	".\incls\_proxyOop.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\proxyOop.hpp"\
	".\..\vm\runtime\bootstrap.hpp"\
	

"$(INTDIR)\proxyOop.obj" : $(SOURCE) $(DEP_CPP_PROXY) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\proxyKlass.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_PROXYK=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_proxyKlass.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\proxyKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\proxyKlass.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_PROXYK=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_proxyKlass.cpp.incl"\
	

"$(INTDIR)\proxyKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_PROXYK=\
	".\incls\_precompiled.incl"\
	".\incls\_proxyKlass.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\proxyOop.hpp"\
	".\..\vm\oops\proxyKlass.hpp"\
	

"$(INTDIR)\proxyKlass.obj" : $(SOURCE) $(DEP_CPP_PROXYK) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\processOop.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_PROCE=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_processOop.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\processOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\processOop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_PROCE=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_processOop.cpp.incl"\
	

"$(INTDIR)\processOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_PROCE=\
	".\incls\_precompiled.incl"\
	".\incls\_processOop.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\oops\processOop.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	

"$(INTDIR)\processOop.obj" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\processKlass.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_PROCES=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_processKlass.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\processKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\processKlass.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_PROCES=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_processKlass.cpp.incl"\
	

"$(INTDIR)\processKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_PROCES=\
	".\incls\_precompiled.incl"\
	".\incls\_processKlass.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\processOop.hpp"\
	".\..\vm\oops\processKlass.hpp"\
	

"$(INTDIR)\processKlass.obj" : $(SOURCE) $(DEP_CPP_PROCES) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\oop.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_OOP_C=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_oop.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\oop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\oop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_OOP_C=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_oop.cpp.incl"\
	

"$(INTDIR)\oop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_OOP_C=\
	".\incls\_precompiled.incl"\
	".\incls\_oop.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\oop.obj" : $(SOURCE) $(DEP_CPP_OOP_C) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\objArrayOop.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_OBJAR=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_objArrayOop.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\objArrayOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\objArrayOop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_OBJAR=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_objArrayOop.cpp.incl"\
	

"$(INTDIR)\objArrayOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_OBJAR=\
	".\incls\_precompiled.incl"\
	".\incls\_objArrayOop.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\objArrayKlass.hpp"\
	".\..\vm\runtime\bootstrap.hpp"\
	

"$(INTDIR)\objArrayOop.obj" : $(SOURCE) $(DEP_CPP_OBJAR) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\objArrayKlass.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_OBJARR=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_objArrayKlass.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\objArrayKlass.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\objArrayKlass.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_OBJARR=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_objArrayKlass.cpp.incl"\
	

"$(INTDIR)\objArrayKlass.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_OBJARR=\
	".\incls\_precompiled.incl"\
	".\incls\_objArrayKlass.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\objArrayKlass.hpp"\
	".\..\vm\oops\weakArrayKlass.hpp"\
	

"$(INTDIR)\objArrayKlass.obj" : $(SOURCE) $(DEP_CPP_OBJARR) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\mixinOop.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_MIXIN=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_mixinOop.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\mixinOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\mixinOop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_MIXIN=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_mixinOop.cpp.incl"\
	

"$(INTDIR)\mixinOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_MIXIN=\
	".\incls\_precompiled.incl"\
	".\incls\_mixinOop.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\mixinOop.hpp"\
	".\..\vm\runtime\bootstrap.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	

"$(INTDIR)\mixinOop.obj" : $(SOURCE) $(DEP_CPP_MIXIN) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\mixinKlass.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_MIXINK=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_mixinKlass.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\mixinKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\mixinKlass.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_MIXINK=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_mixinKlass.cpp.incl"\
	

"$(INTDIR)\mixinKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_MIXINK=\
	".\incls\_precompiled.incl"\
	".\incls\_mixinKlass.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\mixinOop.hpp"\
	".\..\vm\oops\mixinKlass.hpp"\
	

"$(INTDIR)\mixinKlass.obj" : $(SOURCE) $(DEP_CPP_MIXINK) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\methodOop.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_METHODO=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_methodOop.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\methodOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\methodOop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_METHODO=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_methodOop.cpp.incl"\
	

"$(INTDIR)\methodOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_METHODO=\
	".\incls\_precompiled.incl"\
	".\incls\_methodOop.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\bootstrap.hpp"\
	".\..\vm\compiler\costModel.hpp"\
	".\..\vm\interpreter\methodPrinter.hpp"\
	".\..\vm\interpreter\prettyPrinter.hpp"\
	".\..\vm\oops\methodKlass.hpp"\
	".\..\vm\interpreter\dispatchTable.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\oops\mixinOop.hpp"\
	

"$(INTDIR)\methodOop.obj" : $(SOURCE) $(DEP_CPP_METHODO) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\methodKlass.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_METHODK=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_methodKlass.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\methodKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\methodKlass.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_METHODK=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_methodKlass.cpp.incl"\
	

"$(INTDIR)\methodKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_METHODK=\
	".\incls\_precompiled.incl"\
	".\incls\_methodKlass.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\methodKlass.hpp"\
	

"$(INTDIR)\methodKlass.obj" : $(SOURCE) $(DEP_CPP_METHODK) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\memOopKlass.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_MEMOO=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_memOopKlass.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\memOopKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\memOopKlass.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_MEMOO=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_memOopKlass.cpp.incl"\
	

"$(INTDIR)\memOopKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_MEMOO=\
	".\incls\_precompiled.incl"\
	".\incls\_memOopKlass.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\utilities\objectIDTable.hpp"\
	".\..\vm\memory\printlayout.hpp"\
	".\..\vm\oops\mixinOop.hpp"\
	".\..\vm\oops\mixinKlass.hpp"\
	".\..\vm\oops\proxyOop.hpp"\
	".\..\vm\oops\proxyKlass.hpp"\
	".\..\vm\oops\processOop.hpp"\
	".\..\vm\oops\processKlass.hpp"\
	".\..\vm\oops\objArrayKlass.hpp"\
	".\..\vm\oops\byteArrayKlass.hpp"\
	".\..\vm\oops\dByteArrayOop.hpp"\
	".\..\vm\oops\dByteArrayKlass.hpp"\
	".\..\vm\oops\dValueArrayOop.hpp"\
	".\..\vm\oops\dValueArrayKlass.hpp"\
	".\..\vm\oops\weakArrayKlass.hpp"\
	

"$(INTDIR)\memOopKlass.obj" : $(SOURCE) $(DEP_CPP_MEMOO) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\memOop.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_MEMOOP=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_memOop.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\memOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\memOop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_MEMOOP=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_memOop.cpp.incl"\
	

"$(INTDIR)\memOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_MEMOOP=\
	".\incls\_precompiled.incl"\
	".\incls\_memOop.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\bootstrap.hpp"\
	".\..\vm\utilities\objectIDTable.hpp"\
	

"$(INTDIR)\memOop.obj" : $(SOURCE) $(DEP_CPP_MEMOOP) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\markOop.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_MARKO=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_markOop.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\markOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\markOop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_MARKO=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_markOop.cpp.incl"\
	

"$(INTDIR)\markOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_MARKO=\
	".\incls\_precompiled.incl"\
	".\incls\_markOop.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\markOop.obj" : $(SOURCE) $(DEP_CPP_MARKO) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\klassOop.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_KLASS=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_klassOop.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\klassOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\klassOop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_KLASS=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_klassOop.cpp.incl"\
	

"$(INTDIR)\klassOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_KLASS=\
	".\incls\_precompiled.incl"\
	".\incls\_klassOop.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\klassOop.obj" : $(SOURCE) $(DEP_CPP_KLASS) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\klassKlass.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_KLASSK=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_klassKlass.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\klassKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\klassKlass.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_KLASSK=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_klassKlass.cpp.incl"\
	

"$(INTDIR)\klassKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_KLASSK=\
	".\incls\_precompiled.incl"\
	".\incls\_klassKlass.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\klassKlass.hpp"\
	".\..\vm\utilities\objectIDTable.hpp"\
	

"$(INTDIR)\klassKlass.obj" : $(SOURCE) $(DEP_CPP_KLASSK) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\klass.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_KLASS_=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_klass.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\klass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\klass.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_KLASS_=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_klass.cpp.incl"\
	

"$(INTDIR)\klass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_KLASS_=\
	".\incls\_precompiled.incl"\
	".\incls\_klass.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\runtime\bootstrap.hpp"\
	".\..\vm\oops\klassKlass.hpp"\
	".\..\vm\oops\mixinOop.hpp"\
	

"$(INTDIR)\klass.obj" : $(SOURCE) $(DEP_CPP_KLASS_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\doubleOop.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_DOUBL=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_doubleOop.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\doubleOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\doubleOop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_DOUBL=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_doubleOop.cpp.incl"\
	

"$(INTDIR)\doubleOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_DOUBL=\
	".\incls\_precompiled.incl"\
	".\incls\_doubleOop.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\doubleKlass.hpp"\
	".\..\vm\oops\byteArrayKlass.hpp"\
	".\..\vm\memory\symbolTable.hpp"\
	".\..\vm\oops\symbolKlass.hpp"\
	".\..\vm\runtime\bootstrap.hpp"\
	

"$(INTDIR)\doubleOop.obj" : $(SOURCE) $(DEP_CPP_DOUBL) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\doubleKlass.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_DOUBLE=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_doubleKlass.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\doubleKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\doubleKlass.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_DOUBLE=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_doubleKlass.cpp.incl"\
	

"$(INTDIR)\doubleKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_DOUBLE=\
	".\incls\_precompiled.incl"\
	".\incls\_doubleKlass.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\doubleKlass.hpp"\
	

"$(INTDIR)\doubleKlass.obj" : $(SOURCE) $(DEP_CPP_DOUBLE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\dByteArrayOop.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_DBYTE=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_dByteArrayOop.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dByteArrayOop.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\dByteArrayOop.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_DBYTE=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_dByteArrayOop.cpp.incl"\
	

"$(INTDIR)\dByteArrayOop.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_DBYTE=\
	".\incls\_precompiled.incl"\
	".\incls\_dByteArrayOop.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\oops\dByteArrayOop.hpp"\
	".\..\vm\oops\dByteArrayKlass.hpp"\
	".\..\vm\runtime\shell.hpp"\
	".\..\vm\oops\byteArrayKlass.hpp"\
	".\..\vm\memory\symbolTable.hpp"\
	".\..\vm\oops\symbolKlass.hpp"\
	".\..\vm\runtime\bootstrap.hpp"\
	

"$(INTDIR)\dByteArrayOop.obj" : $(SOURCE) $(DEP_CPP_DBYTE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\dByteArrayKlass.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_DBYTEA=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_dByteArrayKlass.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dByteArrayKlass.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\dByteArrayKlass.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_DBYTEA=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_dByteArrayKlass.cpp.incl"\
	

"$(INTDIR)\dByteArrayKlass.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_DBYTEA=\
	".\incls\_precompiled.incl"\
	".\incls\_dByteArrayKlass.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\dByteArrayOop.hpp"\
	".\..\vm\oops\dByteArrayKlass.hpp"\
	

"$(INTDIR)\dByteArrayKlass.obj" : $(SOURCE) $(DEP_CPP_DBYTEA) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\byteArrayOop.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_BYTEA=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_byteArrayOop.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\byteArrayOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\byteArrayOop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_BYTEA=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_byteArrayOop.cpp.incl"\
	

"$(INTDIR)\byteArrayOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_BYTEA=\
	".\incls\_precompiled.incl"\
	".\incls\_byteArrayOop.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\oops\byteArrayKlass.hpp"\
	".\..\vm\runtime\shell.hpp"\
	".\..\vm\memory\symbolTable.hpp"\
	".\..\vm\oops\symbolKlass.hpp"\
	".\..\vm\runtime\bootstrap.hpp"\
	".\..\vm\oops\dByteArrayOop.hpp"\
	

"$(INTDIR)\byteArrayOop.obj" : $(SOURCE) $(DEP_CPP_BYTEA) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\byteArrayKlass.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_BYTEAR=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_byteArrayKlass.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\byteArrayKlass.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\byteArrayKlass.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_BYTEAR=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_byteArrayKlass.cpp.incl"\
	

"$(INTDIR)\byteArrayKlass.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_BYTEAR=\
	".\incls\_precompiled.incl"\
	".\incls\_byteArrayKlass.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\byteArrayKlass.hpp"\
	

"$(INTDIR)\byteArrayKlass.obj" : $(SOURCE) $(DEP_CPP_BYTEAR) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\blockOop.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_BLOCK=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_blockOop.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\blockOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\blockOop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_BLOCK=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_blockOop.cpp.incl"\
	

"$(INTDIR)\blockOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_BLOCK=\
	".\incls\_precompiled.incl"\
	".\incls\_blockOop.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\block_prims.hpp"\
	".\..\vm\runtime\stackChunk.hpp"\
	

"$(INTDIR)\blockOop.obj" : $(SOURCE) $(DEP_CPP_BLOCK) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\blockKlass.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_BLOCKK=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_blockKlass.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\blockKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\blockKlass.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_BLOCKK=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_blockKlass.cpp.incl"\
	

"$(INTDIR)\blockKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_BLOCKK=\
	".\incls\_precompiled.incl"\
	".\incls\_blockKlass.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\oops\byteArrayKlass.hpp"\
	".\..\vm\memory\symbolTable.hpp"\
	".\..\vm\oops\symbolKlass.hpp"\
	

"$(INTDIR)\blockKlass.obj" : $(SOURCE) $(DEP_CPP_BLOCKK) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\associationOop.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_ASSOC=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_associationOop.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\associationOop.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\associationOop.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_ASSOC=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_associationOop.cpp.incl"\
	

"$(INTDIR)\associationOop.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_ASSOC=\
	".\incls\_precompiled.incl"\
	".\incls\_associationOop.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\runtime\bootstrap.hpp"\
	

"$(INTDIR)\associationOop.obj" : $(SOURCE) $(DEP_CPP_ASSOC) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\associationKlass.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_ASSOCI=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_associationKlass.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\associationKlass.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\associationKlass.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_ASSOCI=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_associationKlass.cpp.incl"\
	

"$(INTDIR)\associationKlass.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_ASSOCI=\
	".\incls\_precompiled.incl"\
	".\incls\_associationKlass.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\oops\associationKlass.hpp"\
	

"$(INTDIR)\associationKlass.obj" : $(SOURCE) $(DEP_CPP_ASSOCI) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\weakArrayKlass.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\symbolOop.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\symbolKlass.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\smiOop.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\smiKlass.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\proxyOop.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\proxyKlass.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\processOop.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\processKlass.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\oopsHierarchy.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\oop.inline.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\oop.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\objArrayOop.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\objArrayKlass.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\mixinOop.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\mixinKlass.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\methodOop.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\methodKlass.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\memOop.inline.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\memOop.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\markOop.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\klassOop.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\klassKlass.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\klass.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\doubleOop.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\doubleKlass.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\dByteArrayOop.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\dByteArrayKlass.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\byteArrayOop.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\byteArrayKlass.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\blockOop.inline.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\blockOop.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\blockKlass.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\associationOop.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\associationKlass.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\system_prims.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_SYSTE=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_system_prims.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\system_prims.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\system_prims.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_SYSTE=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_system_prims.cpp.incl"\
	

"$(INTDIR)\system_prims.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_SYSTE=\
	".\incls\_precompiled.incl"\
	".\incls\_system_prims.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\system_prims.hpp"\
	".\..\vm\prims\prim_impl.hpp"\
	".\..\vm\code\inliningdb.hpp"\
	".\..\vm\oops\mixinOop.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\memory\snapshot.hpp"\
	".\..\vm\oops\proxyOop.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\oops\processOop.hpp"\
	".\..\vm\interpreter\dispatchTable.hpp"\
	".\..\vm\runtime\timer.hpp"\
	".\..\vm\runtime\fprofiler.hpp"\
	".\..\vm\oops\objArrayKlass.hpp"\
	".\..\vm\oops\weakArrayKlass.hpp"\
	".\..\vm\memory\reflection.hpp"\
	".\..\vm\code\pcDesc.hpp"\
	".\..\vm\compiler\rscope.hpp"\
	".\..\vm\oops\dByteArrayOop.hpp"\
	".\..\vm\runtime\systemAverage.hpp"\
	

"$(INTDIR)\system_prims.obj" : $(SOURCE) $(DEP_CPP_SYSTE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\smi_prims.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_SMI_P=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_smi_prims.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\smi_prims.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\smi_prims.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_SMI_P=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_smi_prims.cpp.incl"\
	

"$(INTDIR)\smi_prims.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_SMI_P=\
	".\incls\_precompiled.incl"\
	".\incls\_smi_prims.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\smi_prims.hpp"\
	".\..\vm\prims\prim_impl.hpp"\
	".\..\vm\utilities\objectIDTable.hpp"\
	

"$(INTDIR)\smi_prims.obj" : $(SOURCE) $(DEP_CPP_SMI_P) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\proxy_prims.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_PROXY_=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_proxy_prims.cpp.incl"\
	

BuildCmds= \
	$(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /I "." /I "incls" /I "..\vm\asm" /I\
 "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I "..\vm\interpreter" /I\
 "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I "..\vm\prims" /I\
 "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes" /I\
 "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I\
 "C:/Program Files/MSDEV/INCL" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D\
 "MICROSOFT" /D "ASSERT" /D "DEBUG" /D "DELTA_COMPILER" /FR"$(INTDIR)/"\
 /Fp"$(INTDIR)/strongtalk.pch" /Yu"incls/_precompiled.incl" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\proxy_prims.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\proxy_prims.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_PROXY_=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_proxy_prims.cpp.incl"\
	
# ADD CPP /Ot  /Oi /Op 
# SUBTRACT CPP /Og /Os /YX /Yc /Yu

"$(INTDIR)\proxy_prims.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Ot  /Oi /Op  /I "." /I "incls" /I\
 "..\vm\asm" /I "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I\
 "..\vm\interpreter" /I "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I\
 "..\vm\prims" /I "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes"\
 /I "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I\
 "C:/Program Files/MSDEV/INCL" /D "WIN32" /D "_CONSOLE" /D "MICROSOFT" /D\
 "DELTA_COMPILER" /D "DEBUG" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_PROXY_=\
	".\incls\_precompiled.incl"\
	".\incls\_proxy_prims.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\proxy_prims.hpp"\
	".\..\vm\prims\prim_impl.hpp"\
	".\..\vm\oops\proxyOop.hpp"\
	
# ADD BASE CPP /Ot  /Oi /Op 
# SUBTRACT BASE CPP /Og /Os /YX /Yc /Yu
# ADD CPP /Ot  /Oi /Op 
# SUBTRACT CPP /Og /Os /YX /Yc /Yu

"$(INTDIR)\proxy_prims.obj" : $(SOURCE) $(DEP_CPP_PROXY_) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /GX /Ot  /Oi /Op  /I "." /I "incls" /I "..\vm\asm"\
 /I "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I "..\vm\interpreter" /I\
 "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I "..\vm\prims" /I\
 "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes" /I\
 "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I\
 "C:/Program Files/MSDEV/INCL" /D "PRODUCT" /D "WIN32" /D "_CONSOLE" /D\
 "MICROSOFT" /D "DELTA_COMPILER" /Fo"$(INTDIR)/" /c $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\process_prims.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_PROCESS=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_process_prims.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\process_prims.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\process_prims.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_PROCESS=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_process_prims.cpp.incl"\
	

"$(INTDIR)\process_prims.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_PROCESS=\
	".\incls\_precompiled.incl"\
	".\incls\_process_prims.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\interpreter\dispatchTable.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\process_prims.hpp"\
	".\..\vm\prims\prim_impl.hpp"\
	".\..\vm\oops\processOop.hpp"\
	".\..\vm\oops\vframeOop.hpp"\
	

"$(INTDIR)\process_prims.obj" : $(SOURCE) $(DEP_CPP_PROCESS) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\prim.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

DEP_CPP_PRIM_=\
	".\..\vm\prims\prims.inc"\
	
NODEP_CPP_PRIM_=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_prim.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\prim.obj" : $(SOURCE) $(DEP_CPP_PRIM_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\prim.sbr" : $(SOURCE) $(DEP_CPP_PRIM_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

DEP_CPP_PRIM_=\
	".\..\vm\prims\prims.inc"\
	
NODEP_CPP_PRIM_=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_prim.cpp.incl"\
	

"$(INTDIR)\prim.obj" : $(SOURCE) $(DEP_CPP_PRIM_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_PRIM_=\
	".\incls\_precompiled.incl"\
	".\incls\_prim.cpp.incl"\
	".\..\vm\prims\prims.inc"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\oops\processOop.hpp"\
	".\..\vm\oops\byteArrayKlass.hpp"\
	".\..\vm\memory\symbolTable.hpp"\
	".\..\vm\oops\symbolKlass.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\oop_prims.hpp"\
	".\..\vm\prims\double_prims.hpp"\
	".\..\vm\prims\smi_prims.hpp"\
	".\..\vm\prims\proxy_prims.hpp"\
	".\..\vm\prims\behavior_prims.hpp"\
	".\..\vm\prims\debug_prims.hpp"\
	".\..\vm\prims\block_prims.hpp"\
	".\..\vm\prims\system_prims.hpp"\
	".\..\vm\prims\byteArray_prims.hpp"\
	".\..\vm\prims\dByteArray_prims.hpp"\
	".\..\vm\prims\objArray_prims.hpp"\
	".\..\vm\prims\dValueArray_prims.hpp"\
	".\..\vm\prims\process_prims.hpp"\
	".\..\vm\prims\method_prims.hpp"\
	".\..\vm\prims\mixin_prims.hpp"\
	".\..\vm\prims\callBack_prims.hpp"\
	".\..\vm\prims\vframe_prims.hpp"\
	

"$(INTDIR)\prim.obj" : $(SOURCE) $(DEP_CPP_PRIM_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\oop_prims.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_OOP_P=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_oop_prims.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\oop_prims.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\oop_prims.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_OOP_P=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_oop_prims.cpp.incl"\
	

"$(INTDIR)\oop_prims.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_OOP_P=\
	".\incls\_precompiled.incl"\
	".\incls\_oop_prims.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\delta.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\oop_prims.hpp"\
	".\..\vm\prims\prim_impl.hpp"\
	".\..\vm\utilities\objectIDTable.hpp"\
	

"$(INTDIR)\oop_prims.obj" : $(SOURCE) $(DEP_CPP_OOP_P) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\objArray_prims.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_OBJARRA=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_objArray_prims.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\objArray_prims.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\objArray_prims.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_OBJARRA=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_objArray_prims.cpp.incl"\
	

"$(INTDIR)\objArray_prims.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_OBJARRA=\
	".\incls\_precompiled.incl"\
	".\incls\_objArray_prims.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\objArray_prims.hpp"\
	".\..\vm\prims\prim_impl.hpp"\
	".\..\vm\memory\handle.hpp"\
	

"$(INTDIR)\objArray_prims.obj" : $(SOURCE) $(DEP_CPP_OBJARRA) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\mixin_prims.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_MIXIN_=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_mixin_prims.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\mixin_prims.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\mixin_prims.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_MIXIN_=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_mixin_prims.cpp.incl"\
	

"$(INTDIR)\mixin_prims.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_MIXIN_=\
	".\incls\_precompiled.incl"\
	".\incls\_mixin_prims.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\mixin_prims.hpp"\
	".\..\vm\prims\prim_impl.hpp"\
	".\..\vm\oops\mixinOop.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	

"$(INTDIR)\mixin_prims.obj" : $(SOURCE) $(DEP_CPP_MIXIN_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\method_prims.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_METHOD_=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_method_prims.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\method_prims.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\method_prims.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_METHOD_=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_method_prims.cpp.incl"\
	

"$(INTDIR)\method_prims.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_METHOD_=\
	".\incls\_precompiled.incl"\
	".\incls\_method_prims.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\method_prims.hpp"\
	".\..\vm\prims\prim_impl.hpp"\
	".\..\vm\oops\methodKlass.hpp"\
	".\..\vm\interpreter\prettyPrinter.hpp"\
	

"$(INTDIR)\method_prims.obj" : $(SOURCE) $(DEP_CPP_METHOD_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\double_prims.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_DOUBLE_=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_double_prims.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\double_prims.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\double_prims.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_DOUBLE_=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_double_prims.cpp.incl"\
	

"$(INTDIR)\double_prims.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_DOUBLE_=\
	".\incls\_precompiled.incl"\
	".\incls\_double_prims.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\double_prims.hpp"\
	".\..\vm\prims\prim_impl.hpp"\
	".\..\vm\oops\dByteArrayOop.hpp"\
	

"$(INTDIR)\double_prims.obj" : $(SOURCE) $(DEP_CPP_DOUBLE_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\dll.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_DLL_C=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_dll.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dll.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\dll.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_DLL_C=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_dll.cpp.incl"\
	

"$(INTDIR)\dll.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_DLL_C=\
	".\incls\_precompiled.incl"\
	".\incls\_dll.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\runtime\delta.hpp"\
	".\..\vm\code\stubRoutines.hpp"\
	".\..\vm\oops\proxyOop.hpp"\
	

"$(INTDIR)\dll.obj" : $(SOURCE) $(DEP_CPP_DLL_C) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\debug_prims.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_DEBUG_=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_debug_prims.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\debug_prims.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\debug_prims.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_DEBUG_=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_debug_prims.cpp.incl"\
	

"$(INTDIR)\debug_prims.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_DEBUG_=\
	".\incls\_precompiled.incl"\
	".\incls\_debug_prims.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\debug_prims.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\interpreter\dispatchTable.hpp"\
	".\..\vm\interpreter\interpreter.hpp"\
	".\..\vm\prims\prim_impl.hpp"\
	".\..\vm\interpreter\methodPrinter.hpp"\
	".\..\vm\interpreter\prettyPrinter.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	

"$(INTDIR)\debug_prims.obj" : $(SOURCE) $(DEP_CPP_DEBUG_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\dByteArray_prims.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_DBYTEAR=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_dByteArray_prims.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dByteArray_prims.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\dByteArray_prims.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_DBYTEAR=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_dByteArray_prims.cpp.incl"\
	

"$(INTDIR)\dByteArray_prims.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_DBYTEAR=\
	".\incls\_precompiled.incl"\
	".\incls\_dByteArray_prims.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\dByteArray_prims.hpp"\
	".\..\vm\prims\prim_impl.hpp"\
	".\..\vm\oops\dByteArrayOop.hpp"\
	".\..\vm\memory\symbolTable.hpp"\
	

"$(INTDIR)\dByteArray_prims.obj" : $(SOURCE) $(DEP_CPP_DBYTEAR) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\callBack_prims.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_CALLB=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_callBack_prims.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\callBack_prims.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\callBack_prims.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_CALLB=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_callBack_prims.cpp.incl"\
	

"$(INTDIR)\callBack_prims.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_CALLB=\
	".\incls\_precompiled.incl"\
	".\incls\_callBack_prims.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\callBack_prims.hpp"\
	".\..\vm\prims\prim_impl.hpp"\
	".\..\vm\runtime\callBack.hpp"\
	".\..\vm\oops\proxyOop.hpp"\
	

"$(INTDIR)\callBack_prims.obj" : $(SOURCE) $(DEP_CPP_CALLB) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\byteArray_prims.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_BYTEARR=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_byteArray_prims.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\byteArray_prims.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\byteArray_prims.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_BYTEARR=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_byteArray_prims.cpp.incl"\
	

"$(INTDIR)\byteArray_prims.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_BYTEARR=\
	".\incls\_precompiled.incl"\
	".\incls\_byteArray_prims.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\byteArray_prims.hpp"\
	".\..\vm\prims\prim_impl.hpp"\
	".\..\vm\prims\integerOps.hpp"\
	".\..\vm\memory\symbolTable.hpp"\
	

"$(INTDIR)\byteArray_prims.obj" : $(SOURCE) $(DEP_CPP_BYTEARR) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\block_prims.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_BLOCK_=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_block_prims.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\block_prims.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\block_prims.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_BLOCK_=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_block_prims.cpp.incl"\
	

"$(INTDIR)\block_prims.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_BLOCK_=\
	".\incls\_precompiled.incl"\
	".\incls\_block_prims.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\block_prims.hpp"\
	".\..\vm\prims\prim_impl.hpp"\
	".\..\vm\runtime\delta.hpp"\
	

"$(INTDIR)\block_prims.obj" : $(SOURCE) $(DEP_CPP_BLOCK_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\behavior_prims.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_BEHAV=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_behavior_prims.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\behavior_prims.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\behavior_prims.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_BEHAV=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_behavior_prims.cpp.incl"\
	

"$(INTDIR)\behavior_prims.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_BEHAV=\
	".\incls\_precompiled.incl"\
	".\incls\_behavior_prims.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\behavior_prims.hpp"\
	".\..\vm\prims\prim_impl.hpp"\
	".\..\vm\oops\methodKlass.hpp"\
	".\..\vm\oops\mixinOop.hpp"\
	

"$(INTDIR)\behavior_prims.obj" : $(SOURCE) $(DEP_CPP_BEHAV) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\system_prims.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\smi_prims.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\proxy_prims.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\process_prims.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\prim_impl.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\prim_def.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\prim.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\oop_prims.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\objArray_prims.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\mixin_prims.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\method_prims.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\double_prims.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\dll.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\debug_prims.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\dByteArray_prims.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\callBack_prims.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\byteArray_prims.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\block_prims.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\behavior_prims.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\smi_prims_asm.asm

!IF  "$(CFG)" == "Delta - Win32 Debug"

# Begin Custom Build
OutDir=.\debug
InputPath=..\vm\prims\smi_prims_asm.asm
InputName=smi_prims_asm

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

# Begin Custom Build
OutDir=.\fast
InputPath=..\vm\prims\smi_prims_asm.asm
InputName=smi_prims_asm

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

# Begin Custom Build
OutDir=.\product
InputPath=..\vm\prims\smi_prims_asm.asm
InputName=smi_prims_asm

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\recompiler\rframe.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\recompiler\rframe.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_RFRAM=\
	".\..\vm\recompiler\incls\_precompiled.incl"\
	".\..\vm\recompiler\incls\_rframe.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\rframe.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\rframe.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_RFRAM=\
	".\..\vm\recompiler\incls\_precompiled.incl"\
	".\..\vm\recompiler\incls\_rframe.cpp.incl"\
	

"$(INTDIR)\rframe.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_RFRAM=\
	".\incls\_precompiled.incl"\
	".\incls\_rframe.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\code\compiledPIC.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\interpreter\dispatchTable.hpp"\
	".\..\vm\interpreter\interpreter.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\recompiler\recompile.hpp"\
	".\..\vm\recompiler\rframe.hpp"\
	

"$(INTDIR)\rframe.obj" : $(SOURCE) $(DEP_CPP_RFRAM) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\recompiler\recompiler.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\recompiler\recompiler.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_RECOM=\
	".\..\vm\recompiler\incls\_precompiled.incl"\
	".\..\vm\recompiler\incls\_recompiler.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\recompiler.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\recompiler.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_RECOM=\
	".\..\vm\recompiler\incls\_precompiled.incl"\
	".\..\vm\recompiler\incls\_recompiler.cpp.incl"\
	

"$(INTDIR)\recompiler.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_RECOM=\
	".\incls\_precompiled.incl"\
	".\incls\_recompiler.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\interpreter\dispatchTable.hpp"\
	".\..\vm\interpreter\interpreter.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\recompiler\recompile.hpp"\
	".\..\vm\recompiler\recompiler.hpp"\
	".\..\vm\recompiler\rframe.hpp"\
	

"$(INTDIR)\recompiler.obj" : $(SOURCE) $(DEP_CPP_RECOM) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\recompiler\recompile.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\recompiler\recompile.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_RECOMP=\
	".\..\vm\recompiler\incls\_precompiled.incl"\
	".\..\vm\recompiler\incls\_recompile.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\recompile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\recompile.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_RECOMP=\
	".\..\vm\recompiler\incls\_precompiled.incl"\
	".\..\vm\recompiler\incls\_recompile.cpp.incl"\
	

"$(INTDIR)\recompile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_RECOMP=\
	".\incls\_precompiled.incl"\
	".\incls\_recompile.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\delta.hpp"\
	".\..\vm\code\inliningdb.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\interpreter\dispatchTable.hpp"\
	".\..\vm\interpreter\interpreter.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\recompiler\recompile.hpp"\
	".\..\vm\recompiler\recompiler.hpp"\
	".\..\vm\recompiler\rframe.hpp"\
	".\..\vm\runtime\timer.hpp"\
	".\..\vm\runtime\sweeper.hpp"\
	

"$(INTDIR)\recompile.obj" : $(SOURCE) $(DEP_CPP_RECOMP) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\vmOperations.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_VMOPE=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_vmOperations.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\vmOperations.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\vmOperations.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_VMOPE=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_vmOperations.cpp.incl"\
	

"$(INTDIR)\vmOperations.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_VMOPE=\
	".\incls\_precompiled.incl"\
	".\incls\_vmOperations.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\runtime\evaluator.hpp"\
	

"$(INTDIR)\vmOperations.obj" : $(SOURCE) $(DEP_CPP_VMOPE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\virtualspace.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_VIRTU=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_virtualspace.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\virtualspace.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\virtualspace.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_VIRTU=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_virtualspace.cpp.incl"\
	

"$(INTDIR)\virtualspace.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_VIRTU=\
	".\incls\_precompiled.incl"\
	".\incls\_virtualspace.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\virtualspace.obj" : $(SOURCE) $(DEP_CPP_VIRTU) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\vframe.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_VFRAM=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_vframe.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\vframe.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\vframe.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_VFRAM=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_vframe.cpp.incl"\
	

"$(INTDIR)\vframe.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_VFRAM=\
	".\incls\_precompiled.incl"\
	".\incls\_vframe.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\code\pcDesc.hpp"\
	".\..\vm\interpreter\prettyPrinter.hpp"\
	".\..\vm\runtime\tempDecoder.hpp"\
	".\..\vm\runtime\savedRegisters.hpp"\
	".\..\vm\runtime\stackChunk.hpp"\
	

"$(INTDIR)\vframe.obj" : $(SOURCE) $(DEP_CPP_VFRAM) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\uncommonBranch.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_UNCOM=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_uncommonBranch.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\uncommonBranch.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\uncommonBranch.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_UNCOM=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_uncommonBranch.cpp.incl"\
	

"$(INTDIR)\uncommonBranch.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_UNCOM=\
	".\incls\_precompiled.incl"\
	".\incls\_uncommonBranch.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\recompiler\recompile.hpp"\
	".\..\vm\recompiler\recompiler.hpp"\
	".\..\vm\runtime\stackChunk.hpp"\
	".\..\vm\code\stubRoutines.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\runtime\uncommonBranch.hpp"\
	

"$(INTDIR)\uncommonBranch.obj" : $(SOURCE) $(DEP_CPP_UNCOM) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\timer.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_TIMER=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_timer.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\timer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\timer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_TIMER=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_timer.cpp.incl"\
	

"$(INTDIR)\timer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_TIMER=\
	".\incls\_precompiled.incl"\
	".\incls\_timer.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\timer.hpp"\
	

"$(INTDIR)\timer.obj" : $(SOURCE) $(DEP_CPP_TIMER) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\tempDecoder.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_TEMPD=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_tempDecoder.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\tempDecoder.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\tempDecoder.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_TEMPD=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_tempDecoder.cpp.incl"\
	

"$(INTDIR)\tempDecoder.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_TEMPD=\
	".\incls\_precompiled.incl"\
	".\incls\_tempDecoder.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\tempDecoder.hpp"\
	

"$(INTDIR)\tempDecoder.obj" : $(SOURCE) $(DEP_CPP_TEMPD) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\task.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_TASK_=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_task.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\task.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\task.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_TASK_=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_task.cpp.incl"\
	

"$(INTDIR)\task.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_TASK_=\
	".\incls\_precompiled.incl"\
	".\incls\_task.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\task.hpp"\
	

"$(INTDIR)\task.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\stackChunk.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_STACK=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_stackChunk.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\stackChunk.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\stackChunk.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_STACK=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_stackChunk.cpp.incl"\
	

"$(INTDIR)\stackChunk.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_STACK=\
	".\incls\_precompiled.incl"\
	".\incls\_stackChunk.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\stackChunk.hpp"\
	

"$(INTDIR)\stackChunk.obj" : $(SOURCE) $(DEP_CPP_STACK) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\shell.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_SHELL=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_shell.cpp.incl"\
	
# ADD CPP /Yc"incls/_precompiled.incl"

BuildCmds= \
	$(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /I "." /I "incls" /I "..\vm\asm" /I\
 "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I "..\vm\interpreter" /I\
 "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I "..\vm\prims" /I\
 "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes" /I\
 "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I\
 "C:/Program Files/MSDEV/INCL" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D\
 "MICROSOFT" /D "ASSERT" /D "DEBUG" /D "DELTA_COMPILER" /FR"$(INTDIR)/"\
 /Fp"$(INTDIR)/strongtalk.pch" /Yc"incls/_precompiled.incl" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\shell.obj" : $(SOURCE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\shell.sbr" : $(SOURCE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\strongtalk.pch" : $(SOURCE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_SHELL=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_shell.cpp.incl"\
	
# ADD CPP /Yc"incls/_precompiled.incl"

BuildCmds= \
	$(CPP) /nologo /MLd /W3 /Gm /GX /Zi  /Oi /Ot  /Ob2 /Oy /Gs /GF /Gy /I "." /I "incls" /I "..\vm\asm" /I\
 "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I "..\vm\interpreter" /I\
 "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I "..\vm\prims" /I\
 "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes" /I\
 "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I\
 "C:/Program Files/MSDEV/INCL" /D "WIN32" /D "_CONSOLE" /D "MICROSOFT" /D\
 "DELTA_COMPILER" /D "DEBUG" /Fp"$(INTDIR)/strongtalk.pch"\
 /Yc"incls/_precompiled.incl" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\shell.obj" : $(SOURCE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\strongtalk.pch" : $(SOURCE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_SHELL=\
	".\incls\_precompiled.incl"\
	".\incls\_shell.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\arguments.hpp"\
	".\..\vm\runtime\bootstrap.hpp"\
	".\..\vm\runtime\init.hpp"\
	".\..\vm\code\inliningdb.hpp"\
	".\..\vm\runtime\shell.hpp"\
	".\..\vm\runtime\timer.hpp"\
	
# ADD BASE CPP /Yc"incls/_precompiled.incl"
# ADD CPP /Yc"incls/_precompiled.incl"

BuildCmds= \
	$(CPP) /nologo /MLd /W3 /GX  /Oi /Ot  /Ob2 /Oy /Gs /GF /Gy /I "." /I "incls" /I "..\vm\asm" /I\
 "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I "..\vm\interpreter" /I\
 "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I "..\vm\prims" /I\
 "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes" /I\
 "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I\
 "C:/Program Files/MSDEV/INCL" /D "PRODUCT" /D "WIN32" /D "_CONSOLE" /D\
 "MICROSOFT" /D "DELTA_COMPILER" /Fp"$(INTDIR)/strongtalk.pch"\
 /Yc"incls/_precompiled.incl" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\shell.obj" : $(SOURCE) $(DEP_CPP_SHELL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\strongtalk.pch" : $(SOURCE) $(DEP_CPP_SHELL) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\runtime.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_RUNTI=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_runtime.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\runtime.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\runtime.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_RUNTI=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_runtime.cpp.incl"\
	

"$(INTDIR)\runtime.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_RUNTI=\
	".\incls\_precompiled.incl"\
	".\incls\_runtime.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\runtime.obj" : $(SOURCE) $(DEP_CPP_RUNTI) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\process.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_PROCESS_=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_process.cpp.incl"\
	

BuildCmds= \
	$(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /I "." /I "incls" /I "..\vm\asm" /I\
 "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I "..\vm\interpreter" /I\
 "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I "..\vm\prims" /I\
 "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes" /I\
 "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I\
 "C:/Program Files/MSDEV/INCL" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D\
 "MICROSOFT" /D "ASSERT" /D "DEBUG" /D "DELTA_COMPILER" /FR"$(INTDIR)/"\
 /Fp"$(INTDIR)/strongtalk.pch" /Yu"incls/_precompiled.incl" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\process.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\process.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_PROCESS_=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_process.cpp.incl"\
	
# ADD CPP  /Oi /Ot  /Ob2 /Oy /Gs /GF /Gy /Op 
# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\process.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /Gm /GX /Zi  /Oi /Ot  /Ob2 /Oy /Gs /GF /Gy /Op  /I "." /I "incls" /I\
 "..\vm\asm" /I "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I\
 "..\vm\interpreter" /I "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I\
 "..\vm\prims" /I "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes"\
 /I "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I\
 "C:/Program Files/MSDEV/INCL" /D "WIN32" /D "_CONSOLE" /D "MICROSOFT" /D\
 "DELTA_COMPILER" /D "DEBUG" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_PROCESS_=\
	".\incls\_precompiled.incl"\
	".\incls\_process.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\runtime\delta.hpp"\
	".\..\vm\oops\processOop.hpp"\
	".\..\vm\runtime\shell.hpp"\
	".\..\vm\runtime\stackChunk.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\runtime\evaluator.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\runtime\timer.hpp"\
	".\..\vm\runtime\sweeper.hpp"\
	
# ADD BASE CPP  /Oi /Ot  /Ob2 /Oy /Gs /GF /Gy /Op 
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP  /Oi /Ot  /Ob2 /Oy /Gs /GF /Gy /Op 
# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\process.obj" : $(SOURCE) $(DEP_CPP_PROCESS_) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /GX  /Oi /Ot  /Ob2 /Oy /Gs /GF /Gy /Op  /I "." /I "incls" /I "..\vm\asm" /I\
 "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I "..\vm\interpreter" /I\
 "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I "..\vm\prims" /I\
 "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes" /I\
 "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I\
 "C:/Program Files/MSDEV/INCL" /D "PRODUCT" /D "WIN32" /D "_CONSOLE" /D\
 "MICROSOFT" /D "DELTA_COMPILER" /Fo"$(INTDIR)/" /c $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\os.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_OS_CP=\
	".\..\vm\runtime\incls\_os.cpp.incl"\
	
# SUBTRACT CPP /YX /Yc /Yu

BuildCmds= \
	$(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /I "." /I "incls" /I "..\vm\asm" /I\
 "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I "..\vm\interpreter" /I\
 "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I "..\vm\prims" /I\
 "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes" /I\
 "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I\
 "C:/Program Files/MSDEV/INCL" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D\
 "MICROSOFT" /D "ASSERT" /D "DEBUG" /D "DELTA_COMPILER" /FR"$(INTDIR)/"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\os.obj" : $(SOURCE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\os.sbr" : $(SOURCE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_OS_CP=\
	".\..\vm\runtime\incls\_os.cpp.incl"\
	
# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\os.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /Gm /GX /Zi  /Oi /Ot  /Ob2 /Oy /Gs /GF /Gy /I "." /I "incls" /I "..\vm\asm" /I\
 "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I "..\vm\interpreter" /I\
 "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I "..\vm\prims" /I\
 "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes" /I\
 "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I\
 "C:/Program Files/MSDEV/INCL" /D "WIN32" /D "_CONSOLE" /D "MICROSOFT" /D\
 "DELTA_COMPILER" /D "DEBUG" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_OS_CP=\
	".\incls\_os.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\memory\error.hpp"\
	
# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\os.obj" : $(SOURCE) $(DEP_CPP_OS_CP) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /GX  /Oi /Ot  /Ob2 /Oy /Gs /GF /Gy /I "." /I "incls" /I "..\vm\asm" /I\
 "..\vm\code" /I "..\vm\compiler" /I "..\vm\disasm" /I "..\vm\interpreter" /I\
 "..\vm\lookup" /I "..\vm\memory" /I "..\vm\oops" /I "..\vm\prims" /I\
 "..\vm\recompiler" /I "..\vm\runtime" /I "..\vm\topIncludes" /I\
 "..\vm\utilities" /I "C:/Program Files/MSDEV/INCLUDE" /I\
 "C:/Program Files/MSDEV/INCL" /D "PRODUCT" /D "WIN32" /D "_CONSOLE" /D\
 "MICROSOFT" /D "DELTA_COMPILER" /Fo"$(INTDIR)/" /c $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\init.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_INIT_=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_init.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\init.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\init.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_INIT_=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_init.cpp.incl"\
	

"$(INTDIR)\init.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_INIT_=\
	".\incls\_precompiled.incl"\
	".\incls\_init.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\init.hpp"\
	

"$(INTDIR)\init.obj" : $(SOURCE) $(DEP_CPP_INIT_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\frame.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_FRAME=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_frame.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\frame.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\frame.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_FRAME=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_frame.cpp.incl"\
	

"$(INTDIR)\frame.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_FRAME=\
	".\incls\_precompiled.incl"\
	".\incls\_frame.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\code\compiledPIC.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\interpreter\dispatchTable.hpp"\
	".\..\vm\interpreter\interpreter.hpp"\
	

"$(INTDIR)\frame.obj" : $(SOURCE) $(DEP_CPP_FRAME) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\fprofiler.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_FPROF=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_fprofiler.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\fprofiler.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\fprofiler.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_FPROF=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_fprofiler.cpp.incl"\
	

"$(INTDIR)\fprofiler.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_FPROF=\
	".\incls\_precompiled.incl"\
	".\incls\_fprofiler.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\timer.hpp"\
	".\..\vm\runtime\fprofiler.hpp"\
	".\..\vm\runtime\task.hpp"\
	".\..\vm\code\compiledPIC.hpp"\
	".\..\vm\code\stubRoutines.hpp"\
	

"$(INTDIR)\fprofiler.obj" : $(SOURCE) $(DEP_CPP_FPROF) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\evaluator.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_EVALU=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_evaluator.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\evaluator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\evaluator.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_EVALU=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_evaluator.cpp.incl"\
	

"$(INTDIR)\evaluator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_EVALU=\
	".\incls\_precompiled.incl"\
	".\incls\_evaluator.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\runtime\evaluator.hpp"\
	".\..\vm\utilities\objectIDTable.hpp"\
	".\..\vm\interpreter\prettyPrinter.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\interpreter\dispatchTable.hpp"\
	".\..\vm\runtime\delta.hpp"\
	

"$(INTDIR)\evaluator.obj" : $(SOURCE) $(DEP_CPP_EVALU) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\delta.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_DELTA=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_delta.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\delta.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\delta.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_DELTA=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_delta.cpp.incl"\
	

"$(INTDIR)\delta.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_DELTA=\
	".\incls\_precompiled.incl"\
	".\incls\_delta.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\delta.hpp"\
	

"$(INTDIR)\delta.obj" : $(SOURCE) $(DEP_CPP_DELTA) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\debug.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_DEBUG_C=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_debug.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\debug.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\debug.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_DEBUG_C=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_debug.cpp.incl"\
	

"$(INTDIR)\debug.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_DEBUG_C=\
	".\incls\_precompiled.incl"\
	".\incls\_debug.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\abort.hpp"\
	

"$(INTDIR)\debug.obj" : $(SOURCE) $(DEP_CPP_DEBUG_C) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\callBack.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_CALLBA=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_callBack.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\callBack.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\callBack.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_CALLBA=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_callBack.cpp.incl"\
	

"$(INTDIR)\callBack.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_CALLBA=\
	".\incls\_precompiled.incl"\
	".\incls\_callBack.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\runtime\callBack.hpp"\
	".\..\vm\runtime\delta.hpp"\
	".\..\vm\oops\proxyOop.hpp"\
	

"$(INTDIR)\callBack.obj" : $(SOURCE) $(DEP_CPP_CALLBA) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\bootstrap.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_BOOTS=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_bootstrap.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\bootstrap.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\bootstrap.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_BOOTS=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_bootstrap.cpp.incl"\
	

"$(INTDIR)\bootstrap.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_BOOTS=\
	".\incls\_precompiled.incl"\
	".\incls\_bootstrap.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\bootstrap.hpp"\
	".\..\vm\oops\smiKlass.hpp"\
	".\..\vm\oops\klassKlass.hpp"\
	".\..\vm\oops\byteArrayKlass.hpp"\
	".\..\vm\oops\dByteArrayOop.hpp"\
	".\..\vm\oops\dByteArrayKlass.hpp"\
	".\..\vm\oops\dValueArrayOop.hpp"\
	".\..\vm\oops\dValueArrayKlass.hpp"\
	".\..\vm\oops\objArrayKlass.hpp"\
	".\..\vm\memory\symbolTable.hpp"\
	".\..\vm\oops\symbolKlass.hpp"\
	".\..\vm\oops\doubleKlass.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\oops\associationKlass.hpp"\
	".\..\vm\oops\methodKlass.hpp"\
	".\..\vm\oops\proxyOop.hpp"\
	".\..\vm\oops\proxyKlass.hpp"\
	".\..\vm\oops\mixinOop.hpp"\
	".\..\vm\oops\mixinKlass.hpp"\
	".\..\vm\oops\weakArrayKlass.hpp"\
	".\..\vm\oops\processOop.hpp"\
	".\..\vm\oops\processKlass.hpp"\
	".\..\vm\oops\vframeOop.hpp"\
	".\..\vm\oops\vframeKlass.hpp"\
	

"$(INTDIR)\bootstrap.obj" : $(SOURCE) $(DEP_CPP_BOOTS) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\arguments.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_ARGUM=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_arguments.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\arguments.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\arguments.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_ARGUM=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_arguments.cpp.incl"\
	

"$(INTDIR)\arguments.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_ARGUM=\
	".\incls\_precompiled.incl"\
	".\incls\_arguments.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\arguments.hpp"\
	".\..\vm\runtime\disclaimer.hpp"\
	

"$(INTDIR)\arguments.obj" : $(SOURCE) $(DEP_CPP_ARGUM) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\abort.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_ABORT=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_abort.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\abort.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\abort.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_ABORT=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_abort.cpp.incl"\
	

"$(INTDIR)\abort.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_ABORT=\
	".\incls\_precompiled.incl"\
	".\incls\_abort.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\interpreter\dispatchTable.hpp"\
	

"$(INTDIR)\abort.obj" : $(SOURCE) $(DEP_CPP_ABORT) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\vmOperations.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\virtualspace.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\vframe.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\uncommonBranch.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\timer.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\tempDecoder.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\task.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\stackChunk.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\shell.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\runtime.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\process.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\os.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\init.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\frame.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\fprofiler.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\evaluator.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\delta.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\debug.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\callBack.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\bootstrap.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\arguments.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\abort.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\callBackStubs.asm

!IF  "$(CFG)" == "Delta - Win32 Debug"

# Begin Custom Build
OutDir=.\debug
InputPath=..\vm\runtime\callBackStubs.asm
InputName=callBackStubs

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

# Begin Custom Build
OutDir=.\fast
InputPath=..\vm\runtime\callBackStubs.asm
InputName=callBackStubs

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

# Begin Custom Build
OutDir=.\product
InputPath=..\vm\runtime\callBackStubs.asm
InputName=callBackStubs

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\topIncludes\types.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\topIncludes\top.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\topIncludes\tag.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\topIncludes\std_includes.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\topIncludes\macros.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\topIncludes\config.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\topIncludes\bits.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\topIncludes\bits.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_BITS_=\
	".\..\vm\topIncludes\incls\_precompiled.incl"\
	".\..\vm\topIncludes\incls\_bits.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\bits.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\bits.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_BITS_=\
	".\..\vm\topIncludes\incls\_precompiled.incl"\
	".\..\vm\topIncludes\incls\_bits.cpp.incl"\
	

"$(INTDIR)\bits.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_BITS_=\
	".\incls\_precompiled.incl"\
	".\incls\_bits.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\bits.obj" : $(SOURCE) $(DEP_CPP_BITS_) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\topIncludes\asserts.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\utilities\ostream.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\utilities\ostream.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_OSTRE=\
	".\..\vm\utilities\incls\_precompiled.incl"\
	".\..\vm\utilities\incls\_ostream.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\ostream.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\ostream.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_OSTRE=\
	".\..\vm\utilities\incls\_precompiled.incl"\
	".\..\vm\utilities\incls\_ostream.cpp.incl"\
	

"$(INTDIR)\ostream.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_OSTRE=\
	".\incls\_precompiled.incl"\
	".\incls\_ostream.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\ostream.obj" : $(SOURCE) $(DEP_CPP_OSTRE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\utilities\objectIDTable.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\utilities\objectIDTable.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_OBJEC=\
	".\..\vm\utilities\incls\_precompiled.incl"\
	".\..\vm\utilities\incls\_objectIDTable.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\objectIDTable.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\objectIDTable.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_OBJEC=\
	".\..\vm\utilities\incls\_precompiled.incl"\
	".\..\vm\utilities\incls\_objectIDTable.cpp.incl"\
	

"$(INTDIR)\objectIDTable.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_OBJEC=\
	".\incls\_precompiled.incl"\
	".\incls\_objectIDTable.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\utilities\objectIDTable.hpp"\
	

"$(INTDIR)\objectIDTable.obj" : $(SOURCE) $(DEP_CPP_OBJEC) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\utilities\lprintf.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\utilities\lprintf.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_LPRIN=\
	".\..\vm\utilities\incls\_precompiled.incl"\
	".\..\vm\utilities\incls\_lprintf.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\lprintf.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\lprintf.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_LPRIN=\
	".\..\vm\utilities\incls\_precompiled.incl"\
	".\..\vm\utilities\incls\_lprintf.cpp.incl"\
	

"$(INTDIR)\lprintf.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_LPRIN=\
	".\incls\_precompiled.incl"\
	".\incls\_lprintf.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\lprintf.obj" : $(SOURCE) $(DEP_CPP_LPRIN) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\utilities\growableArray.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\utilities\growableArray.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_GROWA=\
	".\..\vm\utilities\incls\_precompiled.incl"\
	".\..\vm\utilities\incls\_growableArray.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\growableArray.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\growableArray.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_GROWA=\
	".\..\vm\utilities\incls\_precompiled.incl"\
	".\..\vm\utilities\incls\_growableArray.cpp.incl"\
	

"$(INTDIR)\growableArray.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_GROWA=\
	".\incls\_precompiled.incl"\
	".\incls\_growableArray.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\growableArray.obj" : $(SOURCE) $(DEP_CPP_GROWA) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\utilities\eventLog.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\utilities\eventlog.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_EVENT=\
	".\..\vm\utilities\incls\_precompiled.incl"\
	".\..\vm\utilities\incls\_eventlog.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\eventlog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\eventlog.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_EVENT=\
	".\..\vm\utilities\incls\_precompiled.incl"\
	".\..\vm\utilities\incls\_eventlog.cpp.incl"\
	

"$(INTDIR)\eventlog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_EVENT=\
	".\incls\_precompiled.incl"\
	".\incls\_eventLog.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\eventlog.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\error.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_ERROR=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_error.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\error.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\error.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_ERROR=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_error.cpp.incl"\
	

"$(INTDIR)\error.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_ERROR=\
	".\incls\_precompiled.incl"\
	".\incls\_error.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\error.obj" : $(SOURCE) $(DEP_CPP_ERROR) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\asm\assembler.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_ASSEM=\
	".\..\vm\asm\incls\_precompiled.incl"\
	".\..\vm\asm\incls\_assembler.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\assembler.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\assembler.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_ASSEM=\
	".\..\vm\asm\incls\_precompiled.incl"\
	".\..\vm\asm\incls\_assembler.cpp.incl"\
	

"$(INTDIR)\assembler.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_ASSEM=\
	".\incls\_precompiled.incl"\
	".\incls\_assembler.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\code\stubRoutines.hpp"\
	

"$(INTDIR)\assembler.obj" : $(SOURCE) $(DEP_CPP_ASSEM) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\asm\assembler.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\interpreter\bytecodes.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\interpreter\bytecodes.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_BYTEC=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_bytecodes.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\bytecodes.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\bytecodes.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_BYTEC=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_bytecodes.cpp.incl"\
	

"$(INTDIR)\bytecodes.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_BYTEC=\
	".\incls\_precompiled.incl"\
	".\incls\_bytecodes.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\disclaimer.hpp"\
	

"$(INTDIR)\bytecodes.obj" : $(SOURCE) $(DEP_CPP_BYTEC) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\interpreter\interpretedIC.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\interpreter\interpretedIC.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_INTERPR=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_interpretedIC.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\interpretedIC.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\interpretedIC.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_INTERPR=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_interpretedIC.cpp.incl"\
	

"$(INTDIR)\interpretedIC.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_INTERPR=\
	".\incls\_precompiled.incl"\
	".\incls\_interpretedIC.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\delta.hpp"\
	".\..\vm\interpreter\dispatchTable.hpp"\
	".\..\vm\oops\objArrayKlass.hpp"\
	

"$(INTDIR)\interpretedIC.obj" : $(SOURCE) $(DEP_CPP_INTERPR) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\locations.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\locations.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_LOCATI=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_locations.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\locations.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\locations.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_LOCATI=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_locations.cpp.incl"\
	

"$(INTDIR)\locations.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_LOCATI=\
	".\incls\_precompiled.incl"\
	".\incls\_locations.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\code\locations.hpp"\
	

"$(INTDIR)\locations.obj" : $(SOURCE) $(DEP_CPP_LOCATI) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\pregMapping.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\pregMapping.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_PREGM=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_PRegMapping.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\pregMapping.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\pregMapping.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_PREGM=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_PRegMapping.cpp.incl"\
	

"$(INTDIR)\pregMapping.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_PREGM=\
	".\incls\_precompiled.incl"\
	".\incls\_pregMapping.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\disasm\disassembler.hpp"\
	".\..\vm\code\mapConformance.hpp"\
	".\..\vm\code\locations.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\compiler\basicBlock.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	".\..\vm\compiler\node.hpp"\
	".\..\vm\code\pregMapping.hpp"\
	

"$(INTDIR)\pregMapping.obj" : $(SOURCE) $(DEP_CPP_PREGM) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\stubRoutines.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\stubRoutines.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_STUBR=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_stubRoutines.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\stubRoutines.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\stubRoutines.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_STUBR=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_stubRoutines.cpp.incl"\
	

"$(INTDIR)\stubRoutines.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_STUBR=\
	".\incls\_precompiled.incl"\
	".\incls\_stubRoutines.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\code\compiledPIC.hpp"\
	".\..\vm\runtime\disclaimer.hpp"\
	".\..\vm\oops\proxyOop.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\recompiler\recompile.hpp"\
	".\..\vm\code\stubRoutines.hpp"\
	".\..\vm\runtime\savedRegisters.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\runtime\uncommonBranch.hpp"\
	

"$(INTDIR)\stubRoutines.obj" : $(SOURCE) $(DEP_CPP_STUBR) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\costModel.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\costModel.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_COSTM=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_costModel.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\costModel.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\costModel.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_COSTM=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_costModel.cpp.incl"\
	

"$(INTDIR)\costModel.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_COSTM=\
	".\incls\_precompiled.incl"\
	".\incls\_costModel.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\compiler\costModel.hpp"\
	

"$(INTDIR)\costModel.obj" : $(SOURCE) $(DEP_CPP_COSTM) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\utilities\longInt.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\utilities\longInt.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_LONGI=\
	".\..\vm\utilities\incls\_precompiled.incl"\
	".\..\vm\utilities\incls\_longInt.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\longInt.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\longInt.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_LONGI=\
	".\..\vm\utilities\incls\_precompiled.incl"\
	".\..\vm\utilities\incls\_longInt.cpp.incl"\
	

"$(INTDIR)\longInt.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_LONGI=\
	".\incls\_precompiled.incl"\
	".\incls\_longInt.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\longInt.obj" : $(SOURCE) $(DEP_CPP_LONGI) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\sweeper.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\sweeper.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_SWEEP=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_sweeper.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\sweeper.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\sweeper.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_SWEEP=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_sweeper.cpp.incl"\
	

"$(INTDIR)\sweeper.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_SWEEP=\
	".\incls\_precompiled.incl"\
	".\incls\_sweeper.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\runtime\timer.hpp"\
	".\..\vm\runtime\sweeper.hpp"\
	".\..\vm\oops\mixinOop.hpp"\
	".\..\vm\runtime\task.hpp"\
	

"$(INTDIR)\sweeper.obj" : $(SOURCE) $(DEP_CPP_SWEEP) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\memOopKlass.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\savedRegisters.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\savedRegisters.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_SAVED=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_savedRegisters.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\savedRegisters.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\savedRegisters.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_SAVED=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_savedRegisters.cpp.incl"\
	

"$(INTDIR)\savedRegisters.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_SAVED=\
	".\incls\_precompiled.incl"\
	".\incls\_savedRegisters.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\savedRegisters.hpp"\
	

"$(INTDIR)\savedRegisters.obj" : $(SOURCE) $(DEP_CPP_SAVED) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\loopOpt.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\loopOpt.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_LOOPO=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_loopOpt.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\loopOpt.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\loopOpt.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_LOOPO=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_loopOpt.cpp.incl"\
	

"$(INTDIR)\loopOpt.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_LOOPO=\
	".\incls\_precompiled.incl"\
	".\incls\_loopOpt.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\compiler\loopOpt.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\compiler\basicBlock.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	".\..\vm\compiler\node.hpp"\
	

"$(INTDIR)\loopOpt.obj" : $(SOURCE) $(DEP_CPP_LOOPO) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\interpreter\floats.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\interpreter\floats.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_FLOAT=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_floats.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\floats.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\floats.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_FLOAT=\
	".\..\vm\interpreter\incls\_precompiled.incl"\
	".\..\vm\interpreter\incls\_floats.cpp.incl"\
	

"$(INTDIR)\floats.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_FLOAT=\
	".\incls\_precompiled.incl"\
	".\incls\_floats.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\disclaimer.hpp"\
	

"$(INTDIR)\floats.obj" : $(SOURCE) $(DEP_CPP_FLOAT) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\nativeInstruction.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\nativeInstruction.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_NATIV=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_nativeInstruction.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\nativeInstruction.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\nativeInstruction.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_NATIV=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_nativeInstruction.cpp.incl"\
	

"$(INTDIR)\nativeInstruction.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_NATIV=\
	".\incls\_precompiled.incl"\
	".\incls\_nativeInstruction.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	

"$(INTDIR)\nativeInstruction.obj" : $(SOURCE) $(DEP_CPP_NATIV) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\inliningdb.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\inliningdb.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_INLINI=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_inliningdb.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\inliningdb.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\inliningdb.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_INLINI=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_inliningdb.cpp.incl"\
	

"$(INTDIR)\inliningdb.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_INLINI=\
	".\incls\_precompiled.incl"\
	".\incls\_inliningdb.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\code\inliningdb.hpp"\
	".\..\vm\code\pcDesc.hpp"\
	".\..\vm\compiler\rscope.hpp"\
	".\..\vm\runtime\timer.hpp"\
	

"$(INTDIR)\inliningdb.obj" : $(SOURCE) $(DEP_CPP_INLINI) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\intervalInfo.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\intervalInfo.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_INTERV=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_intervalInfo.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\intervalInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\intervalInfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_INTERV=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_intervalInfo.cpp.incl"\
	

"$(INTDIR)\intervalInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_INTERV=\
	".\incls\_precompiled.incl"\
	".\incls\_intervalInfo.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\compiler\intervalInfo.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\compiler\basicBlock.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	".\..\vm\compiler\node.hpp"\
	

"$(INTDIR)\intervalInfo.obj" : $(SOURCE) $(DEP_CPP_INTERV) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\dValueArrayOop.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\dValueArrayOop.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_DVALU=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_dValueArrayOop.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dValueArrayOop.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\dValueArrayOop.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_DVALU=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_dValueArrayOop.cpp.incl"\
	

"$(INTDIR)\dValueArrayOop.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_DVALU=\
	".\incls\_precompiled.incl"\
	".\incls\_dValueArrayOop.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\dValueArrayOop.hpp"\
	".\..\vm\oops\dValueArrayKlass.hpp"\
	".\..\vm\runtime\bootstrap.hpp"\
	

"$(INTDIR)\dValueArrayOop.obj" : $(SOURCE) $(DEP_CPP_DVALU) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\dValueArrayKlass.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\dValueArrayKlass.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_DVALUE=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_dValueArrayKlass.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dValueArrayKlass.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\dValueArrayKlass.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_DVALUE=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_dValueArrayKlass.cpp.incl"\
	

"$(INTDIR)\dValueArrayKlass.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_DVALUE=\
	".\incls\_precompiled.incl"\
	".\incls\_dValueArrayKlass.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\dValueArrayOop.hpp"\
	".\..\vm\oops\dValueArrayKlass.hpp"\
	

"$(INTDIR)\dValueArrayKlass.obj" : $(SOURCE) $(DEP_CPP_DVALUE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\dValueArray_prims.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\dValueArray_prims.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_DVALUEA=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_dValueArray_prims.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dValueArray_prims.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\dValueArray_prims.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_DVALUEA=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_dValueArray_prims.cpp.incl"\
	

"$(INTDIR)\dValueArray_prims.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_DVALUEA=\
	".\incls\_precompiled.incl"\
	".\incls\_dValueArray_prims.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\dValueArray_prims.hpp"\
	".\..\vm\prims\prim_impl.hpp"\
	".\..\vm\oops\dValueArrayOop.hpp"\
	

"$(INTDIR)\dValueArray_prims.obj" : $(SOURCE) $(DEP_CPP_DVALUEA) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\block_prims_asm.asm

!IF  "$(CFG)" == "Delta - Win32 Debug"

# Begin Custom Build
OutDir=.\debug
InputPath=..\vm\prims\block_prims_asm.asm
InputName=block_prims_asm

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

# Begin Custom Build
OutDir=.\fast
InputPath=..\vm\prims\block_prims_asm.asm
InputName=block_prims_asm

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

# Begin Custom Build
OutDir=.\product
InputPath=..\vm\prims\block_prims_asm.asm
InputName=block_prims_asm

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\double_prims_asm.asm

!IF  "$(CFG)" == "Delta - Win32 Debug"

# Begin Custom Build
OutDir=.\debug
InputPath=..\vm\prims\double_prims_asm.asm
InputName=double_prims_asm

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

# Begin Custom Build
OutDir=.\fast
InputPath=..\vm\prims\double_prims_asm.asm
InputName=double_prims_asm

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

# Begin Custom Build
OutDir=.\product
InputPath=..\vm\prims\double_prims_asm.asm
InputName=double_prims_asm

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\reflection.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\reflection.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_REFLE=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_reflection.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\reflection.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\reflection.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_REFLE=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_reflection.cpp.incl"\
	

"$(INTDIR)\reflection.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_REFLE=\
	".\incls\_precompiled.incl"\
	".\incls\_reflection.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\memory\reflection.hpp"\
	".\..\vm\oops\mixinOop.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\oops\proxyOop.hpp"\
	".\..\vm\runtime\delta.hpp"\
	".\..\vm\runtime\evaluator.hpp"\
	".\..\vm\oops\dByteArrayOop.hpp"\
	".\..\vm\oops\dValueArrayOop.hpp"\
	".\..\vm\oops\processOop.hpp"\
	".\..\vm\runtime\timer.hpp"\
	

"$(INTDIR)\reflection.obj" : $(SOURCE) $(DEP_CPP_REFLE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\compiler\oldCodeGenerator.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\compiler\oldCodeGenerator.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_OLDCO=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_oldCodeGenerator.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\oldCodeGenerator.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\oldCodeGenerator.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_OLDCO=\
	".\..\vm\compiler\incls\_precompiled.incl"\
	".\..\vm\compiler\incls\_oldCodeGenerator.cpp.incl"\
	

"$(INTDIR)\oldCodeGenerator.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_OLDCO=\
	".\incls\_precompiled.incl"\
	".\incls\_oldCodeGenerator.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\block_prims.hpp"\
	".\..\vm\oops\associationOop.hpp"\
	".\..\vm\interpreter\dispatchTable.hpp"\
	".\..\vm\interpreter\interpreter.hpp"\
	".\..\vm\compiler\loopOpt.hpp"\
	".\..\vm\compiler\defUse.hpp"\
	".\..\vm\compiler\basicBlock.hpp"\
	".\..\vm\compiler\compileTimeClosure.hpp"\
	".\..\vm\code\scopeDescRecorder.hpp"\
	".\..\vm\compiler\preg.hpp"\
	".\..\vm\compiler\expr.hpp"\
	".\..\vm\compiler\bitVector.hpp"\
	".\..\vm\code\registerMask.hpp"\
	".\..\vm\compiler\scope.hpp"\
	".\..\vm\compiler\node.hpp"\
	".\..\vm\compiler\oldCodeGenerator.hpp"\
	".\..\vm\oops\proxyOop.hpp"\
	".\..\vm\runtime\vmOperations.hpp"\
	".\..\vm\recompiler\recompile.hpp"\
	".\..\vm\recompiler\recompiler.hpp"\
	".\..\vm\compiler\regAlloc.hpp"\
	".\..\vm\code\stubRoutines.hpp"\
	".\..\vm\runtime\abort.hpp"\
	".\..\vm\runtime\uncommonBranch.hpp"\
	

"$(INTDIR)\oldCodeGenerator.obj" : $(SOURCE) $(DEP_CPP_OLDCO) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\systemAverage.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\systemAverage.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_SYSTEM=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_systemAverage.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\systemAverage.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\systemAverage.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_SYSTEM=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_systemAverage.cpp.incl"\
	

"$(INTDIR)\systemAverage.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_SYSTEM=\
	".\incls\_precompiled.incl"\
	".\incls\_systemAverage.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\code\compiledPIC.hpp"\
	".\..\vm\code\stubRoutines.hpp"\
	".\..\vm\runtime\systemAverage.hpp"\
	".\..\vm\runtime\task.hpp"\
	

"$(INTDIR)\systemAverage.obj" : $(SOURCE) $(DEP_CPP_SYSTEM) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\asm\codeBuffer.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\asm\codeBuffer.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_CODEB=\
	".\..\vm\asm\incls\_precompiled.incl"\
	".\..\vm\asm\incls\_codeBuffer.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\codeBuffer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\codeBuffer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_CODEB=\
	".\..\vm\asm\incls\_precompiled.incl"\
	".\..\vm\asm\incls\_codeBuffer.cpp.incl"\
	

"$(INTDIR)\codeBuffer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_CODEB=\
	".\incls\_precompiled.incl"\
	".\incls\_codeBuffer.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\disasm\disassembler.hpp"\
	

"$(INTDIR)\codeBuffer.obj" : $(SOURCE) $(DEP_CPP_CODEB) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\runtime\disclaimer.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\runtime\disclaimer.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_DISCL=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_disclaimer.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\disclaimer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\disclaimer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_DISCL=\
	".\..\vm\runtime\incls\_precompiled.incl"\
	".\..\vm\runtime\incls\_disclaimer.cpp.incl"\
	

"$(INTDIR)\disclaimer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_DISCL=\
	".\incls\_precompiled.incl"\
	".\incls\_disclaimer.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\runtime\disclaimer.hpp"\
	

"$(INTDIR)\disclaimer.obj" : $(SOURCE) $(DEP_CPP_DISCL) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\memory\handle.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\handle.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_HANDL=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_handle.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\handle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\handle.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_HANDL=\
	".\..\vm\memory\incls\_precompiled.incl"\
	".\..\vm\memory\incls\_handle.cpp.incl"\
	

"$(INTDIR)\handle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_HANDL=\
	".\incls\_precompiled.incl"\
	".\incls\_handle.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\memory\handle.hpp"\
	

"$(INTDIR)\handle.obj" : $(SOURCE) $(DEP_CPP_HANDL) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\vframe_prims.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\vframe_prims.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_VFRAME=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_vframe_prims.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\vframe_prims.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\vframe_prims.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_VFRAME=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_vframe_prims.cpp.incl"\
	

"$(INTDIR)\vframe_prims.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_VFRAME=\
	".\incls\_precompiled.incl"\
	".\incls\_vframe_prims.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\prims\prim_def.hpp"\
	".\..\vm\prims\vframe_prims.hpp"\
	".\..\vm\prims\prim_impl.hpp"\
	".\..\vm\oops\vframeOop.hpp"\
	".\..\vm\oops\processOop.hpp"\
	".\..\vm\interpreter\prettyPrinter.hpp"\
	

"$(INTDIR)\vframe_prims.obj" : $(SOURCE) $(DEP_CPP_VFRAME) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\vframeOop.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\vframeOop.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_VFRAMEO=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_vframeOop.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\vframeOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\vframeOop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_VFRAMEO=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_vframeOop.cpp.incl"\
	

"$(INTDIR)\vframeOop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_VFRAMEO=\
	".\incls\_precompiled.incl"\
	".\incls\_vframeOop.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\vframeOop.hpp"\
	".\..\vm\oops\processOop.hpp"\
	

"$(INTDIR)\vframeOop.obj" : $(SOURCE) $(DEP_CPP_VFRAMEO) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\oops\vframeKlass.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\oops\vframeKlass.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_VFRAMEK=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_vframeKlass.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\vframeKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\vframeKlass.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_VFRAMEK=\
	".\..\vm\oops\incls\_precompiled.incl"\
	".\..\vm\oops\incls\_vframeKlass.cpp.incl"\
	

"$(INTDIR)\vframeKlass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_VFRAMEK=\
	".\incls\_precompiled.incl"\
	".\incls\_vframeKlass.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\oops\vframeOop.hpp"\
	".\..\vm\oops\vframeKlass.hpp"\
	".\..\vm\oops\processOop.hpp"\
	

"$(INTDIR)\vframeKlass.obj" : $(SOURCE) $(DEP_CPP_VFRAMEK) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\memory\util_asm.asm

!IF  "$(CFG)" == "Delta - Win32 Debug"

# Begin Custom Build
OutDir=.\debug
InputPath=..\vm\memory\util_asm.asm
InputName=util_asm

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

# Begin Custom Build - Performing Custom Build Step
OutDir=.\fast
InputPath=..\vm\memory\util_asm.asm
InputName=util_asm

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

# Begin Custom Build - Performing Custom Build Step
OutDir=.\product
InputPath=..\vm\memory\util_asm.asm
InputName=util_asm

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\code\mapConformance.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\code\mapConformance.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_MAPCO=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_mapConformance.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\mapConformance.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\mapConformance.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_MAPCO=\
	".\..\vm\code\incls\_precompiled.incl"\
	".\..\vm\code\incls\_mapConformance.cpp.incl"\
	

"$(INTDIR)\mapConformance.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_MAPCO=\
	".\incls\_precompiled.incl"\
	".\incls\_mapConformance.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\code\mapConformance.hpp"\
	

"$(INTDIR)\mapConformance.obj" : $(SOURCE) $(DEP_CPP_MAPCO) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\behavior_prims_asm.asm

!IF  "$(CFG)" == "Delta - Win32 Debug"

# Begin Custom Build - Performing Custom Build Step
OutDir=.\debug
InputPath=..\vm\prims\behavior_prims_asm.asm
InputName=behavior_prims_asm

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

# Begin Custom Build
OutDir=.\fast
InputPath=..\vm\prims\behavior_prims_asm.asm
InputName=behavior_prims_asm

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

# Begin Custom Build
OutDir=.\product
InputPath=..\vm\prims\behavior_prims_asm.asm
InputName=behavior_prims_asm

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   ..\tools\tasm\bin\tasm32.exe /ml /t $(InputPath) $(OutDir)\$(InputName).obj

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\strongtalk\vm\prims\integerOps.hpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\prims\integerOps.cpp

!IF  "$(CFG)" == "Delta - Win32 Debug"

NODEP_CPP_INTEG=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_integerOps.cpp.incl"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\integerOps.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

"$(INTDIR)\integerOps.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

NODEP_CPP_INTEG=\
	".\..\vm\prims\incls\_precompiled.incl"\
	".\..\vm\prims\incls\_integerOps.cpp.incl"\
	

"$(INTDIR)\integerOps.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

DEP_CPP_INTEG=\
	".\incls\_precompiled.incl"\
	".\incls\_integerOps.cpp.incl"\
	".\..\vm\topIncludes\config.hpp"\
	".\..\vm\memory\error.hpp"\
	".\..\vm\oops\oopsHierarchy.hpp"\
	".\..\vm\topIncludes\std_includes.hpp"\
	".\..\vm\utilities\longInt.hpp"\
	".\..\vm\topIncludes\types.hpp"\
	".\..\vm\topIncludes\asserts.hpp"\
	".\..\vm\topIncludes\macros.hpp"\
	".\..\vm\topIncludes\bits.hpp"\
	".\..\vm\utilities\lprintf.hpp"\
	".\..\vm\memory\util.hpp"\
	".\..\vm\memory\allocation.hpp"\
	".\..\vm\utilities\ostream.hpp"\
	".\..\vm\topIncludes\tag.hpp"\
	".\..\vm\topIncludes\top.hpp"\
	".\..\vm\utilities\eventLog.hpp"\
	".\..\vm\runtime\debug.hpp"\
	".\..\vm\memory\iterator.hpp"\
	".\..\vm\utilities\growableArray.hpp"\
	".\..\vm\runtime\process.hpp"\
	".\..\vm\runtime\runtime.hpp"\
	".\..\vm\memory\rSet.hpp"\
	".\..\vm\memory\space.hpp"\
	".\..\vm\runtime\virtualspace.hpp"\
	".\..\vm\memory\generation.hpp"\
	".\..\vm\oops\oop.hpp"\
	".\..\vm\memory\generation.inline.hpp"\
	".\..\vm\memory\spaceSize.hpp"\
	".\..\vm\memory\universe.hpp"\
	".\..\vm\runtime\frame.hpp"\
	".\..\vm\oops\smiOop.hpp"\
	".\..\vm\oops\markOop.hpp"\
	".\..\vm\oops\memOop.hpp"\
	".\..\vm\interpreter\bytecodes.hpp"\
	".\..\vm\memory\universe.store.hpp"\
	".\..\vm\oops\klass.hpp"\
	".\..\vm\oops\byteArrayOop.hpp"\
	".\..\vm\oops\symbolOop.hpp"\
	".\..\vm\oops\methodOop.hpp"\
	".\..\vm\oops\blockOop.hpp"\
	".\..\vm\oops\klassOop.hpp"\
	".\..\vm\oops\memOopKlass.hpp"\
	".\..\vm\oops\blockKlass.hpp"\
	".\..\vm\oops\blockOop.inline.hpp"\
	".\..\vm\asm\location.hpp"\
	".\..\vm\runtime\vframe.hpp"\
	".\..\vm\runtime\os.hpp"\
	".\..\vm\memory\ageTable.hpp"\
	".\..\vm\code\nativeInstruction.hpp"\
	".\..\vm\code\compiledIC.hpp"\
	".\..\vm\code\iCache.hpp"\
	".\..\vm\code\relocInfo.hpp"\
	".\..\vm\asm\codeBuffer.hpp"\
	".\..\vm\asm\assembler.hpp"\
	".\..\vm\memory\universe.inline.hpp"\
	".\..\vm\memory\vmSymbols.hpp"\
	".\..\vm\oops\oop.inline.hpp"\
	".\..\vm\memory\markSweep.hpp"\
	".\..\vm\oops\memOop.inline.hpp"\
	".\..\vm\memory\oopFactory.hpp"\
	".\..\vm\code\jumpTable.hpp"\
	".\..\vm\code\ncode.hpp"\
	".\..\vm\lookup\key.hpp"\
	".\..\vm\asm\x86_mapping.hpp"\
	".\..\vm\code\nameDesc.hpp"\
	".\..\vm\code\scopeDesc.hpp"\
	".\..\vm\code\codeTable.hpp"\
	".\..\vm\code\zoneHeap.hpp"\
	".\..\vm\code\zone.hpp"\
	".\..\vm\code\nmethodScopes.hpp"\
	".\..\vm\code\nmethod.hpp"\
	".\..\vm\oops\objArrayOop.hpp"\
	".\..\vm\prims\dll.hpp"\
	".\..\vm\interpreter\codeIterator.hpp"\
	".\..\vm\interpreter\ic_iterator.hpp"\
	".\..\vm\lookup\lookupCache.hpp"\
	".\..\vm\interpreter\interpretedIC.hpp"\
	".\..\vm\oops\doubleOop.hpp"\
	".\..\vm\prims\prim.hpp"\
	".\..\vm\compiler\compUtils.hpp"\
	".\..\vm\interpreter\floats.hpp"\
	".\..\vm\interpreter\methodIterator.hpp"\
	".\..\vm\compiler\opcode.hpp"\
	".\..\vm\compiler\nodeBuilder.hpp"\
	".\..\vm\compiler\inliner.hpp"\
	".\..\vm\compiler\slist.hpp"\
	".\..\vm\compiler\compiler.hpp"\
	".\..\vm\prims\integerOps.hpp"\
	

"$(INTDIR)\integerOps.obj" : $(SOURCE) $(DEP_CPP_INTEG) "$(INTDIR)"\
 "$(INTDIR)\strongtalk.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\build\includeDB2

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\vm\build\includeDB

!IF  "$(CFG)" == "Delta - Win32 Debug"

!ELSEIF  "$(CFG)" == "Delta - Win32 Fast"

!ELSEIF  "$(CFG)" == "Delta - Win32 Product"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
