#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int convertStrToByteCode(const char *str, unsigned char inst[], int size);
void disassemble(unsigned char opcode);

int main(int argc, char **argv)
{
  FILE *pFile = NULL;

  char buffer[15];

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
    
    //disassemble(instruction[0]);
  }

  fclose(pFile);

  return 0;
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

void disassemble(unsigned char opcode)
{
  //Opcode is converted to decimal first to make it easier to work with
  switch(opcode)
  {
    //All single byte instructions are handled here
    case 0:
      printf("halt\n");
      break;
    case 16:
      printf("nop\n");
      break;
    case 144:
      printf("ret\n");
      break;


    //All two byte instructions are handled here
    case 32:
      printf("rrmovl\n");
      break;
    case 33:
      printf("cmovle\n");
      break;
    case 34:
      printf("movl\n");
      break;
    case 35:
      printf("cmove\n");
      break;
    case 36:
      printf("cmovne\n");
      break;
    case 37:
      printf("cmovge\n");
      break;
    case 38:
      printf("cmovg\n");
      break;

    case 96:
      printf("addl\n");
      break;
    case 97:
      printf("subl\n");
      break;
    case 98:
      printf("andl\n");
      break;
    case 99:
      printf("xorl\n");
      break;

    case 160:
      printf("pushl\n");
      break;
    case 176:
      printf("popl\n");
      break;

    default:
      printf("TODO: undisassembled opcode. The first byte of the instruction is 0x%X\n", opcode);
      break;
  }

}