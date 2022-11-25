CXXFLAGS=-g -std=c++11 -Wall -Wimplicit-fallthrough

OFILES=util.o console.o ParseError.o RuleNode.o NumericCondition.o PatternFinder.o PatternCompare.o PatternCount.o parseRule.o MultiRule.o StringGenerator.o Game.o linenoise/linenoise.o

app: $(OFILES) main.o
	g++ -std=c++11 main.o $(OFILES) -lreadline -o app


# List of classes and their dependencies

main.o: main.cpp MultiRule.h PatternFinder.h RuleNode.h parseRule.h console.h util.h

NumericCondition.o: NumericCondition.cpp NumericCondition.h RuleNode.h

PatternFinder.o: PatternFinder.cpp PatternFinder.h RuleNode.h util.h

PatternCount.o: PatternCount.cpp PatternCount.h PatternFinder.h NumericCondition.h RuleNode.h

MultiRule.o: MultiRule.cpp MultiRule.h PatternFinder.h PatternCount.h

Game.o: Game.cpp Game.h MultiRule.h PatternFinder.h StringGenerator.h


# For line editing library
linenoise/linenoise.o: linenoise/linenoise.c linenoise/linenoise.h

# we want AGGRESSIVE optimization on this one
StringGenerator.o: StringGenerator.cpp StringGenerator.h
	g++ $(CXXFLAGS) -O2 -c StringGenerator.cpp

run: app
	./app

clean:
	rm -f *.o
	rm -f app

valgrind: app
	valgrind ./app

valgrind-full: app
	valgrind --leak-check=full ./app

testProg: $(OFILES) Test.o doTest.o
	g++ -std=c++11 Test.o doTest.o $(OFILES) -lreadline -o testProg

test: testProg
	./testProg
