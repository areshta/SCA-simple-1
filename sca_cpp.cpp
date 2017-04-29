//******************************************************************************
//* Static Code C++ Analyzers testing example
//*
//* Warning: This example is not the way to find out what analyzer "is better".
//*          It consists few issues only. There are thousands code issues.
//*          If you create other example with other set of errors you could have other results of comparing.
//*          Results depends from analyzers options too          
//*	
//* Purpose: The purpose of this example is draft verifying of analyzer settings and demonstration the fact
//*          that any analyzer does not cover all issues in the code that can be found during static analyses 
//*
//* Technical notes:
//*           - For looking false negative analyzer issues only
//*           - The examples includes 38 genera c++ issues of different priority, 6 of them are catastrophic
//*           - The example does not include stl issues and c++11/14/17 specific issues
//*           - The example does not include multi-thread issues
//*           - The example does not include cross-modules issues
//*           - A lot of of style and non-serious issues is uncounted
//*           - gcc 5.4.0 compiles it without warnings (if used without parameters)
//*
//* Created by: Alex Reshta. 
//* 			No restrictions for copying or using
//* 			Version 1.02 2017/04/29
//******************************************************************************  

#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std; // error 34: whole namespace is opened must be:
					 // using std::cout;
					 // using std::endl;
					 // using std::string;


class baseA
{
public:
    baseA() // error 38: initialisation of members must be placed in initialisation list
	{
		int64_t* pB = new int64_t[10]; 	// error 1,2: local pB hides member pB; pB member is not initialized
        pA = new int64_t[10];
        pC = new int64_t;
		pD = new int64_t[10]; 
		int a = 7; 						// error 35: using 'raw' int instead size specific int (as int32_t);
        cout << "Using 'raw' int " << a << endl;
		
		
    } 									// error 3: unused pB, error 4: memory leaks
    
	~baseA()							// error 5: must be virtual ~baseA()
    {
		cout << "Destructor parent class baseA." << endl; 
		delete pA; 						// error 6: must be 'delete[]';
		delete pB; 						// error 7: 'delete' of non-initialized point64_ter
		delete[] pC; 					// error 8: must be 'delete pC';
		
    } //error 9: pD is not freed;
	const char *getstr()
	{
		string a = "Ref to local variable" ;
		return a.c_str(); //error 10: return the point64_ter that point64_ts inside local variable
    }

private:
    int64_t *pA;
	  int64_t *pB;
    int64_t *pC;
    int64_t *pD;
}; // error 26: no user-provided copy constructor, error 27: no user-provided copy assignment operator


class childB: public baseA
{
public:

	childB() 
    {
		const char *str = "Some String"; // error 36: using 'raw' char type ( could be wchar_t, char8_t, signed char...).
		pString = new char[strlen(str)]; // error 11: Must be 'strlen(str)+1'
		(void)strcpy(pString,str);		 // error 37: using of strcpy is danger
        cout << "Non-initialized variable issue a = " << a << endl; //error 12: Non-initialized variable using.
    } 
    void rec()
	{
		cout << "rec" << endl;
		rec();	// error 13: recursion error
	}    
    void recA()
    {
		cout << "recA" << endl;
		recB(); // error 14: cross recursion error
    }
    void recB()
    {
		cout << "recB" << endl;
		recA();
    }	

	
	static int64_t getA()
	{
		return 1;
	}
	static int64_t getB()
	{
		return 0;
	}
	static int64_t devZero()
	{
		return getA() / getB(); //error 15 division by zero
	}


    
	~childB() // destructor will never call via error3 - memory leaks
	{
		cout << "Destructor child class childB."<<endl; 
		delete pString; // error 16: Must be delete[]
    }


private:
	char *pString;
	int64_t a;
}; // error 28: no user-provided copy constructor, error 29: no user-provided copy assignment operator



class SimplyCreazy
{
public:
	SimplyCreazy(): pZero(0) {}	
	
	void UnbrokenFor()
	{
		int a[10]; 
		for(int64_t i=0; i <10; i++)
		{
			if( i || !(42 < 0) ) 	// error 17: no sense always true uder if, 
			{
				i--; // error 18: circle counter! as result the circle is unbroken
				cout << "a[i] = " << a[i] << endl;  // error 30: access to non-init array, error 31: using signed index (size_t could be used)
													// error 32: using array where vector could be used
			}
		}
	}

	void BadPointerOperation()
	{
		
		strcpy(pZero,"Hello"); 		// error 19: copy to zero point64_ter		
		strcpy(pNonIni,"Hello"); 	// error 20: copy to non-initialized point64_ter
		*pZero++; 					// error 21: non-using result of operation (* - no sense, unused), error 22: using nul point64_ter
		cout << "Creazy Point64_ters string: " << pZero << " " << pNonIni << endl; // error 22: using nul point64_ter
		return;
		// error 23: bad control flow: unreachable instruction
		cout << "never accessable output " << endl;
	}
	
	void PassStringAsValue(string s) // error 24: bad string sending. Must be PassStringAsValue(const string& s)
	{
	    cout << "Bad sending string example: " << s << endl; 
	}

private:
	char *pZero;
	char *pNonIni;
};


int32_t  main (int32_t argc, const char* const argv[] ) 
{
    cout << "=== Happy start of the program :) ===" << endl;

	int64_t crashCode = 0;
	if ( argc > 1 )
    {
		crashCode = atoi(argv[1]);
    }
	
	SimplyCreazy sc;
	
	baseA *pB = new childB(); // result of errors 1,2,3,4,5,6
    cout << "Error:"<< pB->getstr() << endl; 
    delete pB;

	if (crashCode > 6)
	{
		goto JUMP; 		// error 33: using goto
	}
	
	if (crashCode == 1 )
    {
		childB b1;
		childB b2(b1);  // error 25: show error of absence of deep copying
		b1 = b2;		// error 25: show error of absence of deep copying
    }
	
	else if (crashCode == 2 )
    {
		cout << "Unbroken recursion is starting..." << endl;
		childB b1;
		b1.rec(); 					// error 13: recursion error
    }
	
	else if (crashCode == 3 )
    {
		cout << "Unbroken cross recursion is starting..." << endl;
		childB b1;
		b1.recA(); 					// error 14: cross recursion error
    }
	
	else if (crashCode == 4 ) 		//error 15: division by zero
    {
		cout << "Division by zero is starting..." << endl;
		int64_t c = childB::devZero() ;
		cout << "c = " << c << endl;
    }
 
	else if (crashCode == 5 )
    {
		sc.UnbrokenFor(); 			// error 17, 16: if with always true condition and unbroken circle 
    }
		
	else if (crashCode == 6 )
    {
		sc.BadPointerOperation(); 	// error 19, 18, 19, 20
    }								// finished else is recommended for else-if sequence

JUMP:
	
	string s("Some string");
	sc.PassStringAsValue(s); 		// error 24: uncorrect string sending

    cout << "=== Happy end of the program :) ===" << endl;

	return 0;
}


