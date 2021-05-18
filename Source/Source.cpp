/*
Sean Hoversten
CS 4100-002
Fall 2020
Instructor : Albert Brouillette
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <limits>

// namespaces used in this file
using std::cout;
using std::endl;
using std::setfill;
using std::setw;
using std::vector;
using std::string;

class SymbolTable;
class QuadTable;
class ReserveTable;
class Interpreter;

/*** START OF ABSTRACT DATA TYPES ***/

// support function for abstract data type output methods
string LimitOutput(string str, int amount)
{
	string ret = "";

	for (int i = 0; i < str.length() && i < amount; i++)
		ret += str[i];

	return ret;
}

/* Stucture that contains the data of a symbol,
to be used with Symbol Table and Interpreter.

Kind | -1 = NUL, 0 = label, 1 = variable/identifyer, 2 = constant
Dataypes | -1 = nul, 0 = int, 1 = float, 2 = string
*/
struct SymbolData
{
	string symbol;
	int kind = -1;
	int datatype = -1;
	int sInt = -1;
	double sDbl = -1;
	string str = "_#_";
};

/* Structure that contains quad data,
to be used in QuadTable and Interpreter
*/
struct Quad
{
	int opcode = 0;
	int op1 = 0;
	int op2 = 0;
	int op3 = 0;
};

/* Class definition of the Reserve Table, an object
that holds a list of names and associated codes.
For use by QuadTable and Interpreter
*/
class ReserveTable
{
private:
	vector<std::string> m_names;	// vector containing the names of the table
	vector<int> m_codes;			// vector containing the codes of the table
	int m_size;						// keeps track of the amount of names and codes in the table
public:
	// Constructor (initializer)
	ReserveTable()
	{
		m_size = 0;
	}

	// Destructor
	~ReserveTable() {}

	/* Adds a string and integer code to the reserve table in
	 there respective vectors and returns the index it was stored */
	int Add(std::string name, int code)
	{
		m_names.push_back(name);
		m_codes.push_back(code);
		return m_size++;
	}

	/* Returns the code of the name asociated with it in the table,
	 returns -1 if failure to the contents */
	int LookupName(std::string name)
	{
		int ret = -1;
		bool searching = true;

		for (int i = 0; searching && i < m_size; i++)
		{
			if (name == m_names[i])
			{
				searching = false;
				ret = m_codes[i];
			}
		}

		return ret;
	}

	/* Searches table for the name associated with the code and returns it,
	 returns "DNE", for "Does not exist", if not in the table */
	string LookupCode(int code)
	{
		string ret = "DNE";
		bool searching = true;

		for (int i = 0; searching && i < m_size; i++)
		{
			if (code == m_codes[i])
			{
				searching = false;
				ret = m_names[i];
			}
		}

		return ret;
	}

	/* Prints the current contents of the Reserve Table in a simple format */
	void PrintReserveTable()
	{
		std::cout << "|   Name   | Code |" << std::endl;
		std::cout << "|----------|------|" << std::endl;

		for (int i = 0; i < m_size; i++)
		{
			std::cout << "|" << setw(10) << m_names[i] << "|" <<
				setw(6) << m_codes[i] << "|" << std::endl;
		}

	}
};

/*	Class definitions for SymbolTable,
A table containing symbol data from the source code with
all of the associated data with them
*/
class SymbolTable
{
private:
	vector<SymbolData> m_SymbolTable;
public:
	/* Constructor to instanciate the SymbolTable */
	SymbolTable() {}

	/* Destructor */
	~SymbolTable() {}

	/* Add symbol with integer data type */
	int AddSymbol(std::string iSymbol, int iKind, int iValue)
	{
		int index = 0;

		m_SymbolTable.push_back(SymbolData());

		index = m_SymbolTable.size() - 1;

		m_SymbolTable[index].symbol = iSymbol;
		m_SymbolTable[index].kind = iKind;
		m_SymbolTable[index].sInt = iValue;
		m_SymbolTable[index].datatype = 0;

		return index;
	}

	// Add symbol with floating point data type
	int AddSymbol(std::string iSymbol, int iKind, double iValue)
	{
		int index = 0;

		m_SymbolTable.push_back(SymbolData());

		index = m_SymbolTable.size() - 1;

		m_SymbolTable[index].symbol = iSymbol;
		m_SymbolTable[index].kind = iKind;
		m_SymbolTable[index].sDbl = iValue;
		m_SymbolTable[index].datatype = 1;

		return index;
	}

	// Add symbol with string data type
	int AddSymbol(std::string iSymbol, int iKind, std::string iValue)
	{
		int index = 0;

		m_SymbolTable.push_back(SymbolData());

		index = m_SymbolTable.size() - 1;

		m_SymbolTable[index].symbol = iSymbol;
		m_SymbolTable[index].kind = iKind;
		m_SymbolTable[index].str = iValue;
		m_SymbolTable[index].datatype = 2;

		return index;
	}

	// Returns index position of the symbol being looked up
	int LookupSymbol(std::string iSymbol)
	{
		int ret = -1;
		int size = m_SymbolTable.size();
		bool searching = true;

		for (int i = 0; searching && i < size; i++)
		{
			if (m_SymbolTable[i].symbol == iSymbol)
			{
				ret = i;
				searching = false;
			}
		}

		return ret;
	}

	// returns a copy of the symbol's data from the symbol table
	SymbolData GetSymbol(int index)
	{
		SymbolData data;

		if (index < m_SymbolTable.size())
		{
			data.symbol = m_SymbolTable[index].symbol;
			data.kind = m_SymbolTable[index].kind;
			data.datatype = m_SymbolTable[index].datatype;
			data.sInt = m_SymbolTable[index].sInt;
			data.sDbl = m_SymbolTable[index].sDbl;
			data.str = m_SymbolTable[index].str;
		}
		
		return data;
	}

	// Updates symbol with integer data type
	void UpdateSymbol(int index, int kind, int value)
	{
		m_SymbolTable[index].kind = kind;
		m_SymbolTable[index].sInt = value;
		m_SymbolTable[index].datatype = 0;
	}

	// Updates symbol with float data type
	void UpdateSymbol(int index, int kind, double value)
	{
		m_SymbolTable[index].kind = kind;
		m_SymbolTable[index].sDbl = value;
		m_SymbolTable[index].datatype = 1;
	}

	// Updates symbol with string data type
	void UpdateSymbol(int index, int kind, std::string value)
	{
		m_SymbolTable[index].kind = kind;
		m_SymbolTable[index].str = value;
		m_SymbolTable[index].datatype = 2;
	}

	// Prints the utilized rows of the symbol table in neat tabular format
	// showing only the value field which is active for that row
	void PrintSymbolTable()
	{
		int size = m_SymbolTable.size();

		if (size == 0)
		{
			cout << "No symbols set in table" << endl;
		}
		else
		{
			cout << "|Index|    Symbol     |Kind|DataType| Integer  |  sDbl  |  String  |" << endl;

			for (int i = 0; i < size; i++)
			{
				cout << "|" << setw(5) << i << "|" << setw(15) << LimitOutput(m_SymbolTable[i].symbol, 15) << "|" <<
					setw(4) << m_SymbolTable[i].kind << "|" << setw(8) << m_SymbolTable[i].datatype << "|";

				if (m_SymbolTable[i].datatype == 0)
					cout << setw(10) << m_SymbolTable[i].sInt << "|" << setw(9) << "-" << "|" << setw(11) << m_SymbolTable[i].str << "|" << endl;
				else if (m_SymbolTable[i].datatype == 1)
					cout << setw(11) << "-|" << setw(8) << m_SymbolTable[i].sDbl << "|" << setw(11) << "|" << endl;
				else if (m_SymbolTable[i].datatype == 2)
					cout << setw(11) << "-|" << setw(9) << "-|" << setw(10) << m_SymbolTable[i].str << " | " << endl;
				else
					cout << "***ERROR***" << endl;
			}
		}
		cout << endl;
	}
};

/*	Class definition for QuadTable.  A table that holds a vector of Quad structures
holding the integer values for opcode, op1, op2, and op3, variables to be used in the
interpreter for code execution.
*/
class QuadTable
{
private:
	vector<Quad> m_QuadRows;	// vector stucture containing all the Quads
	int m_nextQuad;				// integer value showing the position of the next quad slot
	int m_maxQuad;				// the maximum amount of quads that can be contained in the table
	ReserveTable* m_mnemonicsTable;	// pointer to the Reserve Table holding the mnemonics for the Quad Codes
public:
	/* Constructor that sets the max amount of quads for the table */
	QuadTable(int amount)
	{
		m_maxQuad = amount;
		m_nextQuad = 0;
		m_mnemonicsTable = 0;
	}

	/* Destuctor */
	~QuadTable() {}

	/* create new Quad and push it into the vector */
	void AddQuad(int iOpcode, int iOp1, int iOp2, int iOp3)
	{
		if (m_nextQuad < m_maxQuad)
		{
			m_QuadRows.push_back(Quad());
			m_QuadRows[m_nextQuad].opcode = iOpcode;
			m_QuadRows[m_nextQuad].op1 = iOp1;
			m_QuadRows[m_nextQuad].op2 = iOp2;
			m_QuadRows[m_nextQuad].op3 = iOp3;
			m_nextQuad++;
		}
		else
			cout << "Quad Limit Reached, could not add quad" << endl;
	}

	// Returns the int index of the next open slot in the QuadTable
	int NextQuad()
	{
		return m_nextQuad;
	}

	// Returns the max amount of quads the instance a quad table will take
	int MaxQuad()
	{
		return m_maxQuad;
	}

	/* Returns a pointer for the Quad located at the index
	 error messages are outputed if the index exceeds the
	 set amount of quads or maximum amount of quads */
	Quad GetQuad(int index)
	{
		Quad retQuad;
		if (index > m_maxQuad)
			cout << "Index exceeds the maximum amount of quads set for this table" << endl;
		else if (index > m_nextQuad)
			cout << "Index exceeds the last quad that was set: does not exist" << endl;
		else
			retQuad = m_QuadRows[index];
		return retQuad;
	}

	/* Sets new op values to the specified Quad via an index */
	void SetQuad(int index, int opcode, int op1, int op2, int op3)
	{
		if (index < m_maxQuad && index >= 0)
		{
			int og = m_QuadRows[index].opcode;	// debug
			m_QuadRows[index].opcode = opcode;
			m_QuadRows[index].op1 = op1;
			m_QuadRows[index].op2 = op2;
			m_QuadRows[index].op3 = op3;
		}
		else
			cout << "Error, index out of bounds" << endl;
	}


	/* Sets the private pointer for mnemonic table for the GetMnomonic method*/
	void SetMnomonic(ReserveTable& table)
	{
		m_mnemonicsTable = &table;
	}

	/* Returns the mnemonic string associated with the opcode paramenter */
	string GetMnemonic(int opcode)
	{
		string ret = "No Mnomonic found";

		if (m_mnemonicsTable == 0)
			ret = "Mnomonic table not set";
		else
			ret = m_mnemonicsTable->LookupCode(opcode);

		return ret;
	}

	/* Prints the currently used contents of the Quad table in neat tabular format */
	void PrintQuadTable(SymbolTable st, ReserveTable qrt)
	{

		if (m_nextQuad == 0)
			cout << "No quads set in table" << endl;
		else
		{
			for (int i = 0; i < m_nextQuad; i++)
			{
				cout << "  " << setw(5) << qrt.LookupCode(m_QuadRows[i].opcode);

				if (st.GetSymbol(m_QuadRows[i].op1).symbol == "---")
					cout << " | --------------";
				else
					cout << " | " << setw(10) << LimitOutput(st.GetSymbol(m_QuadRows[i].op1).symbol, 10) << " <" << m_QuadRows[i].op1 << ">";

				if (st.GetSymbol(m_QuadRows[i].op2).symbol == "---")
					cout << " | --------------";
				else
					cout << " | " << setw(10) << st.GetSymbol(m_QuadRows[i].op2).symbol << " <" << m_QuadRows[i].op2 << ">";

				if (m_QuadRows[i].opcode >= 8 && m_QuadRows[i].opcode <= 14)
				{
					int loops = 0;

					if (m_QuadRows[i].op3 < 1000 && m_QuadRows[i].op3 >= 100)
						loops = 1;
					else if (m_QuadRows[i].op3 < 100 && m_QuadRows[i].op3 >= 10)
						loops = 2;
					else if (m_QuadRows[i].op3 < 10)
						loops = 3;

					cout << setw(10) << " | PC : ";

					for (int j = 0; j < loops; j++)
						cout << "0";

					cout << m_QuadRows[i].op3;
				}
				else
				{
					if (st.GetSymbol(m_QuadRows[i].op3).symbol == "---")
						cout << " | --------------";
					else
						cout << " | " << setw(10) << st.GetSymbol(m_QuadRows[i].op3).symbol << " <" << m_QuadRows[i].op3 << ">";
				}
				cout << endl << endl;
			}
		}
		cout << endl;
	}
};


