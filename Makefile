# Makefile for the smash program
CC = gcc
CPP = g++
CFLAGS =‫‪-std=c++11‬‬ ‫‪-Wall‬‬ ‫‪-Werror‬‬ ‫‪-pedantic-errors‬‬ ‫‪-DNDEBUG‬‬
CCLINK = $(CPP)
OBJS = smash.o commands.o signals.o
TARGET = smash
RM = rm -f
# Creating the  executable
smash: $(OBJS)
	$(CCLINK) -o smash $(OBJS)
# Creating the object files
commands.o: commands.cpp commands.h



smash.o: smash.cpp commands.h



signals.o: signals.cpp signals.h

# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

