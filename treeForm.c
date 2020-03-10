/* treeForm.c for assignment 4 on logical formulas
 *
 * This recognizes and makes syntax trees for the following grammar:
 *
 * <atom>     ::=  'T' | 'F' | <identifier> | '(' <formula> ')'
 * <literal>  ::=  <atom> | '~' <atom>
 * <formula>  ::=  <literal> { '&' <literal> }
 *
 */

#include <stdio.h>  /* getchar, printf */
#include <stdlib.h> /* NULL */
#include <assert.h>

#include "scanner.h"
#include "treeForm.h"

/* The acceptCharacter function takes a pointer to a token list and a character.
 * It checks whether the first token in the list is a Symbol with the given character.
 * When that is the case, it returns 1 and moves the pointer to the rest of the token list.
 * Otherwise it yields 0 and the pointer remains unchanged.
 */

int acceptCharacter(List *lp, char c) {
  if (*lp != NULL && (*lp)->tt == Symbol && ((*lp)->t).symbol == c ) {
    *lp = (*lp)->next;
    return 1;
  }
  return 0;
}

FormTree newFormTreeNode(TokenType tt, Token t, FormTree tL, FormTree tR) {
  FormTree new = malloc(sizeof(FormTreeNode));
  assert (new != NULL);
  new->tt = tt;
  new->t = t;
  new->left = tL;
  new->right = tR;
  return new;
}

void freeTree(FormTree t) {
  if (t == NULL) {
    return;
  }
  freeTree(t->left);
  freeTree(t->right);
  free(t);
}

int treeIdentifier(List *lp, FormTree *t) {
  if (*lp != NULL && (*lp)->tt == Identifier ) {
    *t = newFormTreeNode(Identifier, (*lp)->t, NULL, NULL);
    *lp = (*lp)->next;
    return 1;
  }
  return 0;
}

// <atom>  ::=  'T' | 'F' | <identifier> | '(' <formula> ')'
int treeAtom(List *lp, FormTree *t) {
  if (acceptCharacter(lp,'T')) {
    Token tok;
    tok.symbol = 'T';
    *t = newFormTreeNode(Symbol, tok, NULL, NULL);
    return 1;
  }
  if (acceptCharacter(lp,'F')) {
    Token tok;
    tok.symbol = 'F';
    *t = newFormTreeNode(Symbol, tok, NULL, NULL);
    return 1;
  }
  if (treeIdentifier(lp,t)) {
    return 1;
  }
  if (acceptCharacter(lp,'(') && treeBiconditional(lp,t) && acceptCharacter(lp,')') ) {
    return 1;
  }
  return 0;
}

// <literal>  ::=  <atom> | '~' <atom>
int treeLiteral(List *lp, FormTree *t) {
  if (treeAtom(lp,t)) {
    return 1;
  }
  if (acceptCharacter(lp,'~')) {
    FormTree tL = NULL;
    if (treeAtom(lp, &tL)) {
      Token tok;
      tok.symbol = '~';
      *t = newFormTreeNode(Symbol, tok, tL, NULL);
      return 1;
    }
    freeTree(tL);
  }
  return 0;
}


int treeConjunction(List *lp, FormTree *t) {
  if (!treeLiteral(lp,t) ) {
    return 0;
  }
  while (acceptCharacter(lp,'&') ) {
    FormTree tL = *t;
    FormTree tR = NULL;
    if ( !treeLiteral(lp,&tR) ) {
      freeTree(tR);
      return 0;
    }
    Token tok;
    tok.symbol = '&';
    *t = newFormTreeNode(Symbol, tok, tL, tR);
  } // no '&', so we reached the end of disjunction 
  return 1;
}

int treeDisjunction(List *lp, FormTree *t) {
  if ( !treeConjunction(lp,t) ) {
    return 0;
  }
  while (acceptCharacter(lp,'|') ) {
    FormTree tL = *t;
    FormTree tR = NULL;
    if ( !treeConjunction(lp,&tR) ) {
      freeTree(tR);
      return 0;
    }
    Token tok;
    tok.symbol = '|';
    *t = newFormTreeNode(Symbol, tok, tL, tR);
  } // no '|', so we reached the end of disjunction 
  return 1;
}

int treeImplication(List *lp, FormTree *t) {
  if (!treeDisjunction(lp,t) ) {
    return 0;
  }
  if (acceptCharacter(lp, '-')) {
	if (acceptCharacter(lp, '>')) {
		FormTree tL = *t;
		FormTree tR = NULL;
		if (!treeDisjunction(lp,&tR) ) {
			freeTree(tR);
			return 0;
		}
		Token tok;
		tok.symbol = '-';
		*t = newFormTreeNode(Symbol, tok, tL, tR);
	}
  }
  return 1;
}

int treeBiconditional(List *lp, FormTree *t) {
  if (!treeImplication(lp,t) ) {
    return 0;
  }
  if (acceptCharacter(lp, '<')) {
	if (acceptCharacter(lp, '-')) {
		if (acceptCharacter(lp, '>')) {
			FormTree tL = *t;
			FormTree tR = NULL;
			if (!treeImplication(lp,&tR) ) {
				freeTree(tR);
				return 0;
			}
			Token tok;
			tok.symbol = '<';
			*t = newFormTreeNode(Symbol, tok, tL, tR);
		}
	}
  }
  return 1;
}


void printTree(FormTree t) {
  if (t == NULL) {
    return;
  }
  switch (t->tt) {
  case Symbol:
    switch (t->t.symbol) {
    case 'T':
    case 'F':
      printf("%c",t->t.symbol);
      break;
    case '~':
      printf("(~");
      printTree(t->left);
      printf(")");
      break;
    case '-':
	  printf("(");
	  printTree(t->left);
      printf(" -> ");
      printTree(t->right);
      printf(")");
      break;
    case '<':
	  printf("(");
	  printTree(t->left);
      printf(" <-> ");
      printTree(t->right);
      printf(")");
      break;
    default:
      printf("(");
      printTree(t->left);
      printf(" %c ",t->t.symbol);
      printTree(t->right);
      printf(")");
      break;
    }
    break;
  case Identifier:
    printf("%s", t->t.identifier);
    break;
  }
}


/* Complexity function adapted from:
 https://www.geeksforgeeks.org/write-a-c-program-to-find-the-maximum-depth-or-height-of-a-tree/ */

/* Finds the complexity of the tree by finding the maximum depth */
int complexityTree(FormTree t) { 
   if (t == NULL) { 
       return 0; 
   }
   else { 
       /* compute the depth of each subtree */
		int leftDepth = complexityTree(t->left); 
		int rightDepth = complexityTree(t->right); 
  
       /* use the larger one */
		if (leftDepth > rightDepth) {
			return (leftDepth+1);
		} else {
			return (rightDepth+1);
		}
   }
} 
