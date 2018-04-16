# --- DEVELOPER AREA START ---

# We use nested namespaces for convenience
CXXFLAGS += -g -std=c++1z -Wall -Wextra -lpthread

# Include project-specific files
CPPFLAGS += -I ../include

# Where to find library code
LIB_DIR = ../include/algs

# Where to find test scenarios
SCENARIOS_DIR = scenarios

# Where to put intermediate files and test executables
BUILD_DIR = build

# --- DEVELOPER AREA END ---

# --- GOOGLE TEST AREA START; DO NOT TOUCH ---

GTEST_DIR = ../submodules/googletest/googletest

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

CPPFLAGS += -isystem $(GTEST_DIR)/include

# note: 2>&1 - pipe stderr to stdout, >/dev/null - pipe stdout to /dev/null
gtest.a :
	@$(MAKE) -sf Makefile.googletest gtest.a 2>&1 >/dev/null

gtest_main.a :
	@$(MAKE) -sf Makefile.googletest gtest_main.a 2>&1 >/dev/null

# --- GOOGLE TEST AREA END; DO NOT TOUCH ---

# --- DEVELOPER AREA START (add tests here) ---

sort.o : $(SCENARIOS_DIR)/sort.cpp \
		 $(GTEST_HEADERS)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
		-c $(filter %.cpp,$^) \
		-o $(BUILD_DIR)/$@

tree.o : $(SCENARIOS_DIR)/tree.cpp \
		 $(GTEST_HEADERS)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) \
		-c $(filter %.cpp,$^) \
		-o $(BUILD_DIR)/$@

# note: gtest_main.a defines main, gtest.a don't
test : sort.o tree.o gtest_main.a
	@cd $(BUILD_DIR) && \
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^

run : test
	@./build/test

clean :
	@rm -rf $(BUILD_DIR) && mkdir -p $(BUILD_DIR) && touch $(BUILD_DIR)/.gitignore

all : run

