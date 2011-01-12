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

static char* signature_21[] = { "IndexedByteInstanceVariables", "IndexedByteInstanceVariables", "Integer"};
static char* errors_21[] = {NULL};
static primitive_desc primitive_21 = {
	"primitiveAlienAddress:ifFail:", 
	fntype(&byteArrayPrimitives::alienSetAddress), 
	1312258, 
	signature_21, 
	errors_21
};

static char* signature_22[] = { "Integer", "IndexedByteInstanceVariables"};
static char* errors_22[] = {NULL};
static primitive_desc primitive_22 = {
	"primitiveAlienAddressIfFail:", 
	fntype(&byteArrayPrimitives::alienGetAddress), 
	1312257, 
	signature_22, 
	errors_22
};

static char* signature_23[] = { "Integer", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_23[] = {NULL};
static primitive_desc primitive_23 = {
	"primitiveAlienCallResult:ifFail:", 
	fntype(&byteArrayPrimitives::alienCallResult0), 
	1312258, 
	signature_23, 
	errors_23
};

static char* signature_24[] = { "Integer", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables|SmallInteger"};
static char* errors_24[] = {NULL};
static primitive_desc primitive_24 = {
	"primitiveAlienCallResult:with:ifFail:", 
	fntype(&byteArrayPrimitives::alienCallResult1), 
	1312259, 
	signature_24, 
	errors_24
};

static char* signature_25[] = { "Integer", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger"};
static char* errors_25[] = {NULL};
static primitive_desc primitive_25 = {
	"primitiveAlienCallResult:with:with:ifFail:", 
	fntype(&byteArrayPrimitives::alienCallResult2), 
	1312260, 
	signature_25, 
	errors_25
};

static char* signature_26[] = { "Integer", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger"};
static char* errors_26[] = {NULL};
static primitive_desc primitive_26 = {
	"primitiveAlienCallResult:with:with:with:ifFail:", 
	fntype(&byteArrayPrimitives::alienCallResult3), 
	1312261, 
	signature_26, 
	errors_26
};

static char* signature_27[] = { "Integer", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger"};
static char* errors_27[] = {NULL};
static primitive_desc primitive_27 = {
	"primitiveAlienCallResult:with:with:with:with:ifFail:", 
	fntype(&byteArrayPrimitives::alienCallResult4), 
	1312262, 
	signature_27, 
	errors_27
};

static char* signature_28[] = { "Integer", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger"};
static char* errors_28[] = {NULL};
static primitive_desc primitive_28 = {
	"primitiveAlienCallResult:with:with:with:with:with:ifFail:", 
	fntype(&byteArrayPrimitives::alienCallResult5), 
	1312263, 
	signature_28, 
	errors_28
};

static char* signature_29[] = { "Integer", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger"};
static char* errors_29[] = {NULL};
static primitive_desc primitive_29 = {
	"primitiveAlienCallResult:with:with:with:with:with:with:ifFail:", 
	fntype(&byteArrayPrimitives::alienCallResult6), 
	1312264, 
	signature_29, 
	errors_29
};

static char* signature_30[] = { "Integer", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables|SmallInteger"};
static char* errors_30[] = {NULL};
static primitive_desc primitive_30 = {
	"primitiveAlienCallResult:with:with:with:with:with:with:with:ifFail:", 
	fntype(&byteArrayPrimitives::alienCallResult7), 
	1312265, 
	signature_30, 
	errors_30
};

static char* signature_31[] = { "Integer", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables", "IndexedInstanceVariables"};
static char* errors_31[] = {NULL};
static primitive_desc primitive_31 = {
	"primitiveAlienCallResult:withArguments:ifFail:", 
	fntype(&byteArrayPrimitives::alienCallResultWithArguments), 
	1312259, 
	signature_31, 
	errors_31
};

static char* signature_32[] = { "Float", "SmallInteger"};
static char* errors_32[] = {NULL};
static primitive_desc primitive_32 = {
	"primitiveAlienCalloc:ifFail:", 
	fntype(&systemPrimitives::alienCalloc), 
	327681, 
	signature_32, 
	errors_32
};

static char* signature_33[] = { "Double", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_33[] = {NULL};
static primitive_desc primitive_33 = {
	"primitiveAlienDoubleAt:ifFail:", 
	fntype(&byteArrayPrimitives::alienDoubleAt), 
	1312258, 
	signature_33, 
	errors_33
};

static char* signature_34[] = { "Double", "IndexedByteInstanceVariables", "SmallInteger", "Double"};
static char* errors_34[] = {NULL};
static primitive_desc primitive_34 = {
	"primitiveAlienDoubleAt:put:ifFail:", 
	fntype(&byteArrayPrimitives::alienDoubleAtPut), 
	1312259, 
	signature_34, 
	errors_34
};

static char* signature_35[] = { "Double", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_35[] = {NULL};
static primitive_desc primitive_35 = {
	"primitiveAlienFloatAt:ifFail:", 
	fntype(&byteArrayPrimitives::alienFloatAt), 
	1312258, 
	signature_35, 
	errors_35
};

static char* signature_36[] = { "Double", "IndexedByteInstanceVariables", "SmallInteger", "Double"};
static char* errors_36[] = {NULL};
static primitive_desc primitive_36 = {
	"primitiveAlienFloatAt:put:ifFail:", 
	fntype(&byteArrayPrimitives::alienFloatAtPut), 
	1312259, 
	signature_36, 
	errors_36
};

static char* signature_37[] = { "Float", "SmallInteger|LargeInteger"};
static char* errors_37[] = {NULL};
static primitive_desc primitive_37 = {
	"primitiveAlienFree:ifFail:", 
	fntype(&systemPrimitives::alienFree), 
	327681, 
	signature_37, 
	errors_37
};

static char* signature_38[] = { "Float", "SmallInteger"};
static char* errors_38[] = {NULL};
static primitive_desc primitive_38 = {
	"primitiveAlienMalloc:ifFail:", 
	fntype(&systemPrimitives::alienMalloc), 
	327681, 
	signature_38, 
	errors_38
};

static char* signature_39[] = { "SmallInteger", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_39[] = {NULL};
static primitive_desc primitive_39 = {
	"primitiveAlienSignedByteAt:ifFail:", 
	fntype(&byteArrayPrimitives::alienSignedByteAt), 
	1312258, 
	signature_39, 
	errors_39
};

static char* signature_40[] = { "SmallInteger", "IndexedByteInstanceVariables", "SmallInteger", "SmallInteger"};
static char* errors_40[] = {NULL};
static primitive_desc primitive_40 = {
	"primitiveAlienSignedByteAt:put:ifFail:", 
	fntype(&byteArrayPrimitives::alienSignedByteAtPut), 
	1312259, 
	signature_40, 
	errors_40
};

static char* signature_41[] = { "SmallInteger|LargeInteger", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_41[] = {NULL};
static primitive_desc primitive_41 = {
	"primitiveAlienSignedLongAt:ifFail:", 
	fntype(&byteArrayPrimitives::alienSignedLongAt), 
	1312258, 
	signature_41, 
	errors_41
};

static char* signature_42[] = { "SmallInteger|LargeInteger", "IndexedByteInstanceVariables", "SmallInteger", "SmallInteger|LargeInteger"};
static char* errors_42[] = {NULL};
static primitive_desc primitive_42 = {
	"primitiveAlienSignedLongAt:put:ifFail:", 
	fntype(&byteArrayPrimitives::alienSignedLongAtPut), 
	1312259, 
	signature_42, 
	errors_42
};

static char* signature_43[] = { "SmallInteger", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_43[] = {NULL};
static primitive_desc primitive_43 = {
	"primitiveAlienSignedShortAt:ifFail:", 
	fntype(&byteArrayPrimitives::alienSignedShortAt), 
	1312258, 
	signature_43, 
	errors_43
};

static char* signature_44[] = { "SmallInteger", "IndexedByteInstanceVariables", "SmallInteger", "SmallInteger"};
static char* errors_44[] = {NULL};
static primitive_desc primitive_44 = {
	"primitiveAlienSignedShortAt:put:ifFail:", 
	fntype(&byteArrayPrimitives::alienSignedShortAtPut), 
	1312259, 
	signature_44, 
	errors_44
};

static char* signature_45[] = { "IndexedByteInstanceVariables", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_45[] = {NULL};
static primitive_desc primitive_45 = {
	"primitiveAlienSize:ifFail:", 
	fntype(&byteArrayPrimitives::alienSetSize), 
	1312258, 
	signature_45, 
	errors_45
};

static char* signature_46[] = { "SmallInteger", "IndexedByteInstanceVariables"};
static char* errors_46[] = {NULL};
static primitive_desc primitive_46 = {
	"primitiveAlienSizeIfFail:", 
	fntype(&byteArrayPrimitives::alienGetSize), 
	1312257, 
	signature_46, 
	errors_46
};

static char* signature_47[] = { "SmallInteger", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_47[] = {NULL};
static primitive_desc primitive_47 = {
	"primitiveAlienUnsignedByteAt:ifFail:", 
	fntype(&byteArrayPrimitives::alienUnsignedByteAt), 
	1312258, 
	signature_47, 
	errors_47
};

static char* signature_48[] = { "SmallInteger", "IndexedByteInstanceVariables", "SmallInteger", "SmallInteger"};
static char* errors_48[] = {NULL};
static primitive_desc primitive_48 = {
	"primitiveAlienUnsignedByteAt:put:ifFail:", 
	fntype(&byteArrayPrimitives::alienUnsignedByteAtPut), 
	1312259, 
	signature_48, 
	errors_48
};

static char* signature_49[] = { "SmallInteger|LargeInteger", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_49[] = {NULL};
static primitive_desc primitive_49 = {
	"primitiveAlienUnsignedLongAt:ifFail:", 
	fntype(&byteArrayPrimitives::alienUnsignedLongAt), 
	1312258, 
	signature_49, 
	errors_49
};

static char* signature_50[] = { "SmallInteger|LargeInteger", "IndexedByteInstanceVariables", "SmallInteger", "SmallInteger|LargeInteger"};
static char* errors_50[] = {NULL};
static primitive_desc primitive_50 = {
	"primitiveAlienUnsignedLongAt:put:ifFail:", 
	fntype(&byteArrayPrimitives::alienUnsignedLongAtPut), 
	1312259, 
	signature_50, 
	errors_50
};

static char* signature_51[] = { "SmallInteger", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_51[] = {NULL};
static primitive_desc primitive_51 = {
	"primitiveAlienUnsignedShortAt:ifFail:", 
	fntype(&byteArrayPrimitives::alienUnsignedShortAt), 
	1312258, 
	signature_51, 
	errors_51
};

static char* signature_52[] = { "SmallInteger", "IndexedByteInstanceVariables", "SmallInteger", "SmallInteger"};
static char* errors_52[] = {NULL};
static primitive_desc primitive_52 = {
	"primitiveAlienUnsignedShortAt:put:ifFail:", 
	fntype(&byteArrayPrimitives::alienUnsignedShortAtPut), 
	1312259, 
	signature_52, 
	errors_52
};

static char* signature_53[] = { "IndexedInstanceVariables", "SmallInteger"};
static char* errors_53[] = {NULL};
static primitive_desc primitive_53 = {
	"primitiveAllObjectsLimit:ifFail:", 
	fntype(&systemPrimitives::all_objects), 
	327681, 
	signature_53, 
	errors_53
};

static char* signature_54[] = { "Object", "IndexedInstanceVariables"};
static char* errors_54[] = {NULL};
static primitive_desc primitive_54 = {
	"primitiveApplyChange:ifFail:", 
	fntype(&systemPrimitives::applyChange), 
	327681, 
	signature_54, 
	errors_54
};

static char* signature_55[] = { "Float", "SmallInteger"};
static char* errors_55[] = {NULL};
static primitive_desc primitive_55 = {
	"primitiveAsFloat", 
	fntype(&double_from_smi), 
	1573377, 
	signature_55, 
	errors_55
};

static char* signature_56[] = { "SmallInteger", "Object"};
static char* errors_56[] = {NULL};
static primitive_desc primitive_56 = {
	"primitiveAsObjectID", 
	fntype(&oopPrimitives::asObjectID), 
	1114113, 
	signature_56, 
	errors_56
};

static char* signature_57[] = { "Object", "SmallInteger"};
static char* errors_57[] = { "OutOfBounds", NULL};
static primitive_desc primitive_57 = {
	"primitiveAsObjectIfFail:", 
	fntype(&smiOopPrimitives::asObject), 
	1310721, 
	signature_57, 
	errors_57
};

static char* signature_58[] = { "Object", "Object", "Object"};
static char* errors_58[] = { "RecieverHasWrongType", NULL};
static primitive_desc primitive_58 = {
	"primitiveBecome:ifFail:", 
	fntype(&oopPrimitives::become), 
	1376258, 
	signature_58, 
	errors_58
};

static char* signature_59[] = { "GlobalAssociation", "Behavior", "SmallInteger"};
static char* errors_59[] = { "OutOfBounds", NULL};
static primitive_desc primitive_59 = {
	"primitiveBehavior:classVariableAt:ifFail:", 
	fntype(&behaviorPrimitives::classVariableAt), 
	327682, 
	signature_59, 
	errors_59
};

static char* signature_60[] = { "IndexedInstanceVariables", "Behavior"};
static char* errors_60[] = { "OutOfBounds", NULL};
static primitive_desc primitive_60 = {
	"primitiveBehavior:classVariablesIfFail:", 
	fntype(&behaviorPrimitives::classVariables), 
	327681, 
	signature_60, 
	errors_60
};

static char* signature_61[] = { "Boolean", "Behavior"};
static char* errors_61[] = {NULL};
static primitive_desc primitive_61 = {
	"primitiveBehaviorCanBeSubclassed:ifFail:", 
	fntype(&behaviorPrimitives::can_be_subclassed), 
	327681, 
	signature_61, 
	errors_61
};

static char* signature_62[] = { "Boolean", "Behavior"};
static char* errors_62[] = {NULL};
static primitive_desc primitive_62 = {
	"primitiveBehaviorCanHaveInstanceVariables:ifFail:", 
	fntype(&behaviorPrimitives::can_have_instance_variables), 
	327681, 
	signature_62, 
	errors_62
};

static char* signature_63[] = { "Symbol", "Behavior"};
static char* errors_63[] = {NULL};
static primitive_desc primitive_63 = {
	"primitiveBehaviorFormat:ifFail:", 
	fntype(&behaviorPrimitives::format), 
	327681, 
	signature_63, 
	errors_63
};

static char* signature_64[] = { "SmallInteger", "Behavior"};
static char* errors_64[] = {NULL};
static primitive_desc primitive_64 = {
	"primitiveBehaviorHeaderSizeOf:ifFail:", 
	fntype(&behaviorPrimitives::headerSize), 
	327681, 
	signature_64, 
	errors_64
};

static char* signature_65[] = { "Boolean", "Behavior", "Object"};
static char* errors_65[] = {NULL};
static primitive_desc primitive_65 = {
	"primitiveBehaviorIsClassOf:", 
	fntype(&behaviorPrimitives::is_class_of), 
	5308418, 
	signature_65, 
	errors_65
};

static char* signature_66[] = { "Boolean", "Behavior"};
static char* errors_66[] = {NULL};
static primitive_desc primitive_66 = {
	"primitiveBehaviorIsSpecializedClass:ifFail:", 
	fntype(&behaviorPrimitives::is_specialized_class), 
	327681, 
	signature_66, 
	errors_66
};

static char* signature_67[] = { "Mixin", "Behavior"};
static char* errors_67[] = {NULL};
static primitive_desc primitive_67 = {
	"primitiveBehaviorMixinOf:ifFail:", 
	fntype(&behaviorPrimitives::mixinOf), 
	327681, 
	signature_67, 
	errors_67
};

static char* signature_68[] = { "SmallInteger", "Behavior"};
static char* errors_68[] = {NULL};
static primitive_desc primitive_68 = {
	"primitiveBehaviorNonIndexableSizeOf:ifFail:", 
	fntype(&behaviorPrimitives::nonIndexableSize), 
	327681, 
	signature_68, 
	errors_68
};

static char* signature_69[] = { "Symbol", "Behavior"};
static char* errors_69[] = {NULL};
static primitive_desc primitive_69 = {
	"primitiveBehaviorVMType:ifFail:", 
	fntype(&behaviorPrimitives::vm_type), 
	327681, 
	signature_69, 
	errors_69
};

static char* signature_70[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_70[] = {NULL};
static primitive_desc primitive_70 = {
	"primitiveBitAnd:ifFail:", 
	fntype(&smiOopPrimitives::bitAnd), 
	6029826, 
	signature_70, 
	errors_70
};

static char* signature_71[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_71[] = {NULL};
static primitive_desc primitive_71 = {
	"primitiveBitOr:ifFail:", 
	fntype(&smiOopPrimitives::bitOr), 
	6029826, 
	signature_71, 
	errors_71
};

static char* signature_72[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_72[] = {NULL};
static primitive_desc primitive_72 = {
	"primitiveBitShift:ifFail:", 
	fntype(&smiOopPrimitives::bitShift), 
	6029826, 
	signature_72, 
	errors_72
};

static char* signature_73[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_73[] = {NULL};
static primitive_desc primitive_73 = {
	"primitiveBitXor:ifFail:", 
	fntype(&smiOopPrimitives::bitXor), 
	6029826, 
	signature_73, 
	errors_73
};

static char* signature_74[] = { "Boolean", "Block"};
static char* errors_74[] = {NULL};
static primitive_desc primitive_74 = {
	"primitiveBlockIsOptimized", 
	fntype(&block_is_optimized), 
	1114113, 
	signature_74, 
	errors_74
};

static char* signature_75[] = { "Method", "Block"};
static char* errors_75[] = {NULL};
static primitive_desc primitive_75 = {
	"primitiveBlockMethod", 
	fntype(&block_method), 
	1114113, 
	signature_75, 
	errors_75
};

static char* signature_76[] = { "Boolean", "Symbol"};
static char* errors_76[] = { "NotFound", NULL};
static primitive_desc primitive_76 = {
	"primitiveBooleanFlagAt:ifFail:", 
	fntype(&debugPrimitives::boolAt), 
	327681, 
	signature_76, 
	errors_76
};

static char* signature_77[] = { "Boolean", "Symbol", "Boolean"};
static char* errors_77[] = { "NotFound", NULL};
static primitive_desc primitive_77 = {
	"primitiveBooleanFlagAt:put:ifFail:", 
	fntype(&debugPrimitives::boolAtPut), 
	327682, 
	signature_77, 
	errors_77
};

static char* signature_78[] = { "Object"};
static char* errors_78[] = {NULL};
static primitive_desc primitive_78 = {
	"primitiveBreakpoint", 
	fntype(&systemPrimitives::breakpoint), 
	65536, 
	signature_78, 
	errors_78
};

static char* signature_79[] = { "Object", "Object", "Proxy"};
static char* errors_79[] = {NULL};
static primitive_desc primitive_79 = {
	"primitiveCallBackInvokeC2:ifFail:", 
	fntype(&callBackPrimitives::invokeC), 
	1376258, 
	signature_79, 
	errors_79
};

static char* signature_80[] = { "Object", "Object", "Proxy"};
static char* errors_80[] = {NULL};
static primitive_desc primitive_80 = {
	"primitiveCallBackInvokePascal2:ifFail:", 
	fntype(&callBackPrimitives::invokePascal), 
	1376258, 
	signature_80, 
	errors_80
};

static char* signature_81[] = { "Object", "Object", "Symbol"};
static char* errors_81[] = {NULL};
static primitive_desc primitive_81 = {
	"primitiveCallBackReceiver:selector:ifFail:", 
	fntype(&callBackPrimitives::initialize), 
	327682, 
	signature_81, 
	errors_81
};

static char* signature_82[] = { "Object", "SmallInteger", "Proxy"};
static char* errors_82[] = {NULL};
static primitive_desc primitive_82 = {
	"primitiveCallBackRegisterCCall:result:ifFail:", 
	fntype(&callBackPrimitives::registerCCall), 
	327682, 
	signature_82, 
	errors_82
};

static char* signature_83[] = { "Object", "SmallInteger", "SmallInteger", "Proxy"};
static char* errors_83[] = {NULL};
static primitive_desc primitive_83 = {
	"primitiveCallBackRegisterPascalCall:numberOfArguments:result:ifFail:", 
	fntype(&callBackPrimitives::registerPascalCall), 
	327683, 
	signature_83, 
	errors_83
};

static char* signature_84[] = { "Object", "Object", "Proxy"};
static char* errors_84[] = {NULL};
static primitive_desc primitive_84 = {
	"primitiveCallBackUnregister:ifFail:", 
	fntype(&callBackPrimitives::unregister), 
	1376258, 
	signature_84, 
	errors_84
};

static char* signature_85[] = { "Boolean"};
static char* errors_85[] = {NULL};
static primitive_desc primitive_85 = {
	"primitiveCanScavenge", 
	fntype(&systemPrimitives::canScavenge), 
	65536, 
	signature_85, 
	errors_85
};

static char* signature_86[] = { "Proxy", "SmallInteger"};
static char* errors_86[] = { "OutOfBounds", NULL};
static primitive_desc primitive_86 = {
	"primitiveCharacterFor:ifFail:", 
	fntype(&systemPrimitives::characterFor), 
	327681, 
	signature_86, 
	errors_86
};

static char* signature_87[] = { "Self class", "Object"};
static char* errors_87[] = {NULL};
static primitive_desc primitive_87 = {
	"primitiveClass", 
	fntype(&oopPrimitives::klass), 
	5767169, 
	signature_87, 
	errors_87
};

static char* signature_88[] = { "Behavior", "Object"};
static char* errors_88[] = {NULL};
static primitive_desc primitive_88 = {
	"primitiveClassOf:", 
	fntype(&oopPrimitives::klass_of), 
	4718593, 
	signature_88, 
	errors_88
};

static char* signature_89[] = { "Object"};
static char* errors_89[] = {NULL};
static primitive_desc primitive_89 = {
	"primitiveClearInlineCaches", 
	fntype(&debugPrimitives::clearInlineCaches), 
	65536, 
	signature_89, 
	errors_89
};

static char* signature_90[] = { "Object"};
static char* errors_90[] = {NULL};
static primitive_desc primitive_90 = {
	"primitiveClearInvocationCounters", 
	fntype(&debugPrimitives::clearInvocationCounters), 
	65536, 
	signature_90, 
	errors_90
};

static char* signature_91[] = { "Object"};
static char* errors_91[] = {NULL};
static primitive_desc primitive_91 = {
	"primitiveClearLookupCache", 
	fntype(&debugPrimitives::clearLookupCache), 
	65536, 
	signature_91, 
	errors_91
};

static char* signature_92[] = { "Object"};
static char* errors_92[] = {NULL};
static primitive_desc primitive_92 = {
	"primitiveClearLookupCacheStatistics", 
	fntype(&debugPrimitives::clearLookupCacheStatistics), 
	65536, 
	signature_92, 
	errors_92
};

static char* signature_93[] = { "Object"};
static char* errors_93[] = {NULL};
static primitive_desc primitive_93 = {
	"primitiveClearNMethodCounters", 
	fntype(&debugPrimitives::clearNMethodCounters), 
	65536, 
	signature_93, 
	errors_93
};

static char* signature_94[] = { "Object"};
static char* errors_94[] = {NULL};
static primitive_desc primitive_94 = {
	"primitiveClearPrimitiveCounters", 
	fntype(&debugPrimitives::clearPrimitiveCounters), 
	65536, 
	signature_94, 
	errors_94
};

static char* signature_95[] = { "Array[String]"};
static char* errors_95[] = {NULL};
static primitive_desc primitive_95 = {
	"primitiveCommandLineArgs", 
	fntype(&systemPrimitives::command_line_args), 
	65536, 
	signature_95, 
	errors_95
};

static char* signature_96[] = { "Block"};
static char* errors_96[] = {NULL};
static primitive_desc primitive_96 = {
	"primitiveCompiledBlockAllocate0", 
	fntype(&allocateBlock0), 
	6359040, 
	signature_96, 
	errors_96
};

static char* signature_97[] = { "Block"};
static char* errors_97[] = {NULL};
static primitive_desc primitive_97 = {
	"primitiveCompiledBlockAllocate1", 
	fntype(&allocateBlock1), 
	6359040, 
	signature_97, 
	errors_97
};

static char* signature_98[] = { "Block"};
static char* errors_98[] = {NULL};
static primitive_desc primitive_98 = {
	"primitiveCompiledBlockAllocate2", 
	fntype(&allocateBlock2), 
	6359040, 
	signature_98, 
	errors_98
};

static char* signature_99[] = { "Block"};
static char* errors_99[] = {NULL};
static primitive_desc primitive_99 = {
	"primitiveCompiledBlockAllocate3", 
	fntype(&allocateBlock3), 
	6359040, 
	signature_99, 
	errors_99
};

static char* signature_100[] = { "Block"};
static char* errors_100[] = {NULL};
static primitive_desc primitive_100 = {
	"primitiveCompiledBlockAllocate4", 
	fntype(&allocateBlock4), 
	6359040, 
	signature_100, 
	errors_100
};

static char* signature_101[] = { "Block"};
static char* errors_101[] = {NULL};
static primitive_desc primitive_101 = {
	"primitiveCompiledBlockAllocate5", 
	fntype(&allocateBlock5), 
	6359040, 
	signature_101, 
	errors_101
};

static char* signature_102[] = { "Block"};
static char* errors_102[] = {NULL};
static primitive_desc primitive_102 = {
	"primitiveCompiledBlockAllocate6", 
	fntype(&allocateBlock6), 
	6359040, 
	signature_102, 
	errors_102
};

static char* signature_103[] = { "Block"};
static char* errors_103[] = {NULL};
static primitive_desc primitive_103 = {
	"primitiveCompiledBlockAllocate7", 
	fntype(&allocateBlock7), 
	6359040, 
	signature_103, 
	errors_103
};

static char* signature_104[] = { "Block"};
static char* errors_104[] = {NULL};
static primitive_desc primitive_104 = {
	"primitiveCompiledBlockAllocate8", 
	fntype(&allocateBlock8), 
	6359040, 
	signature_104, 
	errors_104
};

static char* signature_105[] = { "Block"};
static char* errors_105[] = {NULL};
static primitive_desc primitive_105 = {
	"primitiveCompiledBlockAllocate9", 
	fntype(&allocateBlock9), 
	6359040, 
	signature_105, 
	errors_105
};

static char* signature_106[] = { "Block", "SmallInteger"};
static char* errors_106[] = {NULL};
static primitive_desc primitive_106 = {
	"primitiveCompiledBlockAllocate:", 
	fntype(&allocateBlock), 
	2164737, 
	signature_106, 
	errors_106
};

static char* signature_107[] = { "Object"};
static char* errors_107[] = {NULL};
static primitive_desc primitive_107 = {
	"primitiveCompiledContextAllocate0", 
	fntype(&allocateContext0), 
	6359040, 
	signature_107, 
	errors_107
};

static char* signature_108[] = { "Object"};
static char* errors_108[] = {NULL};
static primitive_desc primitive_108 = {
	"primitiveCompiledContextAllocate1", 
	fntype(&allocateContext1), 
	6359040, 
	signature_108, 
	errors_108
};

static char* signature_109[] = { "Object"};
static char* errors_109[] = {NULL};
static primitive_desc primitive_109 = {
	"primitiveCompiledContextAllocate2", 
	fntype(&allocateContext2), 
	6359040, 
	signature_109, 
	errors_109
};

static char* signature_110[] = { "Object", "SmallInteger"};
static char* errors_110[] = {NULL};
static primitive_desc primitive_110 = {
	"primitiveCompiledContextAllocate:", 
	fntype(&allocateContext), 
	6359041, 
	signature_110, 
	errors_110
};

static char* signature_111[] = { "Method", "Object", "SmallInteger", "SmallInteger", "Array", "ByteArray", "Array"};
static char* errors_111[] = {NULL};
static primitive_desc primitive_111 = {
	"primitiveConstructMethod:flags:nofArgs:debugInfo:bytes:oops:ifFail:", 
	fntype(&methodOopPrimitives::constructMethod), 
	327686, 
	signature_111, 
	errors_111
};

static char* signature_112[] = { "Object", "Object"};
static char* errors_112[] = { "NotOops", NULL};
static primitive_desc primitive_112 = {
	"primitiveCopyTenuredIfFail:", 
	fntype(&oopPrimitives::copy_tenured), 
	1376257, 
	signature_112, 
	errors_112
};

static char* signature_113[] = { "GlobalAssociation", "Mixin", "Symbol", "Boolean", "Behavior", "Symbol"};
static char* errors_113[] = { "WrongFormat", NULL};
static primitive_desc primitive_113 = {
	"primitiveCreateInvocationOf:named:isPrimaryInvocation:superclass:format:ifFail:", 
	fntype(&systemPrimitives::createNamedInvocation), 
	327685, 
	signature_113, 
	errors_113
};

static char* signature_114[] = { "GlobalAssociation", "Mixin", "Behavior", "Symbol"};
static char* errors_114[] = { "WrongFormat", NULL};
static primitive_desc primitive_114 = {
	"primitiveCreateInvocationOf:superclass:format:ifFail:", 
	fntype(&systemPrimitives::createInvocation), 
	327683, 
	signature_114, 
	errors_114
};

static char* signature_115[] = { "SmallInteger"};
static char* errors_115[] = {NULL};
static primitive_desc primitive_115 = {
	"primitiveCurrentThreadId", 
	fntype(&systemPrimitives::current_thread_id), 
	65536, 
	signature_115, 
	errors_115
};

static char* signature_116[] = { "Proxy", "Symbol", "Proxy"};
static char* errors_116[] = {NULL};
static primitive_desc primitive_116 = {
	"primitiveDLLLoad:result:ifFail:", 
	fntype(&systemPrimitives::dll_load), 
	327682, 
	signature_116, 
	errors_116
};

static char* signature_117[] = { "Proxy", "Symbol", "Proxy", "Proxy"};
static char* errors_117[] = {NULL};
static primitive_desc primitive_117 = {
	"primitiveDLLLookup:in:result:ifFail:", 
	fntype(&systemPrimitives::dll_lookup), 
	327683, 
	signature_117, 
	errors_117
};

static char* signature_118[] = { "Object", "Object", "Symbol"};
static char* errors_118[] = {NULL};
static primitive_desc primitive_118 = {
	"primitiveDLLSetupLookup:selector:ifFail:", 
	fntype(&systemPrimitives::dll_setup), 
	327682, 
	signature_118, 
	errors_118
};

static char* signature_119[] = { "Object", "Proxy"};
static char* errors_119[] = {NULL};
static primitive_desc primitive_119 = {
	"primitiveDLLUnload:ifFail:", 
	fntype(&systemPrimitives::dll_unload), 
	327681, 
	signature_119, 
	errors_119
};

static char* signature_120[] = { "Object"};
static char* errors_120[] = {NULL};
static primitive_desc primitive_120 = {
	"primitiveDecodeAllMethods", 
	fntype(&debugPrimitives::decodeAllMethods), 
	65536, 
	signature_120, 
	errors_120
};

static char* signature_121[] = { "Object", "Object", "Symbol"};
static char* errors_121[] = { "NotFound", NULL};
static primitive_desc primitive_121 = {
	"primitiveDecodeMethod:ifFail:", 
	fntype(&debugPrimitives::decodeMethod), 
	1376258, 
	signature_121, 
	errors_121
};

static char* signature_122[] = { "Proxy", "Proxy"};
static char* errors_122[] = {NULL};
static primitive_desc primitive_122 = {
	"primitiveDefWindowProc:ifFail:", 
	fntype(&systemPrimitives::defWindowProc), 
	327681, 
	signature_122, 
	errors_122
};

static char* signature_123[] = { "Object"};
static char* errors_123[] = {NULL};
static primitive_desc primitive_123 = {
	"primitiveDeoptimizeStacks", 
	fntype(&debugPrimitives::deoptimizeStacks), 
	196608, 
	signature_123, 
	errors_123
};

static char* signature_124[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_124[] = { "Overflow", "DivisionByZero", NULL};
static primitive_desc primitive_124 = {
	"primitiveDiv:ifFail:", 
	fntype(&smiOopPrimitives_div), 
	6029826, 
	signature_124, 
	errors_124
};

static char* signature_125[] = { "Float"};
static char* errors_125[] = {NULL};
static primitive_desc primitive_125 = {
	"primitiveElapsedTime", 
	fntype(&systemPrimitives::elapsedTime), 
	65536, 
	signature_125, 
	errors_125
};

static char* signature_126[] = { "Boolean", "Object", "Object"};
static char* errors_126[] = {NULL};
static primitive_desc primitive_126 = {
	"primitiveEqual:", 
	fntype(&oopPrimitives::equal), 
	1572866, 
	signature_126, 
	errors_126
};

static char* signature_127[] = { "Object", "SmallInteger"};
static char* errors_127[] = {NULL};
static primitive_desc primitive_127 = {
	"primitiveExpandMemory:", 
	fntype(&systemPrimitives::expandMemory), 
	65537, 
	signature_127, 
	errors_127
};

static char* signature_128[] = { "SmallInteger"};
static char* errors_128[] = {NULL};
static primitive_desc primitive_128 = {
	"primitiveExpansions", 
	fntype(&systemPrimitives::expansions), 
	65536, 
	signature_128, 
	errors_128
};

static char* signature_129[] = { "Process|nil"};
static char* errors_129[] = {NULL};
static primitive_desc primitive_129 = {
	"primitiveFlatProfilerDisengage", 
	fntype(&systemPrimitives::flat_profiler_disengage), 
	65536, 
	signature_129, 
	errors_129
};

static char* signature_130[] = { "Process", "Process"};
static char* errors_130[] = {NULL};
static primitive_desc primitive_130 = {
	"primitiveFlatProfilerEngage:ifFail:", 
	fntype(&systemPrimitives::flat_profiler_engage), 
	327681, 
	signature_130, 
	errors_130
};

static char* signature_131[] = { "Object"};
static char* errors_131[] = {NULL};
static primitive_desc primitive_131 = {
	"primitiveFlatProfilerPrint", 
	fntype(&systemPrimitives::flat_profiler_print), 
	65536, 
	signature_131, 
	errors_131
};

static char* signature_132[] = { "Process|nil"};
static char* errors_132[] = {NULL};
static primitive_desc primitive_132 = {
	"primitiveFlatProfilerProcess", 
	fntype(&systemPrimitives::flat_profiler_process), 
	65536, 
	signature_132, 
	errors_132
};

static char* signature_133[] = { "Object"};
static char* errors_133[] = {NULL};
static primitive_desc primitive_133 = {
	"primitiveFlatProfilerReset", 
	fntype(&systemPrimitives::flat_profiler_reset), 
	65536, 
	signature_133, 
	errors_133
};

static char* signature_134[] = { "Float", "Float"};
static char* errors_134[] = {NULL};
static primitive_desc primitive_134 = {
	"primitiveFloatAbs", 
	fntype(&doubleOopPrimitives::abs), 
	1573889, 
	signature_134, 
	errors_134
};

static char* signature_135[] = { "Float", "Float", "Float"};
static char* errors_135[] = {NULL};
static primitive_desc primitive_135 = {
	"primitiveFloatAdd:ifFail:", 
	fntype(&double_add), 
	6030338, 
	signature_135, 
	errors_135
};

static char* signature_136[] = { "Float", "Float"};
static char* errors_136[] = {NULL};
static primitive_desc primitive_136 = {
	"primitiveFloatArcCosineIfFail:", 
	fntype(&doubleOopPrimitives::arcCosine), 
	1836033, 
	signature_136, 
	errors_136
};

static char* signature_137[] = { "Float", "Float"};
static char* errors_137[] = {NULL};
static primitive_desc primitive_137 = {
	"primitiveFloatArcSineIfFail:", 
	fntype(&doubleOopPrimitives::arcSine), 
	1836033, 
	signature_137, 
	errors_137
};

static char* signature_138[] = { "Float", "Float"};
static char* errors_138[] = {NULL};
static primitive_desc primitive_138 = {
	"primitiveFloatArcTangentIfFail:", 
	fntype(&doubleOopPrimitives::arcTangent), 
	1836033, 
	signature_138, 
	errors_138
};

static char* signature_139[] = { "SmallInteger", "Float"};
static char* errors_139[] = { "SmallIntegerConversionFailed", NULL};
static primitive_desc primitive_139 = {
	"primitiveFloatAsSmallIntegerIfFail:", 
	fntype(&doubleOopPrimitives::asSmallInteger), 
	1836033, 
	signature_139, 
	errors_139
};

static char* signature_140[] = { "Float", "Float"};
static char* errors_140[] = {NULL};
static primitive_desc primitive_140 = {
	"primitiveFloatCeiling", 
	fntype(&doubleOopPrimitives::ceiling), 
	1573889, 
	signature_140, 
	errors_140
};

static char* signature_141[] = { "Float", "Float", "Float"};
static char* errors_141[] = {NULL};
static primitive_desc primitive_141 = {
	"primitiveFloatCopySign:", 
	fntype(&doubleOopPrimitives::copySign), 
	1573890, 
	signature_141, 
	errors_141
};

static char* signature_142[] = { "Float", "Float"};
static char* errors_142[] = {NULL};
static primitive_desc primitive_142 = {
	"primitiveFloatCosine", 
	fntype(&doubleOopPrimitives::cosine), 
	1573889, 
	signature_142, 
	errors_142
};

static char* signature_143[] = { "Float", "Float", "Float"};
static char* errors_143[] = {NULL};
static primitive_desc primitive_143 = {
	"primitiveFloatDivide:ifFail:", 
	fntype(&double_divide), 
	6030338, 
	signature_143, 
	errors_143
};

static char* signature_144[] = { "Boolean", "Float", "Float"};
static char* errors_144[] = {NULL};
static primitive_desc primitive_144 = {
	"primitiveFloatEqual:ifFail:", 
	fntype(&doubleOopPrimitives::equal), 
	6030082, 
	signature_144, 
	errors_144
};

static char* signature_145[] = { "Float", "Float"};
static char* errors_145[] = {NULL};
static primitive_desc primitive_145 = {
	"primitiveFloatExp", 
	fntype(&doubleOopPrimitives::exp), 
	1573889, 
	signature_145, 
	errors_145
};

static char* signature_146[] = { "SmallInteger", "Float"};
static char* errors_146[] = {NULL};
static primitive_desc primitive_146 = {
	"primitiveFloatExponent", 
	fntype(&doubleOopPrimitives::exponent), 
	1573889, 
	signature_146, 
	errors_146
};

static char* signature_147[] = { "Float", "Float"};
static char* errors_147[] = {NULL};
static primitive_desc primitive_147 = {
	"primitiveFloatFloor", 
	fntype(&doubleOopPrimitives::floor), 
	1573889, 
	signature_147, 
	errors_147
};

static char* signature_148[] = { "Boolean", "Float", "Float"};
static char* errors_148[] = {NULL};
static primitive_desc primitive_148 = {
	"primitiveFloatGreaterThan:ifFail:", 
	fntype(&doubleOopPrimitives::greaterThan), 
	6030082, 
	signature_148, 
	errors_148
};

static char* signature_149[] = { "Boolean", "Float", "Float"};
static char* errors_149[] = {NULL};
static primitive_desc primitive_149 = {
	"primitiveFloatGreaterThanOrEqual:ifFail:", 
	fntype(&doubleOopPrimitives::greaterThanOrEqual), 
	6030082, 
	signature_149, 
	errors_149
};

static char* signature_150[] = { "Float", "Float"};
static char* errors_150[] = {NULL};
static primitive_desc primitive_150 = {
	"primitiveFloatHyperbolicCosineIfFail:", 
	fntype(&doubleOopPrimitives::hyperbolicCosine), 
	1836033, 
	signature_150, 
	errors_150
};

static char* signature_151[] = { "Float", "Float"};
static char* errors_151[] = {NULL};
static primitive_desc primitive_151 = {
	"primitiveFloatHyperbolicSineIfFail:", 
	fntype(&doubleOopPrimitives::hyperbolicSine), 
	1836033, 
	signature_151, 
	errors_151
};

static char* signature_152[] = { "Float", "Float"};
static char* errors_152[] = {NULL};
static primitive_desc primitive_152 = {
	"primitiveFloatHyperbolicTangentIfFail:", 
	fntype(&doubleOopPrimitives::hyperbolicTangent), 
	1836033, 
	signature_152, 
	errors_152
};

static char* signature_153[] = { "Boolean", "Float"};
static char* errors_153[] = {NULL};
static primitive_desc primitive_153 = {
	"primitiveFloatIsFinite", 
	fntype(&doubleOopPrimitives::isFinite), 
	1573889, 
	signature_153, 
	errors_153
};

static char* signature_154[] = { "Boolean", "Float"};
static char* errors_154[] = {NULL};
static primitive_desc primitive_154 = {
	"primitiveFloatIsInfinity", 
	fntype(&doubleOopPrimitives::isInfinity), 
	1573889, 
	signature_154, 
	errors_154
};

static char* signature_155[] = { "Boolean", "Float"};
static char* errors_155[] = {NULL};
static primitive_desc primitive_155 = {
	"primitiveFloatIsNan", 
	fntype(&doubleOopPrimitives::isNan), 
	1573889, 
	signature_155, 
	errors_155
};

static char* signature_156[] = { "Boolean", "Float"};
static char* errors_156[] = {NULL};
static primitive_desc primitive_156 = {
	"primitiveFloatIsNormal", 
	fntype(&doubleOopPrimitives::isNormal), 
	1573889, 
	signature_156, 
	errors_156
};

static char* signature_157[] = { "Boolean", "Float", "Float"};
static char* errors_157[] = {NULL};
static primitive_desc primitive_157 = {
	"primitiveFloatLessThan:ifFail:", 
	fntype(&doubleOopPrimitives::lessThan), 
	6030082, 
	signature_157, 
	errors_157
};

static char* signature_158[] = { "Boolean", "Float", "Float"};
static char* errors_158[] = {NULL};
static primitive_desc primitive_158 = {
	"primitiveFloatLessThanOrEqual:ifFail:", 
	fntype(&doubleOopPrimitives::lessThanOrEqual), 
	6030082, 
	signature_158, 
	errors_158
};

static char* signature_159[] = { "Float", "Float"};
static char* errors_159[] = { "ReceiverNotStrictlyPositive", NULL};
static primitive_desc primitive_159 = {
	"primitiveFloatLnIfFail:", 
	fntype(&doubleOopPrimitives::ln), 
	1836033, 
	signature_159, 
	errors_159
};

static char* signature_160[] = { "Float", "Float"};
static char* errors_160[] = { "ReceiverNotStrictlyPositive", NULL};
static primitive_desc primitive_160 = {
	"primitiveFloatLog10IfFail:", 
	fntype(&doubleOopPrimitives::log10), 
	1836033, 
	signature_160, 
	errors_160
};

static char* signature_161[] = { "Float", "Float"};
static char* errors_161[] = {NULL};
static primitive_desc primitive_161 = {
	"primitiveFloatMantissa", 
	fntype(&doubleOopPrimitives::mantissa), 
	1573889, 
	signature_161, 
	errors_161
};

static char* signature_162[] = { "Float"};
static char* errors_162[] = {NULL};
static primitive_desc primitive_162 = {
	"primitiveFloatMaxValue", 
	fntype(&doubleOopPrimitives::max_value), 
	524288, 
	signature_162, 
	errors_162
};

static char* signature_163[] = { "Float"};
static char* errors_163[] = {NULL};
static primitive_desc primitive_163 = {
	"primitiveFloatMinPositiveValue", 
	fntype(&doubleOopPrimitives::min_positive_value), 
	524288, 
	signature_163, 
	errors_163
};

static char* signature_164[] = { "Float", "Float", "Float"};
static char* errors_164[] = {NULL};
static primitive_desc primitive_164 = {
	"primitiveFloatMod:ifFail:", 
	fntype(&doubleOopPrimitives::mod), 
	1836034, 
	signature_164, 
	errors_164
};

static char* signature_165[] = { "Float", "Float", "Float"};
static char* errors_165[] = {NULL};
static primitive_desc primitive_165 = {
	"primitiveFloatMultiply:ifFail:", 
	fntype(&double_multiply), 
	6030338, 
	signature_165, 
	errors_165
};

static char* signature_166[] = { "Float", "Float", "Float"};
static char* errors_166[] = {NULL};
static primitive_desc primitive_166 = {
	"primitiveFloatNextAfter:", 
	fntype(&doubleOopPrimitives::nextAfter), 
	1573890, 
	signature_166, 
	errors_166
};

static char* signature_167[] = { "Boolean", "Float", "Float"};
static char* errors_167[] = {NULL};
static primitive_desc primitive_167 = {
	"primitiveFloatNotEqual:ifFail:", 
	fntype(&doubleOopPrimitives::notEqual), 
	6030082, 
	signature_167, 
	errors_167
};

static char* signature_168[] = { "Self", "Float", "IndexedByteInstanceVariables"};
static char* errors_168[] = {NULL};
static primitive_desc primitive_168 = {
	"primitiveFloatPrintFormat:ifFail:", 
	fntype(&doubleOopPrimitives::printFormat), 
	1310722, 
	signature_168, 
	errors_168
};

static char* signature_169[] = { "IndexedByteInstanceVariables", "Float"};
static char* errors_169[] = {NULL};
static primitive_desc primitive_169 = {
	"primitiveFloatPrintString", 
	fntype(&doubleOopPrimitives::printString), 
	1048577, 
	signature_169, 
	errors_169
};

static char* signature_170[] = { "SmallInteger", "Float"};
static char* errors_170[] = { "SmallIntegerConversionFailed", NULL};
static primitive_desc primitive_170 = {
	"primitiveFloatRoundedAsSmallIntegerIfFail:", 
	fntype(&doubleOopPrimitives::roundedAsSmallInteger), 
	1836033, 
	signature_170, 
	errors_170
};

static char* signature_171[] = { "Float", "Float"};
static char* errors_171[] = {NULL};
static primitive_desc primitive_171 = {
	"primitiveFloatSign", 
	fntype(&doubleOopPrimitives::sign), 
	1573889, 
	signature_171, 
	errors_171
};

static char* signature_172[] = { "Float", "Float"};
static char* errors_172[] = {NULL};
static primitive_desc primitive_172 = {
	"primitiveFloatSine", 
	fntype(&doubleOopPrimitives::sine), 
	1573889, 
	signature_172, 
	errors_172
};

static char* signature_173[] = { "SmallInteger", "Float"};
static char* errors_173[] = { "ConversionFailed", NULL};
static primitive_desc primitive_173 = {
	"primitiveFloatSmallIntegerFloorIfFail:", 
	fntype(&doubleOopPrimitives::smi_floor), 
	6030337, 
	signature_173, 
	errors_173
};

static char* signature_174[] = { "Float", "Float"};
static char* errors_174[] = { "ReceiverNegative", NULL};
static primitive_desc primitive_174 = {
	"primitiveFloatSqrtIfFail:", 
	fntype(&doubleOopPrimitives::sqrt), 
	1836033, 
	signature_174, 
	errors_174
};

static char* signature_175[] = { "Float", "Float"};
static char* errors_175[] = {NULL};
static primitive_desc primitive_175 = {
	"primitiveFloatSquared", 
	fntype(&doubleOopPrimitives::squared), 
	1573889, 
	signature_175, 
	errors_175
};

static char* signature_176[] = { "ByteArray", "Float"};
static char* errors_176[] = {NULL};
static primitive_desc primitive_176 = {
	"primitiveFloatStoreString", 
	fntype(&doubleOopPrimitives::store_string), 
	1048577, 
	signature_176, 
	errors_176
};

static char* signature_177[] = { "Float", "Float", "Float"};
static char* errors_177[] = {NULL};
static primitive_desc primitive_177 = {
	"primitiveFloatSubtract:ifFail:", 
	fntype(&double_subtract), 
	6030338, 
	signature_177, 
	errors_177
};

static char* signature_178[] = { "Float", "Float"};
static char* errors_178[] = {NULL};
static primitive_desc primitive_178 = {
	"primitiveFloatTangentIfFail:", 
	fntype(&doubleOopPrimitives::tangent), 
	1836033, 
	signature_178, 
	errors_178
};

static char* signature_179[] = { "Float", "Float", "SmallInteger"};
static char* errors_179[] = { "RangeError", NULL};
static primitive_desc primitive_179 = {
	"primitiveFloatTimesTwoPower:ifFail:", 
	fntype(&doubleOopPrimitives::timesTwoPower), 
	1836034, 
	signature_179, 
	errors_179
};

static char* signature_180[] = { "Float", "Float"};
static char* errors_180[] = {NULL};
static primitive_desc primitive_180 = {
	"primitiveFloatTruncated", 
	fntype(&doubleOopPrimitives::truncated), 
	1573889, 
	signature_180, 
	errors_180
};

static char* signature_181[] = { "Object"};
static char* errors_181[] = {NULL};
static primitive_desc primitive_181 = {
	"primitiveFlushCodeCache", 
	fntype(&systemPrimitives::flush_code_cache), 
	65536, 
	signature_181, 
	errors_181
};

static char* signature_182[] = { "Object"};
static char* errors_182[] = {NULL};
static primitive_desc primitive_182 = {
	"primitiveFlushDeadCode", 
	fntype(&systemPrimitives::flush_dead_code), 
	65536, 
	signature_182, 
	errors_182
};

static char* signature_183[] = { "SmallInteger"};
static char* errors_183[] = {NULL};
static primitive_desc primitive_183 = {
	"primitiveFreeSpace", 
	fntype(&systemPrimitives::freeSpace), 
	65536, 
	signature_183, 
	errors_183
};

static char* signature_184[] = { "Self", "Object"};
static char* errors_184[] = {NULL};
static primitive_desc primitive_184 = {
	"primitiveGarbageCollect", 
	fntype(&systemPrimitives::garbageGollect), 
	1114113, 
	signature_184, 
	errors_184
};

static char* signature_185[] = { "Object", "Object", "Symbol"};
static char* errors_185[] = { "NotFound", NULL};
static primitive_desc primitive_185 = {
	"primitiveGenerateIR:ifFail:", 
	fntype(&debugPrimitives::generateIR), 
	1376258, 
	signature_185, 
	errors_185
};

static char* signature_186[] = { "Integer"};
static char* errors_186[] = {NULL};
static primitive_desc primitive_186 = {
	"primitiveGetLastError", 
	fntype(&systemPrimitives::getLastError), 
	65536, 
	signature_186, 
	errors_186
};

static char* signature_187[] = { "Boolean", "GlobalAssociation"};
static char* errors_187[] = {NULL};
static primitive_desc primitive_187 = {
	"primitiveGlobalAssociationIsConstant", 
	fntype(&systemPrimitives::globalAssociationIsConstant), 
	1114113, 
	signature_187, 
	errors_187
};

static char* signature_188[] = { "Symbol", "GlobalAssociation"};
static char* errors_188[] = {NULL};
static primitive_desc primitive_188 = {
	"primitiveGlobalAssociationKey", 
	fntype(&systemPrimitives::globalAssociationKey), 
	1114113, 
	signature_188, 
	errors_188
};

static char* signature_189[] = { "Boolean", "GlobalAssociation", "Boolean"};
static char* errors_189[] = {NULL};
static primitive_desc primitive_189 = {
	"primitiveGlobalAssociationSetConstant:", 
	fntype(&systemPrimitives::globalAssociationSetConstant), 
	1114114, 
	signature_189, 
	errors_189
};

static char* signature_190[] = { "Object", "GlobalAssociation", "Symbol"};
static char* errors_190[] = {NULL};
static primitive_desc primitive_190 = {
	"primitiveGlobalAssociationSetKey:", 
	fntype(&systemPrimitives::globalAssociationSetKey), 
	1114114, 
	signature_190, 
	errors_190
};

static char* signature_191[] = { "Object", "GlobalAssociation", "Object"};
static char* errors_191[] = {NULL};
static primitive_desc primitive_191 = {
	"primitiveGlobalAssociationSetValue:", 
	fntype(&systemPrimitives::globalAssociationSetValue), 
	1114114, 
	signature_191, 
	errors_191
};

static char* signature_192[] = { "Object", "GlobalAssociation"};
static char* errors_192[] = {NULL};
static primitive_desc primitive_192 = {
	"primitiveGlobalAssociationValue", 
	fntype(&systemPrimitives::globalAssociationValue), 
	1114113, 
	signature_192, 
	errors_192
};

static char* signature_193[] = { "Boolean", "SmallInteger", "SmallInteger"};
static char* errors_193[] = {NULL};
static primitive_desc primitive_193 = {
	"primitiveGreaterThan:ifFail:", 
	fntype(&smiOopPrimitives::greaterThan), 
	6029570, 
	signature_193, 
	errors_193
};

static char* signature_194[] = { "Boolean", "SmallInteger", "SmallInteger"};
static char* errors_194[] = {NULL};
static primitive_desc primitive_194 = {
	"primitiveGreaterThanOrEqual:ifFail:", 
	fntype(&smiOopPrimitives::greaterThanOrEqual), 
	6029570, 
	signature_194, 
	errors_194
};

static char* signature_195[] = { "Boolean", "Object"};
static char* errors_195[] = {NULL};
static primitive_desc primitive_195 = {
	"primitiveHadNearDeathExperience:", 
	fntype(&systemPrimitives::hadNearDeathExperience), 
	65537, 
	signature_195, 
	errors_195
};

static char* signature_196[] = { "Object"};
static char* errors_196[] = {NULL};
static primitive_desc primitive_196 = {
	"primitiveHalt", 
	fntype(&systemPrimitives::halt), 
	65536, 
	signature_196, 
	errors_196
};

static char* signature_197[] = { "SmallInteger", "Object"};
static char* errors_197[] = {NULL};
static primitive_desc primitive_197 = {
	"primitiveHash", 
	fntype(&oopPrimitives::hash), 
	1114113, 
	signature_197, 
	errors_197
};

static char* signature_198[] = { "SmallInteger", "Object"};
static char* errors_198[] = {NULL};
static primitive_desc primitive_198 = {
	"primitiveHashOf:", 
	fntype(&oopPrimitives::hash_of), 
	65537, 
	signature_198, 
	errors_198
};

static char* signature_199[] = { "SmallInteger", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_199[] = { "OutOfBounds", NULL};
static primitive_desc primitive_199 = {
	"primitiveIndexedByteAt:ifFail:", 
	fntype(&byteArrayPrimitives::at), 
	1312258, 
	signature_199, 
	errors_199
};

static char* signature_200[] = { "SmallInteger", "IndexedByteInstanceVariables", "SmallInteger", "SmallInteger"};
static char* errors_200[] = { "OutOfBounds", "ValueOutOfBounds", NULL};
static primitive_desc primitive_200 = {
	"primitiveIndexedByteAt:put:ifFail:", 
	fntype(&byteArrayPrimitives::atPut), 
	1312259, 
	signature_200, 
	errors_200
};

static char* signature_201[] = { "Self", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_201[] = {NULL};
static primitive_desc primitive_201 = {
	"primitiveIndexedByteAtAllPut:ifFail:", 
	fntype(&byteArrayPrimitives::at_all_put), 
	1376258, 
	signature_201, 
	errors_201
};

static char* signature_202[] = { "SmallInteger", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_202[] = { "OutOfBounds", NULL};
static primitive_desc primitive_202 = {
	"primitiveIndexedByteCharacterAt:ifFail:", 
	fntype(&byteArrayPrimitives::characterAt), 
	1312258, 
	signature_202, 
	errors_202
};

static char* signature_203[] = { "SmallInteger", "IndexedByteInstanceVariables", "String"};
static char* errors_203[] = {NULL};
static primitive_desc primitive_203 = {
	"primitiveIndexedByteCompare:ifFail:", 
	fntype(&byteArrayPrimitives::compare), 
	1376258, 
	signature_203, 
	errors_203
};

static char* signature_204[] = { "SmallInteger", "IndexedByteInstanceVariables"};
static char* errors_204[] = {NULL};
static primitive_desc primitive_204 = {
	"primitiveIndexedByteHash", 
	fntype(&byteArrayPrimitives::hash), 
	1574401, 
	signature_204, 
	errors_204
};

static char* signature_205[] = { "CompressedSymbol", "IndexedByteInstanceVariables"};
static char* errors_205[] = { "ValueOutOfBounds", NULL};
static primitive_desc primitive_205 = {
	"primitiveIndexedByteInternIfFail:", 
	fntype(&byteArrayPrimitives::intern), 
	1376257, 
	signature_205, 
	errors_205
};

static char* signature_206[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_206[] = { "ArgumentIsInvalid", NULL};
static primitive_desc primitive_206 = {
	"primitiveIndexedByteLargeIntegerAdd:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerAdd), 
	1312258, 
	signature_206, 
	errors_206
};

static char* signature_207[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_207[] = { "ArgumentIsInvalid", "DivisionByZero", NULL};
static primitive_desc primitive_207 = {
	"primitiveIndexedByteLargeIntegerAnd:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerAnd), 
	1312258, 
	signature_207, 
	errors_207
};

static char* signature_208[] = { "Float", "IndexedByteInstanceVariables"};
static char* errors_208[] = {NULL};
static primitive_desc primitive_208 = {
	"primitiveIndexedByteLargeIntegerAsFloatIfFail:", 
	fntype(&byteArrayPrimitives::largeIntegerToFloat), 
	1312257, 
	signature_208, 
	errors_208
};

static char* signature_209[] = { "SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_209[] = {NULL};
static primitive_desc primitive_209 = {
	"primitiveIndexedByteLargeIntegerCompare:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerCompare), 
	1312258, 
	signature_209, 
	errors_209
};

static char* signature_210[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_210[] = { "ArgumentIsInvalid", "DivisionByZero", NULL};
static primitive_desc primitive_210 = {
	"primitiveIndexedByteLargeIntegerDiv:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerDiv), 
	1312258, 
	signature_210, 
	errors_210
};

static char* signature_211[] = { "IndexedByteInstanceVariables", "IndexedByteInstanceVariables class", "Float"};
static char* errors_211[] = {NULL};
static primitive_desc primitive_211 = {
	"primitiveIndexedByteLargeIntegerFromFloat:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerFromDouble), 
	1310722, 
	signature_211, 
	errors_211
};

static char* signature_212[] = { "IndexedByteInstanceVariables", "IndexedByteInstanceVariables class", "SmallInteger"};
static char* errors_212[] = {NULL};
static primitive_desc primitive_212 = {
	"primitiveIndexedByteLargeIntegerFromSmallInteger:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerFromSmallInteger), 
	1310722, 
	signature_212, 
	errors_212
};

static char* signature_213[] = { "IndexedByteInstanceVariables", "IndexedByteInstanceVariables class", "String", "Integer"};
static char* errors_213[] = { "ConversionFailed", NULL};
static primitive_desc primitive_213 = {
	"primitiveIndexedByteLargeIntegerFromString:base:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerFromString), 
	1312259, 
	signature_213, 
	errors_213
};

static char* signature_214[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_214[] = { "ArgumentIsInvalid", "DivisionByZero", NULL};
static primitive_desc primitive_214 = {
	"primitiveIndexedByteLargeIntegerMod:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerMod), 
	1312258, 
	signature_214, 
	errors_214
};

static char* signature_215[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_215[] = { "ArgumentIsInvalid", NULL};
static primitive_desc primitive_215 = {
	"primitiveIndexedByteLargeIntegerMultiply:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerMultiply), 
	1312258, 
	signature_215, 
	errors_215
};

static char* signature_216[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_216[] = { "ArgumentIsInvalid", "DivisionByZero", NULL};
static primitive_desc primitive_216 = {
	"primitiveIndexedByteLargeIntegerOr:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerOr), 
	1312258, 
	signature_216, 
	errors_216
};

static char* signature_217[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_217[] = { "ArgumentIsInvalid", "DivisionByZero", NULL};
static primitive_desc primitive_217 = {
	"primitiveIndexedByteLargeIntegerQuo:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerQuo), 
	1312258, 
	signature_217, 
	errors_217
};

static char* signature_218[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_218[] = { "ArgumentIsInvalid", "DivisionByZero", NULL};
static primitive_desc primitive_218 = {
	"primitiveIndexedByteLargeIntegerRem:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerRem), 
	1312258, 
	signature_218, 
	errors_218
};

static char* signature_219[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "SmallInt"};
static char* errors_219[] = { "ArgumentIsInvalid", "DivisionByZero", NULL};
static primitive_desc primitive_219 = {
	"primitiveIndexedByteLargeIntegerShift:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerShift), 
	1312258, 
	signature_219, 
	errors_219
};

static char* signature_220[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_220[] = { "ArgumentIsInvalid", NULL};
static primitive_desc primitive_220 = {
	"primitiveIndexedByteLargeIntegerSubtract:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerSubtract), 
	1312258, 
	signature_220, 
	errors_220
};

static char* signature_221[] = { "String", "IndexedByteInstanceVariables", "SmallInteger"};
static char* errors_221[] = {NULL};
static primitive_desc primitive_221 = {
	"primitiveIndexedByteLargeIntegerToStringBase:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerToString), 
	1312258, 
	signature_221, 
	errors_221
};

static char* signature_222[] = { "IndexedByteInstanceVariables|SmallInteger", "IndexedByteInstanceVariables", "IndexedByteInstanceVariables"};
static char* errors_222[] = { "ArgumentIsInvalid", "DivisionByZero", NULL};
static primitive_desc primitive_222 = {
	"primitiveIndexedByteLargeIntegerXor:ifFail:", 
	fntype(&byteArrayPrimitives::largeIntegerXor), 
	1312258, 
	signature_222, 
	errors_222
};

static char* signature_223[] = { "Object", "IndexedByteInstanceVariables class", "SmallInteger"};
static char* errors_223[] = { "NegativeSize", NULL};
static primitive_desc primitive_223 = {
	"primitiveIndexedByteNew:ifFail:", 
	fntype(&byteArrayPrimitives::allocateSize), 
	1376258, 
	signature_223, 
	errors_223
};

static char* signature_224[] = { "Object", "IndexedByteInstanceVariables class", "SmallInteger", "Boolean"};
static char* errors_224[] = { "NegativeSize", NULL};
static primitive_desc primitive_224 = {
	"primitiveIndexedByteNew:size:tenured:ifFail:", 
	fntype(&byteArrayPrimitives::allocateSize2), 
	327683, 
	signature_224, 
	errors_224
};

static char* signature_225[] = { "SmallInteger", "IndexedByteInstanceVariables"};
static char* errors_225[] = {NULL};
static primitive_desc primitive_225 = {
	"primitiveIndexedByteSize", 
	fntype(&byteArrayPrimitives::size), 
	1574401, 
	signature_225, 
	errors_225
};

static char* signature_226[] = { "SmallInteger", "IndexedDoubleByteInstanceVariables", "SmallInteger"};
static char* errors_226[] = { "OutOfBounds", NULL};
static primitive_desc primitive_226 = {
	"primitiveIndexedDoubleByteAt:ifFail:", 
	fntype(&doubleByteArrayPrimitives::at), 
	1312514, 
	signature_226, 
	errors_226
};

static char* signature_227[] = { "SmallInteger", "IndexedDoubleByteInstanceVariables", "SmallInteger", "SmallInteger"};
static char* errors_227[] = { "OutOfBounds", "ValueOutOfBounds", NULL};
static primitive_desc primitive_227 = {
	"primitiveIndexedDoubleByteAt:put:ifFail:", 
	fntype(&doubleByteArrayPrimitives::atPut), 
	1312515, 
	signature_227, 
	errors_227
};

static char* signature_228[] = { "SmallInteger", "IndexedDoubleByteInstanceVariables", "SmallInteger"};
static char* errors_228[] = { "OutOfBounds", NULL};
static primitive_desc primitive_228 = {
	"primitiveIndexedDoubleByteCharacterAt:ifFail:", 
	fntype(&doubleByteArrayPrimitives::characterAt), 
	1312514, 
	signature_228, 
	errors_228
};

static char* signature_229[] = { "SmallInteger", "IndexedDoubleByteInstanceVariables", "String"};
static char* errors_229[] = {NULL};
static primitive_desc primitive_229 = {
	"primitiveIndexedDoubleByteCompare:ifFail:", 
	fntype(&doubleByteArrayPrimitives::compare), 
	1310722, 
	signature_229, 
	errors_229
};

static char* signature_230[] = { "SmallInteger", "IndexedDoubleByteInstanceVariables"};
static char* errors_230[] = {NULL};
static primitive_desc primitive_230 = {
	"primitiveIndexedDoubleByteHash", 
	fntype(&doubleByteArrayPrimitives::hash), 
	1114113, 
	signature_230, 
	errors_230
};

static char* signature_231[] = { "CompressedSymbol", "IndexedDoubleByteInstanceVariables"};
static char* errors_231[] = { "ValueOutOfBounds", NULL};
static primitive_desc primitive_231 = {
	"primitiveIndexedDoubleByteInternIfFail:", 
	fntype(&doubleByteArrayPrimitives::intern), 
	1376257, 
	signature_231, 
	errors_231
};

static char* signature_232[] = { "Object", "IndexedDoubleByteInstanceVariables class", "SmallInteger"};
static char* errors_232[] = { "NegativeSize", NULL};
static primitive_desc primitive_232 = {
	"primitiveIndexedDoubleByteNew:ifFail:", 
	fntype(&doubleByteArrayPrimitives::allocateSize), 
	1376258, 
	signature_232, 
	errors_232
};

static char* signature_233[] = { "Object", "IndexedDoubleByteInstanceVariables class", "SmallInteger", "Boolean"};
static char* errors_233[] = { "NegativeSize", NULL};
static primitive_desc primitive_233 = {
	"primitiveIndexedDoubleByteNew:size:tenured:ifFail:", 
	fntype(&doubleByteArrayPrimitives::allocateSize2), 
	327683, 
	signature_233, 
	errors_233
};

static char* signature_234[] = { "SmallInteger", "IndexedDoubleByteInstanceVariables"};
static char* errors_234[] = {NULL};
static primitive_desc primitive_234 = {
	"primitiveIndexedDoubleByteSize", 
	fntype(&doubleByteArrayPrimitives::size), 
	1574657, 
	signature_234, 
	errors_234
};

static char* signature_235[] = { "Float", "IndexedFloatValueInstanceVariables", "SmallInteger"};
static char* errors_235[] = { "OutOfBounds", NULL};
static primitive_desc primitive_235 = {
	"primitiveIndexedFloatValueAt:ifFail:", 
	fntype(&doubleValueArrayPrimitives::at), 
	1310722, 
	signature_235, 
	errors_235
};

static char* signature_236[] = { "Float", "IndexedFloatValueInstanceVariables", "SmallInteger", "Float"};
static char* errors_236[] = { "OutOfBounds", "ValueOutOfBounds", NULL};
static primitive_desc primitive_236 = {
	"primitiveIndexedFloatValueAt:put:ifFail:", 
	fntype(&doubleValueArrayPrimitives::atPut), 
	1310723, 
	signature_236, 
	errors_236
};

static char* signature_237[] = { "Object", "IndexedFloatValueInstanceVariables class", "SmallInteger"};
static char* errors_237[] = { "NegativeSize", NULL};
static primitive_desc primitive_237 = {
	"primitiveIndexedFloatValueNew:ifFail:", 
	fntype(&doubleValueArrayPrimitives::allocateSize), 
	1376258, 
	signature_237, 
	errors_237
};

static char* signature_238[] = { "SmallInteger", "IndexedFloatValueInstanceVariables"};
static char* errors_238[] = {NULL};
static primitive_desc primitive_238 = {
	"primitiveIndexedFloatValueSize", 
	fntype(&doubleValueArrayPrimitives::size), 
	1572865, 
	signature_238, 
	errors_238
};

static char* signature_239[] = { "SmallInteger", "IndexedInstanceVariables", "SmallInteger"};
static char* errors_239[] = { "OutOfBounds", NULL};
static primitive_desc primitive_239 = {
	"primitiveIndexedObjectAt:ifFail:", 
	fntype(&objArrayPrimitives::at), 
	1312002, 
	signature_239, 
	errors_239
};

static char* signature_240[] = { "Object", "IndexedInstanceVariables", "SmallInteger", "Object"};
static char* errors_240[] = { "OutOfBounds", NULL};
static primitive_desc primitive_240 = {
	"primitiveIndexedObjectAt:put:ifFail:", 
	fntype(&objArrayPrimitives::atPut), 
	1312003, 
	signature_240, 
	errors_240
};

static char* signature_241[] = { "Self", "IndexedInstanceVariables", "Object"};
static char* errors_241[] = {NULL};
static primitive_desc primitive_241 = {
	"primitiveIndexedObjectAtAllPut:", 
	fntype(&objArrayPrimitives::at_all_put), 
	1049858, 
	signature_241, 
	errors_241
};

static char* signature_242[] = { "Self", "IndexedInstanceVariables", "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_242[] = { "OutOfBounds", "NegativeSize", NULL};
static primitive_desc primitive_242 = {
	"primitiveIndexedObjectCopyFrom:startingAt:size:ifFail:", 
	fntype(&objArrayPrimitives::copy_size), 
	1377540, 
	signature_242, 
	errors_242
};

static char* signature_243[] = { "Object", "IndexedInstanceVariables class", "SmallInteger"};
static char* errors_243[] = { "NegativeSize", NULL};
static primitive_desc primitive_243 = {
	"primitiveIndexedObjectNew:ifFail:", 
	fntype(&objArrayPrimitives::allocateSize), 
	1376258, 
	signature_243, 
	errors_243
};

static char* signature_244[] = { "Object", "IndexedInstanceVariables class", "SmallInteger", "Boolean"};
static char* errors_244[] = { "NegativeSize", NULL};
static primitive_desc primitive_244 = {
	"primitiveIndexedObjectNew:size:tenured:ifFail:", 
	fntype(&objArrayPrimitives::allocateSize2), 
	327683, 
	signature_244, 
	errors_244
};

static char* signature_245[] = { "Self", "IndexedInstanceVariables", "SmallInteger", "SmallInteger", "IndexedInstanceVariables", "SmallInteger"};
static char* errors_245[] = { "OutOfBounds", NULL};
static primitive_desc primitive_245 = {
	"primitiveIndexedObjectReplaceFrom:to:with:startingAt:ifFail:", 
	fntype(&objArrayPrimitives::replace_from_to), 
	1377541, 
	signature_245, 
	errors_245
};

static char* signature_246[] = { "Self", "IndexedInstanceVariables"};
static char* errors_246[] = {NULL};
static primitive_desc primitive_246 = {
	"primitiveIndexedObjectSize", 
	fntype(&objArrayPrimitives::size), 
	1574145, 
	signature_246, 
	errors_246
};

static char* signature_247[] = { "Instance", "Behavior", "SmallInt"};
static char* errors_247[] = {NULL};
static primitive_desc primitive_247 = {
	"primitiveInlineAllocations:count:", 
	fntype(&primitiveInlineAllocations), 
	4259842, 
	signature_247, 
	errors_247
};

static char* signature_248[] = { "Boolean", "Behavior", "Symbol"};
static char* errors_248[] = {NULL};
static primitive_desc primitive_248 = {
	"primitiveInliningDatabaseAddLookupEntryClass:selector:ifFail:", 
	fntype(&systemPrimitives::inlining_database_add_entry), 
	327682, 
	signature_248, 
	errors_248
};

static char* signature_249[] = { "Boolean"};
static char* errors_249[] = {NULL};
static primitive_desc primitive_249 = {
	"primitiveInliningDatabaseCompile", 
	fntype(&systemPrimitives::inlining_database_compile_next), 
	65536, 
	signature_249, 
	errors_249
};

static char* signature_250[] = { "Object", "String"};
static char* errors_250[] = {NULL};
static primitive_desc primitive_250 = {
	"primitiveInliningDatabaseCompile:ifFail:", 
	fntype(&systemPrimitives::inlining_database_compile), 
	327681, 
	signature_250, 
	errors_250
};

static char* signature_251[] = { "IndexedByteInstanceVariables", "String"};
static char* errors_251[] = {NULL};
static primitive_desc primitive_251 = {
	"primitiveInliningDatabaseCompileDemangled:ifFail:", 
	fntype(&systemPrimitives::inlining_database_demangle), 
	327681, 
	signature_251, 
	errors_251
};

static char* signature_252[] = { "Symbol"};
static char* errors_252[] = {NULL};
static primitive_desc primitive_252 = {
	"primitiveInliningDatabaseDirectory", 
	fntype(&systemPrimitives::inlining_database_directory), 
	65536, 
	signature_252, 
	errors_252
};

static char* signature_253[] = { "SmallInteger"};
static char* errors_253[] = {NULL};
static primitive_desc primitive_253 = {
	"primitiveInliningDatabaseFileOutAllIfFail:", 
	fntype(&systemPrimitives::inlining_database_file_out_all), 
	327680, 
	signature_253, 
	errors_253
};

static char* signature_254[] = { "SmallInteger", "Behavior"};
static char* errors_254[] = {NULL};
static primitive_desc primitive_254 = {
	"primitiveInliningDatabaseFileOutClass:ifFail:", 
	fntype(&systemPrimitives::inlining_database_file_out_class), 
	327681, 
	signature_254, 
	errors_254
};

static char* signature_255[] = { "IndexedByteInstanceVariables", "String"};
static char* errors_255[] = {NULL};
static primitive_desc primitive_255 = {
	"primitiveInliningDatabaseMangle:ifFail:", 
	fntype(&systemPrimitives::inlining_database_mangle), 
	327681, 
	signature_255, 
	errors_255
};

static char* signature_256[] = { "Symbol", "Symbol"};
static char* errors_256[] = {NULL};
static primitive_desc primitive_256 = {
	"primitiveInliningDatabaseSetDirectory:ifFail:", 
	fntype(&systemPrimitives::inlining_database_set_directory), 
	327681, 
	signature_256, 
	errors_256
};

static char* signature_257[] = { "Object", "Object", "SmallInteger"};
static char* errors_257[] = { "OutOfBounds", NULL};
static primitive_desc primitive_257 = {
	"primitiveInstVarAt:ifFail:", 
	fntype(&oopPrimitives::instVarAt), 
	1376258, 
	signature_257, 
	errors_257
};

static char* signature_258[] = { "Symbol", "Reciever", "Object", "SmallInteger"};
static char* errors_258[] = { "OutOfBounds", NULL};
static primitive_desc primitive_258 = {
	"primitiveInstVarNameFor:at:ifFail:", 
	fntype(&oopPrimitives::instance_variable_name_at), 
	1376259, 
	signature_258, 
	errors_258
};

static char* signature_259[] = { "Object", "Object", "SmallInteger", "Object"};
static char* errors_259[] = { "OutOfBounds", NULL};
static primitive_desc primitive_259 = {
	"primitiveInstVarOf:at:put:ifFail:", 
	fntype(&oopPrimitives::instVarAtPut), 
	327683, 
	signature_259, 
	errors_259
};

static char* signature_260[] = { "IndexedInstanceVariables", "Class", "SmallInteger"};
static char* errors_260[] = {NULL};
static primitive_desc primitive_260 = {
	"primitiveInstancesOf:limit:ifFail:", 
	fntype(&systemPrimitives::instances_of), 
	327682, 
	signature_260, 
	errors_260
};

static char* signature_261[] = { "SmallInteger"};
static char* errors_261[] = {NULL};
static primitive_desc primitive_261 = {
	"primitiveInterpreterInvocationCounterLimit", 
	fntype(&debugPrimitives::interpreterInvocationCounterLimit), 
	65536, 
	signature_261, 
	errors_261
};

static char* signature_262[] = { "SmallInteger", "IndexedByteInstanceVariables"};
static char* errors_262[] = {NULL};
static primitive_desc primitive_262 = {
	"primitiveLargeIntegerHash", 
	fntype(&byteArrayPrimitives::largeIntegerHash), 
	1574401, 
	signature_262, 
	errors_262
};

static char* signature_263[] = { "Boolean", "SmallInteger", "SmallInteger"};
static char* errors_263[] = {NULL};
static primitive_desc primitive_263 = {
	"primitiveLessThan:ifFail:", 
	fntype(&smiOopPrimitives::lessThan), 
	6029570, 
	signature_263, 
	errors_263
};

static char* signature_264[] = { "Boolean", "SmallInteger", "SmallInteger"};
static char* errors_264[] = {NULL};
static primitive_desc primitive_264 = {
	"primitiveLessThanOrEqual:ifFail:", 
	fntype(&smiOopPrimitives::lessThanOrEqual), 
	6029570, 
	signature_264, 
	errors_264
};

static char* signature_265[] = { "SmallInteger", "Float", "Float", "SmallInteger"};
static char* errors_265[] = {NULL};
static primitive_desc primitive_265 = {
	"primitiveMandelbrotAtRe:im:iterate:ifFail:", 
	fntype(&doubleOopPrimitives::mandelbrot), 
	4980739, 
	signature_265, 
	errors_265
};

static char* signature_266[] = { "Block", "Method", "Object"};
static char* errors_266[] = {NULL};
static primitive_desc primitive_266 = {
	"primitiveMethodAllocateBlock:ifFail:", 
	fntype(&methodOopPrimitives::allocate_block_self), 
	1376258, 
	signature_266, 
	errors_266
};

static char* signature_267[] = { "Block", "Method"};
static char* errors_267[] = {NULL};
static primitive_desc primitive_267 = {
	"primitiveMethodAllocateBlockIfFail:", 
	fntype(&methodOopPrimitives::allocate_block), 
	1376257, 
	signature_267, 
	errors_267
};

static char* signature_268[] = { "Object", "Method"};
static char* errors_268[] = {NULL};
static primitive_desc primitive_268 = {
	"primitiveMethodBody", 
	fntype(&methodOopPrimitives::fileout_body), 
	1114113, 
	signature_268, 
	errors_268
};

static char* signature_269[] = { "Object", "Method"};
static char* errors_269[] = {NULL};
static primitive_desc primitive_269 = {
	"primitiveMethodDebugInfo", 
	fntype(&methodOopPrimitives::debug_info), 
	1114113, 
	signature_269, 
	errors_269
};

static char* signature_270[] = { "Method", "Behavior", "CompressedSymbol"};
static char* errors_270[] = { "NotFound", NULL};
static primitive_desc primitive_270 = {
	"primitiveMethodFor:ifFail:", 
	fntype(&behaviorPrimitives::methodFor), 
	1376258, 
	signature_270, 
	errors_270
};

static char* signature_271[] = { "Symbol", "Method"};
static char* errors_271[] = {NULL};
static primitive_desc primitive_271 = {
	"primitiveMethodInliningInfo", 
	fntype(&methodOopPrimitives::inlining_info), 
	1114113, 
	signature_271, 
	errors_271
};

static char* signature_272[] = { "SmallInteger", "Method"};
static char* errors_272[] = {NULL};
static primitive_desc primitive_272 = {
	"primitiveMethodNumberOfArguments", 
	fntype(&methodOopPrimitives::numberOfArguments), 
	1114113, 
	signature_272, 
	errors_272
};

static char* signature_273[] = { "Symbol", "Method", "Method"};
static char* errors_273[] = {NULL};
static primitive_desc primitive_273 = {
	"primitiveMethodOuter:ifFail:", 
	fntype(&methodOopPrimitives::setOuter), 
	1376258, 
	signature_273, 
	errors_273
};

static char* signature_274[] = { "Method", "Method"};
static char* errors_274[] = { "ReceiverNotBlockMethod", NULL};
static primitive_desc primitive_274 = {
	"primitiveMethodOuterIfFail:", 
	fntype(&methodOopPrimitives::outer), 
	1376257, 
	signature_274, 
	errors_274
};

static char* signature_275[] = { "Method", "Method", "Object"};
static char* errors_275[] = {NULL};
static primitive_desc primitive_275 = {
	"primitiveMethodPrettyPrintKlass:ifFail:", 
	fntype(&methodOopPrimitives::prettyPrint), 
	1376258, 
	signature_275, 
	errors_275
};

static char* signature_276[] = { "ByteIndexedInstanceVariables", "Method", "Object"};
static char* errors_276[] = {NULL};
static primitive_desc primitive_276 = {
	"primitiveMethodPrettyPrintSourceKlass:ifFail:", 
	fntype(&methodOopPrimitives::prettyPrintSource), 
	1376258, 
	signature_276, 
	errors_276
};

static char* signature_277[] = { "Symbol", "Method"};
static char* errors_277[] = {NULL};
static primitive_desc primitive_277 = {
	"primitiveMethodPrintCodes", 
	fntype(&methodOopPrimitives::printCodes), 
	1114113, 
	signature_277, 
	errors_277
};

static char* signature_278[] = { "IndexedInstanceVariables", "Method"};
static char* errors_278[] = {NULL};
static primitive_desc primitive_278 = {
	"primitiveMethodReferencedClassVarNames", 
	fntype(&methodOopPrimitives::referenced_class_variable_names), 
	1114113, 
	signature_278, 
	errors_278
};

static char* signature_279[] = { "IndexedInstanceVariables", "Method"};
static char* errors_279[] = {NULL};
static primitive_desc primitive_279 = {
	"primitiveMethodReferencedGlobalNames", 
	fntype(&methodOopPrimitives::referenced_global_names), 
	1114113, 
	signature_279, 
	errors_279
};

static char* signature_280[] = { "IndexedInstanceVariables", "Method", "Mixin"};
static char* errors_280[] = {NULL};
static primitive_desc primitive_280 = {
	"primitiveMethodReferencedInstVarNamesMixin:ifFail:", 
	fntype(&methodOopPrimitives::referenced_instance_variable_names), 
	1376258, 
	signature_280, 
	errors_280
};

static char* signature_281[] = { "Symbol", "Method"};
static char* errors_281[] = {NULL};
static primitive_desc primitive_281 = {
	"primitiveMethodSelector", 
	fntype(&methodOopPrimitives::selector), 
	1114113, 
	signature_281, 
	errors_281
};

static char* signature_282[] = { "Symbol", "Method", "Symbol"};
static char* errors_282[] = {NULL};
static primitive_desc primitive_282 = {
	"primitiveMethodSelector:ifFail:", 
	fntype(&methodOopPrimitives::setSelector), 
	1376258, 
	signature_282, 
	errors_282
};

static char* signature_283[] = { "IndexedInstanceVariables", "Method"};
static char* errors_283[] = {NULL};
static primitive_desc primitive_283 = {
	"primitiveMethodSenders", 
	fntype(&methodOopPrimitives::senders), 
	1114113, 
	signature_283, 
	errors_283
};

static char* signature_284[] = { "Symbol", "Method", "Symbol"};
static char* errors_284[] = { "ArgumentIsInvalid", NULL};
static primitive_desc primitive_284 = {
	"primitiveMethodSetInliningInfo:ifFail:", 
	fntype(&methodOopPrimitives::set_inlining_info), 
	1376258, 
	signature_284, 
	errors_284
};

static char* signature_285[] = { "Object", "Method"};
static char* errors_285[] = {NULL};
static primitive_desc primitive_285 = {
	"primitiveMethodSizeAndFlags", 
	fntype(&methodOopPrimitives::size_and_flags), 
	1114113, 
	signature_285, 
	errors_285
};

static char* signature_286[] = { "Mixin", "Mixin", "Symbol"};
static char* errors_286[] = { "IsInstalled", "OutOfBounds", NULL};
static primitive_desc primitive_286 = {
	"primitiveMixin:addClassVariable:ifFail:", 
	fntype(&mixinOopPrimitives::add_class_variable), 
	327682, 
	signature_286, 
	errors_286
};

static char* signature_287[] = { "Symbol", "Mixin", "Symbol"};
static char* errors_287[] = { "IsInstalled", "OutOfBounds", NULL};
static primitive_desc primitive_287 = {
	"primitiveMixin:addInstanceVariable:ifFail:", 
	fntype(&mixinOopPrimitives::add_instance_variable), 
	327682, 
	signature_287, 
	errors_287
};

static char* signature_288[] = { "Method", "Mixin", "Method"};
static char* errors_288[] = { "IsInstalled", NULL};
static primitive_desc primitive_288 = {
	"primitiveMixin:addMethod:ifFail:", 
	fntype(&mixinOopPrimitives::add_method), 
	327682, 
	signature_288, 
	errors_288
};

static char* signature_289[] = { "Symbol", "Mixin", "SmallInteger"};
static char* errors_289[] = { "OutOfBounds", NULL};
static primitive_desc primitive_289 = {
	"primitiveMixin:classVariableAt:ifFail:", 
	fntype(&mixinOopPrimitives::class_variable_at), 
	327682, 
	signature_289, 
	errors_289
};

static char* signature_290[] = { "Symbol", "Mixin"};
static char* errors_290[] = {NULL};
static primitive_desc primitive_290 = {
	"primitiveMixin:classVariablesIfFail:", 
	fntype(&mixinOopPrimitives::class_variables), 
	327681, 
	signature_290, 
	errors_290
};

static char* signature_291[] = { "Symbol", "Mixin", "SmallInteger"};
static char* errors_291[] = { "OutOfBounds", NULL};
static primitive_desc primitive_291 = {
	"primitiveMixin:instanceVariableAt:ifFail:", 
	fntype(&mixinOopPrimitives::instance_variable_at), 
	327682, 
	signature_291, 
	errors_291
};

static char* signature_292[] = { "Symbol", "Mixin"};
static char* errors_292[] = {NULL};
static primitive_desc primitive_292 = {
	"primitiveMixin:instanceVariablesIfFail:", 
	fntype(&mixinOopPrimitives::instance_variables), 
	327681, 
	signature_292, 
	errors_292
};

static char* signature_293[] = { "Method", "Mixin", "SmallInteger"};
static char* errors_293[] = { "OutOfBounds", NULL};
static primitive_desc primitive_293 = {
	"primitiveMixin:methodAt:ifFail:", 
	fntype(&mixinOopPrimitives::method_at), 
	327682, 
	signature_293, 
	errors_293
};

static char* signature_294[] = { "Symbol", "Mixin"};
static char* errors_294[] = {NULL};
static primitive_desc primitive_294 = {
	"primitiveMixin:methodsIfFail:", 
	fntype(&mixinOopPrimitives::methods), 
	327681, 
	signature_294, 
	errors_294
};

static char* signature_295[] = { "Symbol", "Mixin", "SmallInteger"};
static char* errors_295[] = { "IsInstalled", "OutOfBounds", NULL};
static primitive_desc primitive_295 = {
	"primitiveMixin:removeClassVariableAt:ifFail:", 
	fntype(&mixinOopPrimitives::remove_class_variable_at), 
	327682, 
	signature_295, 
	errors_295
};

static char* signature_296[] = { "Symbol", "Mixin", "SmallInteger"};
static char* errors_296[] = { "IsInstalled", "OutOfBounds", NULL};
static primitive_desc primitive_296 = {
	"primitiveMixin:removeInstanceVariableAt:ifFail:", 
	fntype(&mixinOopPrimitives::remove_instance_variable_at), 
	327682, 
	signature_296, 
	errors_296
};

static char* signature_297[] = { "Method", "Mixin", "SmallInteger"};
static char* errors_297[] = { "IsInstalled", "OutOfBounds", NULL};
static primitive_desc primitive_297 = {
	"primitiveMixin:removeMethodAt:ifFail:", 
	fntype(&mixinOopPrimitives::remove_method_at), 
	327682, 
	signature_297, 
	errors_297
};

static char* signature_298[] = { "Mixin", "Mixin"};
static char* errors_298[] = {NULL};
static primitive_desc primitive_298 = {
	"primitiveMixinClassMixinOf:ifFail:", 
	fntype(&mixinOopPrimitives::class_mixin), 
	327681, 
	signature_298, 
	errors_298
};

static char* signature_299[] = { "Boolean", "Mixin"};
static char* errors_299[] = {NULL};
static primitive_desc primitive_299 = {
	"primitiveMixinIsInstalled:ifFail:", 
	fntype(&mixinOopPrimitives::is_installed), 
	327681, 
	signature_299, 
	errors_299
};

static char* signature_300[] = { "SmallInteger", "Mixin"};
static char* errors_300[] = {NULL};
static primitive_desc primitive_300 = {
	"primitiveMixinNumberOfClassVariablesOf:ifFail:", 
	fntype(&mixinOopPrimitives::number_of_class_variables), 
	327681, 
	signature_300, 
	errors_300
};

static char* signature_301[] = { "SmallInteger", "Mixin"};
static char* errors_301[] = {NULL};
static primitive_desc primitive_301 = {
	"primitiveMixinNumberOfInstanceVariablesOf:ifFail:", 
	fntype(&mixinOopPrimitives::number_of_instance_variables), 
	327681, 
	signature_301, 
	errors_301
};

static char* signature_302[] = { "SmallInteger", "Mixin"};
static char* errors_302[] = {NULL};
static primitive_desc primitive_302 = {
	"primitiveMixinNumberOfMethodsOf:ifFail:", 
	fntype(&mixinOopPrimitives::number_of_methods), 
	327681, 
	signature_302, 
	errors_302
};

static char* signature_303[] = { "Class", "Mixin"};
static char* errors_303[] = {NULL};
static primitive_desc primitive_303 = {
	"primitiveMixinPrimaryInvocationOf:ifFail:", 
	fntype(&mixinOopPrimitives::primary_invocation), 
	327681, 
	signature_303, 
	errors_303
};

static char* signature_304[] = { "Mixin", "Mixin", "Mixin"};
static char* errors_304[] = { "IsInstalled", NULL};
static primitive_desc primitive_304 = {
	"primitiveMixinSetClassMixinOf:to:ifFail:", 
	fntype(&mixinOopPrimitives::set_class_mixin), 
	327682, 
	signature_304, 
	errors_304
};

static char* signature_305[] = { "Boolean", "Mixin"};
static char* errors_305[] = {NULL};
static primitive_desc primitive_305 = {
	"primitiveMixinSetInstalled:ifFail:", 
	fntype(&mixinOopPrimitives::set_installed), 
	327681, 
	signature_305, 
	errors_305
};

static char* signature_306[] = { "Class", "Mixin", "Class"};
static char* errors_306[] = { "IsInstalled", NULL};
static primitive_desc primitive_306 = {
	"primitiveMixinSetPrimaryInvocationOf:to:ifFail:", 
	fntype(&mixinOopPrimitives::set_primary_invocation), 
	327682, 
	signature_306, 
	errors_306
};

static char* signature_307[] = { "Boolean", "Mixin"};
static char* errors_307[] = {NULL};
static primitive_desc primitive_307 = {
	"primitiveMixinSetUnInstalled:ifFail:", 
	fntype(&mixinOopPrimitives::set_uninstalled), 
	327681, 
	signature_307, 
	errors_307
};

static char* signature_308[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_308[] = { "Overflow", "DivisionByZero", NULL};
static primitive_desc primitive_308 = {
	"primitiveMod:ifFail:", 
	fntype(&smiOopPrimitives_mod), 
	6029826, 
	signature_308, 
	errors_308
};

static char* signature_309[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_309[] = { "Overflow", NULL};
static primitive_desc primitive_309 = {
	"primitiveMultiply:ifFail:", 
	fntype(&smiOopPrimitives_multiply), 
	6029826, 
	signature_309, 
	errors_309
};

static char* signature_310[] = { "Instance", "Behavior", "Boolean"};
static char* errors_310[] = {NULL};
static primitive_desc primitive_310 = {
	"primitiveNew0:ifFail:", 
	fntype(&primitiveNew0), 
	7667714, 
	signature_310, 
	errors_310
};

static char* signature_311[] = { "Instance", "Behavior", "Boolean"};
static char* errors_311[] = {NULL};
static primitive_desc primitive_311 = {
	"primitiveNew1:ifFail:", 
	fntype(&primitiveNew1), 
	7667714, 
	signature_311, 
	errors_311
};

static char* signature_312[] = { "Instance", "Behavior", "Boolean"};
static char* errors_312[] = {NULL};
static primitive_desc primitive_312 = {
	"primitiveNew2:ifFail:", 
	fntype(&primitiveNew2), 
	7667714, 
	signature_312, 
	errors_312
};

static char* signature_313[] = { "Instance", "Behavior"};
static char* errors_313[] = { "ReceiverIsIndexable", NULL};
static primitive_desc primitive_313 = {
	"primitiveNew2IfFail:", 
	fntype(&behaviorPrimitives::allocate2), 
	1376257, 
	signature_313, 
	errors_313
};

static char* signature_314[] = { "Instance", "Behavior", "Boolean"};
static char* errors_314[] = {NULL};
static primitive_desc primitive_314 = {
	"primitiveNew3:ifFail:", 
	fntype(&primitiveNew3), 
	7667714, 
	signature_314, 
	errors_314
};

static char* signature_315[] = { "Instance", "Behavior", "Boolean"};
static char* errors_315[] = {NULL};
static primitive_desc primitive_315 = {
	"primitiveNew4:ifFail:", 
	fntype(&primitiveNew4), 
	7667714, 
	signature_315, 
	errors_315
};

static char* signature_316[] = { "Instance", "Behavior", "Boolean"};
static char* errors_316[] = {NULL};
static primitive_desc primitive_316 = {
	"primitiveNew5:ifFail:", 
	fntype(&primitiveNew5), 
	7667714, 
	signature_316, 
	errors_316
};

static char* signature_317[] = { "Instance", "Behavior", "Boolean"};
static char* errors_317[] = {NULL};
static primitive_desc primitive_317 = {
	"primitiveNew6:ifFail:", 
	fntype(&primitiveNew6), 
	7667714, 
	signature_317, 
	errors_317
};

static char* signature_318[] = { "Instance", "Behavior", "Boolean"};
static char* errors_318[] = {NULL};
static primitive_desc primitive_318 = {
	"primitiveNew7:ifFail:", 
	fntype(&primitiveNew7), 
	7667714, 
	signature_318, 
	errors_318
};

static char* signature_319[] = { "Instance", "Behavior", "Boolean"};
static char* errors_319[] = {NULL};
static primitive_desc primitive_319 = {
	"primitiveNew8:ifFail:", 
	fntype(&primitiveNew8), 
	7667714, 
	signature_319, 
	errors_319
};

static char* signature_320[] = { "Instance", "Behavior", "Boolean"};
static char* errors_320[] = {NULL};
static primitive_desc primitive_320 = {
	"primitiveNew9:ifFail:", 
	fntype(&primitiveNew9), 
	7667714, 
	signature_320, 
	errors_320
};

static char* signature_321[] = { "Instance", "Behavior", "Boolean"};
static char* errors_321[] = { "ReceiverIsIndexable", NULL};
static primitive_desc primitive_321 = {
	"primitiveNew:tenured:ifFail:", 
	fntype(&behaviorPrimitives::allocate3), 
	327682, 
	signature_321, 
	errors_321
};

static char* signature_322[] = { "Instance", "Behavior"};
static char* errors_322[] = { "ReceiverIsIndexable", NULL};
static primitive_desc primitive_322 = {
	"primitiveNewIfFail:", 
	fntype(&behaviorPrimitives::allocate), 
	1376257, 
	signature_322, 
	errors_322
};

static char* signature_323[] = { "Boolean", "Object", "Object"};
static char* errors_323[] = {NULL};
static primitive_desc primitive_323 = {
	"primitiveNotEqual:", 
	fntype(&oopPrimitives::not_equal), 
	1572866, 
	signature_323, 
	errors_323
};

static char* signature_324[] = { "Object"};
static char* errors_324[] = { "EmptyQueue", NULL};
static primitive_desc primitive_324 = {
	"primitiveNotificationQueueGetIfFail:", 
	fntype(&systemPrimitives::notificationQueueGet), 
	327680, 
	signature_324, 
	errors_324
};

static char* signature_325[] = { "Object", "Object"};
static char* errors_325[] = {NULL};
static primitive_desc primitive_325 = {
	"primitiveNotificationQueuePut:", 
	fntype(&systemPrimitives::notificationQueuePut), 
	65537, 
	signature_325, 
	errors_325
};

static char* signature_326[] = { "SmallInteger"};
static char* errors_326[] = {NULL};
static primitive_desc primitive_326 = {
	"primitiveNumberOfLookupCacheMisses", 
	fntype(&debugPrimitives::numberOfLookupCacheMisses), 
	65536, 
	signature_326, 
	errors_326
};

static char* signature_327[] = { "SmallInteger"};
static char* errors_327[] = {NULL};
static primitive_desc primitive_327 = {
	"primitiveNumberOfMethodInvocations", 
	fntype(&debugPrimitives::numberOfMethodInvocations), 
	65536, 
	signature_327, 
	errors_327
};

static char* signature_328[] = { "SmallInteger"};
static char* errors_328[] = {NULL};
static primitive_desc primitive_328 = {
	"primitiveNumberOfNMethodInvocations", 
	fntype(&debugPrimitives::numberOfNMethodInvocations), 
	65536, 
	signature_328, 
	errors_328
};

static char* signature_329[] = { "SmallInteger"};
static char* errors_329[] = {NULL};
static primitive_desc primitive_329 = {
	"primitiveNumberOfPrimaryLookupCacheHits", 
	fntype(&debugPrimitives::numberOfPrimaryLookupCacheHits), 
	65536, 
	signature_329, 
	errors_329
};

static char* signature_330[] = { "SmallInteger"};
static char* errors_330[] = {NULL};
static primitive_desc primitive_330 = {
	"primitiveNumberOfSecondaryLookupCacheHits", 
	fntype(&debugPrimitives::numberOfSecondaryLookupCacheHits), 
	65536, 
	signature_330, 
	errors_330
};

static char* signature_331[] = { "SmallInteger"};
static char* errors_331[] = {NULL};
static primitive_desc primitive_331 = {
	"primitiveNurseryFreeSpace", 
	fntype(&systemPrimitives::nurseryFreeSpace), 
	65536, 
	signature_331, 
	errors_331
};

static char* signature_332[] = { "Float"};
static char* errors_332[] = {NULL};
static primitive_desc primitive_332 = {
	"primitiveObjectMemorySize", 
	fntype(&systemPrimitives::object_memory_size), 
	65536, 
	signature_332, 
	errors_332
};

static char* signature_333[] = { "SmallInteger", "Object"};
static char* errors_333[] = {NULL};
static primitive_desc primitive_333 = {
	"primitiveOopSize", 
	fntype(&oopPrimitives::oop_size), 
	1572865, 
	signature_333, 
	errors_333
};

static char* signature_334[] = { "Object", "Object", "Symbol"};
static char* errors_334[] = { "NotFound", NULL};
static primitive_desc primitive_334 = {
	"primitiveOptimizeMethod:ifFail:", 
	fntype(&debugPrimitives::optimizeMethod), 
	1376258, 
	signature_334, 
	errors_334
};

static char* signature_335[] = { "Object", "Object", "CompressedSymbol", "Array"};
static char* errors_335[] = { "SelectorHasWrongNumberOfArguments", NULL};
static primitive_desc primitive_335 = {
	"primitivePerform:arguments:ifFail:", 
	fntype(&oopPrimitives::performArguments), 
	1507331, 
	signature_335, 
	errors_335
};

static char* signature_336[] = { "Object", "Object", "CompressedSymbol"};
static char* errors_336[] = { "SelectorHasWrongNumberOfArguments", NULL};
static primitive_desc primitive_336 = {
	"primitivePerform:ifFail:", 
	fntype(&oopPrimitives::perform), 
	1376258, 
	signature_336, 
	errors_336
};

static char* signature_337[] = { "Object", "Object", "CompressedSymbol", "Object"};
static char* errors_337[] = { "SelectorHasWrongNumberOfArguments", NULL};
static primitive_desc primitive_337 = {
	"primitivePerform:with:ifFail:", 
	fntype(&oopPrimitives::performWith), 
	1507331, 
	signature_337, 
	errors_337
};

static char* signature_338[] = { "Object", "Object", "CompressedSymbol", "Object", "Object"};
static char* errors_338[] = { "SelectorHasWrongNumberOfArguments", NULL};
static primitive_desc primitive_338 = {
	"primitivePerform:with:with:ifFail:", 
	fntype(&oopPrimitives::performWithWith), 
	1507332, 
	signature_338, 
	errors_338
};

static char* signature_339[] = { "Object", "Object", "CompressedSymbol", "Object", "Object", "Object"};
static char* errors_339[] = { "SelectorHasWrongNumberOfArguments", NULL};
static primitive_desc primitive_339 = {
	"primitivePerform:with:with:with:ifFail:", 
	fntype(&oopPrimitives::performWithWithWith), 
	1507333, 
	signature_339, 
	errors_339
};

static char* signature_340[] = { "Self", "Object"};
static char* errors_340[] = {NULL};
static primitive_desc primitive_340 = {
	"primitivePrint", 
	fntype(&oopPrimitives::print), 
	1114113, 
	signature_340, 
	errors_340
};

static char* signature_341[] = { "SmallInteger", "SmallInteger"};
static char* errors_341[] = { "OutOfBounds", NULL};
static primitive_desc primitive_341 = {
	"primitivePrintCharacterIfFail:", 
	fntype(&smiOopPrimitives::printCharacter), 
	1310721, 
	signature_341, 
	errors_341
};

static char* signature_342[] = { "Object", "SmallInteger"};
static char* errors_342[] = {NULL};
static primitive_desc primitive_342 = {
	"primitivePrintInvocationCounterHistogram:ifFail:", 
	fntype(&debugPrimitives::printInvocationCounterHistogram), 
	327681, 
	signature_342, 
	errors_342
};

static char* signature_343[] = { "Object"};
static char* errors_343[] = {NULL};
static primitive_desc primitive_343 = {
	"primitivePrintLayout", 
	fntype(&debugPrimitives::printMemoryLayout), 
	65536, 
	signature_343, 
	errors_343
};

static char* signature_344[] = { "Object"};
static char* errors_344[] = {NULL};
static primitive_desc primitive_344 = {
	"primitivePrintLookupCacheStatistics", 
	fntype(&debugPrimitives::printLookupCacheStatistics), 
	65536, 
	signature_344, 
	errors_344
};

static char* signature_345[] = { "Object"};
static char* errors_345[] = {NULL};
static primitive_desc primitive_345 = {
	"primitivePrintMemory", 
	fntype(&systemPrimitives::print_memory), 
	65536, 
	signature_345, 
	errors_345
};

static char* signature_346[] = { "Behavior", "Behavior", "ByteArray"};
static char* errors_346[] = {NULL};
static primitive_desc primitive_346 = {
	"primitivePrintMethod:ifFail:", 
	fntype(&behaviorPrimitives::printMethod), 
	1376258, 
	signature_346, 
	errors_346
};

static char* signature_347[] = { "Object", "Object", "Symbol"};
static char* errors_347[] = { "NotFound", NULL};
static primitive_desc primitive_347 = {
	"primitivePrintMethodCodes:ifFail:", 
	fntype(&debugPrimitives::printMethodCodes), 
	1376258, 
	signature_347, 
	errors_347
};

static char* signature_348[] = { "Object", "SmallInteger"};
static char* errors_348[] = {NULL};
static primitive_desc primitive_348 = {
	"primitivePrintNMethodCounterHistogram:ifFail:", 
	fntype(&debugPrimitives::printNMethodCounterHistogram), 
	327681, 
	signature_348, 
	errors_348
};

static char* signature_349[] = { "Object"};
static char* errors_349[] = {NULL};
static primitive_desc primitive_349 = {
	"primitivePrintObjectHistogram", 
	fntype(&debugPrimitives::printObjectHistogram), 
	65536, 
	signature_349, 
	errors_349
};

static char* signature_350[] = { "Object"};
static char* errors_350[] = {NULL};
static primitive_desc primitive_350 = {
	"primitivePrintPrimitiveCounters", 
	fntype(&debugPrimitives::printPrimitiveCounters), 
	65536, 
	signature_350, 
	errors_350
};

static char* signature_351[] = { "Object"};
static char* errors_351[] = {NULL};
static primitive_desc primitive_351 = {
	"primitivePrintPrimitiveTable", 
	fntype(&systemPrimitives::printPrimitiveTable), 
	65536, 
	signature_351, 
	errors_351
};

static char* signature_352[] = { "Self", "Object"};
static char* errors_352[] = {NULL};
static primitive_desc primitive_352 = {
	"primitivePrintValue", 
	fntype(&oopPrimitives::printValue), 
	1114113, 
	signature_352, 
	errors_352
};

static char* signature_353[] = { "Object"};
static char* errors_353[] = {NULL};
static primitive_desc primitive_353 = {
	"primitivePrintZone", 
	fntype(&systemPrimitives::print_zone), 
	65536, 
	signature_353, 
	errors_353
};

static char* signature_354[] = { "Object"};
static char* errors_354[] = {NULL};
static primitive_desc primitive_354 = {
	"primitiveProcessActiveProcess", 
	fntype(&processOopPrimitives::activeProcess), 
	65536, 
	signature_354, 
	errors_354
};

static char* signature_355[] = { "Process", "Process class", "BlockWithoutArguments"};
static char* errors_355[] = { "ProcessAllocationFailed", NULL};
static primitive_desc primitive_355 = {
	"primitiveProcessCreate:ifFail:", 
	fntype(&processOopPrimitives::create), 
	1376258, 
	signature_355, 
	errors_355
};

static char* signature_356[] = { "Process"};
static char* errors_356[] = {NULL};
static primitive_desc primitive_356 = {
	"primitiveProcessEnterCritical", 
	fntype(&processOopPrimitives::enter_critical), 
	65536, 
	signature_356, 
	errors_356
};

static char* signature_357[] = { "Process"};
static char* errors_357[] = {NULL};
static primitive_desc primitive_357 = {
	"primitiveProcessLeaveCritical", 
	fntype(&processOopPrimitives::leave_critical), 
	65536, 
	signature_357, 
	errors_357
};

static char* signature_358[] = { "Boolean", "Process", "SmallInteger"};
static char* errors_358[] = {NULL};
static primitive_desc primitive_358 = {
	"primitiveProcessSchedulerWait:ifFail:", 
	fntype(&processOopPrimitives::scheduler_wait), 
	1376258, 
	signature_358, 
	errors_358
};

static char* signature_359[] = { "Symbol", "Process", "Symbol", "Activation", "Object"};
static char* errors_359[] = { "InScheduler", "Dead", NULL};
static primitive_desc primitive_359 = {
	"primitiveProcessSetMode:activation:returnValue:ifFail:", 
	fntype(&processOopPrimitives::set_mode), 
	1376260, 
	signature_359, 
	errors_359
};

static char* signature_360[] = { "IndexedInstanceVariables", "Process", "SmallInteger"};
static char* errors_360[] = {NULL};
static primitive_desc primitive_360 = {
	"primitiveProcessStackLimit:ifFail:", 
	fntype(&processOopPrimitives::stack), 
	1376258, 
	signature_360, 
	errors_360
};

static char* signature_361[] = { "Object", "Process"};
static char* errors_361[] = { "NotInScheduler", "ProcessCannotContinue", "Dead", NULL};
static primitive_desc primitive_361 = {
	"primitiveProcessStartEvaluator:ifFail:", 
	fntype(&processOopPrimitives::start_evaluator), 
	327681, 
	signature_361, 
	errors_361
};

static char* signature_362[] = { "Symbol", "Process"};
static char* errors_362[] = {NULL};
static primitive_desc primitive_362 = {
	"primitiveProcessStatus", 
	fntype(&processOopPrimitives::status), 
	1114113, 
	signature_362, 
	errors_362
};

static char* signature_363[] = { "Process"};
static char* errors_363[] = {NULL};
static primitive_desc primitive_363 = {
	"primitiveProcessStop", 
	fntype(&processOopPrimitives::stop), 
	65536, 
	signature_363, 
	errors_363
};

static char* signature_364[] = { "Float", "Process"};
static char* errors_364[] = {NULL};
static primitive_desc primitive_364 = {
	"primitiveProcessSystemTime", 
	fntype(&processOopPrimitives::user_time), 
	1114113, 
	signature_364, 
	errors_364
};

static char* signature_365[] = { "Self", "Process"};
static char* errors_365[] = { "Dead", NULL};
static primitive_desc primitive_365 = {
	"primitiveProcessTerminateIfFail:", 
	fntype(&processOopPrimitives::terminate), 
	1507329, 
	signature_365, 
	errors_365
};

static char* signature_366[] = { "Self", "Process", "SmallInteger"};
static char* errors_366[] = {NULL};
static primitive_desc primitive_366 = {
	"primitiveProcessTraceStack:ifFail:", 
	fntype(&processOopPrimitives::trace_stack), 
	1376258, 
	signature_366, 
	errors_366
};

static char* signature_367[] = { "Object", "Process"};
static char* errors_367[] = { "NotInScheduler", "ProcessCannotContinue", "Dead", NULL};
static primitive_desc primitive_367 = {
	"primitiveProcessTransferTo:ifFail:", 
	fntype(&processOopPrimitives::transferTo), 
	327681, 
	signature_367, 
	errors_367
};

static char* signature_368[] = { "Float", "Process"};
static char* errors_368[] = {NULL};
static primitive_desc primitive_368 = {
	"primitiveProcessUserTime", 
	fntype(&processOopPrimitives::user_time), 
	1114113, 
	signature_368, 
	errors_368
};

static char* signature_369[] = { "Process"};
static char* errors_369[] = {NULL};
static primitive_desc primitive_369 = {
	"primitiveProcessYield", 
	fntype(&processOopPrimitives::yield), 
	65536, 
	signature_369, 
	errors_369
};

static char* signature_370[] = { "Process"};
static char* errors_370[] = {NULL};
static primitive_desc primitive_370 = {
	"primitiveProcessYieldInCritical", 
	fntype(&processOopPrimitives::yield_in_critical), 
	65536, 
	signature_370, 
	errors_370
};

static char* signature_371[] = { "SmallInteger", "Proxy", "SmallInteger"};
static char* errors_371[] = {NULL};
static primitive_desc primitive_371 = {
	"primitiveProxyByteAt:ifFail:", 
	fntype(&proxyOopPrimitives::byteAt), 
	5570562, 
	signature_371, 
	errors_371
};

static char* signature_372[] = { "SmallInteger", "Proxy", "SmallInteger", "SmallInteger"};
static char* errors_372[] = {NULL};
static primitive_desc primitive_372 = {
	"primitiveProxyByteAt:put:ifFail:", 
	fntype(&proxyOopPrimitives::byteAtPut), 
	5570563, 
	signature_372, 
	errors_372
};

static char* signature_373[] = { "Self", "Proxy", "SmallInteger"};
static char* errors_373[] = {NULL};
static primitive_desc primitive_373 = {
	"primitiveProxyCalloc:ifFail:", 
	fntype(&proxyOopPrimitives::calloc), 
	1376258, 
	signature_373, 
	errors_373
};

static char* signature_374[] = { "SmallInteger", "Proxy", "SmallInteger"};
static char* errors_374[] = {NULL};
static primitive_desc primitive_374 = {
	"primitiveProxyDoubleByteAt:ifFail:", 
	fntype(&proxyOopPrimitives::doubleByteAt), 
	1376258, 
	signature_374, 
	errors_374
};

static char* signature_375[] = { "SmallInteger", "Proxy", "SmallInteger", "SmallInteger"};
static char* errors_375[] = {NULL};
static primitive_desc primitive_375 = {
	"primitiveProxyDoubleByteAt:put:ifFail:", 
	fntype(&proxyOopPrimitives::doubleByteAtPut), 
	1376259, 
	signature_375, 
	errors_375
};

static char* signature_376[] = { "Float", "Proxy", "SmallInteger"};
static char* errors_376[] = {NULL};
static primitive_desc primitive_376 = {
	"primitiveProxyDoublePrecisionFloatAt:ifFail:", 
	fntype(&proxyOopPrimitives::doublePrecisionFloatAt), 
	1376258, 
	signature_376, 
	errors_376
};

static char* signature_377[] = { "Self", "Proxy", "SmallInteger", "Float"};
static char* errors_377[] = { "ConversionFailed", NULL};
static primitive_desc primitive_377 = {
	"primitiveProxyDoublePrecisionFloatAt:put:ifFail:", 
	fntype(&proxyOopPrimitives::doublePrecisionFloatAtPut), 
	1376259, 
	signature_377, 
	errors_377
};

static char* signature_378[] = { "Self", "Proxy"};
static char* errors_378[] = {NULL};
static primitive_desc primitive_378 = {
	"primitiveProxyFree", 
	fntype(&proxyOopPrimitives::free), 
	1114113, 
	signature_378, 
	errors_378
};

static char* signature_379[] = { "SmallInteger", "Proxy"};
static char* errors_379[] = {NULL};
static primitive_desc primitive_379 = {
	"primitiveProxyGetHigh", 
	fntype(&proxyOopPrimitives::getHigh), 
	1114113, 
	signature_379, 
	errors_379
};

static char* signature_380[] = { "SmallInteger", "Proxy"};
static char* errors_380[] = { "ConversionFailed", NULL};
static primitive_desc primitive_380 = {
	"primitiveProxyGetIfFail:", 
	fntype(&proxyOopPrimitives::getSmi), 
	1376257, 
	signature_380, 
	errors_380
};

static char* signature_381[] = { "SmallInteger", "Proxy"};
static char* errors_381[] = {NULL};
static primitive_desc primitive_381 = {
	"primitiveProxyGetLow", 
	fntype(&proxyOopPrimitives::getLow), 
	1114113, 
	signature_381, 
	errors_381
};

static char* signature_382[] = { "Boolean", "Proxy"};
static char* errors_382[] = {NULL};
static primitive_desc primitive_382 = {
	"primitiveProxyIsAllOnes", 
	fntype(&proxyOopPrimitives::isAllOnes), 
	1114113, 
	signature_382, 
	errors_382
};

static char* signature_383[] = { "Boolean", "Proxy"};
static char* errors_383[] = {NULL};
static primitive_desc primitive_383 = {
	"primitiveProxyIsNull", 
	fntype(&proxyOopPrimitives::isNull), 
	1114113, 
	signature_383, 
	errors_383
};

static char* signature_384[] = { "Self", "Proxy", "SmallInteger"};
static char* errors_384[] = {NULL};
static primitive_desc primitive_384 = {
	"primitiveProxyMalloc:ifFail:", 
	fntype(&proxyOopPrimitives::malloc), 
	1376258, 
	signature_384, 
	errors_384
};

static char* signature_385[] = { "Proxy", "Proxy", "SmallInteger", "Proxy"};
static char* errors_385[] = {NULL};
static primitive_desc primitive_385 = {
	"primitiveProxyProxyAt:put:ifFail:", 
	fntype(&proxyOopPrimitives::proxyAtPut), 
	1376259, 
	signature_385, 
	errors_385
};

static char* signature_386[] = { "Proxy", "Proxy", "SmallInteger", "Proxy"};
static char* errors_386[] = {NULL};
static primitive_desc primitive_386 = {
	"primitiveProxyProxyAt:result:ifFail:", 
	fntype(&proxyOopPrimitives::proxyAt), 
	1376259, 
	signature_386, 
	errors_386
};

static char* signature_387[] = { "Self", "Proxy", "SmallInteger|Proxy"};
static char* errors_387[] = {NULL};
static primitive_desc primitive_387 = {
	"primitiveProxySet:ifFail:", 
	fntype(&proxyOopPrimitives::set), 
	1376258, 
	signature_387, 
	errors_387
};

static char* signature_388[] = { "Self", "Proxy", "SmallInteger", "SmallInteger"};
static char* errors_388[] = {NULL};
static primitive_desc primitive_388 = {
	"primitiveProxySetHigh:low:ifFail:", 
	fntype(&proxyOopPrimitives::setHighLow), 
	1376259, 
	signature_388, 
	errors_388
};

static char* signature_389[] = { "Float", "Proxy", "SmallInteger"};
static char* errors_389[] = {NULL};
static primitive_desc primitive_389 = {
	"primitiveProxySinglePrecisionFloatAt:ifFail:", 
	fntype(&proxyOopPrimitives::singlePrecisionFloatAt), 
	1376258, 
	signature_389, 
	errors_389
};

static char* signature_390[] = { "Self", "Proxy", "SmallInteger", "Float"};
static char* errors_390[] = { "ConversionFailed", NULL};
static primitive_desc primitive_390 = {
	"primitiveProxySinglePrecisionFloatAt:put:ifFail:", 
	fntype(&proxyOopPrimitives::singlePrecisionFloatAtPut), 
	1376259, 
	signature_390, 
	errors_390
};

static char* signature_391[] = { "SmallInteger", "Proxy", "SmallInteger"};
static char* errors_391[] = { "ConversionFailed", NULL};
static primitive_desc primitive_391 = {
	"primitiveProxySmiAt:ifFail:", 
	fntype(&proxyOopPrimitives::smiAt), 
	1376258, 
	signature_391, 
	errors_391
};

static char* signature_392[] = { "SmallInteger", "Proxy", "SmallInteger", "SmallInteger"};
static char* errors_392[] = {NULL};
static primitive_desc primitive_392 = {
	"primitiveProxySmiAt:put:ifFail:", 
	fntype(&proxyOopPrimitives::smiAtPut), 
	1376259, 
	signature_392, 
	errors_392
};

static char* signature_393[] = { "Proxy", "Proxy", "SmallInteger", "Proxy"};
static char* errors_393[] = {NULL};
static primitive_desc primitive_393 = {
	"primitiveProxySubProxyAt:result:ifFail:", 
	fntype(&proxyOopPrimitives::subProxyAt), 
	1376259, 
	signature_393, 
	errors_393
};

static char* signature_394[] = { "BottomType"};
static char* errors_394[] = {NULL};
static primitive_desc primitive_394 = {
	"primitiveQuit", 
	fntype(&systemPrimitives::quit), 
	65536, 
	signature_394, 
	errors_394
};

static char* signature_395[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_395[] = { "NotImplementedYet", NULL};
static primitive_desc primitive_395 = {
	"primitiveQuo:ifFail:", 
	fntype(&smiOopPrimitives_quo), 
	6029826, 
	signature_395, 
	errors_395
};

static char* signature_396[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_396[] = {NULL};
static primitive_desc primitive_396 = {
	"primitiveRawBitShift:ifFail:", 
	fntype(&smiOopPrimitives::rawBitShift), 
	6029826, 
	signature_396, 
	errors_396
};

static char* signature_397[] = { "Object", "Process"};
static char* errors_397[] = {NULL};
static primitive_desc primitive_397 = {
	"primitiveRecordMainProcessIfFail:", 
	fntype(&processOopPrimitives::setMainProcess), 
	1376257, 
	signature_397, 
	errors_397
};

static char* signature_398[] = { "IndexedInstanceVariables", "Object", "SmallInteger"};
static char* errors_398[] = {NULL};
static primitive_desc primitive_398 = {
	"primitiveReferencesTo:limit:ifFail:", 
	fntype(&systemPrimitives::references_to), 
	327682, 
	signature_398, 
	errors_398
};

static char* signature_399[] = { "IndexedInstanceVariables", "Class", "SmallInteger"};
static char* errors_399[] = {NULL};
static primitive_desc primitive_399 = {
	"primitiveReferencesToInstancesOf:limit:ifFail:", 
	fntype(&systemPrimitives::references_to_instances_of), 
	327682, 
	signature_399, 
	errors_399
};

static char* signature_400[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_400[] = { "DivisionByZero", NULL};
static primitive_desc primitive_400 = {
	"primitiveRemainder:ifFail:", 
	fntype(&smiOopPrimitives_remainder), 
	6029826, 
	signature_400, 
	errors_400
};

static char* signature_401[] = { "BottomType", "BlockWithoutArguments"};
static char* errors_401[] = {NULL};
static primitive_desc primitive_401 = {
	"primitiveRepeat", 
	fntype(&blockRepeat), 
	1245185, 
	signature_401, 
	errors_401
};

static char* signature_402[] = { "Self", "Object"};
static char* errors_402[] = {NULL};
static primitive_desc primitive_402 = {
	"primitiveScavenge", 
	fntype(&systemPrimitives::scavenge), 
	1114113, 
	signature_402, 
	errors_402
};

static char* signature_403[] = { "Object", "SmallInteger"};
static char* errors_403[] = {NULL};
static primitive_desc primitive_403 = {
	"primitiveSetInterpreterInvocationCounterLimitTo:ifFail:", 
	fntype(&debugPrimitives::setInterpreterInvocationCounterLimit), 
	327681, 
	signature_403, 
	errors_403
};

static char* signature_404[] = { "Behavior", "Behavior", "Behavior"};
static char* errors_404[] = { "NotAClass", NULL};
static primitive_desc primitive_404 = {
	"primitiveSetSuperclassOf:toClass:ifFail:", 
	fntype(&behaviorPrimitives::setSuperclass), 
	327682, 
	signature_404, 
	errors_404
};

static char* signature_405[] = { "Object", "Object"};
static char* errors_405[] = { "ReceiverHasWrongType", NULL};
static primitive_desc primitive_405 = {
	"primitiveShallowCopyIfFail:", 
	fntype(&oopPrimitives::shallowCopy), 
	1376257, 
	signature_405, 
	errors_405
};

static char* signature_406[] = { "Object", "SmallInteger"};
static char* errors_406[] = {NULL};
static primitive_desc primitive_406 = {
	"primitiveShrinkMemory:", 
	fntype(&systemPrimitives::shrinkMemory), 
	65537, 
	signature_406, 
	errors_406
};

static char* signature_407[] = { "SmallInteger"};
static char* errors_407[] = {NULL};
static primitive_desc primitive_407 = {
	"primitiveSizeOfOop", 
	fntype(&systemPrimitives::oopSize), 
	65536, 
	signature_407, 
	errors_407
};

static char* signature_408[] = { "IndexedInstanceVariables"};
static char* errors_408[] = {NULL};
static primitive_desc primitive_408 = {
	"primitiveSlidingSystemAverageIfFail:", 
	fntype(&systemPrimitives::sliding_system_average), 
	327680, 
	signature_408, 
	errors_408
};

static char* signature_409[] = { "Boolean", "SmallInteger", "SmallInteger"};
static char* errors_409[] = {NULL};
static primitive_desc primitive_409 = {
	"primitiveSmallIntegerEqual:ifFail:", 
	fntype(&smiOopPrimitives::equal), 
	6029570, 
	signature_409, 
	errors_409
};

static char* signature_410[] = { "SmallInteger", "Symbol"};
static char* errors_410[] = { "NotFound", NULL};
static primitive_desc primitive_410 = {
	"primitiveSmallIntegerFlagAt:ifFail:", 
	fntype(&debugPrimitives::smiAt), 
	327681, 
	signature_410, 
	errors_410
};

static char* signature_411[] = { "Boolean", "Symbol", "Boolean"};
static char* errors_411[] = { "NotFound", NULL};
static primitive_desc primitive_411 = {
	"primitiveSmallIntegerFlagAt:put:ifFail:", 
	fntype(&debugPrimitives::smiAtPut), 
	327682, 
	signature_411, 
	errors_411
};

static char* signature_412[] = { "Boolean", "SmallInteger", "SmallInteger"};
static char* errors_412[] = {NULL};
static primitive_desc primitive_412 = {
	"primitiveSmallIntegerNotEqual:ifFail:", 
	fntype(&smiOopPrimitives::notEqual), 
	6029570, 
	signature_412, 
	errors_412
};

static char* signature_413[] = { "IndexedInstanceVariables"};
static char* errors_413[] = {NULL};
static primitive_desc primitive_413 = {
	"primitiveSmalltalkArray", 
	fntype(&systemPrimitives::smalltalk_array), 
	65536, 
	signature_413, 
	errors_413
};

static char* signature_414[] = { "GlobalAssociation", "Symbol", "Object"};
static char* errors_414[] = {NULL};
static primitive_desc primitive_414 = {
	"primitiveSmalltalkAt:Put:ifFail:", 
	fntype(&systemPrimitives::smalltalk_at_put), 
	327682, 
	signature_414, 
	errors_414
};

static char* signature_415[] = { "GlobalAssociation", "SmallInteger"};
static char* errors_415[] = { "OutOfBounds", NULL};
static primitive_desc primitive_415 = {
	"primitiveSmalltalkAt:ifFail:", 
	fntype(&systemPrimitives::smalltalk_at), 
	327681, 
	signature_415, 
	errors_415
};

static char* signature_416[] = { "GlobalAssociation", "SmallInteger"};
static char* errors_416[] = {NULL};
static primitive_desc primitive_416 = {
	"primitiveSmalltalkRemoveAt:ifFail:", 
	fntype(&systemPrimitives::smalltalk_remove_at), 
	327681, 
	signature_416, 
	errors_416
};

static char* signature_417[] = { "SmallInteger"};
static char* errors_417[] = {NULL};
static primitive_desc primitive_417 = {
	"primitiveSmalltalkSize", 
	fntype(&systemPrimitives::smalltalk_size), 
	65536, 
	signature_417, 
	errors_417
};

static char* signature_418[] = { "SmallInteger", "SmallInteger", "SmallInteger"};
static char* errors_418[] = { "Overflow", NULL};
static primitive_desc primitive_418 = {
	"primitiveSubtract:ifFail:", 
	fntype(&smiOopPrimitives_subtract), 
	6029826, 
	signature_418, 
	errors_418
};

static char* signature_419[] = { "Behavior|Nil", "Behavior"};
static char* errors_419[] = {NULL};
static primitive_desc primitive_419 = {
	"primitiveSuperclass", 
	fntype(&behaviorPrimitives::superclass), 
	1114113, 
	signature_419, 
	errors_419
};

static char* signature_420[] = { "Behavior|Nil", "Behavior"};
static char* errors_420[] = {NULL};
static primitive_desc primitive_420 = {
	"primitiveSuperclassOf:ifFail:", 
	fntype(&behaviorPrimitives::superclass_of), 
	327681, 
	signature_420, 
	errors_420
};

static char* signature_421[] = { "SmallInteger", "IndexedByteInstanceVariables"};
static char* errors_421[] = {NULL};
static primitive_desc primitive_421 = {
	"primitiveSymbolNumberOfArguments", 
	fntype(&byteArrayPrimitives::numberOfArguments), 
	1574401, 
	signature_421, 
	errors_421
};

static char* signature_422[] = { "Float"};
static char* errors_422[] = {NULL};
static primitive_desc primitive_422 = {
	"primitiveSystemTime", 
	fntype(&systemPrimitives::systemTime), 
	65536, 
	signature_422, 
	errors_422
};

static char* signature_423[] = { "Object"};
static char* errors_423[] = {NULL};
static primitive_desc primitive_423 = {
	"primitiveTimerPrintBuffer", 
	fntype(&debugPrimitives::timerPrintBuffer), 
	65536, 
	signature_423, 
	errors_423
};

static char* signature_424[] = { "Object"};
static char* errors_424[] = {NULL};
static primitive_desc primitive_424 = {
	"primitiveTimerStart", 
	fntype(&debugPrimitives::timerStart), 
	65536, 
	signature_424, 
	errors_424
};

static char* signature_425[] = { "Object"};
static char* errors_425[] = {NULL};
static primitive_desc primitive_425 = {
	"primitiveTimerStop", 
	fntype(&debugPrimitives::timerStop), 
	65536, 
	signature_425, 
	errors_425
};

static char* signature_426[] = { "Object"};
static char* errors_426[] = {NULL};
static primitive_desc primitive_426 = {
	"primitiveTraceStack", 
	fntype(&systemPrimitives::traceStack), 
	65536, 
	signature_426, 
	errors_426
};

static char* signature_427[] = { "Object", "BlockWithoutArguments", "BlockWithoutArguments"};
static char* errors_427[] = {NULL};
static primitive_desc primitive_427 = {
	"primitiveUnwindProtect:ifFail:", 
	fntype(&unwindprotect), 
	1507330, 
	signature_427, 
	errors_427
};

static char* signature_428[] = { "Float"};
static char* errors_428[] = {NULL};
static primitive_desc primitive_428 = {
	"primitiveUserTime", 
	fntype(&systemPrimitives::userTime), 
	65536, 
	signature_428, 
	errors_428
};

static char* signature_429[] = { "Object"};
static char* errors_429[] = {NULL};
static primitive_desc primitive_429 = {
	"primitiveVMBreakpoint", 
	fntype(&systemPrimitives::vmbreakpoint), 
	65536, 
	signature_429, 
	errors_429
};

static char* signature_430[] = { "Object", "BlockWithoutArguments"};
static char* errors_430[] = {NULL};
static primitive_desc primitive_430 = {
	"primitiveValue", 
	fntype(&primitiveValue0), 
	5441537, 
	signature_430, 
	errors_430
};

static char* signature_431[] = { "Object", "BlockWithOneArgument", "Object"};
static char* errors_431[] = {NULL};
static primitive_desc primitive_431 = {
	"primitiveValue:", 
	fntype(&primitiveValue1), 
	5441538, 
	signature_431, 
	errors_431
};

static char* signature_432[] = { "Object", "BlockWithTwoArguments", "Object", "Object"};
static char* errors_432[] = {NULL};
static primitive_desc primitive_432 = {
	"primitiveValue:value:", 
	fntype(&primitiveValue2), 
	5441539, 
	signature_432, 
	errors_432
};

static char* signature_433[] = { "Object", "BlockWithThreeArguments", "Object", "Object", "Object"};
static char* errors_433[] = {NULL};
static primitive_desc primitive_433 = {
	"primitiveValue:value:value:", 
	fntype(&primitiveValue3), 
	5441540, 
	signature_433, 
	errors_433
};

static char* signature_434[] = { "Object", "BlockWithFourArguments", "Object", "Object", "Object", "Object"};
static char* errors_434[] = {NULL};
static primitive_desc primitive_434 = {
	"primitiveValue:value:value:value:", 
	fntype(&primitiveValue4), 
	5441541, 
	signature_434, 
	errors_434
};

static char* signature_435[] = { "Object", "BlockWithFiveArguments", "Object", "Object", "Object", "Object", "Object"};
static char* errors_435[] = {NULL};
static primitive_desc primitive_435 = {
	"primitiveValue:value:value:value:value:", 
	fntype(&primitiveValue5), 
	5441542, 
	signature_435, 
	errors_435
};

static char* signature_436[] = { "Object", "BlockWithSixArguments", "Object", "Object", "Object", "Object", "Object", "Object"};
static char* errors_436[] = {NULL};
static primitive_desc primitive_436 = {
	"primitiveValue:value:value:value:value:value:", 
	fntype(&primitiveValue6), 
	5441543, 
	signature_436, 
	errors_436
};

static char* signature_437[] = { "Object", "BlockWithSevenArguments", "Object", "Object", "Object", "Object", "Object", "Object", "Object"};
static char* errors_437[] = {NULL};
static primitive_desc primitive_437 = {
	"primitiveValue:value:value:value:value:value:value:", 
	fntype(&primitiveValue7), 
	5441544, 
	signature_437, 
	errors_437
};

static char* signature_438[] = { "Object", "BlockWithEightArguments", "Object", "Object", "Object", "Object", "Object", "Object", "Object", "Object"};
static char* errors_438[] = {NULL};
static primitive_desc primitive_438 = {
	"primitiveValue:value:value:value:value:value:value:value:", 
	fntype(&primitiveValue8), 
	5441545, 
	signature_438, 
	errors_438
};

static char* signature_439[] = { "Object", "BlockWithNineArguments", "Object", "Object", "Object", "Object", "Object", "Object", "Object", "Object", "Object"};
static char* errors_439[] = {NULL};
static primitive_desc primitive_439 = {
	"primitiveValue:value:value:value:value:value:value:value:value:", 
	fntype(&primitiveValue9), 
	5441546, 
	signature_439, 
	errors_439
};

static char* signature_440[] = { "Object"};
static char* errors_440[] = {NULL};
static primitive_desc primitive_440 = {
	"primitiveVerify", 
	fntype(&debugPrimitives::verify), 
	65536, 
	signature_440, 
	errors_440
};

static char* signature_441[] = { "Proxy", "Proxy"};
static char* errors_441[] = {NULL};
static primitive_desc primitive_441 = {
	"primitiveWindowsHInstance:ifFail:", 
	fntype(&systemPrimitives::windowsHInstance), 
	327681, 
	signature_441, 
	errors_441
};

static char* signature_442[] = { "Proxy", "Proxy"};
static char* errors_442[] = {NULL};
static primitive_desc primitive_442 = {
	"primitiveWindowsHPrevInstance:ifFail:", 
	fntype(&systemPrimitives::windowsHPrevInstance), 
	327681, 
	signature_442, 
	errors_442
};

static char* signature_443[] = { "Object"};
static char* errors_443[] = {NULL};
static primitive_desc primitive_443 = {
	"primitiveWindowsNCmdShow", 
	fntype(&systemPrimitives::windowsNCmdShow), 
	65536, 
	signature_443, 
	errors_443
};

static char* signature_444[] = { "Object", "String"};
static char* errors_444[] = {NULL};
static primitive_desc primitive_444 = {
	"primitiveWriteSnapshot:", 
	fntype(&systemPrimitives::writeSnapshot), 
	65537, 
	signature_444, 
	errors_444
};

static int size_of_primitive_table = 445; 
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
  &primitive_398,
  &primitive_399,
  &primitive_400,
  &primitive_401,
  &primitive_402,
  &primitive_403,
  &primitive_404,
  &primitive_405,
  &primitive_406,
  &primitive_407,
  &primitive_408,
  &primitive_409,
  &primitive_410,
  &primitive_411,
  &primitive_412,
  &primitive_413,
  &primitive_414,
  &primitive_415,
  &primitive_416,
  &primitive_417,
  &primitive_418,
  &primitive_419,
  &primitive_420,
  &primitive_421,
  &primitive_422,
  &primitive_423,
  &primitive_424,
  &primitive_425,
  &primitive_426,
  &primitive_427,
  &primitive_428,
  &primitive_429,
  &primitive_430,
  &primitive_431,
  &primitive_432,
  &primitive_433,
  &primitive_434,
  &primitive_435,
  &primitive_436,
  &primitive_437,
  &primitive_438,
  &primitive_439,
  &primitive_440,
  &primitive_441,
  &primitive_442,
  &primitive_443,
  &primitive_444
}; 
