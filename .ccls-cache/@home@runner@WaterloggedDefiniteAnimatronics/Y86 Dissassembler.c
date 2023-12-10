#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int convertStrToByteCode(const char *str, unsigned char inst[], int size);

int main(int argc, char **argv)
{
  FILE *pFile = NULL;

  char buffer[15];
  char hex = NULL;

  if (argc < 2)
  {
    pFile = fopen("./test1.txt", "r");
  }
  else
  {
    pFile = fopen(argv[1], "r");
  }

  if (pFile == NULL)
  {
    printf("Error open test file, please make sure they exist.\n");

    return 0;
  }

  while (fgets(buffer, 15, pFile) && strlen(buffer) > 1)
  {
    //This unsigned char array stores an instruction read from the file
    //As the largest y86 instruction is 6 bytes, there are 6 unsigned char in the array where
    //each represents a byte.
    unsigned char instruction[6] = {0, 0, 0, 0, 0, 0};
    convertStrToByteCode(buffer, instruction, 6);

    //TODO: From here, your task is to complete the implementation so that all y86 opcodes can be disassembled.
    //Any undisassembled opcode should display as "TODO: undisassembled opcode"
    printf("TODO: undisassembled opcode. The first byte of the instruction is 0x%X\n", instruction[0]);
    dissassembleOpcode(instruction[0]);
  }

  fclose(pFile);

  return 0;
}

void dissassembleOpcode(unsigned char opcode)
{
  switch(opcode)
  {
    case '00':
      printf("halt");
      break;
    case '10':
      printf("nop");
      break;
    case '20':
      printf("rrmovl");
      break;
    case '21':
      printf("cmovle");
      break;
    case '22':
      printf("cmovl");
      break;
    case '23':
      printf("cmove");
      break;
    case '24':
      printf("cmovne");
      break;
    case '25':
      printf("cmovge");
      break;
    case '26':
      printf("cmovg");
      break;
    case '60':
      printf("addl");
      break; 
    case '61':
      printf("subl");
      break;
    case '62':
      printf("andl");
      break;
    case '63':
      printf("xorl");
      break;
    case '90':
      printf("ret");
      break;
    case 'A0':
      printf("pushl");
      break;
    case 'B0':
      printf("popl");
      break;
    case 'C0':
      printf("call");
      break;
    case 'D0':
      printf("jmp");
      break;
    default:
      printf("unknwon")
      break;


  }
}

/****************************************************************************
N.B. You do not need to modify or work in this function.
Description:
This function converts a line of machine code read from the text file
into machine byte code.
The machine code is stored in an unsigned char array.
******************************************************************************/
int convertStrToByteCode(const char *str, unsigned char inst[], int size)
{
  int numHexDigits = 0;
  char *endstr;
  //Each instruction should consist of at most 12 hex digits
  numHexDigits = strlen(str) - 1;
  //Convert the string to integer, N.B. this integer is in decimal
  long long value = strtol(str, &endstr, 16);

  int numBytes = numHexDigits >> 1;
  int byteCount = numHexDigits >> 1;

  while (byteCount > 0)
  {
    unsigned long long mask = 0xFF;
    unsigned long shift = (numBytes - byteCount) << 3;

    inst[byteCount - 1] = (value & (mask << shift)) >> shift;
    byteCount--;
  }

  //Return the size of the instruction in bytes
  return numHexDigits >> 1;
}

