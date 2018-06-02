#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

//Program Counter
unsigned char PC[32];

//32 Bit B Register
unsigned char B[32];

//32 Bit ALU out Register
unsigned char AluOut[32];

//Control unit signals as global variable
unsigned char PCWrite;
unsigned char PCWriteCond;
unsigned char IorD;
unsigned char MemRead;
unsigned char MemWrite;
unsigned char Irwrite;
unsigned char ALUSrcA;
unsigned char RegWrite;
unsigned char BNE;
unsigned char RegDst[2];
unsigned char MemtoReg[2];
unsigned char PCSource[2];
unsigned char ALUOp[2];
unsigned char ALUSrcB[2];

//All aux variables
unsigned char PcIn = 0;

//32 Bit aux Instruction Register
unsigned char AuxInstructionRegister[32];

//32 Bit aux Memory Data Register
unsigned char AuxMemoryDataRegister[32];

//32 Bit aux A Register
unsigned char AuxA[32];

//32 Bit aux B Register
unsigned char AuxB[32];

//aux Program Counter
unsigned char AuxPC[32];

//Matrix as 32 aux registers of 32 Bit bank
unsigned char AuxREG[32][32];

//aux ALUOUT
unsigned char AuxAluOut[32];

//ZERO AND ONE
unsigned char Z[1] = {0};
unsigned char O[1] = {1};


//Control unit state
int old=0;
int next=0;

int convertCharToInt(unsigned char *binaryArray, int lenght){
    int result = 0;

    for (int i = (lenght-1); i >= 0; --i){
        if(binaryArray[i] == 1)
            result += pow(2, (lenght-1)-i);
    }
    return result;
}

void unit_control(unsigned char opcode[6]){

   int op = convertCharToInt(opcode, 6);
    int b[4];

    	//b3 = less significative bit
        b[3]=(!(old)|!((old^0x2)|(op^0x23))|
        !((old^0x2)|(op^0x2b))|
        !(old^0x6)|!((old^0x1)|(op^0x2))|
        !((old^0x1)|(op^0x8))|
        !(((old^0x1)|(op^0x14))&((old^0x15)|(op^0x15)))|
        !((old^0x1)|(op^0x15)));

        b[2]=!((old^0x1)|((op^0x23)&(op^0x2b))) |
        !((old^0x2)|(op^0x23)) |
        !((old^0x1)|!!(op)) |
        !(old^0x6) |
        !((old^0x1)|(op^0x5)) |
        !((old^0x1)|(op^0x8)) |
        !((old^0x1)|(op^0x3)) |
        !((old^0x1)|(op^0x15));

        b[1]= !(old^0x3) |
        !((old^0x2)|(op^0x2b)) |
        !((old^0x1)|!!(op)) |
        !(old^0x6) |
        !((old^0x1)|(op^0xc)) |
        !(((old^0x1)|(op^0x14))&((old^0x15)|(op^0x15))) |
        !((old^0x1)|(op^0x3)) |
        !((old^0x1)|(op^0x15));

        b[0]= !((old^0x1)|(op^0x4)) |
        !((old^0x1)|(op^0x2)) |
        !((old^0x1)|(op^0x5)) |
        !((old^0x1)|(op^0x8)) |
        !((old^0x1)|(op^0xc)) |
        !(((old^0x1)|(op^0x14))&((old^0x15)|(op^0x15))) |
        !((old^0x1)|(op^0x3)) |
        !((old^0x1)|(op^0x15));



        old=8*b[0]+4*b[1]+2*b[2]+1*b[3];



		PCWrite=!b[0]&!b[1]&!b[2]&!b[3]|b[0]&!b[1]&!b[2]&b[3];
        PCWriteCond=b[0]&!b[1]&!b[2]&!b[3]|b[0]&!b[1]&b[2]&!b[3];
        IorD=!b[0]&!b[1]&b[2]&b[3]|!b[0]&b[1]&!b[2]&b[3];
        MemRead=!b[0]&!b[1]&!b[2]&!b[3]|!b[0]&!b[1]&b[2]&b[3];
        MemWrite=!b[0]&b[1]&!b[2]&b[3];
        Irwrite=!b[0]&!b[1]&!b[2]&!b[3];
        MemtoReg[0]=!b[0]&b[1]&!b[2]&!b[3];
        PCSource[1]=b[0]&!b[1]&!b[2]&b[3];
        PCSource[0]=b[0]&!b[1]&!b[2]&!b[3]|b[0]&!b[1]&b[2]&!b[3]|b[0]&b[1]&!b[2]&b[3];
        ALUOp[1]=!b[0]&b[1]&b[2]&!b[3];
        ALUOp[0]=b[0]&!b[1]&!b[2]&!b[3]|b[0]&!b[1]&b[2]&!b[3];
        ALUSrcB[1]=!b[0]&!b[1]&b[2]&!b[3];
        ALUSrcB[0]=!b[0]&!b[1]&!b[2]&!b[3]|!b[0]&!b[1]&!b[2]&b[3]|!b[0]&!b[1]&!b[2]&!b[3];
        ALUSrcA=!b[0]&!b[1]&b[2]&!b[3]|!b[0]&b[1]&b[2]&!b[3]|b[0]&!b[1]&!b[2]&!b[3]|b[0]&!b[1]&b[2]&!b[3];
        RegWrite=!b[0]&b[1]&!b[2]&!b[3]|!b[0]&b[1]&b[2]&b[3]|!b[0]&!b[1]&!b[2]&!b[3]|!b[0]&!b[1]&!b[2]&!b[3];
        RegDst[0]=!b[0]&b[1]&b[2]&b[3];
        RegDst[1]=b[0]&b[1]&b[2]&!b[3];
        BNE=b[0]&!b[1]&b[2]&!b[3];
        MemtoReg[1]=0;

return;
}

