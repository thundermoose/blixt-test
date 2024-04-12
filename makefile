.SILENT:
-include makefile.local
ifndef compiler
compiler := g++ -std=c++20 -Wall -Werror -c
endif

ifndef linker
linker := g++ -std=c++20 -Wall -Werror
endif

ifndef install_path
install_path := /usr/local
endif

compiler_flags := -I./src -ggdb -fPIC
linker_flags := -lm

source_path := src
mode_path := .tmp
object_path := .tmp/objects
dependencies_path := .tmp/dependencies

all_source_files := $(shell find $(source_path) -regex [^\#]*\\.cpp$)
header_files := $(shell find $(source_path) -regex [^\#]*\\.hh$)
program_source_files := $(filter $(source_path)/programs/%.cpp,$(all_source_files))
function_source_files := $(filter-out $(source_path)/programs/%.cpp $(source_path)/tests/%.cpp,$(all_source_files))
function_object_files := $(function_source_files:$(source_path)/%.cpp=$(object_path)/%.o)
dependencies_files := $(all_source_files:$(source_path)/%.cpp=$(dependencies_path)/%.d)
program_names := $(program_source_files:$(source_path)/programs/%.cpp=./release/%.x)
test_program_names := $(program_source_files:$(source_path)/programs/%.cpp=./test/%.x)
shared_object := libblixttest.so

all: compiler_flags+=-DNDEBUG -DNLOGING
all: $(mode_path)/release.mode $(header_files) documentation
	make release/lib/$(shared_object)
	make release/include/test.hh

install:
	mkdir -p $(install_path)/lib
	mkdir -p $(install_path)/include/blixttest
	mkdir -p $(install_path)/share/man/man3
	cp release/lib/$(shared_object) $(install_path)/lib/
	cp release/include/*.hh $(install_path)/include/blixttest/
	

uninstall:
	rm $(install_path)/lib/$(shared_object)
	rm -r $(install_path)/include/blixttest

test: $(mode_path)/test.mode
	make $(test_program_names)

documentation: doxygen.conf $(all_source_files)
	doxygen $<

.PRECIOUS: $(dependencies_files) $(function_object_files) $(header_files)

clear:
	echo "Cleaning up"
	rm -rf ./.tmp/
	rm -rf ./release/
	rm -rf ./test/

$(mode_path)/%.mode: 
	echo "Generating: $@"
	rm -rf ./.tmp/
	mkdir -p $(@D)
	echo "Intentionaly empty" > $@

release/%.x: compiler_flags+=-O3 -DNDEBUG -DNLOGING
release/%.x: $(object_path)/programs/%.o $(function_object_files)
	echo "Linking: $@"
	mkdir -p $(@D)
	$(linker) -o $(@) $^ $(linker_flags)

release/lib/%.so: compiler_flags+=-O3
release/lib/%.so: $(function_object_files)
	echo "Linking shared object: $@"
	mkdir -p $(@D)
	$(linker) -shared -o $@ $^

release/include/test.hh: $(source_path)/test/test.hh
	echo "Coping header file: $@"
	mkdir -p $(@D)
	cp $< $@

test/%.x: linker_flags+=$(thundertester_linker_flags)
test/%.x: compiler_flags+=-ggdb $(thundertester_compiler_flags) -DTEST_DATA="\"./test_data/\""
test/%.x: $(object_path)/programs/%.o $(function_object_files)
	echo "Linking test: $@"
	mkdir -p $(@D)
	$(linker) -o $(@) $^ $(linker_flags)

-include $(dependencies_files)
$(object_path)/%.o: $(source_path)/%.cpp $(dependencies_path)/%.d
	echo "Compiling: $@"
	mkdir -p $(@D)
	$(compiler) -o $@ $< $(compiler_flags)

$(dependencies_path)/%.d: $(source_path)/%.cpp
	echo "Generating: $@"
	mkdir -p $(@D)
	$(compiler) -o /dev/null -M -MF $@ -MT $(@:$(dependencies_path)/%.d=$(object_path)/%.o) $< $(compiler_flags)

