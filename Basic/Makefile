# You may modify Makefile as your like
# but you should keep `make clean` and `make` works correct

PROGRAM = Basic

CXX = g++
CXXFLAGS = -IStanfordCPPLib -fvisibility-inlines-hidden -g

CPP_FILES = $(wildcard *.cpp)
H_FILES = $(wildcard *.h)

LDOPTIONS = -L.
LIB = -lStanfordCPPLib

all: $(PROGRAM) spl.jar score

$(PROGRAM): $(CPP_FILES) $(H_FILES) libStanfordCPPLib.a
	$(CXX) -o $(PROGRAM) $(CXXFLAGS) $(LDOPTIONS) $(CPP_FILES) $(LIB)

score: ../Test/score
	ln -s ../Test/score .

../Test/score:
	(cd ../Test; make)

libStanfordCPPLib.a:
	@rm -f ../libStanfordCPPLib.a
	(cd ../StanfordCPPLib; make all)
	ln -s ../StanfordCPPLib/libStanfordCPPLib.a .

spl.jar:
	ln -s ../StanfordCPPLib/spl.jar .

tidy:
	rm -f ,* .,* *~ core a.out *.err

clean scratch: tidy
	rm -f *.o *.a $(PROGRAM) spl.jar score
