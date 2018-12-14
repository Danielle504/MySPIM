// Names: Juan Parra, Rasha Rahaman, Danielle Garsten
// Description: Create a MIPS Simulator using C language
// Course: CDA 3103
// Deadline: November 30, 2018

#include "spimcore.h"
#include <stdio.h>

/* ALU */
/* 10 Points */

void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult, char *Zero)
{

// switches between ALU controls
  switch (ALUControl)
  {
    // adds A and B
      case 0:
        *ALUresult = A + B;
        break;

    // subtracts b from A
      case 1:
        *ALUresult = A - B;
        break;

    // If case for signed functions of A and B
      case 2:
        *ALUresult = ((signed)A < (signed)B) ? 1 : 0;
        break;

    // if case for unsigned functions of A and B
      case 3:
        *ALUresult = (A < B) ? 1 : 0;
        break;

    // result equals A and B
      case 4:
        *ALUresult = A & B;
        break;

    // result equals A or B
      case 5:
        *ALUresult = A | B;
        break;

    // result shifts left by 16 positions
      case 6:
        *ALUresult = B << 16;
        break;

    // result is not A
      case 7:
        *ALUresult = ~A;
        break;

    // default breaks the function if none of the above occur
      default:
        break;
  }

  if(A-B == 0)
    *Zero = 1;

  else
    *Zero = 0;

}

/* instruction fetch */
/* 10 Points */

int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction)
{
  // stores the memory by multiplying by 4
  unsigned i = PC >> 2;
  // i = 4 * i;

  // if the memory is not divisble by 4, 1 is returned
  if (PC % 4 != 0)
    return 1;

  // instructions is now a stored array
  *instruction = Mem[i];

  return 0;

}

/* instruction partition */
/* 10 Points */

void instruction_partition(unsigned instruction, unsigned *op, unsigned
*r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset,
unsigned *jsec)
{

  *op = (instruction >> 26) & 0b00000000000000000000000000111111; // instruction [31-26]
  *r1 = (instruction >> 21) & 0b11111;                            // instruction [25-21]
  *r2 = (instruction >> 16) & 0b11111;                            // instruction [20-16]
  *r3 = (instruction >> 11) & 0b11111;                            // instruction [15-11]
  *funct = instruction & 0b00000000000000000000000000111111;      // instruction [5-0]
  *offset = instruction & 0b00000000000000001111111111111111;     // instruction [15-0]
  *jsec = instruction & 0b00000011111111111111111111111111;       // instruction [25-0]

}

/* instruction decode */
/* 15 Points */

int instruction_decode(unsigned op, struct_controls *controls)
{
  switch (op)
  {
    // controls->ALUOp = 0; // addition or dont care
    // controls->ALUOp = 1; // subtraction
    // controls->ALUOp = 2; // set less than opeation
    // controls->ALUOp = 3; // set less than unsigned operation
    // controls->ALUOp = 4; // and operation
    // controls->ALUOp = 5; // or operation
    // controls->ALUOp = 6; // shift left instruction
    // controls->ALUOp = 7; //r type instruction // CHECKED

    // these are the necessary functions we need to "represent"
    // r type
    case 0:
      controls->RegDst = 1;
      controls->Jump = 0;
      controls->Branch = 0;
      controls->MemRead = 0;
      controls->MemtoReg = 0;
      controls->ALUOp = 7;
      controls->MemWrite = 0;
      controls->ALUSrc = 0;
      controls->RegWrite = 1;
      break;

    // add immediate
    case 8:
      controls->RegDst = 0;
      controls->Jump = 0;
      controls->Branch = 0;
      controls->MemRead = 0;
      controls->MemtoReg = 0;
      controls->ALUOp = 0;
      controls->MemWrite = 0;
      controls->ALUSrc = 1;
      controls->RegWrite = 1;
      break;

    // load word
    case 35:
      controls->RegDst = 0;
      controls->Jump =  0;
      controls->Branch = 0;
      controls->MemRead = 1;
      controls->MemtoReg = 1;
      controls->ALUOp = 0;
      controls->MemWrite = 0;
      controls->ALUSrc = 1;
      controls->RegWrite = 1;
      break;

    // lood upper immediate
    case 15:
      controls->RegDst = 0;
      controls->Jump =  0;
      controls->Branch = 0;
      controls->MemRead = 0;
      controls->MemtoReg = 0;
      controls->ALUOp = 6;
      controls->MemWrite = 0;
      controls->ALUSrc = 1;
      controls->RegWrite = 1;
      break;

    // store word
    case 43:
      controls->RegDst = 2;
      controls->Jump =  0;
      controls->Branch = 0;
      controls->MemRead = 0;
      controls->MemtoReg = 0;
      controls->ALUOp = 0;
      controls->MemWrite = 1;
      controls->ALUSrc = 1;
      controls->RegWrite = 0;
      break;

    // branch on equal
    case 4:
      controls->RegDst = 2;
      controls->Jump = 0;
      controls->Branch = 1;
      controls->MemRead = 0;
      controls->MemtoReg = 0;
      controls->ALUOp = 1;
      controls->MemWrite = 0;
      controls->ALUSrc = 0;
      controls->RegWrite = 0;
      break;

    // set less than immediate
    case 10:
      controls->RegDst = 0;
      controls->Jump =  0;
      controls->Branch = 0;
      controls->MemRead = 0;
      controls->MemtoReg = 0;
      controls->ALUOp = 2;
      controls->MemWrite = 0;
      controls->ALUSrc = 1;
      controls->RegWrite = 1;
      break;

    // set less than unsigned
    case 11:
      controls->RegDst = 0;
      controls->Jump =  0;
      controls->Branch = 2;
      controls->MemRead = 0;
      controls->MemtoReg = 0;
      controls->ALUOp = 3;
      controls->MemWrite = 0;
      controls->ALUSrc = 1;
      controls->RegWrite = 1;
      break;

    // jump
    case 2:
      controls->RegDst = 2;
      controls->Jump =  1;
      controls->Branch = 0;
      controls->MemRead = 0;
      controls->MemtoReg = 2;
      controls->ALUOp = 0;
      controls->MemWrite = 0;
      controls->ALUSrc = 1;
      controls->RegWrite = 0;
      break;

    default:
      return 1;
  }

  return 0;
}

