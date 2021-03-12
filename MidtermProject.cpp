/// Joshua Eslinger
/// CSC 376-C
/// Spring 2021
/// Midterm Project - PEP/8 Simulator

#include <fstream>
#include <iostream>
#include <string>
using namespace std;


#pragma region CONSTANTS
unsigned char   A       =   0           ;
unsigned char   X       =   1           ;
unsigned char   PC      =   2           ;
unsigned char   SP      =   3           ;

#pragma region INSTRUCTION SPECIFIERS
unsigned char   STOP    =   0b00000000  ;
unsigned char   BINV    =   0b0001100   ;
unsigned char   SHFL    =   0b0001110   ;
unsigned char   SHFR    =   0b0001111   ;
unsigned char   ROTL    =   0b0010000   ;
unsigned char   ROTR    =   0b0010001   ;
unsigned char   DECI    =   0b00110     ;
unsigned char   DECO    =   0b00111     ;
unsigned char   CHRI    =   0b01001     ;
unsigned char   CHRO    =   0b01010     ;
unsigned char   ADD     =   0b0111      ;
unsigned char   SUB     =   0b1000      ;
unsigned char   AND     =   0b1001      ;
unsigned char   OR      =   0b1010      ;
unsigned char   LDR     =   0b1100      ;
unsigned char   LDB     =   0b1101      ;
unsigned char   STR     =   0b1110      ;
unsigned char   STB     =   0b1111      ;
#pragma endregion

#pragma region ADDRESSING MODES
unsigned char   IMD     =   0b000       ; // One's digit also stands for 'Immediate'
unsigned char   DIR     =   0b001       ; // One's digit also stands for 'Indexed'
unsigned char   INDR    =   0b010       ;
unsigned char   SRL     =   0b011       ;
unsigned char   SRLD    =   0b100       ;
unsigned char   INDX    =   0b101       ;
unsigned char   SNX     =   0b110       ;
unsigned char   SNXD    =   0b111       ;
#pragma endregion
#pragma endregion

#pragma region STRUCTURES
struct OpHalf
{
    unsigned short int      l;
    unsigned short int      r;
};

union Operand
{
    OpHalf                  h;
    unsigned int            w;
};

struct Tern // 0000 0 000
{
    unsigned char c : 3;
    unsigned char b : 1;
    unsigned char a : 4;
};
struct Bin1 // 00000 000
{
    unsigned char b : 3;
    unsigned char a : 5;
};
struct Bin2 // 0000000 0
{
    unsigned char b : 1;
    unsigned char a : 7;
};

union UnaryInstruction
{
    Tern                    t;
    Bin1                    b1;
    Bin2                    b2;
    unsigned char           whole;
};

struct BinaryInstruction
{
    UnaryInstruction        i;
    Operand                 op;
};

struct Registers
{
    unsigned short int      A_X_PC_SP[4];
    unsigned long           IR : 24;
};

struct CPU
{
    Registers reg;
};
#pragma endregion

#pragma region INSTRUCTIONS
void BitwiseInvert(){ };
void ShiftLeft(){ };
void ShiftRight(){ };
void RotateLeft(){ };
void RotateRight(){ };
void DecimIn(){ };
void DecimOut(){ };
void CharIn(){ };
void CharOut(){ };
void Add(){ };
void Subtract(){ };
void BitwiseAnd(){ };
void BitwiseOr(){ };
void LoadFromMemory(){ };
void LoadByteFromMemory(){ };
void StoreToMemory(){ };
void StoreByteToMemory(){ };
#pragma endregion

#pragma region FIELDS
unsigned char   mem[65536]  ;
CPU             cpu         ;

string          program     ;
#pragma endregion

#pragma region FUNCTIONS
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
        return "00";

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
string IsValidHex(string in, int length)
{
    if (in.length() != length)
        return "!! INSTRUCTION MUST BE 3 HEX PAIRS SEPARATED BY WHITESPACE !!";
    else
    {
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
            default: return "!! INVALID CHARACTER AT POSITION " + to_string(i) + " !!";
            }
        }
        return "";
    }
}

// Initiate an input loop for writing a machine code program
string WriteProgram()
{
    const char* GREETING = "Welcome to the PEP\\8 virtual computer!\n"
        "Please input one 6-digit hex instruction at a time in this format: \'FF FF FF\'\n"
        "WARNING : Hex numbers are case sensitive so turn on Caps Lock.\n"
        "Terminate the program with exit code \'00 00 00\' to run.\n"
        "You may also supply a .pepm file at the command line to run.\n\n"
        "--PROGRAM--";
    const string exitCode = "00 00 00";

    string out;

    cout << GREETING << endl;

    // Program writing loop
    while (true)
    {
        string in;
        bool valid = false;
        
        cout << ">> "; getline(cin, in);

        // Check for valid characters
        string msg = IsValidHex(in, 8);
        (msg == "") ? (valid = true) : (valid = false);

        if (valid)
            out = out + TrimWhitespace(in);
        else
            cout << msg << "\n";

        if (in == exitCode)
            break;
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

// Find and execute a given instruction
void ExecuteInstruction(UnaryInstruction i)
{

}
void ExecuteInstruction(BinaryInstruction i)
{

}

// A loop to run the program and display internals
void RunProgram()
{
    cout << "--PROGRAM--\n";

    // Increment
    unsigned short int* pc = &cpu.reg.A_X_PC_SP[PC];
    *pc = 0x000000;
    
    while (true)
    {
        // Fetch
        
    }
}
#pragma endregion


int main()
{
    LoadProgram("4900240E49002400");
    RunProgram();
}