/* Class definition for the Interpreter.
Used to read quads from the quad table and
use the op codes to change symbols in the symbol table
or retrieve data from the symbol table for other uses
*/
class Interpreter
{
private:
	int m_PC;	// program counter for the interpreter when code runs
	int m_opcode;
	int m_op1;
	int m_op2;
	int m_op3;
	int MAXQUADS;
	bool run;
	vector<string> output;

	//$INCOMPLETE
	bool ValidOpCode()
	{
		return true;
	}

	/*Returns true if the opcode is found in the reserve table,
	false otherwise
	*/
	bool CheckValidOp(ReserveTable& rt)
	{
		bool ret = false;

		if (rt.LookupCode(m_opcode) != "DNE")
			ret = true;

		return ret;
	}

public:
	// Constructor for Interpreter, sets private variables to defualt values
	Interpreter()
	{
		m_PC = 0;
		m_opcode = -1;
		m_op1 = -1;
		m_op2 = -1;
		m_op3 = -1;
		MAXQUADS = 0;
		run = false;
	}

	// Destructor
	~Interpreter() {}

	void PrintOutput()
	{
		cout << "OUTPUT" << endl << "--------" << endl;
		for (int i = 0; i < output.size(); i++)
		{
			cout << output[i] << endl;
		}
	}

	// Primary Function for the Interpreter
	void InterpretQuads(QuadTable& q, SymbolTable& s, ReserveTable& r, bool TraceOn)
	{
		// defining variables to be used
		SymbolData data1;
		SymbolData data2;
		SymbolData data3;
		bool jump = false;	// used to determine if there was a jump during an instruction
		bool print = false;
		int instrPC = 0;

		// set values of interpreter variables for start of loop process
		run = true;
		m_PC = 0;
		MAXQUADS = q.MaxQuad();

		while (run && m_PC < MAXQUADS)
		{
			m_opcode = q.GetQuad(m_PC).opcode;
			m_op1 = q.GetQuad(m_PC).op1;
			m_op2 = q.GetQuad(m_PC).op2;
			m_op3 = q.GetQuad(m_PC).op3;

			data1 = s.GetSymbol(m_op1);
			data2 = s.GetSymbol(m_op2);
			data3 = s.GetSymbol(m_op3);

			instrPC = m_PC;		// for output purposes
			jump = false;

			// $check for valid opcode
			if (CheckValidOp(r))
			{
				// opcode conditions
				if (m_opcode == 0)	// STOP: Interpreter will stop processing after this iteration
					run = false;
				else if (m_opcode == 1)	// DIV: divide integers
				{
					// confirm that the data is an integer
					if (0 == data1.datatype == data2.datatype == data3.datatype)
					{
						s.UpdateSymbol(m_op3, data3.kind, data1.sInt / data2.sInt);
					}
					else
					{
						// log error
						cout << "Process Terminated: Dividing non-integer values" << endl;
						run = false;
					}
				}
				else if (m_opcode == 2)	// MUL : Multiply values
				{
					if (data3.datatype != 2 && data1.datatype == 0 && data2.datatype == 0)
						s.UpdateSymbol(m_op3, data3.kind, data1.sInt * data2.sInt);
					else if (data3.datatype != 2 && data1.datatype == 1 && data2.datatype == 0)
						s.UpdateSymbol(m_op3, data3.kind, data1.sDbl * data2.sInt);
					else if (data3.datatype != 2 && data1.datatype == 0 && data2.datatype == 1)
						s.UpdateSymbol(m_op3, data3.kind, data1.sInt * data2.sDbl);
					else if (data3.datatype != 2 && data1.datatype == 1 && data2.datatype == 1)
						s.UpdateSymbol(m_op3, data3.kind, data1.sDbl * data2.sDbl);
					else
					{
						// log error
						cout << "Tried to put arithmetic result into a string symbol" << endl;
						run = false;
					}
				}
				else if (m_opcode == 3)	// SUB : subtract values
				{
					if (data3.datatype != 2 && data1.datatype == 0 && data2.datatype == 0)
						s.UpdateSymbol(m_op3, data3.kind, data1.sInt - data2.sInt);
					else if (data3.datatype != 2 && data1.datatype == 1 && data2.datatype == 0)
						s.UpdateSymbol(m_op3, data3.kind, data1.sDbl - data2.sInt);
					else if (data3.datatype != 2 && data1.datatype == 0 && data2.datatype == 1)
						s.UpdateSymbol(m_op3, data3.kind, data1.sInt - data2.sDbl);
					else if (data3.datatype != 2 && data1.datatype == 1 && data2.datatype == 1)
						s.UpdateSymbol(m_op3, data3.kind, data1.sDbl - data2.sDbl);
					else
					{
						// log error
						cout << "Tried to put arithmetic result into a string symbol" << endl;
						run = false;
					}
				}
				else if (m_opcode == 4)	// ADD : addition for op1 and op2, stored in op3
				{
					if (data3.datatype != 2 && data1.datatype == 0 && data2.datatype == 0)
						s.UpdateSymbol(m_op3, data3.kind, data1.sInt + data2.sInt);
					else if (data3.datatype != 2 && data1.datatype == 1 && data2.datatype == 0)
						s.UpdateSymbol(m_op3, data3.kind, data1.sDbl + data2.sInt);
					else if (data3.datatype != 2 && data1.datatype == 0 && data2.datatype == 1)
						s.UpdateSymbol(m_op3, data3.kind, data1.sInt + data2.sDbl);
					else if (data3.datatype != 2 && data1.datatype == 1 && data2.datatype == 1)
						s.UpdateSymbol(m_op3, data3.kind, data1.sDbl + data2.sDbl);
					else
					{
						// log error
						cout << "Tried to put arithmetic result into a string symbol" << endl;
						run = false;
					}
				}
				else if (m_opcode == 5)	// MOV : assign the value in op1 into op3
				{
					if (data3.datatype == 0 && data1.datatype == 0)	// assinging int values
						s.UpdateSymbol(m_op3, data3.kind, data1.sInt);
					else if (data3.datatype == 1 && data1.datatype == 1)
						s.UpdateSymbol(m_op3, data3.kind, data1.sDbl);
					else if (data3.datatype == 2 && data1.datatype == 2)
						s.UpdateSymbol(m_op3, data3.kind, data1.str);
					else
					{
						cout << "Quad attempted to assign incompatible value | op1 datatype: " << data1.datatype <<
							" | op3 datatype: " << data3.datatype << endl;
						run = false;
					}
				}
				else if (m_opcode == 6)	// STI : assign the value in op1 into op2 + offset op3
				{
					if (data3.datatype == 0)
					{
						data1 = s.GetSymbol(m_op1 + m_op3);
						s.UpdateSymbol(m_op2, data2.kind, data1.sInt);
						s.UpdateSymbol(m_op2, data2.kind, data1.sDbl);
						s.UpdateSymbol(m_op2, data2.kind, data1.str);
					}
					else
					{
						cout << "Error: Integer value needed for offset calculation" << endl;
					}
				}
				else if (m_opcode == 7)	// LDI : assign the value in op1 + offset op2, into op3
				{
					if (data2.datatype == 0)
					{
						data1 = s.GetSymbol(m_op1 + m_op2);	// find symbol from offset
						s.UpdateSymbol(m_op3, data3.kind, data1.sInt);
						s.UpdateSymbol(m_op3, data3.kind, data1.sDbl);
						s.UpdateSymbol(m_op3, data3.kind, data1.str);
					}
					else
					{
						cout << "Error: Integer value needed for offset calculation" << endl;
					}
				}
				else if (m_opcode == 8)	// BNZ : Branch Not Zero, 
				{
					if ((data1.datatype == 0 && data1.sInt != 0) || (data1.datatype == 1 && data1.sDbl != 0))
					{
						m_PC = m_op3;
						jump = true;
					}
					else if (data1.datatype == 2)
						cout << "Cannot use string to compare to zero for branch operation" << endl;
				}
				else if (m_opcode == 9)	// BNP : Branch Not Positive
				{
					if ((data1.datatype == 0 && data1.sInt <= 0) || (data1.datatype == 1 && data1.sDbl <= 0))
					{
						m_PC = m_op3;
						jump = true;
					}
					else if (data1.datatype == 2)
						cout << "Cannot use string to compare to zero for branch operation" << endl;
				}
				else if (m_opcode == 10) // BNN : Branch Not Negative
				{
					if ((data1.datatype == 0 && data1.sInt >= 0) || (data1.datatype == 1 && data1.sDbl >= 0))
					{
						m_PC = m_op3;
						jump = true;
					}
					else if (data1.datatype == 2)
						cout << "Cannot use string to compare to zero for branch operation" << endl;
				}
				else if (m_opcode == 11)	// BZ : Branch if Zero
				{
					if ((data1.datatype == 0 && data1.sInt == 0) || (data1.datatype == 1 && data1.sDbl == 0))
					{
						m_PC = m_op3;
						jump = true;
					}
					else if (data1.datatype == 2)
						cout << "Cannot use string to compare to zero for branch operation" << endl;
				}
				else if (m_opcode == 12)	// BP : Branch if Positive
				{
					if ((data1.datatype == 0 && data1.sInt > 0) || (data1.datatype == 1 && data1.sDbl > 0))
					{
						m_PC = m_op3;
						jump = true;
					}
					else if (data1.datatype == 2)
						cout << "Cannot use string to compare to zero for branch operation" << endl;
				}
				else if (m_opcode == 13)	// BN : Branch if Negative
				{
					if ((data1.datatype == 0 && data1.sInt < 0) || (data1.datatype == 1 && data1.sDbl < 0))
					{
						m_PC = m_op3;
						jump = true;
					}
					else if (data1.datatype == 2)
						cout << "Cannot use string to compare to zero for branch operation" << endl;
				}
				else if (m_opcode == 14)	// BR : Branch unconditional 
				{
					m_PC = m_op3;
					jump = true;
				}
				else if (m_opcode == 15)	// BINDR : Branch
				{
					if (data3.datatype == 0)
						m_PC = data3.sInt;
					else if (data3.datatype == 1)
						m_PC = data3.sDbl;
					else if (data3.datatype == 2)
						cout << "Need a numeric value for branching" << endl;

					jump = true;
				}
				else if (m_opcode == 16)	// PRINT : write out symbol name and value of op1
				{
					string str;
					print = true;
					
					if (data1.datatype == 0)
						str = std::to_string(data1.sInt);
					else if (data1.datatype == 1)
						str = data1.sDbl;
					else if (data1.datatype == 2)
						str = data1.str;
					else
						cout << "Error, datatype not set for of op1" << endl;

					output.push_back(str);
				}
			}
			else
			{
				cout << "Invalide opcode, terminating program" << endl;
				run = false;
			}

			if (TraceOn)
			{
				cout << "PC = ";
				
				int loops = 0;

				if (instrPC < 1000 && instrPC >= 100)
					loops = 1;
				else if (instrPC < 100 && instrPC >= 10)
					loops = 2;
				else if (instrPC < 10)
					loops = 3;

				for (int i = 0; i < loops; i++)
				{
					cout << "0";
				}

				cout << instrPC << ": ";

				if (print)	// print mode
				{
					if (data1.datatype == 0)
						cout << "PRINT " << data1.symbol << " = " << data1.sInt << endl << endl;
					else if (data1.datatype == 1)
						cout << "PRINT " << data1.symbol << " = " << data1.sDbl << endl << endl;
					else if (data1.datatype == 2)
						cout << "PRINT " << data1.symbol << " = " << data1.str << endl << endl;

					print = false;
				}
				else	// default trace
				{
					cout << r.LookupCode(m_opcode);

					if (s.GetSymbol(m_op1).symbol == "---")
						cout << " | " << s.GetSymbol(m_op1).symbol;
					else if (s.GetSymbol(m_op1).datatype == 0)
						cout << " | " << s.GetSymbol(m_op1).symbol << " (" << s.GetSymbol(m_op1).sInt << ")";
					else if (s.GetSymbol(m_op1).datatype == 2)
						cout << " | " << s.GetSymbol(m_op1).symbol << " (" << s.GetSymbol(m_op1).sDbl << ")";
					else
						cout << " | " << s.GetSymbol(m_op1).symbol << " (" << s.GetSymbol(m_op1).str << ")";

					if (s.GetSymbol(m_op2).symbol == "---")
						cout << " | " << s.GetSymbol(m_op2).symbol;
					else if (s.GetSymbol(m_op2).datatype == 0) 
						cout << " | " << s.GetSymbol(m_op2).symbol << " (" << s.GetSymbol(m_op2).sInt << ")";
					else if (s.GetSymbol(m_op2).datatype == 2)
						cout << " | " << s.GetSymbol(m_op2).symbol << " (" << s.GetSymbol(m_op2).sDbl << ")";
					else
						cout << " | " << s.GetSymbol(m_op2).symbol << " (" << s.GetSymbol(m_op2).str << ")";

					if (m_opcode >= 8 && m_opcode <= 14)
					{
						cout << " | " << m_op3;
					}
					else
					{
						if (s.GetSymbol(m_op3).symbol == "---")
							cout << " | " << s.GetSymbol(m_op3).symbol;
						else if (s.GetSymbol(m_op3).datatype == 0)
							cout << " | " << s.GetSymbol(m_op3).symbol << " (" << s.GetSymbol(m_op3).sInt << ")";
						else if (s.GetSymbol(m_op3).datatype == 2)
							cout << " | " << s.GetSymbol(m_op3).symbol << " (" << s.GetSymbol(m_op3).sDbl << ")";
						else
							cout << " | " << s.GetSymbol(m_op3).symbol << " (" << s.GetSymbol(m_op3).str << ")";
					}
					cout << endl << endl;
				}
			}

			if (!jump)	// check for jump instruction before incrementing the Program Counter
				m_PC++;
		}
	}
};