/* Read Register */
/* 5 Points */

void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
  // stores data in an array
  *data1 = Reg[r1];
  *data2 = Reg[r2];
}

/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset, unsigned *extended_value)
{
    unsigned sign = offset >> 15;


    // this will check if its negative (looking for a 1)
    if (sign == 1)
      *extended_value = offset | 0xffff0000;

    // if positive (looking for a 0)
    else
    {
      *extended_value = offset & 0x0000ffff;
      // pow(2,16) is 65536 so we need to subtract by 1
    }
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned
extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned
*ALUresult,char *Zero)
{
  if (ALUOp < 0 || ALUOp > 7)
    return 1;

  if (ALUOp == 7)
  {
    switch (funct)
    {
      // add
      case 32:
        ALUOp = 0;
        break;

      // subtract
      case 34:
        ALUOp = 1;
        break;

      // SLT
      case 42:
          ALUOp = 2;
          break;

      // SLTU
      case 43:
        ALUOp = 3;
        break;

      // AND
      case 36:
        ALUOp = 4;
        break;

      // OR
      case 37:
        ALUOp = 5;
        break;

      // shift left extended_value by 16 bits
      case 39:
        ALUOp = 6;
        break;

      default:
        return 1;
    }
  }

  // we need to call the ALU function from above
  if (ALUSrc == 1)
    ALU(data1, extended_value, ALUOp, ALUresult, Zero);

  else
    ALU(data1, data2, ALUOp, ALUresult, Zero);

    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char
MemRead, unsigned *memdata, unsigned *Mem)
{
  int check = 0;

  // write into memory
  if (MemWrite == 1)
  {
    if (ALUresult % 4 == 0)
      Mem[ALUresult >> 2] = data2;
    else
      check = 1;
  }

  // read into memory
  if (MemRead == 1)
  {
    if(ALUresult % 4 == 0)
      *memdata = Mem[ALUresult >> 2];

    else
      check = 1;
  }

    return (check == 0) ? 0 : 1;
}

/* Write Register */
/* 10 Points */
void write_register(unsigned r2, unsigned r3, unsigned memdata, unsigned
ALUresult, char RegWrite, char RegDst, char MemtoReg, unsigned *Reg)
{
  // the register Reg uses r2 and r3
  // write the data (ALUresult or memdata) to a register reg addressed by r2 or r3
  if (RegWrite == 1)
  {
    if (MemtoReg == 1)
    {
      // data coming from memory is memdata
      if (RegDst == 0)
        Reg[r2] = memdata;

      else
        Reg[r3] = memdata;
    }

    if (MemtoReg == 0)
    {
      if (RegDst == 1)
        Reg[r3] = ALUresult;

      else
        Reg[r2] = ALUresult;
    }
  }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char
Jump, char Zero,unsigned *PC)
{

  // Updates the counter
  *PC += 4;

  // If both are equal to then add the extended value 4 times
  if (Branch == 1 && Zero == 1)
    *PC += (extended_value << 2);

  // if the condition is true, then jump to the register
  // and multiple the jump register by 4
  if (Jump == 1)
    *PC = (*PC & 0xf0000000) | (jsec << 2);

}
