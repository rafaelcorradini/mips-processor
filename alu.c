#include <stdio.h>
#include <stdlib.h>
#include <math.h>


unsigned char *mux(
    unsigned char *control,
    unsigned char *input0,
    unsigned char *input1,
    unsigned char *input2,
    unsigned char *input3
) {
    if(control[0] == '0') {
        if(control[1] == '0')
            return input0;
        else
            return input2;
    } else {
        if(control[1] == '0')
            return input1;
        else
            return input3;
    }
}

unsigned char *shiftLeft(unsigned char *input) {
    int i;
    unsigned char *result = malloc(sizeof(unsigned char) * 32);
    
    for (i = 0; i < 30; i++) {
        result[i+2] = input[i];
    }
    result[0] = '0';
    result[1] = '0';

    return result;
}

int checkFunctionField(unsigned char *instructionFunction){
    if(instructionFunction[0] == '1' && instructionFunction[1] == '0' && instructionFunction[2] == '0' 
        && instructionFunction[3] == '0' && instructionFunction[4] == '0' && instructionFunction[5] == '0'){

        return 0;
    } else if (instructionFunction[0] == '1' && instructionFunction[1] == '0' && instructionFunction[2] == '0' 
        && instructionFunction[3] == '0' && instructionFunction[4] == '1' && instructionFunction[5] == '0'){
       
        return 1;
    } else if (instructionFunction[0] == '1' && instructionFunction[1] == '0' && instructionFunction[2] == '0' 
        && instructionFunction[3] == '1' && instructionFunction[4] == '0' && instructionFunction[5] == '0'){
        
        return 2;
    } else if (instructionFunction[0] == '1' && instructionFunction[1] == '0' && instructionFunction[2] == '0' 
        && instructionFunction[3] == '1' && instructionFunction[4] == '0' && instructionFunction[5] == '1'){

        return 3;
    } else if (instructionFunction[0] == '1' && instructionFunction[1] == '0' && instructionFunction[2] == '1' 
        && instructionFunction[3] == '0' && instructionFunction[4] == '1' && instructionFunction[5] == '0'){

        return 4;
    }
        
}

int checkALUoperation(unsigned char *aluControlInput){
    if(aluControlInput[0] == '0' && aluControlInput[1] == '0' && aluControlInput[2] == '1' 
        && aluControlInput[3] == '0'){

        return 0;
    } else if (aluControlInput[0] == '0' && aluControlInput[1] == '1' && aluControlInput[2] == '1' 
        && aluControlInput[3] == '0'){
       
        return 1;
    } else if (aluControlInput[0] == '0' && aluControlInput[1] == '0' && aluControlInput[2] == '0' 
        && aluControlInput[3] == '0'){
        
        return 2;
    } else if (aluControlInput[0] == '0' && aluControlInput[1] == '0' && aluControlInput[2] == '0' 
        && aluControlInput[3] == '1'){

        return 3;
    } else if (aluControlInput[0] == '0' && aluControlInput[1] == '1' && aluControlInput[2] == '1' 
        && aluControlInput[3] == '1'){

        return 4;
    }
        
}

