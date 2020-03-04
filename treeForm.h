/* treeForm.h for assignment 4 on logical formulas */

#ifndef TREEFORM_H
#define TREEFORM_H

#include "scanner.h"

/* A FormTree is a binary tree with nodes containing tokens. */

typedef struct FormTreeNode *FormTree;

typedef struct FormTreeNode {
  TokenType tt;
  Token t;
  FormTree left;
  FormTree right;
} FormTreeNode;

FormTree newFormTreeNode(TokenType tt, Token t, FormTree tL, FormTree tR);

int treeIdentifier(List *lp, FormTree *t);
int treeAtom(List *lp, FormTree *t);
int treeLiteral(List *lp, FormTree *t);
int treeFormula(List *lp, FormTree *t);

int treeFormulaOr (List *lp, FormTree *t);

void treeFormulas();

void freeTree(FormTree t);

void printTree(FormTree t);

#endif
