#
# Makefile for openmp programs
#
# Author : Kapil Agarwal
#
# Date : 3 Feb 2014
#

FILES = pi matvec gauss sieve

CC = gcc
CFLAGS = -fopenmp
LDFLAGS = -lm
RM = rm -rf

all: $(FILES)

$(FILES) : %:%.o
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS) 

clean:
	$(RM) *.o $(FILES)
