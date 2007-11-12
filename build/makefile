# makefile for Strongtalk
#
# revision history
# -----------------------------------------------------------------------
# 2007.02.20	dave.raymer@gmail.com		file created
# -----------------------------------------------------------------------

include makefile-macros.incl

LIB_DIR = $(BIN_DIR)/asm_objs/COFF/converted
TEST_DIRECTORIES	= $(TEST_SRC_DIR)/runtime
SCRIPT_EXECUTABLE = strongtalk_script
EXECUTABLE = strongtalk
DIRECTORIES	= $(VM_SRC_DIR)/asm \
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

default: dependencies objects link

link:
	g++ -v -o $(EXECUTABLE) -L/usr/lib $(VM_SRC_DIR)/*/*.o -lpthread -ldl -lrt

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
