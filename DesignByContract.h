
//════════════════════════════════════════════================================
// Name        : DesignByContract.h
// Author      : Serge Demeyer
// Version     :
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
// Description : Declarations for design by contract valid C++
//════════════════════════════════════════════================================

#include <assert.h>
#include <iostream>

#ifdef _WIN32
// Windows:
// ════════════════════════════════════════════=========================================
#define REQUIRE(assertion, what) if (!(assertion)) _assert (what, __FILE__, __LINE__)
#define ENSURE(assertion, what) if (!(assertion)) _assert (what, __FILE__, __LINE__)
// ════════════════════════════════════════════=========================================
#else
// WSL:
// ════════════════════════════════════════════=========================================
#define REQUIRE(assertion, what) if (!(assertion)) __assert (what, __FILE__, __LINE__)
#define ENSURE(assertion, what) if (!(assertion)) __assert (what, __FILE__, __LINE__)
// ════════════════════════════════════════════=========================================
#endif
