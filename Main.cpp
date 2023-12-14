//A project to practise some basic computer theory using C++.
// This work is based on Cohen, D. I. A. (1997) 'Introduction to Computer Theory'.
// It implements the mathematical models / machines and algorithms described by Cohen, using the conviniences of C++
// while attempting to remain accurate to the simplicity of the described models.
// This is done for the purpose of one individual's practice with these models.
// See code comments for page citations. 


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

//Turing Machines
//Cohen 1997:456,456 - A turning machine consists of Alphabet of input letters, tape of cells incl blanks,
//a tape head beginning at first cell, alphabet of characters on the tape, finite num of states (one start, one+ halt)
//and a program, which dictates from a state and read letter > change state, print to tape, and move tape head (Letter, letter, direction)
//What follows are a number of TMs, taken from examples by Cohen
//For all, the input alphabet will be a and b (used to write unary numbers) and the tape alphabet will be a, b, and blank (ie erase, using a c++ method)
//Each TM will be written as a number of 'rows', each representing the program information (see Cohen 1997:548 on encoding TMs)
//State 1 is start and State 2 is halt
//The result is a string of a's and b's, representing a number in unary

struct TMRow
{
	int from;
	int to;
	char read;
	char write;
	char move;

	TMRow(int iFrom, int iTo, char iRead, char iWrite, char iMove) : 
		from(iFrom), to(iTo), read(iRead), write(iWrite), move(iMove) {}

	TMRow(int iTo) : to(iTo) {};

};

//Addition. Accepts a string in form a*ba*, meaning some numbers of a's (representing two unary numbers) seperated by one b
//See Cohen 1997:595
std::string ADDER(std::string input)
{
	//Input alphabet is a and b. Tape alphabet is a, b, and blank (erase, represented by #). The tape is the input string.
	int tapehead = 0;//tapehead begins at 'first cell'. Since the tape here is an std::string, which begins indexing at 0, tapehead begins at 0
	int state = 1;
	input.append("#", 1);//Adding a blank to the tape end

	//Program (in full)
	struct programInstructions {
		TMRow One = TMRow(1, 1, 'a', 'a', 'R');
		TMRow Two = TMRow(1, 2, 'b', 'a', 'R');
		TMRow Three = TMRow(2, 2, 'a', 'a', 'R');
		TMRow Four = TMRow(2, 3, '#', '#', 'L');
		TMRow Five = TMRow(3, 4, 'a', '#', 'R');
		//State 2 should be the halt state, but here it's state 4 (the state after executing instruction Five). This
		//function will manually check this, but it's something to fix for a generalised TM in the future.
	};
	programInstructions programInstructions;

	//Program (in a std::vector, for convinient access)
	std::vector<TMRow> program =
	{
		programInstructions.One,
		programInstructions.Two,
		programInstructions.Three,
		programInstructions.Four,
		programInstructions.Five,
	};

	//Executing the program
	bool haltProgram = false;
	while (!haltProgram)
	{
		//Find the next instruction
		TMRow nextInstruction(0);//Initializes 'nextInstruction' with 'to' set to 0 ie an invalid state.
		//This will be overwritten if a valid instruction from the state+read is found. If it isn't,
		//then it means the TM would crash at this point, and likewise this method should terminate.
		for (TMRow instruction : program)
		{
			//If correct state and and correct read:
			if (instruction.from == state && input[tapehead] == instruction.read)//check 'input[tapehead]' plays nice
			{
				nextInstruction = instruction;
			}
		}
		//If no valid instruction is found, the program will terminate here.
		//This is analogous to a TM crashing when reading an input which corresponds to no edge from the current state.
		if (nextInstruction.to == 0)
		{
			std::cout << "Bad string input - TM has crashed" << std::endl;
			return input;//Return the working string to see where it crashed
		}

		//Write to tape, change state, move tapehead
		input[tapehead] = nextInstruction.write;
		state = nextInstruction.to;
		switch (nextInstruction.move)
		{
		case 'R':
			tapehead += 1;
			break;
		case 'L':
			tapehead -= 1;
			//In C++, this ^ can cause an out of bounds error. This is analogous to a TM crashing if the tapehead goes over the left edge, ie bad input.
			//C++ will also cause an out of bounds error if it goes over the 'right' end. This wouldn't happen on a TM, since it has an infinite series of blanks
			//at the end. It *shouldn't* come up in this ADDER, but it's worth thinking about in more complicated TMs.
			break;
		}

		//Check if it has reached the halt state. As mentioned above, by convention this is state 2, but for convinience here it is 4.
		if (state == 4)
		{
			//Program has succesfully terminated. The result is the contents of the tapehead (here input string) (a number written in unary, plus some blanks #)
			return input;
		}


	}

}

//Encode a TM (program) into a string
//This works by translating each row of instructions into a standard format (code word language, CWL), which can be concatonated and later decoded
//See Cohen 1997:545
std::string EncodeTM(std::vector<TMRow> program)
{
	std::string output;

	for (TMRow instruction : program)
	{
		//Add 'from' and 'to', in the form of unary numbers (a's) ended by b's
		output.append(instruction.from, 'a');
		output.append("b", 1);
		output.append(instruction.to, 'a');
		output.append("b", 1);

		//Add 'read' and 'write'
		//Each are given a 2 character code - a = aa, b = ab, blank (#) = ba
		if (instruction.read == 'a') { output.append("aa", 1); }
		else if (instruction.read == 'b') { output.append("ab", 1); }
		else if (instruction.read == '#') { output.append("ba", 1); }

		if (instruction.write == 'a') { output.append("aa", 1); }
		else if (instruction.write == 'b') { output.append("ab", 1); }
		else if (instruction.write == '#') { output.append("ba", 1); }

		//Add 'move'
		//L becomes a, R becomes b
		if (instruction.move == 'L') { output.append("a", 1); }
		if (instruction.move == 'R') { output.append("b", 1); }

	}

	return output;
}



int main()
{
	//Convert to prefix - outputs first pass
	std::cout << ConvertToPrefixNotation("1+2+3+4") << std::endl;

	//ADDER with a valid string
	std::cout << ADDER(std::string("aaabaa")) << std::endl;

	//ADDER with an invalid string
	std::cout << ADDER(std::string("abaabaa")) << std::endl;

	//EncodeTM, using the example Cohen gives (Cohen 1997:546)
	std::vector<TMRow> exampleToEncode = { TMRow(6,2,'b','a','L') };
	std::cout << "Encoded TM: " << EncodeTM(exampleToEncode) << std::endl;

	//Can revisit - create a decode TM function (desc by Cohen on p 547/548), a TM that makes use of this to accept another
	//TM in CWL and an input string to run the input on said TM
}