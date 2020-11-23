#include "catch2/catch.hpp"

//! include explicitely since we are not using the pre-compiled header for tests
//! may change this in the future
#include "stdafx.h" 

#include "cstring.h"

TEST_CASE("CString operations", "[port]") {

	// check creation and comparison
	CString myString("ThisIsAString");
	CHECK(myString == "ThisIsAString");
	CHECK(myString == CString("ThisIsAString"));
	
	SECTION("append data") {

		myString += " - SomeAdditional Data";
		CHECK(myString == "ThisIsAString - SomeAdditional Data");
	}

	SECTION("overwrite data") {

		myString = "Some New Data";
		CHECK(myString == "Some New Data");
	}

	SECTION("conversions") {

		CHECK(myString.ToString() == std::string("ThisIsAString"));

		char* buffer = new char[myString.GetLength()+1];
		sprintf(buffer, "%s", (const char*)myString);
		CHECK(myString == buffer);
		delete[] buffer;
	}
	
	SECTION("casing") {
		
		CHECK(myString.MakeLower() == "thisisastring");
		CHECK(myString.MakeUpper() == "THISISASTRING");	
	}

	SECTION("triming") {
		
		myString = "     With Some Leading Spaces and Trailing Spaces   ";

		SECTION("both") {

			myString.Trim();
			CHECK(myString == "With Some Leading Spaces and Trailing Spaces");
		}
		
		SECTION("left") {

			myString.TrimLeft();
			CHECK(myString == "With Some Leading Spaces and Trailing Spaces   ");
		}
		
		SECTION("right") {

			myString.TrimRight();
			CHECK(myString == "     With Some Leading Spaces and Trailing Spaces");
		}
	}

	SECTION("format") {

		myString.Format("This is an integer %d", 10);
		CHECK(myString == "This is an integer 10");
		std::string fmtString = fmt::sprintf("This is an integer %d", 10);
		CHECK(fmtString == "This is an integer 10");
	}

	SECTION("Access operator"){
		
		CHECK(myString == "ThisIsAString");
		CHECK(myString[8] == 't');
		myString[6] = ' ';
		CHECK(myString == "ThisIs String");
	}
	
	SECTION("Search") {\
	
		myString = "ThisIsAStringThisIsAString";

		CHECK(myString.Find('z') == -1);
		CHECK(myString.Find('\0') == -1);
		CHECK(myString.Find('i') == 2);
		
		CHECK(myString.Find("toto") == -1);
		CHECK(myString.Find("is") == 2);
		CHECK(myString.Find("Is") == 4);

		CHECK(myString.ReverseFind('z') == -1);
		CHECK(myString.ReverseFind('\0') == -1);
		CHECK(myString.ReverseFind('i') == 23);
		
		CHECK(myString.ReverseFind("toto") == -1);
		CHECK(myString.ReverseFind("is") == 15);
		CHECK(myString.ReverseFind("Is") == 17);
	}

	SECTION("Replace") {

		myString.Replace("ThisIsAString", "My new string is actually a sentence");
		CHECK(myString == "My new string is actually a sentence");

		myString.Replace("Toto", "Not in string");
		CHECK(myString == "My new string is actually a sentence");
		
		myString.Replace("", "Not");
		CHECK(myString == "My new string is actually a sentence");
		
		myString.Replace(" ", "Not");
		CHECK(myString == "MyNotnewNotstringNotisNotactuallyNotaNotsentence");
		
		myString = "This is a sentence with a several a thingy!";
		myString.Replace("a", "Euh");
		CHECK(myString == "This is Euh sentence with Euh severEuhl Euh thingy!");
	}

	SECTION("Substring") {

		CString left = myString.Left(100);
		CHECK(left == "ThisIsAString");
		left = myString.Left(3);
		CHECK(left == "Thi");
		left = myString.Left(-100);
		CHECK(left == "");

		CString right = myString.Right(100);
		CHECK(right == "ThisIsAString");
		right = myString.Right(4);
		CHECK(right == "ring");
		right = myString.Right(-100);
		CHECK(right == "");

		CString mid = myString.Mid(100);
		CHECK(mid == "");
		mid = myString.Mid(-100);
		CHECK(mid == "ThisIsAString");

		mid = myString.Mid(4, 4);
		CHECK(mid == "IsAS");
		mid = myString.Mid(7, 100);
		CHECK(mid == "String");
	}

    SECTION("Tokenize") {

        myString = "%First Second#Third";

        CString resToken;
        int curPos = 0;

        resToken = myString.Tokenize("% #", curPos);
        CHECK(resToken == "First");
        CHECK(curPos == 6);
        resToken = myString.Tokenize("% #", curPos);
        CHECK(resToken == "Second");
        CHECK(curPos == 13);
        resToken = myString.Tokenize("% #", curPos);
        CHECK(resToken == "Third");
        CHECK(curPos == 19);
        resToken = myString.Tokenize("% #", curPos);
        CHECK(resToken == "");
        CHECK(curPos == 19);
    }

    SECTION("Span Excluding") {

        myString = "World Cup '98";

        CString spaned = myString.SpanExcluding(";,.-'");
        CHECK(spaned == "World Cup ");

        CString str1 ("Hello World! Goodbye!");
        CString str2 = str1.SpanExcluding(".!?");
        CHECK(str2 == "Hello World");

        CString str3 ("Hello World Goodbye");
        CString str4 = str3.SpanExcluding(".!?");
        CHECK(str4 == "Hello World Goodbye");
    }

    SECTION("Operations") {

        myString = "World Cup '98";

        CString added = myString + " toto";
        CHECK(added == "World Cup '98 toto");
    }

    SECTION("Format") {

        CString added;
		added.Format("%7.2f", 6.432432);
        CHECK(added == "   6.43");
    }
}
