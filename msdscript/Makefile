# Makefile -builds CS6015 MSDscript assignment
#
# To build a target, enter:
# make <target>
#
# Targets:
# clean -deletes all .o, .a, binaries, etc
# test -run all tests
# doc -run doxygen

SRCS = main.cpp cmdline.cpp expr.cpp testExpr.cpp parse.cpp val.cpp env.cpp
OBJS = main.o cmdline.o expr.o testExpr.o parse.o val.o env.o
OBJS1 = exec.o run_msdscript.o
#HEADERS = cmdline.h expr.h catch.h parse.h

CXXFLAGS = --std=c++14

msdscript: $(OBJS)
	$(CXX) $(CXXFLAGS) -o msdscript $(OBJS)

test_msdscript: $(OBJS1)
	$(CXX) $(CXXFLAGS) -o test_msdscript $(OBJS1)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *.d *.out msdscript test_msdscript
	
.PHONY: test
test: msdscript
	./msdscript --test

.PHONY: doc
doc:
	cd documentation && doxygen

.PHONY: showdoc
showdoc:
	cd documentation/html && open index.html