/*** END OF ABSTRACT DATA TYPES ***/
/*---------------------------------------------------*/
/*** GLOBAL ACCESS VARIABLES FOR LEXICAL ANALYZER ***/
std::ifstream INPUT_FILE;	// input file stream for reading the source code file
bool END_OF_FILE = false;	// boolean that determines the continuation of the main function loop
string FILE_LINE = "";	// string to hold the current readin line from the file
int LINE_INDEX = 0;		// int to keep track of which char the program is at
int LINE_NUM = 0;			// int to keep track of lines in source code for debugging purposes
string NEXT_TOKEN;			// string to contain the lexeme of the next token found with GetNextToken
int TOKEN_CODE = -1;		// token code variable
char NEXT_CHAR = '\0';		// char to hold the next char during the lexical process
const int IDENT_MAX = 30;	// maximum length of characters for a identifyer
const int NUMERIC_MAX = 10;	// maximum length of characters for a numeric token
bool ERROR = false;			// global error flag
bool TRACE_CFG = false;		// trace CFG boolean
bool ECHO_MODE = false;		// echo mode boolean
bool TRACE_INTERPRETER;		// trace interpreter for 
vector<string> errorList;	// list of errors gathered during program
bool PRINT_TOKENS = false;	// bool to determine if tokens print when found

// Global Tables
ReserveTable QUAD_RT;
ReserveTable RESERVED_WORDS;	// reserved table for the reserved words
ReserveTable FOUR_CHAR_MNEMONIC;
SymbolTable SYM_TAB;
QuadTable QUAD_CODE(1000);	// Quad Table that holds the quads for the code

/*** END OF GLOBAL VARIABLES***/
/*---------------------------------------------------------*/
/*** START OF SUPPORT FUNCTIONS FOR LEXICAL ANALYZER ***/

// Converts all lower case letters to upper case
string AllCaps(string convert)
{
	string retStr = "";
	for (int i = 0; i < convert.length(); i++)
	{
		if (convert[i] >= 97 && convert[i] <= 122)
			retStr += convert[i] - 32;
		else
			retStr += convert[i];
	}

	return retStr;
}

// Function to print out Undefined Character error message
void undefChar()
{
	cout << endl << "###*** UNDEFINED CHARACTER ***###" << endl;
	cout << "### Line Number : " << LINE_NUM << endl;
	cout << "### Token Code : " << TOKEN_CODE << endl << endl;

	ERROR = true;
}

// Prints the line being read from the file, for use by GetNextChar
void EchoLine()
{
	cout << endl << "Line " << LINE_NUM << ": " << FILE_LINE << endl;
}

// Global Function to get the next line from the file being read from
void GetNextLine()
{
	if (INPUT_FILE.eof())
	{
		END_OF_FILE = true;
	}
	else
	{
		FILE_LINE.clear();
		getline(INPUT_FILE, FILE_LINE);	// fetch line from input stream
		LINE_NUM++;
		LINE_INDEX = 0;
	}
}

// Function to get the next char from the line being read in
// and put it in the NEXT_CHAR variable
void GetNextChar()
{
	NEXT_CHAR = FILE_LINE[LINE_INDEX++];
}

// returns true if NEXT_CHAR has a whitespace character
bool IsWhiteSpace()
{
	bool retVal = false;

	if (NEXT_CHAR == ' ' || NEXT_CHAR == '\t' || NEXT_CHAR == '\n')
		retVal = true;

	return retVal;
}

// funtion that skips through blanks of the Input Line
void SkipBlanks()
{
	bool loop = true;

	while (loop)
	{
		if (!IsWhiteSpace())
			loop = false;
		else
			GetNextChar();
	}
}

// determines if the character being read is a letter 
// in ascii code returns true if so and 
// if the char is a letter it changes it to upper case
bool IsLetter()
{
	bool retVal = false;

	if (NEXT_CHAR >= 65 && NEXT_CHAR <= 90)
	{
		retVal = true;
	}
	else if (NEXT_CHAR >= 97 && NEXT_CHAR <= 122)
	{
		retVal = true;
	}

	return retVal;
}

// determines if the character is a digit in ascii code
// returns true if so
bool IsDigit()
{
	bool retVal = false;

	if (NEXT_CHAR >= 48 && NEXT_CHAR <= 57)
		retVal = true;

	return retVal;
}

// returns true is NEXT_CHAR contains a '{'
bool IsOpenCurly()
{
	bool retVal = false;

	if (NEXT_CHAR == '{')
		retVal = true;

	return retVal;
}

// returns true is NEXT_CHAR contains a '}'
bool IsClosedCurly()
{
	bool retVal = false;

	if (NEXT_CHAR == '}')
		retVal = true;

	return retVal;
}

// returns true is NEXT_CHAR contains a '('
bool IsOpenParen()
{
	bool retVal = false;

	if (NEXT_CHAR == '(')
		retVal = true;

	return retVal;
}

// returns true is NEXT_CHAR contains a ')'
bool IsClosedParen()
{
	bool retVal = false;

	if (NEXT_CHAR == ')')
		retVal = true;

	return retVal;
}

// returns true is NEXT_CHAR contains a '*'
bool IsAsterisk()
{
	bool retVal = false;

	if (NEXT_CHAR == '*')
		retVal = true;

	return retVal;
}

// returns true is NEXT_CHAR contains a '_'
bool IsUnderscore()
{
	bool retVal = false;

	if (NEXT_CHAR == '_')
		retVal = true;

	return retVal;
}

// returns true is NEXT_CHAR contains a '$'
bool IsDollar()
{
	bool retVal = false;

	if (NEXT_CHAR == '$')
		retVal = true;

	return retVal;
}

// returns true is NEXT_CHAR contains a '.'
bool IsPeriod()
{
	bool retVal = false;

	if (NEXT_CHAR == '.')
		retVal = true;

	return retVal;
}

// returns true is NEXT_CHAR contains a operator token
bool IsOp()
{
	bool retVal = false;

	if (NEXT_CHAR == '/' || NEXT_CHAR == '*' || NEXT_CHAR == '+' || NEXT_CHAR == '-' ||
		NEXT_CHAR == '(' || NEXT_CHAR == ')' || NEXT_CHAR == ';' || NEXT_CHAR == ':' ||
		NEXT_CHAR == '>' || NEXT_CHAR == '<' || NEXT_CHAR == '=' || NEXT_CHAR == ',' ||
		NEXT_CHAR == '[' || NEXT_CHAR == ']' || NEXT_CHAR == '.')
		retVal = true;

	return retVal;
}

// returns true is NEXT_CHAR contains a '"'
bool IsQuoteMark()
{
	bool retVal = false;

	if (NEXT_CHAR == '"')
		retVal = true;

	return retVal;
}

// support function when dealing with (* *) based comments
void FindNextAsterisk()
{
	do
	{
		GetNextChar();
	} while (!IsAsterisk);
}

// Print columns of info on the tokens
void PrintToken(string NEXT_TOKEN, int code)
{
	// Neatly printed straight columns
	// 1) The lexeme
	// 2) The Token Code
	// 3) The table looked up 4 character mnomonic for that code
	// 4) The identifyers and literals added to the symbol table and the index of the token


	cout << "|  Lexeme  |" << "Token Code" << "|" << "Mnem" << "|" << "TableIndex" << " |" << endl;
	cout << "|--------------------------------------|" << endl;
	cout << "|" << setw(10) << NEXT_TOKEN << "| Code: " << setw(2) << code << " |" << setw(4) << FOUR_CHAR_MNEMONIC.LookupCode(code) <<
		"|Index: " << setw(3) << SYM_TAB.LookupSymbol(NEXT_TOKEN) << " |" << endl;
	cout << "|--------------------------------------|" << endl << endl;
}

/*** END OF SUPPORT FUNCTIONS FOR LEXICAL ANALYZER ***/
/***-----------------------------------------------------***/

