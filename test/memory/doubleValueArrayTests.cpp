# include "incls/_precompiled.incl"
# include "incls/_dValueArrayKlass.cpp.incl"
#include "test.h"

using namespace easyunit;

// reinstate once status of dValueArray* is firmly established

//extern "C" oop* eden_top;
//extern "C" oop* eden_end;
//
//DECLARE(DoubleValueArrayKlassTests)
//  //klassOop theClass;
//  //oop* oldEdenTop;
//END_DECLARE
//
//SETUP(DoubleValueArrayKlassTests) {
//  //theClass = klassOop(Universe::find_global("DoubleValueArray"));
//  //oldEdenTop = eden_top;
//}
//
//TEARDOWN(DoubleValueArrayKlassTests){
//  //eden_top = oldEdenTop;
//  //MarkSweep::collect();
//}
//
////TESTF(DoubleValueArrayKlassTests, shouldBeDoubleDoubleValueArray) {
////  eden_top = eden_end;
////  ASSERT_TRUE(theClass->klass_part()->oop_is_DoubleValueArray());
////}
////TESTF(DoubleValueArrayKlassTests, allocateShouldFailWhenAllowedAndNoSpace) {
////  eden_top = eden_end;
////  ASSERT_EQUALS((int)NULL, (int)(theClass->klass_part()->allocateObjectSize(100, false)));
////}
////
////TESTF(DoubleValueArrayKlassTests, allocateShouldAllocateTenuredWhenRequired) {
////  ASSERT_TRUE(Universe::old_gen.contains(theClass->klass_part()->allocateObjectSize(100, false, true)));
////}
////
////TESTF(DoubleValueArrayKlassTests, allocateShouldNotFailWhenNotAllowedAndNoSpace) {
////  eden_top = eden_end;
////  ASSERT_TRUE(Universe::new_gen.eden()->free() < 4 * oopSize);
////  ASSERT_TRUE(Universe::new_gen.contains(theClass->klass_part()->allocateObjectSize(100, true)));
////}
//
//class findDoubleValueArray : public klassOopClosure {
//public:
//  char* className;
//  bool found;
//  findDoubleValueArray() {
//    className = NULL;
//    found = false;
//  }
//  void do_klass(klassOop klass) {
//    oop instance = klass->primitive_allocate_size(1);
//    if (instance->is_doubleValueArray()){
//      symbolOop name = symbolOop(klass->instVarAt(klassOopDesc::header_size()));
//      char* sname = name->chars();
//      className = sname;
//      found = true;
//      printf("Class name is : %s)", sname);
//    }
//  }
//};
//
//TESTF(DoubleValueArrayKlassTests, findDoubleValueArrayClass) {
//  findDoubleValueArray closure;
//  Universe::classes_do(&closure);
//  if (!closure.found)
//    printf("No matching class found :(");
//}