void PC_func(unsigned char mxPC [32], unsigned char PcIn){
	int i;
	if(PcIn == 1){
		for(i = 0; i < 32; i++){
			AuxPC[i] = mxPC[i];
		}
	}
}



void memory(unsigned char adress[32], unsigned char data[32], unsigned char read, unsigned char write)
{
	//For a read operation
	if(read == 1)	
	{
		//Copy memory data to return_array
		int adr = convertCharToInt(adress, 32);
		for(int i = 0; i < 32; i++)
		{
			if(Irwrite == 1)
			{
				AuxInstructionRegister[i] = MEM[adr][i];
			}
			AuxMemoryDataRegister[i] =  MEM[adr][i];
		}
	}
	//For a write operation
	else if(write == 1)
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
	if(RegWrite == 1)
	{   
		int reg_num = convertCharToInt(WriteRegister, 5);
		for (int i = 0; i < 32; ++i)
		{
			AuxREG[reg_num][i] = WriteData[i];
		}
	}
	//To read both ReadRegister 1 and ReadRegister 2 number registers and copy the contents to registers A and B respectively
      
	int reg_num1 = convertCharToInt(ReadRegister1, 5);
	int reg_num2 = convertCharToInt(ReadRegister2, 5);
	for (int i = 0; i < 32; ++i)
	{
		A[i] = REG[reg_num1][i];
		B[i] = REG[reg_num2][i];	
	}
}

unsigned char *mux2(
    unsigned char *input0,
    unsigned char *input1,
    unsigned char *input2,
    unsigned char *input3,
    unsigned char *control
) {
    if(control[0] == 0) {
        if(control[1] == 0)
            return input0;
        else
            return input1;
    } else {
        if(control[1] == 0)
            return input2;
        else
            return input3;
    }
}

unsigned char *mux(
    unsigned char *input0,
    unsigned char *input1,
    unsigned char control
) {
    if(control == 0) {
        return input0;
        
    } 
    else {
        return input1;
    }
}

