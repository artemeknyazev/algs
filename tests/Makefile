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

$(BUILD_DIR) :
	mkdir -p $(BUILD_DIR)

OBJ_FILES := $(addprefix $(BUILD_DIR)/,sort.o tree.o)
$(OBJ_FILES): | $(BUILD_DIR)

$(BUILD_DIR)/sort.o : $(SCENARIOS_DIR)/sort.cpp \
	$(HELPERS_DIR)/test_sort_on_container.hpp \
	$(SCENARIOS_DIR)/sort/selection.hpp $(LIB_DIR)/sort/selection.hpp \
	$(SCENARIOS_DIR)/sort/insertion.hpp $(LIB_DIR)/sort/insertion.hpp \
	$(SCENARIOS_DIR)/sort/shell.hpp $(LIB_DIR)/sort/shell.hpp \
	$(SCENARIOS_DIR)/sort/merge.hpp $(LIB_DIR)/sort/merge.hpp \
	$(SCENARIOS_DIR)/sort/heap.hpp $(LIB_DIR)/sort/heap.hpp \
	$(SCENARIOS_DIR)/sort/quicksort.hpp $(LIB_DIR)/sort/quicksort.hpp

$(BUILD_DIR)/tree.o : $(SCENARIOS_DIR)/tree.cpp \
	$(SCENARIOS_DIR)/tree/bst_recursive.hpp $(LIB_DIR)/tree/bst_recursive.hpp \
	$(SCENARIOS_DIR)/tree/avl.hpp $(LIB_DIR)/tree/avl.hpp

$(BUILD_DIR)/%.o :
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(filter %.cpp,$^) -o $@

# note: gtest_main.a defines main, gtest.a don't
$(BUILD_DIR)/test : $(OBJ_FILES) $(BUILD_DIR)/gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^

test : $(BUILD_DIR)/test

run : test
	./$(BUILD_DIR)/test

clean :
	rm -rf $(BUILD_DIR)

all : run

