# makefile for Strongtalk
#
# revision history
# -----------------------------------------------------------------------
# 2007.02.20	dave.raymer@gmail.com		file created
# -----------------------------------------------------------------------

include makefile-macros.incl

LIB_DIR = $(BIN_DIR)/asm_objs/COFF/converted
TEST_DIRECTORIES	= $(TEST_SRC_DIR)/utilities \
$(TEST_SRC_DIR)/main
SCRIPT_EXECUTABLE = strongtalk_script
TEST-EXECUTABLE = strongtalk-test
TEST_SHARED_LIB = strongtalk-test.so
EXECUTABLE = strongtalk
SHARED_LIB = strongtalk.so
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

default: dependencies objects link test

test:
	export LD_LIBRARY_PATH=$(WRK)
	./$(TEST-EXECUTABLE)

link:
#	g++ -v $(OPT) -o $(EXECUTABLE) -L/usr/lib $(VM_SRC_DIR)/*/*.o -lpthread -ldl -lrt
	g++ -v $(OPT) -shared -o $(SHARED_LIB) -L/usr/lib `find $(VM_SRC_DIR) -name *.o|grep -v main.o` -lpthread -ldl -lrt
	g++ -v $(OPT) -o $(EXECUTABLE) -L/usr/lib $(VM_SRC_DIR)/*/main.o $(SHARED_LIB)
	g++ -v $(OPT) -shared -o $(TEST_SHARED_LIB) -L/usr/lib `find $(TEST_SRC_DIR) -name *.o|grep -v main.o` -leasyunit
	g++ -v $(OPT) -o $(TEST-EXECUTABLE) $(TEST_SHARED_LIB) $(SHARED_LIB) $(TEST_SRC_DIR)/main/main.o

objects:
	for i in $(DIRECTORIES);\
	do \
		make -C $$i -f $(BUILD_DIR)/makefile.incl $@;\
	done

$(BUILD_DIR)/makedeps: $(TOOLS_SRC_DIR)/makedeps/makeDeps.cpp
	$(C++) -o $@ $^

dependencies: $(BUILD_DIR)/makedeps $(INCL_PLATFORM) includeDB.all
	$(BUILD_DIR)/makedeps $(INCL_PLATFORM) includeDB.all
	$(MV) includeDB.all includeDB.current
	$(TOUCH) $(INCL_DIR)/_precompiled.incl

includeDB.all: $(INCL_DB1) $(INCL_DB2) 
	$(RMDIR) $(INCL_DIR)
	$(RM) Dependencies.hh includeDB.all includeDB.current
	$(MKDIR) $(INCL_DIR)
	$(CAT) $(INCL_DB1) $(INCL_DB2) >> includeDB.all

$(INCL_PLATFORM):
	$(TOUCH) $@

clean:
	$(RMDIR) $(INCL_DIR)
	$(RM) Dependencies.hh includeDB.all includeDB.current
	$(RM) *~
	for i in $(DIRECTORIES);\
	do \
		make -C $$i -f $(BUILD_DIR)/makefile.incl clean;\
	done

pristine: clean
	$(RM) $(BIN_DIR)/makedeps
