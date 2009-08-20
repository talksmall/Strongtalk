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

static char* signature_21[] = { "Float", "SmallInteger"};
static char* errors_21[] = {NULL};
static primitive_desc primitive_21 = {
	"primitiveAlienCalloc:ifFail:", 
	fntype(&systemPrimitives::alienCalloc), 
	327681, 
	signature_21, 
	errors_21
};

static char* signature_22[] = { "Float", "SmallInteger|LargeInteger"};
static char* errors_22[] = {NULL};
static primitive_desc primitive_22 = {
	"primitiveAlienFree:ifFail:", 
	fntype(&systemPrimitives::alienFree), 
	327681, 
	signature_22, 
	errors_22
};

static char* signature_23[] = { "Float", "SmallInteger"};
static char* errors_23[] = {NULL};
static primitive_desc primitive_23 = {
	"primitiveAlienMalloc:ifFail:", 
	fntype(&systemPrimitives::alienMalloc), 
	327681, 
	signature_23, 
	errors_23
};

static char* signature_24[] = { "IndexedInstanceVariables", "SmallInteger"};
static char* errors_24[] = {NULL};
static primitive_desc primitive_24 = {
	"primitiveAllObjectsLimit:ifFail:", 
	fntype(&systemPrimitives::all_objects), 
	327681, 
	signature_24, 
	errors_24
};

static char* signature_25[] = { "Object", "IndexedInstanceVariables"};
static char* errors_25[] = {NULL};
static primitive_desc primitive_25 = {
	"primitiveApplyChange:ifFail:", 
	fntype(&systemPrimitives::applyChange), 
	327681, 
	signature_25, 
	errors_25
};

static char* signature_26[] = { "Float", "SmallInteger"};
static char* errors_26[] = {NULL};
static primitive_desc primitive_26 = {
	"primitiveAsFloat", 
	fntype(&double_from_smi), 
	1573377, 
	signature_26, 
	errors_26
};

static char* signature_27[] = { "SmallInteger", "Object"};
static char* errors_27[] = {NULL};
static primitive_desc primitive_27 = {
	"primitiveAsObjectID", 
	fntype(&oopPrimitives::asObjectID), 
	1114113, 
	signature_27, 
	errors_27
};

static char* signature_28[] = { "Object", "SmallInteger"};
static char* errors_28[] = { "OutOfBounds", NULL};
static primitive_desc primitive_28 = {
	"primitiveAsObjectIfFail:", 
	fntype(&smiOopPrimitives::asObject), 
	1310721, 
	signature_28, 
	errors_28
};

static char* signature_29[] = { "Object", "Object", "Object"};
static char* errors_29[] = { "RecieverHasWrongType", NULL};
static primitive_desc primitive_29 = {
	"primitiveBecome:ifFail:", 
	fntype(&oopPrimitives::become), 
	1376258, 
	signature_29, 
	errors_29
};

static char* signature_30[] = { "GlobalAssociation", "Behavior", "SmallInteger"};
static char* errors_30[] = { "OutOfBounds", NULL};
static primitive_desc primitive_30 = {
	"primitiveBehavior:classVariableAt:ifFail:", 
	fntype(&behaviorPrimitives::classVariableAt), 
	327682, 
	signature_30, 
	errors_30
};

static char* signature_31[] = { "IndexedInstanceVariables", "Behavior"};
static char* errors_31[] = { "OutOfBounds", NULL};
static primitive_desc primitive_31 = {
	"primitiveBehavior:classVariablesIfFail:", 
	fntype(&behaviorPrimitives::classVariables), 
	327681, 
	signature_31, 
	errors_31
};

static char* signature_32[] = { "Boolean", "Behavior"};
static char* errors_32[] = {NULL};
static primitive_desc primitive_32 = {
	"primitiveBehaviorCanBeSubclassed:ifFail:", 
	fntype(&behaviorPrimitives::can_be_subclassed), 
	327681, 
	signature_32, 
	errors_32
};

static char* signature_33[] = { "Boolean", "Behavior"};
static char* errors_33[] = {NULL};
static primitive_desc primitive_33 = {
	"primitiveBehaviorCanHaveInstanceVariables:ifFail:", 
	fntype(&behaviorPrimitives::can_have_instance_variables), 
	327681, 
	signature_33, 
	errors_33
};

static char* signature_34[] = { "Symbol", "Behavior"};
static char* errors_34[] = {NULL};
static primitive_desc primitive_34 = {
	"primitiveBehaviorFormat:ifFail:", 
	fntype(&behaviorPrimitives::format), 
	327681, 
	signature_34, 
	errors_34
};

static char* signature_35[] = { "SmallInteger", "Behavior"};
static char* errors_35[] = {NULL};
static primitive_desc primitive_35 = {
	"primitiveBehaviorHeaderSizeOf:ifFail:", 
	fntype(&behaviorPrimitives::headerSize), 
	327681, 
	signature_35, 
	errors_35
};

static char* signature_36[] = { "Boolean", "Behavior", "Object"};
static char* errors_36[] = {NULL};
static primitive_desc primitive_36 = {
	"primitiveBehaviorIsClassOf:", 
	fntype(&behaviorPrimitives::is_class_of), 
	5308418, 
	signature_36, 
	errors_36
};

static char* signature_37[] = { "Boolean", "Behavior"};
static char* errors_37[] = {NULL};
static primitive_desc primitive_37 = {
	"primitiveBehaviorIsSpecializedClass:ifFail:", 
	fntype(&behaviorPrimitives::is_specialized_class), 
	327681, 
	signature_37, 
	errors_37
};

static char* signature_38[] = { "Mixin", "Behavior"};
static char* errors_38[] = {NULL};
static primitive_desc primitive_38 = {
	"primitiveBehaviorMixinOf:ifFail:", 
	fntype(&behaviorPrimitives::mixinOf), 
	327681, 
	signature_38, 
	errors_38
};

static char* signature_39[] = { "SmallInteger", "Behavior"};
static char* errors_39[] = {NULL};
static primitive_desc primitive_39 = {
	"primitiveBehaviorNonIndexableSizeOf:ifFail:", 
	fntype(&behaviorPrimitives::nonIndexableSize), 
	327681, 
	signature_39, 
	errors_39
};

static char* signature_40[] = { "Symbol", "Behavior"};
static char* errors_40[] = {NULL};
static primitive_desc primitive_40 = {
	"primitiveBehaviorVMType:ifFail:", 
	fntype(&behaviorPrimitives::vm_type), 
	327681, 
	signature_40, 
	errors_40
};