/*--------------------------------------------------------*/
/* Lexical Analyzer Start
||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
*/
void GetNextToken(bool echoOn)
{
	bool warning = false;	// bool to determine warning message
	bool acceptState = false;	// bool to determine if a token is accepted or not
	bool nextCharFlag = false;	// when raised, skip next char
	bool newLineFlag = false;
	int charCount = 0;		// variable to count the amount of characters being read for a token
	int dfaState = 0;		// dfa state variable for finding tokens

	NEXT_TOKEN = "";	// clear out NEXT_TOKEN

	// Loop through to take care of white space and potential comments
	while (!END_OF_FILE && !acceptState)
	{
		// Loop and get rid of whitespace and get next line if needed
		while (FILE_LINE == "" || NEXT_CHAR == '\0')
		{
			if (dfaState == 30)	// String const state, throw error message
			{
				string temp = "###*** ERROR ***###\n### Line Number : ";
				temp += std::to_string(LINE_NUM) + "\n### String does not complete on the same line it started\n";
				errorList.push_back(temp);

				cout << errorList[errorList.size() - 1] << endl;
				//ERROR = true;
				NEXT_TOKEN = "";
				dfaState = 0;
			}

			if (dfaState != 0 && dfaState < 50)
			{
				acceptState = true;
				break;
			}
			else if (!INPUT_FILE.eof())
			{
				GetNextLine();
				if (echoOn)
					EchoLine();

				GetNextChar();

				if (IsWhiteSpace())
					SkipBlanks();
			}
			else
			{
				END_OF_FILE = true;
				break;
			}
		}

		if (END_OF_FILE || acceptState)
			continue;

		// DFA Process
		if (dfaState == 0)	// ***START STATE***
		{
			if (IsOpenCurly())	// -> CURLY COMMENT STATE
			{
				dfaState = 50;
			}
			else if (IsOpenParen())	// -> PARENTHESES COMMENT STATE
			{
				dfaState = 55;
			}
			else if (IsLetter())	// -> IDENT STATE
			{
				dfaState = 10;
				charCount = 1;
				NEXT_TOKEN += NEXT_CHAR;
			}
			else if (IsDigit()) // -> NUMERIC STATE
			{
				dfaState = 20;
				charCount = 1;
				NEXT_TOKEN += NEXT_CHAR;
			}
			else if (IsQuoteMark())	// -> STRING CONST STATE
			{
				dfaState = 30;	// Do not read in the " for the next token
				NEXT_TOKEN += NEXT_CHAR;
			}
			else if (IsOp()) // -> OPERATOR STATE
			{
				dfaState = 40;
				NEXT_TOKEN = NEXT_CHAR;
				
				if (!((NEXT_TOKEN == ":" || NEXT_TOKEN == ">" || NEXT_TOKEN == "<")))
				{
					acceptState = true;
					GetNextChar();
				}
			}
			else if (NEXT_CHAR != '\0' && !IsWhiteSpace())	// -> UNKNOWN TOKEN STATE
			{
				dfaState = 99;
				NEXT_TOKEN = NEXT_CHAR;
				GetNextChar();
				acceptState = true;
			}
		}
		else if (dfaState == 10)	// ***IDENT STATE***
		{
			if (IsLetter() || IsDigit() || IsUnderscore() || IsDollar())
			{
				if (++charCount <= IDENT_MAX)
					NEXT_TOKEN += NEXT_CHAR;
				else if (!warning)
					warning = true;
			}
			else
				acceptState = true;
		}
		else if (dfaState == 20)	// ***NUMERIC STATE***
		{
			if (IsDigit())
			{
				if (++charCount <= NUMERIC_MAX)
					NEXT_TOKEN += NEXT_CHAR;
				else if (!warning)
					warning = true;
			}
			else if (IsPeriod())	// -> FLOAT STATE
			{
				if (++charCount <= NUMERIC_MAX)
				{
					dfaState = 21;
					NEXT_TOKEN += NEXT_CHAR;
				}
				else if (!warning)
					warning = true;
			}
			else
				acceptState = true;
		}
		else if (dfaState == 21)	// ***FLOAT STATE***
		{
			if (IsDigit())
			{
				if (++charCount <= NUMERIC_MAX)
					NEXT_TOKEN += NEXT_CHAR;
				else if (!warning)
					warning = true;
			}
			else if (NEXT_CHAR == 'E' || NEXT_CHAR == 'e')	// -> FLOAT W/ EXPONENT STATE
			{
				if (++charCount <= NUMERIC_MAX)
				{
					NEXT_TOKEN += NEXT_CHAR;
					dfaState = 22;
				}
				else if (!warning)
					warning = true;
			}
			else
				acceptState = true;
		}
		else if (dfaState == 22)	// ***FLOAT W/ EXPONENT STATE 1***
		{
			if (NEXT_CHAR == '+' || NEXT_CHAR == '-' || IsDigit())
			{
				charCount++;
				NEXT_TOKEN += NEXT_CHAR;
				dfaState = 23;
			}
			else if (!warning)
				warning = true;
		}
		else if (dfaState == 23)	// ***FLOAT W/ EXPONENT STATE 2***
		{
			if (IsDigit())
			{
				if (++charCount <= NUMERIC_MAX)
					NEXT_TOKEN += NEXT_CHAR;
			}
			else
				acceptState = true;
		}
		else if (dfaState == 30)	// ***STRING CONSTANT STATE***
		{
			NEXT_TOKEN += NEXT_CHAR;

			if (IsQuoteMark())
			{
				acceptState = true;
				GetNextChar();
			}
		}
		else if (dfaState == 40)
		{
			if (IsWhiteSpace())
				SkipBlanks();
			if (NEXT_CHAR == '=')
			{
				NEXT_TOKEN += NEXT_CHAR;
				GetNextChar();
			}

			acceptState = true;
		}
		else if (dfaState == 50)	// ***CURLY COMMENT STATE***
		{
			if (IsClosedCurly())
				dfaState = 0;
		}
		else if (dfaState == 55)	// ***PARENTHESES COMMENT STATE 1***
		{
			if (IsAsterisk())	// -> Paren State 2
				dfaState = 56;
			else			// no asterisk, operator found
			{
				dfaState = 40;
				NEXT_TOKEN = "(";
				acceptState = true;
			}
		}
		else if (dfaState == 56)	// ***PARENTHESES COMMENT STATE 2***
		{
			if (IsAsterisk())	// -> Paren State 3
				dfaState = 57;

			if (END_OF_FILE)
			{
				string temp = "###*** ERROR ***###\n### Line Number : ";
				temp += std::to_string(LINE_NUM) + "\n### Source code ends before comment closes\n";
				errorList.push_back(temp);

				cout << errorList[errorList.size() - 1] << endl;
			}
		}
		else if (dfaState == 57)	// ***PARENTHESES COMMENT STATE 3***
		{
			if (IsClosedParen())	// Comment complete, back to state 0		
				dfaState = 0;
			else
				dfaState = 56;	// *) not found -> Paren state 2
		}

		if (!acceptState)
			GetNextChar();
	}

	// Exit Loop when a char that leads to a token appears
	// Go through conditions and branch out from there

	if (dfaState <= 59 && dfaState >= 50)
	{
		string temp = "###*** ERROR ***###\n### Line Number : ";
		temp += std::to_string(LINE_NUM) + "\n### Comment does not end before end of file\n";
		errorList.push_back(temp);

		cout << errorList[errorList.size() - 1] << endl;

		ERROR = true;
	}

	if (warning)
	{
		string temp = "###*** WARNING ***###\n### Line Number : ";
		temp += std::to_string(LINE_NUM) + "\n### Too many characters for token\n";
		errorList.push_back(temp);

		cout << errorList[errorList.size() - 1] << endl;
	}

	// Accept States
	if (acceptState || END_OF_FILE)
	{
		if (dfaState == 99)	// Unknown Token accepted
		{
			TOKEN_CODE = 99;
			undefChar();
		}
		else if (dfaState == 10)	// IDENT Token accepted
		{
			// convert to upper case so it can be compared to reserved words
			string temp = AllCaps(NEXT_TOKEN);

			// run through reserved words
			TOKEN_CODE = RESERVED_WORDS.LookupName(temp);

			// if not reserved then code = 50
			if (TOKEN_CODE == -1)
			{
				TOKEN_CODE = 50;

				if (SYM_TAB.LookupSymbol(temp) == -1)
					SYM_TAB.AddSymbol(temp, 1, "IDENT");
			}
		}
		else if (dfaState == 20)	// INTEGER TOKEN ACCEPTED
		{
			TOKEN_CODE = 51;

			if (SYM_TAB.LookupSymbol(NEXT_TOKEN) == -1 && (charCount >= NUMERIC_MAX))
				SYM_TAB.AddSymbol(NEXT_TOKEN, 2, INT_MAX);
			else if (SYM_TAB.LookupSymbol(NEXT_TOKEN) == -1)
				SYM_TAB.AddSymbol(NEXT_TOKEN, 2, std::stoi(NEXT_TOKEN));
		}
		else if (dfaState >= 21 && dfaState <= 23)	// FLOAT TOKEN ACCEPTED
		{
			TOKEN_CODE = 52;

			if (SYM_TAB.LookupSymbol(NEXT_TOKEN) == -1)
				SYM_TAB.AddSymbol(NEXT_TOKEN, 2, std::stof(NEXT_TOKEN));
		}
		else if (dfaState == 30)	// STRING CONSTANT TOKEN ACCEPTED
		{
			TOKEN_CODE = 53;

			if (SYM_TAB.LookupSymbol(NEXT_TOKEN) == -1)
				SYM_TAB.AddSymbol(NEXT_TOKEN, 2, NEXT_TOKEN);
		}
		else if (dfaState == 40)	// OPERATOR TOKEN ACCEPTED
		{
			TOKEN_CODE = RESERVED_WORDS.LookupName(NEXT_TOKEN);
		}

		if (PRINT_TOKENS)
			PrintToken(NEXT_TOKEN, TOKEN_CODE);
	}
}

/* Lexical Analyzer End
||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
*/

/*** START OF FUNCTIONS FOR SYNTAXICAL ANALYZER ***/

// Global vectors to be used by syntaxical analyzer
vector<string> DEC_VARS;		// list of declared variables
vector<bool> VAR_USE;	// list in parrelel with decVars, determines if they were used or not
vector<string> DEC_LABELS;	// list of declared labels in the code
vector<bool> LABEL_USE;	// list in parrelel with decLabels, determines if they were used or not
vector<int> LABEL_PC;
string PROGRAM_HEADER = "";	// string of the program header
bool DECLARATION = false;	// bool to determine if syntax analyzer is searching for a declaration
bool SEARCH_LABEL = false;	// bool to determine if syntax analyzer is searching for a label
bool LABEL = false;			// bool for use in label declaration
bool SEARCH_HEADER = false;	// bool for when identifier finds a header
bool ERROR_EXEMPT = false;	// bool for when errors are exempted for certain non-terminal 
int PC = 0;					// program counter for the program being read in
int PLUS_1_INDEX = -1;		// global to hold the index location of const 1 in symbol table, to be defined in main
int MINUS_1_INDEX = -1;		// global to hold the index location of const -1 in symbol table, to be defined in main
int TEMP_NUM = 0;		// keeps track of the amount of temp variables in the symbol table for naming purposes

// declaring the function name
int nt_program();
int nt_block();
int nt_blockBody();
int nt_labelDec();
int nt_varDecSec();
int nt_varDec();
int nt_statement();
int nt_var();
int nt_label();
int nt_relExp();
int nt_relop();
int nt_simpExp();
int nt_addop();
int nt_sign();
int nt_term();
int nt_mulop();
int nt_factor();
int nt_type();
int nt_simpType();
int nt_constant();
int nt_unsignConst();
int nt_unsignNum();
int nt_ident();
int nt_strConst();

// non-terminal functions that return an integer value
// debug method for entry and exiting messages
void DEBUG(bool entering, string name)
{
	if (entering)
		cout << "Entering: " << name << endl;
	else
		cout << "Exiting : " << name << endl;
}

// Function to print out Syntax errors message
void SyntaxError(string expected, string found)
{
	if (!ERROR_EXEMPT)
	{
		string temp = "###*** SYNTAX ERROR ***###\n### Line Number : ";
		temp += std::to_string(LINE_NUM) + "\n### Expected : '" + expected + "', but '" + found + "' found\n";
		errorList.push_back(temp);

		cout << errorList[errorList.size() - 1] << endl;

		ERROR = true;
	}
}

// Variable to check if the variable has already been declared
void CheckDecVar()
{
	string temp = AllCaps(NEXT_TOKEN);
	int size = DEC_VARS.size();
	bool found = false;

	// check decVars
	if (size > 0)
	{
		for (int i = 0; i < size && !found; i++)
		{
			if (DEC_VARS[i] == temp)
				found = true;
		}

		if (found)
			cout << "WARNING: " << temp << " variable has already been declared" << endl;
		else
		{
			DEC_VARS.push_back(AllCaps(NEXT_TOKEN));
			VAR_USE.push_back(false);
		}
	}
	else
	{
		DEC_VARS.push_back(AllCaps(NEXT_TOKEN));
		VAR_USE.push_back(false);
	}
}

// Check if variable has been declared or not and marks it's use as true
void CheckVar()
{
	string temp = AllCaps(NEXT_TOKEN);
	int size = DEC_VARS.size();
	int index = 0;
	bool found = false;

	// check decVars
	if (size > 0)
	{
		for (int i = 0; i < size && !found; i++)
		{
			if (DEC_VARS[i] == temp)
			{
				found = true;
				index = i;
			}
		}
	}	

	if (!found)
	{
		cout << "WARNING: " << temp << " variable has not been declared" << endl;
		DEC_VARS.push_back(AllCaps(NEXT_TOKEN));
		VAR_USE.push_back(true);
	}
	else
		VAR_USE[index] = true;
}

// Outputs if a variable was not used
void OutputVarUse()
{
	for (int i = 0; i < VAR_USE.size(); i++)
	{
		if (!VAR_USE[i])
			cout << "WARNING: " << DEC_VARS[i] << " was not used in " << PROGRAM_HEADER << endl;
	}
}

// Function to check declaration of variables in syntax analysis
void CheckDecLabels()
{
	string temp = AllCaps(NEXT_TOKEN);
	int size = DEC_LABELS.size();
	bool found = false;

	// check decVars
	if (size > 0)
	{

		for (int i = 0; i < size && !found; i++)
		{
			if (DEC_LABELS[i] == temp)
				found = true;
		}

		if (found)
			cout << "WARNING: " << temp << " label has already been declared" << endl;
		else
		{
			DEC_LABELS.push_back(AllCaps(NEXT_TOKEN));
			LABEL_USE.push_back(false);
			LABEL_PC.push_back(-1);
		}
	}
	else
	{
		DEC_LABELS.push_back(AllCaps(NEXT_TOKEN));
		LABEL_USE.push_back(false);
		LABEL_PC.push_back(-1);
	}
}

