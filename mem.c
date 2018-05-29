#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Memory of 64 word (256 bytes) as global variable
unsigned char MEM[64][32];

//Matrix as 32 registers of 32 Bit bank
unsigned char REG[32][32];

//32 Bit Instruction Register
unsigned char InstructionRegister[32];

//32 Bit Memory Data Register
unsigned char MemoryDataRegister[32];

//32 Bit A Register
unsigned char A[32];

//32 Bit B Register
unsigned char B[32];

int convertCharToInt(unsigned char *binaryArray, int lenght){
    int result = 0;
    for (int i = (lenght-1); i >= 0; --i){
        if(binaryArray[i] == '1')
            result += pow(2, (lenght-1)-i);
    }
    //printf("%d\n", result);
    return result;
}

void memory(unsigned char adress[32], unsigned char data[32], unsigned char read, unsigned char write)
{
	//For a read operation
	if(read == '1')	
	{
		//Copy memory data to return_array
		int adr = convertCharToInt(adress, 32);
		for(int i = 0; i < 32; i++)
		{
			InstructionRegister[i] = MEM[adr][i];
			MemoryDataRegister[i] =  MEM[adr][i];
		}
	}
	//For a write operation
	else if(write == '1')
	{
		//Copy memory data to return_array
		int adr = convertCharToInt(adress, 32);
		for(int i = 0; i < 32; i++)
		{
			MEM[adr][i] = data[i];
		}
	}
}

void registers(unsigned char ReadRegister1[5], unsigned char ReadRegister2[5], unsigned char WriteRegister[5], unsigned char WriteData[32], unsigned char RegWrite)	
{
	//To write WriteData content at register of number WriteRegister
	if(RegWrite == '1')
	{
		int reg_num = convertCharToInt(WriteRegister, 5);
		for (int i = 0; i < 32; ++i)
		{
			REG[reg_num][i] = WriteData[i];
		}
	}
	//To read both ReadRegister 1 and ReadRegister 2 number registers and copy the contents to registers A and B respectively  
	else
	{
		int reg_num1 = convertCharToInt(ReadRegister1, 5);
		int reg_num2 = convertCharToInt(ReadRegister2, 5);
		for (int i = 0; i < 32; ++i)
		{
			A[i] = REG[reg_num1][i];
			B[i] = REG[reg_num2][i];	
		}
	}
}

int main()
{
	unsigned char $num[32] = {0};
	$num[30] = '1';

	printf("%d\n", convertCharToInt($num, 32));
	

	return 0;
}