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
* File name: Parser.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/* Parser data */
extern ParserData psData; /* BNF statistics */

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
/* TO_DO: This is the function to start the parser - check your program definition */

void startParser() {
	/* TO_DO: Initialize Parser data */
	int32 i = 0;
	for (i = 0; i < NUM_BNF_RULES; i++) {
		psData.parsHistogram[i] = 0;
	}
	/* Proceed parser */
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
/* TO_DO: This is the main code for match - check your definition */
void matchToken(int32 tokenCode, int32 tokenAttribute) {
	int32 matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType != tokenAttribute)
			matchFlag = 0;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
/* TO_DO: This is the function to handler error - adjust basically datatypes */
void syncErrorHandler(int32 syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
/* TO_DO: This is the function to error printing - adjust basically datatypes */
void printError() {
	extern numParserErrors;			/* link to number of errors (defined in Parser.h) */
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("*ERROR*: %s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T:\t\t%s\t\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("KW_T: %s\n", keywordTable[t.attribute.codeType]);
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case EOS_T:
		printf("NA\n");
		break;
	case CMT_T:
		printf("CMT_T\n");
		break;
	case VID_T:
		printf("VID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case INL_T:
		printf("INL_T\t\t%d\n", t.attribute.intValue);
		break;
	case FPL_T:
		printf("FPL_T\t\t%f\n", t.attribute.floatValue);
		break;
	case ARI_T:
		printf("ARI_T\n");
		break;
	case REL_T:
		printf("REL_T\n");
		break;
	case LOG_T:
		printf("LOG_T\n");
		break;
	case ASS_T:
		printf("ASS_T\n");
		break;
	case COM_T:
		printf("COM_T\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
		numParserErrors++; // Updated parser error
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> main& { <opt_statements> }
 * FIRST(<program>)= {CMT_T, MNID_T (main&), SEOF_T}.
 ***********************************************************
 */
void program() {
	/* Update program statistics */
	psData.parsHistogram[BNF_program]++;
	/* Program code */
	switch (lookahead.code) {
	case CMT_T:
		comment();
	case KW_T:
		if (lookahead.attribute.codeType == KW_func) {
			matchToken(KW_T, KW_func);
			switch (lookahead.code) {
			case MNID_T:
				if (strncmp(lookahead.attribute.idLexeme, LANG_MAIN, 4) == 0) {
					matchToken(MNID_T, NO_ATTR);
					matchToken(LPR_T, NO_ATTR);
					optVarListDeclarations();
					matchToken(RPR_T, NO_ATTR);

					matchToken(LBR_T, NO_ATTR);
					optionalStatements();
					matchToken(RBR_T, NO_ATTR);
					break;
				}
				else {
					printError();
				}
			default:
				break;
			}
		}
		break;
	case SEOF_T:
		; // Empty
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

/*
 ************************************************************
 * comment
 * BNF: comment
 * FIRST(<comment>)= {CMT_T}.
 ***********************************************************
 */
void comment() {
	psData.parsHistogram[BNF_comment]++;
	matchToken(CMT_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Comment parsed");
}


/*
 ************************************************************
 * Optional Var List Declarations
 * BNF: <opt_varlist_declarations> -> <varlist_declarations> | e
 * FIRST(<opt_varlist_declarations>) = { e, KW_T (KW_int), KW_T (KW_real), KW_T (KW_string)}.
 ***********************************************************
 */
void optVarListDeclarations() {
	psData.parsHistogram[BNF_optVarListDeclarations]++;
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case KW_float32:
		case KW_int32:
		case KW_int64:
		case KW_rune:
		case KW_string:
			matchToken(KW_T, lookahead.attribute.codeType);
			varList(); 
			break;
		default:
			break;
		}
		break;
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
}

void varList() {
	psData.parsHistogram[BNF_varList]++;
	varIdentifier();
	varListPrime();
	printf("%s%s\n", STR_LANGNAME, ": Variable List Declarations parsed");
}

void varListPrime() {
	psData.parsHistogram[BNF_varListPrime]++;
	switch (lookahead.code) {
	case COM_T:
		matchToken(COM_T, NO_ATTR);
		varIdentifier();
		varListPrime();
		break;
	case EOS_T:
		matchToken(EOS_T, NO_ATTR);
		break;
	default:
		break;
	}
}


void varIdentifier() {
	psData.parsHistogram[BNF_varIdentifier]++;
	switch (lookahead.code) {
	case VID_T:
		matchToken(VID_T, NO_ATTR);
		break;
	default: 
		printError(); 
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Variable Identifier parsed");
}


/* TO_DO: Continue the development (all non-terminal functions) */

/*
 ************************************************************
 * Optional statement
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<opt_statements>) = { ϵ , IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *				KW_T(KW_while), MNID_T(print&), MNID_T(input&) }
 ***********************************************************
 */
void optionalStatements() {
	psData.parsHistogram[BNF_optionalStatements]++;
	switch (lookahead.code) {
	case CMT_T:
		comment();
	case MNID_T:
		if ((strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 7) == 0) ||
			(strncmp(lookahead.attribute.idLexeme, LANG_READ, 4) == 0)) {
			statements();
			break;
		}
	case KW_T:
		if (lookahead.attribute.codeType != KW_func &&
			lookahead.attribute.codeType != KW_else) {
			statements();
			break;
		}
		break;
	case VID_T:
		statements();
		break;
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*
 ************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *		KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
void statements() {
	psData.parsHistogram[BNF_statements]++;
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
 ************************************************************
 * Statements Prime
 * BNF: <statementsPrime> -> <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T, 
 *		KW_T(KW_if), KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
void statementsPrime() {
	switch (lookahead.code) {
	case VID_T:
		statement();
		statementsPrime();
		break;
	case MNID_T:
		if ((strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 7) == 0) ||
			(strncmp(lookahead.attribute.idLexeme, LANG_READ, 4) == 0)) {
			statement();
			statementsPrime();
		}
		break;
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case KW_else:
		case KW_func:
			printError();
			break;
		default:
			statement();
			statementsPrime();
			break;
		}
	}
}


/*
 ************************************************************
 * Single statement
 * BNF: <statement> -> <assignment statement> | <selection statement> |
 *	<iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if), KW_T(KW_while),
 *			MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
void statement() {
	psData.parsHistogram[BNF_statement]++;
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case KW_if:
			selectionStatement();
			break;
		case KW_for:
			iterationStatement();
			break;
		case KW_float32:
		case KW_int32:
		case KW_int64:
		case KW_rune:
		case KW_string:
			optVarListDeclarations();
			break;
		}
		break;
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 7) == 0) {
			outputStatement();
			break;
		}
		if (strncmp(lookahead.attribute.idLexeme, LANG_READ, 4) == 0) {
			inputStatement();
			break;
		}
		break;
	case VID_T:
		assignmentStatement();
		break;
	default:
		printError();
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*
 ************************************************************
 * Output Statement
 * BNF: <output statement> -> print& (<output statementPrime>);
 * FIRST(<output statement>) = { MNID_T(print&) }
 ***********************************************************
 */
void outputStatement() {
	psData.parsHistogram[BNF_outputStatement]++;
	matchToken(MNID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	outputVariableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

/*
 ************************************************************
 * Output Variable List
 * BNF: <opt_variable list> -> <variable list> | ϵ
 * FIRST(<opt_variable_list>) = { IVID_T, FVID_T, SVID_T, ϵ }
 ***********************************************************
 */
void outputVariableList() {
	psData.parsHistogram[BNF_outputVariableList]++;
	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	case VID_T:
		varList();
		break;
	default:
		; //episilon
	}
	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}

void inputStatement() {
	psData.parsHistogram[BNF_inputStatement]++;
	matchToken(MNID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	varList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Input statement parsed");
}

void iterationStatement() {
	psData.parsHistogram[BNF_iterationStatement]++;
	matchToken(KW_T, KW_for);
	conditionalExpression();
	matchToken(LBR_T, NO_ATTR);
	statements();
	matchToken(RBR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Iteration statement parsed");
}

void assignmentExpression() {
	psData.parsHistogram[BNF_assignmentExpression]++;
	switch (lookahead.code)
	{
	case VID_T:
		matchToken(VID_T, NO_ATTR);
		matchToken(ASS_T, NO_ATTR);
		if (lookahead.code == STR_T)
			stringExpression();
		else
			arithmeticExpression();
		break;
	default:
		printError();
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Assignment expression parsed");
}

void assignmentStatement() {
	psData.parsHistogram[BNF_assignmentStatement]++;
	assignmentExpression();
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Assignment statement parsed");
}


void selectionStatement() {
	psData.parsHistogram[BNF_selectionStatement]++;
	matchToken(KW_T, KW_if);
	conditionalExpression();
	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	matchToken(RBR_T, NO_ATTR);
	matchToken(KW_T, KW_else);
	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	matchToken(RBR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Selection statement parsed");
}

void conditionalExpression() {
	psData.parsHistogram[BNF_conditionalExpression]++;
	logical_OR_Expression();
	printf("%s%s\n", STR_LANGNAME, ": Conditional expression parsed");
}

void arithmeticExpression() {
	psData.parsHistogram[BNF_arithmeticExpression]++;
	switch (lookahead.code)
	{
	case ARI_T:
		switch (lookahead.attribute.arithmeticOperator) {
		case OP_ADD:
		case OP_SUB:
			unaryArithmeticExpression();
			break;
		default:
			break;
		}
		break;
	case INL_T:
	case FPL_T:
	case VID_T:
	case LPR_T:
		additiveArithmeticExpression();
		break;
	default:
		printError();
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Arithmetic expression parsed");
}

void unaryArithmeticExpression() {
	psData.parsHistogram[BNF_unaryArithmeticExpression]++;
	switch (lookahead.code)
	{
	case ARI_T:
		switch (lookahead.attribute.arithmeticOperator)
		{
		case OP_ADD:
			matchToken(ARI_T, OP_ADD);
			primaryArithmeticExpression();
			break;
		case OP_SUB:
			matchToken(ARI_T, OP_SUB);
			primaryArithmeticExpression();
			break;
		default:
			printError();
			break;
		}
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Unary arithmetic expression parsed");
}

void primaryArithmeticExpression() {
	psData.parsHistogram[BNF_primaryArithmeticExpression]++;
	switch (lookahead.code)
	{
	case VID_T:
		matchToken(VID_T, NO_ATTR);
		break;
	case INL_T:
		matchToken(INL_T, NO_ATTR);
		break;
	case FPL_T:
		matchToken(FPL_T, NO_ATTR);
		break;
	case LPR_T:
		matchToken(LPR_T, NO_ATTR);
		arithmeticExpression();
		matchToken(RPR_T, NO_ATTR);
		break;
	default:
		printError();
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Primary arithmetic expression parsed");
}

void additiveArithmeticExpression() {
	psData.parsHistogram[BNF_additiveArithmeticExpression]++;
	multiplicativeArithmeticExpression();
	additiveArithmetic_ePrime();
	printf("%s%s\n", STR_LANGNAME, ": Additive arithmetic expression parsed");
}

void additiveArithmetic_ePrime() {
	switch (lookahead.code)
	{
	case ARI_T:
		switch (lookahead.attribute.arithmeticOperator) {
		case OP_ADD:
			matchToken(ARI_T, OP_ADD);
			if (lookahead.code == STR_T) {
				stringExpression();
			}
			else {
				multiplicativeArithmeticExpression();
				additiveArithmetic_ePrime();
			}
			break;
		case OP_SUB:
			matchToken(ARI_T, OP_SUB);
			multiplicativeArithmeticExpression();
			additiveArithmetic_ePrime();
			break;
		}
	default:
		break;
	}
}

void multiplicativeArithmeticExpression() {
	psData.parsHistogram[BNF_multiplicativeArithmeticExpression]++;
	primaryArithmeticExpression();
	multiplicativeArithmetic_ePrime();
	printf("%s%s\n", STR_LANGNAME, ": Multiplicative arithmetic expression parsed");
}

void multiplicativeArithmetic_ePrime() {
	switch (lookahead.code)
	{
	case ARI_T:
		if (lookahead.attribute.arithmeticOperator == OP_MUL || lookahead.attribute.arithmeticOperator == OP_DIV) {
			matchToken(ARI_T, lookahead.attribute.arithmeticOperator);
			primaryArithmeticExpression();
			multiplicativeArithmetic_ePrime();
		}
		break;
	default:
		break;
	}
}

void logical_OR_Expression() {
	psData.parsHistogram[BNF_logicalORExpression]++;
	logical_AND_Expression();
	logical_OR_ePrime();
	printf("%s%s\n", STR_LANGNAME, ": Logical OR expression parsed");
}

void logical_OR_ePrime() {
	if (lookahead.code == LOG_T) {
		if (lookahead.attribute.logicalOperator == OP_OR){
			matchToken(LOG_T, OP_OR);
			logical_AND_Expression();
			logical_OR_ePrime();
		}
	}
}

void logical_AND_Expression() {
	psData.parsHistogram[BNF_logicalANDExpression]++;
	relationalExpression();
	logical_AND_ePrime();
	printf("%s%s\n", STR_LANGNAME, ":  Logical AND expression parsed");
}

void logical_AND_ePrime() {
	switch (lookahead.code) {
	case LOG_T:
		if (lookahead.attribute.logicalOperator == OP_AND) {
			matchToken(LOG_T, OP_AND);
			relationalExpression();
			logical_AND_ePrime();
		}
		break;
	}
}

void logical_NOT_Expression() {
	psData.parsHistogram[BNF_logicalNOTExpression]++;
	switch (lookahead.code) {
	case LOG_T:
		if (lookahead.attribute.logicalOperator == OP_NOT) {
			matchToken(LOG_T, OP_NOT);
			relationalExpression();
			printf("%s%s\n", STR_LANGNAME, ":  Logical NOT expression parsed");
		}
		break;
	}
}

void relationalExpression(){
	psData.parsHistogram[BNF_relationalExpression]++;
	switch (lookahead.code) {
	case FPL_T:
	case INL_T:
	case VID_T:
		primary_a_RelationalExpression();
		if (lookahead.code == REL_T) {
			switch (lookahead.attribute.relationalOperator) {
			case OP_GT:
				matchToken(REL_T, OP_GT);
				break;
			case OP_LT:
				matchToken(REL_T, OP_LT);
				break;
			case OP_EQ:
				matchToken(REL_T, OP_EQ);
				break;
			case OP_NE:
				matchToken(REL_T, OP_NE);
			}
			primary_a_RelationalExpression();
			break;
		}
	default:
		printError();
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ":  Relational expression parsed");
}

void primary_a_RelationalExpression() {
	psData.parsHistogram[BNF_a_relationalExpression]++;
	switch (lookahead.code)
	{
	case FPL_T:
		matchToken(FPL_T, NO_ATTR);
		break;
	case INL_T:
		matchToken(INL_T, NO_ATTR);
		break;
	case VID_T:
		matchToken(VID_T, NO_ATTR);
		break;
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	default:
		printError();
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Primary arithmetic relational expression parsed");
}


void primaryStringExpression() {
	psData.parsHistogram[BNF_primaryStringExpression]++;
	switch (lookahead.code)
	{
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	case VID_T:
		matchToken(VID_T, NO_ATTR);
		break;
	default:
		printError();
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Primary string expression parsed");
}

void stringExpressionPrime() {
	switch (lookahead.code)
	{
	case ARI_T:
		if (lookahead.attribute.arithmeticOperator == OP_ADD) {
			matchToken(ARI_T, OP_ADD);
			primaryStringExpression();
			stringExpressionPrime();
		}
		break;
	default:
		break;
	}
}

void stringExpression() {
	psData.parsHistogram[BNF_stringExpression]++;
	primaryStringExpression();
	stringExpressionPrime();
	printf("%s%s\n", STR_LANGNAME, ": String expression parsed");
}

/*
 ************************************************************
 * The function prints statistics of BNF rules
 * Param:
 *	- Parser data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
/*
void printBNFData(ParserData psData) {
}
*/
void printBNFData(ParserData psData) {
	/* Print Parser statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_BNF_RULES; cont++) {
		if (psData.parsHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", BNFStrTable[cont], "]=", psData.parsHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}
