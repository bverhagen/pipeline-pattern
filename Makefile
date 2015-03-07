CC=gcc

IDIR =../include
CFLAGS=-I$(IDIR) -isystem include/catch/include
ELEVENFLAGS=-std=c++11

ODIR=obj
OUTPUTDIR=bin
LDIR =../lib

LIBS=-lstdc++

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = catchMain.o testPipelinePattern.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: all

all: testNinetyEight testEleven

.PHONY: testNinetyEight
testNinetyEight: $(OUTPUTDIR)/testNinetyEight 

.PHONY: testEleven
testEleven: $(OUTPUTDIR)/testEleven

$(OUTPUTDIR)/testNinetyEight: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(OUTPUTDIR)/testEleven: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS) $(ELEVENFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f $(OUTPUTDIR)/*
	rm -f $(ODIR)/*
