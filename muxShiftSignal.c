#include <stdio.h>
#include <stdlib.h>

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

void printInput(unsigned char *input) {
    int i;

    for (i = 0; i < 32; i++) {
        printf("%c", input[i]);
    }
    printf("\n");
}

// testing
int main() {
    unsigned char control[32], input0[32], input1[32];
    unsigned char *result;
    int i;

    for(i = 0; i < 32; i++) {
        control[i] = '0';
        input0[i] = '0';
        input1[i] = '0';
    }
    
    // mux
    input0[3] = '1';
    input1[5] = '1';
    printf("input0 = ");
    printInput(mux(control, input0, input1, NULL, NULL));

    control[0] = '1';
    printf("input1 = ");
    printInput(mux(control, input0, input1, NULL, NULL));
    
    // shiftleft
    printf("input0  + shift = ");
    result = shiftLeft(input0);
    printInput(result);

    return 0;
}