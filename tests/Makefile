# --- DEVELOPER AREA START ---

# We use nested namespaces for convenience
CXXFLAGS += -g -std=c++1z -Wall -Wextra

# Include project-specific files
CPPFLAGS += -I ../include

# Where to find library code
LIB_DIR = ../include/algs

# Where to find test scenarios
SCENARIOS_DIR = scenarios

# Where to find test helpers
HELPERS_DIR = helpers

# Where to put intermediate files and test executables
BUILD_DIR = build

# --- DEVELOPER AREA END ---

# --- GOOGLE TEST AREA START; DO NOT TOUCH ---

GTEST_DIR = ../submodules/googletest/googletest

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

CPPFLAGS += -isystem $(GTEST_DIR)/include

# note: 2>&1 - pipe stderr to stdout, >/dev/null - pipe stdout to /dev/null, use 2>&1 >/dev/null
$(BUILD_DIR)/gtest.a :
	BUILD_DIR=$(BUILD_DIR) $(MAKE) -f Makefile.googletest gtest.a

$(BUILD_DIR)/gtest_main.a :
	BUILD_DIR=$(BUILD_DIR) $(MAKE) -f Makefile.googletest gtest_main.a

# --- GOOGLE TEST AREA END; DO NOT TOUCH ---

# --- DEVELOPER AREA START (add tests here) ---

OBJ_FILES_SORT := $(addprefix sort_,selection.o insertion.o shell.o merge.o heap.o quicksort.o)
$(BUILD_DIR)/sort_selection.o : $(SCENARIOS_DIR)/sort/common.hpp \
	$(SCENARIOS_DIR)/sort/selection.cpp \
	$(LIB_DIR)/sort/selection.hpp
$(BUILD_DIR)/sort_insertion.o : $(SCENARIOS_DIR)/sort/common.hpp \
	$(SCENARIOS_DIR)/sort/insertion.cpp \
	$(LIB_DIR)/sort/insertion.hpp
$(BUILD_DIR)/sort_shell.o : $(SCENARIOS_DIR)/sort/common.hpp \
	$(SCENARIOS_DIR)/sort/shell.cpp \
	$(LIB_DIR)/sort/shell.hpp
$(BUILD_DIR)/sort_merge.o : $(SCENARIOS_DIR)/sort/common.hpp \
	$(SCENARIOS_DIR)/sort/merge.cpp \
	$(LIB_DIR)/sort/merge.hpp
$(BUILD_DIR)/sort_heap.o : $(SCENARIOS_DIR)/sort/common.hpp \
	$(SCENARIOS_DIR)/sort/heap.cpp \
	$(LIB_DIR)/sort/heap.hpp
$(BUILD_DIR)/sort_quicksort.o : $(SCENARIOS_DIR)/sort/common.hpp \
	$(SCENARIOS_DIR)/sort/quicksort.cpp \
	$(LIB_DIR)/sort/quicksort.hpp

OBJ_FILES_TREE := $(addprefix tree_,bst_recursive.o avl.o)
$(BUILD_DIR)/tree_bst_recursive.o : $(SCENARIOS_DIR)/tree/common.hpp \
	$(SCENARIOS_DIR)/tree/bst_recursive.cpp \
	$(LIB_DIR)/tree/bst_recursive.hpp
$(BUILD_DIR)/tree_avl.o : $(SCENARIOS_DIR)/tree/common.hpp \
	$(SCENARIOS_DIR)/tree/avl.cpp \
	$(LIB_DIR)/tree/avl.hpp

OBJ_FILES := $(addprefix $(BUILD_DIR)/,$(OBJ_FILES_SORT) $(OBJ_FILES_TREE))
$(BUILD_DIR) :
	mkdir -p $(BUILD_DIR)
$(OBJ_FILES): | $(BUILD_DIR)
$(BUILD_DIR)/%.o :
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(filter %.cpp,$^) -o $@

# note: gtest_main.a defines main, gtest.a don't
$(BUILD_DIR)/test : $(OBJ_FILES) $(BUILD_DIR)/gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^

# --- DEVELOPER AREA START (general commands) ---

test : $(BUILD_DIR)/test

run : test
	./$(BUILD_DIR)/test

clean :
	rm -rf $(BUILD_DIR)

all : run

help :
	@echo "Available commands:" && \
	echo "    <empty>    (default) build and run tests" && \
	echo "    test       build tests" && \
	echo "    run        build and run tests" && \
	echo "    clean      clean tests build folder" && \
	echo "    help       this message"

