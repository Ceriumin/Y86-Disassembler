#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

const char *register_names[] = {"%eax", "%ecx", "%edx",
                                "%ebx", "%esp", "%ebp",
                                "%esi", "%edi", "UNKNOWN REGISTER"};

int convertStrToByteCode(const char *str, unsigned char inst[], int size);
void disassemble(unsigned char opcode[], int size);

int main(int argc, char **argv)
{
  FILE *pFile = NULL;

  char buffer[15];
  int bitCount = strlen(buffer) - 1;

  if 
    (argc < 2) pFile = fopen("./test.txt", "r");
  else 
    pFile = fopen(argv[1], "r");

  if (pFile == NULL)
  {
    printf("Error open test file, please make sure they exist.\n");
    return 0;
  }

  while (fgets(buffer, 15, pFile) && strlen(buffer) > 1)
  {
    static unsigned char instruction[6] = {0, 0, 0, 0, 0, 0};
    convertStrToByteCode(buffer, instruction, 6);

    /**********************************************************
    The bytevalue is used to reperesent the amount of bytes
    that are in the instruction set so all can be referenced
    in the disassemble function.
    **********************************************************/

    int byteValue = sizeof(instruction) / sizeof(instruction[0]);
    disassemble(instruction, byteValue);

  }

  fclose(pFile);

  return 0;
}

int convertStrToByteCode(const char *str, unsigned char inst[], int size)
{
  int numHexDigits = 0;
  char *endstr;
  numHexDigits = strlen(str) - 1;
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

void disassemble(unsigned char opcode[], int size)
{
  //opcode is the first byte of the instruction set
  unsigned char opcode_ = opcode[0];

  /*****************************************************************
  regA and regB represent the registers that are being used in the
  instruction set. The first 4 bits of the second byte represent
  regA and the last 4 bits represent regB. This is done by bit shifting
  *****************************************************************/

  unsigned char regA = opcode[1] >> 4;
  unsigned char regB = opcode[1] & 0x0F;

  //value and destination are the the 32bit integer values that are used in the instruction set
  unsigned int value = opcode[2] << 24 | opcode[3] << 16 | opcode[4] << 8 | opcode[5];
  value = htonl(value);

  unsigned int dest = opcode[1] << 24 | opcode[2] << 16 | opcode[3] << 8 | opcode[4];
  dest = htonl(dest);

  /*****************************************************************
  Opcode is determined from a decimal rather than a hexidecimal so it
  is a lot easier to work with and allows for better calculations.
  *****************************************************************/

  //One byte
  switch(opcode_)
  {
    case 00:
      printf("halt\n");
      break;
    case 16:
      printf("nop\n");
      break;
    case 144:
      printf("ret\n");
      break;

    //Two bytes
    case 32:
      printf("rrmovl");
      printf(" %s", register_names[regA]);
      printf(" %s\n", register_names[regB]);
      break;
    case 33:
      printf("cmovle");
      printf(" %s", register_names[regA]);
      printf(" %s\n", register_names[regB]);   
      break;
    case 34:
      printf("movl");
      printf(" %s", register_names[regA]);
      printf(" %s\n", register_names[regB]);  
      break;
    case 35:
      printf("cmove");
      printf(" %s", register_names[regA]);
      printf(" %s\n", register_names[regB]);  
      break;
    case 36:
      printf("cmovne");
      printf(" %s", register_names[regA]);
      printf(" %s\n", register_names[regB]);  
      break;
    case 37:
      printf("cmovge");
      printf(" %s", register_names[regA]);
      printf(" %s\n", register_names[regB]);  
      break;
    case 38:
      printf("cmovg");
      printf(" %s", register_names[regA]);
      printf(" %s\n", register_names[regB]);  
      break;

    case 96:
      printf("addl");
      printf(" %s", register_names[regA]);
      printf(" %s\n", register_names[regB]);  
      break;
    case 97:
      printf("subl");
      printf(" %s", register_names[regA]);
      printf(" %s\n", register_names[regB]);  
      break; 
    case 98:
      printf("andl");
      printf(" %s", register_names[regA]);
      printf(" %s\n", register_names[regB]);  
      break;
    case 99:
      printf("xorl");
      printf(" %s", register_names[regA]);
      printf(" %s\n", register_names[regB]);  
      break;

    case 160:
      printf("pushl");
      printf(" %s\n", register_names[regA]);
      break;
    case 176:
      printf("popl");
      printf(" %s\n", register_names[regA]);
      break;

    //Four bytes
    case 48:
      printf("irmovl"); 
      printf(" $%d", value);
      printf(" %s\n", register_names[regB]);
      break;

    case 64:
      printf("rmmovl");
      printf(" %s", register_names[regA]);
      printf(" %d(%s)\n", value, register_names[regB]);
      break;
    case 80:
      printf("mrmovl");
      printf(" %d(%s)", value, register_names[regB]);
      printf(" %s\n", register_names[regA]);
      break;

    case 112:
      printf("jmp");
      printf(" %d\n", dest);
      break;
    case 113:
      printf("jle");
      printf(" %d\n", dest);
      break;
    case 114:
      printf("jl");
      printf(" %d\n", dest);
      break;
    case 115:
      printf("je");
      printf(" %d\n", dest);
      break;
    case 116:
      printf("jne");
      printf(" %d\n", dest);
      break;
    case 117:
      printf("jge");
      printf(" %d\n", dest);
      break;
    case 118:
      printf("jg");
      printf(" %d\n", dest);
      break;
    case 128:
      printf("call");
      printf(" %d\n", dest);
      break;

    //if for whatever reason the opcode cannot be found, print the following
    default:
      printf("undisassembled opcode. The first byte of the instruction is 0x%X\n", opcode_);
      break;
  }

}