static char* signature_41[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_41[] = {NULL};
static primitive_desc primitive_41 = {
	"primitiveBitAnd:ifFail:", 
	fntype(&smiOopPrimitives::bitAnd), 
	6029826, 
	signature_41, 
	errors_41
};

static char* signature_42[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_42[] = {NULL};
static primitive_desc primitive_42 = {
	"primitiveBitOr:ifFail:", 
	fntype(&smiOopPrimitives::bitOr), 
	6029826, 
	signature_42, 
	errors_42
};

static char* signature_43[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_43[] = {NULL};
static primitive_desc primitive_43 = {
	"primitiveBitShift:ifFail:", 
	fntype(&smiOopPrimitives::bitShift), 
	6029826, 
	signature_43, 
	errors_43
};

static char* signature_44[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_44[] = {NULL};
static primitive_desc primitive_44 = {
	"primitiveBitXor:ifFail:", 
	fntype(&smiOopPrimitives::bitXor), 
	6029826, 
	signature_44, 
	errors_44
};

static char* signature_45[] = { "Boolean", "Block"};
static char* errors_45[] = {NULL};
static primitive_desc primitive_45 = {
	"primitiveBlockIsOptimized", 
	fntype(&block_is_optimized), 
	1114113, 
	signature_45, 
	errors_45
};

static char* signature_46[] = { "Method", "Block"};
static char* errors_46[] = {NULL};
static primitive_desc primitive_46 = {
	"primitiveBlockMethod", 
	fntype(&block_method), 
	1114113, 
	signature_46, 
	errors_46
};

static char* signature_47[] = { "Boolean", "Symbol"};
static char* errors_47[] = { "NotFound", NULL};
static primitive_desc primitive_47 = {
	"primitiveBooleanFlagAt:ifFail:", 
	fntype(&debugPrimitives::boolAt), 
	327681, 
	signature_47, 
	errors_47
};

static char* signature_48[] = { "Boolean", "Symbol", "Boolean"};
static char* errors_48[] = { "NotFound", NULL};
static primitive_desc primitive_48 = {
	"primitiveBooleanFlagAt:put:ifFail:", 
	fntype(&debugPrimitives::boolAtPut), 
	327682, 
	signature_48, 
	errors_48
};

static char* signature_49[] = { "Object"};
static char* errors_49[] = {NULL};
static primitive_desc primitive_49 = {
	"primitiveBreakpoint", 
	fntype(&systemPrimitives::breakpoint), 
	65536, 
	signature_49, 
	errors_49
};

static char* signature_50[] = { "Object", "Object", "Proxy"};
static char* errors_50[] = {NULL};
static primitive_desc primitive_50 = {
	"primitiveCallBackInvokeC2:ifFail:", 
	fntype(&callBackPrimitives::invokeC), 
	1376258, 
	signature_50, 
	errors_50
};

static char* signature_51[] = { "Object", "Object", "Proxy"};
static char* errors_51[] = {NULL};
static primitive_desc primitive_51 = {
	"primitiveCallBackInvokePascal2:ifFail:", 
	fntype(&callBackPrimitives::invokePascal), 
	1376258, 
	signature_51, 
	errors_51
};

static char* signature_52[] = { "Object", "Object", "Symbol"};
static char* errors_52[] = {NULL};
static primitive_desc primitive_52 = {
	"primitiveCallBackReceiver:selector:ifFail:", 
	fntype(&callBackPrimitives::initialize), 
	327682, 
	signature_52, 
	errors_52
};

static char* signature_53[] = { "Object", "SmallInteger", "Proxy"};
static char* errors_53[] = {NULL};
static primitive_desc primitive_53 = {
	"primitiveCallBackRegisterCCall:result:ifFail:", 
	fntype(&callBackPrimitives::registerCCall), 
	327682, 
	signature_53, 
	errors_53
};

static char* signature_54[] = { "Object", "SmallInteger", "SmallInteger", "Proxy"};
static char* errors_54[] = {NULL};
static primitive_desc primitive_54 = {
	"primitiveCallBackRegisterPascalCall:numberOfArguments:result:ifFail:", 
	fntype(&callBackPrimitives::registerPascalCall), 
	327683, 
	signature_54, 
	errors_54
};

static char* signature_55[] = { "Object", "Object", "Proxy"};
static char* errors_55[] = {NULL};
static primitive_desc primitive_55 = {
	"primitiveCallBackUnregister:ifFail:", 
	fntype(&callBackPrimitives::unregister), 
	1376258, 
	signature_55, 
	errors_55
};

static char* signature_56[] = { "Proxy", "SmallInteger"};
static char* errors_56[] = { "OutOfBounds", NULL};
static primitive_desc primitive_56 = {
	"primitiveCharacterFor:ifFail:", 
	fntype(&systemPrimitives::characterFor), 
	327681, 
	signature_56, 
	errors_56
};

static char* signature_57[] = { "Self class", "Object"};
static char* errors_57[] = {NULL};
static primitive_desc primitive_57 = {
	"primitiveClass", 
	fntype(&oopPrimitives::klass), 
	5767169, 
	signature_57, 
	errors_57
};

static char* signature_58[] = { "Behavior", "Object"};
static char* errors_58[] = {NULL};
static primitive_desc primitive_58 = {
	"primitiveClassOf:", 
	fntype(&oopPrimitives::klass_of), 
	4718593, 
	signature_58, 
	errors_58
};

static char* signature_59[] = { "Object"};
static char* errors_59[] = {NULL};
static primitive_desc primitive_59 = {
	"primitiveClearInlineCaches", 
	fntype(&debugPrimitives::clearInlineCaches), 
	65536, 
	signature_59, 
	errors_59
};

static char* signature_60[] = { "Object"};
static char* errors_60[] = {NULL};
static primitive_desc primitive_60 = {
	"primitiveClearInvocationCounters", 
	fntype(&debugPrimitives::clearInvocationCounters), 
	65536, 
	signature_60, 
	errors_60
};

static char* signature_61[] = { "Object"};
static char* errors_61[] = {NULL};
static primitive_desc primitive_61 = {
	"primitiveClearLookupCache", 
	fntype(&debugPrimitives::clearLookupCache), 
	65536, 
	signature_61, 
	errors_61
};

static char* signature_62[] = { "Object"};
static char* errors_62[] = {NULL};
static primitive_desc primitive_62 = {
	"primitiveClearLookupCacheStatistics", 
	fntype(&debugPrimitives::clearLookupCacheStatistics), 
	65536, 
	signature_62, 
	errors_62
};

static char* signature_63[] = { "Object"};
static char* errors_63[] = {NULL};
static primitive_desc primitive_63 = {
	"primitiveClearNMethodCounters", 
	fntype(&debugPrimitives::clearNMethodCounters), 
	65536, 
	signature_63, 
	errors_63
};

static char* signature_64[] = { "Object"};
static char* errors_64[] = {NULL};
static primitive_desc primitive_64 = {
	"primitiveClearPrimitiveCounters", 
	fntype(&debugPrimitives::clearPrimitiveCounters), 
	65536, 
	signature_64, 
	errors_64
};

static char* signature_65[] = { "Array[String]"};
static char* errors_65[] = {NULL};
static primitive_desc primitive_65 = {
	"primitiveCommandLineArgs", 
	fntype(&systemPrimitives::command_line_args), 
	65536, 
	signature_65, 
	errors_65
};

static char* signature_66[] = { "Block"};
static char* errors_66[] = {NULL};
static primitive_desc primitive_66 = {
	"primitiveCompiledBlockAllocate0", 
	fntype(&allocateBlock0), 
	6359040, 
	signature_66, 
	errors_66
};

static char* signature_67[] = { "Block"};
static char* errors_67[] = {NULL};
static primitive_desc primitive_67 = {
	"primitiveCompiledBlockAllocate1", 
	fntype(&allocateBlock1), 
	6359040, 
	signature_67, 
	errors_67
};

static char* signature_68[] = { "Block"};
static char* errors_68[] = {NULL};
static primitive_desc primitive_68 = {
	"primitiveCompiledBlockAllocate2", 
	fntype(&allocateBlock2), 
	6359040, 
	signature_68, 
	errors_68
};

static char* signature_69[] = { "Block"};
static char* errors_69[] = {NULL};
static primitive_desc primitive_69 = {
	"primitiveCompiledBlockAllocate3", 
	fntype(&allocateBlock3), 
	6359040, 
	signature_69, 
	errors_69
};

static char* signature_70[] = { "Block"};
static char* errors_70[] = {NULL};
static primitive_desc primitive_70 = {
	"primitiveCompiledBlockAllocate4", 
	fntype(&allocateBlock4), 
	6359040, 
	signature_70, 
	errors_70
};

static char* signature_71[] = { "Block"};
static char* errors_71[] = {NULL};
static primitive_desc primitive_71 = {
	"primitiveCompiledBlockAllocate5", 
	fntype(&allocateBlock5), 
	6359040, 
	signature_71, 
	errors_71
};

static char* signature_72[] = { "Block"};
static char* errors_72[] = {NULL};
static primitive_desc primitive_72 = {
	"primitiveCompiledBlockAllocate6", 
	fntype(&allocateBlock6), 
	6359040, 
	signature_72, 
	errors_72
};

static char* signature_73[] = { "Block"};
static char* errors_73[] = {NULL};
static primitive_desc primitive_73 = {
	"primitiveCompiledBlockAllocate7", 
	fntype(&allocateBlock7), 
	6359040, 
	signature_73, 
	errors_73
};

static char* signature_74[] = { "Block"};
static char* errors_74[] = {NULL};
static primitive_desc primitive_74 = {
	"primitiveCompiledBlockAllocate8", 
	fntype(&allocateBlock8), 
	6359040, 
	signature_74, 
	errors_74
};

static char* signature_75[] = { "Block"};
static char* errors_75[] = {NULL};
static primitive_desc primitive_75 = {
	"primitiveCompiledBlockAllocate9", 
	fntype(&allocateBlock9), 
	6359040, 
	signature_75, 
	errors_75
};

static char* signature_76[] = { "Block", "SmallInteger"};
static char* errors_76[] = {NULL};
static primitive_desc primitive_76 = {
	"primitiveCompiledBlockAllocate:", 
	fntype(&allocateBlock), 
	2164737, 
	signature_76, 
	errors_76
};

static char* signature_77[] = { "Object"};
static char* errors_77[] = {NULL};
static primitive_desc primitive_77 = {
	"primitiveCompiledContextAllocate0", 
	fntype(&allocateContext0), 
	6359040, 
	signature_77, 
	errors_77
};

static char* signature_78[] = { "Object"};
static char* errors_78[] = {NULL};
static primitive_desc primitive_78 = {
	"primitiveCompiledContextAllocate1", 
	fntype(&allocateContext1), 
	6359040, 
	signature_78, 
	errors_78
};

static char* signature_79[] = { "Object"};
static char* errors_79[] = {NULL};
static primitive_desc primitive_79 = {
	"primitiveCompiledContextAllocate2", 
	fntype(&allocateContext2), 
	6359040, 
	signature_79, 
	errors_79
};

static char* signature_80[] = { "Object", "SmallInteger"};
static char* errors_80[] = {NULL};
static primitive_desc primitive_80 = {
	"primitiveCompiledContextAllocate:", 
	fntype(&allocateContext), 
	6359041, 
	signature_80, 
	errors_80
};

static char* signature_81[] = { "Method", "Object", "SmallInteger", "SmallInteger", "Array", "ByteArray", "Array"};
static char* errors_81[] = {NULL};
static primitive_desc primitive_81 = {
	"primitiveConstructMethod:flags:nofArgs:debugInfo:bytes:oops:ifFail:", 
	fntype(&methodOopPrimitives::constructMethod), 
	327686, 
	signature_81, 
	errors_81
};

static char* signature_82[] = { "Object", "Object"};
static char* errors_82[] = { "NotOops", NULL};
static primitive_desc primitive_82 = {
	"primitiveCopyTenuredIfFail:", 
	fntype(&oopPrimitives::copy_tenured), 
	1376257, 
	signature_82, 
	errors_82
};

static char* signature_83[] = { "GlobalAssociation", "Mixin", "Symbol", "Boolean", "Behavior", "Symbol"};
static char* errors_83[] = { "WrongFormat", NULL};
static primitive_desc primitive_83 = {
	"primitiveCreateInvocationOf:named:isPrimaryInvocation:superclass:format:ifFail:", 
	fntype(&systemPrimitives::createNamedInvocation), 
	327685, 
	signature_83, 
	errors_83
};

static char* signature_84[] = { "GlobalAssociation", "Mixin", "Behavior", "Symbol"};
static char* errors_84[] = { "WrongFormat", NULL};
static primitive_desc primitive_84 = {
	"primitiveCreateInvocationOf:superclass:format:ifFail:", 
	fntype(&systemPrimitives::createInvocation), 
	327683, 
	signature_84, 
	errors_84
};

static char* signature_85[] = { "SmallInteger"};
static char* errors_85[] = {NULL};
static primitive_desc primitive_85 = {
	"primitiveCurrentThreadId", 
	fntype(&systemPrimitives::current_thread_id), 
	65536, 
	signature_85, 
	errors_85
};

static char* signature_86[] = { "Proxy", "Symbol", "Proxy"};
static char* errors_86[] = {NULL};
static primitive_desc primitive_86 = {
	"primitiveDLLLoad:result:ifFail:", 
	fntype(&systemPrimitives::dll_load), 
	327682, 
	signature_86, 
	errors_86
};

static char* signature_87[] = { "Proxy", "Symbol", "Proxy", "Proxy"};
static char* errors_87[] = {NULL};
static primitive_desc primitive_87 = {
	"primitiveDLLLookup:in:result:ifFail:", 
	fntype(&systemPrimitives::dll_lookup), 
	327683, 
	signature_87, 
	errors_87
};

static char* signature_88[] = { "Object", "Object", "Symbol"};
static char* errors_88[] = {NULL};
static primitive_desc primitive_88 = {
	"primitiveDLLSetupLookup:selector:ifFail:", 
	fntype(&systemPrimitives::dll_setup), 
	327682, 
	signature_88, 
	errors_88
};

static char* signature_89[] = { "Object", "Proxy"};
static char* errors_89[] = {NULL};
static primitive_desc primitive_89 = {
	"primitiveDLLUnload:ifFail:", 
	fntype(&systemPrimitives::dll_unload), 
	327681, 
	signature_89, 
	errors_89
};

static char* signature_90[] = { "Object"};
static char* errors_90[] = {NULL};
static primitive_desc primitive_90 = {
	"primitiveDecodeAllMethods", 
	fntype(&debugPrimitives::decodeAllMethods), 
	65536, 
	signature_90, 
	errors_90
};

static char* signature_91[] = { "Object", "Object", "Symbol"};
static char* errors_91[] = { "NotFound", NULL};
static primitive_desc primitive_91 = {
	"primitiveDecodeMethod:ifFail:", 
	fntype(&debugPrimitives::decodeMethod), 
	1376258, 
	signature_91, 
	errors_91
};

static char* signature_92[] = { "Proxy", "Proxy"};
static char* errors_92[] = {NULL};
static primitive_desc primitive_92 = {
	"primitiveDefWindowProc:ifFail:", 
	fntype(&systemPrimitives::defWindowProc), 
	327681, 
	signature_92, 
	errors_92
};

static char* signature_93[] = { "Object"};
static char* errors_93[] = {NULL};
static primitive_desc primitive_93 = {
	"primitiveDeoptimizeStacks", 
	fntype(&debugPrimitives::deoptimizeStacks), 
	196608, 
	signature_93, 
	errors_93
};

static char* signature_94[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_94[] = { "Overflow", "DivisionByZero", NULL};
static primitive_desc primitive_94 = {
	"primitiveDiv:ifFail:", 
	fntype(&smiOopPrimitives_div), 
	6029826, 
	signature_94, 
	errors_94
};

static char* signature_95[] = { "Float"};
static char* errors_95[] = {NULL};
static primitive_desc primitive_95 = {
	"primitiveElapsedTime", 
	fntype(&systemPrimitives::elapsedTime), 
	65536, 
	signature_95, 
	errors_95
};

static char* signature_96[] = { "Boolean", "Object", "Object"};
static char* errors_96[] = {NULL};
static primitive_desc primitive_96 = {
	"primitiveEqual:", 
	fntype(&oopPrimitives::equal), 
	1572866, 
	signature_96, 
	errors_96
};

static char* signature_97[] = { "Object", "SmallInteger"};
static char* errors_97[] = {NULL};
static primitive_desc primitive_97 = {
	"primitiveExpandMemory:", 
	fntype(&systemPrimitives::expandMemory), 
	65537, 
	signature_97, 
	errors_97
};

static char* signature_98[] = { "SmallInteger"};
static char* errors_98[] = {NULL};
static primitive_desc primitive_98 = {
	"primitiveExpansions", 
	fntype(&systemPrimitives::expansions), 
	65536, 
	signature_98, 
	errors_98
};

static char* signature_99[] = { "Process|nil"};
static char* errors_99[] = {NULL};
static primitive_desc primitive_99 = {
	"primitiveFlatProfilerDisengage", 
	fntype(&systemPrimitives::flat_profiler_disengage), 
	65536, 
	signature_99, 
	errors_99
};

static char* signature_100[] = { "Process", "Process"};
static char* errors_100[] = {NULL};
static primitive_desc primitive_100 = {
	"primitiveFlatProfilerEngage:ifFail:", 
	fntype(&systemPrimitives::flat_profiler_engage), 
	327681, 
	signature_100, 
	errors_100
};

static char* signature_101[] = { "Object"};
static char* errors_101[] = {NULL};
static primitive_desc primitive_101 = {
	"primitiveFlatProfilerPrint", 
	fntype(&systemPrimitives::flat_profiler_print), 
	65536, 
	signature_101, 
	errors_101
};

static char* signature_102[] = { "Process|nil"};
static char* errors_102[] = {NULL};
static primitive_desc primitive_102 = {
	"primitiveFlatProfilerProcess", 
	fntype(&systemPrimitives::flat_profiler_process), 
	65536, 
	signature_102, 
	errors_102
};

static char* signature_103[] = { "Object"};
static char* errors_103[] = {NULL};
static primitive_desc primitive_103 = {
	"primitiveFlatProfilerReset", 
	fntype(&systemPrimitives::flat_profiler_reset), 
	65536, 
	signature_103, 
	errors_103
};

static char* signature_104[] = { "Float", "Float", "Float"};
static char* errors_104[] = {NULL};
static primitive_desc primitive_104 = {
	"primitiveFloatAdd:ifFail:", 
	fntype(&double_add), 
	6030338, 
	signature_104, 
	errors_104
};

static char* signature_105[] = { "Float", "Float"};
static char* errors_105[] = {NULL};
static primitive_desc primitive_105 = {
	"primitiveFloatArcCosineIfFail:", 
	fntype(&doubleOopPrimitives::arcCosine), 
	1836033, 
	signature_105, 
	errors_105
};

static char* signature_106[] = { "Float", "Float"};
static char* errors_106[] = {NULL};
static primitive_desc primitive_106 = {
	"primitiveFloatArcSineIfFail:", 
	fntype(&doubleOopPrimitives::arcSine), 
	1836033, 
	signature_106, 
	errors_106
};

static char* signature_107[] = { "Float", "Float"};
static char* errors_107[] = {NULL};
static primitive_desc primitive_107 = {
	"primitiveFloatArcTangentIfFail:", 
	fntype(&doubleOopPrimitives::arcTangent), 
	1836033, 
	signature_107, 
	errors_107
};

static char* signature_108[] = { "SmallInteger", "Float"};
static char* errors_108[] = { "SmallIntegerConversionFailed", NULL};
static primitive_desc primitive_108 = {
	"primitiveFloatAsSmallIntegerIfFail:", 
	fntype(&doubleOopPrimitives::asSmallInteger), 
	1836033, 
	signature_108, 
	errors_108
};

static char* signature_109[] = { "Float", "Float"};
static char* errors_109[] = {NULL};
static primitive_desc primitive_109 = {
	"primitiveFloatCeiling", 
	fntype(&doubleOopPrimitives::ceiling), 
	1573889, 
	signature_109, 
	errors_109
};

static char* signature_110[] = { "Float", "Float"};
static char* errors_110[] = {NULL};
static primitive_desc primitive_110 = {
	"primitiveFloatCosine", 
	fntype(&doubleOopPrimitives::cosine), 
	1573889, 
	signature_110, 
	errors_110
};

static char* signature_111[] = { "Float", "Float", "Float"};
static char* errors_111[] = {NULL};
static primitive_desc primitive_111 = {
	"primitiveFloatDivide:ifFail:", 
	fntype(&double_divide), 
	6030338, 
	signature_111, 
	errors_111
};

static char* signature_112[] = { "Boolean", "Float", "Float"};
static char* errors_112[] = {NULL};
static primitive_desc primitive_112 = {
	"primitiveFloatEqual:ifFail:", 
	fntype(&doubleOopPrimitives::equal), 
	6030082, 
	signature_112, 
	errors_112
};

static char* signature_113[] = { "Float", "Float"};
static char* errors_113[] = {NULL};
static primitive_desc primitive_113 = {
	"primitiveFloatExp", 
	fntype(&doubleOopPrimitives::exp), 
	1573889, 
	signature_113, 
	errors_113
};

static char* signature_114[] = { "SmallInteger", "Float"};
static char* errors_114[] = {NULL};
static primitive_desc primitive_114 = {
	"primitiveFloatExponent", 
	fntype(&doubleOopPrimitives::exponent), 
	1573889, 
	signature_114, 
	errors_114
};

static char* signature_115[] = { "Float", "Float"};
static char* errors_115[] = {NULL};
static primitive_desc primitive_115 = {
	"primitiveFloatFloor", 
	fntype(&doubleOopPrimitives::floor), 
	1573889, 
	signature_115, 
	errors_115
};

static char* signature_116[] = { "Boolean", "Float", "Float"};
static char* errors_116[] = {NULL};
static primitive_desc primitive_116 = {
	"primitiveFloatGreaterThan:ifFail:", 
	fntype(&doubleOopPrimitives::greaterThan), 
	6030082, 
	signature_116, 
	errors_116
};

static char* signature_117[] = { "Boolean", "Float", "Float"};
static char* errors_117[] = {NULL};
static primitive_desc primitive_117 = {
	"primitiveFloatGreaterThanOrEqual:ifFail:", 
	fntype(&doubleOopPrimitives::greaterThanOrEqual), 
	6030082, 
	signature_117, 
	errors_117
};

static char* signature_118[] = { "Float", "Float"};
static char* errors_118[] = {NULL};
static primitive_desc primitive_118 = {
	"primitiveFloatHyperbolicCosineIfFail:", 
	fntype(&doubleOopPrimitives::hyperbolicCosine), 
	1836033, 
	signature_118, 
	errors_118
};

static char* signature_119[] = { "Float", "Float"};
static char* errors_119[] = {NULL};
static primitive_desc primitive_119 = {
	"primitiveFloatHyperbolicSineIfFail:", 
	fntype(&doubleOopPrimitives::hyperbolicSine), 
	1836033, 
	signature_119, 
	errors_119
};

static char* signature_120[] = { "Float", "Float"};
static char* errors_120[] = {NULL};
static primitive_desc primitive_120 = {
	"primitiveFloatHyperbolicTangentIfFail:", 
	fntype(&doubleOopPrimitives::hyperbolicTangent), 
	1836033, 
	signature_120, 
	errors_120
};

static char* signature_121[] = { "Boolean", "Float"};
static char* errors_121[] = {NULL};
static primitive_desc primitive_121 = {
	"primitiveFloatIsFinite", 
	fntype(&doubleOopPrimitives::isFinite), 
	1573889, 
	signature_121, 
	errors_121
};

static char* signature_122[] = { "Boolean", "Float"};
static char* errors_122[] = {NULL};
static primitive_desc primitive_122 = {
	"primitiveFloatIsNan", 
	fntype(&doubleOopPrimitives::isNan), 
	1573889, 
	signature_122, 
	errors_122
};

static char* signature_123[] = { "Boolean", "Float", "Float"};
static char* errors_123[] = {NULL};
static primitive_desc primitive_123 = {
	"primitiveFloatLessThan:ifFail:", 
	fntype(&doubleOopPrimitives::lessThan), 
	6030082, 
	signature_123, 
	errors_123
};

static char* signature_124[] = { "Boolean", "Float", "Float"};
static char* errors_124[] = {NULL};
static primitive_desc primitive_124 = {
	"primitiveFloatLessThanOrEqual:ifFail:", 
	fntype(&doubleOopPrimitives::lessThanOrEqual), 
	6030082, 
	signature_124, 
	errors_124
};

static char* signature_125[] = { "Float", "Float"};
static char* errors_125[] = { "ReceiverNotStrictlyPositive", NULL};
static primitive_desc primitive_125 = {
	"primitiveFloatLnIfFail:", 
	fntype(&doubleOopPrimitives::ln), 
	1836033, 
	signature_125, 
	errors_125
};

static char* signature_126[] = { "Float", "Float"};
static char* errors_126[] = { "ReceiverNotStrictlyPositive", NULL};
static primitive_desc primitive_126 = {
	"primitiveFloatLog10IfFail:", 
	fntype(&doubleOopPrimitives::log10), 
	1836033, 
	signature_126, 
	errors_126
};

static char* signature_127[] = { "Float", "Float"};
static char* errors_127[] = {NULL};
static primitive_desc primitive_127 = {
	"primitiveFloatMantissa", 
	fntype(&doubleOopPrimitives::mantissa), 
	1573889, 
	signature_127, 
	errors_127
};

static char* signature_128[] = { "Float"};
static char* errors_128[] = {NULL};
static primitive_desc primitive_128 = {
	"primitiveFloatMaxValue", 
	fntype(&doubleOopPrimitives::min_positive_value), 
	524288, 
	signature_128, 
	errors_128
};

static char* signature_129[] = { "Float"};
static char* errors_129[] = {NULL};
static primitive_desc primitive_129 = {
	"primitiveFloatMinPositiveValue", 
	fntype(&doubleOopPrimitives::min_positive_value), 
	524288, 
	signature_129, 
	errors_129
};

static char* signature_130[] = { "Float", "Float", "Float"};
static char* errors_130[] = {NULL};
static primitive_desc primitive_130 = {
	"primitiveFloatMod:ifFail:", 
	fntype(&doubleOopPrimitives::mod), 
	1836034, 
	signature_130, 
	errors_130
};

static char* signature_131[] = { "Float", "Float", "Float"};
static char* errors_131[] = {NULL};
static primitive_desc primitive_131 = {
	"primitiveFloatMultiply:ifFail:", 
	fntype(&double_multiply), 
	6030338, 
	signature_131, 
	errors_131
};

static char* signature_132[] = { "Boolean", "Float", "Float"};
static char* errors_132[] = {NULL};
static primitive_desc primitive_132 = {
	"primitiveFloatNotEqual:ifFail:", 
	fntype(&doubleOopPrimitives::notEqual), 
	6030082, 
	signature_132, 
	errors_132
};

static char* signature_133[] = { "Self", "Float", "IndexedByteInstanceVariables"};
static char* errors_133[] = {NULL};
static primitive_desc primitive_133 = {
	"primitiveFloatPrintFormat:ifFail:", 
	fntype(&doubleOopPrimitives::printFormat), 
	1310722, 
	signature_133, 
	errors_133
};

static char* signature_134[] = { "IndexedByteInstanceVariables", "Float"};
static char* errors_134[] = {NULL};
static primitive_desc primitive_134 = {
	"primitiveFloatPrintString", 
	fntype(&doubleOopPrimitives::printString), 
	1048577, 
	signature_134, 
	errors_134
};

static char* signature_135[] = { "SmallInteger", "Float"};
static char* errors_135[] = { "SmallIntegerConversionFailed", NULL};
static primitive_desc primitive_135 = {
	"primitiveFloatRoundedAsSmallIntegerIfFail:", 
	fntype(&doubleOopPrimitives::roundedAsSmallInteger), 
	1836033, 
	signature_135, 
	errors_135
};

static char* signature_136[] = { "Float", "Float"};
static char* errors_136[] = {NULL};
static primitive_desc primitive_136 = {
	"primitiveFloatSine", 
	fntype(&doubleOopPrimitives::sine), 
	1573889, 
	signature_136, 
	errors_136
};

static char* signature_137[] = { "SmallInteger", "Float"};
static char* errors_137[] = { "ConversionFailed", NULL};
static primitive_desc primitive_137 = {
	"primitiveFloatSmallIntegerFloorIfFail:", 
	fntype(&doubleOopPrimitives::smi_floor), 
	6030337, 
	signature_137, 
	errors_137
};

static char* signature_138[] = { "Float", "Float"};
static char* errors_138[] = { "ReceiverNegative", NULL};
static primitive_desc primitive_138 = {
	"primitiveFloatSqrtIfFail:", 
	fntype(&doubleOopPrimitives::sqrt), 
	1836033, 
	signature_138, 
	errors_138
};

static char* signature_139[] = { "Float", "Float"};
static char* errors_139[] = {NULL};
static primitive_desc primitive_139 = {
	"primitiveFloatSquared", 
	fntype(&doubleOopPrimitives::squared), 
	1573889, 
	signature_139, 
	errors_139
};

static char* signature_140[] = { "ByteArray", "Float"};
static char* errors_140[] = {NULL};
static primitive_desc primitive_140 = {
	"primitiveFloatStoreString", 
	fntype(&doubleOopPrimitives::store_string), 
	1048577, 
	signature_140, 
	errors_140
};

static char* signature_141[] = { "Float", "Float", "Float"};
static char* errors_141[] = {NULL};
static primitive_desc primitive_141 = {
	"primitiveFloatSubtract:ifFail:", 
	fntype(&double_subtract), 
	6030338, 
	signature_141, 
	errors_141
};

static char* signature_142[] = { "Float", "Float"};
static char* errors_142[] = {NULL};
static primitive_desc primitive_142 = {
	"primitiveFloatTangentIfFail:", 
	fntype(&doubleOopPrimitives::tangent), 
	1836033, 
	signature_142, 
	errors_142
};

static char* signature_143[] = { "Float", "Float", "SmallInteger"};
static char* errors_143[] = { "RangeError", NULL};
static primitive_desc primitive_143 = {
	"primitiveFloatTimesTwoPower:ifFail:", 
	fntype(&doubleOopPrimitives::timesTwoPower), 
	1836034, 
	signature_143, 
	errors_143
};

static char* signature_144[] = { "Float", "Float"};
static char* errors_144[] = {NULL};
static primitive_desc primitive_144 = {
	"primitiveFloatTruncated", 
	fntype(&doubleOopPrimitives::truncated), 
	1573889, 
	signature_144, 
	errors_144
};

static char* signature_145[] = { "Object"};
static char* errors_145[] = {NULL};
static primitive_desc primitive_145 = {
	"primitiveFlushCodeCache", 
	fntype(&systemPrimitives::flush_code_cache), 
	65536, 
	signature_145, 
	errors_145
};

static char* signature_146[] = { "Object"};
static char* errors_146[] = {NULL};
static primitive_desc primitive_146 = {
	"primitiveFlushDeadCode", 
	fntype(&systemPrimitives::flush_dead_code), 
	65536, 
	signature_146, 
	errors_146
};

static char* signature_147[] = { "SmallInteger"};
static char* errors_147[] = {NULL};
static primitive_desc primitive_147 = {
	"primitiveFreeSpace", 
	fntype(&systemPrimitives::freeSpace), 
	65536, 
	signature_147, 
	errors_147
};

static char* signature_148[] = { "Self", "Object"};
static char* errors_148[] = {NULL};
static primitive_desc primitive_148 = {
	"primitiveGarbageCollect", 
	fntype(&systemPrimitives::garbageGollect), 
	1114113, 
	signature_148, 
	errors_148
};

static char* signature_149[] = { "Object", "Object", "Symbol"};
static char* errors_149[] = { "NotFound", NULL};
static primitive_desc primitive_149 = {
	"primitiveGenerateIR:ifFail:", 
	fntype(&debugPrimitives::generateIR), 
	1376258, 
	signature_149, 
	errors_149
};

static char* signature_150[] = { "Integer"};
static char* errors_150[] = {NULL};
static primitive_desc primitive_150 = {
	"primitiveGetLastError", 
	fntype(&systemPrimitives::getLastError), 
	65536, 
	signature_150, 
	errors_150
};

static char* signature_151[] = { "Boolean", "GlobalAssociation"};
static char* errors_151[] = {NULL};
static primitive_desc primitive_151 = {
	"primitiveGlobalAssociationIsConstant", 
	fntype(&systemPrimitives::globalAssociationIsConstant), 
	1114113, 
	signature_151, 
	errors_151
};

static char* signature_152[] = { "Symbol", "GlobalAssociation"};
static char* errors_152[] = {NULL};
static primitive_desc primitive_152 = {
	"primitiveGlobalAssociationKey", 
	fntype(&systemPrimitives::globalAssociationKey), 
	1114113, 
	signature_152, 
	errors_152
};

static char* signature_153[] = { "Boolean", "GlobalAssociation", "Boolean"};
static char* errors_153[] = {NULL};
static primitive_desc primitive_153 = {
	"primitiveGlobalAssociationSetConstant:", 
	fntype(&systemPrimitives::globalAssociationSetConstant), 
	1114114, 
	signature_153, 
	errors_153
};

static char* signature_154[] = { "Object", "GlobalAssociation", "Symbol"};
static char* errors_154[] = {NULL};
static primitive_desc primitive_154 = {
	"primitiveGlobalAssociationSetKey:", 
	fntype(&systemPrimitives::globalAssociationSetKey), 
	1114114, 
	signature_154, 
	errors_154
};

static char* signature_155[] = { "Object", "GlobalAssociation", "Object"};
static char* errors_155[] = {NULL};
static primitive_desc primitive_155 = {
	"primitiveGlobalAssociationSetValue:", 
	fntype(&systemPrimitives::globalAssociationSetValue), 
	1114114, 
	signature_155, 
	errors_155
};

static char* signature_156[] = { "Object", "GlobalAssociation"};
static char* errors_156[] = {NULL};
static primitive_desc primitive_156 = {
	"primitiveGlobalAssociationValue", 
	fntype(&systemPrimitives::globalAssociationValue), 
	1114113, 
	signature_156, 
	errors_156
};

static char* signature_157[] = { "Boolean", "SmallInteger", "SmallInteger"};
static char* errors_157[] = {NULL};
static primitive_desc primitive_157 = {
	"primitiveGreaterThan:ifFail:", 
	fntype(&smiOopPrimitives::greaterThan), 
	6029570, 
	signature_157, 
	errors_157
};

static char* signature_158[] = { "Boolean", "SmallInteger", "SmallInteger"};
static char* errors_158[] = {NULL};
static primitive_desc primitive_158 = {
	"primitiveGreaterThanOrEqual:ifFail:", 
	fntype(&smiOopPrimitives::greaterThanOrEqual), 
	6029570, 
	signature_158, 
	errors_158
};

static char* signature_159[] = { "Boolean", "Object"};
static char* errors_159[] = {NULL};
static primitive_desc primitive_159 = {
	"primitiveHadNearDeathExperience:", 
	fntype(&systemPrimitives::hadNearDeathExperience), 
	65537, 
	signature_159, 
	errors_159
};

static char* signature_160[] = { "Object"};
static char* errors_160[] = {NULL};
static primitive_desc primitive_160 = {
	"primitiveHalt", 
	fntype(&systemPrimitives::halt), 
	65536, 
	signature_160, 
	errors_160
};

static char* signature_161[] = { "SmallInteger", "Object"};
static char* errors_161[] = {NULL};
static primitive_desc primitive_161 = {
	"primitiveHash", 
	fntype(&oopPrimitives::hash), 
	1114113, 
	signature_161, 
	errors_161
};

static char* signature_162[] = { "SmallInteger", "Object"};
static char* errors_162[] = {NULL};
static primitive_desc primitive_162 = {
	"primitiveHashOf:", 
	fntype(&oopPrimitives::hash_of), 
	65537, 
	signature_162, 
	errors_162
};

static char* signature_163[] = { "SmallInteger", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_163[] = { "OutOfBounds", NULL};
static primitive_desc primitive_163 = {
	"primitiveIndexedByteAt:ifFail:", 
	fntype(&byteArrayPrimitives::at), 
	1312258, 
	signature_163, 
	errors_163
};

static char* signature_164[] = { "SmallInteger", "IndexedByteInstanceVariables", "SmallInteger", "SmallInteger"};
static char* errors_164[] = { "OutOfBounds", "ValueOutOfBounds", NULL};
static primitive_desc primitive_164 = {
	"primitiveIndexedByteAt:put:ifFail:", 
	fntype(&byteArrayPrimitives::atPut), 
	1312259, 
	signature_164, 
	errors_164
};

static char* signature_165[] = { "Self", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_165[] = {NULL};
static primitive_desc primitive_165 = {
	"primitiveIndexedByteAtAllPut:ifFail:", 
	fntype(&byteArrayPrimitives::at_all_put), 
	1376258, 
	signature_165, 
	errors_165
};

static char* signature_166[] = { "SmallInteger", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_166[] = { "OutOfBounds", NULL};
static primitive_desc primitive_166 = {
	"primitiveIndexedByteCharacterAt:ifFail:", 
	fntype(&byteArrayPrimitives::characterAt), 
	1312258, 
	signature_166, 
	errors_166
};

static char* signature_167[] = { "SmallInteger", "IndexedByteInstanceVariables", "String"};
static char* errors_167[] = {NULL};
static primitive_desc primitive_167 = {
	"primitiveIndexedByteCompare:ifFail:", 
	fntype(&byteArrayPrimitives::compare), 
	1376258, 
	signature_167, 
	errors_167
};

static char* signature_168[] = { "SmallInteger", "IndexedByteInstanceVariables"};
static char* errors_168[] = {NULL};
static primitive_desc primitive_168 = {
	"primitiveIndexedByteHash", 
	fntype(&byteArrayPrimitives::hash), 
	1574401, 
	signature_168, 
	errors_168
};

static char* signature_169[] = { "CompressedSymbol", "IndexedByteInstanceVariables"};
static char* errors_169[] = { "ValueOutOfBounds", NULL};
static primitive_desc primitive_169 = {
	"primitiveIndexedByteInternIfFail:", 
	fntype(&byteArrayPrimitives::intern), 
	1376257, 
	signature_169, 
	errors_169
};

static char* signature_170[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_170[] = { "ArgumentIsInvalid", NULL};
static primitive_desc primitive_170 = {
	"primitiveIndexedByteLargeIntegerAdd:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerAdd), 
	1312258, 
	signature_170, 
	errors_170
};

static char* signature_171[] = { "Float", "IndexedByteInstanceVariables"};
static char* errors_171[] = {NULL};
static primitive_desc primitive_171 = {
	"primitiveIndexedByteLargeIntegerAsFloatIfFail:", 
	fntype(&byteArrayPrimitives::largeIntegerToFloat), 
	1312257, 
	signature_171, 
	errors_171
};

static char* signature_172[] = { "SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_172[] = {NULL};
static primitive_desc primitive_172 = {
	"primitiveIndexedByteLargeIntegerCompare:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerCompare), 
	1312258, 
	signature_172, 
	errors_172
};

static char* signature_173[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_173[] = { "ArgumentIsInvalid", "DivisionByZero", NULL};
static primitive_desc primitive_173 = {
	"primitiveIndexedByteLargeIntegerDiv:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerDiv), 
	1312258, 
	signature_173, 
	errors_173
};

static char* signature_174[] = { "IndexedByteInstanceVariables", "IndexedByteInstanceVariables class", "Float"};
static char* errors_174[] = {NULL};
static primitive_desc primitive_174 = {
	"primitiveIndexedByteLargeIntegerFromFloat:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerFromDouble), 
	1310722, 
	signature_174, 
	errors_174
};

static char* signature_175[] = { "IndexedByteInstanceVariables", "IndexedByteInstanceVariables class", "SmallInteger"};
static char* errors_175[] = {NULL};
static primitive_desc primitive_175 = {
	"primitiveIndexedByteLargeIntegerFromSmallInteger:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerFromSmallInteger), 
	1310722, 
	signature_175, 
	errors_175
};

static char* signature_176[] = { "IndexedByteInstanceVariables", "IndexedByteInstanceVariables class", "String", "Integer"};
static char* errors_176[] = { "ConversionFailed", NULL};
static primitive_desc primitive_176 = {
	"primitiveIndexedByteLargeIntegerFromString:base:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerFromString), 
	1312259, 
	signature_176, 
	errors_176
};

static char* signature_177[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_177[] = { "ArgumentIsInvalid", "DivisionByZero", NULL};
static primitive_desc primitive_177 = {
	"primitiveIndexedByteLargeIntegerMod:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerMod), 
	1312258, 
	signature_177, 
	errors_177
};

static char* signature_178[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_178[] = { "ArgumentIsInvalid", NULL};
static primitive_desc primitive_178 = {
	"primitiveIndexedByteLargeIntegerMultiply:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerMultiply), 
	1312258, 
	signature_178, 
	errors_178
};

static char* signature_179[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_179[] = { "ArgumentIsInvalid", NULL};
static primitive_desc primitive_179 = {
	"primitiveIndexedByteLargeIntegerSubtract:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerSubtract), 
	1312258, 
	signature_179, 
	errors_179
};

static char* signature_180[] = { "String", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_180[] = {NULL};
static primitive_desc primitive_180 = {
	"primitiveIndexedByteLargeIntegerToStringBase:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerToString), 
	1312258, 
	signature_180, 
	errors_180
};

static char* signature_181[] = { "Object", "IndexedByteInstanceVariables class", "SmallInteger"};
static char* errors_181[] = { "NegativeSize", NULL};
static primitive_desc primitive_181 = {
	"primitiveIndexedByteNew:ifFail:", 
	fntype(&byteArrayPrimitives::allocateSize), 
	1376258, 
	signature_181, 
	errors_181
};

static char* signature_182[] = { "Object", "IndexedByteInstanceVariables class", "SmallInteger", "Boolean"};
static char* errors_182[] = { "NegativeSize", NULL};
static primitive_desc primitive_182 = {
	"primitiveIndexedByteNew:size:tenured:ifFail:", 
	fntype(&byteArrayPrimitives::allocateSize2), 
	327683, 
	signature_182, 
	errors_182
};

static char* signature_183[] = { "SmallInteger", "IndexedByteInstanceVariables"};
static char* errors_183[] = {NULL};
static primitive_desc primitive_183 = {
	"primitiveIndexedByteSize", 
	fntype(&byteArrayPrimitives::size), 
	1574401, 
	signature_183, 
	errors_183
};

static char* signature_184[] = { "SmallInteger", "IndexedDoubleByteInstanceVariables", "SmallInteger"};
static char* errors_184[] = { "OutOfBounds", NULL};
static primitive_desc primitive_184 = {
	"primitiveIndexedDoubleByteAt:ifFail:", 
	fntype(&doubleByteArrayPrimitives::at), 
	1312514, 
	signature_184, 
	errors_184
};

static char* signature_185[] = { "SmallInteger", "IndexedDoubleByteInstanceVariables", "SmallInteger", "SmallInteger"};
static char* errors_185[] = { "OutOfBounds", "ValueOutOfBounds", NULL};
static primitive_desc primitive_185 = {
	"primitiveIndexedDoubleByteAt:put:ifFail:", 
	fntype(&doubleByteArrayPrimitives::atPut), 
	1312515, 
	signature_185, 
	errors_185
};

static char* signature_186[] = { "SmallInteger", "IndexedDoubleByteInstanceVariables", "SmallInteger"};
static char* errors_186[] = { "OutOfBounds", NULL};
static primitive_desc primitive_186 = {
	"primitiveIndexedDoubleByteCharacterAt:ifFail:", 
	fntype(&doubleByteArrayPrimitives::characterAt), 
	1312514, 
	signature_186, 
	errors_186
};

static char* signature_187[] = { "SmallInteger", "IndexedDoubleByteInstanceVariables", "String"};
static char* errors_187[] = {NULL};
static primitive_desc primitive_187 = {
	"primitiveIndexedDoubleByteCompare:ifFail:", 
	fntype(&doubleByteArrayPrimitives::compare), 
	1310722, 
	signature_187, 
	errors_187
};

static char* signature_188[] = { "SmallInteger", "IndexedDoubleByteInstanceVariables"};
static char* errors_188[] = {NULL};
static primitive_desc primitive_188 = {
	"primitiveIndexedDoubleByteHash", 
	fntype(&doubleByteArrayPrimitives::hash), 
	1114113, 
	signature_188, 
	errors_188
};

static char* signature_189[] = { "CompressedSymbol", "IndexedDoubleByteInstanceVariables"};
static char* errors_189[] = { "ValueOutOfBounds", NULL};
static primitive_desc primitive_189 = {
	"primitiveIndexedDoubleByteInternIfFail:", 
	fntype(&doubleByteArrayPrimitives::intern), 
	1376257, 
	signature_189, 
	errors_189
};

static char* signature_190[] = { "Object", "IndexedDoubleByteInstanceVariables class", "SmallInteger"};
static char* errors_190[] = { "NegativeSize", NULL};
static primitive_desc primitive_190 = {
	"primitiveIndexedDoubleByteNew:ifFail:", 
	fntype(&doubleByteArrayPrimitives::allocateSize), 
	1376258, 
	signature_190, 
	errors_190
};

static char* signature_191[] = { "Object", "IndexedDoubleByteInstanceVariables class", "SmallInteger", "Boolean"};
static char* errors_191[] = { "NegativeSize", NULL};
static primitive_desc primitive_191 = {
	"primitiveIndexedDoubleByteNew:size:tenured:ifFail:", 
	fntype(&doubleByteArrayPrimitives::allocateSize2), 
	327683, 
	signature_191, 
	errors_191
};

static char* signature_192[] = { "SmallInteger", "IndexedDoubleByteInstanceVariables"};
static char* errors_192[] = {NULL};
static primitive_desc primitive_192 = {
	"primitiveIndexedDoubleByteSize", 
	fntype(&doubleByteArrayPrimitives::size), 
	1574657, 
	signature_192, 
	errors_192
};

static char* signature_193[] = { "Float", "IndexedFloatValueInstanceVariables", "SmallInteger"};
static char* errors_193[] = { "OutOfBounds", NULL};
static primitive_desc primitive_193 = {
	"primitiveIndexedFloatValueAt:ifFail:", 
	fntype(&doubleValueArrayPrimitives::at), 
	1310722, 
	signature_193, 
	errors_193
};

static char* signature_194[] = { "Float", "IndexedFloatValueInstanceVariables", "SmallInteger", "Float"};
static char* errors_194[] = { "OutOfBounds", "ValueOutOfBounds", NULL};
static primitive_desc primitive_194 = {
	"primitiveIndexedFloatValueAt:put:ifFail:", 
	fntype(&doubleValueArrayPrimitives::atPut), 
	1310723, 
	signature_194, 
	errors_194
};

static char* signature_195[] = { "Object", "IndexedFloatValueInstanceVariables class", "SmallInteger"};
static char* errors_195[] = { "NegativeSize", NULL};
static primitive_desc primitive_195 = {
	"primitiveIndexedFloatValueNew:ifFail:", 
	fntype(&doubleValueArrayPrimitives::allocateSize), 
	1376258, 
	signature_195, 
	errors_195
};

static char* signature_196[] = { "SmallInteger", "IndexedFloatValueInstanceVariables"};
static char* errors_196[] = {NULL};
static primitive_desc primitive_196 = {
	"primitiveIndexedFloatValueSize", 
	fntype(&doubleValueArrayPrimitives::size), 
	1572865, 
	signature_196, 
	errors_196
};

static char* signature_197[] = { "SmallInteger", "IndexedInstanceVariables", "SmallInteger"};
static char* errors_197[] = { "OutOfBounds", NULL};
static primitive_desc primitive_197 = {
	"primitiveIndexedObjectAt:ifFail:", 
	fntype(&objArrayPrimitives::at), 
	1312002, 
	signature_197, 
	errors_197
};

static char* signature_198[] = { "Object", "IndexedInstanceVariables", "SmallInteger", "Object"};
static char* errors_198[] = { "OutOfBounds", NULL};
static primitive_desc primitive_198 = {
	"primitiveIndexedObjectAt:put:ifFail:", 
	fntype(&objArrayPrimitives::atPut), 
	1312003, 
	signature_198, 
	errors_198
};

static char* signature_199[] = { "Self", "IndexedInstanceVariables", "Object"};
static char* errors_199[] = {NULL};
static primitive_desc primitive_199 = {
	"primitiveIndexedObjectAtAllPut:", 
	fntype(&objArrayPrimitives::at_all_put), 
	1049858, 
	signature_199, 
	errors_199
};

static char* signature_200[] = { "Self", "IndexedInstanceVariables", "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_200[] = { "OutOfBounds", "NegativeSize", NULL};
static primitive_desc primitive_200 = {
	"primitiveIndexedObjectCopyFrom:startingAt:size:ifFail:", 
	fntype(&objArrayPrimitives::copy_size), 
	1377540, 
	signature_200, 
	errors_200
};

static char* signature_201[] = { "Object", "IndexedInstanceVariables class", "SmallInteger"};
static char* errors_201[] = { "NegativeSize", NULL};
static primitive_desc primitive_201 = {
	"primitiveIndexedObjectNew:ifFail:", 
	fntype(&objArrayPrimitives::allocateSize), 
	1376258, 
	signature_201, 
	errors_201
};

static char* signature_202[] = { "Object", "IndexedInstanceVariables class", "SmallInteger", "Boolean"};
static char* errors_202[] = { "NegativeSize", NULL};
static primitive_desc primitive_202 = {
	"primitiveIndexedObjectNew:size:tenured:ifFail:", 
	fntype(&objArrayPrimitives::allocateSize2), 
	327683, 
	signature_202, 
	errors_202
};

static char* signature_203[] = { "Self", "IndexedInstanceVariables", "SmallInteger", "SmallInteger", "IndexedInstanceVariables", "SmallInteger"};
static char* errors_203[] = { "OutOfBounds", NULL};
static primitive_desc primitive_203 = {
	"primitiveIndexedObjectReplaceFrom:to:with:startingAt:ifFail:", 
	fntype(&objArrayPrimitives::replace_from_to), 
	1377541, 
	signature_203, 
	errors_203
};

static char* signature_204[] = { "Self", "IndexedInstanceVariables"};
static char* errors_204[] = {NULL};
static primitive_desc primitive_204 = {
	"primitiveIndexedObjectSize", 
	fntype(&objArrayPrimitives::size), 
	1574145, 
	signature_204, 
	errors_204
};

static char* signature_205[] = { "Instance", "Behavior", "SmallInt"};
static char* errors_205[] = {NULL};
static primitive_desc primitive_205 = {
	"primitiveInlineAllocations:count:", 
	fntype(&primitiveInlineAllocations), 
	4259842, 
	signature_205, 
	errors_205
};

static char* signature_206[] = { "Boolean", "Behavior", "Symbol"};
static char* errors_206[] = {NULL};
static primitive_desc primitive_206 = {
	"primitiveInliningDatabaseAddLookupEntryClass:selector:ifFail:", 
	fntype(&systemPrimitives::inlining_database_add_entry), 
	327682, 
	signature_206, 
	errors_206
};

static char* signature_207[] = { "Boolean"};
static char* errors_207[] = {NULL};
static primitive_desc primitive_207 = {
	"primitiveInliningDatabaseCompile", 
	fntype(&systemPrimitives::inlining_database_compile_next), 
	65536, 
	signature_207, 
	errors_207
};

static char* signature_208[] = { "Object", "String"};
static char* errors_208[] = {NULL};
static primitive_desc primitive_208 = {
	"primitiveInliningDatabaseCompile:ifFail:", 
	fntype(&systemPrimitives::inlining_database_compile), 
	327681, 
	signature_208, 
	errors_208
};

static char* signature_209[] = { "IndexedByteInstanceVariables", "String"};
static char* errors_209[] = {NULL};
static primitive_desc primitive_209 = {
	"primitiveInliningDatabaseCompileDemangled:ifFail:", 
	fntype(&systemPrimitives::inlining_database_demangle), 
	327681, 
	signature_209, 
	errors_209
};

static char* signature_210[] = { "Symbol"};
static char* errors_210[] = {NULL};
static primitive_desc primitive_210 = {
	"primitiveInliningDatabaseDirectory", 
	fntype(&systemPrimitives::inlining_database_directory), 
	65536, 
	signature_210, 
	errors_210
};

static char* signature_211[] = { "SmallInteger"};
static char* errors_211[] = {NULL};
static primitive_desc primitive_211 = {
	"primitiveInliningDatabaseFileOutAllIfFail:", 
	fntype(&systemPrimitives::inlining_database_file_out_all), 
	327680, 
	signature_211, 
	errors_211
};

static char* signature_212[] = { "SmallInteger", "Behavior"};
static char* errors_212[] = {NULL};
static primitive_desc primitive_212 = {
	"primitiveInliningDatabaseFileOutClass:ifFail:", 
	fntype(&systemPrimitives::inlining_database_file_out_class), 
	327681, 
	signature_212, 
	errors_212
};

static char* signature_213[] = { "IndexedByteInstanceVariables", "String"};
static char* errors_213[] = {NULL};
static primitive_desc primitive_213 = {
	"primitiveInliningDatabaseMangle:ifFail:", 
	fntype(&systemPrimitives::inlining_database_mangle), 
	327681, 
	signature_213, 
	errors_213
};

static char* signature_214[] = { "Symbol", "Symbol"};
static char* errors_214[] = {NULL};
static primitive_desc primitive_214 = {
	"primitiveInliningDatabaseSetDirectory:ifFail:", 
	fntype(&systemPrimitives::inlining_database_set_directory), 
	327681, 
	signature_214, 
	errors_214
};

static char* signature_215[] = { "Object", "Object", "SmallInteger"};
static char* errors_215[] = { "OutOfBounds", NULL};
static primitive_desc primitive_215 = {
	"primitiveInstVarAt:ifFail:", 
	fntype(&oopPrimitives::instVarAt), 
	1376258, 
	signature_215, 
	errors_215
};

static char* signature_216[] = { "Object", "Object", "SmallInteger", "Object"};
static char* errors_216[] = { "OutOfBounds", NULL};
static primitive_desc primitive_216 = {
	"primitiveInstVarAt:put:ifFail:", 
	fntype(&oopPrimitives::instVarAtPut), 
	1376259, 
	signature_216, 
	errors_216
};

static char* signature_217[] = { "Symbol", "Reciever", "Object", "SmallInteger"};
static char* errors_217[] = { "OutOfBounds", NULL};
static primitive_desc primitive_217 = {
	"primitiveInstVarNameFor:at:ifFail:", 
	fntype(&oopPrimitives::instance_variable_name_at), 
	1376259, 
	signature_217, 
	errors_217
};

static char* signature_218[] = { "IndexedInstanceVariables", "Class", "SmallInteger"};
static char* errors_218[] = {NULL};
static primitive_desc primitive_218 = {
	"primitiveInstancesOf:limit:ifFail:", 
	fntype(&systemPrimitives::instances_of), 
	327682, 
	signature_218, 
	errors_218
};

static char* signature_219[] = { "SmallInteger"};
static char* errors_219[] = {NULL};
static primitive_desc primitive_219 = {
	"primitiveInterpreterInvocationCounterLimit", 
	fntype(&debugPrimitives::interpreterInvocationCounterLimit), 
	65536, 
	signature_219, 
	errors_219
};

static char* signature_220[] = { "Boolean", "SmallInteger", "SmallInteger"};
static char* errors_220[] = {NULL};
static primitive_desc primitive_220 = {
	"primitiveLessThan:ifFail:", 
	fntype(&smiOopPrimitives::lessThan), 
	6029570, 
	signature_220, 
	errors_220
};

static char* signature_221[] = { "Boolean", "SmallInteger", "SmallInteger"};
static char* errors_221[] = {NULL};
static primitive_desc primitive_221 = {
	"primitiveLessThanOrEqual:ifFail:", 
	fntype(&smiOopPrimitives::lessThanOrEqual), 
	6029570, 
	signature_221, 
	errors_221
};

static char* signature_222[] = { "SmallInteger", "Float", "Float", "SmallInteger"};
static char* errors_222[] = {NULL};
static primitive_desc primitive_222 = {
	"primitiveMandelbrotAtRe:im:iterate:ifFail:", 
	fntype(&doubleOopPrimitives::mandelbrot), 
	4980739, 
	signature_222, 
	errors_222
};

static char* signature_223[] = { "Block", "Method", "Object"};
static char* errors_223[] = {NULL};
static primitive_desc primitive_223 = {
	"primitiveMethodAllocateBlock:ifFail:", 
	fntype(&methodOopPrimitives::allocate_block_self), 
	1376258, 
	signature_223, 
	errors_223
};

static char* signature_224[] = { "Block", "Method"};
static char* errors_224[] = {NULL};
static primitive_desc primitive_224 = {
	"primitiveMethodAllocateBlockIfFail:", 
	fntype(&methodOopPrimitives::allocate_block), 
	1376257, 
	signature_224, 
	errors_224
};

static char* signature_225[] = { "Object", "Method"};
static char* errors_225[] = {NULL};
static primitive_desc primitive_225 = {
	"primitiveMethodBody", 
	fntype(&methodOopPrimitives::fileout_body), 
	1114113, 
	signature_225, 
	errors_225
};

static char* signature_226[] = { "Object", "Method"};
static char* errors_226[] = {NULL};
static primitive_desc primitive_226 = {
	"primitiveMethodDebugInfo", 
	fntype(&methodOopPrimitives::debug_info), 
	1114113, 
	signature_226, 
	errors_226
};

static char* signature_227[] = { "Method", "Behavior", "CompressedSymbol"};
static char* errors_227[] = { "NotFound", NULL};
static primitive_desc primitive_227 = {
	"primitiveMethodFor:ifFail:", 
	fntype(&behaviorPrimitives::methodFor), 
	1376258, 
	signature_227, 
	errors_227
};

static char* signature_228[] = { "Symbol", "Method"};
static char* errors_228[] = {NULL};
static primitive_desc primitive_228 = {
	"primitiveMethodInliningInfo", 
	fntype(&methodOopPrimitives::inlining_info), 
	1114113, 
	signature_228, 
	errors_228
};

static char* signature_229[] = { "SmallInteger", "Method"};
static char* errors_229[] = {NULL};
static primitive_desc primitive_229 = {
	"primitiveMethodNumberOfArguments", 
	fntype(&methodOopPrimitives::numberOfArguments), 
	1114113, 
	signature_229, 
	errors_229
};

static char* signature_230[] = { "Symbol", "Method", "Method"};
static char* errors_230[] = {NULL};
static primitive_desc primitive_230 = {
	"primitiveMethodOuter:ifFail:", 
	fntype(&methodOopPrimitives::setOuter), 
	1376258, 
	signature_230, 
	errors_230
};

static char* signature_231[] = { "Method", "Method"};
static char* errors_231[] = { "ReceiverNotBlockMethod", NULL};
static primitive_desc primitive_231 = {
	"primitiveMethodOuterIfFail:", 
	fntype(&methodOopPrimitives::outer), 
	1376257, 
	signature_231, 
	errors_231
};

static char* signature_232[] = { "Method", "Method", "Object"};
static char* errors_232[] = {NULL};
static primitive_desc primitive_232 = {
	"primitiveMethodPrettyPrintKlass:ifFail:", 
	fntype(&methodOopPrimitives::prettyPrint), 
	1376258, 
	signature_232, 
	errors_232
};

static char* signature_233[] = { "ByteIndexedInstanceVariables", "Method", "Object"};
static char* errors_233[] = {NULL};
static primitive_desc primitive_233 = {
	"primitiveMethodPrettyPrintSourceKlass:ifFail:", 
	fntype(&methodOopPrimitives::prettyPrintSource), 
	1376258, 
	signature_233, 
	errors_233
};

static char* signature_234[] = { "Symbol", "Method"};
static char* errors_234[] = {NULL};
static primitive_desc primitive_234 = {
	"primitiveMethodPrintCodes", 
	fntype(&methodOopPrimitives::printCodes), 
	1114113, 
	signature_234, 
	errors_234
};

static char* signature_235[] = { "IndexedInstanceVariables", "Method"};
static char* errors_235[] = {NULL};
static primitive_desc primitive_235 = {
	"primitiveMethodReferencedClassVarNames", 
	fntype(&methodOopPrimitives::referenced_class_variable_names), 
	1114113, 
	signature_235, 
	errors_235
};

static char* signature_236[] = { "IndexedInstanceVariables", "Method"};
static char* errors_236[] = {NULL};
static primitive_desc primitive_236 = {
	"primitiveMethodReferencedGlobalNames", 
	fntype(&methodOopPrimitives::referenced_global_names), 
	1114113, 
	signature_236, 
	errors_236
};

static char* signature_237[] = { "IndexedInstanceVariables", "Method", "Mixin"};
static char* errors_237[] = {NULL};
static primitive_desc primitive_237 = {
	"primitiveMethodReferencedInstVarNamesMixin:ifFail:", 
	fntype(&methodOopPrimitives::referenced_instance_variable_names), 
	1376258, 
	signature_237, 
	errors_237
};

static char* signature_238[] = { "Symbol", "Method"};
static char* errors_238[] = {NULL};
static primitive_desc primitive_238 = {
	"primitiveMethodSelector", 
	fntype(&methodOopPrimitives::selector), 
	1114113, 
	signature_238, 
	errors_238
};

static char* signature_239[] = { "Symbol", "Method", "Symbol"};
static char* errors_239[] = {NULL};
static primitive_desc primitive_239 = {
	"primitiveMethodSelector:ifFail:", 
	fntype(&methodOopPrimitives::setSelector), 
	1376258, 
	signature_239, 
	errors_239
};

static char* signature_240[] = { "IndexedInstanceVariables", "Method"};
static char* errors_240[] = {NULL};
static primitive_desc primitive_240 = {
	"primitiveMethodSenders", 
	fntype(&methodOopPrimitives::senders), 
	1114113, 
	signature_240, 
	errors_240
};

static char* signature_241[] = { "Symbol", "Method", "Symbol"};
static char* errors_241[] = { "ArgumentIsInvalid", NULL};
static primitive_desc primitive_241 = {
	"primitiveMethodSetInliningInfo:ifFail:", 
	fntype(&methodOopPrimitives::set_inlining_info), 
	1376258, 
	signature_241, 
	errors_241
};

static char* signature_242[] = { "Object", "Method"};
static char* errors_242[] = {NULL};
static primitive_desc primitive_242 = {
	"primitiveMethodSizeAndFlags", 
	fntype(&methodOopPrimitives::size_and_flags), 
	1114113, 
	signature_242, 
	errors_242
};

static char* signature_243[] = { "Mixin", "Mixin", "Symbol"};
static char* errors_243[] = { "IsInstalled", "OutOfBounds", NULL};
static primitive_desc primitive_243 = {
	"primitiveMixin:addClassVariable:ifFail:", 
	fntype(&mixinOopPrimitives::add_class_variable), 
	327682, 
	signature_243, 
	errors_243
};

static char* signature_244[] = { "Symbol", "Mixin", "Symbol"};
static char* errors_244[] = { "IsInstalled", "OutOfBounds", NULL};
static primitive_desc primitive_244 = {
	"primitiveMixin:addInstanceVariable:ifFail:", 
	fntype(&mixinOopPrimitives::add_instance_variable), 
	327682, 
	signature_244, 
	errors_244
};

static char* signature_245[] = { "Method", "Mixin", "Method"};
static char* errors_245[] = { "IsInstalled", NULL};
static primitive_desc primitive_245 = {
	"primitiveMixin:addMethod:ifFail:", 
	fntype(&mixinOopPrimitives::add_method), 
	327682, 
	signature_245, 
	errors_245
};

static char* signature_246[] = { "Symbol", "Mixin", "SmallInteger"};
static char* errors_246[] = { "OutOfBounds", NULL};
static primitive_desc primitive_246 = {
	"primitiveMixin:classVariableAt:ifFail:", 
	fntype(&mixinOopPrimitives::class_variable_at), 
	327682, 
	signature_246, 
	errors_246
};

static char* signature_247[] = { "Symbol", "Mixin"};
static char* errors_247[] = {NULL};
static primitive_desc primitive_247 = {
	"primitiveMixin:classVariablesIfFail:", 
	fntype(&mixinOopPrimitives::class_variables), 
	327681, 
	signature_247, 
	errors_247
};

static char* signature_248[] = { "Symbol", "Mixin", "SmallInteger"};
static char* errors_248[] = { "OutOfBounds", NULL};
static primitive_desc primitive_248 = {
	"primitiveMixin:instanceVariableAt:ifFail:", 
	fntype(&mixinOopPrimitives::instance_variable_at), 
	327682, 
	signature_248, 
	errors_248
};

static char* signature_249[] = { "Symbol", "Mixin"};
static char* errors_249[] = {NULL};
static primitive_desc primitive_249 = {
	"primitiveMixin:instanceVariablesIfFail:", 
	fntype(&mixinOopPrimitives::instance_variables), 
	327681, 
	signature_249, 
	errors_249
};

static char* signature_250[] = { "Method", "Mixin", "SmallInteger"};
static char* errors_250[] = { "OutOfBounds", NULL};
static primitive_desc primitive_250 = {
	"primitiveMixin:methodAt:ifFail:", 
	fntype(&mixinOopPrimitives::method_at), 
	327682, 
	signature_250, 
	errors_250
};

static char* signature_251[] = { "Symbol", "Mixin"};
static char* errors_251[] = {NULL};
static primitive_desc primitive_251 = {
	"primitiveMixin:methodsIfFail:", 
	fntype(&mixinOopPrimitives::methods), 
	327681, 
	signature_251, 
	errors_251
};

static char* signature_252[] = { "Symbol", "Mixin", "SmallInteger"};
static char* errors_252[] = { "IsInstalled", "OutOfBounds", NULL};
static primitive_desc primitive_252 = {
	"primitiveMixin:removeClassVariableAt:ifFail:", 
	fntype(&mixinOopPrimitives::remove_class_variable_at), 
	327682, 
	signature_252, 
	errors_252
};

static char* signature_253[] = { "Symbol", "Mixin", "SmallInteger"};
static char* errors_253[] = { "IsInstalled", "OutOfBounds", NULL};
static primitive_desc primitive_253 = {
	"primitiveMixin:removeInstanceVariableAt:ifFail:", 
	fntype(&mixinOopPrimitives::remove_instance_variable_at), 
	327682, 
	signature_253, 
	errors_253
};

static char* signature_254[] = { "Method", "Mixin", "SmallInteger"};
static char* errors_254[] = { "IsInstalled", "OutOfBounds", NULL};
static primitive_desc primitive_254 = {
	"primitiveMixin:removeMethodAt:ifFail:", 
	fntype(&mixinOopPrimitives::remove_method_at), 
	327682, 
	signature_254, 
	errors_254
};

static char* signature_255[] = { "Mixin", "Mixin"};
static char* errors_255[] = {NULL};
static primitive_desc primitive_255 = {
	"primitiveMixinClassMixinOf:ifFail:", 
	fntype(&mixinOopPrimitives::class_mixin), 
	327681, 
	signature_255, 
	errors_255
};

static char* signature_256[] = { "Boolean", "Mixin"};
static char* errors_256[] = {NULL};
static primitive_desc primitive_256 = {
	"primitiveMixinIsInstalled:ifFail:", 
	fntype(&mixinOopPrimitives::is_installed), 
	327681, 
	signature_256, 
	errors_256
};

static char* signature_257[] = { "SmallInteger", "Mixin"};
static char* errors_257[] = {NULL};
static primitive_desc primitive_257 = {
	"primitiveMixinNumberOfClassVariablesOf:ifFail:", 
	fntype(&mixinOopPrimitives::number_of_class_variables), 
	327681, 
	signature_257, 
	errors_257
};

static char* signature_258[] = { "SmallInteger", "Mixin"};
static char* errors_258[] = {NULL};
static primitive_desc primitive_258 = {
	"primitiveMixinNumberOfInstanceVariablesOf:ifFail:", 
	fntype(&mixinOopPrimitives::number_of_instance_variables), 
	327681, 
	signature_258, 
	errors_258
};

static char* signature_259[] = { "SmallInteger", "Mixin"};
static char* errors_259[] = {NULL};
static primitive_desc primitive_259 = {
	"primitiveMixinNumberOfMethodsOf:ifFail:", 
	fntype(&mixinOopPrimitives::number_of_methods), 
	327681, 
	signature_259, 
	errors_259
};

static char* signature_260[] = { "Class", "Mixin"};
static char* errors_260[] = {NULL};
static primitive_desc primitive_260 = {
	"primitiveMixinPrimaryInvocationOf:ifFail:", 
	fntype(&mixinOopPrimitives::primary_invocation), 
	327681, 
	signature_260, 
	errors_260
};

static char* signature_261[] = { "Mixin", "Mixin", "Mixin"};
static char* errors_261[] = { "IsInstalled", NULL};
static primitive_desc primitive_261 = {
	"primitiveMixinSetClassMixinOf:to:ifFail:", 
	fntype(&mixinOopPrimitives::set_class_mixin), 
	327682, 
	signature_261, 
	errors_261
};

static char* signature_262[] = { "Boolean", "Mixin"};
static char* errors_262[] = {NULL};
static primitive_desc primitive_262 = {
	"primitiveMixinSetInstalled:ifFail:", 
	fntype(&mixinOopPrimitives::set_installed), 
	327681, 
	signature_262, 
	errors_262
};

static char* signature_263[] = { "Class", "Mixin", "Class"};
static char* errors_263[] = { "IsInstalled", NULL};
static primitive_desc primitive_263 = {
	"primitiveMixinSetPrimaryInvocationOf:to:ifFail:", 
	fntype(&mixinOopPrimitives::set_primary_invocation), 
	327682, 
	signature_263, 
	errors_263
};

static char* signature_264[] = { "Boolean", "Mixin"};
static char* errors_264[] = {NULL};
static primitive_desc primitive_264 = {
	"primitiveMixinSetUnInstalled:ifFail:", 
	fntype(&mixinOopPrimitives::set_uninstalled), 
	327681, 
	signature_264, 
	errors_264
};

static char* signature_265[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_265[] = { "Overflow", "DivisionByZero", NULL};
static primitive_desc primitive_265 = {
	"primitiveMod:ifFail:", 
	fntype(&smiOopPrimitives_mod), 
	6029826, 
	signature_265, 
	errors_265
};

static char* signature_266[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_266[] = { "Overflow", NULL};
static primitive_desc primitive_266 = {
	"primitiveMultiply:ifFail:", 
	fntype(&smiOopPrimitives_multiply), 
	6029826, 
	signature_266, 
	errors_266
};

static char* signature_267[] = { "Instance", "Behavior", "Boolean"};
static char* errors_267[] = {NULL};
static primitive_desc primitive_267 = {
	"primitiveNew0:ifFail:", 
	fntype(&primitiveNew0), 
	7667714, 
	signature_267, 
	errors_267
};

static char* signature_268[] = { "Instance", "Behavior", "Boolean"};
static char* errors_268[] = {NULL};
static primitive_desc primitive_268 = {
	"primitiveNew1:ifFail:", 
	fntype(&primitiveNew1), 
	7667714, 
	signature_268, 
	errors_268
};

static char* signature_269[] = { "Instance", "Behavior", "Boolean"};
static char* errors_269[] = {NULL};
static primitive_desc primitive_269 = {
	"primitiveNew2:ifFail:", 
	fntype(&primitiveNew2), 
	7667714, 
	signature_269, 
	errors_269
};

static char* signature_270[] = { "Instance", "Behavior"};
static char* errors_270[] = { "ReceiverIsIndexable", NULL};
static primitive_desc primitive_270 = {
	"primitiveNew2IfFail:", 
	fntype(&behaviorPrimitives::allocate2), 
	1376257, 
	signature_270, 
	errors_270
};

static char* signature_271[] = { "Instance", "Behavior", "Boolean"};
static char* errors_271[] = {NULL};
static primitive_desc primitive_271 = {
	"primitiveNew3:ifFail:", 
	fntype(&primitiveNew3), 
	7667714, 
	signature_271, 
	errors_271
};

static char* signature_272[] = { "Instance", "Behavior", "Boolean"};
static char* errors_272[] = {NULL};
static primitive_desc primitive_272 = {
	"primitiveNew4:ifFail:", 
	fntype(&primitiveNew4), 
	7667714, 
	signature_272, 
	errors_272
};

static char* signature_273[] = { "Instance", "Behavior", "Boolean"};
static char* errors_273[] = {NULL};
static primitive_desc primitive_273 = {
	"primitiveNew5:ifFail:", 
	fntype(&primitiveNew5), 
	7667714, 
	signature_273, 
	errors_273
};

static char* signature_274[] = { "Instance", "Behavior", "Boolean"};
static char* errors_274[] = {NULL};
static primitive_desc primitive_274 = {
	"primitiveNew6:ifFail:", 
	fntype(&primitiveNew6), 
	7667714, 
	signature_274, 
	errors_274
};

static char* signature_275[] = { "Instance", "Behavior", "Boolean"};
static char* errors_275[] = {NULL};
static primitive_desc primitive_275 = {
	"primitiveNew7:ifFail:", 
	fntype(&primitiveNew7), 
	7667714, 
	signature_275, 
	errors_275
};

static char* signature_276[] = { "Instance", "Behavior", "Boolean"};
static char* errors_276[] = {NULL};
static primitive_desc primitive_276 = {
	"primitiveNew8:ifFail:", 
	fntype(&primitiveNew8), 
	7667714, 
	signature_276, 
	errors_276
};

static char* signature_277[] = { "Instance", "Behavior", "Boolean"};
static char* errors_277[] = {NULL};
static primitive_desc primitive_277 = {
	"primitiveNew9:ifFail:", 
	fntype(&primitiveNew9), 
	7667714, 
	signature_277, 
	errors_277
};

static char* signature_278[] = { "Instance", "Behavior", "Boolean"};
static char* errors_278[] = { "ReceiverIsIndexable", NULL};
static primitive_desc primitive_278 = {
	"primitiveNew:tenured:ifFail:", 
	fntype(&behaviorPrimitives::allocate3), 
	327682, 
	signature_278, 
	errors_278
};

static char* signature_279[] = { "Instance", "Behavior"};
static char* errors_279[] = { "ReceiverIsIndexable", NULL};
static primitive_desc primitive_279 = {
	"primitiveNewIfFail:", 
	fntype(&behaviorPrimitives::allocate), 
	1376257, 
	signature_279, 
	errors_279
};

static char* signature_280[] = { "Boolean", "Object", "Object"};
static char* errors_280[] = {NULL};
static primitive_desc primitive_280 = {
	"primitiveNotEqual:", 
	fntype(&oopPrimitives::not_equal), 
	1572866, 
	signature_280, 
	errors_280
};

static char* signature_281[] = { "Object"};
static char* errors_281[] = { "EmptyQueue", NULL};
static primitive_desc primitive_281 = {
	"primitiveNotificationQueueGetIfFail:", 
	fntype(&systemPrimitives::notificationQueueGet), 
	327680, 
	signature_281, 
	errors_281
};

static char* signature_282[] = { "Object", "Object"};
static char* errors_282[] = {NULL};
static primitive_desc primitive_282 = {
	"primitiveNotificationQueuePut:", 
	fntype(&systemPrimitives::notificationQueuePut), 
	65537, 
	signature_282, 
	errors_282
};

static char* signature_283[] = { "SmallInteger"};
static char* errors_283[] = {NULL};
static primitive_desc primitive_283 = {
	"primitiveNumberOfLookupCacheMisses", 
	fntype(&debugPrimitives::numberOfLookupCacheMisses), 
	65536, 
	signature_283, 
	errors_283
};

static char* signature_284[] = { "SmallInteger"};
static char* errors_284[] = {NULL};
static primitive_desc primitive_284 = {
	"primitiveNumberOfMethodInvocations", 
	fntype(&debugPrimitives::numberOfMethodInvocations), 
	65536, 
	signature_284, 
	errors_284
};

static char* signature_285[] = { "SmallInteger"};
static char* errors_285[] = {NULL};
static primitive_desc primitive_285 = {
	"primitiveNumberOfNMethodInvocations", 
	fntype(&debugPrimitives::numberOfNMethodInvocations), 
	65536, 
	signature_285, 
	errors_285
};

static char* signature_286[] = { "SmallInteger"};
static char* errors_286[] = {NULL};
static primitive_desc primitive_286 = {
	"primitiveNumberOfPrimaryLookupCacheHits", 
	fntype(&debugPrimitives::numberOfPrimaryLookupCacheHits), 
	65536, 
	signature_286, 
	errors_286
};

static char* signature_287[] = { "SmallInteger"};
static char* errors_287[] = {NULL};
static primitive_desc primitive_287 = {
	"primitiveNumberOfSecondaryLookupCacheHits", 
	fntype(&debugPrimitives::numberOfSecondaryLookupCacheHits), 
	65536, 
	signature_287, 
	errors_287
};

static char* signature_288[] = { "SmallInteger"};
static char* errors_288[] = {NULL};
static primitive_desc primitive_288 = {
	"primitiveNurseryFreeSpace", 
	fntype(&systemPrimitives::nurseryFreeSpace), 
	65536, 
	signature_288, 
	errors_288
};

static char* signature_289[] = { "Float"};
static char* errors_289[] = {NULL};
static primitive_desc primitive_289 = {
	"primitiveObjectMemorySize", 
	fntype(&systemPrimitives::object_memory_size), 
	65536, 
	signature_289, 
	errors_289
};

static char* signature_290[] = { "SmallInteger", "Object"};
static char* errors_290[] = {NULL};
static primitive_desc primitive_290 = {
	"primitiveOopSize", 
	fntype(&oopPrimitives::oop_size), 
	1572865, 
	signature_290, 
	errors_290
};

static char* signature_291[] = { "Object", "Object", "Symbol"};
static char* errors_291[] = { "NotFound", NULL};
static primitive_desc primitive_291 = {
	"primitiveOptimizeMethod:ifFail:", 
	fntype(&debugPrimitives::optimizeMethod), 
	1376258, 
	signature_291, 
	errors_291
};

static char* signature_292[] = { "Object", "Object", "CompressedSymbol", "Array"};
static char* errors_292[] = { "SelectorHasWrongNumberOfArguments", NULL};
static primitive_desc primitive_292 = {
	"primitivePerform:arguments:ifFail:", 
	fntype(&oopPrimitives::performArguments), 
	1507331, 
	signature_292, 
	errors_292
};

static char* signature_293[] = { "Object", "Object", "CompressedSymbol"};
static char* errors_293[] = { "SelectorHasWrongNumberOfArguments", NULL};
static primitive_desc primitive_293 = {
	"primitivePerform:ifFail:", 
	fntype(&oopPrimitives::perform), 
	1376258, 
	signature_293, 
	errors_293
};

static char* signature_294[] = { "Object", "Object", "CompressedSymbol", "Object"};
static char* errors_294[] = { "SelectorHasWrongNumberOfArguments", NULL};
static primitive_desc primitive_294 = {
	"primitivePerform:with:ifFail:", 
	fntype(&oopPrimitives::performWith), 
	1507331, 
	signature_294, 
	errors_294
};

static char* signature_295[] = { "Object", "Object", "CompressedSymbol", "Object", "Object"};
static char* errors_295[] = { "SelectorHasWrongNumberOfArguments", NULL};
static primitive_desc primitive_295 = {
	"primitivePerform:with:with:ifFail:", 
	fntype(&oopPrimitives::performWithWith), 
	1507332, 
	signature_295, 
	errors_295
};

static char* signature_296[] = { "Object", "Object", "CompressedSymbol", "Object", "Object", "Object"};
static char* errors_296[] = { "SelectorHasWrongNumberOfArguments", NULL};
static primitive_desc primitive_296 = {
	"primitivePerform:with:with:with:ifFail:", 
	fntype(&oopPrimitives::performWithWithWith), 
	1507333, 
	signature_296, 
	errors_296
};

static char* signature_297[] = { "Self", "Object"};
static char* errors_297[] = {NULL};
static primitive_desc primitive_297 = {
	"primitivePrint", 
	fntype(&oopPrimitives::print), 
	1114113, 
	signature_297, 
	errors_297
};

static char* signature_298[] = { "SmallInteger", "SmallInteger"};
static char* errors_298[] = { "OutOfBounds", NULL};
static primitive_desc primitive_298 = {
	"primitivePrintCharacterIfFail:", 
	fntype(&smiOopPrimitives::printCharacter), 
	1310721, 
	signature_298, 
	errors_298
};

static char* signature_299[] = { "Object", "SmallInteger"};
static char* errors_299[] = {NULL};
static primitive_desc primitive_299 = {
	"primitivePrintInvocationCounterHistogram:ifFail:", 
	fntype(&debugPrimitives::printInvocationCounterHistogram), 
	327681, 
	signature_299, 
	errors_299
};

static char* signature_300[] = { "Object"};
static char* errors_300[] = {NULL};
static primitive_desc primitive_300 = {
	"primitivePrintLayout", 
	fntype(&debugPrimitives::printMemoryLayout), 
	65536, 
	signature_300, 
	errors_300
};

static char* signature_301[] = { "Object"};
static char* errors_301[] = {NULL};
static primitive_desc primitive_301 = {
	"primitivePrintLookupCacheStatistics", 
	fntype(&debugPrimitives::printLookupCacheStatistics), 
	65536, 
	signature_301, 
	errors_301
};

static char* signature_302[] = { "Object"};
static char* errors_302[] = {NULL};
static primitive_desc primitive_302 = {
	"primitivePrintMemory", 
	fntype(&systemPrimitives::print_memory), 
	65536, 
	signature_302, 
	errors_302
};

static char* signature_303[] = { "Behavior", "Behavior", "ByteArray"};
static char* errors_303[] = {NULL};
static primitive_desc primitive_303 = {
	"primitivePrintMethod:ifFail:", 
	fntype(&behaviorPrimitives::printMethod), 
	1376258, 
	signature_303, 
	errors_303
};

static char* signature_304[] = { "Object", "Object", "Symbol"};
static char* errors_304[] = { "NotFound", NULL};
static primitive_desc primitive_304 = {
	"primitivePrintMethodCodes:ifFail:", 
	fntype(&debugPrimitives::printMethodCodes), 
	1376258, 
	signature_304, 
	errors_304
};

static char* signature_305[] = { "Object", "SmallInteger"};
static char* errors_305[] = {NULL};
static primitive_desc primitive_305 = {
	"primitivePrintNMethodCounterHistogram:ifFail:", 
	fntype(&debugPrimitives::printNMethodCounterHistogram), 
	327681, 
	signature_305, 
	errors_305
};

static char* signature_306[] = { "Object"};
static char* errors_306[] = {NULL};
static primitive_desc primitive_306 = {
	"primitivePrintObjectHistogram", 
	fntype(&debugPrimitives::printObjectHistogram), 
	65536, 
	signature_306, 
	errors_306
};

static char* signature_307[] = { "Object"};
static char* errors_307[] = {NULL};
static primitive_desc primitive_307 = {
	"primitivePrintPrimitiveCounters", 
	fntype(&debugPrimitives::printPrimitiveCounters), 
	65536, 
	signature_307, 
	errors_307
};

static char* signature_308[] = { "Object"};
static char* errors_308[] = {NULL};
static primitive_desc primitive_308 = {
	"primitivePrintPrimitiveTable", 
	fntype(&systemPrimitives::printPrimitiveTable), 
	65536, 
	signature_308, 
	errors_308
};

static char* signature_309[] = { "Self", "Object"};
static char* errors_309[] = {NULL};
static primitive_desc primitive_309 = {
	"primitivePrintValue", 
	fntype(&oopPrimitives::printValue), 
	1114113, 
	signature_309, 
	errors_309
};

static char* signature_310[] = { "Object"};
static char* errors_310[] = {NULL};
static primitive_desc primitive_310 = {
	"primitivePrintZone", 
	fntype(&systemPrimitives::print_zone), 
	65536, 
	signature_310, 
	errors_310
};

static char* signature_311[] = { "Object"};
static char* errors_311[] = {NULL};
static primitive_desc primitive_311 = {
	"primitiveProcessActiveProcess", 
	fntype(&processOopPrimitives::activeProcess), 
	65536, 
	signature_311, 
	errors_311
};

static char* signature_312[] = { "Process", "Process class", "BlockWithoutArguments"};
static char* errors_312[] = { "ProcessAllocationFailed", NULL};
static primitive_desc primitive_312 = {
	"primitiveProcessCreate:ifFail:", 
	fntype(&processOopPrimitives::create), 
	1376258, 
	signature_312, 
	errors_312
};

static char* signature_313[] = { "Process"};
static char* errors_313[] = {NULL};
static primitive_desc primitive_313 = {
	"primitiveProcessEnterCritical", 
	fntype(&processOopPrimitives::enter_critical), 
	65536, 
	signature_313, 
	errors_313
};

static char* signature_314[] = { "Process"};
static char* errors_314[] = {NULL};
static primitive_desc primitive_314 = {
	"primitiveProcessLeaveCritical", 
	fntype(&processOopPrimitives::leave_critical), 
	65536, 
	signature_314, 
	errors_314
};

static char* signature_315[] = { "Boolean", "Process", "SmallInteger"};
static char* errors_315[] = {NULL};
static primitive_desc primitive_315 = {
	"primitiveProcessSchedulerWait:ifFail:", 
	fntype(&processOopPrimitives::scheduler_wait), 
	1376258, 
	signature_315, 
	errors_315
};

static char* signature_316[] = { "Symbol", "Process", "Symbol", "Activation", "Object"};
static char* errors_316[] = { "InScheduler", "Dead", NULL};
static primitive_desc primitive_316 = {
	"primitiveProcessSetMode:activation:returnValue:ifFail:", 
	fntype(&processOopPrimitives::set_mode), 
	1376260, 
	signature_316, 
	errors_316
};

static char* signature_317[] = { "IndexedInstanceVariables", "Process", "SmallInteger"};
static char* errors_317[] = {NULL};
static primitive_desc primitive_317 = {
	"primitiveProcessStackLimit:ifFail:", 
	fntype(&processOopPrimitives::stack), 
	1376258, 
	signature_317, 
	errors_317
};

static char* signature_318[] = { "Object", "Process"};
static char* errors_318[] = { "NotInScheduler", "ProcessCannotContinue", "Dead", NULL};
static primitive_desc primitive_318 = {
	"primitiveProcessStartEvaluator:ifFail:", 
	fntype(&processOopPrimitives::start_evaluator), 
	327681, 
	signature_318, 
	errors_318
};

static char* signature_319[] = { "Symbol", "Process"};
static char* errors_319[] = {NULL};
static primitive_desc primitive_319 = {
	"primitiveProcessStatus", 
	fntype(&processOopPrimitives::status), 
	1114113, 
	signature_319, 
	errors_319
};

static char* signature_320[] = { "Process"};
static char* errors_320[] = {NULL};
static primitive_desc primitive_320 = {
	"primitiveProcessStop", 
	fntype(&processOopPrimitives::stop), 
	65536, 
	signature_320, 
	errors_320
};

static char* signature_321[] = { "Float", "Process"};
static char* errors_321[] = {NULL};
static primitive_desc primitive_321 = {
	"primitiveProcessSystemTime", 
	fntype(&processOopPrimitives::user_time), 
	1114113, 
	signature_321, 
	errors_321
};

static char* signature_322[] = { "Self", "Process"};
static char* errors_322[] = { "Dead", NULL};
static primitive_desc primitive_322 = {
	"primitiveProcessTerminateIfFail:", 
	fntype(&processOopPrimitives::terminate), 
	1507329, 
	signature_322, 
	errors_322
};

static char* signature_323[] = { "Self", "Process", "SmallInteger"};
static char* errors_323[] = {NULL};
static primitive_desc primitive_323 = {
	"primitiveProcessTraceStack:ifFail:", 
	fntype(&processOopPrimitives::trace_stack), 
	1376258, 
	signature_323, 
	errors_323
};

static char* signature_324[] = { "Object", "Process"};
static char* errors_324[] = { "NotInScheduler", "ProcessCannotContinue", "Dead", NULL};
static primitive_desc primitive_324 = {
	"primitiveProcessTransferTo:ifFail:", 
	fntype(&processOopPrimitives::transferTo), 
	327681, 
	signature_324, 
	errors_324
};

static char* signature_325[] = { "Float", "Process"};
static char* errors_325[] = {NULL};
static primitive_desc primitive_325 = {
	"primitiveProcessUserTime", 
	fntype(&processOopPrimitives::user_time), 
	1114113, 
	signature_325, 
	errors_325
};

static char* signature_326[] = { "Process"};
static char* errors_326[] = {NULL};
static primitive_desc primitive_326 = {
	"primitiveProcessYield", 
	fntype(&processOopPrimitives::yield), 
	65536, 
	signature_326, 
	errors_326
};

static char* signature_327[] = { "Process"};
static char* errors_327[] = {NULL};
static primitive_desc primitive_327 = {
	"primitiveProcessYieldInCritical", 
	fntype(&processOopPrimitives::yield_in_critical), 
	65536, 
	signature_327, 
	errors_327
};

static char* signature_328[] = { "SmallInteger", "Proxy", "SmallInteger"};
static char* errors_328[] = {NULL};
static primitive_desc primitive_328 = {
	"primitiveProxyByteAt:ifFail:", 
	fntype(&proxyOopPrimitives::byteAt), 
	5570562, 
	signature_328, 
	errors_328
};

static char* signature_329[] = { "SmallInteger", "Proxy", "SmallInteger", "SmallInteger"};
static char* errors_329[] = {NULL};
static primitive_desc primitive_329 = {
	"primitiveProxyByteAt:put:ifFail:", 
	fntype(&proxyOopPrimitives::byteAtPut), 
	5570563, 
	signature_329, 
	errors_329
};

static char* signature_330[] = { "Self", "Proxy", "SmallInteger"};
static char* errors_330[] = {NULL};
static primitive_desc primitive_330 = {
	"primitiveProxyCalloc:ifFail:", 
	fntype(&proxyOopPrimitives::calloc), 
	1376258, 
	signature_330, 
	errors_330
};

static char* signature_331[] = { "SmallInteger", "Proxy", "SmallInteger"};
static char* errors_331[] = {NULL};
static primitive_desc primitive_331 = {
	"primitiveProxyDoubleByteAt:ifFail:", 
	fntype(&proxyOopPrimitives::doubleByteAt), 
	1376258, 
	signature_331, 
	errors_331
};

static char* signature_332[] = { "SmallInteger", "Proxy", "SmallInteger", "SmallInteger"};
static char* errors_332[] = {NULL};
static primitive_desc primitive_332 = {
	"primitiveProxyDoubleByteAt:put:ifFail:", 
	fntype(&proxyOopPrimitives::doubleByteAtPut), 
	1376259, 
	signature_332, 
	errors_332
};

static char* signature_333[] = { "Float", "Proxy", "SmallInteger"};
static char* errors_333[] = {NULL};
static primitive_desc primitive_333 = {
	"primitiveProxyDoublePrecisionFloatAt:ifFail:", 
	fntype(&proxyOopPrimitives::doublePrecisionFloatAt), 
	1376258, 
	signature_333, 
	errors_333
};

static char* signature_334[] = { "Self", "Proxy", "SmallInteger", "Float"};
static char* errors_334[] = { "ConversionFailed", NULL};
static primitive_desc primitive_334 = {
	"primitiveProxyDoublePrecisionFloatAt:put:ifFail:", 
	fntype(&proxyOopPrimitives::doublePrecisionFloatAtPut), 
	1376259, 
	signature_334, 
	errors_334
};

static char* signature_335[] = { "Self", "Proxy"};
static char* errors_335[] = {NULL};
static primitive_desc primitive_335 = {
	"primitiveProxyFree", 
	fntype(&proxyOopPrimitives::free), 
	1114113, 
	signature_335, 
	errors_335
};

static char* signature_336[] = { "SmallInteger", "Proxy"};
static char* errors_336[] = {NULL};
static primitive_desc primitive_336 = {
	"primitiveProxyGetHigh", 
	fntype(&proxyOopPrimitives::getHigh), 
	1114113, 
	signature_336, 
	errors_336
};

static char* signature_337[] = { "SmallInteger", "Proxy"};
static char* errors_337[] = { "ConversionFailed", NULL};
static primitive_desc primitive_337 = {
	"primitiveProxyGetIfFail:", 
	fntype(&proxyOopPrimitives::getSmi), 
	1376257, 
	signature_337, 
	errors_337
};

static char* signature_338[] = { "SmallInteger", "Proxy"};
static char* errors_338[] = {NULL};
static primitive_desc primitive_338 = {
	"primitiveProxyGetLow", 
	fntype(&proxyOopPrimitives::getLow), 
	1114113, 
	signature_338, 
	errors_338
};

static char* signature_339[] = { "Boolean", "Proxy"};
static char* errors_339[] = {NULL};
static primitive_desc primitive_339 = {
	"primitiveProxyIsAllOnes", 
	fntype(&proxyOopPrimitives::isAllOnes), 
	1114113, 
	signature_339, 
	errors_339
};

static char* signature_340[] = { "Boolean", "Proxy"};
static char* errors_340[] = {NULL};
static primitive_desc primitive_340 = {
	"primitiveProxyIsNull", 
	fntype(&proxyOopPrimitives::isNull), 
	1114113, 
	signature_340, 
	errors_340
};

static char* signature_341[] = { "Self", "Proxy", "SmallInteger"};
static char* errors_341[] = {NULL};
static primitive_desc primitive_341 = {
	"primitiveProxyMalloc:ifFail:", 
	fntype(&proxyOopPrimitives::malloc), 
	1376258, 
	signature_341, 
	errors_341
};

static char* signature_342[] = { "Proxy", "Proxy", "SmallInteger", "Proxy"};
static char* errors_342[] = {NULL};
static primitive_desc primitive_342 = {
	"primitiveProxyProxyAt:put:ifFail:", 
	fntype(&proxyOopPrimitives::proxyAtPut), 
	1376259, 
	signature_342, 
	errors_342
};

static char* signature_343[] = { "Proxy", "Proxy", "SmallInteger", "Proxy"};
static char* errors_343[] = {NULL};
static primitive_desc primitive_343 = {
	"primitiveProxyProxyAt:result:ifFail:", 
	fntype(&proxyOopPrimitives::proxyAt), 
	1376259, 
	signature_343, 
	errors_343
};

static char* signature_344[] = { "Self", "Proxy", "SmallInteger|Proxy"};
static char* errors_344[] = {NULL};
static primitive_desc primitive_344 = {
	"primitiveProxySet:ifFail:", 
	fntype(&proxyOopPrimitives::set), 
	1376258, 
	signature_344, 
	errors_344
};

static char* signature_345[] = { "Self", "Proxy", "SmallInteger", "SmallInteger"};
static char* errors_345[] = {NULL};
static primitive_desc primitive_345 = {
	"primitiveProxySetHigh:low:ifFail:", 
	fntype(&proxyOopPrimitives::setHighLow), 
	1376259, 
	signature_345, 
	errors_345
};

static char* signature_346[] = { "Float", "Proxy", "SmallInteger"};
static char* errors_346[] = {NULL};
static primitive_desc primitive_346 = {
	"primitiveProxySinglePrecisionFloatAt:ifFail:", 
	fntype(&proxyOopPrimitives::singlePrecisionFloatAt), 
	1376258, 
	signature_346, 
	errors_346
};

static char* signature_347[] = { "Self", "Proxy", "SmallInteger", "Float"};
static char* errors_347[] = { "ConversionFailed", NULL};
static primitive_desc primitive_347 = {
	"primitiveProxySinglePrecisionFloatAt:put:ifFail:", 
	fntype(&proxyOopPrimitives::singlePrecisionFloatAtPut), 
	1376259, 
	signature_347, 
	errors_347
};

static char* signature_348[] = { "SmallInteger", "Proxy", "SmallInteger"};
static char* errors_348[] = { "ConversionFailed", NULL};
static primitive_desc primitive_348 = {
	"primitiveProxySmiAt:ifFail:", 
	fntype(&proxyOopPrimitives::smiAt), 
	1376258, 
	signature_348, 
	errors_348
};

static char* signature_349[] = { "SmallInteger", "Proxy", "SmallInteger", "SmallInteger"};
static char* errors_349[] = {NULL};
static primitive_desc primitive_349 = {
	"primitiveProxySmiAt:put:ifFail:", 
	fntype(&proxyOopPrimitives::smiAtPut), 
	1376259, 
	signature_349, 
	errors_349
};

static char* signature_350[] = { "Proxy", "Proxy", "SmallInteger", "Proxy"};
static char* errors_350[] = {NULL};
static primitive_desc primitive_350 = {
	"primitiveProxySubProxyAt:result:ifFail:", 
	fntype(&proxyOopPrimitives::subProxyAt), 
	1376259, 
	signature_350, 
	errors_350
};

static char* signature_351[] = { "BottomType"};
static char* errors_351[] = {NULL};
static primitive_desc primitive_351 = {
	"primitiveQuit", 
	fntype(&systemPrimitives::quit), 
	65536, 
	signature_351, 
	errors_351
};

static char* signature_352[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_352[] = { "NotImplementedYet", NULL};
static primitive_desc primitive_352 = {
	"primitiveQuo:ifFail:", 
	fntype(&smiOopPrimitives_quo), 
	6029826, 
	signature_352, 
	errors_352
};

static char* signature_353[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_353[] = {NULL};
static primitive_desc primitive_353 = {
	"primitiveRawBitShift:ifFail:", 
	fntype(&smiOopPrimitives::rawBitShift), 
	6029826, 
	signature_353, 
	errors_353
};

static char* signature_354[] = { "IndexedInstanceVariables", "Object", "SmallInteger"};
static char* errors_354[] = {NULL};
static primitive_desc primitive_354 = {
	"primitiveReferencesTo:limit:ifFail:", 
	fntype(&systemPrimitives::references_to), 
	327682, 
	signature_354, 
	errors_354
};

static char* signature_355[] = { "IndexedInstanceVariables", "Class", "SmallInteger"};
static char* errors_355[] = {NULL};
static primitive_desc primitive_355 = {
	"primitiveReferencesToInstancesOf:limit:ifFail:", 
	fntype(&systemPrimitives::references_to_instances_of), 
	327682, 
	signature_355, 
	errors_355
};

static char* signature_356[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_356[] = { "DivisionByZero", NULL};
static primitive_desc primitive_356 = {
	"primitiveRemainder:ifFail:", 
	fntype(&smiOopPrimitives_remainder), 
	6029826, 
	signature_356, 
	errors_356
};

static char* signature_357[] = { "BottomType", "BlockWithoutArguments"};
static char* errors_357[] = {NULL};
static primitive_desc primitive_357 = {
	"primitiveRepeat", 
	fntype(&blockRepeat), 
	1245185, 
	signature_357, 
	errors_357
};

static char* signature_358[] = { "Self", "Object"};
static char* errors_358[] = {NULL};
static primitive_desc primitive_358 = {
	"primitiveScavenge", 
	fntype(&systemPrimitives::scavenge), 
	1114113, 
	signature_358, 
	errors_358
};

static char* signature_359[] = { "Object", "SmallInteger"};
static char* errors_359[] = {NULL};
static primitive_desc primitive_359 = {
	"primitiveSetInterpreterInvocationCounterLimitTo:ifFail:", 
	fntype(&debugPrimitives::setInterpreterInvocationCounterLimit), 
	327681, 
	signature_359, 
	errors_359
};

static char* signature_360[] = { "Object", "Object"};
static char* errors_360[] = { "ReceiverHasWrongType", NULL};
static primitive_desc primitive_360 = {
	"primitiveShallowCopyIfFail:", 
	fntype(&oopPrimitives::shallowCopy), 
	1376257, 
	signature_360, 
	errors_360
};

static char* signature_361[] = { "Object", "SmallInteger"};
static char* errors_361[] = {NULL};
static primitive_desc primitive_361 = {
	"primitiveShrinkMemory:", 
	fntype(&systemPrimitives::shrinkMemory), 
	65537, 
	signature_361, 
	errors_361
};

static char* signature_362[] = { "SmallInteger"};
static char* errors_362[] = {NULL};
static primitive_desc primitive_362 = {
	"primitiveSizeOfOop", 
	fntype(&systemPrimitives::oopSize), 
	65536, 
	signature_362, 
	errors_362
};

static char* signature_363[] = { "IndexedInstanceVariables"};
static char* errors_363[] = {NULL};
static primitive_desc primitive_363 = {
	"primitiveSlidingSystemAverageIfFail:", 
	fntype(&systemPrimitives::sliding_system_average), 
	327680, 
	signature_363, 
	errors_363
};

static char* signature_364[] = { "Boolean", "SmallInteger", "SmallInteger"};
static char* errors_364[] = {NULL};
static primitive_desc primitive_364 = {
	"primitiveSmallIntegerEqual:ifFail:", 
	fntype(&smiOopPrimitives::equal), 
	6029570, 
	signature_364, 
	errors_364
};

static char* signature_365[] = { "SmallInteger", "Symbol"};
static char* errors_365[] = { "NotFound", NULL};
static primitive_desc primitive_365 = {
	"primitiveSmallIntegerFlagAt:ifFail:", 
	fntype(&debugPrimitives::smiAt), 
	327681, 
	signature_365, 
	errors_365
};

static char* signature_366[] = { "Boolean", "Symbol", "Boolean"};
static char* errors_366[] = { "NotFound", NULL};
static primitive_desc primitive_366 = {
	"primitiveSmallIntegerFlagAt:put:ifFail:", 
	fntype(&debugPrimitives::smiAtPut), 
	327682, 
	signature_366, 
	errors_366
};

static char* signature_367[] = { "Boolean", "SmallInteger", "SmallInteger"};
static char* errors_367[] = {NULL};
static primitive_desc primitive_367 = {
	"primitiveSmallIntegerNotEqual:ifFail:", 
	fntype(&smiOopPrimitives::notEqual), 
	6029570, 
	signature_367, 
	errors_367
};

static char* signature_368[] = { "IndexedInstanceVariables"};
static char* errors_368[] = {NULL};
static primitive_desc primitive_368 = {
	"primitiveSmalltalkArray", 
	fntype(&systemPrimitives::smalltalk_array), 
	65536, 
	signature_368, 
	errors_368
};

static char* signature_369[] = { "GlobalAssociation", "Symbol", "Object"};
static char* errors_369[] = {NULL};
static primitive_desc primitive_369 = {
	"primitiveSmalltalkAt:Put:ifFail:", 
	fntype(&systemPrimitives::smalltalk_at_put), 
	327682, 
	signature_369, 
	errors_369
};

static char* signature_370[] = { "GlobalAssociation", "SmallInteger"};
static char* errors_370[] = { "OutOfBounds", NULL};
static primitive_desc primitive_370 = {
	"primitiveSmalltalkAt:ifFail:", 
	fntype(&systemPrimitives::smalltalk_at), 
	327681, 
	signature_370, 
	errors_370
};

static char* signature_371[] = { "GlobalAssociation", "SmallInteger"};
static char* errors_371[] = {NULL};
static primitive_desc primitive_371 = {
	"primitiveSmalltalkRemoveAt:ifFail:", 
	fntype(&systemPrimitives::smalltalk_remove_at), 
	327681, 
	signature_371, 
	errors_371
};

static char* signature_372[] = { "SmallInteger"};
static char* errors_372[] = {NULL};
static primitive_desc primitive_372 = {
	"primitiveSmalltalkSize", 
	fntype(&systemPrimitives::smalltalk_size), 
	65536, 
	signature_372, 
	errors_372
};

static char* signature_373[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_373[] = { "Overflow", NULL};
static primitive_desc primitive_373 = {
	"primitiveSubtract:ifFail:", 
	fntype(&smiOopPrimitives_subtract), 
	6029826, 
	signature_373, 
	errors_373
};

static char* signature_374[] = { "Behavior|Nil", "Behavior"};
static char* errors_374[] = {NULL};
static primitive_desc primitive_374 = {
	"primitiveSuperclass", 
	fntype(&behaviorPrimitives::superclass), 
	1114113, 
	signature_374, 
	errors_374
};

static char* signature_375[] = { "Behavior|Nil", "Behavior"};
static char* errors_375[] = {NULL};
static primitive_desc primitive_375 = {
	"primitiveSuperclassOf:ifFail:", 
	fntype(&behaviorPrimitives::superclass_of), 
	327681, 
	signature_375, 
	errors_375
};

static char* signature_376[] = { "Float"};
static char* errors_376[] = {NULL};
static primitive_desc primitive_376 = {
	"primitiveSystemTime", 
	fntype(&systemPrimitives::systemTime), 
	65536, 
	signature_376, 
	errors_376
};

static char* signature_377[] = { "Object"};
static char* errors_377[] = {NULL};
static primitive_desc primitive_377 = {
	"primitiveTimerPrintBuffer", 
	fntype(&debugPrimitives::timerPrintBuffer), 
	65536, 
	signature_377, 
	errors_377
};

static char* signature_378[] = { "Object"};
static char* errors_378[] = {NULL};
static primitive_desc primitive_378 = {
	"primitiveTimerStart", 
	fntype(&debugPrimitives::timerStart), 
	65536, 
	signature_378, 
	errors_378
};

static char* signature_379[] = { "Object"};
static char* errors_379[] = {NULL};
static primitive_desc primitive_379 = {
	"primitiveTimerStop", 
	fntype(&debugPrimitives::timerStop), 
	65536, 
	signature_379, 
	errors_379
};

static char* signature_380[] = { "Object"};
static char* errors_380[] = {NULL};
static primitive_desc primitive_380 = {
	"primitiveTraceStack", 
	fntype(&systemPrimitives::traceStack), 
	65536, 
	signature_380, 
	errors_380
};

static char* signature_381[] = { "Object", "BlockWithoutArguments", "BlockWithoutArguments"};
static char* errors_381[] = {NULL};
static primitive_desc primitive_381 = {
	"primitiveUnwindProtect:ifFail:", 
	fntype(&unwindprotect), 
	1507330, 
	signature_381, 
	errors_381
};

static char* signature_382[] = { "Float"};
static char* errors_382[] = {NULL};
static primitive_desc primitive_382 = {
	"primitiveUserTime", 
	fntype(&systemPrimitives::userTime), 
	65536, 
	signature_382, 
	errors_382
};

static char* signature_383[] = { "Object"};
static char* errors_383[] = {NULL};
static primitive_desc primitive_383 = {
	"primitiveVMBreakpoint", 
	fntype(&systemPrimitives::vmbreakpoint), 
	65536, 
	signature_383, 
	errors_383
};

static char* signature_384[] = { "Object", "BlockWithoutArguments"};
static char* errors_384[] = {NULL};
static primitive_desc primitive_384 = {
	"primitiveValue", 
	fntype(&primitiveValue0), 
	5441537, 
	signature_384, 
	errors_384
};

static char* signature_385[] = { "Object", "BlockWithOneArgument", "Object"};
static char* errors_385[] = {NULL};
static primitive_desc primitive_385 = {
	"primitiveValue:", 
	fntype(&primitiveValue1), 
	5441538, 
	signature_385, 
	errors_385
};

static char* signature_386[] = { "Object", "BlockWithTwoArguments", "Object", "Object"};
static char* errors_386[] = {NULL};
static primitive_desc primitive_386 = {
	"primitiveValue:value:", 
	fntype(&primitiveValue2), 
	5441539, 
	signature_386, 
	errors_386
};

static char* signature_387[] = { "Object", "BlockWithThreeArguments", "Object", "Object", "Object"};
static char* errors_387[] = {NULL};
static primitive_desc primitive_387 = {
	"primitiveValue:value:value:", 
	fntype(&primitiveValue3), 
	5441540, 
	signature_387, 
	errors_387
};

static char* signature_388[] = { "Object", "BlockWithFourArguments", "Object", "Object", "Object", "Object"};
static char* errors_388[] = {NULL};
static primitive_desc primitive_388 = {
	"primitiveValue:value:value:value:", 
	fntype(&primitiveValue4), 
	5441541, 
	signature_388, 
	errors_388
};

static char* signature_389[] = { "Object", "BlockWithFiveArguments", "Object", "Object", "Object", "Object", "Object"};
static char* errors_389[] = {NULL};
static primitive_desc primitive_389 = {
	"primitiveValue:value:value:value:value:", 
	fntype(&primitiveValue5), 
	5441542, 
	signature_389, 
	errors_389
};

static char* signature_390[] = { "Object", "BlockWithSixArguments", "Object", "Object", "Object", "Object", "Object", "Object"};
static char* errors_390[] = {NULL};
static primitive_desc primitive_390 = {
	"primitiveValue:value:value:value:value:value:", 
	fntype(&primitiveValue6), 
	5441543, 
	signature_390, 
	errors_390
};

static char* signature_391[] = { "Object", "BlockWithSevenArguments", "Object", "Object", "Object", "Object", "Object", "Object", "Object"};
static char* errors_391[] = {NULL};
static primitive_desc primitive_391 = {
	"primitiveValue:value:value:value:value:value:value:", 
	fntype(&primitiveValue7), 
	5441544, 
	signature_391, 
	errors_391
};

static char* signature_392[] = { "Object", "BlockWithEightArguments", "Object", "Object", "Object", "Object", "Object", "Object", "Object", "Object"};
static char* errors_392[] = {NULL};
static primitive_desc primitive_392 = {
	"primitiveValue:value:value:value:value:value:value:value:", 
	fntype(&primitiveValue8), 
	5441545, 
	signature_392, 
	errors_392
};

static char* signature_393[] = { "Object", "BlockWithNineArguments", "Object", "Object", "Object", "Object", "Object", "Object", "Object", "Object", "Object"};
static char* errors_393[] = {NULL};
static primitive_desc primitive_393 = {
	"primitiveValue:value:value:value:value:value:value:value:value:", 
	fntype(&primitiveValue9), 
	5441546, 
	signature_393, 
	errors_393
};

static char* signature_394[] = { "Object"};
static char* errors_394[] = {NULL};
static primitive_desc primitive_394 = {
	"primitiveVerify", 
	fntype(&debugPrimitives::verify), 
	65536, 
	signature_394, 
	errors_394
};

static char* signature_395[] = { "Proxy", "Proxy"};
static char* errors_395[] = {NULL};
static primitive_desc primitive_395 = {
	"primitiveWindowsHInstance:ifFail:", 
	fntype(&systemPrimitives::windowsHInstance), 
	327681, 
	signature_395, 
	errors_395
};

static char* signature_396[] = { "Proxy", "Proxy"};
static char* errors_396[] = {NULL};
static primitive_desc primitive_396 = {
	"primitiveWindowsHPrevInstance:ifFail:", 
	fntype(&systemPrimitives::windowsHPrevInstance), 
	327681, 
	signature_396, 
	errors_396
};

static char* signature_397[] = { "Object"};
static char* errors_397[] = {NULL};
static primitive_desc primitive_397 = {
	"primitiveWindowsNCmdShow", 
	fntype(&systemPrimitives::windowsNCmdShow), 
	65536, 
	signature_397, 
	errors_397
};

static char* signature_398[] = { "Object", "String"};
static char* errors_398[] = {NULL};
static primitive_desc primitive_398 = {
	"primitiveWriteSnapshot:", 
	fntype(&systemPrimitives::writeSnapshot), 
	65537, 
	signature_398, 
	errors_398
};

static int size_of_primitive_table = 399; 
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
  &primitive_393,
  &primitive_394,
  &primitive_395,
  &primitive_396,
  &primitive_397,
  &primitive_398
}; 
