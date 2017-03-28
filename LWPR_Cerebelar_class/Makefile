#****************************************************************************

# DEBUG can be set to YES to include debugging info, or NO otherwise
DEBUG          := NO

# PROFILE can be set to YES to include profiling info, or NO otherwise
PROFILE        := NO

# BACKUP can be set to YES in order to generate a backup each time
# a compilation is done, or NO otherwise.
BACKUP		   := NO

#****************************************************************************
DEBUG_CXXFLAGS   := -Wall -Wno-format -g -DDEBUG
RELEASE_CXXFLAGS := -Wall -Wno-unknown-pragmas -Wno-format -O3

DEBUG_LDFLAGS    := -g
RELEASE_LDFLAGS  :=

ifeq (YES, ${DEBUG})
   CXXFLAGS     := ${DEBUG_CXXFLAGS}
   LDFLAGS      := ${DEBUG_LDFLAGS}
   TYPE 		= Debug
else
   CXXFLAGS     := ${RELEASE_CXXFLAGS}
   LDFLAGS      := ${RELEASE_LDFLAGS}
   TYPE 		= Release
endif

ifeq (YES, ${PROFILE})
   CXXFLAGS := ${CXXFLAGS} -pg -O3
   LDFLAGS  := ${LDFLAGS} -pg
endif
#****************************************************************************
# Preprocessor directives
#**************************************************************************** 
DEFS = 
#DEFS += -DRAPIDXML_NO_EXCEPTIONS
#DEFS += -DSAVE_ROTVEL_PROFILE
#DEFS += -DSAVE_TRANSVEL_PROFILE

CXX = g++
CXXFLAGS :=	${CXXFLAGS} ${DEFS}

LIBS = -pthread -llwpr

#****************************************************************************
# Directories
#****************************************************************************
DIRECTORIES = src
BACKUP_DIR = /home/mauricio/Dropbox/C_Projects/RS485/

# Add directories to the include and library paths
INCPATH = $(DIRECTORIES)
LIBPATH = 

# Which files to add to backups, apart from the source code
EXTRA_FILES = Makefile

# Where to store object files.
STORE = $(TYPE)
# Makes a list of the source (.cpp) files.
SOURCE := $(foreach DIR,$(DIRECTORIES),$(wildcard $(DIR)/*.cpp))
# List of header files.
HEADERS := $(foreach DIR,$(DIRECTORIES),$(wildcard $(DIR)/*.h))
HEADERS += $(foreach DIR,$(DIRECTORIES),$(wildcard $(DIR)/*.hpp))
# Makes a list of the object files that will have to be created.
OBJECTS := $(addprefix $(STORE)/, $(SOURCE:.cpp=.o))
DFILES := $(addprefix $(STORE)/,$(SOURCE:.cpp=.d))

TARGET =	lwpr_cerebelar_example

# Specify phony rules. These are rules that are not real files.
.PHONY: clean backup dirs

$(TARGET): dirs $(OBJECTS)
		@echo 'Building target: $@'
		@echo 'Invoking: GCC C++ Linker'
		$(CXX) -o $(TARGET) $(OBJECTS) $(LIBS)
		@echo 'Finished building target: $@'
		@echo ' '

$(STORE)/%.o: %.cpp
	@echo 'Building partial codes: $^'
	$(CXX) -Wp,-MMD,$(STORE)/$*.dd $(CXXFLAGS) $(foreach INC,$(INCPATH),-I$(INC)) -c $^ -o $@ 
	@sed -e '1s/^\(.*\)$$/$(subst /,\/,$(dir $@))\1/' $(STORE)/$*.dd > $(STORE)/$*.d
	@echo ' '
	
# Empty rule to prevent problems when a header is deleted.
%.hpp: ;

all:	backup $(TARGET)

# Cleans up the objects, .d files and executables.
clean:
	@echo 'Cleaning all!!!'
	@-rm -f $(foreach DIR,$(DIRECTORIES),$(STORE)/$(DIR)/*.d $(STORE)/$(DIR)/*.dd $(STORE)/$(DIR)/*.o)
	@-rm -f $(TARGET)
	@echo ' '

# Backup the source files.
backup:
ifeq (YES, ${BACKUP})
	@echo 'Doing backup!'
	@-if [ ! -e $(BACKUP_DIR) ]; then mkdir $(BACKUP_DIR); fi;
	@-rsync --progress -auvh $(SOURCE) $(HEADERS) $(EXTRA_FILES) $(BACKUP_DIR)
	@echo ' '
#	@zip $(BACKUP_DIR)/backup_`date +%d-%m-%y_%H.%M`.zip $(SOURCE) $(HEADERS) $(EXTRA_FILES)
endif

# Create necessary directories
dirs:
	@-if [ ! -e $(STORE) ]; then mkdir $(STORE); fi;
	@-$(foreach DIR,$(DIRECTORIES), if [ ! -e $(STORE)/$(DIR) ]; then mkdir $(STORE)/$(DIR); fi; )
