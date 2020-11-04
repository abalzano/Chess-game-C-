CC = g++
CFLAGS ?= -lcurses -Wall -Wextra -Werror -Wvla

LDLIBS?=

INCLUDE_PATH = ./include

TARGET   = all

SRCDIR   = src
OBJDIR   = obj

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(INCLUDE_PATH)/*.hpp)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

$(TARGET): $(OBJECTS)
	$(CC) -o chess $^ $(CFLAGS) $(LDLIBS)
	@echo Done

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) -I$(INCLUDE_PATH)

doxygen:
	doxygen Doxyfile

.PHONY: clean
clean:
	rm -f chess
	rm -rf obj
	rm -rf html
