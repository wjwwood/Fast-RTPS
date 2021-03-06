BASEDIR=.
OUTDIR= $(BASEDIR)/output
CC=gcc
CPP=g++
LN=g++
AR=ar
CP=cp

COMMON_CFLAGS= -c -Wall -D__LITTLE_ENDIAN__

## CHOOSE HERE BETWEEN 32 and 64 bit architecture

##32 BIT ARCH:
#COMMON_CFLAGS+= -m32 -fpic
#LDFLAGS=-m32
#EPROSIMA_TARGET=i86Linux2.6gcc

#64BIT ARCH:
COMMON_CFLAGS+= -m64 -fpic
LDFLAGS=-m64
EPROSIMA_TARGET=x64Linux2.6gcc

CFLAGS = $(COMMON_CFLAGS) -O2


THROUGHPUTTEST_OUTDIR= $(OUTDIR)/ThroughputTest
THROUGHPUTTEST_OUTDIR_RELEASE = $(THROUGHPUTTEST_OUTDIR)/release

THROUGHPUTTEST_SED_OUTPUT_DIR_RELEASE= $(subst /,\\/,$(THROUGHPUTTEST_OUTDIR_RELEASE))

THROUGHPUTTEST_CFLAGS = $(CFLAGS) -std=c++0x 


THROUGHPUTTEST_TARGET= $(BASEDIR)/bin/$(EPROSIMA_TARGET)/ThroughputTest

THROUGHPUTTEST_INCLUDE_DIRS= $(INCLUDE_DIRS) -I$(BASEDIR)/../../../include \
                         -I$(BASEDIR)/../../../thirdparty/eprosima-common-code

THROUGHPUTTEST_LIBS_RELEASE=  -L$(BASEDIR)/../../../lib/$(EPROSIMA_TARGET)  -lfastrtps -lboost_system -lboost_thread -lboost_date_time

THROUGHPUTTEST_SRC_CFILES=

THROUGHPUTTEST_SRC_CPPFILES= $(BASEDIR)/../../useTests/ThroughputTypes.cpp \
								$(BASEDIR)/../../ThroughputPublisher.cpp \
								$(BASEDIR)/../../ThroughputSubscriber.cpp \
								$(BASEDIR)/../../main_ThroughputTest.cpp 
								

# Project sources are copied to the current directory
THROUGHPUTTEST_SRCS= $(THROUGHPUTTEST_SRC_CFILES) $(THROUGHPUTTEST_SRC_CPPFILES)

# Source directories
THROUGHPUTTEST_SOURCES_DIRS_AUX= $(foreach srcdir, $(dir $(THROUGHPUTTEST_SRCS)), $(srcdir))
THROUGHPUTTEST_SOURCES_DIRS= $(shell echo $(THROUGHPUTTEST_SOURCES_DIRS_AUX) | tr " " "\n" | sort | uniq | tr "\n" " ")


THROUGHPUTTEST_OBJS_RELEASE = $(foreach obj,$(notdir $(addsuffix .o, $(THROUGHPUTTEST_SRCS))), $(THROUGHPUTTEST_OUTDIR_RELEASE)/$(obj))
THROUGHPUTTEST_DEPS_RELEASE = $(foreach dep,$(notdir $(addsuffix .d, $(THROUGHPUTTEST_SRCS))), $(THROUGHPUTTEST_OUTDIR_RELEASE)/$(dep))

OBJS+=  $(THROUGHPUTTEST_OBJS_RELEASE)
DEPS+=  $(THROUGHPUTTEST_DEPS_RELEASE)

.PHONY: ThroughputTest checkThroughputTestDirectories

ThroughputTest: checkThroughputTestDirectories  $(THROUGHPUTTEST_TARGET)

checkThroughputTestDirectories:
	@mkdir -p $(OUTDIR)
	@mkdir -p $(THROUGHPUTTEST_OUTDIR)
	@mkdir -p $(THROUGHPUTTEST_OUTDIR_RELEASE)
	@mkdir -p $(BASEDIR)/bin
	@mkdir -p $(BASEDIR)/bin/$(EPROSIMA_TARGET)



$(THROUGHPUTTEST_TARGET): $(THROUGHPUTTEST_OBJS_RELEASE)
	$(LN) $(LDFLAGS) -o $(THROUGHPUTTEST_TARGET) $(THROUGHPUTTEST_OBJS_RELEASE) $(LIBRARY_PATH) $(THROUGHPUTTEST_LIBS_RELEASE) $(THROUGHPUTTEST_STATIC_LIBS)

vpath %.c $(THROUGHPUTTEST_SOURCES_DIRS)
vpath %.cpp $(THROUGHPUTTEST_SOURCES_DIRS)



$(THROUGHPUTTEST_OUTDIR_RELEASE)/%.o:%.c
	@echo Calculating dependencies \(RELEASE mode\) $<
	@$(CC) $(THROUGHPUTTEST_CFLAGS) -MM $(THROUGHPUTTEST_CFLAGS) $(THROUGHPUTTEST_INCLUDE_DIRS) $< | sed "s/^.*:/$(THROUGHPUTTEST_SED_OUTPUT_DIR_RELEASE)\/&/g" > $(@:%.o=%.d)
	@echo Compiling \(RELEASE mode\) $<
	@$(CC) $(THROUGHPUTTEST_CFLAGS) $(THROUGHPUTTEST_INCLUDE_DIRS) $< -o $@



$(THROUGHPUTTEST_OUTDIR_RELEASE)/%.c.o:%.c
	@echo Calculating dependencies \(RELEASE mode\) $<
	@$(CC) $(THROUGHPUTTEST_CFLAGS) -MM $(THROUGHPUTTEST_CFLAGS) $(THROUGHPUTTEST_INCLUDE_DIRS) $< | sed "s/^.*:/$(THROUGHPUTTEST_SED_OUTPUT_DIR_RELEASE)\/&/g" > $(@:%.o=%.d)
	@echo Compiling \(RELEASE mode\) $<
	@$(CC) $(THROUGHPUTTEST_CFLAGS) $(THROUGHPUTTEST_INCLUDE_DIRS) $< -o $@



$(THROUGHPUTTEST_OUTDIR_RELEASE)/%.cpp.o:%.cpp
	@echo Calculating dependencies \(RELEASE mode\) $<
	@$(CPP) $(THROUGHPUTTEST_CFLAGS) -MM $(THROUGHPUTTEST_CFLAGS) $(THROUGHPUTTEST_INCLUDE_DIRS) $< | sed "s/^.*:/$(THROUGHPUTTEST_SED_OUTPUT_DIR_RELEASE)\/&/g" > $(@:%.o=%.d)
	@echo Compiling \(RELEASE mode\) $<
	@$(CPP) $(THROUGHPUTTEST_CFLAGS) $(THROUGHPUTTEST_INCLUDE_DIRS) $< -o $@

.PHONY: all

all: THROUGHPUT

clean:
	@rm -f $(OBJS)
	@rm -f $(DEPS)

ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS)
endif