unsigned char *shiftLeft(unsigned char *input) {
    int i;
    unsigned char *result = malloc(sizeof(unsigned char) * 32);
    
    for (i = 31; i >= 0; i--) {
        result[i-2] = input[i];
    }
    result[31] = 0;
    result[30] = 0;

    return result;
}

unsigned char *signalExtend(unsigned char *input) {
    unsigned char *result = malloc(sizeof(unsigned char) * 32);
    
    for (int i = 0, j = 16; i < 16; i++, j++) {
        result[j] = input[i];
    }
    for (int j = 0; j < 16; j++) {
        result[j] = input[0];
    } 

    return result;
}

int checkFunctionField(unsigned char *instructionFunction){
    if(instructionFunction[0] == 1 && instructionFunction[1] == 0 && instructionFunction[2] == 0 
        && instructionFunction[3] == 0 && instructionFunction[4] == 0 && instructionFunction[5] == 0){

        return 0;
    } else if (instructionFunction[0] == 1 && instructionFunction[1] == 0 && instructionFunction[2] == 0 
        && instructionFunction[3] == 0 && instructionFunction[4] == 1 && instructionFunction[5] == 0){
       
        return 1;
    } else if (instructionFunction[0] == 1 && instructionFunction[1] == 0 && instructionFunction[2] == 0 
        && instructionFunction[3] == 1 && instructionFunction[4] == 0 && instructionFunction[5] == 0){
        
        return 2;
    } else if (instructionFunction[0] == 1 && instructionFunction[1] == 0 && instructionFunction[2] == 0 
        && instructionFunction[3] == 1 && instructionFunction[4] == 0 && instructionFunction[5] == 1){

        return 3;
    } else if (instructionFunction[0] == 1 && instructionFunction[1] == 0 && instructionFunction[2] == 1 
        && instructionFunction[3] == 0 && instructionFunction[4] == 1 && instructionFunction[5] == 0){

        return 4;
    }
        
}

int checkALUoperation(unsigned char *aluControlInput){
    if(aluControlInput[0] == 0 && aluControlInput[1] == 0 && aluControlInput[2] == 1 
        && aluControlInput[3] == 0){

        return 0;
    } else if (aluControlInput[0] == 0 && aluControlInput[1] == 1 && aluControlInput[2] == 1 
        && aluControlInput[3] == 0){
       
        return 1;
    } else if (aluControlInput[0] == 0 && aluControlInput[1] == 0 && aluControlInput[2] == 0 
        && aluControlInput[3] == 0){
        
        return 2;
    } else if (aluControlInput[0] == 0 && aluControlInput[1] == 0 && aluControlInput[2] == 0
        && aluControlInput[3] == 1){

        return 3;
    } else if (aluControlInput[0] == 0 && aluControlInput[1] == 1 && aluControlInput[2] == 1 
        && aluControlInput[3] == 1){

        return 4;
    }
        
}

unsigned char *aluControl(unsigned char *ALUop, unsigned char *instructionFunction){
    unsigned char *result = malloc(sizeof(unsigned char) * 4);

    //sum Store/Load word
    if(ALUop[0] == 0 && ALUop[1] == 0){
        result[0] = 0;
        result[1] = 0;
        result[2] = 1;
        result[3] = 0;
        return result;
    }
    // branch subtraction
    else if (ALUop[0] == 0 && ALUop[1] == 1){
        result[0] = 0;
        result[1] = 1;
        result[2] = 1;
        result[3] = 0;
        return result;
    }
    else if (ALUop[0] == 1 && ALUop[1] == 1){
        result[0] = 0;
        result[1] = 0;
        result[2] = 0;
        result[3] = 0;
        return result;
    }
    // R-type
    else if (ALUop[0] == 1 && ALUop[1] == 0){
        //Sum
        if (checkFunctionField(instructionFunction) == 0){
            result[0] = 0;
            result[1] = 0;
            result[2] = 1;
            result[3] = 0;
            return result;
        }
        //Subtraction
        else if (checkFunctionField(instructionFunction) == 1){
            result[0] = 0;
            result[1] = 1;
            result[2] = 1;
            result[3] = 0;
            return result;
        }
        //And
        else if (checkFunctionField(instructionFunction) == 2){
            result[0] = 0;
            result[1] = 0;
            result[2] = 0;
            result[3] = 0;
            return result;
        }
        // or
        else if (checkFunctionField(instructionFunction) == 3){
            result[0] = 0;
            result[1] = 0;
            result[2] = 0;
            result[3] = 1;
            return result;
        }
        //Set on less than
        else if (checkFunctionField(instructionFunction) == 4){
            result[0] = 0;
            result[1] = 1;
            result[2] = 1;
            result[3] = 1;
            return result;
        }
    }

}

