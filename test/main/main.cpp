#include "incls/_precompiled.incl"
#include "incls/_shell.cpp.incl"
#include "easyunit/testharness.h"

void ostream_init();

using namespace easyunit;
extern "C" void load_image();

int main(int argc, char* argv[]) {
  parse_arguments(argc, argv);
  init_globals();
  load_image();
	TestRegistry::runAndPrint();
}
