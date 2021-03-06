/// Joshua Eslinger
/// CSC 376-C
/// Spring 2021
/// Midterm Project - PEP/8 Simulator

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;


#pragma region Constants
#pragma region Register Locations
const unsigned char   A       =   0           ;
const unsigned char   X       =   1           ;
const unsigned char   PC      =   2           ;
const unsigned char   SP      =   3           ;
#pragma endregion

#pragma region Opcodes
const unsigned char   STOP    =   0b00000000  ;
const unsigned char   BINV    =   0b0001100   ;
const unsigned char   SHFL    =   0b0001110   ;
const unsigned char   SHFR    =   0b0001111   ;
const unsigned char   ROTL    =   0b0010000   ;
const unsigned char   ROTR    =   0b0010001   ;
const unsigned char   DECI    =   0b00110     ;
const unsigned char   DECO    =   0b00111     ;
const unsigned char   CHRI    =   0b01001     ;
const unsigned char   CHRO    =   0b01010     ;
const unsigned char   ADD     =   0b0111      ;
const unsigned char   SUB     =   0b1000      ;
const unsigned char   AND     =   0b1001      ;
const unsigned char   OR      =   0b1010      ;
const unsigned char   LDR     =   0b1100      ;
const unsigned char   LDB     =   0b1101      ;
const unsigned char   STR     =   0b1110      ;
const unsigned char   STB     =   0b1111      ;
#pragma endregion

#pragma region Addressing Modes
const unsigned char   IMD     =   0b000       ; // One's digit also stands for 'Immediate'
const unsigned char   DIR     =   0b001       ; // One's digit also stands for 'Indexed'
const unsigned char   INDR    =   0b010       ;
const unsigned char   SRL     =   0b011       ;
const unsigned char   SRLD    =   0b100       ;
const unsigned char   INDX    =   0b101       ;
const unsigned char   SNX     =   0b110       ;
const unsigned char   SNXD    =   0b111       ;
#pragma endregion
#pragma endregion


#pragma region Structures
#pragma region Opcode Specifier
struct Half
{
    unsigned char           r;
    unsigned char           l;
};

union Opspec
{
    Half                    half;
    unsigned short int      whole;
};
#pragma endregion

#pragma region Opcode
struct TripartiteForm // 0000 0 000
{
    unsigned char           aaa : 3;
    unsigned char           r   : 1;
    unsigned char           o   : 4;
};
struct BipartiteForm1 // 00000 000
{
    unsigned char           aaa : 3;
    unsigned char           o   : 5;
};
struct BipartiteForm2 // 0000000 0
{
    unsigned char           r  : 1;
    unsigned char           o  : 7;
};

union Opcode
{
    TripartiteForm          t;
    BipartiteForm1          b1;
    BipartiteForm2          b2;
    unsigned char           whole;
};
#pragma endregion

struct Instruction
{
    Opcode                  opcode;
    Opspec                  opspec;
};

struct Registers
{
    unsigned short int      A_X_PC_SP[4];
    Instruction             IR;
};

struct CPU
{
    Registers               reg;
};
#pragma endregion


#pragma region Fields
unsigned char   mem[65536]  ;
CPU             cpu         ;

string          program     ;
#pragma endregion


#pragma region Functions
// Convert Hexadecimal string and chars to decimal
int HexToDec(string hex)
{
    int out = 0;
    for (int d = 0; d < hex.length(); d++)
    {
        char digit = hex[hex.length() - (d + 1)];
        if (digit == 'A')
            out += 10 * pow(16, d);
        else if (digit == 'B')
            out += 11 * pow(16, d);
        else if (digit == 'C')
            out += 12 * pow(16, d);
        else if (digit == 'D')
            out += 13 * pow(16, d);
        else if (digit == 'E')
            out += 14 * pow(16, d);
        else if (digit == 'F')
            out += 15 * pow(16, d);
        else
            out += (digit - '0') * pow(16, d);
    }
    return out;
}
int HexToDec(char hex)
{
    string s(1, hex);
    return HexToDec(s);
}

