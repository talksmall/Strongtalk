static char* signature_0[] = { "Proxy", "Proxy", "Proxy"};
static char* errors_0[] = {NULL};
static primitive_desc primitive_0 = {
	"primitiveAPICallResult:ifFail:", 
	fntype(&proxyOopPrimitives::callOut0), 
	1507330, 
	signature_0, 
	errors_0
};

static char* signature_1[] = { "Proxy", "Proxy", "Proxy|SmallInteger", "Proxy"};
static char* errors_1[] = {NULL};
static primitive_desc primitive_1 = {
	"primitiveAPICallValue:result:ifFail:", 
	fntype(&proxyOopPrimitives::callOut1), 
	1507331, 
	signature_1, 
	errors_1
};

static char* signature_2[] = { "Proxy", "Proxy", "Proxy|SmallInteger", "Proxy|SmallInteger", "Proxy"};
static char* errors_2[] = {NULL};
static primitive_desc primitive_2 = {
	"primitiveAPICallValue:value:result:ifFail:", 
	fntype(&proxyOopPrimitives::callOut2), 
	1507332, 
	signature_2, 
	errors_2
};

static char* signature_3[] = { "Proxy", "Proxy", "Proxy|SmallInteger", "Proxy|SmallInteger", "Proxy|SmallInteger", "Proxy"};
static char* errors_3[] = {NULL};
static primitive_desc primitive_3 = {
	"primitiveAPICallValue:value:value:result:ifFail:", 
	fntype(&proxyOopPrimitives::callOut3), 
	1507333, 
	signature_3, 
	errors_3
};

static char* signature_4[] = { "Proxy", "Proxy", "Proxy|SmallInteger", "Proxy|SmallInteger", "Proxy|SmallInteger", "Proxy|SmallInteger", "Proxy"};
static char* errors_4[] = {NULL};
static primitive_desc primitive_4 = {
	"primitiveAPICallValue:value:value:value:result:ifFail:", 
	fntype(&proxyOopPrimitives::callOut4), 
	1507334, 
	signature_4, 
	errors_4
};

static char* signature_5[] = { "Proxy", "Proxy", "Proxy|SmallInteger", "Proxy|SmallInteger", "Proxy|SmallInteger", "Proxy|SmallInteger", "Proxy|SmallInteger", "Proxy"};
static char* errors_5[] = {NULL};
static primitive_desc primitive_5 = {
	"primitiveAPICallValue:value:value:value:value:result:ifFail:", 
	fntype(&proxyOopPrimitives::callOut5), 
	1507335, 
	signature_5, 
	errors_5
};

static char* signature_6[] = { "IndexedInstanceVariables", "Activation"};
static char* errors_6[] = {NULL};
static primitive_desc primitive_6 = {
	"primitiveActivationArgumentsIfFail:", 
	fntype(&vframeOopPrimitives::arguments), 
	1835009, 
	signature_6, 
	errors_6
};

static char* signature_7[] = { "SmallInteger", "Activation"};
static char* errors_7[] = {NULL};
static primitive_desc primitive_7 = {
	"primitiveActivationByteCodeIndexIfFail:", 
	fntype(&vframeOopPrimitives::byte_code_index), 
	1835009, 
	signature_7, 
	errors_7
};

static char* signature_8[] = { "IndexedInstanceVariables", "Activation"};
static char* errors_8[] = {NULL};
static primitive_desc primitive_8 = {
	"primitiveActivationExpressionStackIfFail:", 
	fntype(&vframeOopPrimitives::expression_stack), 
	1835009, 
	signature_8, 
	errors_8
};

static char* signature_9[] = { "SmallInteger", "Activation"};
static char* errors_9[] = {NULL};
static primitive_desc primitive_9 = {
	"primitiveActivationIndex", 
	fntype(&vframeOopPrimitives::index), 
	1572865, 
	signature_9, 
	errors_9
};

static char* signature_10[] = { "Boolean", "Activation"};
static char* errors_10[] = {NULL};
static primitive_desc primitive_10 = {
	"primitiveActivationIsSmalltalkActivationIfFail:", 
	fntype(&vframeOopPrimitives::is_smalltalk_activation), 
	1835009, 
	signature_10, 
	errors_10
};

static char* signature_11[] = { "SmallInteger", "Activation"};
static char* errors_11[] = {NULL};
static primitive_desc primitive_11 = {
	"primitiveActivationMethodIfFail:", 
	fntype(&vframeOopPrimitives::method), 
	1835009, 
	signature_11, 
	errors_11
};

static char* signature_12[] = { "IndexedByteInstanceVariables", "Activation"};
static char* errors_12[] = {NULL};
static primitive_desc primitive_12 = {
	"primitiveActivationPrettyPrintIfFail:", 
	fntype(&vframeOopPrimitives::pretty_print), 
	1835009, 
	signature_12, 
	errors_12
};

static char* signature_13[] = { "SmallInteger", "Activation"};
static char* errors_13[] = {NULL};
static primitive_desc primitive_13 = {
	"primitiveActivationProcess", 
	fntype(&vframeOopPrimitives::process), 
	1572865, 
	signature_13, 
	errors_13
};

static char* signature_14[] = { "SmallInteger", "Activation"};
static char* errors_14[] = {NULL};
static primitive_desc primitive_14 = {
	"primitiveActivationReceiverIfFail:", 
	fntype(&vframeOopPrimitives::receiver), 
	1835009, 
	signature_14, 
	errors_14
};

static char* signature_15[] = { "Object", "Activation"};
static char* errors_15[] = { "ProcessCannotContinue", "Dead", NULL};
static primitive_desc primitive_15 = {
	"primitiveActivationSingleStep:ifFail:", 
	fntype(&vframeOopPrimitives::single_step), 
	327681, 
	signature_15, 
	errors_15
};

static char* signature_16[] = { "Object", "Activation"};
static char* errors_16[] = { "ProcessCannotContinue", "Dead", NULL};
static primitive_desc primitive_16 = {
	"primitiveActivationStepNext:ifFail:", 
	fntype(&vframeOopPrimitives::step_next), 
	327681, 
	signature_16, 
	errors_16
};

static char* signature_17[] = { "Object", "Activation"};
static char* errors_17[] = { "ProcessCannotContinue", "Dead", NULL};
static primitive_desc primitive_17 = {
	"primitiveActivationStepReturn:ifFail:", 
	fntype(&vframeOopPrimitives::step_return), 
	327681, 
	signature_17, 
	errors_17
};

static char* signature_18[] = { "IndexedInstanceVariables", "Activation"};
static char* errors_18[] = {NULL};
static primitive_desc primitive_18 = {
	"primitiveActivationTemporariesIfFail:", 
	fntype(&vframeOopPrimitives::temporaries), 
	1835009, 
	signature_18, 
	errors_18
};

static char* signature_19[] = { "SmallInteger", "Activation"};
static char* errors_19[] = {NULL};
static primitive_desc primitive_19 = {
	"primitiveActivationTimeStamp", 
	fntype(&vframeOopPrimitives::time_stamp), 
	1572865, 
	signature_19, 
	errors_19
};

