# --- DEVELOPER AREA START ---

# We use nested namespaces for convenience
CXXFLAGS += -std=c++1z

# Include project-specific files
CPPFLAGS += -I ../include

# Where to find library code
LIB_DIR = ../include/algs

# Where to find test scenarios
SCENARIOS_DIR = scenarios

# Where to put intermediate files and test executables
BUILD_DIR = build

# Add test targets here
TESTS = sort_selection sort_insertion

# --- DEVELOPER AREA END ---

# --- GOOGLE TEST AREA START; DO NOT TOUCH ---

# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR = ../submodules/googletest/googletest

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# House-keeping build targets.

all : $(TESTS)

clean :
	rm -rf $(BUILD_DIR) && mkdir -p $(BUILD_DIR) && touch $(BUILD_DIR)/.gitignore

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

# --- GOOGLE TEST AREA END; DO NOT TOUCH ---

# --- DEVELOPER AREA START ---
# Add tests here
# note: gtest_main.a defines main, gtest.a don't
# note: each target depends on library header (and possibly cpp file),
#       scenario cpp file and $(GTEST_HEADERS)

sort_selection.o: $(LIB_DIR)/sort/selection.hpp \
				  $(SCENARIOS_DIR)/sort/selection.cpp \
				  $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
		-c $(SCENARIOS_DIR)/sort/selection.cpp \
		-o $(BUILD_DIR)/sort_selection.o

sort_selection : sort_selection.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread \
		$(BUILD_DIR)/sort_selection.o $(BUILD_DIR)/gtest_main.a \
		-o $(BUILD_DIR)/$@

sort_insertion.o: $(LIB_DIR)/sort/insertion.hpp \
				  $(SCENARIOS_DIR)/sort/insertion.cpp \
				  $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
		-c $(SCENARIOS_DIR)/sort/insertion.cpp \
		-o $(BUILD_DIR)/sort_insertion.o

sort_insertion : sort_insertion.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread \
		$(BUILD_DIR)/sort_insertion.o $(BUILD_DIR)/gtest_main.a \
		-o $(BUILD_DIR)/$@

sort_shell.o: $(LIB_DIR)/sort/shell.hpp \
		   	  $(SCENARIOS_DIR)/sort/shell.cpp \
		   	  $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
		-c $(SCENARIOS_DIR)/sort/shell.cpp \
		-o $(BUILD_DIR)/sort_shell.o

sort_shell : sort_shell.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread \
		$(BUILD_DIR)/sort_shell.o $(BUILD_DIR)/gtest_main.a \
		-o $(BUILD_DIR)/$@
