//A project to practise some basic computer theory using C++.
// This work is based on Cohen, D. I. A. (1997) 'Introduction to Computer Theory'.
// It implements the mathematical models / machines and algorithms described by Cohen, using the conviniences of C++
// while attempting to remain accurate to the simplicity of the described models.
// This is done for the purpose of one individual's practice with these models.
// See code comments for page citations. 

//Evaluate postfix expression - p 424

#include <iostream>
#include <string>
#include <vector>


//A simple stack for our Pushdown Automata to use.
//For the sake of this practice, the stack will be limited to 500 characters
class Stack {

		char* stack = new char[500];
		int stackPtr = 0;

public:

	Stack() {
		for (int i = 0; i < 500; i++) { stack[i] = '#'; }
	}

	~Stack()
	{
		delete[] stack;
	}

	char Pop()
	{
		//No leftbound checking - will cause a c++ crash next time pop or push is called
		char result = stack[stackPtr - 1]; 
		stack[stackPtr] = '#';
		stackPtr -= 1;
		return result;
	}

	void Push(char pushItem)
	{
		//No rightbound checking - will cause a c++ crash on overflow
		stack[stackPtr] = pushItem;
		stackPtr += 1;
		return;
	}

	void Print()
	{
		for (int i = 0; i < 500; i++)
		{
			//if (stack[i] == '#')
				//{ break; }
			std::cout << stack[i];
		}
	}

};

//Convert a string of arithmatic (+ only) partially to prefix notation
//Based on Cohen 1997:248
//This only does one pass on the string, creating a string which is partially in prefix and partially in infix. Cohen doesn't cover
//how to work with such a mixed string, and my beginner compsci brain can't figure it out. Consider revisiting in future.

class Node {
public:
	char content;
	std::string LString;
	std::string RString;

	//# represents the termination of a branch (ie no nodes beneath, no operators in the inputstring)
	Node(char icontent, std::string iLString = "#", std::string iRString = "#")
	{
		content = icontent;
		LString = iLString;
		RString = iRString;
	}
};

//Recursive method to create tree from string
void CreateTreeFromString(std::string inputString, std::vector<Node>& Tree)
{

	//Scan for +
	for (int i = 0; i < inputString.length(); i++)
	{
		if (inputString.compare(i, 1, "+") == 0)
		{
			Node nodeToAdd = Node('+', inputString.substr(0, i), inputString.substr(i + 1));
			Tree.push_back(nodeToAdd);
			CreateTreeFromString(nodeToAdd.LString, Tree);
			CreateTreeFromString(nodeToAdd.RString, Tree);
			return;
		}

	}

	//No operator - terminal number
	//Note this assumes 1) No operator = valid number 2) terminal is one digit
	Node nodeToAdd = Node(inputString.at(0));
	Tree.push_back(nodeToAdd);
	return;
}

std::string ConvertToPrefixNotation(std::string inputString)
{
	bool inPrefixNotation = false;


	while (!inPrefixNotation)
	{
		//Check if in prefix notation
		//Assumes legal input (ie numbers and +s and/or *s, no white space)
		//(Note this is technically pointless, since the goal isn't to produce a string in fully prefix. If later parts could do this,
		//the method would be used recursively until this part spat out the complete string.)
		bool validString = true;
		bool beginsWithSign = (inputString[0] == '+' || inputString.compare(0, 1, "*") == 0);

		if (beginsWithSign)
		{
			bool encounteredNumber = false;
			for (int i = 0; i < inputString.length(); i++)
			{
				if (encounteredNumber)
				{
					if (inputString.compare(i, 1, "+") == 0 || inputString.compare(i, 1, "*") == 0)
					{
						validString = false;
						break;
					}
				}

				else if (inputString.compare(i, 1, "+") != 0 && inputString.compare(i, 1, "*") != 0)
				{
					encounteredNumber = true;
				}
			}
		}

		else { validString = false; }

		if (validString) { return inputString; }

		
		//Create tree from string
		std::vector<Node> Tree;
		CreateTreeFromString(inputString, Tree);

		//Create (new) string from tree
		//This method works since the tree was created along the left edge, so Tree is already in the create order
		std::string newString;
		for (int i = 0; i < Tree.size(); i++)
		{
			newString.append(1, Tree[i].content);
		}

		//Note - if the prefix checking worked, this should be outside this loop
		return newString;
	}
}



int main()
{


	//Fix comparisons not working (not being able to use == on element from std::string) - use string.compare instead (returns 0 on true)
	std::cout << ConvertToPrefixNotation("1+2+3+4") << std::endl;
	//std::cout << ConvertToPrefixNotation("1+ 1") << std::endl;
	//std::cout << ConvertToPrefixNotation("++1 * ") << std::endl;


}