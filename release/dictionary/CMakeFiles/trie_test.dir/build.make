# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/students/inf/k/kc359073/IPP/SPELLCHECK/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/students/inf/k/kc359073/IPP/SPELLCHECK/release

# Include any dependencies generated for this target.
include dictionary/CMakeFiles/trie_test.dir/depend.make

# Include the progress variables for this target.
include dictionary/CMakeFiles/trie_test.dir/progress.make

# Include the compile flags for this target's objects.
include dictionary/CMakeFiles/trie_test.dir/flags.make

dictionary/CMakeFiles/trie_test.dir/trie.c.o: dictionary/CMakeFiles/trie_test.dir/flags.make
dictionary/CMakeFiles/trie_test.dir/trie.c.o: /home/students/inf/k/kc359073/IPP/SPELLCHECK/src/dictionary/trie.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/students/inf/k/kc359073/IPP/SPELLCHECK/release/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object dictionary/CMakeFiles/trie_test.dir/trie.c.o"
	cd /home/students/inf/k/kc359073/IPP/SPELLCHECK/release/dictionary && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/trie_test.dir/trie.c.o   -c /home/students/inf/k/kc359073/IPP/SPELLCHECK/src/dictionary/trie.c

dictionary/CMakeFiles/trie_test.dir/trie.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/trie_test.dir/trie.c.i"
	cd /home/students/inf/k/kc359073/IPP/SPELLCHECK/release/dictionary && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/students/inf/k/kc359073/IPP/SPELLCHECK/src/dictionary/trie.c > CMakeFiles/trie_test.dir/trie.c.i

dictionary/CMakeFiles/trie_test.dir/trie.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/trie_test.dir/trie.c.s"
	cd /home/students/inf/k/kc359073/IPP/SPELLCHECK/release/dictionary && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/students/inf/k/kc359073/IPP/SPELLCHECK/src/dictionary/trie.c -o CMakeFiles/trie_test.dir/trie.c.s

dictionary/CMakeFiles/trie_test.dir/trie.c.o.requires:
.PHONY : dictionary/CMakeFiles/trie_test.dir/trie.c.o.requires

dictionary/CMakeFiles/trie_test.dir/trie.c.o.provides: dictionary/CMakeFiles/trie_test.dir/trie.c.o.requires
	$(MAKE) -f dictionary/CMakeFiles/trie_test.dir/build.make dictionary/CMakeFiles/trie_test.dir/trie.c.o.provides.build
.PHONY : dictionary/CMakeFiles/trie_test.dir/trie.c.o.provides

dictionary/CMakeFiles/trie_test.dir/trie.c.o.provides.build: dictionary/CMakeFiles/trie_test.dir/trie.c.o

dictionary/CMakeFiles/trie_test.dir/trie_test.c.o: dictionary/CMakeFiles/trie_test.dir/flags.make
dictionary/CMakeFiles/trie_test.dir/trie_test.c.o: /home/students/inf/k/kc359073/IPP/SPELLCHECK/src/dictionary/trie_test.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/students/inf/k/kc359073/IPP/SPELLCHECK/release/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object dictionary/CMakeFiles/trie_test.dir/trie_test.c.o"
	cd /home/students/inf/k/kc359073/IPP/SPELLCHECK/release/dictionary && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/trie_test.dir/trie_test.c.o   -c /home/students/inf/k/kc359073/IPP/SPELLCHECK/src/dictionary/trie_test.c

dictionary/CMakeFiles/trie_test.dir/trie_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/trie_test.dir/trie_test.c.i"
	cd /home/students/inf/k/kc359073/IPP/SPELLCHECK/release/dictionary && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/students/inf/k/kc359073/IPP/SPELLCHECK/src/dictionary/trie_test.c > CMakeFiles/trie_test.dir/trie_test.c.i

dictionary/CMakeFiles/trie_test.dir/trie_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/trie_test.dir/trie_test.c.s"
	cd /home/students/inf/k/kc359073/IPP/SPELLCHECK/release/dictionary && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/students/inf/k/kc359073/IPP/SPELLCHECK/src/dictionary/trie_test.c -o CMakeFiles/trie_test.dir/trie_test.c.s

dictionary/CMakeFiles/trie_test.dir/trie_test.c.o.requires:
.PHONY : dictionary/CMakeFiles/trie_test.dir/trie_test.c.o.requires

dictionary/CMakeFiles/trie_test.dir/trie_test.c.o.provides: dictionary/CMakeFiles/trie_test.dir/trie_test.c.o.requires
	$(MAKE) -f dictionary/CMakeFiles/trie_test.dir/build.make dictionary/CMakeFiles/trie_test.dir/trie_test.c.o.provides.build
.PHONY : dictionary/CMakeFiles/trie_test.dir/trie_test.c.o.provides

dictionary/CMakeFiles/trie_test.dir/trie_test.c.o.provides.build: dictionary/CMakeFiles/trie_test.dir/trie_test.c.o

# Object files for target trie_test
trie_test_OBJECTS = \
"CMakeFiles/trie_test.dir/trie.c.o" \
"CMakeFiles/trie_test.dir/trie_test.c.o"

# External object files for target trie_test
trie_test_EXTERNAL_OBJECTS =

dictionary/trie_test: dictionary/CMakeFiles/trie_test.dir/trie.c.o
dictionary/trie_test: dictionary/CMakeFiles/trie_test.dir/trie_test.c.o
dictionary/trie_test: dictionary/CMakeFiles/trie_test.dir/build.make
dictionary/trie_test: /usr/local/lib/libcmocka.so
dictionary/trie_test: dictionary/CMakeFiles/trie_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable trie_test"
	cd /home/students/inf/k/kc359073/IPP/SPELLCHECK/release/dictionary && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/trie_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
dictionary/CMakeFiles/trie_test.dir/build: dictionary/trie_test
.PHONY : dictionary/CMakeFiles/trie_test.dir/build

dictionary/CMakeFiles/trie_test.dir/requires: dictionary/CMakeFiles/trie_test.dir/trie.c.o.requires
dictionary/CMakeFiles/trie_test.dir/requires: dictionary/CMakeFiles/trie_test.dir/trie_test.c.o.requires
.PHONY : dictionary/CMakeFiles/trie_test.dir/requires

dictionary/CMakeFiles/trie_test.dir/clean:
	cd /home/students/inf/k/kc359073/IPP/SPELLCHECK/release/dictionary && $(CMAKE_COMMAND) -P CMakeFiles/trie_test.dir/cmake_clean.cmake
.PHONY : dictionary/CMakeFiles/trie_test.dir/clean

dictionary/CMakeFiles/trie_test.dir/depend:
	cd /home/students/inf/k/kc359073/IPP/SPELLCHECK/release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/students/inf/k/kc359073/IPP/SPELLCHECK/src /home/students/inf/k/kc359073/IPP/SPELLCHECK/src/dictionary /home/students/inf/k/kc359073/IPP/SPELLCHECK/release /home/students/inf/k/kc359073/IPP/SPELLCHECK/release/dictionary /home/students/inf/k/kc359073/IPP/SPELLCHECK/release/dictionary/CMakeFiles/trie_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : dictionary/CMakeFiles/trie_test.dir/depend