unsigned char *decToBinary(int n){
    unsigned char *result = malloc(sizeof(unsigned char) * 32);
    // Size of an integer is assumed to be 32 bits
    for (int i = 31; i >= 0; i--) {
        result[31 - i] = n & 1 << i ? 1 : 0;
    }
    return result;
}

void printInput(unsigned char *input, int size) {
    int i;

    for (i = 0; i < size; i++) {
        printf("%d", input[i]);
    }
    printf("\n");
}

void ALU(unsigned char *aluControlInput, unsigned char *dataOne, unsigned char *dataTwo){
    int operandOne = convertCharToInt(dataOne, 32);
    int operandTwo = convertCharToInt(dataTwo, 32);
    unsigned char *result;

    // printf("\n");
    // printInput(dataOne, 32);
    // printInput(dataTwo, 32);
    // printf("\n");
    
    if (checkALUoperation(aluControlInput) == 0){
        result = decToBinary(operandOne + operandTwo);
    }
    else if (checkALUoperation(aluControlInput) == 1){
        result = decToBinary(operandOne - operandTwo);
    }
    else if (checkALUoperation(aluControlInput) == 2){
        result = decToBinary(operandOne & operandTwo);
    }
    else if (checkALUoperation(aluControlInput) == 3){
        result = decToBinary(operandOne | operandTwo);
    }
    else if (checkALUoperation(aluControlInput) == 4){
        if(operandOne < operandTwo){
            result = decToBinary(1);
        }
        else{
            result = decToBinary(0);
        }
    }
    for (int i = 0; i < 32; ++i)
    {
    	AuxAluOut[i] = result[i];
    }
    free(result);
}

void clockUpper()
{
	for (int i = 0; i < 32; ++i)
	{
		//32 Bit Instruction Register
		InstructionRegister[i] = AuxInstructionRegister[i];

		//32 Bit Memory Data Register
		MemoryDataRegister[i] = AuxMemoryDataRegister[i];

		//32 Bit A Register
		A[i] = AuxA[i];

		//32 Bit B Register
		B[i] = AuxB[i];

		//Program Counter
		PC[i] = AuxPC[i];

		//32 Bit Instruction Register
	    InstructionRegister[i] = AuxInstructionRegister[i];

		//32 Bit Memory Data Register
	    MemoryDataRegister[i] = AuxMemoryDataRegister[i];

	    //32 Bit AluOut Register
	    AluOut[i] = AuxAluOut[i];
	
		for (int j = 0; j < 32; ++j)
		{
			//Matrix as 32 registers of 32 Bit bank
			REG[i][j] = AuxREG[i][j];
		}
	}	
}

unsigned char *split(int init, int end, unsigned char *vetor)
{
	unsigned char *array;
	array = malloc(sizeof(unsigned char) * (end - init + 1));
	for (int i = init, j = 0; i <= end; ++i, ++j)
	{
		array[j] = vetor[i];
	}
	return array;
}


unsigned char Pc_In(unsigned char write, unsigned char cond, unsigned char *b_n_e)
{
	return (b_n_e&&cond)||write;
}

