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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/karras/Studia/IPP/Spell_Checker/SPELLCHECKII/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/karras/Studia/IPP/Spell_Checker/SPELLCHECKII/debug

# Include any dependencies generated for this target.
include dict-editor/CMakeFiles/dict-editor.dir/depend.make

# Include the progress variables for this target.
include dict-editor/CMakeFiles/dict-editor.dir/progress.make

# Include the compile flags for this target's objects.
include dict-editor/CMakeFiles/dict-editor.dir/flags.make

dict-editor/CMakeFiles/dict-editor.dir/dict-editor.c.o: dict-editor/CMakeFiles/dict-editor.dir/flags.make
dict-editor/CMakeFiles/dict-editor.dir/dict-editor.c.o: /home/karras/Studia/IPP/Spell_Checker/SPELLCHECKII/src/dict-editor/dict-editor.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/karras/Studia/IPP/Spell_Checker/SPELLCHECKII/debug/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object dict-editor/CMakeFiles/dict-editor.dir/dict-editor.c.o"
	cd /home/karras/Studia/IPP/Spell_Checker/SPELLCHECKII/debug/dict-editor && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/dict-editor.dir/dict-editor.c.o   -c /home/karras/Studia/IPP/Spell_Checker/SPELLCHECKII/src/dict-editor/dict-editor.c

dict-editor/CMakeFiles/dict-editor.dir/dict-editor.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dict-editor.dir/dict-editor.c.i"
	cd /home/karras/Studia/IPP/Spell_Checker/SPELLCHECKII/debug/dict-editor && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/karras/Studia/IPP/Spell_Checker/SPELLCHECKII/src/dict-editor/dict-editor.c > CMakeFiles/dict-editor.dir/dict-editor.c.i

dict-editor/CMakeFiles/dict-editor.dir/dict-editor.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dict-editor.dir/dict-editor.c.s"
	cd /home/karras/Studia/IPP/Spell_Checker/SPELLCHECKII/debug/dict-editor && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/karras/Studia/IPP/Spell_Checker/SPELLCHECKII/src/dict-editor/dict-editor.c -o CMakeFiles/dict-editor.dir/dict-editor.c.s

dict-editor/CMakeFiles/dict-editor.dir/dict-editor.c.o.requires:
.PHONY : dict-editor/CMakeFiles/dict-editor.dir/dict-editor.c.o.requires

dict-editor/CMakeFiles/dict-editor.dir/dict-editor.c.o.provides: dict-editor/CMakeFiles/dict-editor.dir/dict-editor.c.o.requires
	$(MAKE) -f dict-editor/CMakeFiles/dict-editor.dir/build.make dict-editor/CMakeFiles/dict-editor.dir/dict-editor.c.o.provides.build
.PHONY : dict-editor/CMakeFiles/dict-editor.dir/dict-editor.c.o.provides

dict-editor/CMakeFiles/dict-editor.dir/dict-editor.c.o.provides.build: dict-editor/CMakeFiles/dict-editor.dir/dict-editor.c.o

# Object files for target dict-editor
dict__editor_OBJECTS = \
"CMakeFiles/dict-editor.dir/dict-editor.c.o"

# External object files for target dict-editor
dict__editor_EXTERNAL_OBJECTS =

dict-editor/dict-editor: dict-editor/CMakeFiles/dict-editor.dir/dict-editor.c.o
dict-editor/dict-editor: dict-editor/CMakeFiles/dict-editor.dir/build.make
dict-editor/dict-editor: dictionary/libdictionary.a
dict-editor/dict-editor: dict-editor/CMakeFiles/dict-editor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable dict-editor"
	cd /home/karras/Studia/IPP/Spell_Checker/SPELLCHECKII/debug/dict-editor && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dict-editor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
dict-editor/CMakeFiles/dict-editor.dir/build: dict-editor/dict-editor
.PHONY : dict-editor/CMakeFiles/dict-editor.dir/build

dict-editor/CMakeFiles/dict-editor.dir/requires: dict-editor/CMakeFiles/dict-editor.dir/dict-editor.c.o.requires
.PHONY : dict-editor/CMakeFiles/dict-editor.dir/requires

dict-editor/CMakeFiles/dict-editor.dir/clean:
	cd /home/karras/Studia/IPP/Spell_Checker/SPELLCHECKII/debug/dict-editor && $(CMAKE_COMMAND) -P CMakeFiles/dict-editor.dir/cmake_clean.cmake
.PHONY : dict-editor/CMakeFiles/dict-editor.dir/clean

dict-editor/CMakeFiles/dict-editor.dir/depend:
	cd /home/karras/Studia/IPP/Spell_Checker/SPELLCHECKII/debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/karras/Studia/IPP/Spell_Checker/SPELLCHECKII/src /home/karras/Studia/IPP/Spell_Checker/SPELLCHECKII/src/dict-editor /home/karras/Studia/IPP/Spell_Checker/SPELLCHECKII/debug /home/karras/Studia/IPP/Spell_Checker/SPELLCHECKII/debug/dict-editor /home/karras/Studia/IPP/Spell_Checker/SPELLCHECKII/debug/dict-editor/CMakeFiles/dict-editor.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : dict-editor/CMakeFiles/dict-editor.dir/depend

