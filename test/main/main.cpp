#include "easyunit/testharness.h"
# include "incls/_precompiled.incl"
# include "incls/_debug.cpp.incl"

void ostream_init();

using namespace easyunit;

int main() {
  AllowGlobalNew = true;
	TestRegistry::runAndPrint();
}
