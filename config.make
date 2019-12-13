# This is a generated file

# comment out to exclude building documentation
DOC := doc


# CRTS prefix
PREFIX = /usr/local/crts


# compiler options
CFLAGS := -g -Wall -Werror
CXXFLAGS := $(CFLAGS)

# Add extra debugging code to the buffering code
# only when the below DEBUG thing is defined
CPPFLAGS := -DBUFFER_DEBUG


# select 1 of the next two lines, effects include/crts/debug.h
DEBUG := \#define DEBUG
#DEBUG := // DEBUG is not defined


# select 1 of the next few lines, effects include/crts/debug.h
#SPEW_LEVEL := \#define SPEW_LEVEL_ERROR
#SPEW_LEVEL := \#define SPEW_LEVEL_WARN
#SPEW_LEVEL := \#define SPEW_LEVEL_NOTICE
#SPEW_LEVEL := \#define SPEW_LEVEL_INFO
SPEW_LEVEL  := \#define SPEW_LEVEL_DEBUG


# A JSON array of objects that list computers and port numbers:
#USRP_HOSTS := [\"\",{\"host\":\"198.82.18.140\",\"port\":\"7000\"}]
USRP_HOSTS := [\"\"]