unsigned char *aluControl(unsigned char *ALUop, unsigned char *instructionFunction){
    unsigned char *result = malloc(sizeof(unsigned char) * 4);

    //sum Store/Load word
    if(ALUop[0] == '0' && ALUop[1] == '0'){
        result[0] = '0';
        result[1] = '0';
        result[2] = '1';
        result[3] = '0';
        return result;
    }
    // branch subtraction
    else if (ALUop[0] == '0' && ALUop[1] == '1'){
        result[0] = '0';
        result[1] = '1';
        result[2] = '1';
        result[3] = '0';
        return result;
    }
    else if (ALUop[0] == '1' && ALUop[1] == '1'){
        result[0] = '0';
        result[1] = '0';
        result[2] = '0';
        result[3] = '0';
        return result;
    }
    // R-type
    else if (ALUop[0] == '1' && ALUop[1] == '0'){
        //Sum
        if (checkFunctionField(instructionFunction) == 0){
            result[0] = '0';
            result[1] = '0';
            result[2] = '1';
            result[3] = '0';
            return result;
        }
        //Subtraction
        else if (checkFunctionField(instructionFunction) == 1){
            result[0] = '0';
            result[1] = '1';
            result[2] = '1';
            result[3] = '0';
            return result;
        }
        //And
        else if (checkFunctionField(instructionFunction) == 2){
            result[0] = '0';
            result[1] = '0';
            result[2] = '0';
            result[3] = '0';
            return result;
        }
        // or
        else if (checkFunctionField(instructionFunction) == 3){
            result[0] = '0';
            result[1] = '0';
            result[2] = '0';
            result[3] = '1';
            return result;
        }
        //Set on less than
        else if (checkFunctionField(instructionFunction) == 4){
            result[0] = '0';
            result[1] = '1';
            result[2] = '1';
            result[3] = '1';
            return result;
        }
    }

}

int convertCharToInt(unsigned char *binaryArray, int lenght){
    int result = 0;
    for (int i = (lenght-1); i >= 0; --i){
        if(binaryArray[i] == '1')
            result += pow(2, (lenght-1)-i);
    }
    printf("%d\n", result);
    return result;
}

unsigned char *decToBinary(int n){
    unsigned char *result = malloc(sizeof(unsigned char) * 32);
    // Size of an integer is assumed to be 32 bits
    for (int i = 31; i >= 0; i--) {
        result[31 - i] = n & 1 << i ? '1' : '0';
    }
    return result;
}

void printInput(unsigned char *input, int size) {
    int i;

    for (i = 0; i < size; i++) {
        printf("%c", input[i]);
    }
    printf("\n");
}

unsigned char *ALU(unsigned char *aluControlInput, unsigned char *dataOne, unsigned char *dataTwo){
    int operandOne = convertCharToInt(dataOne, 32);
    int operandTwo = convertCharToInt(dataTwo, 32);
    unsigned char *result;
    
    if (checkALUoperation(aluControlInput) == 0){
        result = decToBinary(operandOne + operandTwo);
        return result;
    }
    else if (checkALUoperation(aluControlInput) == 1){
        result = decToBinary(operandOne - operandTwo);
        return result;
    }
    else if (checkALUoperation(aluControlInput) == 2){
        result = decToBinary(operandOne & operandTwo);
        return result;
    }
    else if (checkALUoperation(aluControlInput) == 3){
        result = decToBinary(operandOne | operandTwo);
        return result;
    }
    else if (checkALUoperation(aluControlInput) == 4){
        if(operandOne < operandTwo){
            result = decToBinary(1);
        }
        else{
            result = decToBinary(0);
        }
        return result;
    }
}



// testing
int main() {
    unsigned char control[32], input0[32], input1[32], ALUop[2], instructionFunction[4];
    unsigned char *result;
    int i;
    ALUop[0] = '1';
    ALUop[1] = '0';

    instructionFunction[0] = '1'; 
    instructionFunction[1] = '0'; 
    instructionFunction[2] = '0';
    instructionFunction[3] = '0';  
    instructionFunction[4] = '1'; 
    instructionFunction[5] = '0'; 
   
    printInput(decToBinary(537395210), 32);
    printInput(ALU(aluControl(ALUop, instructionFunction), decToBinary(5), decToBinary(4)), 32);
    //printInput(aluControl(ALUop, instructionFunction), 4);

    for(i = 0; i < 32; i++) {
        control[i] = '0';
        input0[i] = '0';
        input1[i] = '0';
    }
    
    // mux
    input0[3] = '1';
    input1[5] = '1';
    printf("input0 = ");
    printInput(mux(control, input0, input1, NULL, NULL), 32);

    control[0] = '1';
    printf("input1 = ");
    printInput(mux(control, input0, input1, NULL, NULL), 32);
    
    // shiftleft
    printf("input0  + shift = ");
    result = shiftLeft(input0);
    printInput(result, 32);

    return 0;
}
