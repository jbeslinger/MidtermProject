/// Joshua Eslinger
/// CSC 376-C
/// Spring 2021
/// Midterm Project - PEP/8 Simulator

#include <iostream>
#include <string>
using namespace std;


#pragma region FUNCTIONS
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

void ExecuteProgram(string code)
{

}
#pragma endregion

#pragma region CONSTANTS
unsigned char ACC_REG  =   0          ;
unsigned char NDX_REG  =   1          ;

#pragma region INSTRUCTION SPECIFIERS
unsigned char   STOP   =   0b00000000 ;
unsigned char   BINV   =   0b0001100  ;
unsigned char   SHFL   =   0b0001110  ;
unsigned char   SHFR   =   0b0001111  ;
unsigned char   ROTL   =   0b0010000  ;
unsigned char   ROTR   =   0b0010001  ;
unsigned char   DECI   =   0b00110    ;
unsigned char   DECO   =   0b00111    ;
unsigned char   CHRI   =   0b01001    ;
unsigned char   CHRO   =   0b01010    ;
unsigned char   ADD    =   0b0111     ;
unsigned char   SUB    =   0b1000     ;
unsigned char   AND    =   0b1001     ;
unsigned char   OR     =   0b1010     ;
unsigned char   LDR    =   0b1100     ;
unsigned char   LDB    =   0b1101     ;
unsigned char   STR    =   0b1110     ;
unsigned char   STB    =   0b1111     ;
#pragma endregion

#pragma region ADDRESSING MODES
unsigned char   IMD    =   0b000      ; // One's digit is also Immediate
unsigned char   DIR    =   0b001      ; // One's digit is also Indexed
unsigned char   INDR   =   0b010      ;
unsigned char   SRL    =   0b011      ;
unsigned char   SRLD   =   0b100      ;
unsigned char   INDX   =   0b101      ;
unsigned char   SNX    =   0b110      ;
unsigned char   SNXD   =   0b111      ;
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
    unsigned char c     :   3;
    unsigned char b     :   1;
    unsigned char a     :   4;
};
struct Bin1 // 00000 000
{
    unsigned char b     :   3;
    unsigned char a     :   5;
};
struct Bin2 // 0000000 0
{
    unsigned char b     :   1;
    unsigned char a     :   7;
};

union UnaryInstruction
{
    Tern                    t;
    Bin1                    b1;
    Bin2                    b2;
    unsigned char           w;
};

struct BinaryInstruction
{
    UnaryInstruction        n;
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

#pragma region FIELDS
unsigned char   mem[65536]  ;
CPU             cpu         ;
#pragma endregion


int main()
{

}