static char* signature_20[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_20[] = { "Overflow", NULL};
static primitive_desc primitive_20 = {
	"primitiveAdd:ifFail:", 
	fntype(&smiOopPrimitives_add), 
	6029826, 
	signature_20, 
	errors_20
};

static char* signature_21[] = { "IndexedInstanceVariables", "SmallInteger"};
static char* errors_21[] = {NULL};
static primitive_desc primitive_21 = {
	"primitiveAllObjectsLimit:ifFail:", 
	fntype(&systemPrimitives::all_objects), 
	327681, 
	signature_21, 
	errors_21
};

static char* signature_22[] = { "Object", "IndexedInstanceVariables"};
static char* errors_22[] = {NULL};
static primitive_desc primitive_22 = {
	"primitiveApplyChange:ifFail:", 
	fntype(&systemPrimitives::applyChange), 
	327681, 
	signature_22, 
	errors_22
};

static char* signature_23[] = { "Float", "SmallInteger"};
static char* errors_23[] = {NULL};
static primitive_desc primitive_23 = {
	"primitiveAsFloat", 
	fntype(&double_from_smi), 
	1573377, 
	signature_23, 
	errors_23
};

static char* signature_24[] = { "SmallInteger", "Object"};
static char* errors_24[] = {NULL};
static primitive_desc primitive_24 = {
	"primitiveAsObjectID", 
	fntype(&oopPrimitives::asObjectID), 
	1114113, 
	signature_24, 
	errors_24
};

static char* signature_25[] = { "Object", "SmallInteger"};
static char* errors_25[] = { "OutOfBounds", NULL};
static primitive_desc primitive_25 = {
	"primitiveAsObjectIfFail:", 
	fntype(&smiOopPrimitives::asObject), 
	1310721, 
	signature_25, 
	errors_25
};

static char* signature_26[] = { "Object", "Object", "Object"};
static char* errors_26[] = { "RecieverHasWrongType", NULL};
static primitive_desc primitive_26 = {
	"primitiveBecome:ifFail:", 
	fntype(&oopPrimitives::become), 
	1376258, 
	signature_26, 
	errors_26
};

static char* signature_27[] = { "GlobalAssociation", "Behavior", "SmallInteger"};
static char* errors_27[] = { "OutOfBounds", NULL};
static primitive_desc primitive_27 = {
	"primitiveBehavior:classVariableAt:ifFail:", 
	fntype(&behaviorPrimitives::classVariableAt), 
	327682, 
	signature_27, 
	errors_27
};

static char* signature_28[] = { "IndexedInstanceVariables", "Behavior"};
static char* errors_28[] = { "OutOfBounds", NULL};
static primitive_desc primitive_28 = {
	"primitiveBehavior:classVariablesIfFail:", 
	fntype(&behaviorPrimitives::classVariables), 
	327681, 
	signature_28, 
	errors_28
};

static char* signature_29[] = { "Boolean", "Behavior"};
static char* errors_29[] = {NULL};
static primitive_desc primitive_29 = {
	"primitiveBehaviorCanBeSubclassed:ifFail:", 
	fntype(&behaviorPrimitives::can_be_subclassed), 
	327681, 
	signature_29, 
	errors_29
};

static char* signature_30[] = { "Boolean", "Behavior"};
static char* errors_30[] = {NULL};
static primitive_desc primitive_30 = {
	"primitiveBehaviorCanHaveInstanceVariables:ifFail:", 
	fntype(&behaviorPrimitives::can_have_instance_variables), 
	327681, 
	signature_30, 
	errors_30
};

static char* signature_31[] = { "Symbol", "Behavior"};
static char* errors_31[] = {NULL};
static primitive_desc primitive_31 = {
	"primitiveBehaviorFormat:ifFail:", 
	fntype(&behaviorPrimitives::format), 
	327681, 
	signature_31, 
	errors_31
};

static char* signature_32[] = { "SmallInteger", "Behavior"};
static char* errors_32[] = {NULL};
static primitive_desc primitive_32 = {
	"primitiveBehaviorHeaderSizeOf:ifFail:", 
	fntype(&behaviorPrimitives::headerSize), 
	327681, 
	signature_32, 
	errors_32
};

static char* signature_33[] = { "Boolean", "Behavior", "Object"};
static char* errors_33[] = {NULL};
static primitive_desc primitive_33 = {
	"primitiveBehaviorIsClassOf:", 
	fntype(&behaviorPrimitives::is_class_of), 
	5308418, 
	signature_33, 
	errors_33
};

static char* signature_34[] = { "Boolean", "Behavior"};
static char* errors_34[] = {NULL};
static primitive_desc primitive_34 = {
	"primitiveBehaviorIsSpecializedClass:ifFail:", 
	fntype(&behaviorPrimitives::is_specialized_class), 
	327681, 
	signature_34, 
	errors_34
};

static char* signature_35[] = { "Mixin", "Behavior"};
static char* errors_35[] = {NULL};
static primitive_desc primitive_35 = {
	"primitiveBehaviorMixinOf:ifFail:", 
	fntype(&behaviorPrimitives::mixinOf), 
	327681, 
	signature_35, 
	errors_35
};

static char* signature_36[] = { "SmallInteger", "Behavior"};
static char* errors_36[] = {NULL};
static primitive_desc primitive_36 = {
	"primitiveBehaviorNonIndexableSizeOf:ifFail:", 
	fntype(&behaviorPrimitives::nonIndexableSize), 
	327681, 
	signature_36, 
	errors_36
};

static char* signature_37[] = { "Symbol", "Behavior"};
static char* errors_37[] = {NULL};
static primitive_desc primitive_37 = {
	"primitiveBehaviorVMType:ifFail:", 
	fntype(&behaviorPrimitives::vm_type), 
	327681, 
	signature_37, 
	errors_37
};

static char* signature_38[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_38[] = {NULL};
static primitive_desc primitive_38 = {
	"primitiveBitAnd:ifFail:", 
	fntype(&smiOopPrimitives::bitAnd), 
	6029826, 
	signature_38, 
	errors_38
};

static char* signature_39[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_39[] = {NULL};
static primitive_desc primitive_39 = {
	"primitiveBitOr:ifFail:", 
	fntype(&smiOopPrimitives::bitOr), 
	6029826, 
	signature_39, 
	errors_39
};

static char* signature_40[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_40[] = {NULL};
static primitive_desc primitive_40 = {
	"primitiveBitShift:ifFail:", 
	fntype(&smiOopPrimitives::bitShift), 
	6029826, 
	signature_40, 
	errors_40
};

static char* signature_41[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_41[] = {NULL};
static primitive_desc primitive_41 = {
	"primitiveBitXor:ifFail:", 
	fntype(&smiOopPrimitives::bitXor), 
	6029826, 
	signature_41, 
	errors_41
};

static char* signature_42[] = { "Boolean", "Block"};
static char* errors_42[] = {NULL};
static primitive_desc primitive_42 = {
	"primitiveBlockIsOptimized", 
	fntype(&block_is_optimized), 
	1114113, 
	signature_42, 
	errors_42
};

static char* signature_43[] = { "Method", "Block"};
static char* errors_43[] = {NULL};
static primitive_desc primitive_43 = {
	"primitiveBlockMethod", 
	fntype(&block_method), 
	1114113, 
	signature_43, 
	errors_43
};

static char* signature_44[] = { "Boolean", "Symbol"};
static char* errors_44[] = { "NotFound", NULL};
static primitive_desc primitive_44 = {
	"primitiveBooleanFlagAt:ifFail:", 
	fntype(&debugPrimitives::boolAt), 
	327681, 
	signature_44, 
	errors_44
};

static char* signature_45[] = { "Boolean", "Symbol", "Boolean"};
static char* errors_45[] = { "NotFound", NULL};
static primitive_desc primitive_45 = {
	"primitiveBooleanFlagAt:put:ifFail:", 
	fntype(&debugPrimitives::boolAtPut), 
	327682, 
	signature_45, 
	errors_45
};

static char* signature_46[] = { "Object"};
static char* errors_46[] = {NULL};
static primitive_desc primitive_46 = {
	"primitiveBreakpoint", 
	fntype(&systemPrimitives::breakpoint), 
	65536, 
	signature_46, 
	errors_46
};

static char* signature_47[] = { "Object", "Object", "Proxy"};
static char* errors_47[] = {NULL};
static primitive_desc primitive_47 = {
	"primitiveCallBackInvokeC2:ifFail:", 
	fntype(&callBackPrimitives::invokeC), 
	1376258, 
	signature_47, 
	errors_47
};

static char* signature_48[] = { "Object", "Object", "Proxy"};
static char* errors_48[] = {NULL};
static primitive_desc primitive_48 = {
	"primitiveCallBackInvokePascal2:ifFail:", 
	fntype(&callBackPrimitives::invokePascal), 
	1376258, 
	signature_48, 
	errors_48
};

static char* signature_49[] = { "Object", "Object", "Symbol"};
static char* errors_49[] = {NULL};
static primitive_desc primitive_49 = {
	"primitiveCallBackReceiver:selector:ifFail:", 
	fntype(&callBackPrimitives::initialize), 
	327682, 
	signature_49, 
	errors_49
};

static char* signature_50[] = { "Object", "SmallInteger", "Proxy"};
static char* errors_50[] = {NULL};
static primitive_desc primitive_50 = {
	"primitiveCallBackRegisterCCall:result:ifFail:", 
	fntype(&callBackPrimitives::registerCCall), 
	327682, 
	signature_50, 
	errors_50
};

static char* signature_51[] = { "Object", "SmallInteger", "SmallInteger", "Proxy"};
static char* errors_51[] = {NULL};
static primitive_desc primitive_51 = {
	"primitiveCallBackRegisterPascalCall:numberOfArguments:result:ifFail:", 
	fntype(&callBackPrimitives::registerPascalCall), 
	327683, 
	signature_51, 
	errors_51
};

static char* signature_52[] = { "Object", "Object", "Proxy"};
static char* errors_52[] = {NULL};
static primitive_desc primitive_52 = {
	"primitiveCallBackUnregister:ifFail:", 
	fntype(&callBackPrimitives::unregister), 
	1376258, 
	signature_52, 
	errors_52
};

static char* signature_53[] = { "Proxy", "SmallInteger"};
static char* errors_53[] = { "OutOfBounds", NULL};
static primitive_desc primitive_53 = {
	"primitiveCharacterFor:ifFail:", 
	fntype(&systemPrimitives::characterFor), 
	327681, 
	signature_53, 
	errors_53
};

static char* signature_54[] = { "Self class", "Object"};
static char* errors_54[] = {NULL};
static primitive_desc primitive_54 = {
	"primitiveClass", 
	fntype(&oopPrimitives::klass), 
	5767169, 
	signature_54, 
	errors_54
};

static char* signature_55[] = { "Behavior", "Object"};
static char* errors_55[] = {NULL};
static primitive_desc primitive_55 = {
	"primitiveClassOf:", 
	fntype(&oopPrimitives::klass_of), 
	4718593, 
	signature_55, 
	errors_55
};

static char* signature_56[] = { "Object"};
static char* errors_56[] = {NULL};
static primitive_desc primitive_56 = {
	"primitiveClearInlineCaches", 
	fntype(&debugPrimitives::clearInlineCaches), 
	65536, 
	signature_56, 
	errors_56
};

static char* signature_57[] = { "Object"};
static char* errors_57[] = {NULL};
static primitive_desc primitive_57 = {
	"primitiveClearInvocationCounters", 
	fntype(&debugPrimitives::clearInvocationCounters), 
	65536, 
	signature_57, 
	errors_57
};

static char* signature_58[] = { "Object"};
static char* errors_58[] = {NULL};
static primitive_desc primitive_58 = {
	"primitiveClearLookupCache", 
	fntype(&debugPrimitives::clearLookupCache), 
	65536, 
	signature_58, 
	errors_58
};

static char* signature_59[] = { "Object"};
static char* errors_59[] = {NULL};
static primitive_desc primitive_59 = {
	"primitiveClearLookupCacheStatistics", 
	fntype(&debugPrimitives::clearLookupCacheStatistics), 
	65536, 
	signature_59, 
	errors_59
};

static char* signature_60[] = { "Object"};
static char* errors_60[] = {NULL};
static primitive_desc primitive_60 = {
	"primitiveClearNMethodCounters", 
	fntype(&debugPrimitives::clearNMethodCounters), 
	65536, 
	signature_60, 
	errors_60
};

static char* signature_61[] = { "Object"};
static char* errors_61[] = {NULL};
static primitive_desc primitive_61 = {
	"primitiveClearPrimitiveCounters", 
	fntype(&debugPrimitives::clearPrimitiveCounters), 
	65536, 
	signature_61, 
	errors_61
};

static char* signature_62[] = { "Array[String]"};
static char* errors_62[] = {NULL};
static primitive_desc primitive_62 = {
	"primitiveCommandLineArgs", 
	fntype(&systemPrimitives::command_line_args), 
	65536, 
	signature_62, 
	errors_62
};

static char* signature_63[] = { "Block"};
static char* errors_63[] = {NULL};
static primitive_desc primitive_63 = {
	"primitiveCompiledBlockAllocate0", 
	fntype(&allocateBlock0), 
	6359040, 
	signature_63, 
	errors_63
};

static char* signature_64[] = { "Block"};
static char* errors_64[] = {NULL};
static primitive_desc primitive_64 = {
	"primitiveCompiledBlockAllocate1", 
	fntype(&allocateBlock1), 
	6359040, 
	signature_64, 
	errors_64
};

static char* signature_65[] = { "Block"};
static char* errors_65[] = {NULL};
static primitive_desc primitive_65 = {
	"primitiveCompiledBlockAllocate2", 
	fntype(&allocateBlock2), 
	6359040, 
	signature_65, 
	errors_65
};

static char* signature_66[] = { "Block"};
static char* errors_66[] = {NULL};
static primitive_desc primitive_66 = {
	"primitiveCompiledBlockAllocate3", 
	fntype(&allocateBlock3), 
	6359040, 
	signature_66, 
	errors_66
};

static char* signature_67[] = { "Block"};
static char* errors_67[] = {NULL};
static primitive_desc primitive_67 = {
	"primitiveCompiledBlockAllocate4", 
	fntype(&allocateBlock4), 
	6359040, 
	signature_67, 
	errors_67
};

static char* signature_68[] = { "Block"};
static char* errors_68[] = {NULL};
static primitive_desc primitive_68 = {
	"primitiveCompiledBlockAllocate5", 
	fntype(&allocateBlock5), 
	6359040, 
	signature_68, 
	errors_68
};

static char* signature_69[] = { "Block"};
static char* errors_69[] = {NULL};
static primitive_desc primitive_69 = {
	"primitiveCompiledBlockAllocate6", 
	fntype(&allocateBlock6), 
	6359040, 
	signature_69, 
	errors_69
};

static char* signature_70[] = { "Block"};
static char* errors_70[] = {NULL};
static primitive_desc primitive_70 = {
	"primitiveCompiledBlockAllocate7", 
	fntype(&allocateBlock7), 
	6359040, 
	signature_70, 
	errors_70
};

static char* signature_71[] = { "Block"};
static char* errors_71[] = {NULL};
static primitive_desc primitive_71 = {
	"primitiveCompiledBlockAllocate8", 
	fntype(&allocateBlock8), 
	6359040, 
	signature_71, 
	errors_71
};

static char* signature_72[] = { "Block"};
static char* errors_72[] = {NULL};
static primitive_desc primitive_72 = {
	"primitiveCompiledBlockAllocate9", 
	fntype(&allocateBlock9), 
	6359040, 
	signature_72, 
	errors_72
};

static char* signature_73[] = { "Block", "SmallInteger"};
static char* errors_73[] = {NULL};
static primitive_desc primitive_73 = {
	"primitiveCompiledBlockAllocate:", 
	fntype(&allocateBlock), 
	2164737, 
	signature_73, 
	errors_73
};

static char* signature_74[] = { "Object"};
static char* errors_74[] = {NULL};
static primitive_desc primitive_74 = {
	"primitiveCompiledContextAllocate0", 
	fntype(&allocateContext0), 
	6359040, 
	signature_74, 
	errors_74
};

static char* signature_75[] = { "Object"};
static char* errors_75[] = {NULL};
static primitive_desc primitive_75 = {
	"primitiveCompiledContextAllocate1", 
	fntype(&allocateContext1), 
	6359040, 
	signature_75, 
	errors_75
};

static char* signature_76[] = { "Object"};
static char* errors_76[] = {NULL};
static primitive_desc primitive_76 = {
	"primitiveCompiledContextAllocate2", 
	fntype(&allocateContext2), 
	6359040, 
	signature_76, 
	errors_76
};

static char* signature_77[] = { "Object", "SmallInteger"};
static char* errors_77[] = {NULL};
static primitive_desc primitive_77 = {
	"primitiveCompiledContextAllocate:", 
	fntype(&allocateContext), 
	6359041, 
	signature_77, 
	errors_77
};

static char* signature_78[] = { "Method", "Object", "SmallInteger", "SmallInteger", "Array", "ByteArray", "Array"};
static char* errors_78[] = {NULL};
static primitive_desc primitive_78 = {
	"primitiveConstructMethod:flags:nofArgs:debugInfo:bytes:oops:ifFail:", 
	fntype(&methodOopPrimitives::constructMethod), 
	327686, 
	signature_78, 
	errors_78
};

static char* signature_79[] = { "Object", "Object"};
static char* errors_79[] = { "NotOops", NULL};
static primitive_desc primitive_79 = {
	"primitiveCopyTenuredIfFail:", 
	fntype(&oopPrimitives::copy_tenured), 
	1376257, 
	signature_79, 
	errors_79
};

static char* signature_80[] = { "GlobalAssociation", "Mixin", "Symbol", "Boolean", "Behavior", "Symbol"};
static char* errors_80[] = { "WrongFormat", NULL};
static primitive_desc primitive_80 = {
	"primitiveCreateInvocationOf:named:isPrimaryInvocation:superclass:format:ifFail:", 
	fntype(&systemPrimitives::createNamedInvocation), 
	327685, 
	signature_80, 
	errors_80
};

static char* signature_81[] = { "GlobalAssociation", "Mixin", "Behavior", "Symbol"};
static char* errors_81[] = { "WrongFormat", NULL};
static primitive_desc primitive_81 = {
	"primitiveCreateInvocationOf:superclass:format:ifFail:", 
	fntype(&systemPrimitives::createInvocation), 
	327683, 
	signature_81, 
	errors_81
};

static char* signature_82[] = { "SmallInteger"};
static char* errors_82[] = {NULL};
static primitive_desc primitive_82 = {
	"primitiveCurrentThreadId", 
	fntype(&systemPrimitives::current_thread_id), 
	65536, 
	signature_82, 
	errors_82
};

static char* signature_83[] = { "Proxy", "Symbol", "Proxy"};
static char* errors_83[] = {NULL};
static primitive_desc primitive_83 = {
	"primitiveDLLLoad:result:ifFail:", 
	fntype(&systemPrimitives::dll_load), 
	327682, 
	signature_83, 
	errors_83
};

static char* signature_84[] = { "Proxy", "Symbol", "Proxy", "Proxy"};
static char* errors_84[] = {NULL};
static primitive_desc primitive_84 = {
	"primitiveDLLLookup:in:result:ifFail:", 
	fntype(&systemPrimitives::dll_lookup), 
	327683, 
	signature_84, 
	errors_84
};

static char* signature_85[] = { "Object", "Object", "Symbol"};
static char* errors_85[] = {NULL};
static primitive_desc primitive_85 = {
	"primitiveDLLSetupLookup:selector:ifFail:", 
	fntype(&systemPrimitives::dll_setup), 
	327682, 
	signature_85, 
	errors_85
};

static char* signature_86[] = { "Object", "Proxy"};
static char* errors_86[] = {NULL};
static primitive_desc primitive_86 = {
	"primitiveDLLUnload:ifFail:", 
	fntype(&systemPrimitives::dll_unload), 
	327681, 
	signature_86, 
	errors_86
};

static char* signature_87[] = { "Object"};
static char* errors_87[] = {NULL};
static primitive_desc primitive_87 = {
	"primitiveDecodeAllMethods", 
	fntype(&debugPrimitives::decodeAllMethods), 
	65536, 
	signature_87, 
	errors_87
};

static char* signature_88[] = { "Object", "Object", "Symbol"};
static char* errors_88[] = { "NotFound", NULL};
static primitive_desc primitive_88 = {
	"primitiveDecodeMethod:ifFail:", 
	fntype(&debugPrimitives::decodeMethod), 
	1376258, 
	signature_88, 
	errors_88
};

static char* signature_89[] = { "Proxy", "Proxy"};
static char* errors_89[] = {NULL};
static primitive_desc primitive_89 = {
	"primitiveDefWindowProc:ifFail:", 
	fntype(&systemPrimitives::defWindowProc), 
	327681, 
	signature_89, 
	errors_89
};

static char* signature_90[] = { "Object"};
static char* errors_90[] = {NULL};
static primitive_desc primitive_90 = {
	"primitiveDeoptimizeStacks", 
	fntype(&debugPrimitives::deoptimizeStacks), 
	196608, 
	signature_90, 
	errors_90
};

static char* signature_91[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_91[] = { "Overflow", "DivisionByZero", NULL};
static primitive_desc primitive_91 = {
	"primitiveDiv:ifFail:", 
	fntype(&smiOopPrimitives_div), 
	6029826, 
	signature_91, 
	errors_91
};

static char* signature_92[] = { "Float"};
static char* errors_92[] = {NULL};
static primitive_desc primitive_92 = {
	"primitiveElapsedTime", 
	fntype(&systemPrimitives::elapsedTime), 
	65536, 
	signature_92, 
	errors_92
};

static char* signature_93[] = { "Boolean", "Object", "Object"};
static char* errors_93[] = {NULL};
static primitive_desc primitive_93 = {
	"primitiveEqual:", 
	fntype(&oopPrimitives::equal), 
	1572866, 
	signature_93, 
	errors_93
};

static char* signature_94[] = { "Object", "SmallInteger"};
static char* errors_94[] = {NULL};
static primitive_desc primitive_94 = {
	"primitiveExpandMemory:", 
	fntype(&systemPrimitives::expandMemory), 
	65537, 
	signature_94, 
	errors_94
};

static char* signature_95[] = { "SmallInteger"};
static char* errors_95[] = {NULL};
static primitive_desc primitive_95 = {
	"primitiveExpansions", 
	fntype(&systemPrimitives::expansions), 
	65536, 
	signature_95, 
	errors_95
};

static char* signature_96[] = { "Process|nil"};
static char* errors_96[] = {NULL};
static primitive_desc primitive_96 = {
	"primitiveFlatProfilerDisengage", 
	fntype(&systemPrimitives::flat_profiler_disengage), 
	65536, 
	signature_96, 
	errors_96
};

static char* signature_97[] = { "Process", "Process"};
static char* errors_97[] = {NULL};
static primitive_desc primitive_97 = {
	"primitiveFlatProfilerEngage:ifFail:", 
	fntype(&systemPrimitives::flat_profiler_engage), 
	327681, 
	signature_97, 
	errors_97
};

static char* signature_98[] = { "Object"};
static char* errors_98[] = {NULL};
static primitive_desc primitive_98 = {
	"primitiveFlatProfilerPrint", 
	fntype(&systemPrimitives::flat_profiler_print), 
	65536, 
	signature_98, 
	errors_98
};

static char* signature_99[] = { "Process|nil"};
static char* errors_99[] = {NULL};
static primitive_desc primitive_99 = {
	"primitiveFlatProfilerProcess", 
	fntype(&systemPrimitives::flat_profiler_process), 
	65536, 
	signature_99, 
	errors_99
};

static char* signature_100[] = { "Object"};
static char* errors_100[] = {NULL};
static primitive_desc primitive_100 = {
	"primitiveFlatProfilerReset", 
	fntype(&systemPrimitives::flat_profiler_reset), 
	65536, 
	signature_100, 
	errors_100
};

static char* signature_101[] = { "Float", "Float", "Float"};
static char* errors_101[] = {NULL};
static primitive_desc primitive_101 = {
	"primitiveFloatAdd:ifFail:", 
	fntype(&double_add), 
	6030338, 
	signature_101, 
	errors_101
};

static char* signature_102[] = { "Float", "Float"};
static char* errors_102[] = {NULL};
static primitive_desc primitive_102 = {
	"primitiveFloatArcCosineIfFail:", 
	fntype(&doubleOopPrimitives::arcCosine), 
	1836033, 
	signature_102, 
	errors_102
};

static char* signature_103[] = { "Float", "Float"};
static char* errors_103[] = {NULL};
static primitive_desc primitive_103 = {
	"primitiveFloatArcSineIfFail:", 
	fntype(&doubleOopPrimitives::arcSine), 
	1836033, 
	signature_103, 
	errors_103
};

static char* signature_104[] = { "Float", "Float"};
static char* errors_104[] = {NULL};
static primitive_desc primitive_104 = {
	"primitiveFloatArcTangentIfFail:", 
	fntype(&doubleOopPrimitives::arcTangent), 
	1836033, 
	signature_104, 
	errors_104
};

static char* signature_105[] = { "SmallInteger", "Float"};
static char* errors_105[] = { "SmallIntegerConversionFailed", NULL};
static primitive_desc primitive_105 = {
	"primitiveFloatAsSmallIntegerIfFail:", 
	fntype(&doubleOopPrimitives::asSmallInteger), 
	1836033, 
	signature_105, 
	errors_105
};

static char* signature_106[] = { "Float", "Float"};
static char* errors_106[] = {NULL};
static primitive_desc primitive_106 = {
	"primitiveFloatCeiling", 
	fntype(&doubleOopPrimitives::ceiling), 
	1573889, 
	signature_106, 
	errors_106
};

static char* signature_107[] = { "Float", "Float"};
static char* errors_107[] = {NULL};
static primitive_desc primitive_107 = {
	"primitiveFloatCosine", 
	fntype(&doubleOopPrimitives::cosine), 
	1573889, 
	signature_107, 
	errors_107
};

static char* signature_108[] = { "Float", "Float", "Float"};
static char* errors_108[] = {NULL};
static primitive_desc primitive_108 = {
	"primitiveFloatDivide:ifFail:", 
	fntype(&double_divide), 
	6030338, 
	signature_108, 
	errors_108
};

static char* signature_109[] = { "Boolean", "Float", "Float"};
static char* errors_109[] = {NULL};
static primitive_desc primitive_109 = {
	"primitiveFloatEqual:ifFail:", 
	fntype(&doubleOopPrimitives::equal), 
	6030082, 
	signature_109, 
	errors_109
};

static char* signature_110[] = { "Float", "Float"};
static char* errors_110[] = {NULL};
static primitive_desc primitive_110 = {
	"primitiveFloatExp", 
	fntype(&doubleOopPrimitives::exp), 
	1573889, 
	signature_110, 
	errors_110
};

static char* signature_111[] = { "SmallInteger", "Float"};
static char* errors_111[] = {NULL};
static primitive_desc primitive_111 = {
	"primitiveFloatExponent", 
	fntype(&doubleOopPrimitives::exponent), 
	1573889, 
	signature_111, 
	errors_111
};

static char* signature_112[] = { "Float", "Float"};
static char* errors_112[] = {NULL};
static primitive_desc primitive_112 = {
	"primitiveFloatFloor", 
	fntype(&doubleOopPrimitives::floor), 
	1573889, 
	signature_112, 
	errors_112
};

static char* signature_113[] = { "Boolean", "Float", "Float"};
static char* errors_113[] = {NULL};
static primitive_desc primitive_113 = {
	"primitiveFloatGreaterThan:ifFail:", 
	fntype(&doubleOopPrimitives::greaterThan), 
	6030082, 
	signature_113, 
	errors_113
};

static char* signature_114[] = { "Boolean", "Float", "Float"};
static char* errors_114[] = {NULL};
static primitive_desc primitive_114 = {
	"primitiveFloatGreaterThanOrEqual:ifFail:", 
	fntype(&doubleOopPrimitives::greaterThanOrEqual), 
	6030082, 
	signature_114, 
	errors_114
};

static char* signature_115[] = { "Float", "Float"};
static char* errors_115[] = {NULL};
static primitive_desc primitive_115 = {
	"primitiveFloatHyperbolicCosineIfFail:", 
	fntype(&doubleOopPrimitives::hyperbolicCosine), 
	1836033, 
	signature_115, 
	errors_115
};

static char* signature_116[] = { "Float", "Float"};
static char* errors_116[] = {NULL};
static primitive_desc primitive_116 = {
	"primitiveFloatHyperbolicSineIfFail:", 
	fntype(&doubleOopPrimitives::hyperbolicSine), 
	1836033, 
	signature_116, 
	errors_116
};

static char* signature_117[] = { "Float", "Float"};
static char* errors_117[] = {NULL};
static primitive_desc primitive_117 = {
	"primitiveFloatHyperbolicTangentIfFail:", 
	fntype(&doubleOopPrimitives::hyperbolicTangent), 
	1836033, 
	signature_117, 
	errors_117
};

static char* signature_118[] = { "Boolean", "Float"};
static char* errors_118[] = {NULL};
static primitive_desc primitive_118 = {
	"primitiveFloatIsFinite", 
	fntype(&doubleOopPrimitives::isFinite), 
	1573889, 
	signature_118, 
	errors_118
};

static char* signature_119[] = { "Boolean", "Float"};
static char* errors_119[] = {NULL};
static primitive_desc primitive_119 = {
	"primitiveFloatIsNan", 
	fntype(&doubleOopPrimitives::isNan), 
	1573889, 
	signature_119, 
	errors_119
};

static char* signature_120[] = { "Boolean", "Float", "Float"};
static char* errors_120[] = {NULL};
static primitive_desc primitive_120 = {
	"primitiveFloatLessThan:ifFail:", 
	fntype(&doubleOopPrimitives::lessThan), 
	6030082, 
	signature_120, 
	errors_120
};

static char* signature_121[] = { "Boolean", "Float", "Float"};
static char* errors_121[] = {NULL};
static primitive_desc primitive_121 = {
	"primitiveFloatLessThanOrEqual:ifFail:", 
	fntype(&doubleOopPrimitives::lessThanOrEqual), 
	6030082, 
	signature_121, 
	errors_121
};

static char* signature_122[] = { "Float", "Float"};
static char* errors_122[] = { "ReceiverNotStrictlyPositive", NULL};
static primitive_desc primitive_122 = {
	"primitiveFloatLnIfFail:", 
	fntype(&doubleOopPrimitives::ln), 
	1836033, 
	signature_122, 
	errors_122
};

static char* signature_123[] = { "Float", "Float"};
static char* errors_123[] = { "ReceiverNotStrictlyPositive", NULL};
static primitive_desc primitive_123 = {
	"primitiveFloatLog10IfFail:", 
	fntype(&doubleOopPrimitives::log10), 
	1836033, 
	signature_123, 
	errors_123
};

static char* signature_124[] = { "Float", "Float"};
static char* errors_124[] = {NULL};
static primitive_desc primitive_124 = {
	"primitiveFloatMantissa", 
	fntype(&doubleOopPrimitives::mantissa), 
	1573889, 
	signature_124, 
	errors_124
};

static char* signature_125[] = { "Float"};
static char* errors_125[] = {NULL};
static primitive_desc primitive_125 = {
	"primitiveFloatMaxValue", 
	fntype(&doubleOopPrimitives::min_positive_value), 
	524288, 
	signature_125, 
	errors_125
};

static char* signature_126[] = { "Float"};
static char* errors_126[] = {NULL};
static primitive_desc primitive_126 = {
	"primitiveFloatMinPositiveValue", 
	fntype(&doubleOopPrimitives::min_positive_value), 
	524288, 
	signature_126, 
	errors_126
};

static char* signature_127[] = { "Float", "Float", "Float"};
static char* errors_127[] = {NULL};
static primitive_desc primitive_127 = {
	"primitiveFloatMod:ifFail:", 
	fntype(&doubleOopPrimitives::mod), 
	1836034, 
	signature_127, 
	errors_127
};

static char* signature_128[] = { "Float", "Float", "Float"};
static char* errors_128[] = {NULL};
static primitive_desc primitive_128 = {
	"primitiveFloatMultiply:ifFail:", 
	fntype(&double_multiply), 
	6030338, 
	signature_128, 
	errors_128
};

static char* signature_129[] = { "Boolean", "Float", "Float"};
static char* errors_129[] = {NULL};
static primitive_desc primitive_129 = {
	"primitiveFloatNotEqual:ifFail:", 
	fntype(&doubleOopPrimitives::notEqual), 
	6030082, 
	signature_129, 
	errors_129
};

static char* signature_130[] = { "Self", "Float", "IndexedByteInstanceVariables"};
static char* errors_130[] = {NULL};
static primitive_desc primitive_130 = {
	"primitiveFloatPrintFormat:ifFail:", 
	fntype(&doubleOopPrimitives::printFormat), 
	1310722, 
	signature_130, 
	errors_130
};

static char* signature_131[] = { "IndexedByteInstanceVariables", "Float"};
static char* errors_131[] = {NULL};
static primitive_desc primitive_131 = {
	"primitiveFloatPrintString", 
	fntype(&doubleOopPrimitives::printString), 
	1048577, 
	signature_131, 
	errors_131
};

static char* signature_132[] = { "SmallInteger", "Float"};
static char* errors_132[] = { "SmallIntegerConversionFailed", NULL};
static primitive_desc primitive_132 = {
	"primitiveFloatRoundedAsSmallIntegerIfFail:", 
	fntype(&doubleOopPrimitives::roundedAsSmallInteger), 
	1836033, 
	signature_132, 
	errors_132
};

static char* signature_133[] = { "Float", "Float"};
static char* errors_133[] = {NULL};
static primitive_desc primitive_133 = {
	"primitiveFloatSine", 
	fntype(&doubleOopPrimitives::sine), 
	1573889, 
	signature_133, 
	errors_133
};

static char* signature_134[] = { "SmallInteger", "Float"};
static char* errors_134[] = { "ConversionFailed", NULL};
static primitive_desc primitive_134 = {
	"primitiveFloatSmallIntegerFloorIfFail:", 
	fntype(&doubleOopPrimitives::smi_floor), 
	6030337, 
	signature_134, 
	errors_134
};

static char* signature_135[] = { "Float", "Float"};
static char* errors_135[] = { "ReceiverNegative", NULL};
static primitive_desc primitive_135 = {
	"primitiveFloatSqrtIfFail:", 
	fntype(&doubleOopPrimitives::sqrt), 
	1836033, 
	signature_135, 
	errors_135
};

static char* signature_136[] = { "Float", "Float"};
static char* errors_136[] = {NULL};
static primitive_desc primitive_136 = {
	"primitiveFloatSquared", 
	fntype(&doubleOopPrimitives::squared), 
	1573889, 
	signature_136, 
	errors_136
};

static char* signature_137[] = { "ByteArray", "Float"};
static char* errors_137[] = {NULL};
static primitive_desc primitive_137 = {
	"primitiveFloatStoreString", 
	fntype(&doubleOopPrimitives::store_string), 
	1048577, 
	signature_137, 
	errors_137
};

static char* signature_138[] = { "Float", "Float", "Float"};
static char* errors_138[] = {NULL};
static primitive_desc primitive_138 = {
	"primitiveFloatSubtract:ifFail:", 
	fntype(&double_subtract), 
	6030338, 
	signature_138, 
	errors_138
};

static char* signature_139[] = { "Float", "Float"};
static char* errors_139[] = {NULL};
static primitive_desc primitive_139 = {
	"primitiveFloatTangentIfFail:", 
	fntype(&doubleOopPrimitives::tangent), 
	1836033, 
	signature_139, 
	errors_139
};

static char* signature_140[] = { "Float", "Float", "SmallInteger"};
static char* errors_140[] = { "RangeError", NULL};
static primitive_desc primitive_140 = {
	"primitiveFloatTimesTwoPower:ifFail:", 
	fntype(&doubleOopPrimitives::timesTwoPower), 
	1836034, 
	signature_140, 
	errors_140
};

static char* signature_141[] = { "Float", "Float"};
static char* errors_141[] = {NULL};
static primitive_desc primitive_141 = {
	"primitiveFloatTruncated", 
	fntype(&doubleOopPrimitives::truncated), 
	1573889, 
	signature_141, 
	errors_141
};

static char* signature_142[] = { "Object"};
static char* errors_142[] = {NULL};
static primitive_desc primitive_142 = {
	"primitiveFlushCodeCache", 
	fntype(&systemPrimitives::flush_code_cache), 
	65536, 
	signature_142, 
	errors_142
};

static char* signature_143[] = { "Object"};
static char* errors_143[] = {NULL};
static primitive_desc primitive_143 = {
	"primitiveFlushDeadCode", 
	fntype(&systemPrimitives::flush_dead_code), 
	65536, 
	signature_143, 
	errors_143
};

static char* signature_144[] = { "SmallInteger"};
static char* errors_144[] = {NULL};
static primitive_desc primitive_144 = {
	"primitiveFreeSpace", 
	fntype(&systemPrimitives::freeSpace), 
	65536, 
	signature_144, 
	errors_144
};

static char* signature_145[] = { "Self", "Object"};
static char* errors_145[] = {NULL};
static primitive_desc primitive_145 = {
	"primitiveGarbageCollect", 
	fntype(&systemPrimitives::garbageGollect), 
	1114113, 
	signature_145, 
	errors_145
};

static char* signature_146[] = { "Object", "Object", "Symbol"};
static char* errors_146[] = { "NotFound", NULL};
static primitive_desc primitive_146 = {
	"primitiveGenerateIR:ifFail:", 
	fntype(&debugPrimitives::generateIR), 
	1376258, 
	signature_146, 
	errors_146
};

static char* signature_147[] = { "Boolean", "GlobalAssociation"};
static char* errors_147[] = {NULL};
static primitive_desc primitive_147 = {
	"primitiveGlobalAssociationIsConstant", 
	fntype(&systemPrimitives::globalAssociationIsConstant), 
	1114113, 
	signature_147, 
	errors_147
};

static char* signature_148[] = { "Symbol", "GlobalAssociation"};
static char* errors_148[] = {NULL};
static primitive_desc primitive_148 = {
	"primitiveGlobalAssociationKey", 
	fntype(&systemPrimitives::globalAssociationKey), 
	1114113, 
	signature_148, 
	errors_148
};

static char* signature_149[] = { "Boolean", "GlobalAssociation", "Boolean"};
static char* errors_149[] = {NULL};
static primitive_desc primitive_149 = {
	"primitiveGlobalAssociationSetConstant:", 
	fntype(&systemPrimitives::globalAssociationSetConstant), 
	1114114, 
	signature_149, 
	errors_149
};

static char* signature_150[] = { "Object", "GlobalAssociation", "Symbol"};
static char* errors_150[] = {NULL};
static primitive_desc primitive_150 = {
	"primitiveGlobalAssociationSetKey:", 
	fntype(&systemPrimitives::globalAssociationSetKey), 
	1114114, 
	signature_150, 
	errors_150
};

static char* signature_151[] = { "Object", "GlobalAssociation", "Object"};
static char* errors_151[] = {NULL};
static primitive_desc primitive_151 = {
	"primitiveGlobalAssociationSetValue:", 
	fntype(&systemPrimitives::globalAssociationSetValue), 
	1114114, 
	signature_151, 
	errors_151
};

static char* signature_152[] = { "Object", "GlobalAssociation"};
static char* errors_152[] = {NULL};
static primitive_desc primitive_152 = {
	"primitiveGlobalAssociationValue", 
	fntype(&systemPrimitives::globalAssociationValue), 
	1114113, 
	signature_152, 
	errors_152
};

static char* signature_153[] = { "Boolean", "SmallInteger", "SmallInteger"};
static char* errors_153[] = {NULL};
static primitive_desc primitive_153 = {
	"primitiveGreaterThan:ifFail:", 
	fntype(&smiOopPrimitives::greaterThan), 
	6029570, 
	signature_153, 
	errors_153
};

static char* signature_154[] = { "Boolean", "SmallInteger", "SmallInteger"};
static char* errors_154[] = {NULL};
static primitive_desc primitive_154 = {
	"primitiveGreaterThanOrEqual:ifFail:", 
	fntype(&smiOopPrimitives::greaterThanOrEqual), 
	6029570, 
	signature_154, 
	errors_154
};

static char* signature_155[] = { "Boolean", "Object"};
static char* errors_155[] = {NULL};
static primitive_desc primitive_155 = {
	"primitiveHadNearDeathExperience:", 
	fntype(&systemPrimitives::hadNearDeathExperience), 
	65537, 
	signature_155, 
	errors_155
};

static char* signature_156[] = { "Object"};
static char* errors_156[] = {NULL};
static primitive_desc primitive_156 = {
	"primitiveHalt", 
	fntype(&systemPrimitives::halt), 
	65536, 
	signature_156, 
	errors_156
};

static char* signature_157[] = { "SmallInteger", "Object"};
static char* errors_157[] = {NULL};
static primitive_desc primitive_157 = {
	"primitiveHash", 
	fntype(&oopPrimitives::hash), 
	1114113, 
	signature_157, 
	errors_157
};

static char* signature_158[] = { "SmallInteger", "Object"};
static char* errors_158[] = {NULL};
static primitive_desc primitive_158 = {
	"primitiveHashOf:", 
	fntype(&oopPrimitives::hash_of), 
	65537, 
	signature_158, 
	errors_158
};

static char* signature_159[] = { "SmallInteger", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_159[] = { "OutOfBounds", NULL};
static primitive_desc primitive_159 = {
	"primitiveIndexedByteAt:ifFail:", 
	fntype(&byteArrayPrimitives::at), 
	1312258, 
	signature_159, 
	errors_159
};

static char* signature_160[] = { "SmallInteger", "IndexedByteInstanceVariables", "SmallInteger", "SmallInteger"};
static char* errors_160[] = { "OutOfBounds", "ValueOutOfBounds", NULL};
static primitive_desc primitive_160 = {
	"primitiveIndexedByteAt:put:ifFail:", 
	fntype(&byteArrayPrimitives::atPut), 
	1312259, 
	signature_160, 
	errors_160
};

static char* signature_161[] = { "Self", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_161[] = {NULL};
static primitive_desc primitive_161 = {
	"primitiveIndexedByteAtAllPut:ifFail:", 
	fntype(&byteArrayPrimitives::at_all_put), 
	1376258, 
	signature_161, 
	errors_161
};

static char* signature_162[] = { "SmallInteger", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_162[] = { "OutOfBounds", NULL};
static primitive_desc primitive_162 = {
	"primitiveIndexedByteCharacterAt:ifFail:", 
	fntype(&byteArrayPrimitives::characterAt), 
	1312258, 
	signature_162, 
	errors_162
};

static char* signature_163[] = { "SmallInteger", "IndexedByteInstanceVariables", "String"};
static char* errors_163[] = {NULL};
static primitive_desc primitive_163 = {
	"primitiveIndexedByteCompare:ifFail:", 
	fntype(&byteArrayPrimitives::compare), 
	1376258, 
	signature_163, 
	errors_163
};

static char* signature_164[] = { "SmallInteger", "IndexedByteInstanceVariables"};
static char* errors_164[] = {NULL};
static primitive_desc primitive_164 = {
	"primitiveIndexedByteHash", 
	fntype(&byteArrayPrimitives::hash), 
	1574401, 
	signature_164, 
	errors_164
};

static char* signature_165[] = { "CompressedSymbol", "IndexedByteInstanceVariables"};
static char* errors_165[] = { "ValueOutOfBounds", NULL};
static primitive_desc primitive_165 = {
	"primitiveIndexedByteInternIfFail:", 
	fntype(&byteArrayPrimitives::intern), 
	1376257, 
	signature_165, 
	errors_165
};

static char* signature_166[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_166[] = { "ArgumentIsInvalid", NULL};
static primitive_desc primitive_166 = {
	"primitiveIndexedByteLargeIntegerAdd:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerAdd), 
	1312258, 
	signature_166, 
	errors_166
};

static char* signature_167[] = { "Float", "IndexedByteInstanceVariables"};
static char* errors_167[] = {NULL};
static primitive_desc primitive_167 = {
	"primitiveIndexedByteLargeIntegerAsFloatIfFail:", 
	fntype(&byteArrayPrimitives::largeIntegerToFloat), 
	1312257, 
	signature_167, 
	errors_167
};

static char* signature_168[] = { "SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_168[] = {NULL};
static primitive_desc primitive_168 = {
	"primitiveIndexedByteLargeIntegerCompare:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerCompare), 
	1312258, 
	signature_168, 
	errors_168
};

static char* signature_169[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_169[] = { "ArgumentIsInvalid", "DivisionByZero", NULL};
static primitive_desc primitive_169 = {
	"primitiveIndexedByteLargeIntegerDiv:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerDiv), 
	1312258, 
	signature_169, 
	errors_169
};

static char* signature_170[] = { "IndexedByteInstanceVariables", "IndexedByteInstanceVariables class", "Float"};
static char* errors_170[] = {NULL};
static primitive_desc primitive_170 = {
	"primitiveIndexedByteLargeIntegerFromFloat:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerFromDouble), 
	1310722, 
	signature_170, 
	errors_170
};

static char* signature_171[] = { "IndexedByteInstanceVariables", "IndexedByteInstanceVariables class", "SmallInteger"};
static char* errors_171[] = {NULL};
static primitive_desc primitive_171 = {
	"primitiveIndexedByteLargeIntegerFromSmallInteger:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerFromSmallInteger), 
	1310722, 
	signature_171, 
	errors_171
};

static char* signature_172[] = { "IndexedByteInstanceVariables", "IndexedByteInstanceVariables class", "String", "Integer"};
static char* errors_172[] = { "ConversionFailed", NULL};
static primitive_desc primitive_172 = {
	"primitiveIndexedByteLargeIntegerFromString:base:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerFromString), 
	1312259, 
	signature_172, 
	errors_172
};

static char* signature_173[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_173[] = { "ArgumentIsInvalid", "DivisionByZero", NULL};
static primitive_desc primitive_173 = {
	"primitiveIndexedByteLargeIntegerMod:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerMod), 
	1312258, 
	signature_173, 
	errors_173
};

static char* signature_174[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_174[] = { "ArgumentIsInvalid", NULL};
static primitive_desc primitive_174 = {
	"primitiveIndexedByteLargeIntegerMultiply:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerMultiply), 
	1312258, 
	signature_174, 
	errors_174
};

static char* signature_175[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_175[] = { "ArgumentIsInvalid", NULL};
static primitive_desc primitive_175 = {
	"primitiveIndexedByteLargeIntegerSubtract:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerSubtract), 
	1312258, 
	signature_175, 
	errors_175
};

static char* signature_176[] = { "String", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_176[] = {NULL};
static primitive_desc primitive_176 = {
	"primitiveIndexedByteLargeIntegerToStringBase:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerToString), 
	1312258, 
	signature_176, 
	errors_176
};

static char* signature_177[] = { "Object", "IndexedByteInstanceVariables class", "SmallInteger"};
static char* errors_177[] = { "NegativeSize", NULL};
static primitive_desc primitive_177 = {
	"primitiveIndexedByteNew:ifFail:", 
	fntype(&byteArrayPrimitives::allocateSize), 
	1376258, 
	signature_177, 
	errors_177
};

static char* signature_178[] = { "Object", "IndexedByteInstanceVariables class", "SmallInteger", "Boolean"};
static char* errors_178[] = { "NegativeSize", NULL};
static primitive_desc primitive_178 = {
	"primitiveIndexedByteNew:size:tenured:ifFail:", 
	fntype(&byteArrayPrimitives::allocateSize2), 
	327683, 
	signature_178, 
	errors_178
};

static char* signature_179[] = { "SmallInteger", "IndexedByteInstanceVariables"};
static char* errors_179[] = {NULL};
static primitive_desc primitive_179 = {
	"primitiveIndexedByteSize", 
	fntype(&byteArrayPrimitives::size), 
	1574401, 
	signature_179, 
	errors_179
};

static char* signature_180[] = { "SmallInteger", "IndexedDoubleByteInstanceVariables", "SmallInteger"};
static char* errors_180[] = { "OutOfBounds", NULL};
static primitive_desc primitive_180 = {
	"primitiveIndexedDoubleByteAt:ifFail:", 
	fntype(&doubleByteArrayPrimitives::at), 
	1312514, 
	signature_180, 
	errors_180
};

static char* signature_181[] = { "SmallInteger", "IndexedDoubleByteInstanceVariables", "SmallInteger", "SmallInteger"};
static char* errors_181[] = { "OutOfBounds", "ValueOutOfBounds", NULL};
static primitive_desc primitive_181 = {
	"primitiveIndexedDoubleByteAt:put:ifFail:", 
	fntype(&doubleByteArrayPrimitives::atPut), 
	1312515, 
	signature_181, 
	errors_181
};

static char* signature_182[] = { "SmallInteger", "IndexedDoubleByteInstanceVariables", "SmallInteger"};
static char* errors_182[] = { "OutOfBounds", NULL};
static primitive_desc primitive_182 = {
	"primitiveIndexedDoubleByteCharacterAt:ifFail:", 
	fntype(&doubleByteArrayPrimitives::characterAt), 
	1312514, 
	signature_182, 
	errors_182
};

static char* signature_183[] = { "SmallInteger", "IndexedDoubleByteInstanceVariables", "String"};
static char* errors_183[] = {NULL};
static primitive_desc primitive_183 = {
	"primitiveIndexedDoubleByteCompare:ifFail:", 
	fntype(&doubleByteArrayPrimitives::compare), 
	1310722, 
	signature_183, 
	errors_183
};

static char* signature_184[] = { "SmallInteger", "IndexedDoubleByteInstanceVariables"};
static char* errors_184[] = {NULL};
static primitive_desc primitive_184 = {
	"primitiveIndexedDoubleByteHash", 
	fntype(&doubleByteArrayPrimitives::hash), 
	1114113, 
	signature_184, 
	errors_184
};

static char* signature_185[] = { "CompressedSymbol", "IndexedDoubleByteInstanceVariables"};
static char* errors_185[] = { "ValueOutOfBounds", NULL};
static primitive_desc primitive_185 = {
	"primitiveIndexedDoubleByteInternIfFail:", 
	fntype(&doubleByteArrayPrimitives::intern), 
	1376257, 
	signature_185, 
	errors_185
};

static char* signature_186[] = { "Object", "IndexedDoubleByteInstanceVariables class", "SmallInteger"};
static char* errors_186[] = { "NegativeSize", NULL};
static primitive_desc primitive_186 = {
	"primitiveIndexedDoubleByteNew:ifFail:", 
	fntype(&doubleByteArrayPrimitives::allocateSize), 
	1376258, 
	signature_186, 
	errors_186
};

static char* signature_187[] = { "Object", "IndexedDoubleByteInstanceVariables class", "SmallInteger", "Boolean"};
static char* errors_187[] = { "NegativeSize", NULL};
static primitive_desc primitive_187 = {
	"primitiveIndexedDoubleByteNew:size:tenured:ifFail:", 
	fntype(&doubleByteArrayPrimitives::allocateSize2), 
	327683, 
	signature_187, 
	errors_187
};

static char* signature_188[] = { "SmallInteger", "IndexedDoubleByteInstanceVariables"};
static char* errors_188[] = {NULL};
static primitive_desc primitive_188 = {
	"primitiveIndexedDoubleByteSize", 
	fntype(&doubleByteArrayPrimitives::size), 
	1574657, 
	signature_188, 
	errors_188
};

static char* signature_189[] = { "Float", "IndexedFloatValueInstanceVariables", "SmallInteger"};
static char* errors_189[] = { "OutOfBounds", NULL};
static primitive_desc primitive_189 = {
	"primitiveIndexedFloatValueAt:ifFail:", 
	fntype(&doubleValueArrayPrimitives::at), 
	1310722, 
	signature_189, 
	errors_189
};

static char* signature_190[] = { "Float", "IndexedFloatValueInstanceVariables", "SmallInteger", "Float"};
static char* errors_190[] = { "OutOfBounds", "ValueOutOfBounds", NULL};
static primitive_desc primitive_190 = {
	"primitiveIndexedFloatValueAt:put:ifFail:", 
	fntype(&doubleValueArrayPrimitives::atPut), 
	1310723, 
	signature_190, 
	errors_190
};

static char* signature_191[] = { "Object", "IndexedFloatValueInstanceVariables class", "SmallInteger"};
static char* errors_191[] = { "NegativeSize", NULL};
static primitive_desc primitive_191 = {
	"primitiveIndexedFloatValueNew:ifFail:", 
	fntype(&doubleValueArrayPrimitives::allocateSize), 
	1376258, 
	signature_191, 
	errors_191
};

static char* signature_192[] = { "SmallInteger", "IndexedFloatValueInstanceVariables"};
static char* errors_192[] = {NULL};
static primitive_desc primitive_192 = {
	"primitiveIndexedFloatValueSize", 
	fntype(&doubleValueArrayPrimitives::size), 
	1572865, 
	signature_192, 
	errors_192
};

static char* signature_193[] = { "SmallInteger", "IndexedInstanceVariables", "SmallInteger"};
static char* errors_193[] = { "OutOfBounds", NULL};
static primitive_desc primitive_193 = {
	"primitiveIndexedObjectAt:ifFail:", 
	fntype(&objArrayPrimitives::at), 
	1312002, 
	signature_193, 
	errors_193
};

static char* signature_194[] = { "Object", "IndexedInstanceVariables", "SmallInteger", "Object"};
static char* errors_194[] = { "OutOfBounds", NULL};
static primitive_desc primitive_194 = {
	"primitiveIndexedObjectAt:put:ifFail:", 
	fntype(&objArrayPrimitives::atPut), 
	1312003, 
	signature_194, 
	errors_194
};

static char* signature_195[] = { "Self", "IndexedInstanceVariables", "Object"};
static char* errors_195[] = {NULL};
static primitive_desc primitive_195 = {
	"primitiveIndexedObjectAtAllPut:", 
	fntype(&objArrayPrimitives::at_all_put), 
	1049858, 
	signature_195, 
	errors_195
};

static char* signature_196[] = { "Self", "IndexedInstanceVariables", "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_196[] = { "OutOfBounds", "NegativeSize", NULL};
static primitive_desc primitive_196 = {
	"primitiveIndexedObjectCopyFrom:startingAt:size:ifFail:", 
	fntype(&objArrayPrimitives::copy_size), 
	1377540, 
	signature_196, 
	errors_196
};

static char* signature_197[] = { "Object", "IndexedInstanceVariables class", "SmallInteger"};
static char* errors_197[] = { "NegativeSize", NULL};
static primitive_desc primitive_197 = {
	"primitiveIndexedObjectNew:ifFail:", 
	fntype(&objArrayPrimitives::allocateSize), 
	1376258, 
	signature_197, 
	errors_197
};

static char* signature_198[] = { "Object", "IndexedInstanceVariables class", "SmallInteger", "Boolean"};
static char* errors_198[] = { "NegativeSize", NULL};
static primitive_desc primitive_198 = {
	"primitiveIndexedObjectNew:size:tenured:ifFail:", 
	fntype(&objArrayPrimitives::allocateSize2), 
	327683, 
	signature_198, 
	errors_198
};

static char* signature_199[] = { "Self", "IndexedInstanceVariables", "SmallInteger", "SmallInteger", "IndexedInstanceVariables", "SmallInteger"};
static char* errors_199[] = { "OutOfBounds", NULL};
static primitive_desc primitive_199 = {
	"primitiveIndexedObjectReplaceFrom:to:with:startingAt:ifFail:", 
	fntype(&objArrayPrimitives::replace_from_to), 
	1377541, 
	signature_199, 
	errors_199
};

static char* signature_200[] = { "Self", "IndexedInstanceVariables"};
static char* errors_200[] = {NULL};
static primitive_desc primitive_200 = {
	"primitiveIndexedObjectSize", 
	fntype(&objArrayPrimitives::size), 
	1574145, 
	signature_200, 
	errors_200
};

static char* signature_201[] = { "Instance", "Behavior", "SmallInt"};
static char* errors_201[] = {NULL};
static primitive_desc primitive_201 = {
	"primitiveInlineAllocations:count:", 
	fntype(&primitiveInlineAllocations), 
	4259842, 
	signature_201, 
	errors_201
};

static char* signature_202[] = { "Boolean", "Behavior", "Symbol"};
static char* errors_202[] = {NULL};
static primitive_desc primitive_202 = {
	"primitiveInliningDatabaseAddLookupEntryClass:selector:ifFail:", 
	fntype(&systemPrimitives::inlining_database_add_entry), 
	327682, 
	signature_202, 
	errors_202
};

static char* signature_203[] = { "Boolean"};
static char* errors_203[] = {NULL};
static primitive_desc primitive_203 = {
	"primitiveInliningDatabaseCompile", 
	fntype(&systemPrimitives::inlining_database_compile_next), 
	65536, 
	signature_203, 
	errors_203
};

static char* signature_204[] = { "Object", "String"};
static char* errors_204[] = {NULL};
static primitive_desc primitive_204 = {
	"primitiveInliningDatabaseCompile:ifFail:", 
	fntype(&systemPrimitives::inlining_database_compile), 
	327681, 
	signature_204, 
	errors_204
};

static char* signature_205[] = { "IndexedByteInstanceVariables", "String"};
static char* errors_205[] = {NULL};
static primitive_desc primitive_205 = {
	"primitiveInliningDatabaseCompileDemangled:ifFail:", 
	fntype(&systemPrimitives::inlining_database_demangle), 
	327681, 
	signature_205, 
	errors_205
};

static char* signature_206[] = { "Symbol"};
static char* errors_206[] = {NULL};
static primitive_desc primitive_206 = {
	"primitiveInliningDatabaseDirectory", 
	fntype(&systemPrimitives::inlining_database_directory), 
	65536, 
	signature_206, 
	errors_206
};

static char* signature_207[] = { "SmallInteger"};
static char* errors_207[] = {NULL};
static primitive_desc primitive_207 = {
	"primitiveInliningDatabaseFileOutAllIfFail:", 
	fntype(&systemPrimitives::inlining_database_file_out_all), 
	327680, 
	signature_207, 
	errors_207
};

static char* signature_208[] = { "SmallInteger", "Behavior"};
static char* errors_208[] = {NULL};
static primitive_desc primitive_208 = {
	"primitiveInliningDatabaseFileOutClass:ifFail:", 
	fntype(&systemPrimitives::inlining_database_file_out_class), 
	327681, 
	signature_208, 
	errors_208
};

static char* signature_209[] = { "IndexedByteInstanceVariables", "String"};
static char* errors_209[] = {NULL};
static primitive_desc primitive_209 = {
	"primitiveInliningDatabaseMangle:ifFail:", 
	fntype(&systemPrimitives::inlining_database_mangle), 
	327681, 
	signature_209, 
	errors_209
};

static char* signature_210[] = { "Symbol", "Symbol"};
static char* errors_210[] = {NULL};
static primitive_desc primitive_210 = {
	"primitiveInliningDatabaseSetDirectory:ifFail:", 
	fntype(&systemPrimitives::inlining_database_set_directory), 
	327681, 
	signature_210, 
	errors_210
};

static char* signature_211[] = { "Object", "Object", "SmallInteger"};
static char* errors_211[] = { "OutOfBounds", NULL};
static primitive_desc primitive_211 = {
	"primitiveInstVarAt:ifFail:", 
	fntype(&oopPrimitives::instVarAt), 
	1376258, 
	signature_211, 
	errors_211
};

static char* signature_212[] = { "Object", "Object", "SmallInteger", "Object"};
static char* errors_212[] = { "OutOfBounds", NULL};
static primitive_desc primitive_212 = {
	"primitiveInstVarAt:put:ifFail:", 
	fntype(&oopPrimitives::instVarAtPut), 
	1376259, 
	signature_212, 
	errors_212
};

static char* signature_213[] = { "Symbol", "Reciever", "Object", "SmallInteger"};
static char* errors_213[] = { "OutOfBounds", NULL};
static primitive_desc primitive_213 = {
	"primitiveInstVarNameFor:at:ifFail:", 
	fntype(&oopPrimitives::instance_variable_name_at), 
	1376259, 
	signature_213, 
	errors_213
};

static char* signature_214[] = { "IndexedInstanceVariables", "Class", "SmallInteger"};
static char* errors_214[] = {NULL};
static primitive_desc primitive_214 = {
	"primitiveInstancesOf:limit:ifFail:", 
	fntype(&systemPrimitives::instances_of), 
	327682, 
	signature_214, 
	errors_214
};

static char* signature_215[] = { "SmallInteger"};
static char* errors_215[] = {NULL};
static primitive_desc primitive_215 = {
	"primitiveInterpreterInvocationCounterLimit", 
	fntype(&debugPrimitives::interpreterInvocationCounterLimit), 
	65536, 
	signature_215, 
	errors_215
};

static char* signature_216[] = { "Boolean", "SmallInteger", "SmallInteger"};
static char* errors_216[] = {NULL};
static primitive_desc primitive_216 = {
	"primitiveLessThan:ifFail:", 
	fntype(&smiOopPrimitives::lessThan), 
	6029570, 
	signature_216, 
	errors_216
};

static char* signature_217[] = { "Boolean", "SmallInteger", "SmallInteger"};
static char* errors_217[] = {NULL};
static primitive_desc primitive_217 = {
	"primitiveLessThanOrEqual:ifFail:", 
	fntype(&smiOopPrimitives::lessThanOrEqual), 
	6029570, 
	signature_217, 
	errors_217
};

static char* signature_218[] = { "SmallInteger", "Float", "Float", "SmallInteger"};
static char* errors_218[] = {NULL};
static primitive_desc primitive_218 = {
	"primitiveMandelbrotAtRe:im:iterate:ifFail:", 
	fntype(&doubleOopPrimitives::mandelbrot), 
	4980739, 
	signature_218, 
	errors_218
};

static char* signature_219[] = { "Block", "Method", "Object"};
static char* errors_219[] = {NULL};
static primitive_desc primitive_219 = {
	"primitiveMethodAllocateBlock:ifFail:", 
	fntype(&methodOopPrimitives::allocate_block_self), 
	1376258, 
	signature_219, 
	errors_219
};

static char* signature_220[] = { "Block", "Method"};
static char* errors_220[] = {NULL};
static primitive_desc primitive_220 = {
	"primitiveMethodAllocateBlockIfFail:", 
	fntype(&methodOopPrimitives::allocate_block), 
	1376257, 
	signature_220, 
	errors_220
};

static char* signature_221[] = { "Object", "Method"};
static char* errors_221[] = {NULL};
static primitive_desc primitive_221 = {
	"primitiveMethodBody", 
	fntype(&methodOopPrimitives::fileout_body), 
	1114113, 
	signature_221, 
	errors_221
};

static char* signature_222[] = { "Object", "Method"};
static char* errors_222[] = {NULL};
static primitive_desc primitive_222 = {
	"primitiveMethodDebugInfo", 
	fntype(&methodOopPrimitives::debug_info), 
	1114113, 
	signature_222, 
	errors_222
};

static char* signature_223[] = { "Method", "Behavior", "CompressedSymbol"};
static char* errors_223[] = { "NotFound", NULL};
static primitive_desc primitive_223 = {
	"primitiveMethodFor:ifFail:", 
	fntype(&behaviorPrimitives::methodFor), 
	1376258, 
	signature_223, 
	errors_223
};

static char* signature_224[] = { "Symbol", "Method"};
static char* errors_224[] = {NULL};
static primitive_desc primitive_224 = {
	"primitiveMethodInliningInfo", 
	fntype(&methodOopPrimitives::inlining_info), 
	1114113, 
	signature_224, 
	errors_224
};

static char* signature_225[] = { "SmallInteger", "Method"};
static char* errors_225[] = {NULL};
static primitive_desc primitive_225 = {
	"primitiveMethodNumberOfArguments", 
	fntype(&methodOopPrimitives::numberOfArguments), 
	1114113, 
	signature_225, 
	errors_225
};

static char* signature_226[] = { "Symbol", "Method", "Method"};
static char* errors_226[] = {NULL};
static primitive_desc primitive_226 = {
	"primitiveMethodOuter:ifFail:", 
	fntype(&methodOopPrimitives::setOuter), 
	1376258, 
	signature_226, 
	errors_226
};

static char* signature_227[] = { "Method", "Method"};
static char* errors_227[] = { "ReceiverNotBlockMethod", NULL};
static primitive_desc primitive_227 = {
	"primitiveMethodOuterIfFail:", 
	fntype(&methodOopPrimitives::outer), 
	1376257, 
	signature_227, 
	errors_227
};

static char* signature_228[] = { "Method", "Method", "Object"};
static char* errors_228[] = {NULL};
static primitive_desc primitive_228 = {
	"primitiveMethodPrettyPrintKlass:ifFail:", 
	fntype(&methodOopPrimitives::prettyPrint), 
	1376258, 
	signature_228, 
	errors_228
};

static char* signature_229[] = { "ByteIndexedInstanceVariables", "Method", "Object"};
static char* errors_229[] = {NULL};
static primitive_desc primitive_229 = {
	"primitiveMethodPrettyPrintSourceKlass:ifFail:", 
	fntype(&methodOopPrimitives::prettyPrintSource), 
	1376258, 
	signature_229, 
	errors_229
};

static char* signature_230[] = { "Symbol", "Method"};
static char* errors_230[] = {NULL};
static primitive_desc primitive_230 = {
	"primitiveMethodPrintCodes", 
	fntype(&methodOopPrimitives::printCodes), 
	1114113, 
	signature_230, 
	errors_230
};

static char* signature_231[] = { "IndexedInstanceVariables", "Method"};
static char* errors_231[] = {NULL};
static primitive_desc primitive_231 = {
	"primitiveMethodReferencedClassVarNames", 
	fntype(&methodOopPrimitives::referenced_class_variable_names), 
	1114113, 
	signature_231, 
	errors_231
};

static char* signature_232[] = { "IndexedInstanceVariables", "Method"};
static char* errors_232[] = {NULL};
static primitive_desc primitive_232 = {
	"primitiveMethodReferencedGlobalNames", 
	fntype(&methodOopPrimitives::referenced_global_names), 
	1114113, 
	signature_232, 
	errors_232
};

static char* signature_233[] = { "IndexedInstanceVariables", "Method", "Mixin"};
static char* errors_233[] = {NULL};
static primitive_desc primitive_233 = {
	"primitiveMethodReferencedInstVarNamesMixin:ifFail:", 
	fntype(&methodOopPrimitives::referenced_instance_variable_names), 
	1376258, 
	signature_233, 
	errors_233
};

static char* signature_234[] = { "Symbol", "Method"};
static char* errors_234[] = {NULL};
static primitive_desc primitive_234 = {
	"primitiveMethodSelector", 
	fntype(&methodOopPrimitives::selector), 
	1114113, 
	signature_234, 
	errors_234
};

static char* signature_235[] = { "Symbol", "Method", "Symbol"};
static char* errors_235[] = {NULL};
static primitive_desc primitive_235 = {
	"primitiveMethodSelector:ifFail:", 
	fntype(&methodOopPrimitives::setSelector), 
	1376258, 
	signature_235, 
	errors_235
};

static char* signature_236[] = { "IndexedInstanceVariables", "Method"};
static char* errors_236[] = {NULL};
static primitive_desc primitive_236 = {
	"primitiveMethodSenders", 
	fntype(&methodOopPrimitives::senders), 
	1114113, 
	signature_236, 
	errors_236
};

static char* signature_237[] = { "Symbol", "Method", "Symbol"};
static char* errors_237[] = { "ArgumentIsInvalid", NULL};
static primitive_desc primitive_237 = {
	"primitiveMethodSetInliningInfo:ifFail:", 
	fntype(&methodOopPrimitives::set_inlining_info), 
	1376258, 
	signature_237, 
	errors_237
};

static char* signature_238[] = { "Object", "Method"};
static char* errors_238[] = {NULL};
static primitive_desc primitive_238 = {
	"primitiveMethodSizeAndFlags", 
	fntype(&methodOopPrimitives::size_and_flags), 
	1114113, 
	signature_238, 
	errors_238
};

static char* signature_239[] = { "Mixin", "Mixin", "Symbol"};
static char* errors_239[] = { "IsInstalled", "OutOfBounds", NULL};
static primitive_desc primitive_239 = {
	"primitiveMixin:addClassVariable:ifFail:", 
	fntype(&mixinOopPrimitives::add_class_variable), 
	327682, 
	signature_239, 
	errors_239
};

static char* signature_240[] = { "Symbol", "Mixin", "Symbol"};
static char* errors_240[] = { "IsInstalled", "OutOfBounds", NULL};
static primitive_desc primitive_240 = {
	"primitiveMixin:addInstanceVariable:ifFail:", 
	fntype(&mixinOopPrimitives::add_instance_variable), 
	327682, 
	signature_240, 
	errors_240
};

static char* signature_241[] = { "Method", "Mixin", "Method"};
static char* errors_241[] = { "IsInstalled", NULL};
static primitive_desc primitive_241 = {
	"primitiveMixin:addMethod:ifFail:", 
	fntype(&mixinOopPrimitives::add_method), 
	327682, 
	signature_241, 
	errors_241
};

static char* signature_242[] = { "Symbol", "Mixin", "SmallInteger"};
static char* errors_242[] = { "OutOfBounds", NULL};
static primitive_desc primitive_242 = {
	"primitiveMixin:classVariableAt:ifFail:", 
	fntype(&mixinOopPrimitives::class_variable_at), 
	327682, 
	signature_242, 
	errors_242
};

static char* signature_243[] = { "Symbol", "Mixin"};
static char* errors_243[] = {NULL};
static primitive_desc primitive_243 = {
	"primitiveMixin:classVariablesIfFail:", 
	fntype(&mixinOopPrimitives::class_variables), 
	327681, 
	signature_243, 
	errors_243
};

static char* signature_244[] = { "Symbol", "Mixin", "SmallInteger"};
static char* errors_244[] = { "OutOfBounds", NULL};
static primitive_desc primitive_244 = {
	"primitiveMixin:instanceVariableAt:ifFail:", 
	fntype(&mixinOopPrimitives::instance_variable_at), 
	327682, 
	signature_244, 
	errors_244
};

static char* signature_245[] = { "Symbol", "Mixin"};
static char* errors_245[] = {NULL};
static primitive_desc primitive_245 = {
	"primitiveMixin:instanceVariablesIfFail:", 
	fntype(&mixinOopPrimitives::instance_variables), 
	327681, 
	signature_245, 
	errors_245
};

static char* signature_246[] = { "Method", "Mixin", "SmallInteger"};
static char* errors_246[] = { "OutOfBounds", NULL};
static primitive_desc primitive_246 = {
	"primitiveMixin:methodAt:ifFail:", 
	fntype(&mixinOopPrimitives::method_at), 
	327682, 
	signature_246, 
	errors_246
};

static char* signature_247[] = { "Symbol", "Mixin"};
static char* errors_247[] = {NULL};
static primitive_desc primitive_247 = {
	"primitiveMixin:methodsIfFail:", 
	fntype(&mixinOopPrimitives::methods), 
	327681, 
	signature_247, 
	errors_247
};

static char* signature_248[] = { "Symbol", "Mixin", "SmallInteger"};
static char* errors_248[] = { "IsInstalled", "OutOfBounds", NULL};
static primitive_desc primitive_248 = {
	"primitiveMixin:removeClassVariableAt:ifFail:", 
	fntype(&mixinOopPrimitives::remove_class_variable_at), 
	327682, 
	signature_248, 
	errors_248
};

static char* signature_249[] = { "Symbol", "Mixin", "SmallInteger"};
static char* errors_249[] = { "IsInstalled", "OutOfBounds", NULL};
static primitive_desc primitive_249 = {
	"primitiveMixin:removeInstanceVariableAt:ifFail:", 
	fntype(&mixinOopPrimitives::remove_instance_variable_at), 
	327682, 
	signature_249, 
	errors_249
};

static char* signature_250[] = { "Method", "Mixin", "SmallInteger"};
static char* errors_250[] = { "IsInstalled", "OutOfBounds", NULL};
static primitive_desc primitive_250 = {
	"primitiveMixin:removeMethodAt:ifFail:", 
	fntype(&mixinOopPrimitives::remove_method_at), 
	327682, 
	signature_250, 
	errors_250
};

static char* signature_251[] = { "Mixin", "Mixin"};
static char* errors_251[] = {NULL};
static primitive_desc primitive_251 = {
	"primitiveMixinClassMixinOf:ifFail:", 
	fntype(&mixinOopPrimitives::class_mixin), 
	327681, 
	signature_251, 
	errors_251
};

static char* signature_252[] = { "Boolean", "Mixin"};
static char* errors_252[] = {NULL};
static primitive_desc primitive_252 = {
	"primitiveMixinIsInstalled:ifFail:", 
	fntype(&mixinOopPrimitives::is_installed), 
	327681, 
	signature_252, 
	errors_252
};

static char* signature_253[] = { "SmallInteger", "Mixin"};
static char* errors_253[] = {NULL};
static primitive_desc primitive_253 = {
	"primitiveMixinNumberOfClassVariablesOf:ifFail:", 
	fntype(&mixinOopPrimitives::number_of_class_variables), 
	327681, 
	signature_253, 
	errors_253
};

static char* signature_254[] = { "SmallInteger", "Mixin"};
static char* errors_254[] = {NULL};
static primitive_desc primitive_254 = {
	"primitiveMixinNumberOfInstanceVariablesOf:ifFail:", 
	fntype(&mixinOopPrimitives::number_of_instance_variables), 
	327681, 
	signature_254, 
	errors_254
};

static char* signature_255[] = { "SmallInteger", "Mixin"};
static char* errors_255[] = {NULL};
static primitive_desc primitive_255 = {
	"primitiveMixinNumberOfMethodsOf:ifFail:", 
	fntype(&mixinOopPrimitives::number_of_methods), 
	327681, 
	signature_255, 
	errors_255
};

static char* signature_256[] = { "Class", "Mixin"};
static char* errors_256[] = {NULL};
static primitive_desc primitive_256 = {
	"primitiveMixinPrimaryInvocationOf:ifFail:", 
	fntype(&mixinOopPrimitives::primary_invocation), 
	327681, 
	signature_256, 
	errors_256
};

static char* signature_257[] = { "Mixin", "Mixin", "Mixin"};
static char* errors_257[] = { "IsInstalled", NULL};
static primitive_desc primitive_257 = {
	"primitiveMixinSetClassMixinOf:to:ifFail:", 
	fntype(&mixinOopPrimitives::set_class_mixin), 
	327682, 
	signature_257, 
	errors_257
};

static char* signature_258[] = { "Boolean", "Mixin"};
static char* errors_258[] = {NULL};
static primitive_desc primitive_258 = {
	"primitiveMixinSetInstalled:ifFail:", 
	fntype(&mixinOopPrimitives::set_installed), 
	327681, 
	signature_258, 
	errors_258
};

static char* signature_259[] = { "Class", "Mixin", "Class"};
static char* errors_259[] = { "IsInstalled", NULL};
static primitive_desc primitive_259 = {
	"primitiveMixinSetPrimaryInvocationOf:to:ifFail:", 
	fntype(&mixinOopPrimitives::set_primary_invocation), 
	327682, 
	signature_259, 
	errors_259
};

static char* signature_260[] = { "Boolean", "Mixin"};
static char* errors_260[] = {NULL};
static primitive_desc primitive_260 = {
	"primitiveMixinSetUnInstalled:ifFail:", 
	fntype(&mixinOopPrimitives::set_uninstalled), 
	327681, 
	signature_260, 
	errors_260
};

static char* signature_261[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_261[] = { "Overflow", "DivisionByZero", NULL};
static primitive_desc primitive_261 = {
	"primitiveMod:ifFail:", 
	fntype(&smiOopPrimitives_mod), 
	6029826, 
	signature_261, 
	errors_261
};

static char* signature_262[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_262[] = { "Overflow", NULL};
static primitive_desc primitive_262 = {
	"primitiveMultiply:ifFail:", 
	fntype(&smiOopPrimitives_multiply), 
	6029826, 
	signature_262, 
	errors_262
};

static char* signature_263[] = { "Instance", "Behavior", "Boolean"};
static char* errors_263[] = {NULL};
static primitive_desc primitive_263 = {
	"primitiveNew0:ifFail:", 
	fntype(&primitiveNew0), 
	7667714, 
	signature_263, 
	errors_263
};

static char* signature_264[] = { "Instance", "Behavior", "Boolean"};
static char* errors_264[] = {NULL};
static primitive_desc primitive_264 = {
	"primitiveNew1:ifFail:", 
	fntype(&primitiveNew1), 
	7667714, 
	signature_264, 
	errors_264
};

static char* signature_265[] = { "Instance", "Behavior", "Boolean"};
static char* errors_265[] = {NULL};
static primitive_desc primitive_265 = {
	"primitiveNew2:ifFail:", 
	fntype(&primitiveNew2), 
	7667714, 
	signature_265, 
	errors_265
};

static char* signature_266[] = { "Instance", "Behavior"};
static char* errors_266[] = { "ReceiverIsIndexable", NULL};
static primitive_desc primitive_266 = {
	"primitiveNew2IfFail:", 
	fntype(&behaviorPrimitives::allocate2), 
	1376257, 
	signature_266, 
	errors_266
};

static char* signature_267[] = { "Instance", "Behavior", "Boolean"};
static char* errors_267[] = {NULL};
static primitive_desc primitive_267 = {
	"primitiveNew3:ifFail:", 
	fntype(&primitiveNew3), 
	7667714, 
	signature_267, 
	errors_267
};

static char* signature_268[] = { "Instance", "Behavior", "Boolean"};
static char* errors_268[] = {NULL};
static primitive_desc primitive_268 = {
	"primitiveNew4:ifFail:", 
	fntype(&primitiveNew4), 
	7667714, 
	signature_268, 
	errors_268
};

static char* signature_269[] = { "Instance", "Behavior", "Boolean"};
static char* errors_269[] = {NULL};
static primitive_desc primitive_269 = {
	"primitiveNew5:ifFail:", 
	fntype(&primitiveNew5), 
	7667714, 
	signature_269, 
	errors_269
};

static char* signature_270[] = { "Instance", "Behavior", "Boolean"};
static char* errors_270[] = {NULL};
static primitive_desc primitive_270 = {
	"primitiveNew6:ifFail:", 
	fntype(&primitiveNew6), 
	7667714, 
	signature_270, 
	errors_270
};

static char* signature_271[] = { "Instance", "Behavior", "Boolean"};
static char* errors_271[] = {NULL};
static primitive_desc primitive_271 = {
	"primitiveNew7:ifFail:", 
	fntype(&primitiveNew7), 
	7667714, 
	signature_271, 
	errors_271
};

static char* signature_272[] = { "Instance", "Behavior", "Boolean"};
static char* errors_272[] = {NULL};
static primitive_desc primitive_272 = {
	"primitiveNew8:ifFail:", 
	fntype(&primitiveNew8), 
	7667714, 
	signature_272, 
	errors_272
};

static char* signature_273[] = { "Instance", "Behavior", "Boolean"};
static char* errors_273[] = {NULL};
static primitive_desc primitive_273 = {
	"primitiveNew9:ifFail:", 
	fntype(&primitiveNew9), 
	7667714, 
	signature_273, 
	errors_273
};

static char* signature_274[] = { "Instance", "Behavior", "Boolean"};
static char* errors_274[] = { "ReceiverIsIndexable", NULL};
static primitive_desc primitive_274 = {
	"primitiveNew:tenured:ifFail:", 
	fntype(&behaviorPrimitives::allocate3), 
	327682, 
	signature_274, 
	errors_274
};

static char* signature_275[] = { "Instance", "Behavior"};
static char* errors_275[] = { "ReceiverIsIndexable", NULL};
static primitive_desc primitive_275 = {
	"primitiveNewIfFail:", 
	fntype(&behaviorPrimitives::allocate), 
	1376257, 
	signature_275, 
	errors_275
};

static char* signature_276[] = { "Boolean", "Object", "Object"};
static char* errors_276[] = {NULL};
static primitive_desc primitive_276 = {
	"primitiveNotEqual:", 
	fntype(&oopPrimitives::not_equal), 
	1572866, 
	signature_276, 
	errors_276
};

static char* signature_277[] = { "Object"};
static char* errors_277[] = { "EmptyQueue", NULL};
static primitive_desc primitive_277 = {
	"primitiveNotificationQueueGetIfFail:", 
	fntype(&systemPrimitives::notificationQueueGet), 
	327680, 
	signature_277, 
	errors_277
};

static char* signature_278[] = { "Object", "Object"};
static char* errors_278[] = {NULL};
static primitive_desc primitive_278 = {
	"primitiveNotificationQueuePut:", 
	fntype(&systemPrimitives::notificationQueuePut), 
	65537, 
	signature_278, 
	errors_278
};

static char* signature_279[] = { "SmallInteger"};
static char* errors_279[] = {NULL};
static primitive_desc primitive_279 = {
	"primitiveNumberOfLookupCacheMisses", 
	fntype(&debugPrimitives::numberOfLookupCacheMisses), 
	65536, 
	signature_279, 
	errors_279
};

static char* signature_280[] = { "SmallInteger"};
static char* errors_280[] = {NULL};
static primitive_desc primitive_280 = {
	"primitiveNumberOfMethodInvocations", 
	fntype(&debugPrimitives::numberOfMethodInvocations), 
	65536, 
	signature_280, 
	errors_280
};

static char* signature_281[] = { "SmallInteger"};
static char* errors_281[] = {NULL};
static primitive_desc primitive_281 = {
	"primitiveNumberOfNMethodInvocations", 
	fntype(&debugPrimitives::numberOfNMethodInvocations), 
	65536, 
	signature_281, 
	errors_281
};

static char* signature_282[] = { "SmallInteger"};
static char* errors_282[] = {NULL};
static primitive_desc primitive_282 = {
	"primitiveNumberOfPrimaryLookupCacheHits", 
	fntype(&debugPrimitives::numberOfPrimaryLookupCacheHits), 
	65536, 
	signature_282, 
	errors_282
};

static char* signature_283[] = { "SmallInteger"};
static char* errors_283[] = {NULL};
static primitive_desc primitive_283 = {
	"primitiveNumberOfSecondaryLookupCacheHits", 
	fntype(&debugPrimitives::numberOfSecondaryLookupCacheHits), 
	65536, 
	signature_283, 
	errors_283
};

static char* signature_284[] = { "SmallInteger"};
static char* errors_284[] = {NULL};
static primitive_desc primitive_284 = {
	"primitiveNurseryFreeSpace", 
	fntype(&systemPrimitives::nurseryFreeSpace), 
	65536, 
	signature_284, 
	errors_284
};

static char* signature_285[] = { "Float"};
static char* errors_285[] = {NULL};
static primitive_desc primitive_285 = {
	"primitiveObjectMemorySize", 
	fntype(&systemPrimitives::object_memory_size), 
	65536, 
	signature_285, 
	errors_285
};

static char* signature_286[] = { "SmallInteger", "Object"};
static char* errors_286[] = {NULL};
static primitive_desc primitive_286 = {
	"primitiveOopSize", 
	fntype(&oopPrimitives::oop_size), 
	1572865, 
	signature_286, 
	errors_286
};

static char* signature_287[] = { "Object", "Object", "Symbol"};
static char* errors_287[] = { "NotFound", NULL};
static primitive_desc primitive_287 = {
	"primitiveOptimizeMethod:ifFail:", 
	fntype(&debugPrimitives::optimizeMethod), 
	1376258, 
	signature_287, 
	errors_287
};

static char* signature_288[] = { "Object", "Object", "CompressedSymbol", "Array"};
static char* errors_288[] = { "SelectorHasWrongNumberOfArguments", NULL};
static primitive_desc primitive_288 = {
	"primitivePerform:arguments:ifFail:", 
	fntype(&oopPrimitives::performArguments), 
	1507331, 
	signature_288, 
	errors_288
};

static char* signature_289[] = { "Object", "Object", "CompressedSymbol"};
static char* errors_289[] = { "SelectorHasWrongNumberOfArguments", NULL};
static primitive_desc primitive_289 = {
	"primitivePerform:ifFail:", 
	fntype(&oopPrimitives::perform), 
	1376258, 
	signature_289, 
	errors_289
};

static char* signature_290[] = { "Object", "Object", "CompressedSymbol", "Object"};
static char* errors_290[] = { "SelectorHasWrongNumberOfArguments", NULL};
static primitive_desc primitive_290 = {
	"primitivePerform:with:ifFail:", 
	fntype(&oopPrimitives::performWith), 
	1507331, 
	signature_290, 
	errors_290
};

static char* signature_291[] = { "Object", "Object", "CompressedSymbol", "Object", "Object"};
static char* errors_291[] = { "SelectorHasWrongNumberOfArguments", NULL};
static primitive_desc primitive_291 = {
	"primitivePerform:with:with:ifFail:", 
	fntype(&oopPrimitives::performWithWith), 
	1507332, 
	signature_291, 
	errors_291
};

static char* signature_292[] = { "Object", "Object", "CompressedSymbol", "Object", "Object", "Object"};
static char* errors_292[] = { "SelectorHasWrongNumberOfArguments", NULL};
static primitive_desc primitive_292 = {
	"primitivePerform:with:with:with:ifFail:", 
	fntype(&oopPrimitives::performWithWithWith), 
	1507333, 
	signature_292, 
	errors_292
};

static char* signature_293[] = { "Self", "Object"};
static char* errors_293[] = {NULL};
static primitive_desc primitive_293 = {
	"primitivePrint", 
	fntype(&oopPrimitives::print), 
	1114113, 
	signature_293, 
	errors_293
};

static char* signature_294[] = { "SmallInteger", "SmallInteger"};
static char* errors_294[] = { "OutOfBounds", NULL};
static primitive_desc primitive_294 = {
	"primitivePrintCharacterIfFail:", 
	fntype(&smiOopPrimitives::printCharacter), 
	1310721, 
	signature_294, 
	errors_294
};

static char* signature_295[] = { "Object", "SmallInteger"};
static char* errors_295[] = {NULL};
static primitive_desc primitive_295 = {
	"primitivePrintInvocationCounterHistogram:ifFail:", 
	fntype(&debugPrimitives::printInvocationCounterHistogram), 
	327681, 
	signature_295, 
	errors_295
};

static char* signature_296[] = { "Object"};
static char* errors_296[] = {NULL};
static primitive_desc primitive_296 = {
	"primitivePrintLayout", 
	fntype(&debugPrimitives::printMemoryLayout), 
	65536, 
	signature_296, 
	errors_296
};

static char* signature_297[] = { "Object"};
static char* errors_297[] = {NULL};
static primitive_desc primitive_297 = {
	"primitivePrintLookupCacheStatistics", 
	fntype(&debugPrimitives::printLookupCacheStatistics), 
	65536, 
	signature_297, 
	errors_297
};

static char* signature_298[] = { "Object"};
static char* errors_298[] = {NULL};
static primitive_desc primitive_298 = {
	"primitivePrintMemory", 
	fntype(&systemPrimitives::print_memory), 
	65536, 
	signature_298, 
	errors_298
};

static char* signature_299[] = { "Behavior", "Behavior", "ByteArray"};
static char* errors_299[] = {NULL};
static primitive_desc primitive_299 = {
	"primitivePrintMethod:ifFail:", 
	fntype(&behaviorPrimitives::printMethod), 
	1376258, 
	signature_299, 
	errors_299
};

static char* signature_300[] = { "Object", "Object", "Symbol"};
static char* errors_300[] = { "NotFound", NULL};
static primitive_desc primitive_300 = {
	"primitivePrintMethodCodes:ifFail:", 
	fntype(&debugPrimitives::printMethodCodes), 
	1376258, 
	signature_300, 
	errors_300
};

static char* signature_301[] = { "Object", "SmallInteger"};
static char* errors_301[] = {NULL};
static primitive_desc primitive_301 = {
	"primitivePrintNMethodCounterHistogram:ifFail:", 
	fntype(&debugPrimitives::printNMethodCounterHistogram), 
	327681, 
	signature_301, 
	errors_301
};

static char* signature_302[] = { "Object"};
static char* errors_302[] = {NULL};
static primitive_desc primitive_302 = {
	"primitivePrintObjectHistogram", 
	fntype(&debugPrimitives::printObjectHistogram), 
	65536, 
	signature_302, 
	errors_302
};

static char* signature_303[] = { "Object"};
static char* errors_303[] = {NULL};
static primitive_desc primitive_303 = {
	"primitivePrintPrimitiveCounters", 
	fntype(&debugPrimitives::printPrimitiveCounters), 
	65536, 
	signature_303, 
	errors_303
};

static char* signature_304[] = { "Object"};
static char* errors_304[] = {NULL};
static primitive_desc primitive_304 = {
	"primitivePrintPrimitiveTable", 
	fntype(&systemPrimitives::printPrimitiveTable), 
	65536, 
	signature_304, 
	errors_304
};

static char* signature_305[] = { "Self", "Object"};
static char* errors_305[] = {NULL};
static primitive_desc primitive_305 = {
	"primitivePrintValue", 
	fntype(&oopPrimitives::printValue), 
	1114113, 
	signature_305, 
	errors_305
};

static char* signature_306[] = { "Object"};
static char* errors_306[] = {NULL};
static primitive_desc primitive_306 = {
	"primitivePrintZone", 
	fntype(&systemPrimitives::print_zone), 
	65536, 
	signature_306, 
	errors_306
};

static char* signature_307[] = { "Object"};
static char* errors_307[] = {NULL};
static primitive_desc primitive_307 = {
	"primitiveProcessActiveProcess", 
	fntype(&processOopPrimitives::activeProcess), 
	65536, 
	signature_307, 
	errors_307
};

static char* signature_308[] = { "Process", "Process class", "BlockWithoutArguments"};
static char* errors_308[] = { "ProcessAllocationFailed", NULL};
static primitive_desc primitive_308 = {
	"primitiveProcessCreate:ifFail:", 
	fntype(&processOopPrimitives::create), 
	1376258, 
	signature_308, 
	errors_308
};

static char* signature_309[] = { "Process"};
static char* errors_309[] = {NULL};
static primitive_desc primitive_309 = {
	"primitiveProcessEnterCritical", 
	fntype(&processOopPrimitives::enter_critical), 
	65536, 
	signature_309, 
	errors_309
};

static char* signature_310[] = { "Process"};
static char* errors_310[] = {NULL};
static primitive_desc primitive_310 = {
	"primitiveProcessLeaveCritical", 
	fntype(&processOopPrimitives::leave_critical), 
	65536, 
	signature_310, 
	errors_310
};

static char* signature_311[] = { "Boolean", "Process", "SmallInteger"};
static char* errors_311[] = {NULL};
static primitive_desc primitive_311 = {
	"primitiveProcessSchedulerWait:ifFail:", 
	fntype(&processOopPrimitives::scheduler_wait), 
	1376258, 
	signature_311, 
	errors_311
};

static char* signature_312[] = { "Symbol", "Process", "Symbol", "Activation", "Object"};
static char* errors_312[] = { "InScheduler", "Dead", NULL};
static primitive_desc primitive_312 = {
	"primitiveProcessSetMode:activation:returnValue:ifFail:", 
	fntype(&processOopPrimitives::set_mode), 
	1376260, 
	signature_312, 
	errors_312
};

static char* signature_313[] = { "IndexedInstanceVariables", "Process", "SmallInteger"};
static char* errors_313[] = {NULL};
static primitive_desc primitive_313 = {
	"primitiveProcessStackLimit:ifFail:", 
	fntype(&processOopPrimitives::stack), 
	1376258, 
	signature_313, 
	errors_313
};

static char* signature_314[] = { "Object", "Process"};
static char* errors_314[] = { "NotInScheduler", "ProcessCannotContinue", "Dead", NULL};
static primitive_desc primitive_314 = {
	"primitiveProcessStartEvaluator:ifFail:", 
	fntype(&processOopPrimitives::start_evaluator), 
	327681, 
	signature_314, 
	errors_314
};

static char* signature_315[] = { "Symbol", "Process"};
static char* errors_315[] = {NULL};
static primitive_desc primitive_315 = {
	"primitiveProcessStatus", 
	fntype(&processOopPrimitives::status), 
	1114113, 
	signature_315, 
	errors_315
};

static char* signature_316[] = { "Process"};
static char* errors_316[] = {NULL};
static primitive_desc primitive_316 = {
	"primitiveProcessStop", 
	fntype(&processOopPrimitives::stop), 
	65536, 
	signature_316, 
	errors_316
};

static char* signature_317[] = { "Float", "Process"};
static char* errors_317[] = {NULL};
static primitive_desc primitive_317 = {
	"primitiveProcessSystemTime", 
	fntype(&processOopPrimitives::user_time), 
	1114113, 
	signature_317, 
	errors_317
};

static char* signature_318[] = { "Self", "Process"};
static char* errors_318[] = { "Dead", NULL};
static primitive_desc primitive_318 = {
	"primitiveProcessTerminateIfFail:", 
	fntype(&processOopPrimitives::terminate), 
	1507329, 
	signature_318, 
	errors_318
};

static char* signature_319[] = { "Self", "Process", "SmallInteger"};
static char* errors_319[] = {NULL};
static primitive_desc primitive_319 = {
	"primitiveProcessTraceStack:ifFail:", 
	fntype(&processOopPrimitives::trace_stack), 
	1376258, 
	signature_319, 
	errors_319
};

static char* signature_320[] = { "Object", "Process"};
static char* errors_320[] = { "NotInScheduler", "ProcessCannotContinue", "Dead", NULL};
static primitive_desc primitive_320 = {
	"primitiveProcessTransferTo:ifFail:", 
	fntype(&processOopPrimitives::transferTo), 
	327681, 
	signature_320, 
	errors_320
};

static char* signature_321[] = { "Float", "Process"};
static char* errors_321[] = {NULL};
static primitive_desc primitive_321 = {
	"primitiveProcessUserTime", 
	fntype(&processOopPrimitives::user_time), 
	1114113, 
	signature_321, 
	errors_321
};

static char* signature_322[] = { "Process"};
static char* errors_322[] = {NULL};
static primitive_desc primitive_322 = {
	"primitiveProcessYield", 
	fntype(&processOopPrimitives::yield), 
	65536, 
	signature_322, 
	errors_322
};

static char* signature_323[] = { "Process"};
static char* errors_323[] = {NULL};
static primitive_desc primitive_323 = {
	"primitiveProcessYieldInCritical", 
	fntype(&processOopPrimitives::yield_in_critical), 
	65536, 
	signature_323, 
	errors_323
};

static char* signature_324[] = { "SmallInteger", "Proxy", "SmallInteger"};
static char* errors_324[] = {NULL};
static primitive_desc primitive_324 = {
	"primitiveProxyByteAt:ifFail:", 
	fntype(&proxyOopPrimitives::byteAt), 
	5570562, 
	signature_324, 
	errors_324
};

static char* signature_325[] = { "SmallInteger", "Proxy", "SmallInteger", "SmallInteger"};
static char* errors_325[] = {NULL};
static primitive_desc primitive_325 = {
	"primitiveProxyByteAt:put:ifFail:", 
	fntype(&proxyOopPrimitives::byteAtPut), 
	5570563, 
	signature_325, 
	errors_325
};

static char* signature_326[] = { "Self", "Proxy", "SmallInteger"};
static char* errors_326[] = {NULL};
static primitive_desc primitive_326 = {
	"primitiveProxyCalloc:ifFail:", 
	fntype(&proxyOopPrimitives::calloc), 
	1376258, 
	signature_326, 
	errors_326
};

static char* signature_327[] = { "SmallInteger", "Proxy", "SmallInteger"};
static char* errors_327[] = {NULL};
static primitive_desc primitive_327 = {
	"primitiveProxyDoubleByteAt:ifFail:", 
	fntype(&proxyOopPrimitives::doubleByteAt), 
	1376258, 
	signature_327, 
	errors_327
};

static char* signature_328[] = { "SmallInteger", "Proxy", "SmallInteger", "SmallInteger"};
static char* errors_328[] = {NULL};
static primitive_desc primitive_328 = {
	"primitiveProxyDoubleByteAt:put:ifFail:", 
	fntype(&proxyOopPrimitives::doubleByteAtPut), 
	1376259, 
	signature_328, 
	errors_328
};

static char* signature_329[] = { "Float", "Proxy", "SmallInteger"};
static char* errors_329[] = {NULL};
static primitive_desc primitive_329 = {
	"primitiveProxyDoublePrecisionFloatAt:ifFail:", 
	fntype(&proxyOopPrimitives::doublePrecisionFloatAt), 
	1376258, 
	signature_329, 
	errors_329
};

static char* signature_330[] = { "Self", "Proxy", "SmallInteger", "Float"};
static char* errors_330[] = { "ConversionFailed", NULL};
static primitive_desc primitive_330 = {
	"primitiveProxyDoublePrecisionFloatAt:put:ifFail:", 
	fntype(&proxyOopPrimitives::doublePrecisionFloatAtPut), 
	1376259, 
	signature_330, 
	errors_330
};

static char* signature_331[] = { "Self", "Proxy"};
static char* errors_331[] = {NULL};
static primitive_desc primitive_331 = {
	"primitiveProxyFree", 
	fntype(&proxyOopPrimitives::free), 
	1114113, 
	signature_331, 
	errors_331
};

static char* signature_332[] = { "SmallInteger", "Proxy"};
static char* errors_332[] = {NULL};
static primitive_desc primitive_332 = {
	"primitiveProxyGetHigh", 
	fntype(&proxyOopPrimitives::getHigh), 
	1114113, 
	signature_332, 
	errors_332
};

static char* signature_333[] = { "SmallInteger", "Proxy"};
static char* errors_333[] = { "ConversionFailed", NULL};
static primitive_desc primitive_333 = {
	"primitiveProxyGetIfFail:", 
	fntype(&proxyOopPrimitives::getSmi), 
	1376257, 
	signature_333, 
	errors_333
};

static char* signature_334[] = { "SmallInteger", "Proxy"};
static char* errors_334[] = {NULL};
static primitive_desc primitive_334 = {
	"primitiveProxyGetLow", 
	fntype(&proxyOopPrimitives::getLow), 
	1114113, 
	signature_334, 
	errors_334
};

static char* signature_335[] = { "Boolean", "Proxy"};
static char* errors_335[] = {NULL};
static primitive_desc primitive_335 = {
	"primitiveProxyIsAllOnes", 
	fntype(&proxyOopPrimitives::isAllOnes), 
	1114113, 
	signature_335, 
	errors_335
};

static char* signature_336[] = { "Boolean", "Proxy"};
static char* errors_336[] = {NULL};
static primitive_desc primitive_336 = {
	"primitiveProxyIsNull", 
	fntype(&proxyOopPrimitives::isNull), 
	1114113, 
	signature_336, 
	errors_336
};

static char* signature_337[] = { "Self", "Proxy", "SmallInteger"};
static char* errors_337[] = {NULL};
static primitive_desc primitive_337 = {
	"primitiveProxyMalloc:ifFail:", 
	fntype(&proxyOopPrimitives::malloc), 
	1376258, 
	signature_337, 
	errors_337
};

static char* signature_338[] = { "Proxy", "Proxy", "SmallInteger", "Proxy"};
static char* errors_338[] = {NULL};
static primitive_desc primitive_338 = {
	"primitiveProxyProxyAt:put:ifFail:", 
	fntype(&proxyOopPrimitives::proxyAtPut), 
	1376259, 
	signature_338, 
	errors_338
};

static char* signature_339[] = { "Proxy", "Proxy", "SmallInteger", "Proxy"};
static char* errors_339[] = {NULL};
static primitive_desc primitive_339 = {
	"primitiveProxyProxyAt:result:ifFail:", 
	fntype(&proxyOopPrimitives::proxyAt), 
	1376259, 
	signature_339, 
	errors_339
};

static char* signature_340[] = { "Self", "Proxy", "SmallInteger|Proxy"};
static char* errors_340[] = {NULL};
static primitive_desc primitive_340 = {
	"primitiveProxySet:ifFail:", 
	fntype(&proxyOopPrimitives::set), 
	1376258, 
	signature_340, 
	errors_340
};

static char* signature_341[] = { "Self", "Proxy", "SmallInteger", "SmallInteger"};
static char* errors_341[] = {NULL};
static primitive_desc primitive_341 = {
	"primitiveProxySetHigh:low:ifFail:", 
	fntype(&proxyOopPrimitives::setHighLow), 
	1376259, 
	signature_341, 
	errors_341
};

static char* signature_342[] = { "Float", "Proxy", "SmallInteger"};
static char* errors_342[] = {NULL};
static primitive_desc primitive_342 = {
	"primitiveProxySinglePrecisionFloatAt:ifFail:", 
	fntype(&proxyOopPrimitives::singlePrecisionFloatAt), 
	1376258, 
	signature_342, 
	errors_342
};

static char* signature_343[] = { "Self", "Proxy", "SmallInteger", "Float"};
static char* errors_343[] = { "ConversionFailed", NULL};
static primitive_desc primitive_343 = {
	"primitiveProxySinglePrecisionFloatAt:put:ifFail:", 
	fntype(&proxyOopPrimitives::singlePrecisionFloatAtPut), 
	1376259, 
	signature_343, 
	errors_343
};

static char* signature_344[] = { "SmallInteger", "Proxy", "SmallInteger"};
static char* errors_344[] = { "ConversionFailed", NULL};
static primitive_desc primitive_344 = {
	"primitiveProxySmiAt:ifFail:", 
	fntype(&proxyOopPrimitives::smiAt), 
	1376258, 
	signature_344, 
	errors_344
};

static char* signature_345[] = { "SmallInteger", "Proxy", "SmallInteger", "SmallInteger"};
static char* errors_345[] = {NULL};
static primitive_desc primitive_345 = {
	"primitiveProxySmiAt:put:ifFail:", 
	fntype(&proxyOopPrimitives::smiAtPut), 
	1376259, 
	signature_345, 
	errors_345
};

static char* signature_346[] = { "Proxy", "Proxy", "SmallInteger", "Proxy"};
static char* errors_346[] = {NULL};
static primitive_desc primitive_346 = {
	"primitiveProxySubProxyAt:result:ifFail:", 
	fntype(&proxyOopPrimitives::subProxyAt), 
	1376259, 
	signature_346, 
	errors_346
};

static char* signature_347[] = { "BottomType"};
static char* errors_347[] = {NULL};
static primitive_desc primitive_347 = {
	"primitiveQuit", 
	fntype(&systemPrimitives::quit), 
	65536, 
	signature_347, 
	errors_347
};

static char* signature_348[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_348[] = { "NotImplementedYet", NULL};
static primitive_desc primitive_348 = {
	"primitiveQuo:ifFail:", 
	fntype(&smiOopPrimitives_quo), 
	6029826, 
	signature_348, 
	errors_348
};

static char* signature_349[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_349[] = {NULL};
static primitive_desc primitive_349 = {
	"primitiveRawBitShift:ifFail:", 
	fntype(&smiOopPrimitives::rawBitShift), 
	6029826, 
	signature_349, 
	errors_349
};

static char* signature_350[] = { "IndexedInstanceVariables", "Object", "SmallInteger"};
static char* errors_350[] = {NULL};
static primitive_desc primitive_350 = {
	"primitiveReferencesTo:limit:ifFail:", 
	fntype(&systemPrimitives::references_to), 
	327682, 
	signature_350, 
	errors_350
};

static char* signature_351[] = { "IndexedInstanceVariables", "Class", "SmallInteger"};
static char* errors_351[] = {NULL};
static primitive_desc primitive_351 = {
	"primitiveReferencesToInstancesOf:limit:ifFail:", 
	fntype(&systemPrimitives::references_to_instances_of), 
	327682, 
	signature_351, 
	errors_351
};

static char* signature_352[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_352[] = { "DivisionByZero", NULL};
static primitive_desc primitive_352 = {
	"primitiveRemainder:ifFail:", 
	fntype(&smiOopPrimitives_remainder), 
	6029826, 
	signature_352, 
	errors_352
};

static char* signature_353[] = { "BottomType", "BlockWithoutArguments"};
static char* errors_353[] = {NULL};
static primitive_desc primitive_353 = {
	"primitiveRepeat", 
	fntype(&blockRepeat), 
	1245185, 
	signature_353, 
	errors_353
};

static char* signature_354[] = { "Self", "Object"};
static char* errors_354[] = {NULL};
static primitive_desc primitive_354 = {
	"primitiveScavenge", 
	fntype(&systemPrimitives::scavenge), 
	1114113, 
	signature_354, 
	errors_354
};

static char* signature_355[] = { "Object", "SmallInteger"};
static char* errors_355[] = {NULL};
static primitive_desc primitive_355 = {
	"primitiveSetInterpreterInvocationCounterLimitTo:ifFail:", 
	fntype(&debugPrimitives::setInterpreterInvocationCounterLimit), 
	327681, 
	signature_355, 
	errors_355
};

static char* signature_356[] = { "Object", "Object"};
static char* errors_356[] = { "ReceiverHasWrongType", NULL};
static primitive_desc primitive_356 = {
	"primitiveShallowCopyIfFail:", 
	fntype(&oopPrimitives::shallowCopy), 
	1376257, 
	signature_356, 
	errors_356
};

static char* signature_357[] = { "Object", "SmallInteger"};
static char* errors_357[] = {NULL};
static primitive_desc primitive_357 = {
	"primitiveShrinkMemory:", 
	fntype(&systemPrimitives::shrinkMemory), 
	65537, 
	signature_357, 
	errors_357
};

static char* signature_358[] = { "SmallInteger"};
static char* errors_358[] = {NULL};
static primitive_desc primitive_358 = {
	"primitiveSizeOfOop", 
	fntype(&systemPrimitives::oopSize), 
	65536, 
	signature_358, 
	errors_358
};

static char* signature_359[] = { "IndexedInstanceVariables"};
static char* errors_359[] = {NULL};
static primitive_desc primitive_359 = {
	"primitiveSlidingSystemAverageIfFail:", 
	fntype(&systemPrimitives::sliding_system_average), 
	327680, 
	signature_359, 
	errors_359
};

static char* signature_360[] = { "Boolean", "SmallInteger", "SmallInteger"};
static char* errors_360[] = {NULL};
static primitive_desc primitive_360 = {
	"primitiveSmallIntegerEqual:ifFail:", 
	fntype(&smiOopPrimitives::equal), 
	6029570, 
	signature_360, 
	errors_360
};

static char* signature_361[] = { "SmallInteger", "Symbol"};
static char* errors_361[] = { "NotFound", NULL};
static primitive_desc primitive_361 = {
	"primitiveSmallIntegerFlagAt:ifFail:", 
	fntype(&debugPrimitives::smiAt), 
	327681, 
	signature_361, 
	errors_361
};

static char* signature_362[] = { "Boolean", "Symbol", "Boolean"};
static char* errors_362[] = { "NotFound", NULL};
static primitive_desc primitive_362 = {
	"primitiveSmallIntegerFlagAt:put:ifFail:", 
	fntype(&debugPrimitives::smiAtPut), 
	327682, 
	signature_362, 
	errors_362
};

static char* signature_363[] = { "Boolean", "SmallInteger", "SmallInteger"};
static char* errors_363[] = {NULL};
static primitive_desc primitive_363 = {
	"primitiveSmallIntegerNotEqual:ifFail:", 
	fntype(&smiOopPrimitives::notEqual), 
	6029570, 
	signature_363, 
	errors_363
};

static char* signature_364[] = { "IndexedInstanceVariables"};
static char* errors_364[] = {NULL};
static primitive_desc primitive_364 = {
	"primitiveSmalltalkArray", 
	fntype(&systemPrimitives::smalltalk_array), 
	65536, 
	signature_364, 
	errors_364
};

static char* signature_365[] = { "GlobalAssociation", "Symbol", "Object"};
static char* errors_365[] = {NULL};
static primitive_desc primitive_365 = {
	"primitiveSmalltalkAt:Put:ifFail:", 
	fntype(&systemPrimitives::smalltalk_at_put), 
	327682, 
	signature_365, 
	errors_365
};

static char* signature_366[] = { "GlobalAssociation", "SmallInteger"};
static char* errors_366[] = { "OutOfBounds", NULL};
static primitive_desc primitive_366 = {
	"primitiveSmalltalkAt:ifFail:", 
	fntype(&systemPrimitives::smalltalk_at), 
	327681, 
	signature_366, 
	errors_366
};

static char* signature_367[] = { "GlobalAssociation", "SmallInteger"};
static char* errors_367[] = {NULL};
static primitive_desc primitive_367 = {
	"primitiveSmalltalkRemoveAt:ifFail:", 
	fntype(&systemPrimitives::smalltalk_remove_at), 
	327681, 
	signature_367, 
	errors_367
};

static char* signature_368[] = { "SmallInteger"};
static char* errors_368[] = {NULL};
static primitive_desc primitive_368 = {
	"primitiveSmalltalkSize", 
	fntype(&systemPrimitives::smalltalk_size), 
	65536, 
	signature_368, 
	errors_368
};

static char* signature_369[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_369[] = { "Overflow", NULL};
static primitive_desc primitive_369 = {
	"primitiveSubtract:ifFail:", 
	fntype(&smiOopPrimitives_subtract), 
	6029826, 
	signature_369, 
	errors_369
};

static char* signature_370[] = { "Behavior|Nil", "Behavior"};
static char* errors_370[] = {NULL};
static primitive_desc primitive_370 = {
	"primitiveSuperclass", 
	fntype(&behaviorPrimitives::superclass), 
	1114113, 
	signature_370, 
	errors_370
};

static char* signature_371[] = { "Behavior|Nil", "Behavior"};
static char* errors_371[] = {NULL};
static primitive_desc primitive_371 = {
	"primitiveSuperclassOf:ifFail:", 
	fntype(&behaviorPrimitives::superclass_of), 
	327681, 
	signature_371, 
	errors_371
};

static char* signature_372[] = { "Float"};
static char* errors_372[] = {NULL};
static primitive_desc primitive_372 = {
	"primitiveSystemTime", 
	fntype(&systemPrimitives::systemTime), 
	65536, 
	signature_372, 
	errors_372
};

static char* signature_373[] = { "Object"};
static char* errors_373[] = {NULL};
static primitive_desc primitive_373 = {
	"primitiveTimerPrintBuffer", 
	fntype(&debugPrimitives::timerPrintBuffer), 
	65536, 
	signature_373, 
	errors_373
};

static char* signature_374[] = { "Object"};
static char* errors_374[] = {NULL};
static primitive_desc primitive_374 = {
	"primitiveTimerStart", 
	fntype(&debugPrimitives::timerStart), 
	65536, 
	signature_374, 
	errors_374
};

static char* signature_375[] = { "Object"};
static char* errors_375[] = {NULL};
static primitive_desc primitive_375 = {
	"primitiveTimerStop", 
	fntype(&debugPrimitives::timerStop), 
	65536, 
	signature_375, 
	errors_375
};

static char* signature_376[] = { "Object"};
static char* errors_376[] = {NULL};
static primitive_desc primitive_376 = {
	"primitiveTraceStack", 
	fntype(&systemPrimitives::traceStack), 
	65536, 
	signature_376, 
	errors_376
};

static char* signature_377[] = { "Object", "BlockWithoutArguments", "BlockWithoutArguments"};
static char* errors_377[] = {NULL};
static primitive_desc primitive_377 = {
	"primitiveUnwindProtect:ifFail:", 
	fntype(&unwindprotect), 
	1507330, 
	signature_377, 
	errors_377
};

static char* signature_378[] = { "Float"};
static char* errors_378[] = {NULL};
static primitive_desc primitive_378 = {
	"primitiveUserTime", 
	fntype(&systemPrimitives::userTime), 
	65536, 
	signature_378, 
	errors_378
};

static char* signature_379[] = { "Object", "BlockWithoutArguments"};
static char* errors_379[] = {NULL};
static primitive_desc primitive_379 = {
	"primitiveValue", 
	fntype(&primitiveValue0), 
	5441537, 
	signature_379, 
	errors_379
};

static char* signature_380[] = { "Object", "BlockWithOneArgument", "Object"};
static char* errors_380[] = {NULL};
static primitive_desc primitive_380 = {
	"primitiveValue:", 
	fntype(&primitiveValue1), 
	5441538, 
	signature_380, 
	errors_380
};

static char* signature_381[] = { "Object", "BlockWithTwoArguments", "Object", "Object"};
static char* errors_381[] = {NULL};
static primitive_desc primitive_381 = {
	"primitiveValue:value:", 
	fntype(&primitiveValue2), 
	5441539, 
	signature_381, 
	errors_381
};

static char* signature_382[] = { "Object", "BlockWithThreeArguments", "Object", "Object", "Object"};
static char* errors_382[] = {NULL};
static primitive_desc primitive_382 = {
	"primitiveValue:value:value:", 
	fntype(&primitiveValue3), 
	5441540, 
	signature_382, 
	errors_382
};

static char* signature_383[] = { "Object", "BlockWithFourArguments", "Object", "Object", "Object", "Object"};
static char* errors_383[] = {NULL};
static primitive_desc primitive_383 = {
	"primitiveValue:value:value:value:", 
	fntype(&primitiveValue4), 
	5441541, 
	signature_383, 
	errors_383
};

static char* signature_384[] = { "Object", "BlockWithFiveArguments", "Object", "Object", "Object", "Object", "Object"};
static char* errors_384[] = {NULL};
static primitive_desc primitive_384 = {
	"primitiveValue:value:value:value:value:", 
	fntype(&primitiveValue5), 
	5441542, 
	signature_384, 
	errors_384
};

static char* signature_385[] = { "Object", "BlockWithSixArguments", "Object", "Object", "Object", "Object", "Object", "Object"};
static char* errors_385[] = {NULL};
static primitive_desc primitive_385 = {
	"primitiveValue:value:value:value:value:value:", 
	fntype(&primitiveValue6), 
	5441543, 
	signature_385, 
	errors_385
};

static char* signature_386[] = { "Object", "BlockWithSevenArguments", "Object", "Object", "Object", "Object", "Object", "Object", "Object"};
static char* errors_386[] = {NULL};
static primitive_desc primitive_386 = {
	"primitiveValue:value:value:value:value:value:value:", 
	fntype(&primitiveValue7), 
	5441544, 
	signature_386, 
	errors_386
};

static char* signature_387[] = { "Object", "BlockWithEightArguments", "Object", "Object", "Object", "Object", "Object", "Object", "Object", "Object"};
static char* errors_387[] = {NULL};
static primitive_desc primitive_387 = {
	"primitiveValue:value:value:value:value:value:value:value:", 
	fntype(&primitiveValue8), 
	5441545, 
	signature_387, 
	errors_387
};

static char* signature_388[] = { "Object", "BlockWithNineArguments", "Object", "Object", "Object", "Object", "Object", "Object", "Object", "Object", "Object"};
static char* errors_388[] = {NULL};
static primitive_desc primitive_388 = {
	"primitiveValue:value:value:value:value:value:value:value:value:", 
	fntype(&primitiveValue9), 
	5441546, 
	signature_388, 
	errors_388
};

static char* signature_389[] = { "Object"};
static char* errors_389[] = {NULL};
static primitive_desc primitive_389 = {
	"primitiveVerify", 
	fntype(&debugPrimitives::verify), 
	65536, 
	signature_389, 
	errors_389
};

static char* signature_390[] = { "Proxy", "Proxy"};
static char* errors_390[] = {NULL};
static primitive_desc primitive_390 = {
	"primitiveWindowsHInstance:ifFail:", 
	fntype(&systemPrimitives::windowsHInstance), 
	327681, 
	signature_390, 
	errors_390
};

static char* signature_391[] = { "Proxy", "Proxy"};
static char* errors_391[] = {NULL};
static primitive_desc primitive_391 = {
	"primitiveWindowsHPrevInstance:ifFail:", 
	fntype(&systemPrimitives::windowsHPrevInstance), 
	327681, 
	signature_391, 
	errors_391
};

static char* signature_392[] = { "Object"};
static char* errors_392[] = {NULL};
static primitive_desc primitive_392 = {
	"primitiveWindowsNCmdShow", 
	fntype(&systemPrimitives::windowsNCmdShow), 
	65536, 
	signature_392, 
	errors_392
};

static char* signature_393[] = { "Object", "String"};
static char* errors_393[] = {NULL};
static primitive_desc primitive_393 = {
	"primitiveWriteSnapshot:", 
	fntype(&systemPrimitives::writeSnapshot), 
	65537, 
	signature_393, 
	errors_393
};

static int size_of_primitive_table = 394; 
static primitive_desc* primitive_table[] = { 
  &primitive_0,
  &primitive_1,
  &primitive_2,
  &primitive_3,
  &primitive_4,
  &primitive_5,
  &primitive_6,
  &primitive_7,
  &primitive_8,
  &primitive_9,
  &primitive_10,
  &primitive_11,
  &primitive_12,
  &primitive_13,
  &primitive_14,
  &primitive_15,
  &primitive_16,
  &primitive_17,
  &primitive_18,
  &primitive_19,
  &primitive_20,
  &primitive_21,
  &primitive_22,
  &primitive_23,
  &primitive_24,
  &primitive_25,
  &primitive_26,
  &primitive_27,
  &primitive_28,
  &primitive_29,
  &primitive_30,
  &primitive_31,
  &primitive_32,
  &primitive_33,
  &primitive_34,
  &primitive_35,
  &primitive_36,
  &primitive_37,
  &primitive_38,
  &primitive_39,
  &primitive_40,
  &primitive_41,
  &primitive_42,
  &primitive_43,
  &primitive_44,
  &primitive_45,
  &primitive_46,
  &primitive_47,
  &primitive_48,
  &primitive_49,
  &primitive_50,
  &primitive_51,
  &primitive_52,
  &primitive_53,
  &primitive_54,
  &primitive_55,
  &primitive_56,
  &primitive_57,
  &primitive_58,
  &primitive_59,
  &primitive_60,
  &primitive_61,
  &primitive_62,
  &primitive_63,
  &primitive_64,
  &primitive_65,
  &primitive_66,
  &primitive_67,
  &primitive_68,
  &primitive_69,
  &primitive_70,
  &primitive_71,
  &primitive_72,
  &primitive_73,
  &primitive_74,
  &primitive_75,
  &primitive_76,
  &primitive_77,
  &primitive_78,
  &primitive_79,
  &primitive_80,
  &primitive_81,
  &primitive_82,
  &primitive_83,
  &primitive_84,
  &primitive_85,
  &primitive_86,
  &primitive_87,
  &primitive_88,
  &primitive_89,
  &primitive_90,
  &primitive_91,
  &primitive_92,
  &primitive_93,
  &primitive_94,
  &primitive_95,
  &primitive_96,
  &primitive_97,
  &primitive_98,
  &primitive_99,
  &primitive_100,
  &primitive_101,
  &primitive_102,
  &primitive_103,
  &primitive_104,
  &primitive_105,
  &primitive_106,
  &primitive_107,
  &primitive_108,
  &primitive_109,
  &primitive_110,
  &primitive_111,
  &primitive_112,
  &primitive_113,
  &primitive_114,
  &primitive_115,
  &primitive_116,
  &primitive_117,
  &primitive_118,
  &primitive_119,
  &primitive_120,
  &primitive_121,
  &primitive_122,
  &primitive_123,
  &primitive_124,
  &primitive_125,
  &primitive_126,
  &primitive_127,
  &primitive_128,
  &primitive_129,
  &primitive_130,
  &primitive_131,
  &primitive_132,
  &primitive_133,
  &primitive_134,
  &primitive_135,
  &primitive_136,
  &primitive_137,
  &primitive_138,
  &primitive_139,
  &primitive_140,
  &primitive_141,
  &primitive_142,
  &primitive_143,
  &primitive_144,
  &primitive_145,
  &primitive_146,
  &primitive_147,
  &primitive_148,
  &primitive_149,
  &primitive_150,
  &primitive_151,
  &primitive_152,
  &primitive_153,
  &primitive_154,
  &primitive_155,
  &primitive_156,
  &primitive_157,
  &primitive_158,
  &primitive_159,
  &primitive_160,
  &primitive_161,
  &primitive_162,
  &primitive_163,
  &primitive_164,
  &primitive_165,
  &primitive_166,
  &primitive_167,
  &primitive_168,
  &primitive_169,
  &primitive_170,
  &primitive_171,
  &primitive_172,
  &primitive_173,
  &primitive_174,
  &primitive_175,
  &primitive_176,
  &primitive_177,
  &primitive_178,
  &primitive_179,
  &primitive_180,
  &primitive_181,
  &primitive_182,
  &primitive_183,
  &primitive_184,
  &primitive_185,
  &primitive_186,
  &primitive_187,
  &primitive_188,
  &primitive_189,
  &primitive_190,
  &primitive_191,
  &primitive_192,
  &primitive_193,
  &primitive_194,
  &primitive_195,
  &primitive_196,
  &primitive_197,
  &primitive_198,
  &primitive_199,
  &primitive_200,
  &primitive_201,
  &primitive_202,
  &primitive_203,
  &primitive_204,
  &primitive_205,
  &primitive_206,
  &primitive_207,
  &primitive_208,
  &primitive_209,
  &primitive_210,
  &primitive_211,
  &primitive_212,
  &primitive_213,
  &primitive_214,
  &primitive_215,
  &primitive_216,
  &primitive_217,
  &primitive_218,
  &primitive_219,
  &primitive_220,
  &primitive_221,
  &primitive_222,
  &primitive_223,
  &primitive_224,
  &primitive_225,
  &primitive_226,
  &primitive_227,
  &primitive_228,
  &primitive_229,
  &primitive_230,
  &primitive_231,
  &primitive_232,
  &primitive_233,
  &primitive_234,
  &primitive_235,
  &primitive_236,
  &primitive_237,
  &primitive_238,
  &primitive_239,
  &primitive_240,
  &primitive_241,
  &primitive_242,
  &primitive_243,
  &primitive_244,
  &primitive_245,
  &primitive_246,
  &primitive_247,
  &primitive_248,
  &primitive_249,
  &primitive_250,
  &primitive_251,
  &primitive_252,
  &primitive_253,
  &primitive_254,
  &primitive_255,
  &primitive_256,
  &primitive_257,
  &primitive_258,
  &primitive_259,
  &primitive_260,
  &primitive_261,
  &primitive_262,
  &primitive_263,
  &primitive_264,
  &primitive_265,
  &primitive_266,
  &primitive_267,
  &primitive_268,
  &primitive_269,
  &primitive_270,
  &primitive_271,
  &primitive_272,
  &primitive_273,
  &primitive_274,
  &primitive_275,
  &primitive_276,
  &primitive_277,
  &primitive_278,
  &primitive_279,
  &primitive_280,
  &primitive_281,
  &primitive_282,
  &primitive_283,
  &primitive_284,
  &primitive_285,
  &primitive_286,
  &primitive_287,
  &primitive_288,
  &primitive_289,
  &primitive_290,
  &primitive_291,
  &primitive_292,
  &primitive_293,
  &primitive_294,
  &primitive_295,
  &primitive_296,
  &primitive_297,
  &primitive_298,
  &primitive_299,
  &primitive_300,
  &primitive_301,
  &primitive_302,
  &primitive_303,
  &primitive_304,
  &primitive_305,
  &primitive_306,
  &primitive_307,
  &primitive_308,
  &primitive_309,
  &primitive_310,
  &primitive_311,
  &primitive_312,
  &primitive_313,
  &primitive_314,
  &primitive_315,
  &primitive_316,
  &primitive_317,
  &primitive_318,
  &primitive_319,
  &primitive_320,
  &primitive_321,
  &primitive_322,
  &primitive_323,
  &primitive_324,
  &primitive_325,
  &primitive_326,
  &primitive_327,
  &primitive_328,
  &primitive_329,
  &primitive_330,
  &primitive_331,
  &primitive_332,
  &primitive_333,
  &primitive_334,
  &primitive_335,
  &primitive_336,
  &primitive_337,
  &primitive_338,
  &primitive_339,
  &primitive_340,
  &primitive_341,
  &primitive_342,
  &primitive_343,
  &primitive_344,
  &primitive_345,
  &primitive_346,
  &primitive_347,
  &primitive_348,
  &primitive_349,
  &primitive_350,
  &primitive_351,
  &primitive_352,
  &primitive_353,
  &primitive_354,
  &primitive_355,
  &primitive_356,
  &primitive_357,
  &primitive_358,
  &primitive_359,
  &primitive_360,
  &primitive_361,
  &primitive_362,
  &primitive_363,
  &primitive_364,
  &primitive_365,
  &primitive_366,
  &primitive_367,
  &primitive_368,
  &primitive_369,
  &primitive_370,
  &primitive_371,
  &primitive_372,
  &primitive_373,
  &primitive_374,
  &primitive_375,
  &primitive_376,
  &primitive_377,
  &primitive_378,
  &primitive_379,
  &primitive_380,
  &primitive_381,
  &primitive_382,
  &primitive_383,
  &primitive_384,
  &primitive_385,
  &primitive_386,
  &primitive_387,
  &primitive_388,
  &primitive_389,
  &primitive_390,
  &primitive_391,
  &primitive_392,
  &primitive_393
}; 