// Convert decimal to string hexadecimal
string DecToHex(int dec)
{
    int remainder = 0;
    string out;

    if (dec == 0)
        out = "00";

    while (dec > 0)
    {
        remainder = dec % 16;
        switch (remainder)
        {
        case  0: out = '0' + out; break;
        case  1: out = '1' + out; break;
        case  2: out = '2' + out; break;
        case  3: out = '3' + out; break;
        case  4: out = '4' + out; break;
        case  5: out = '5' + out; break;
        case  6: out = '6' + out; break;
        case  7: out = '7' + out; break;
        case  8: out = '8' + out; break;
        case  9: out = '9' + out; break;
        case 10: out = 'A' + out; break;
        case 11: out = 'B' + out; break;
        case 12: out = 'C' + out; break;
        case 13: out = 'D' + out; break;
        case 14: out = 'E' + out; break;
        case 15: out = 'F' + out; break;
        }
        dec = dec / 16;
    }

    return out;
}
string DecToHex(int dec, int numberOfOutputDigits)
{
    int remainder = 0;
    string out;

    if (dec == 0)
        out = "00";

    while (dec > 0)
    {
        remainder = dec % 16;
        switch (remainder)
        {
        case  0: out = '0' + out; break;
        case  1: out = '1' + out; break;
        case  2: out = '2' + out; break;
        case  3: out = '3' + out; break;
        case  4: out = '4' + out; break;
        case  5: out = '5' + out; break;
        case  6: out = '6' + out; break;
        case  7: out = '7' + out; break;
        case  8: out = '8' + out; break;
        case  9: out = '9' + out; break;
        case 10: out = 'A' + out; break;
        case 11: out = 'B' + out; break;
        case 12: out = 'C' + out; break;
        case 13: out = 'D' + out; break;
        case 14: out = 'E' + out; break;
        case 15: out = 'F' + out; break;
        }
        dec = dec / 16;
    }

    // Padding out the hex number with leading zeroes
    int len = out.length();
    if (len < numberOfOutputDigits)
    {
        for (int x = 0; x < numberOfOutputDigits - len; x++)
            out = "0" + out;
    }

    return out;
}

// Simply adds whitespace every two characters to help with readability
string ReadOutHex(string hex)
{
    string out;
    
    for (int i = 0; i < hex.length(); i++)
    {
        out = out + hex[i];
        if ((i + 1) % 2 == 0)
            out = out + ' ';
    }
    
    return out;
}

// Gets rid of all whitespace in a string
string TrimWhitespace(string in)
{
    string out;

    for (int c = 0; c < in.length(); c++)
    {
        if (in[c] == ' ')
            continue;
        else
            out = out + in[c];
    }

    return out;
}

// Checks for valid Hex characters and will return an error message if needed
string IsValidHex(string in)
{
    if (in.length() % 2 != 0)
        return "\t!! SINGLE DIGIT HEX NUMBER DETECTED !! INPUT IGNORED";

    for (int i = 0; i < in.length(); i++)
    {
        char c = in[i];
        switch (c)
        {
        case '0': continue; break;
        case '1': continue; break;
        case '2': continue; break;
        case '3': continue; break;
        case '4': continue; break;
        case '5': continue; break;
        case '6': continue; break;
        case '7': continue; break;
        case '8': continue; break;
        case '9': continue; break;
        case 'A': continue; break;
        case 'B': continue; break;
        case 'C': continue; break;
        case 'D': continue; break;
        case 'E': continue; break;
        case 'F': continue; break;
        case ' ': continue; break;
        default: return "\t!! INVALID CHARACTER AT POSITION " + to_string(i) + " !! INPUT IGNORED";
        }
    }
    
    return "";
}

// Initiate an input loop for writing a machine code program
string WriteProgram()
{
    const char* GREETING = "Welcome to the PEP\\8 virtual computer!\n"
        "Please input 2-digit hex numbers to write your code; they may be spaced if you desire. Press return to endline.\n"
        "WARNING : Hex numbers are case sensitive so turn on Caps Lock.\n"
        "Terminate the program with exit code \'zz\' to run.\n"
        "You may also supply a .pepm file at the command line to run.\n\n"
        "--PROGRAM--";
    const string exitCode = "zz";

    string out;

    cout << GREETING << endl;

    // Program writing loop
    while (true)
    {
        string in;
        bool valid = false;
        
        cout << ">> "; getline(cin, in);

        if (in == exitCode)
            break;

        // Check for valid characters
        string msg = IsValidHex(TrimWhitespace(in));
        (msg == "") ? (valid = true) : (valid = false);

        if (valid)
            out = out + TrimWhitespace(in);
        else
            cout << msg << "\n";
    }

    return out;
}

// Load the program into system memory
void LoadProgram(string code)
{
    // Reach each byte one at a time
    unsigned char* addr = &mem[0];
    for (int l = 0; l < code.length(); l += 2)
    {
        string hex; hex += code[l]; hex += code[l + 1];
        *addr = HexToDec(hex);
        addr++;
    }
}

