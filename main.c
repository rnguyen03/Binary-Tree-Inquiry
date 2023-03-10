/*******************************************************/
/*               Binary Tree Operation                 */
/*******************************************************/
/*   Code Author: Ryan Nguyen                          */
/*                                                     */
/* Description: This program takes an equation as a    */
/* commandline argument. It then displays a menu       */
/* followed by prompting the user for input. The       */
/* program will execute tasks selected by the user.    */
/* The program will stop eunning after the user tells  */
/* it to or if there isn't the correct amount of       */
/* command line arguments.                             */
/*                                                     */
/* NOTE: integers are expected to be followed by a     */
/* decimal and two more integers.                      */
/*                                                     */
/* NOTE: variables are assumed to start with 'x' and   */
/* end with a corresponding number.                    */
/*                                                     */
/* NOTE: user is to update the variable value by       */
/* typing in the format as such: x#,y.yy               */
/*                                                     */
/* Sample Input:                                       */
/*				  "\(3.00+4.00\)"                      */
/*		"\(\(\(1.56+4.26\)*\(x1-2.23\)\)/x2\)"         */
/* "\(\(3.00*\(\(7.00+1.00\)/4.00\)\)+\(7.00-5.00\)\)" */
/*                                                     */
/*******************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/*Defined structure of binary tree*/
typedef struct tree {
	char* content;
	double value;
	struct tree* left;
	struct tree* right;
}Tree;

/*Defined structure of stack*/
typedef struct stack {
	char* content;
	struct stack* next;
}Stack;

/*Defined structured of stack containting binary tree nodes*/
typedef struct nodeStack {
	Tree* node;
	struct nodeStack* next;
}NodeStack;

/*Function Prototypes*/
void displayMenu();
int getUserSelection();
Tree* newTree(char* content);
Tree* addToTree(char* data, NodeStack** popStack, Tree** treeHold);
Stack* newStack(char* data);
NodeStack* newNodeStack(Tree* tree);
void push(Stack** stack, char* x);
void pushNodeStack(NodeStack** stack, Tree* tree);
int isEmpty(Stack* stack);
int isNodeStackEmpty(NodeStack* stack);
void pop(Stack** stack, char* str);
void popNodeStack(NodeStack** stack, Tree** popped);
char* peek(Stack* stack);
Stack* expressRPN(char* expression);
Tree* stackToTree(Stack* top);
void displayTreeOption(Tree* root, int space);
void promptValue(Tree** root);
void setValue(Tree** root, char* name, double num);
void displayTree(Tree* root, int space);
void displayPrefix(Tree* root);
void displayInfix(Tree* root);
void displayPostfix(Tree* root);
void initializeVal(Tree** root);
double calculateTree(Tree* root, bool* valid);
void printEval(Tree* root);
int heightFind(Tree* root);
void printLevel(Tree* root, int treeLevel, int indent);
void displayVertTree(Tree* root);
void freeTree(Tree* root);

/*Main function that runs other functions*/
int main(int argc, char* argv[]) {
	//Check commandline arguments
	/*
	if (argc != 2) {
		printf("Insufficient quantity of commandline arguments\n");
		return -1;
	}
	*/
	char expression[50] = "\(\(3.00*\(\(7.00+1.00\)/4.00\)\)+\(7.00-5.00\)\)";

	//Declare variables and populate data structures
	Stack* mainStack = expressRPN(expression);
	Tree* mainRoot = stackToTree(mainStack);
	initializeVal(&mainRoot);
	bool exitProgram = false;
	int choice;
	
	
	//Menu responses
	do {
		choice = getUserSelection();
		switch (choice) {
		case 1:
			//Does as prompted
			printf("Displaying the binary tree\n");
			displayTreeOption(mainRoot, 0);
			break;

		case 2:
			//Does as prompted
			printf("Printing expression in order\n");
			displayInfix(mainRoot);
			break;

		case 3:
			//Does as prompted
			printf("Printing expression in pre-order\n");
			displayPrefix(mainRoot);
			break;

		case 4:
			//Does as prompted
			printf("Printing expression in post-order\n");
			displayPostfix(mainRoot);
			break;

		case 5:
			//Does as prompted
			printf("Updating the variables.\n");
			promptValue(&mainRoot);
			break;

		case 6:
			//Does as prompted
			printf("Calculating the expression.\n");
			printEval(mainRoot);
			break;

		case 7:
			//Does as prompted
			exitProgram = true;
			printf("\nQuitting program.\n");
			freeTree(mainRoot);
			break;

		default:
			//Notify user of misinput
			printf("Invalid Input!\n");
		}
	} while (!exitProgram);

	return 0;
}

