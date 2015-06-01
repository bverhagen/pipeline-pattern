CC=gcc

IDIR =../include
CFLAGS=-O3 -I$(IDIR) -isystem include/catch/include -Wall -Wextra -Werror -Wno-long-long -Wno-variadic-macros -fno-tree-vectorize 
PROFILEFLAGS=-g -fno-omit-frame-pointer -fno-inline-functions -fno-inline-functions-called-once -fno-optimize-sibling-calls
GPROF2DOTSFLAGS= --strip

ELEVENFLAGS=-std=c++11

ODIR=obj
OUTPUTDIR=bin
LDIR =../lib

LIBS=-lstdc++

_DEPS =
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = catchMain.o testPipelinePattern.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_TESTOBJ = testPerformance.o dummy.o
TESTOBJ = $(patsubst %,$(ODIR)/%,$(_TESTOBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: all

all: testNinetyEight testEleven

.PHONY: testNinetyEight
testNinetyEight: $(OUTPUTDIR)/testNinetyEight
.PHONY: testEleven
testEleven: $(OUTPUTDIR)/testEleven

$(OUTPUTDIR)/testNinetyEight: $(OBJ) pipelinePattern.hpp
		$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# Add C++11 flag
$(OUTPUTDIR)/testEleven: CFLAGS+=$(ELEVENFLAGS)
$(OUTPUTDIR)/testEleven: $(OBJ) $(HDEPS) pipelinePattern.hpp
		$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: testPerformance
testPerformance: $(OUTPUTDIR)/testPerformance

$(OUTPUTDIR)/testPerformance: CFLAGS+=$(ELEVENFLAGS) $(PROFILEFLAGS)
$(OUTPUTDIR)/testPerformance: $(TESTOBJ) pipelinePattern.hpp
		$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: testPerformanceGprof
testPerformanceGprof: $(OUTPUTDIR)/testPerformanceGprof

$(OUTPUTDIR)/testPerformanceGprof: CFLAGS+=$(ELEVENFLAGS) $(PROFILEFLAGS) -pg
$(OUTPUTDIR)/testPerformanceGprof: $(TESTOBJ) pipelinePattern.hpp
		$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: runPerformance
runPerformance:
		sudo cpupower frequency-set -g performance
		$(OUTPUTDIR)/testPerformance 1000 0 1 2 3
		sudo cpupower frequency-set -g powersave

.PHONY: runPerformanceGprof
runPerformanceGprof:
		$(OUTPUTDIR)/testPerformanceGprof 1000 0 1 2 3
		gprof $(OUTPUTDIR)/testPerformanceGprof gmon.out | gprof2dot $(GPROF2DOTSFLAGS) | dot -Tpng -o output_gprof.png
		echo "Output image in output_grof.png"

.PHONY: runPerformancePerf
runPerformancePerf:
		sudo cpupower frequency-set -g performance
		perf record -g -F 50100 -- $(OUTPUTDIR)/testPerformance 1000 0 1 2 3
		perf script | gprof2dot -f perf $(GPROF2DOTSFLAGS) | dot -Tpng -o output_perf.png
		sudo cpupower frequency-set -g powersave

.PHONY: runPerformanceCallgrind
$(OUTPUTDIR)/testPerformanceCallgrind: CFLAGS+=$(ELEVENFLAGS) $(PROFILEFLAGS) -pg
runPerformanceCallgrind:
		rm callgrind.out.*
		sudo cpupower frequency-set -g performance
		valgrind --tool=callgrind $(OUTPUTDIR)/testPerformance 1000 0 1 2 3
		sudo cpupower frequency-set -g powersave

.PHONY: clean

clean:
	rm -f $(OUTPUTDIR)/*
	rm -f $(ODIR)/*
