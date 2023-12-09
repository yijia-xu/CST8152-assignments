/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2023
* Author: Yijia Xu 041061204
* Professors: Paulo Sousa
************************************************************
=---------------------------------------=
|  COMPILERS - ALGONQUIN COLLEGE (F23)  |
=---------------------------------------=


_|          _|_|    _|      _|    _|_|
_|        _|    _|  _|_|    _|  _|    _|
_|        _|_|_|_|  _|  _|  _|  _|_|_|_|
_|        _|    _|  _|    _|_|  _|    _|
_|_|_|_|  _|    _|  _|      _|  _|    _|
*/

/*
************************************************************
* File name: Parser.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
************************************************************
*/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef READER_H_
#include "Reader.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token			lookahead;
extern BufferPointer	stringLiteralTable;
extern int32			line;
extern Token			tokenizer(void);
extern string			keywordTable[KWT_SIZE];
static int32			syntaxErrorNumber = 0;

#define LANG_WRTE		"Println"
#define LANG_READ		"Scan"
#define LANG_MAIN		"main"

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	KW_func,
	KW_int32,
	KW_int64,
	KW_float32,
	KW_rune,
	KW_string,
	KW_if,
	KW_for,
	KW_else
};

/* TO_DO: Define the number of BNF rules */
#define NUM_BNF_RULES 36

/* Parser */
typedef struct parserData {
	int32 parsHistogram[NUM_BNF_RULES];	/* Number of BNF Statements */
} ParserData, * pParsData;

/* Number of errors */
int32 numParserErrors;

/* Scanner data */
ParserData psData;

/* Function definitions */
void startParser();
void matchToken(int32, int32);
void syncErrorHandler(int32);
void printError();
void printBNFData(ParserData psData);

/* List of BNF statements */
enum BNF_RULES {
	BNF_error,										/*  0: Error token */
	BNF_comment,									/*  1 */
	BNF_optVarListDeclarations,						/*  2 */
	BNF_optionalStatements,							/*  3 */
	BNF_outputStatement,							/*  4 */
	BNF_outputVariableList,							/*  5 */
	BNF_program,									/*  6 */
	BNF_statement,									/*  7 */
	BNF_statements,									/*  8 */
	BNF_statementsPrime,							/*  9 */
	BNF_assignmentExpression,						/*  10 */
	BNF_assignmentStatement,						/*  11 */
	BNF_selectionStatement,							/*  12 */
	BNF_conditionalExpression,						/*  13 */
	BNF_varIdentifier,								/*  14 */
	BNF_varList,									/*  15 */
	BNF_varListPrime,								/*  16 */
	BNF_inputStatement,								/*  17 */
	BNF_iterationStatement,							/*  18 */
	BNF_arithmeticExpression,						/*  19 */
	BNF_unaryArithmeticExpression,					/*  20 */
	BNF_additiveArithmeticExpression,				/*  21 */
	BNF_additiveArithmeticExpressionPrime,			/*  22 */
	BNF_multiplicativeArithmeticExpression,			/*  23 */
	BNF_multiplicativeArithmeticExpressionPrime,	/*  24 */
	BNF_primaryArithmeticExpression,				/*  25 */
	BNF_primaryStringExpression,					/*  26 */
	BNF_stringExpressionPrime,						/*  27 */
	BNF_stringExpression,							/*  28 */
	BNF_logicalORExpression,						/*  29 */
	BNF_logicalORExpressionPrime,					/*  30 */
	BNF_logicalANDExpression,						/*  31 */
	BNF_logicalANDExpressionPrime,					/*  32 */
	BNF_logicalNOTExpression,						/*  33 */
	BNF_relationalExpression,						/*  34 */
	BNF_a_relationalExpression						/*  35 */

};


/* TO_DO: Define the list of keywords */
static string BNFStrTable[NUM_BNF_RULES] = {
	"BNF_error",							// 0
	"BNF_comment",							// 1
	"BNF_optVarListDeclarations",			// 2
	"BNF_optionalStatements",				// 3
	"BNF_outputStatement",					// 4
	"BNF_outputVariableList",				// 5
	"BNF_program",							// 6
	"BNF_statement",						// 7
	"BNF_statements",						// 8
	"BNF_statementsPrime",					// 9		
	"BNF_assignmentExpression",				// 10
	"BNF_assignmentStatement",				// 11
	"BNF_selectionStatement",				// 12
	"BNF_conditionalExpression",			// 13
	"BNF_varIdentifier",					// 14
	"BNF_varList",							// 15
	"BNF_varListPrime",						// 16
	"BNF_inputStatement",					// 17
	"BNF_iterationStatement",				// 18						
	"BNF_arithmeticExpression",				// 19
	"BNF_unaryArithmeticExpression",		// 20
	"BNF_additiveArithmeticExpression",		// 21
	"BNF_additiveArithmeticExpressionPrime",// 22
	"BNF_multiplicativeArithmeticExpression",// 23
	"BNF_multiplicativeArithmeticExpressionPrime",// 24
	"BNF_primaryArithmeticExpression",		// 25
	"BNF_primaryStringExpression",			// 26
	"BNF_stringExpressionPrime",			// 27
	"BNF_stringExpression",					// 28
	"BNF_logical_OR_Expression",			// 29
	"BNF_logical_OR_ExpressionPrime",		// 30
	"BNF_logical_AND_Expression",			// 31
	"BNF_logical_AND_ExpressionPrime",		// 32
	"BNF_logical_NOT_Expression",			// 33
	"BNF_relationalExpression",				// 34
	"BNF_a_relationalExpression"			// 35
};

/* TO_DO: Place ALL non-terminal function declarations */
void comment();
void optVarListDeclarations();
void varList();
void varListPrime();
void varIdentifier();
void inputStatement();
void optionalStatements();
void outputStatement();
void outputVariableList();
void program();
void statement();
void statements();
void statementsPrime();
void assignmentExpression();
void assignmentStatement();
void iterationStatement();
void selectionStatement();
void conditionalExpression();
void arithmeticExpression();
void unaryArithmeticExpression();
void primaryArithmeticExpression();
void additiveArithmeticExpression();
void additiveArithmetic_ePrime();
void multiplicativeArithmeticExpression();
void multiplicativeArithmetic_ePrime();
void logical_OR_Expression();
void logical_OR_ePrime();
void logical_AND_Expression();
void logical_AND_ePrime();
void logical_NOT_Expression();
void relationalExpression();
void primary_a_RelationalExpression();
void primaryStringExpression();
void stringExpressionPrime();
void stringExpression();


#endif