/*Display the menu*/
void displayMenu() {
	printf("\n=======================================\n");
	printf("1. Display current binary tree.\n");
	printf("2. Print expression in order.\n");
	printf("3. Print expression pre-order.\n");
	printf("4. Print expression post-order.\n");
	printf("5. Update expression.\n");
	printf("6. Calculate Expression.\n");
	printf("7. Quit program.\n");
	printf("Please enter your choice (1-7) ");
}

/*Handle menu selection*/
int getUserSelection() {
	bool exit = false;
	int choice = 0;
	char inputStr[256];

	do {
		displayMenu();
		scanf("%s", inputStr);

		//Convert user choice to integer so it can be validated
		choice = atoi(inputStr);

		if ((choice < 1) || (choice > 7)) {
			printf("Please enter valid choice between 1-7\n\n\n");
			choice = 0;
		}
		else {
			exit = true;
		}
	} while (!exit);
	printf("=======================================\n");
	return choice;
}

/*Create new tree node*/
Tree* newTree(char* data) {
	Tree* tree = (Tree*)malloc(sizeof(Tree));
	tree->value = 0.00;
	tree->left = NULL;
	tree->right = NULL;
	tree->content = (char*)malloc(sizeof(char) * (strlen(data) + 1));
	strcpy(tree->content, data);
	return tree;
}

/*Creates a Treenode using existing treenodes as childern*/
Tree* addToTree(char* data, NodeStack** popStack, Tree** treeHold) {
	//Allocate memory
	Tree* newTree = (Tree*)malloc(sizeof(Tree));
	newTree->content = (char*)malloc(sizeof(char) * (strlen(data) + 1));
	//Copy data
	strcpy(newTree->content, data);
	//Initialize attributes
	newTree->value = 0.00;
	//Pop the tree node stack and set the element to the left
	popNodeStack(popStack, treeHold);
	newTree->left = *treeHold;
	//Pop the tree node stack and set the element to the right
	popNodeStack(popStack, treeHold);
	newTree->right = *treeHold;
	return newTree;
}

/*Initialize regular stack node*/
Stack* newStack(char* data) {
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	//Store string in stack node
	stack->content = (char*)malloc(sizeof(char) * (strlen(data) + 1));
	strcpy(stack->content, data);
	stack->next = NULL;
	return stack;
}

/*Initialize stack of tree nodes node*/
NodeStack* newNodeStack(Tree* tree) {
	NodeStack* newNode = (NodeStack*)malloc(sizeof(NodeStack));
	//Store Tree node in Stack of tree ndoes container
	newNode->node = tree;
	newNode->next = NULL;
	return newNode;
}

/*Push node to regular stack*/
void push(Stack** top, char* x) {
	//Create new stack node
	Stack* new = newStack(x);
	//Set the new node's next pointer to the old top
	new->next = *top;
	//Set the new node as the top
	*top = new;
}

/*Push node containing binary tree node to stack of tree nodes*/
void pushNodeStack(NodeStack** top, Tree* tree) {
	//Create new stack node
	NodeStack* newNode = newNodeStack(tree);
	//Set the new node's next pointer to the old top
	newNode->next = *top;
	//Set the new node as the top
	*top = newNode;
}

/*Checks if the stack is empty*/
int isEmpty(Stack* stack) {
	return !stack;
}

/*Checks if the node stack is empty*/
int isNodeStackEmpty(NodeStack* stack) {
	return !stack;
}

/*Pops node from stack*/
void pop(Stack** stack, char* str) {
	if (isEmpty(*stack)) {
		return;
	}
	//Set temp node to current top of stack
	Stack* temp = *stack;
	//Set current top to the node below
	*stack = (*stack)->next;
	//Copy data of popped node
	strcpy(str, temp->content);
	//Free temp node
	free(temp->content);
	free(temp);
}

/*Push binary tree node to specific stack*/
void popNodeStack(NodeStack** stack, Tree** popped) {
	if (isNodeStackEmpty(*stack)) {
		return;
	}
	//Set temp node to current top of stack
	NodeStack* temp = *stack;
	//Set current top to the node below
	*stack = (*stack)->next;
	//Copy data of popped node
	*popped = temp->node;
	//Free temp node
	free(temp);
}

/*Peeks top node from regular stack*/
char* peek(Stack* stack) {
	if (isEmpty(stack)) {
		return NULL;
	}
	return stack->content;
}

