/* scanner.h for assignment 4 on logical formulas */

#ifndef SCANNER_H
#define SCANNER_H

#define MAXINPUT 100  /* maximal length of the input */
#define MAXIDENT 10   /* maximal length of an identifier */

typedef enum TokenType {
  Identifier,
  Symbol
} TokenType;

typedef union Token {
  int number;
  char *identifier;
  char symbol;
} Token;

typedef struct ListNode *List;

typedef struct ListNode {
  TokenType tt;
  Token t;
  List next;
} ListNode;

/* Declaration of the functions that are defined in scanner.c and to be used outside it. */

char *readInput();
List tokenList(char *array);
void printList(List l);
void freeTokenList(List l);
void scanExpressions();

#endif
