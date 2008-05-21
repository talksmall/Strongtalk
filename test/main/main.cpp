#include "incls/_precompiled.incl"
#include "incls/_os.cpp.incl"
#include "easyunit/testharness.h"

void ostream_init();

using namespace easyunit;

int main() {
  os_init();
	TestRegistry::runAndPrint();
  os_exit();
}