/*Expresses the equation in RPN using the Shunting Yard Algorithm*/
/*NOTE: integers are expected to be followed by a decimal and two more integers*/
/*NOTE: variables are assumed to start with 'x' and end with an int*/
Stack* expressRPN(char* expression) {
	Stack* numStack = NULL;
	Stack * opStack = NULL;
	char* str = (char*)malloc(sizeof(char) * 5);

	for (int i = 0; i < strlen(expression); i++) {
		//if it's a number
		if (48 <= (int)expression[i] && (int)expression[i] <= 58){

			//Gather the decimal and two ints
			char number[5] = { expression[i], expression[i + 1], expression[i + 2], expression[i + 3] };

			//Push the number into number stack
			push(&numStack, number);
			i += 3;
		}
		else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/' || expression[i] == '(') {
			char str[2] = "\0";
			str[0] = expression[i];

			//Store conditional chars in operator stack
			push(&opStack, str);
		}
		else if (expression[i] == ')') {
			//While the top of the operator stack isn't an opening bracket push the top into the other stack
			while (strcmp(peek(opStack), "(") != 0) {
				pop(&opStack, str);
				push(&numStack, str);
			}
			//Discard the opening bracket
			pop(&opStack, str);
		}
		else if (expression[i] == 'x') {
			//Push variable in numstack
			char variable[3] = { expression[i], expression[i + 1] };
			push(&numStack, variable);
			i++;
		}
	}
	free(str);
	return numStack;
}

/*Reverses the order of the stack*/
void reverseStack(Stack** top) {
	Stack* temp = NULL;
	char* str = (char*)malloc(sizeof(char) * 5);
	while (*top != NULL) {
		pop(top, str);
		push(&temp, str);
	}
	*top = temp;
	free(str);
}

/*Converts a stack into a binary tree*/
Tree* stackToTree(Stack* top) {
	//Initialize variables
	NodeStack* temp = NULL;
	reverseStack(&top);
	char* popped = (char*)malloc(sizeof(char) * 5);
	Tree* popNode = NULL;
	
	//Loop until the "top" is empty
	while (top != NULL) {
		if (peek(top) != NULL) {
			pop(&top, popped);
			//Pop the next two elements and make them children of the operator
			if (popped[0] == '*' || popped[0] == '+' || popped[0] == '-' || popped[0] == '/') {
				//Create branch and put it into the stack
				pushNodeStack(&temp, addToTree(popped, &temp, &popNode));
			}
			else {
				//Push the number or variable into the stack
				pushNodeStack(&temp, newTree(popped));
			}
		}
	}
	//Pop the last node which is the root
	popNodeStack(&temp, &popNode);
	free(popped);
	return popNode;
}

void displayTreeOption(Tree* root, int space) {
	int treeChoice = -1;
	printf("Vertical(1) or Horizontal(2)?");
	scanf("%d", &treeChoice);
	if (treeChoice == 1) {
		displayVertTree(root);
	}
	else if (treeChoice == 2) {
		displayTree(root, space);
	}
	else {
		printf("Invalid Input");
	}
}

/*Displays the binary tree from left to right*/
void displayTree(Tree* root, int space) {
	//Base Case
	if (root == NULL){
		return;
	}
	space += 10;

	//Branch right
	displayTree(root->right, space);

	printf("\n");

	//Print corresponding space
	for (int i = 10; i < space; i++){
		printf(" ");
	}

	//Print content
	if (root->content[0] == 'x') {
		printf("%s", root->content);
		printf(":%.2f\n", root->value);
	}
	else {
		printf("%s\n", root->content);
	}

	//Branch left
	displayTree(root->left, space);
}

/*Prompts user for variable value*/
void promptValue(Tree** root) {
	char info[8];
	char* name = { " " };
	char* val = { " " };
	double value = 0.0;
	bool valid = false;

	printf("Enter variable name and value (Ex. x1,3.21): \n");
	//Loop until valid input
	do {
		scanf("%s", info);
		name = strtok(info, ",");
		val = strtok(NULL, ",");
			
		if (name[0] != 'x' || val == NULL || strlen(name) != 2) {
			printf("Invalid Input!\n");
		}
		else{
			value = atof(val);
			valid = true;
		}
	} while (!valid);
	
	//Call function that sets the values
	setValue(root, name, value);
}

/*Recurssively searches for the specified variable and sets the value*/
void setValue(Tree** root, char* name, double num) {
	//Base Case
	if (*root == NULL) {
		return;
	}
	//If the variable is found, set the value
	if (strcmp((*root)->content, name) == 0) {
		(*root)->value = num;
	}
	//Branch left
	setValue(&(*root)->left, name, num);
	//Branch right
	setValue(&(*root)->right, name, num);
}

/*Displays equation in prefix order*/
void displayPrefix(Tree* root) {
	//Base Case
	if (root == NULL) {
		return;
	}
	//Print content
	if (root->content[0] == 'x') {
		printf("%s:%.2f ", root->content, root->value);
	}
	else {
		printf("%s ", root->content);
	}
	//Branch right
	displayPrefix(root->right);
	//Branch left
	displayPrefix(root->left);
}