// Write a box to the console that shows the state of the CPU registers
void DisplayRegisters()
{
    cout << "|  A[0x" << DecToHex(cpu.reg.A_X_PC_SP[A], 4) << "] ";
        cout << "   X[0x" << DecToHex(cpu.reg.A_X_PC_SP[X], 4) << "] |" << endl;
    cout << "| PC[0x" << DecToHex(cpu.reg.A_X_PC_SP[PC], 4) << "] ";
        cout << "  SP[0x" << DecToHex(cpu.reg.A_X_PC_SP[SP], 4) << "] |" << endl;
    cout << "|      IR[" << DecToHex(cpu.reg.IR.opcode.whole, 8) << "]       |" << endl;
    cout << endl;
}

// A loop to run the program and display internals
string RunProgram()
{
    cout << "--RUNNING--\n";

    unsigned short int* a  = &cpu.reg.A_X_PC_SP[A ];
    unsigned short int* x  = &cpu.reg.A_X_PC_SP[X ];
    unsigned short int* pc = &cpu.reg.A_X_PC_SP[PC];
    unsigned short int* sp = &cpu.reg.A_X_PC_SP[SP];
    
    *pc = 0x0000;
    *sp = 0xFFF8;

    // A string for building the output of the program
    string out = "";

    DisplayRegisters();

    bool running = true;
    do
    {
        // Fetch the instruction and put it into IR
        cpu.reg.IR.opcode.whole = mem[*pc];

        // Decode and then execute the instruction
        if (cpu.reg.IR.opcode.whole == STOP)
            running = false;
        else if (cpu.reg.IR.opcode.b2.o == BINV)
        {
            unsigned short int* reg = &cpu.reg.A_X_PC_SP[cpu.reg.IR.opcode.b2.r];
            *reg = 0b0000000000000101;
            *reg = ~(*reg);
            *pc += 1;
        }
        else if (cpu.reg.IR.opcode.b2.o == SHFL)
        {
            unsigned short int* reg = &cpu.reg.A_X_PC_SP[cpu.reg.IR.opcode.b2.r];
            *reg = *reg << 1;
            *pc += 1;
        }
        else if (cpu.reg.IR.opcode.b2.o == SHFR)
        {
            unsigned short int* reg = &cpu.reg.A_X_PC_SP[cpu.reg.IR.opcode.b2.r];
            *reg = *reg >> 1;
            *pc += 1;
        }
        else if (cpu.reg.IR.opcode.b2.o == ROTL)
        {
            unsigned short int* reg = &cpu.reg.A_X_PC_SP[cpu.reg.IR.opcode.b2.r];
            *reg = (*reg << 1) | (*reg >> ((sizeof(*reg) * 8) - 1));
            *pc += 1;
        }
        else if (cpu.reg.IR.opcode.b2.o == ROTR)
        {
            unsigned short int* reg = &cpu.reg.A_X_PC_SP[cpu.reg.IR.opcode.b2.r];
            *reg = (*reg >> 1) | (*reg << ((sizeof(*reg) * 8) - 1));
            *pc += 1;
        }
        else if (cpu.reg.IR.opcode.b1.o == DECI)
        {
            cpu.reg.IR.opspec.half.l = mem[*pc + 1]; cpu.reg.IR.opspec.half.r = mem[*pc + 2];
            switch (cpu.reg.IR.opcode.b1.aaa)
            {
            case DIR:
                int i; cout << ">> "; cin >> i;
                mem[cpu.reg.IR.opspec.whole] = HexToDec(DecToHex(i));
                break;
            default:
                return "\t!! ILLEGAL ADDRESSING MODE - PROGRAM TERMINATED !!\n\n";
            }
            *pc += 3;
        }
        else if (cpu.reg.IR.opcode.b1.o == DECO)
        {
            cpu.reg.IR.opspec.half.l = mem[*pc + 1]; cpu.reg.IR.opspec.half.r = mem[*pc + 2];
            switch (cpu.reg.IR.opcode.b1.aaa)
            {
            case IMD:
                out += to_string(cpu.reg.IR.opspec.whole);
                break;
            case DIR:
                out += to_string(mem[cpu.reg.IR.opspec.whole]);
                break;
            default:
                return "\t!! ILLEGAL ADDRESSING MODE - PROGRAM TERMINATED !!\n\n";
            }
            *pc += 3;
        }
        else if (cpu.reg.IR.opcode.b1.o == CHRI)
        {
            cpu.reg.IR.opspec.half.l = mem[*pc + 1]; cpu.reg.IR.opspec.half.r = mem[*pc + 2];
            switch (cpu.reg.IR.opcode.b1.aaa)
            {
            case DIR:
                cout << ">> ";
                char in;
                cin >> in;
                mem[cpu.reg.IR.opspec.whole] = HexToDec(DecToHex(in));
                break;
            default:
                return "\t!! ILLEGAL ADDRESSING MODE - PROGRAM TERMINATED !!\n\n";
            }
            *pc += 3;
        }
        else if (cpu.reg.IR.opcode.b1.o == CHRO)
        {
            cpu.reg.IR.opspec.half.l = mem[*pc + 1]; cpu.reg.IR.opspec.half.r = mem[*pc + 2];
            switch (cpu.reg.IR.opcode.b1.aaa)
            {
            case IMD:
                out += cpu.reg.IR.opspec.whole;
                break;
            case DIR:
                out += mem[cpu.reg.IR.opspec.whole];
                break;
            default:
                return "\t!! ILLEGAL ADDRESSING MODE - PROGRAM TERMINATED !!\n\n";
            }
            *pc += 3;
        }
        else if (cpu.reg.IR.opcode.t.o  == ADD )  
        {
            cpu.reg.IR.opspec.half.l = mem[*pc + 1]; cpu.reg.IR.opspec.half.r = mem[*pc + 2];
            switch (cpu.reg.IR.opcode.t.aaa)
            {
            case IMD:
                cpu.reg.A_X_PC_SP[cpu.reg.IR.opcode.t.r] += cpu.reg.IR.opspec.whole;
                break;
            case DIR:
                cpu.reg.A_X_PC_SP[cpu.reg.IR.opcode.t.r] += mem[cpu.reg.IR.opspec.whole];
                break;
            default:
                return "\t!! ILLEGAL ADDRESSING MODE - PROGRAM TERMINATED !!\n\n";
            }
            *pc += 3;
        }
        else if (cpu.reg.IR.opcode.t.o  == SUB )  
        {
            cpu.reg.IR.opspec.half.l = mem[*pc + 1]; cpu.reg.IR.opspec.half.r = mem[*pc + 2];
            switch (cpu.reg.IR.opcode.t.aaa)
            {
            case IMD:
                cpu.reg.A_X_PC_SP[cpu.reg.IR.opcode.t.r] -= cpu.reg.IR.opspec.whole;
                break;
            case DIR:
                cpu.reg.A_X_PC_SP[cpu.reg.IR.opcode.t.r] -= mem[cpu.reg.IR.opspec.whole];
                break;
            default:
                return "\t!! ILLEGAL ADDRESSING MODE - PROGRAM TERMINATED !!\n\n";
            }
            *pc += 3;
        }
        else if (cpu.reg.IR.opcode.t.o  == AND )  
        {
            cpu.reg.IR.opspec.half.l = mem[*pc + 1]; cpu.reg.IR.opspec.half.r = mem[*pc + 2];
            switch (cpu.reg.IR.opcode.t.aaa)
            {
            case IMD:
                cpu.reg.A_X_PC_SP[cpu.reg.IR.opcode.t.r] &= cpu.reg.IR.opspec.whole;
                break;
            case DIR:
                Opspec opr;
                opr.half.l = mem[cpu.reg.IR.opspec.whole]; opr.half.r = mem[cpu.reg.IR.opspec.whole + 1];
                cpu.reg.A_X_PC_SP[cpu.reg.IR.opcode.t.r] &= opr.whole;
                break;
            default:
                return "\t!! ILLEGAL ADDRESSING MODE - PROGRAM TERMINATED !!\n\n";
            }
            *pc += 3;
        }
        else if (cpu.reg.IR.opcode.t.o  == OR  )   
        {
            cpu.reg.IR.opspec.half.l = mem[*pc + 1]; cpu.reg.IR.opspec.half.r = mem[*pc + 2];
            switch (cpu.reg.IR.opcode.t.aaa)
            {
            case IMD:
                cpu.reg.A_X_PC_SP[cpu.reg.IR.opcode.t.r] |= cpu.reg.IR.opspec.whole;
                break;
            case DIR:
                Opspec opr;
                opr.half.l = mem[cpu.reg.IR.opspec.whole]; opr.half.r = mem[cpu.reg.IR.opspec.whole + 1];
                cpu.reg.A_X_PC_SP[cpu.reg.IR.opcode.t.r] |= opr.whole;
                break;
            default:
                return "\t!! ILLEGAL ADDRESSING MODE - PROGRAM TERMINATED !!\n\n";
            }
            *pc += 3;
        }
        else if (cpu.reg.IR.opcode.t.o  == LDR )  
        {
            cpu.reg.IR.opspec.half.l = mem[*pc + 1]; cpu.reg.IR.opspec.half.r = mem[*pc + 2];
            switch (cpu.reg.IR.opcode.t.aaa)
            {
            case IMD:
                cpu.reg.A_X_PC_SP[cpu.reg.IR.opcode.t.r] = cpu.reg.IR.opspec.whole;
                break;
            case DIR:
                Opspec opr;
                opr.half.l = mem[cpu.reg.IR.opspec.whole]; opr.half.r = mem[cpu.reg.IR.opspec.whole + 1];
                cpu.reg.A_X_PC_SP[cpu.reg.IR.opcode.t.r] = opr.whole;
                break;
            default:
                return "\t!! ILLEGAL ADDRESSING MODE - PROGRAM TERMINATED !!\n\n";
            }
            *pc += 3;
        }
        else if (cpu.reg.IR.opcode.t.o  == LDB )  
        {
            cpu.reg.IR.opspec.half.l = mem[*pc + 1]; cpu.reg.IR.opspec.half.r = mem[*pc + 2];
            switch (cpu.reg.IR.opcode.t.aaa)
            {
            case IMD:
                cpu.reg.A_X_PC_SP[cpu.reg.IR.opcode.t.r] = cpu.reg.IR.opspec.half.r;
                break;
            case DIR:
                cpu.reg.A_X_PC_SP[cpu.reg.IR.opcode.t.r] = mem[cpu.reg.IR.opspec.whole];
                break;
            default:
                return "\t!! ILLEGAL ADDRESSING MODE - PROGRAM TERMINATED !!\n\n";
            }
            *pc += 3;
        }
        else if (cpu.reg.IR.opcode.t.o  == STR )  
        {
            cpu.reg.IR.opspec.half.l = mem[*pc + 1]; cpu.reg.IR.opspec.half.r = mem[*pc + 2];
            switch (cpu.reg.IR.opcode.t.aaa)
            {
            case DIR:
                Opspec o; o.whole = cpu.reg.A_X_PC_SP[cpu.reg.IR.opcode.t.r];
                mem[cpu.reg.IR.opspec.half.l] = o.half.l; mem[cpu.reg.IR.opspec.half.r] = o.half.r;
                break;
            default:
                return "\t!! ILLEGAL ADDRESSING MODE - PROGRAM TERMINATED !!\n\n";
            }
            *pc += 3;
        }
        else if (cpu.reg.IR.opcode.t.o  == STB )  
        {
            cpu.reg.IR.opspec.half.l = mem[*pc + 1]; cpu.reg.IR.opspec.half.r = mem[*pc + 2];
            switch (cpu.reg.IR.opcode.t.aaa)
            {
            case DIR:
                mem[cpu.reg.IR.opspec.whole] = cpu.reg.A_X_PC_SP[cpu.reg.IR.opcode.t.r];
                break;
            default:
                return "\t!! ILLEGAL ADDRESSING MODE - PROGRAM TERMINATED !!\n\n";
            }
            *pc += 3;
        }
        else
            return "\t!! INVALID INSTRUCTION AT PC 0x" + DecToHex(cpu.reg.A_X_PC_SP[PC], 4) + " - PROGRAM TERMINATED !!\n\n";

        DisplayRegisters();
    } while (running);

    return out + "\n\n";
}
#pragma endregion


int main(int argc, char* argv[])
{
    string program;

    // If the user does not provide any arguments
    if (argc == 1)
    {
    program = WriteProgram();
    }
    // If the user provides a valid file on the command line
    else if (argc == 2)
    {
        try
        {
            // Load the program directly from the file provided
            string filename = argv[1];
            ifstream file(filename);
            ostringstream ss;
            ss << file.rdbuf();
            program = ss.str();
            program.erase(remove(program.begin(), program.end(), '\n'), program.end()); // Remove all newlines
            program.erase(remove(program.begin(), program.end(),  ' '), program.end()); // Remove all whitespace
        }
        catch (int e)
        {
            cout << "Inputted file was either not valid." << endl;
            return e; // Close program
        }
    }
    // If the user provides too many arguments
    else
    {
        cout << "Provided arguments are invalid. Ignored." << endl;
        return 1; // Close program
    }
    

    LoadProgram(program);
    cout << endl;
    cout << RunProgram();
    
    system("pause");

    return 0;
}
