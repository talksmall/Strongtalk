# makefile for Strongtalk
#
# revision history
# -----------------------------------------------------------------------
# 2007.02.20	dave.raymer@gmail.com		file created
# -----------------------------------------------------------------------

include ../build.linux/makefile-macros.incl

LIB_DIR = $(BIN_DIR)/asm_objs/COFF/converted
TEST_DIRECTORIES	= $(TEST_SRC_DIR)/utilities \
$(TEST_SRC_DIR)/runtime \
$(TEST_SRC_DIR)/prims \
$(TEST_SRC_DIR)/main \
$(EASYUNIT_DIR)/src
TEST-EXECUTABLE = $(BUILD_DIR)/strongtalk-test
TEST_SHARED_LIB = $(BUILD_DIR)/strongtalk-test.so
EXECUTABLE = $(BUILD_DIR)/strongtalk
SHARED_LIB = $(BUILD_DIR)/strongtalk.so
DIRECTORIES	= $(TEST_DIRECTORIES) \
$(VM_SRC_DIR)/asm \
$(VM_SRC_DIR)/code \
$(VM_SRC_DIR)/compiler \
$(VM_SRC_DIR)/disasm \
$(VM_SRC_DIR)/interpreter \
$(VM_SRC_DIR)/lookup \
$(VM_SRC_DIR)/memory \
$(VM_SRC_DIR)/oops \
$(VM_SRC_DIR)/prims \
$(VM_SRC_DIR)/recompiler \
$(VM_SRC_DIR)/runtime \
$(VM_SRC_DIR)/utilities
WRK=$(shell pwd)

ifneq ($(WRK),$(BUILD_DIR))
$(warning "Running in $(WRK)")
$(error "Must be run from build directory: $(BUILD_DIR)")
endif

default: dependencies objects link test

test:
	export LD_LIBRARY_PATH=$(WRK)
	$(TEST-EXECUTABLE) -b ../strongtalk.bst

link:
	g++ -v $(OPT) $(LDOPTIONS)-shared -o $(SHARED_LIB) -L/usr/lib32 `find $(VM_SRC_DIR) -name "*.o"|grep -v main.o` -lpthread -ldl -lrt 
	g++ -v $(OPT) $(LDOPTIONS) -o $(EXECUTABLE) -L/usr/lib32 $(VM_SRC_DIR)/*/main.o $(SHARED_LIB)
	g++ -v $(OPT) $(LDOPTIONS) -shared -o $(TEST_SHARED_LIB) -L/usr/lib32 `find $(TEST_SRC_DIR) -name "*.o"|grep -v main.o` $(EASYUNIT_DIR)/src/*.o
	g++ -v $(OPT) $(LDOPTIONS) -o $(TEST-EXECUTABLE) $(SHARED_LIB) $(TEST_SHARED_LIB) $(TEST_SRC_DIR)/main/main.o

objects: incls/_precompiled.incl.gch
	for i in $(DIRECTORIES);\
	do \
		make -C $$i -f $(MAKE_DIR)/makefile.incl $@;\
	done

incls/_precompiled.incl.gch: includeDB.all
	$(C++) -c -x c++-header -o $(BUILD_DIR)/incls/_precompiled.incl.gch -g $(OPT) $(C++OPTIONS) $(DEFINES) $(INCL_PATH) incls/_precompiled.incl

$(BUILD_DIR)/makedeps: $(TOOLS_SRC_DIR)/makedeps/makeDeps.cpp
	$(C++) -m32 -Wno-write-strings -o $@ $^

dependencies: $(BUILD_DIR)/makedeps $(INCL_PLATFORM) $(BUILD_DIR)/includeDB.all
	$(BUILD_DIR)/makedeps $(INCL_PLATFORM) $(BUILD_DIR)/includeDB.all

$(BUILD_DIR)/includeDB.all: $(INCL_DB1) $(INCL_DB2) 
	$(RMDIR) $(INCL_DIR)
	$(RM) Dependencies.hh $(BUILD_DIR)/includeDB.all includeDB.current
	$(MKDIR) $(INCL_DIR)
	$(CAT) $(INCL_DB1) $(INCL_DB2) >> $(BUILD_DIR)/includeDB.all

$(INCL_PLATFORM):
	$(TOUCH) $@

clean:
	$(RMDIR) $(INCL_DIR)
	$(RM) Dependencies.hh $(BUILD_DIR)/includeDB.all includeDB.current
	$(RM) *~
	for i in $(DIRECTORIES);\
	do \
		make -C $$i -f $(MAKE_DIR)/makefile.incl clean;\
	done

pristine: clean
	$(RM) $(BUILD_DIR)/makedeps