/*Displays equation in infix order*/
void displayInfix(Tree* root) {
	//Base Case
	if (root == NULL) {
		return;
	}
	//Print open bracket
	if (root->content[0] == '+' || root->content[0] == '-' || root->content[0] == '*' || root->content[0] == '/') {
		printf("(");
	}
	//Branch right
	displayInfix(root->right);
	//Display content
	if (root->content[0] == 'x') {
		printf("%s:%.2f ", root->content, root->value);
	}
	else {
		printf("%s ", root->content);
	}
	//Branch left
	displayInfix(root->left);
	//Print close bracket
	if (root->content[0] == '+' || root->content[0] == '-' || root->content[0] == '*' || root->content[0] == '/') {
		printf(")");
	}
}

/*Displays equation in postfix order*/
void displayPostfix(Tree* root) {
	//Base case
	if (root == NULL) {
		return;
	}
	//Branch right
	displayPostfix(root->right);
	//Branch left
	displayPostfix(root->left);
	//Print content
	if (root->content[0] == 'x') {
		printf("%s:%.2f ", root->content, root->value);
	}
	else {
		printf("%s ", root->content);
	}
}

/*Sets numbers/variable value to corresponding char* */
void initializeVal(Tree** root) {
	//Base Case
	if (*root == NULL) {
		return;
	}
	//if content is not an operator, set the corresponding value
	if ((*root)->content[0] != '+' && (*root)->content[0] != '-' && (*root)->content[0] != '*' && (*root)->content[0] != '/' && (*root)->content[0] != 'x') {
		(*root)->value = atof((*root)->content);
	}
	//Branch right
	initializeVal(&(*root)->right);
	//Branch left
	initializeVal(&(*root)->left);
}

/*Calculates binary tree*/
double calculateTree(Tree* root, bool* valid) {
	//Base Case
	if (root == NULL) {
		return 0;
	}
	//Leaf Case
	if (root->left == NULL && root->right == NULL){
		return root->value;
	}

	//Grab the values
	double leftVal = calculateTree(root->left, valid);
	double rightVal = calculateTree(root->right, valid);

	//Perform corresponding operation
	if (root->content[0] == '+') {
		return rightVal + leftVal;
	}
	else if (root->content[0] == '-') {
		return rightVal - leftVal;
	}
	else if (root->content[0] == '*') {
		return rightVal * leftVal;
	}
	else {
		if (leftVal != 0) {
			return rightVal / leftVal;
		}
		else {
			//set bool to false if divide by 0
			*valid = false;
			return -1;
		}
	}
}

/*Prints binary tree calculation*/
void printEval(Tree* root) {
	bool valid = true;
	double ans = calculateTree(root, &valid);
	
	//Print if no divide by 0 case otherwise notify the user
	if (valid == true){
		printf("The expression is equal to %.2f\n", ans);
	}
	else {
		printf("This expression divides by 0, ensure the equation is valid and all variables are updated.\n");
	}
}


/*Finds the longest path from the root to the leaves*/
int heightFind(Tree* root) {
	if (root == NULL)
		return 0;
	else {
		int left = heightFind(root->left);
		int right = heightFind(root->right);

		if (left > right)
			return (left + 1);
		else
			return (right + 1);
	}
}

/*Prints the data at a given level*/
void printLevel(Tree* root, int treeLevel, int indent) {
	if (root == NULL) {
		printf("     ");
		return;
	}
	if (treeLevel == 1) {
		printf("%*s%s", indent, "", root->content);
		if (root->content[0] == 'x') {
			printf(":%.2f", root->value);
		}
	}
	else if (treeLevel > 1) {
		printLevel(root->left, treeLevel - 1, indent);
		printLevel(root->right, treeLevel - 1, indent);
	}
}

//TODO better spacing
/*Displays a binary tree as is*/
void displayVertTree(Tree* root) {
	int h = heightFind(root);
	int nextTerm = 1;
	int t1;
	int t2 = 1;
	for (int i = 1; i <= h; i++) {
		t1 = t2;
		t2 = nextTerm;
		nextTerm = t2 + t1;
		//if (i >= 4) {
		//	nextTerm += 3;
		//}

		printf("Tree Level %d:", i);
		printLevel(root, i, (100 / nextTerm));
		printf("\n");
	}
}

/*Free all Treenodes*/
void freeTree(Tree* root) {
	//Base Case
	if (root == NULL) {
		return;
	}
	//Branch Left
	freeTree(root->left);
	//Free Node content
	free(root->content);
	//Branch Right
	freeTree(root->right);
	//Free current Node
	free(root);
}