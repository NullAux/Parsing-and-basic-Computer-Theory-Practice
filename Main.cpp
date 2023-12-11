//A project to practise some basic computer theory using C++.
// This work is based on Cohen, D. I. A. (1997) 'Introduction to Computer Theory'.
// It implements the mathematical models / machines and algorithms described by Cohen, using the conviniences of C++
// while attempting to remain accurate to the simplicity of the described models.
// This is done for the purpose of one individual's practice with these models.
// See code comments for page citations. 

//Evaluate postfix expression - p 424

#include <iostream>

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

//Convert a string of arithmatic (+ and * only) to prefix notation)
//Based on Cohen 1997:248


int main()
{


}