// Check if the label has been declared and mark it's use as true
void CheckLabel()
{
	string temp = AllCaps(NEXT_TOKEN);
	int size = DEC_LABELS.size();
	int index = 0;
	bool found = false;

	// check decLabels
	if (size > 0)
	{
		for (int i = 0; i < size && !found; i++)
		{
			if (DEC_LABELS[i] == temp)
			{
				found = true;
				index = i;
			}
		}
	}

	if (!found)
	{
		cout << "WARNING: " << temp << " label has not been declared" << endl;
		DEC_LABELS.push_back(AllCaps(NEXT_TOKEN));
		LABEL_USE.push_back(true);
	}
	else
	{
		LABEL_USE[index] = true;
		LABEL_PC[index] = QUAD_CODE.NextQuad();
	}
}

// Support fuction to get the PC associated with the label in the program
int GetLabelPC(string label)
{
	int pc = -1;
	string str = AllCaps(label);
	int size = LABEL_USE.size();
	bool found = false;

	if (size > 0)
	{
		for (int i = 0; i < size && !found; i++)
		{
			if (DEC_LABELS[i] == str )
			{
				found = true;

				if (LABEL_USE[i])
					pc = LABEL_PC[i];
				else
					pc = -1;
			}
		}
	}

	if (!found)
	{
		cout << "ERROR: " << label << " was not declared" << endl;
		ERROR = true;
	}

	return pc;
}

// Check if header is being used again for the program
void CheckIfHeader()
{
	if (PROGRAM_HEADER == AllCaps(NEXT_TOKEN))
	{
		cout << "ERROR: PROGRAM HEADER CANNOT BE USED AS A IDENTIFIER WITHIN PROGRAM" << endl;
		ERROR = true;
	}
}

// Outputs if a variable was not used
void OutputLabelUse()
{
	for (int i = 0; i < LABEL_USE.size(); i++)
	{
		if (!LABEL_USE[i])
			cout << "WARNING: " << DEC_LABELS[i] << " was not used in " << PROGRAM_HEADER << endl;
	}
}

// Finds a token that works with the start of <statement>
void Resync()
{
	while (ERROR && !END_OF_FILE)
	{
		GetNextToken(ECHO_MODE);
		/* Searching for
				$IDENTIFIER (label/var) | 50
				$BEGIN (block-body)		| 11
				$IF						| 4
				$WHILE					| 14
				$REPEAT					| 17
				$FOR					| 7
				$GOTO					| 0
				$WRITELN				| 9
		*/
		if (TOKEN_CODE == 50 || TOKEN_CODE == 11 || TOKEN_CODE == 4 ||
			TOKEN_CODE == 24 || TOKEN_CODE == 7 || TOKEN_CODE == 0 ||
			TOKEN_CODE == 9)
		{
			ERROR = false;
		}
	}
}

// Flushes tokens until the start of a <statement> is found
int ErrorHandling()
{
	if (TRACE_CFG)
		DEBUG(true, "ERROR HANDLING");
	int ret = -1;
	bool traceWasOn = TRACE_CFG;

	if (TRACE_CFG)
		TRACE_CFG = false;

	while (!END_OF_FILE && TOKEN_CODE != 12)	// Token Code is not $END
	{
		Resync();
		while (!ERROR && !END_OF_FILE && TOKEN_CODE != 12)
			ret = nt_statement();
	}

	if (traceWasOn)
		TRACE_CFG = true;

	if (TRACE_CFG)
		DEBUG(false, "ERROR HANDLING");
	return ret;
}

// Prints intructions that was interpreted during syntax process
void TraceInterpreter(int iPC, Quad instr)
{
	iPC--;
	cout << "PC = " << iPC << ", Executing: " << QUAD_RT.LookupCode(instr.opcode) <<
		", " << instr.op1 << ", " << instr.op2 << ", " << instr.op3 << endl;
}

// Generates a symbol in the symbol table and returns the index of the new symbol
int GenSymbol()
{
	int index = -1;
	string name = "_TEMP";
		
	if (TEMP_NUM < 10)
		name += "0" + std::to_string(TEMP_NUM++);
	else
		name += std::to_string(TEMP_NUM++);

	SYM_TAB.AddSymbol(name, 1, 0);
	index = SYM_TAB.LookupSymbol(name);

	return index;
}

// returns the flip logic branch opcode
int flipRelop(int relop)
{
	int result = -1;

	if (relop == QUAD_RT.LookupName("BZ"))
		result = QUAD_RT.LookupName("BNZ");
	else if (relop == QUAD_RT.LookupName("BNZ"))
		result = QUAD_RT.LookupName("BZ");
	else if (relop == QUAD_RT.LookupName("BN"))
		result = QUAD_RT.LookupName("BNN");
	else if (relop == QUAD_RT.LookupName("BNN"))
		result = QUAD_RT.LookupName("BN");
	else if (relop == QUAD_RT.LookupName("BP"))
		result = QUAD_RT.LookupName("BNP");
	else if (relop == QUAD_RT.LookupName("BNP"))
		result = QUAD_RT.LookupName("BP");

	return result;
}

// Gets array value from symbol table or generates a new array value in it
void GenArray(string name, int max, int type)
{
	string str = "";
	for (int i = 1; i < max; i++)
	{
		str = AllCaps(name) + "[" + std::to_string(i) + "]";
		if (type == 0)	// int
			SYM_TAB.AddSymbol(str, 1, 0);
		else if (type == 1)
			SYM_TAB.AddSymbol(str, 1, 0.0);
		else if (type == 2)
			SYM_TAB.AddSymbol(str, 1, "");
	}
}

// <program> non-terminal function
// <program> -> $UNIT <identifier> $SEMICOLON <block> $PERIOD
int nt_program()
{
	if (TRACE_CFG)
		DEBUG(true, "<program>");
	int ret = -1;

	if (TOKEN_CODE == 15)	// $UNIT
	{
		GetNextToken(ECHO_MODE);

		SEARCH_HEADER = true;
		ret = nt_ident();	// <identifier>
		SEARCH_HEADER = false;
		if (ret == -1)
		{
			SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(50), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
			return -1;
		}

		if (TOKEN_CODE == 36)	// $SEMICOLON
		{
			GetNextToken(ECHO_MODE);

			ret = nt_block();	// <block>
			
			if (TOKEN_CODE == 48)	// $PERIOD
			{
				QUAD_CODE.AddQuad(QUAD_RT.LookupName("STOP"), 0, 0, 0);
				if (TRACE_INTERPRETER)
					TraceInterpreter(QUAD_CODE.NextQuad(), QUAD_CODE.GetQuad(QUAD_CODE.NextQuad() - 1));
			}
			else	
			{
				SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(48), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
				return -1;
			}
		}
		else	
		{
			SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(36), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
			return -1;
		}

	}
	else	// error
	{
		SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(15), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
		return -1;
	}

	// Might need to add a GNT here for the future

	if (TRACE_CFG)
		DEBUG(false, "<program>");
	return ret;
}

// <block> non-terminal function
// [<label - declaration>]{ <variable - dec - sec> }*<block - body>
int nt_block()
{
	if (TRACE_CFG)
		DEBUG(true, "<block>");

	int ret = -1;

	ret = nt_labelDec();	// [<label-declaration>]
	
	if (!ERROR)
	{
		while (nt_varDecSec() != -1) ; // {<varaible-dec-sec}*

		ret = nt_blockBody();	// <block-body>
	}

	if (ERROR)
	{
		ErrorHandling();
		ret = 100;	// I don't think I need this
	}

	if (TRACE_CFG)
		DEBUG(false, "<block>");
	return ret;
}

// <block-body> non-terminal function
// $BEGIN <statement> {$SCOLN <statement>} $END
int nt_blockBody()
{
	if (TRACE_CFG)
		DEBUG(true, "<block-body>");

	int ret = -1;

	if (TOKEN_CODE == 11)	// $BEGIN
	{
		GetNextToken(ECHO_MODE);

		ret = nt_statement();	// <statement> : identifier token code
		if (ERROR)
			return -1;

		while (TOKEN_CODE == 36)	// {$SEMICOLON <statement>}*
		{
			GetNextToken(ECHO_MODE);

			ret = nt_statement();
			if (ERROR)
				return -1;
		}

		if (TOKEN_CODE == 12)	// $END
			GetNextToken(ECHO_MODE);
		else
		{
			SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(12), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
			return -1;
		}
	}
	else
	{
		SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(11), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
		return -1;
	}

	if (TRACE_CFG)
		DEBUG(false, "<block-body>");
	return ret;
}

// <label-declaration> -> $LABEL <identifier> {$COMMA <identifier>}* $SEMICOLON
int nt_labelDec()
{
	if (TRACE_CFG)
		DEBUG(true, "<label-declaration>");

	int ret = -1;

	if (TOKEN_CODE == 16)	// $LABEL
	{
		GetNextToken(ECHO_MODE);

		LABEL = true;
		DECLARATION = true;
		ret = nt_ident();	// <identifier>
		LABEL = false;
		DECLARATION = false;

		if (ret == -1)	// error
		{
			SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(50), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
			return -1;
		}
		
		while (TOKEN_CODE == 44)	// {$COMMA <identifier>}*
		{
			GetNextToken(ECHO_MODE);

			LABEL = true;
			DECLARATION = true;
			ret = nt_ident();
			LABEL = false;
			DECLARATION = false;

			if (ret == -1)
			{
				SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(50), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
				return -1;
			}
		}

		if (TOKEN_CODE == 36)	// $SEMICOLON
		{
			ret = TOKEN_CODE;
			GetNextToken(ECHO_MODE);
		}
		else
		{
			SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(36), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
			return -1;
		}
	}

	if (TRACE_CFG)
		DEBUG(false, "<label-declaration>");
	return ret;
}

// <variable-dec-sec> non-terminal function
// $VAR <variable-declaration>
int nt_varDecSec()
{
	if (TRACE_CFG)
		DEBUG(true, "<variable-dec-sec>");

	int ret = -1;

	if (TOKEN_CODE == 13) // $VAR
	{
		GetNextToken(ECHO_MODE);

		ret = nt_varDec();	// <variable-declaration>
		if (ERROR)
			return -1;
	}

	if (TRACE_CFG)
		DEBUG(false, "<variable-dec-sec>");
	return ret;
}

// <variable-declaration> non-terminal function
// {<identifier> {$COMMA <identifier>}*$COLON <type> $SEMICOLON}+
int nt_varDec()
{
	if (TRACE_CFG)
		DEBUG(true, "<variable-declaration>");

	int ret = -1;
	int type;
	vector<int> varIndexes;
	bool identFound = false;

	DECLARATION = true;
	while ((ret = nt_ident()) != -1)
	{
		if (!identFound)
			identFound = true;

		varIndexes.push_back(ret);

		while (TOKEN_CODE == 44)	// $COMMA
		{
			GetNextToken(ECHO_MODE);

			if ((ret = nt_ident()) == -1)	// <identifier>
			{
				SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(50), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
				return -1;
			}
			varIndexes.push_back(ret);
		}

		if (TOKEN_CODE == 47)	// $COLON
		{
			GetNextToken(ECHO_MODE);

			type = nt_type();	// <type>
			if (ERROR)
				return -1;

			for (int i = 0; i < varIndexes.size(); i++)
			{
				if (type == 1)	// integer
					SYM_TAB.UpdateSymbol(varIndexes[i], SYM_TAB.GetSymbol(varIndexes[i]).kind, 0);
				else if (type == 23)	// real(float)
					SYM_TAB.UpdateSymbol(varIndexes[i], SYM_TAB.GetSymbol(varIndexes[i]).kind, 0.0f);
				else if (type == 24)	// string
					SYM_TAB.UpdateSymbol(varIndexes[i], SYM_TAB.GetSymbol(varIndexes[i]).kind, "");
			}

			if (TOKEN_CODE == 36)	// $SEMICOLON
			{
				ret = TOKEN_CODE;
				GetNextToken(ECHO_MODE);
			}
			else
			{
				SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(36), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
				return -1;
			}

		}
		else
		{
			SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(36), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
			return -1;
		}
	}
	DECLARATION = false;
	
	if (identFound)
		ret = 1;
	else
	{
		SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(50), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
		return -1;
	}

	if (TRACE_CFG)
		DEBUG(false, "<variable-declaration>");
	return ret;
}

