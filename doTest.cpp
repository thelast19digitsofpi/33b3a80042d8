// doTest.cpp
//
// not sure the best way to do this


#include "Test.h"

#include "NumericCondition.h"
#include "PatternFinder.h"

void testNumericCondition();
void testPatternFinder();

int main() {
	testNumericCondition();
	testPatternFinder();
}

void testPatternFinder() {
	Test tLetter("Single Letter E");
	PatternFinder pfLetter("E");
	
	tLetter.assertEqual(pfLetter.count("EEE"), 3, "3 E's");
	tLetter.assertEqual(pfLetter.count("III"), 0, "3 I's, no E's");
	tLetter.assertEqual(pfLetter.count("AEIOUOIEA"), 2, "2 E's and many other letters");
	tLetter.printResults();
	
	Test tLonger("Multiple letter pattern OIA");
	PatternFinder pfLonger("OIA");
	
	tLonger.assertEqual(pfLonger.count("E"), 0, "too short");
	tLonger.assertEqual(pfLonger.count("OIA"), 1, "exact match");
	tLonger.assertEqual(pfLonger.count("OIAOIAOIA"), 3, "three times");
	tLonger.assertEqual(pfLonger.count("OIOIAOIE"), 1, "false starts");
	tLonger.assertEqual(pfLonger.count("OUIUA"), 0, "stuff in the middle");
	tLonger.printResults();
	
	Test tDouble("Wildcards: Double Letters");
	PatternFinder pfDouble("44");
	
	tDouble.assertEqual(pfDouble.count("AA"), 1, "simple doubled letter");
	tDouble.assertEqual(pfDouble.count("AAA"), 2, "3 in a row is two pairs");
	tDouble.assertEqual(pfDouble.count("AAAAAAA"), 6, "7 A's = 6 pairs");
	tDouble.assertEqual(pfDouble.count("AAEEIIOU"), 3, "3 pairs and 2 singletons");
	tDouble.assertEqual(pfDouble.count("EAAEAAEAAE"), 3, "3 pairs separated by the same letter");
	tDouble.assertEqual(pfDouble.count("AEAEAEAEA"), 0, "alternation");
	tDouble.printResults();
	
	Test tWild2("Wildcards: 2 types");
	PatternFinder pfWild2("1717");
	
	tWild2.assertEqual(pfWild2.count("AEAE"), 1, "simple match");
	tWild2.assertEqual(pfWild2.count("AEAEAE"), 3, "overlap allowed"); // 3, not 2, don't forget EAEA!
	tWild2.assertEqual(pfWild2.count("AEAI"), 0, "not complete");
	tWild2.assertEqual(pfWild2.count("AAAAAAA"), 0, "reusing letters not allowed");
	tWild2.printResults();
	
	Test tSeries1("Series of Letters: A*E");
	PatternFinder pfSeries1("A*E");
	tSeries1.assertEqual(pfSeries1.count("AIIIE"), 1, "series of I's");
	tSeries1.assertEqual(pfSeries1.count("AE"), 1, "empty series");
	tSeries1.assertEqual(pfSeries1.count("AUEUE"), 2, "two E's");
	tSeries1.assertEqual(pfSeries1.count("AAOEE"), 4, "two pairs");
	tSeries1.assertEqual(pfSeries1.count("AAAAEEEE"), 16, "n^2");
	tSeries1.assertEqual(pfSeries1.count("EAEAEA"), 3, "backwards doesn't count");
	tSeries1.printResults();
	
	Test tSeries2("Misc Series");
	PatternFinder pfStarOnly("*");
	PatternFinder pfTwoStars("**");
	PatternFinder pfThreeAs("A*A*A");
	tSeries2.assertEqual(pfStarOnly.count("AEIOU"), 15, "n(n+1)/2");
	tSeries2.assertEqual(pfTwoStars.count("AEIOU"), 35, "15 + 10 + 6 + 3 + 1");
	tSeries2.assertEqual(pfThreeAs.count("AUAUAUAUA"), 10, "five choose three");
	tSeries2.assertEqual(pfThreeAs.count("AAAAA"), 10, "five choose three, compressed");
	tSeries2.printResults();
};

