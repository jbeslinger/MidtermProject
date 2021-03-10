/// Joshua Eslinger
/// CSC 376-C
/// Spring 2021
/// Midterm Project - PEP/8 Simulator

#include <iostream>
using namespace std;

#pragma region OPCODES
unsigned char   STOP  =   0b00000000 ;
unsigned char   BINV  =   0b0001100  ;
unsigned char   SHFL  =   0b0001110  ;
unsigned char   SHFR  =   0b0001111  ;
unsigned char   ROTL  =   0b0010000  ;
unsigned char   ROTR  =   0b0010001  ;
unsigned char   DECI  =   0b00110    ;
unsigned char   DECO  =   0b00111    ;
unsigned char   CHRI  =   0b01001    ;
unsigned char   CHRO  =   0b01010    ;
unsigned char   ADD   =   0b0111     ;
unsigned char   SUB   =   0b1000     ;
unsigned char   AND   =   0b1001     ;
unsigned char   OR    =   0b1010     ;
unsigned char   LDR   =   0b1100     ;
unsigned char   LDB   =   0b1101     ;
unsigned char   STR   =   0b1110     ;
unsigned char   STB   =   0b1111     ;
#pragma endregion

#pragma region STRUCTS
struct Registers
{
    unsigned short int A_X_PC_SP[4];
    unsigned long IR;
};

union CPU
{
    Registers r;
};
#pragma endregion

#pragma region FIELDS
unsigned char memory[65536];
CPU cpu;
#pragma endregion

int main()
{
    
}