int main()
{

	unsigned char **op;

	op = (unsigned char **)malloc(sizeof(unsigned char*) * 64);
	for (int i = 0; i < 32; ++i)
	{
		op[i] = (unsigned char *)malloc(sizeof(unsigned char) * 32);
	}

	for (int i = 0; i < 32; ++i)
	{
		op[0][i] = decToBinary(201326610)[i];
		op[1][i] = decToBinary(537395210)[i];
		op[2][i] = decToBinary(537460737)[i];
		op[3][i] = decToBinary(537526274)[i];
		op[4][i] = decToBinary(277348355)[i];
		op[5][i] = decToBinary(8986656)[i];
		op[6][i] = decToBinary(13185058)[i];
		op[7][i] = decToBinary(134217732)[i];
		op[8][i] = decToBinary(8929322)[i];
		op[9][i] = decToBinary(549912577)[i];
		op[10][i] = decToBinary(14702629)[i];
	}

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 32; ++j)
		{
			MEM[i][j] = op[i][j];
		}
	}
	for (int i = 0; i < 32; ++i)
	{
		InstructionRegister[i] = MEM[0][i];
		AuxInstructionRegister[i] = MEM[0][i];
	}
	

	for (int o = 0; o < 17 ; ++o)
	{
		unsigned char *splited = split(0,5,InstructionRegister);


		printf("IR: ");
		for (int i = 0; i < 32; ++i)
		{
			printf("%hhu", InstructionRegister[i]);
		}
		printf("\n");

		printf("OPCODE: ");
		for (int i = 0; i < 6; ++i)
		{
			printf("%hhu", splited[i]);
		}
		printf("\n");

		printf("PC: ");
		for (int i = 0; i < 32; ++i)
		{
			printf("%hhu", PC[i]);
		}
		printf("\n");

		printf("PC WRITE: ");
			printf("%hhu\n", PCWrite);

		printf("PC SOURCE: ");
			printf("%hhu%hhu\n", PCSource[0], PCSource[1]);	

		printf("PC MUST WRITE: ");
		for (int i = 0; i < 32; ++i)
		{
			{
				printf("%hhu",mux2(AuxAluOut, AluOut, shiftLeft(split(6,31,InstructionRegister)), A, PCSource)[i]);
			}
		}
		
		printf("\nALU SOURCE A: ");
			printf("%hhu\n", ALUSrcA);
		printf("ALU SOURCE B: ");
			printf("%hhu%hhu\n", ALUSrcB[0], ALUSrcB[1]);	

		
		printf("\n\n");
		//printf("%d\n", o);
		
		
		//Unit control 
		unit_control(split(0,5,InstructionRegister));
		//Memory block funcion
		memory(mux(PC, AluOut, IorD), B, MemRead, MemWrite);
		//Registers Bank Function
		registers(split(6,10,InstructionRegister), split(11,15,InstructionRegister), mux2(split(11,15,InstructionRegister),
				  split(16,20, InstructionRegister), decToBinary(31), NULL, RegDst), mux2(AluOut, MemoryDataRegister, PC, NULL, MemtoReg), RegWrite);
		//Alu function
		ALU(aluControl(ALUOp, split(26,31, InstructionRegister)), mux(PC, A, ALUSrcA), mux2(B, signalExtend(split(16, 31, InstructionRegister)), decToBinary(1), shiftLeft(signalExtend(split(16, 31, InstructionRegister))), ALUSrcB));
		//PC function
		PC_func(mux2(AuxAluOut, AluOut, shiftLeft(split(6,31,InstructionRegister)), A, PCSource), Pc_In(PCWrite, PCWriteCond, mux(Z,O,BNE)));
		//Pass aux values to registers at upper clock border
		clockUpper();
	}

	printf("REGISTRADORES\n");
	for (int i = 0; i < 32; ++i)
	{
		for (int j = 0; j < 32; ++j)
		{
			printf("%hhu",REG[i][j]);
		}
		printf("\n");
	}

	printf("MEMORIA:\n");
	for (int i = 0; i < 64; ++i)
	{
		for (int j = 0; j < 32; ++j)
		{
			printf("%hhu", MEM[i][j]);
		}
		printf("\n");
	}
	
	return 0;
}