void testNumericCondition() {
	Test tLess("Less Than 4");
	NumericCondition condLess(4, LESS);
	
	tLess.assertEqual(condLess.evaluate(0), true, "0 is less than 4");
	tLess.assertEqual(condLess.evaluate(1), true, "1 is less than 4");
	tLess.assertEqual(condLess.evaluate(4), false, "4 is equal to 4, not less than");
	tLess.assertEqual(condLess.evaluate(6), false, "6 is not less than 4");
	tLess.assertEqual(condLess.toString(), std::string("at most 3"), "should output \"at most 3\"");
	tLess.printResults();
	
	Test tGreater("Greater Than 6");
	NumericCondition condGreater(6, GREATER);
	tGreater.assertEqual(condGreater.evaluate(3), false, "3 is not greater than 6");
	tGreater.assertEqual(condGreater.evaluate(4), false, "4 is not greater than 6");
	tGreater.assertEqual(condGreater.evaluate(6), false, "6 is equal to, not greater than, 6");
	tGreater.assertEqual(condGreater.evaluate(7), true, "7 is greater than 6");
	tGreater.assertEqual(condGreater.toString(), std::string("at least 7"), "should output \"at least 7\"");
	tGreater.printResults();
	
	Test tGreaterEqual("Greater Than or Equal To 6");
	NumericCondition condGreaterEqual(6, GREATER_EQUAL);
	tGreaterEqual.assertEqual(condGreaterEqual.evaluate(3), false, "3 is not >= 6");
	tGreaterEqual.assertEqual(condGreaterEqual.evaluate(4), false, "4 is not >= 6");
	tGreaterEqual.assertEqual(condGreaterEqual.evaluate(6), true, "6 is equal to 6");
	tGreaterEqual.assertEqual(condGreaterEqual.evaluate(7), true, "7 is >= 6");
	tGreaterEqual.assertEqual(condGreaterEqual.toString(), std::string("at least 6"), "should output \"at least 6\"");
	tGreaterEqual.printResults();
	
	Test tEqual("Equal to 5");
	NumericCondition condEqual(5, EQUAL);
	tEqual.assertEqual(condEqual.evaluate(1), false, "1 is not 5");
	tEqual.assertEqual(condEqual.evaluate(5), true, "5 equals 5");
	tEqual.assertEqual(condEqual.evaluate(6), false, "6 does not equal 5");
	tEqual.assertEqual(condEqual.evaluate(5), true, "5 still equals 5 (stability)");
	tEqual.assertEqual(condEqual.toString(), std::string("exactly 5"), "should output \"exactly 5\"");
	tEqual.printResults();
	
	Test tOdd("Parity Odd, using 7");
	NumericCondition condOdd(7, PARITY);
	tOdd.assertEqual(condOdd.evaluate(3), true, "3 is odd");
	tOdd.assertEqual(condOdd.evaluate(7), true, "7 is odd");
	tOdd.assertEqual(condOdd.evaluate(-1), true, "-1 is odd (negatives are allowed)");
	tOdd.assertEqual(condOdd.evaluate(8), false, "8 is not odd");
	tOdd.assertEqual(condOdd.evaluate(0), false, "0 is not odd");
	tOdd.assertEqual(condOdd.toString(), std::string("an odd number"), "should output \"an odd number of\"");
	tOdd.printResults();
	
	Test tEven("Parity Even, using 2");
	NumericCondition condEven(2, PARITY);
	tEven.assertEqual(condEven.evaluate(4), true, "4 is even");
	tEven.assertEqual(condEven.evaluate(1), false, "1 is not even");
	tEven.assertEqual(condEven.evaluate(5), false, "5 is not even");
	tEven.assertEqual(condEven.evaluate(2), true, "2 is even");
	tEven.assertEqual(condEven.evaluate(0), true, "0 is even");
	tEven.assertEqual(condEven.evaluate(-6), true, "-6 is even (negatives are allowed)");
	tEven.assertEqual(condEven.toString(), std::string("an even number"), "should output \"an even number of\"");
	tEven.printResults();
};

