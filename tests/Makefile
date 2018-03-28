# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR = ../submodules/googletest/googletest

# Where to find user code.
LIB_DIR = ../include/algs
SCENARIOS_DIR = scenarios
BUILD_DIR = build

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Include project-specific files
CPPFLAGS += -I ../include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread -std=c++1z

# Add test targets here
TESTS = sort_selection

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# House-keeping build targets.

all : $(TESTS)

clean :
	rm -f $(TESTS) gtest.a gtest_main.a *.o

# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) \
		-c $(GTEST_DIR)/src/gtest-all.cc \
		-o $(BUILD_DIR)/gtest-all.o

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) \
		-c $(GTEST_DIR)/src/gtest_main.cc \
		-o $(BUILD_DIR)/gtest_main.o

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $(BUILD_DIR)/$@ $(BUILD_DIR)/gtest-all.o

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $(BUILD_DIR)/$@ $(BUILD_DIR)/gtest-all.o $(BUILD_DIR)/gtest_main.o

# Add tests here; note: gtest_main.a defines main, gtest.a don't

sort_selection.o: $(LIB_DIR)/sort/selection.hpp \
				  $(SCENARIOS_DIR)/sort/selection.cpp \
				  $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SCENARIOS_DIR)/sort/selection.cpp -o $(BUILD_DIR)/sort_selection.o

sort_selection : sort_selection.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $(BUILD_DIR)/sort_selection.o $(BUILD_DIR)/gtest_main.a -o $(BUILD_DIR)/$@