// <statement> non-terminal function
//{<label> $COLON}*
//[
//	<variable> $ASSIGN(<simple expression> | <stringconst>) | <block - body> |
//	$IF <relexpression> $THEN <statement>[$ELSE <statement>] |
//	$WHILE <relexpression> $DO <statement> |
//	$REPEAT <statement> $UNTIL <relexpression> |
//	$FOR <variable> $ASSIGN <simple expression>
//	$TO <simple expression> $DO <statement> |
//	$GOTO <label> |
//	$WRITELN $LPAR(<simple expression> | <identifier> |
//		<stringconst>) $RPAR
//] +
int nt_statement()
{
	if (TRACE_CFG)
		DEBUG(true, "<statement>");

	int ret = -1;
	int left, right, temp, relexp, elseBr, saveTop, branchTo;
	Quad tempQ;

	//{<label> $COLON}*
	LABEL = true;
	while (nt_label() != -1) // <label>
	{
		if (TOKEN_CODE == 47)	// $COLON
			GetNextToken(ECHO_MODE);
		else	
		{
			SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(47), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
			return -1;
		}
	}
	LABEL = false;

	// <variable> $ASSIGN (<simple expression> | <string literal>)
	if ((ret = nt_var()) != -1)	// <variable> returned with correct token
	{
		int left = ret;	// variable that will be assigned a value
		int right = 0;	// value to assign variable to

		if (TOKEN_CODE == 37)	// $ASSIGN
		{
			GetNextToken(ECHO_MODE);
			if ((ret = nt_simpExp()) != -1)	// <simple expression>
				right = ret;
			else
			{
				if ((ret = nt_strConst()) == -1)	// <stringconst>
				{
					SyntaxError("(<simple expression> | <string constant>)", FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
					return -1;
				}
				right = ret;
			}

			// Generate Code for Statement
			QUAD_CODE.AddQuad(QUAD_RT.LookupName("MOV"), right, 0, left);
			if (TRACE_INTERPRETER)
				TraceInterpreter(QUAD_CODE.NextQuad(), QUAD_CODE.GetQuad(QUAD_CODE.NextQuad() - 1));
		}
		else
		{
			SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(37), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
			return -1;
		}
	}	
	// $IF <relexpression> $THEN <statement> [$ELSE <statement>]
	else if (TOKEN_CODE == 4)	// $IF
	{
		GetNextToken(ECHO_MODE);
		int relexp, elseBr;
		Quad tempQ;
		relexp = nt_relExp();	// <relexpression>
		if (ERROR)
			return -1;

		if (TOKEN_CODE == 5)	// $THEN
		{
			GetNextToken(ECHO_MODE);
			nt_statement();		// <statement>
			if (ERROR)
				return -1;
			
			// Set the branch quad to the next PC and flip the branch code
			tempQ = QUAD_CODE.GetQuad(relexp);
			QUAD_CODE.SetQuad(relexp, flipRelop(tempQ.opcode), tempQ.op1, tempQ.op2, QUAD_CODE.NextQuad());

			// [$ELSE <statement>]
			if (TOKEN_CODE == 6)	// $ELSE
			{
				elseBr = QUAD_CODE.NextQuad();
				QUAD_CODE.AddQuad(QUAD_RT.LookupName("BR"), 0, 0, -1);	// will be set later
				if (TRACE_INTERPRETER)
					TraceInterpreter(QUAD_CODE.NextQuad(), QUAD_CODE.GetQuad(QUAD_CODE.NextQuad() - 1));

				tempQ = QUAD_CODE.GetQuad(relexp);					// reset the false branch to the NextQuad after the this branch due to $ELSE
				QUAD_CODE.SetQuad(relexp, tempQ.opcode, tempQ.op1, tempQ.op2, QUAD_CODE.NextQuad());
				if (TRACE_INTERPRETER)
					TraceInterpreter(relexp + 1, QUAD_CODE.GetQuad(relexp));

				GetNextToken(ECHO_MODE);
				nt_statement();

				tempQ = QUAD_CODE.GetQuad(elseBr);		// set branch before the else statment to go to the instruction after $ELSE
				QUAD_CODE.SetQuad(elseBr, tempQ.opcode, tempQ.op1, tempQ.op2, QUAD_CODE.NextQuad());
				if (TRACE_INTERPRETER)
					TraceInterpreter(elseBr + 1, QUAD_CODE.GetQuad(elseBr));
			}
		}
		else
		{
			SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(5), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
			return -1;
		}
	}
	// | $WHILE <relexpression> $DO <statement> |
	else if (TOKEN_CODE == 14)	// $WHILE
	{
		GetNextToken(ECHO_MODE);

		saveTop = QUAD_CODE.NextQuad();	// NEED TO FIGURE OUT NEXT QUAD
		relexp = nt_relExp();	// <relexpression>
		if (ERROR)
			return -1;

		if (TOKEN_CODE == 3)	// $DO
		{
			GetNextToken(ECHO_MODE);
			nt_statement();	// <statement>
			if (ERROR)
				return -1;

			QUAD_CODE.AddQuad(QUAD_RT.LookupName("BR"), 0, 0, saveTop);
			if (TRACE_INTERPRETER)
				TraceInterpreter(QUAD_CODE.NextQuad(), QUAD_CODE.GetQuad(QUAD_CODE.NextQuad() - 1));

			tempQ = QUAD_CODE.GetQuad(relexp);
			QUAD_CODE.SetQuad(relexp, flipRelop(tempQ.opcode), tempQ.op1, tempQ.op2, QUAD_CODE.NextQuad());	// if false, go back to top
			if (TRACE_INTERPRETER)
				TraceInterpreter(relexp + 1, QUAD_CODE.GetQuad(relexp));
		}
		else
		{
			SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(3), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
			return -1;
		}
	}
	// | $REPEAT <statement> $UNTIL <relexpression> |
	else if (TOKEN_CODE == 17)	// $REPEAT
	{
		saveTop = QUAD_CODE.NextQuad();
		GetNextToken(ECHO_MODE);
		nt_statement();		// <statement>
		if (ERROR)
			return -1;

		if (TOKEN_CODE == 18)	// $UNTIL
		{
			GetNextToken(ECHO_MODE);
			relexp = nt_relExp();	// <relexpression>
			if (ERROR)
				return -1;

			tempQ = QUAD_CODE.GetQuad(relexp);
			QUAD_CODE.SetQuad(relexp, flipRelop(tempQ.opcode), tempQ.op1, tempQ.op2, saveTop);	// if false, go back to top
			if (TRACE_INTERPRETER)
				TraceInterpreter(relexp + 1, QUAD_CODE.GetQuad(relexp));
		}
		else
		{
			SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(3), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
			return -1;
		}
	}
	// | $FOR <variable> $ASSIGN <simple expression> 
	//		$TO <simple expression> $DO <statement> |
	else if (TOKEN_CODE == 7)	// $FOR
	{
		GetNextToken(ECHO_MODE);

		if ((left = nt_var()) == -1)	// error handling
		{
			SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(50), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
			return -1;
		}

		if (TOKEN_CODE == 37)	// $ASSIGN
		{
			GetNextToken(ECHO_MODE);
			right = nt_simpExp();	// <simple expression>
			if (ERROR)
				return -1;

			QUAD_CODE.AddQuad(QUAD_RT.LookupName("MOV"), right, 0, left);	// var := a
			if (TRACE_INTERPRETER)
				TraceInterpreter(QUAD_CODE.NextQuad(), QUAD_CODE.GetQuad(QUAD_CODE.NextQuad() - 1));

			if (TOKEN_CODE == 2)	//	$TO
			{
				GetNextToken(ECHO_MODE);
				ret = nt_simpExp();	// <simple expression>
				if (ERROR)
					return -1;

				saveTop = QUAD_CODE.NextQuad();

				temp = GenSymbol();
				QUAD_CODE.AddQuad(QUAD_RT.LookupName("SUB"), left, ret, temp);
				if (TRACE_INTERPRETER)
					TraceInterpreter(QUAD_CODE.NextQuad(), QUAD_CODE.GetQuad(QUAD_CODE.NextQuad() - 1));

				branchTo = QUAD_CODE.NextQuad();
				QUAD_CODE.AddQuad(QUAD_RT.LookupName("BZ"), temp, 0, -1);	// PC to be set later
				if (TRACE_INTERPRETER)
					TraceInterpreter(QUAD_CODE.NextQuad(), QUAD_CODE.GetQuad(QUAD_CODE.NextQuad() - 1));

				if (TOKEN_CODE == 3)	// $DO
				{
					GetNextToken(ECHO_MODE);
					nt_statement();	// <statement>
					if (ERROR)
						return -1;

					QUAD_CODE.AddQuad(QUAD_RT.LookupName("ADD"), left, PLUS_1_INDEX, left);

					QUAD_CODE.AddQuad(QUAD_RT.LookupName("BR"), 0, 0, saveTop);
					if (TRACE_INTERPRETER)
						TraceInterpreter(QUAD_CODE.NextQuad(), QUAD_CODE.GetQuad(QUAD_CODE.NextQuad() - 1));

					tempQ = QUAD_CODE.GetQuad(branchTo);
					QUAD_CODE.SetQuad(branchTo, tempQ.opcode, tempQ.op1, tempQ.op2, QUAD_CODE.NextQuad());
					if (TRACE_INTERPRETER)
						TraceInterpreter(branchTo, QUAD_CODE.GetQuad(branchTo));
				}
				else
				{
					SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(3), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
					return -1;
				}
			}
			else
			{
				SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(2), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
				return -1;
			}
		}
		else
		{
			SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(37), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
			return -1;
		}
	}
	// | $GOTO <label> |
	else if (TOKEN_CODE == 0)	// $GOTO
	{
		GetNextToken(ECHO_MODE);
		if ((ret = nt_label()) == -1)
		{
			SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(50), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
			return -1;
		}
		else
		{
			branchTo = GetLabelPC(SYM_TAB.GetSymbol(ret).symbol);
			QUAD_CODE.AddQuad(QUAD_RT.LookupName("BR"), 0, 0, branchTo);
			if (TRACE_INTERPRETER)
				TraceInterpreter(QUAD_CODE.NextQuad(), QUAD_CODE.GetQuad(QUAD_CODE.NextQuad() - 1));
		}
	}
	// | $WRITELN $LPAR (<simple expression> | <identifier> | <stringconst>) $RPAR
	else if (TOKEN_CODE == 9)	// $WRITELN
	{
		GetNextToken(ECHO_MODE);
		if (TOKEN_CODE == 34)	// $LPAR
		{
			GetNextToken(ECHO_MODE);

			ERROR_EXEMPT = true;
			ret = nt_simpExp();	// <simple expression>
			ERROR_EXEMPT = false;

			if (ret == -1)	// if simple expression returns 'failure to find terminal'
			{
				if ((ret = nt_ident()) == -1)	// <identifier>
				{
					if ((ret = nt_strConst()) == -1)	// <stringconst>
					{
						SyntaxError("(<simple expression> | <identifier> | <stringconst>)", FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
						return -1;
					}
				}
			}

			if (TOKEN_CODE == 35)	// $RPAR
			{
				QUAD_CODE.AddQuad(QUAD_RT.LookupName("PRINT"), ret, 0, 0);
				if (TRACE_INTERPRETER)
					TraceInterpreter(QUAD_CODE.NextQuad(), QUAD_CODE.GetQuad(QUAD_CODE.NextQuad() - 1));

				GetNextToken(ECHO_MODE);	// move along
			}
			else
			{
				SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(35), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
				return -1;
			}
		}
		else
		{
			SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(34), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
			return -1;
		}
	}
	else	// <block-body>
	{
		ret = nt_blockBody();
		if (ERROR)
			return -1;
	}

	if (TRACE_INTERPRETER)
		DEBUG(false, "<statement>");
	return ret;
}

// <identifier> [$LBRACK <simple expression> $RBRACK]
int nt_var()
{
	if (TRACE_CFG)
		DEBUG(true, "<variable>");
	
	int ret = -1;
	int var, value;
	string name;

	if ((var = nt_ident()) != -1)	// <identifier>
	{
		// [$LBRACK <simple expression> $RBRACK]
		if (TOKEN_CODE == 45)	// $LBRACK
		{
			GetNextToken(ECHO_MODE);
			value = nt_simpExp();	// <simple expression>
			if (ERROR)
				return -1;

			if (TOKEN_CODE == 46)	// $RBRACK
			{
				GetNextToken(ECHO_MODE);

				name = AllCaps(NEXT_TOKEN) + "[" + std::to_string(value) + "]";
				var = SYM_TAB.LookupSymbol(name);
			}
			else
			{
				SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(46), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
				return -1;
			}
		}

		ret = var;
	}

	if (TRACE_CFG)
		DEBUG(false, "<variable>");
	return ret;
}

// <label> -> <identifier>
int nt_label()
{
	if (TRACE_CFG)
		DEBUG(true, "<label>");

	int ret = -1;
	SEARCH_LABEL = true;
	ret = nt_ident();	// <identifier>
	SEARCH_LABEL = false;

	if (TRACE_CFG)
		DEBUG(false, "<label>");
	return ret;
}

// <relexpression> non-terminal function
// <relexpression> -> <simple expression> <relop> <simple expression>
int nt_relExp() 
{
	if (TRACE_CFG)
		DEBUG(true, "<relexpression>");

	int ret = -1;
	int left, right, relop, opcode, temp;

	left = nt_simpExp();
	if (ERROR)
		return -1;

	relop = nt_relop();
	if (relop == -1)
	{
		SyntaxError("<relop>", FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
		return -1;
	}

	right = nt_simpExp();
	if (ERROR)
		return -1;

	temp = GenSymbol();

	// set relop to true branch as default
	if (relop == 42)	// $EQ
		opcode = QUAD_RT.LookupName("BZ");
	else if (relop == 43) // $NEQ
		opcode = QUAD_RT.LookupName("BNZ");
	else if (relop == 39) // $LSS
		opcode = QUAD_RT.LookupName("BN");
	else if (relop == 38) // $GTR
		opcode = QUAD_RT.LookupName("BP");
	else if (relop == 41) // $LEQ
		opcode = QUAD_RT.LookupName("BNP");
	else	// $GEQ
		opcode = QUAD_RT.LookupName("BNN");

	QUAD_CODE.AddQuad(QUAD_RT.LookupName("SUB"), left, right, temp);
	QUAD_CODE.AddQuad(opcode, temp, 0, -1);	// op3 needs to be a pc counter
	if (TRACE_INTERPRETER)
	{
		TraceInterpreter(QUAD_CODE.NextQuad() - 1, QUAD_CODE.GetQuad(QUAD_CODE.NextQuad() - 2));
		TraceInterpreter(QUAD_CODE.NextQuad(), QUAD_CODE.GetQuad(QUAD_CODE.NextQuad() - 1));
	}

	ret = QUAD_CODE.NextQuad() - 1;	// return index of branch so it can be worked on in <statement>

	if (TRACE_CFG)
		DEBUG(false, "<relexpression>");
	return ret;
}

// <relop> non-terminal function
// <relop> -> $EQ | $LSS | $GTR | $NEQ | $LEQ | $GEQ
int nt_relop()
{
	if (TRACE_CFG)
		DEBUG(true, "<relop>");

	int ret = -1;

	if (TOKEN_CODE == 42 || TOKEN_CODE == 39 || TOKEN_CODE == 38 ||	// $EQ | $LSS | $GTR |
		TOKEN_CODE == 43 || TOKEN_CODE == 41 || TOKEN_CODE == 40)	// $NEQ | $LEQ | $GEQ
	{
		ret = TOKEN_CODE;
		GetNextToken(ECHO_MODE);
	}

	if (TRACE_CFG)
		DEBUG(false, "<relop>");
	return ret;
}

// <simple expression> -> [<sign>] <term> {<addop> <term>}*
int nt_simpExp()
{
	if (TRACE_CFG)
		DEBUG(true, "<simple expression>");

	int ret = -1;
	int left, right, signval, temp, opcode;

	signval = nt_sign();	// [<sign>]

	left = nt_term();	// <term>
	if (ERROR)
		return -1;

	if (signval == 33)	// negative token sign
	{
		QUAD_CODE.AddQuad(QUAD_RT.LookupName("MUL"), left, MINUS_1_INDEX, left);
		if (TRACE_INTERPRETER)
			TraceInterpreter(QUAD_CODE.NextQuad(), QUAD_CODE.GetQuad(QUAD_CODE.NextQuad() - 1));
	}

	while ((ret = nt_addop()) != -1)	// {<addop> <term>}*
	{
		if (ret == 32)	// +
			opcode = QUAD_RT.LookupName("ADD");
		else			// -
			opcode = QUAD_RT.LookupName("SUB");

		right = nt_term();
		if (ERROR)
			return -1;

		temp = GenSymbol();
		QUAD_CODE.AddQuad(opcode, left, right, temp);
		if (TRACE_INTERPRETER)
			TraceInterpreter(QUAD_CODE.NextQuad(), QUAD_CODE.GetQuad(QUAD_CODE.NextQuad() - 1));
		left = temp;
	}

	ret = left;

	if (TRACE_CFG)
		DEBUG(false, "<simple expression>");
	return ret;
}

// <addop> non-terminal function
// <addop> -> $PLUS | $MINUS
int nt_addop()
{
	if (TRACE_CFG)
		DEBUG(true, "<addop>");

	int ret = -1;

	if (TOKEN_CODE == 32 || TOKEN_CODE == 33)	// $PLUS || $MINUS
	{
		ret = TOKEN_CODE;
		GetNextToken(ECHO_MODE);
	}

	if (TRACE_CFG)
		DEBUG(false, "<addop>");
	return ret;
}

// <sign> non-terminal function
// <sign> -> $PLUS | $MINUS
int nt_sign()
{
	if (TRACE_CFG)
		DEBUG(true, "<sign>");

	int ret = -1;

	if (TOKEN_CODE == 32 || TOKEN_CODE == 33)
	{
		ret = TOKEN_CODE;
		GetNextToken(ECHO_MODE);
	}

	if (TRACE_CFG)
		DEBUG(false, "<sign>");
	return ret;
}

// <term> non-terminal function
// <term> -> <factor> {<mulop> <factor> }*
int nt_term()
{
	if (TRACE_CFG)
		DEBUG(true, "<term>");

	int ret = -1;
	int left, right, temp, opcode;

	left = nt_factor();	// <factor>
	if (ERROR)
		return -1;
	
	while ((ret = nt_mulop()) != -1)	// <mulop>
	{
		if (ret == RESERVED_WORDS.LookupName("/"))	// DIV
			opcode = QUAD_RT.LookupName("DIV");
		else	// MUL
			opcode = QUAD_RT.LookupName("MUL");

		right = nt_factor();	// <factor>
		if (ERROR)
			return -1;

		temp = GenSymbol();
		QUAD_CODE.AddQuad(opcode, left, right, temp);
		if (TRACE_INTERPRETER)
			TraceInterpreter(QUAD_CODE.NextQuad(), QUAD_CODE.GetQuad(QUAD_CODE.NextQuad() - 1));
		left = temp;
	}

	ret = left;

	if (TRACE_CFG)
		DEBUG(false, "<term>");
	return ret;
}

// <mulop> non-terminal function
// <mulop> -> $MULT | $DIVIDE
int nt_mulop()
{
	if (TRACE_CFG)
		DEBUG(true, "<mulop>");

	int ret = -1;

	if (TOKEN_CODE == 31 || TOKEN_CODE == 30)	// $MULTIPLY | $DIVIDE
	{
		ret = TOKEN_CODE;
		GetNextToken(ECHO_MODE);
	}

	if (TRACE_CFG)
		DEBUG(false, "<mulop>");
	return ret;
}

// <factor> non-terminal function
// <factor> -> <unsigned constant> | <variable> | $LPAR <simple expression> $RPAR
int nt_factor()
{
	if (TRACE_CFG)
		DEBUG(true, "<factor>");
	int ret = -1;

	ret = nt_unsignConst();
	if (ret == -1)
	{
		ret = nt_var();	// <variable>
		if (ret == -1)
		{
			if (TOKEN_CODE == 34)	// $LPAR
			{
				GetNextToken(ECHO_MODE);
				ret = nt_simpExp();
				if (ERROR)
					return -1;

				if (TOKEN_CODE == 35)	// $RPAR
				{
					GetNextToken(ECHO_MODE);
				}
				else
				{
					SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(34), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
					return -1;
				}
			}
			else
			{
				SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(34), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
				return -1;
			}
		}
	}

	if (TRACE_CFG)
		DEBUG(false, "<factor>");
	return ret;
}

// <type> non-terminal function
// <simple type> | $ARRAY $LBRACK $INTTYPE $RBRACK $OF $INTEGER
int nt_type()
{
	if (TRACE_CFG)
		DEBUG(true, "<type>");

	int ret = -1;
	int tokenError = -1;	// experimental variable for error checks
	int amount;

	if ((ret = nt_simpType()) == -1)	// <simple_type>
	{
		if (TOKEN_CODE == 25)	// $ARRAY
		{
			GetNextToken(ECHO_MODE);
			if (TOKEN_CODE == 45)	// $LBRACK
			{
				GetNextToken(ECHO_MODE);
				if (TOKEN_CODE == 51)	// $INTTYPE
				{
					amount = stoi(NEXT_TOKEN);
					GetNextToken(ECHO_MODE);
					if (TOKEN_CODE == 46)	// $RBRACK
					{
						GetNextToken(ECHO_MODE);
						if (TOKEN_CODE == 8)	// $OF
						{
							GetNextToken(ECHO_MODE);
							if (TOKEN_CODE == 1)	// $INTEGER
							{
								GenArray(AllCaps(NEXT_TOKEN), amount, 0);
								ret = SYM_TAB.LookupSymbol(AllCaps(NEXT_TOKEN));
								GetNextToken(ECHO_MODE);
							}
							else
								tokenError = 1;
						}
						else
							tokenError = 8;
					}
					else
						tokenError = 46;
				}
				else
					tokenError = 51;
			}
			else
				tokenError = 45;
		}
		else
			tokenError = 25;

		if (tokenError != -1)
		{
			SyntaxError(FOUR_CHAR_MNEMONIC.LookupCode(tokenError), FOUR_CHAR_MNEMONIC.LookupCode(TOKEN_CODE));
			return -1;
		}
	}

	if (TRACE_CFG)
		DEBUG(false, "<type>");
	return ret;
}

// <simple type> non-terminal function
// <simple type> -> $INTEGER | $FLOAT | $STRING
int nt_simpType()
{
	if (TRACE_CFG)
		DEBUG(true, "<simple type>");

	int ret = -1;

	if (TOKEN_CODE == 1 || TOKEN_CODE == 23 || TOKEN_CODE == 24)	// $INTEGER | $REAL | $STRING
	{
		ret = TOKEN_CODE;
		GetNextToken(ECHO_MODE);
	}

	if (TRACE_CFG)
		DEBUG(false, "<simple type>");
	return ret;
}

// <constant> non-terminal function
// <constant> ->[<sign>] <unsigned constant>
int nt_constant()
{
	if (TRACE_CFG)
		DEBUG(true, "<constant>");

	int ret = -1;
	int signval;

	signval = nt_sign();	// [<sign>]

	ret = nt_unsignConst();
	if (ERROR)
		return -1;

	// code to flip value if sign is negative
	if (signval == 33)	// negative token sign
	{
		QUAD_CODE.AddQuad(QUAD_RT.LookupName("MUL"), ret, MINUS_1_INDEX, ret);
		if (TRACE_INTERPRETER)
			TraceInterpreter(QUAD_CODE.NextQuad(), QUAD_CODE.GetQuad(QUAD_CODE.NextQuad() - 1));
	}

	if (TRACE_CFG)
		DEBUG(false, "<constant>");
	return ret;
}

// <unsigned constant> non-terminal function
// <unsigned constant> -> <unsigned number>
int nt_unsignConst()
{
	if (TRACE_CFG)
		DEBUG(true, "<unsigned constant>");

	int ret = nt_unsignNum();

	if (TRACE_CFG)
		DEBUG(false, "<unsigned constant>");
	return ret;
}

// <unsigned number> non-terminal function
// <unsigned number>->$REAL | $INTTYPE
int nt_unsignNum()
{
	if (TRACE_CFG)
		DEBUG(true, "<unsigned number>");

	int ret = -1;

	if (TOKEN_CODE == 52 || TOKEN_CODE == 51)	// $FLOATTYPE | $INTTYPE
	{
		ret = SYM_TAB.LookupSymbol(NEXT_TOKEN);
		GetNextToken(ECHO_MODE);
	}

	if (TRACE_CFG)
		DEBUG(false, "<unsigned number>");
	return ret;
}

// <stringconst> non-terminal function
// <stringconst> -> $STRINGTYPE
int nt_strConst()
{
	if (TRACE_CFG)
		DEBUG(true, "<constant>");

	int ret = -1;

	if (TOKEN_CODE == 53)	// $STRINGTYPE
	{
		ret = SYM_TAB.LookupSymbol(NEXT_TOKEN);
		GetNextToken(ECHO_MODE);
	}

	if (TRACE_CFG)
		DEBUG(false, "<constant>");
	return ret;
}

// <identifier> non-terminal function
// <identifier> -> $IDENTIFIER
int nt_ident()
{
	if (TRACE_CFG)
		DEBUG(true, "<identifier>");

	int ret = -1;

	if (TOKEN_CODE == 50)	// $IDENTIFIER
	{
		string temp = AllCaps(NEXT_TOKEN);
		ret = SYM_TAB.LookupSymbol(temp);
		if (SEARCH_LABEL && SYM_TAB.GetSymbol(ret).kind != 0)	// if looking for label and not a label return -1
			ret = -1;
		else
		{
			if (LABEL && DECLARATION)
			{
				CheckDecLabels();
				SYM_TAB.UpdateSymbol(ret, 0, "LABEL");
			}
			else if (DECLARATION)
				CheckDecVar();
			else if (LABEL)
				CheckLabel();
			else if (SEARCH_HEADER)
				PROGRAM_HEADER = AllCaps(NEXT_TOKEN);	// assign program header
			else
				CheckVar();
			GetNextToken(ECHO_MODE);
		}
	}

	if (TRACE_CFG)
		DEBUG(false, "<identifier>");
	return ret;
}

/*** END OF FUNCTIONS FOR SYNTAXICAL ANALYZER ***/

/*** FUNCTIONS TO CREATE DATA STRUCTURES FOR MAIN ***/

/* Function to open a file to be read for the compiler
*/
void InitInputFile(string name)
{
	// if the input file is opened with another file, close then open with new file
	if (INPUT_FILE.is_open())
		INPUT_FILE.close();

	LINE_NUM = 0;
	INPUT_FILE.open(name);
}

// Prints out all errors and warnings from the error list
void PrintErrorList()
{
	int size = errorList.size();

	cout << endl << size << " errors found" << endl;

	if (size > 0)
	{
		cout << "##### ERROR LIST #####" << endl;
		for (int i = 0; i < errorList.size(); i++)
		{
			cout << errorList[i] << endl;
		}
		cout << "##### END OF LIST #####" << endl;
	}
}

void InitQuadReserveTable(bool print)
{
	// set mnemonics for reserve table
	QUAD_RT.Add("STOP", 0);
	QUAD_RT.Add("DIV", 1);
	QUAD_RT.Add("MUL", 2);
	QUAD_RT.Add("SUB", 3);
	QUAD_RT.Add("ADD", 4);
	QUAD_RT.Add("MOV", 5);
	QUAD_RT.Add("STI", 6);
	QUAD_RT.Add("LDI", 7);
	QUAD_RT.Add("BNZ", 8);
	QUAD_RT.Add("BNP", 9);
	QUAD_RT.Add("BNN", 10);
	QUAD_RT.Add("BZ", 11);
	QUAD_RT.Add("BP", 12);
	QUAD_RT.Add("BN", 13);
	QUAD_RT.Add("BR", 14);
	QUAD_RT.Add("BINDR", 15);
	QUAD_RT.Add("PRINT", 16);

	if (print)
	{
		QUAD_RT.PrintReserveTable();
		cout << endl;
	}
}

void InitReservedWords(bool print)
{
	RESERVED_WORDS.Add("GOTO", 0);
	RESERVED_WORDS.Add("INTEGER", 1);
	RESERVED_WORDS.Add("TO", 2);
	RESERVED_WORDS.Add("DO", 3);
	RESERVED_WORDS.Add("IF", 4);
	RESERVED_WORDS.Add("THEN", 5);
	RESERVED_WORDS.Add("ELSE", 6);
	RESERVED_WORDS.Add("FOR", 7);
	RESERVED_WORDS.Add("OF", 8);
	RESERVED_WORDS.Add("WRITELN", 9);
	RESERVED_WORDS.Add("READLN", 10);
	RESERVED_WORDS.Add("BEGIN", 11);
	RESERVED_WORDS.Add("END", 12);
	RESERVED_WORDS.Add("VAR", 13);
	RESERVED_WORDS.Add("WHILE", 14);
	RESERVED_WORDS.Add("UNIT", 15);
	RESERVED_WORDS.Add("LABEL", 16);
	RESERVED_WORDS.Add("REPEAT", 17);
	RESERVED_WORDS.Add("UNTIL", 18);
	RESERVED_WORDS.Add("PROCEDURE", 19);
	RESERVED_WORDS.Add("DOWNTO", 20);
	RESERVED_WORDS.Add("FUNCTION", 21);
	RESERVED_WORDS.Add("RETURN", 22);
	RESERVED_WORDS.Add("REAL", 23);
	RESERVED_WORDS.Add("STRING", 24);
	RESERVED_WORDS.Add("ARRAY", 25);

	RESERVED_WORDS.Add("/", 30);
	RESERVED_WORDS.Add("*", 31);
	RESERVED_WORDS.Add("+", 32);
	RESERVED_WORDS.Add("-", 33);
	RESERVED_WORDS.Add("(", 34);
	RESERVED_WORDS.Add(")", 35);
	RESERVED_WORDS.Add(";", 36);
	RESERVED_WORDS.Add(":=", 37);
	RESERVED_WORDS.Add(">", 38);
	RESERVED_WORDS.Add("<", 39);
	RESERVED_WORDS.Add(">=", 40);
	RESERVED_WORDS.Add("<=", 41);
	RESERVED_WORDS.Add("=", 42);
	RESERVED_WORDS.Add("<>", 43);
	RESERVED_WORDS.Add(",", 44);
	RESERVED_WORDS.Add("[", 45);
	RESERVED_WORDS.Add("]", 46);
	RESERVED_WORDS.Add(":", 47);
	RESERVED_WORDS.Add(".", 48);

	if (print)
	{
		RESERVED_WORDS.PrintReserveTable();
		cout << endl;
	}
}

void InitFourCharMnemonics(bool print)
{
	FOUR_CHAR_MNEMONIC.Add("GOTO", 0);
	FOUR_CHAR_MNEMONIC.Add("INTR", 1);
	FOUR_CHAR_MNEMONIC.Add("_TO_", 2);
	FOUR_CHAR_MNEMONIC.Add("_DO_", 3);
	FOUR_CHAR_MNEMONIC.Add("_IF_", 4);
	FOUR_CHAR_MNEMONIC.Add("THEN", 5);
	FOUR_CHAR_MNEMONIC.Add("ELSE", 6);
	FOUR_CHAR_MNEMONIC.Add("FOR_", 7);
	FOUR_CHAR_MNEMONIC.Add("_OF_", 8);
	FOUR_CHAR_MNEMONIC.Add("WRIT", 9);
	FOUR_CHAR_MNEMONIC.Add("READ", 10);
	FOUR_CHAR_MNEMONIC.Add("BEGN", 11);
	FOUR_CHAR_MNEMONIC.Add("END_", 12);
	FOUR_CHAR_MNEMONIC.Add("VARI", 13);
	FOUR_CHAR_MNEMONIC.Add("WHIL", 14);
	FOUR_CHAR_MNEMONIC.Add("UNIT", 15);
	FOUR_CHAR_MNEMONIC.Add("LABL", 16);
	FOUR_CHAR_MNEMONIC.Add("REPT", 17);
	FOUR_CHAR_MNEMONIC.Add("UNTL", 18);
	FOUR_CHAR_MNEMONIC.Add("PRCD", 19);
	FOUR_CHAR_MNEMONIC.Add("DWNT", 20);
	FOUR_CHAR_MNEMONIC.Add("FUNC", 21);
	FOUR_CHAR_MNEMONIC.Add("RETN", 22);
	FOUR_CHAR_MNEMONIC.Add("REAL", 23);
	FOUR_CHAR_MNEMONIC.Add("STRI", 24);
	FOUR_CHAR_MNEMONIC.Add("ARRY", 25);
	FOUR_CHAR_MNEMONIC.Add("BKSL", 30);
	FOUR_CHAR_MNEMONIC.Add("ASTR", 31);
	FOUR_CHAR_MNEMONIC.Add("PLUS", 32);
	FOUR_CHAR_MNEMONIC.Add("DASH", 33);
	FOUR_CHAR_MNEMONIC.Add("LPAR", 34);
	FOUR_CHAR_MNEMONIC.Add("RPAR", 35);
	FOUR_CHAR_MNEMONIC.Add("SEMI", 36);
	FOUR_CHAR_MNEMONIC.Add("ASIN", 37);
	FOUR_CHAR_MNEMONIC.Add("GRTR", 38);
	FOUR_CHAR_MNEMONIC.Add("LESS", 39);
	FOUR_CHAR_MNEMONIC.Add("GTEQ", 40);
	FOUR_CHAR_MNEMONIC.Add("LTEQ", 41);
	FOUR_CHAR_MNEMONIC.Add("EQAL", 42);
	FOUR_CHAR_MNEMONIC.Add("DBBR", 43);
	FOUR_CHAR_MNEMONIC.Add("COMA", 44);
	FOUR_CHAR_MNEMONIC.Add("LBRK", 45);
	FOUR_CHAR_MNEMONIC.Add("RBRK", 46);
	FOUR_CHAR_MNEMONIC.Add("COLN", 47);
	FOUR_CHAR_MNEMONIC.Add("PERI", 48);
	FOUR_CHAR_MNEMONIC.Add("IDEN", 50);
	FOUR_CHAR_MNEMONIC.Add("CINT", 51);
	FOUR_CHAR_MNEMONIC.Add("CFLT", 52);
	FOUR_CHAR_MNEMONIC.Add("CSTR", 53);
	FOUR_CHAR_MNEMONIC.Add("UNKN", 99);

	if (print)
	{
		FOUR_CHAR_MNEMONIC.PrintReserveTable();
		cout << endl;
	}
}

void PrintAssembly()
{
	int instrCount = QUAD_CODE.NextQuad();
	Quad qHolder;
	for (int i = 0; i < instrCount; i++)
	{
		qHolder = QUAD_CODE.GetQuad(i);

		if (qHolder.opcode <= 14 && qHolder.opcode >= 8)
		{
			cout << "|" << setw(4) << i << "|" << setw(6) << QUAD_RT.LookupCode(qHolder.opcode) << "|" << setw(10)
				<< SYM_TAB.GetSymbol(qHolder.op1).symbol << "|" << setw(10) << SYM_TAB.GetSymbol(qHolder.op2).symbol
				<< "|" << setw(10) << "PC:" << qHolder.op3 << "|" << endl;
		}
		else
		{
			cout << "|" << setw(4) << i << "|" << setw(6) << QUAD_RT.LookupCode(qHolder.opcode) << "|" << setw(10)
				<< SYM_TAB.GetSymbol(qHolder.op1).symbol << "|" << setw(10) << SYM_TAB.GetSymbol(qHolder.op2).symbol
				<< "|" << setw(10) << SYM_TAB.GetSymbol(qHolder.op3).symbol << "|" << endl;
		}
	}
}

/*** END OF FUCNTIONS TO SET UP MAIN ***/	

int main()
{
	// Initailize Structures
	InitReservedWords(false);	// false so they don't print their tables
	InitFourCharMnemonics(false);
	InitQuadReserveTable(false);

	// Add constants to symbol table
	SYM_TAB.AddSymbol("---", 0, 0);		// for output purposes
	SYM_TAB.AddSymbol("Const 1", 1, 1);		// for use in the quad code generation process
	SYM_TAB.AddSymbol("Const -1", 1, -1);	// ^
	PLUS_1_INDEX = SYM_TAB.LookupSymbol("Const 1");		// set up global index locations for quad code generation use
	MINUS_1_INDEX = SYM_TAB.LookupSymbol("Const -1");

	// Initailize Input File
	InitInputFile("CS4100FinalProjGOODMinimumFA20.txt");

	PRINT_TOKENS = false;
	TRACE_CFG = false;
	TRACE_INTERPRETER = false;
	ECHO_MODE = true;
	GetNextToken(ECHO_MODE);

	nt_program();

	PrintErrorList();
	OutputVarUse();
	OutputLabelUse();

	// Initial table outputs
	cout << endl;
	SYM_TAB.PrintSymbolTable();
	QUAD_CODE.PrintQuadTable(SYM_TAB, QUAD_RT);
	//PrintAssembly();	// This was for personal debugging

	if (errorList.size() == 0)	// only run the interpreter if there is no errors
	{
		Interpreter myInterpreter;
		myInterpreter.InterpretQuads(QUAD_CODE, SYM_TAB, QUAD_RT, true);
		myInterpreter.PrintOutput();
	}

	// finished symbol table
	cout << endl;
	SYM_TAB.PrintSymbolTable();

	// close global input file if still open
	if (INPUT_FILE.is_open())
		INPUT_FILE.close();
}