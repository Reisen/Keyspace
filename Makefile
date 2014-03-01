# Figure out which OS we're compiling for. Link any extra libraries required
# for the specific operating system in question. XCB for example on Linux, and
# -mwindows for windows.
OSTARGET := $(shell uname)
ifeq ($(OSTARGET), Linux)

SYS_LIBS := -lxcb
SYS_MODS := ui/x11

else

SYS_LIBS := -mwindows
SYS_MODS := ui/win32

endif

# Define flags and options.
LDFLAGS  := -L. -Isrc/ -lnettle -lcairo $(SYS_LIBS)
CPPFLAGS :=
CXXFLAGS := $(LDFLAGS) -std=gnu++0x -Wall -Werror -Werror

MODULES  := types ui $(SYS_MODS)
SRC_DIR  := src/ $(addprefix src/, $(MODULES))
SRC_FLS  := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.cpp))
SRC_OBJ  := $(addprefix build/, $(patsubst %.cpp, %.o, $(notdir $(SRC_FLS))))

# Define VPATH for C++ files only.
vpath %.cpp $(SRC_DIR)

# Build main executable
keyspace: $(SRC_OBJ)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SRC_OBJ) -o keyspace

# Build all object files.
build/%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Maintenance rules
.PHONY: clean

clean:
	rm build/* main
