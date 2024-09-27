#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

//https://www.geeksforgeeks.org/bitwise-operators-in-c-cpp/
//https://www.tutorialspoint.com/cprogramming/c_bitwise_operators.htm
//https://byjus.com/gate/bitwise-operators-in-c/


typedef uint32_t NewFloat;

NewFloat bits_string_to_nfloat(const char* string) {
    NewFloat result=0;
    int i=0;
    while (i<32) {
        if (string[i]=='1') {
            result=result | 1;
        }
        if (i<31) {
            result=result<<1;
        }
        i++;
    }

    return result;
}

void nfloat_debug(NewFloat f) {
    int i=31;
    while(i>=0) {
        printf("%d", ((f >> i) & 0x1));
        if (i==31 || i==26) {
            printf(" ");
        }
        i--;
    }

    printf("\n");
}




NewFloat float_to_nfloat(float f) {
    NewFloat binaryVal=0;
    uint32_t sign;
    if(f>0){
        sign=0;
    }
    else{
        sign=1;
    }

    uint32_t* bits = (uint32_t*)&f;

    uint32_t exponent=((*bits >> 23) & 0xFF)-127+15;

    uint32_t fraction = (*bits & 0x7FFFFF);
    binaryVal = (sign << 31) | (exponent << 26) | fraction<<3;
    return binaryVal;
}

void nfloat_print(NewFloat f){
    uint64_t result;
    uint64_t sign =(f >> 31)& 0x1;
    uint64_t exponent=((f >> 26) & 0x1F);
    if (exponent==0) {
        while ((f & 0x40000000)==0) {
            f <<= 1;
            exponent--;
        }
    }
    exponent=exponent+1023-15;

    uint64_t fraction=(f & 0x3FFFFFF);
    result=(sign << 63) | (exponent << 52) | fraction << 26;
    double* answer=(double*)&result;
    printf("%.7f\n",*answer);
}




NewFloat nfloat_double(NewFloat f) {
    int sign=(f >> 31) & 0x1;
    int exponent=(f >> 26) & 0x1F;
    int fraction = f & 0x3FFFFFF;
    if (exponent == 31) {
        return f;
    }
    else if (exponent==0 && fraction==0) {
        return f;
    }
    else {
        exponent=exponent+1;
        if (exponent == 31) {
            fraction=fraction<<1;
            exponent=30;
        }
    }

    return (sign << 31) | (exponent << 26) | fraction;
}





NewFloat nfloat_add(NewFloat a, NewFloat b) {
    uint32_t resultfraction;
    uint32_t resultExp;
    uint32_t signA = (a >> 31) & 0x1;
    uint32_t exponentA = (a >> 26) & 0x1F;
    uint32_t fractionA = a & 0x3FFFFFF;

    uint32_t signB = (b >> 31) & 0x1;
    uint32_t exponentB = (b >> 26) & 0x1F;
    uint32_t fractionB = b & 0x3FFFFFF;

    if (exponentA==0 && fractionA==0 && exponentB==0 && fractionB==0) {
        return 0;
    }

    if (exponentA==0 && fractionA==0) {
        return b;
    }

    if (exponentB==0 && fractionB==0) {
        return a;
    }
    if(signA==signB){
        resultExp=exponentA;
        fractionA = fractionA | 1 << 26;
        fractionB = fractionB | 1 << 26;
        if(exponentA<exponentB){
            resultExp=exponentB;
            fractionA=fractionA>>(exponentB-exponentA);
        }
        else if(exponentB<exponentA){
            resultExp=exponentA;
            fractionB=fractionB>>(exponentA-exponentB);
        }
        resultfraction = fractionA+fractionB;
        if((resultfraction & (1 <<27))>0){
            resultfraction= resultfraction >> 1;
            resultExp++;
        }
        resultfraction=resultfraction & 0x3FFFFFF;
        return (signA << 31) | (resultExp << 26) | resultfraction;
    }
    else{
        if((exponentB<exponentA) || ((exponentA==exponentB) && (fractionB<fractionA))){
            uint32_t temp1 = signA;
            signA=signB;
            signB=temp1;
            uint32_t temp2 = exponentA;
            exponentA=exponentB;
            exponentB=temp2;
            uint32_t temp3 = fractionA;
            fractionA=fractionB;
            fractionB=temp3;

        }
        resultExp=exponentB;
        fractionA = fractionA | 1 << 26;
        fractionB = fractionB | 1 << 26;
        fractionA=fractionA >> (exponentB-exponentA);
        resultfraction=fractionB-fractionA;
        while((resultfraction & (1<<26))==0){
            resultExp=resultExp-1;
            resultfraction=resultfraction<<1;
        }
        resultfraction=resultfraction & 0x3FFFFFF;
        return (signB << 31) | (resultExp << 26) | resultfraction;


    }




}


int main(void)
{
    int part_num;
    char s1[40];
    float f1, f2;
    NewFloat n1, n2;

    // This will scan the input for you! Uncomment pieces once you implement
    // the functions for that part
    while (1)
    {
        int res = scanf(" p%d", &part_num);

        if (res != 1)
        {
            break;
        }
        else if (part_num == 1)
        {
            scanf("%s", s1);
            n1 = bits_string_to_nfloat(s1);
            nfloat_debug(n1);
        }
        else if (part_num == 2)
        {
            scanf("%f", &f1);
            n1 = float_to_nfloat(f1);
            nfloat_debug(n1);
        }
        else if (part_num == 3)
        {
            scanf("%s", s1);
            n1 = bits_string_to_nfloat(s1);
            nfloat_print(n1);
        }
        else
        {
            scanf("%s ", s1);

            if (s1[0] == 'd')
            {
                scanf("%f", &f1);
                n1 = float_to_nfloat(f1);
                n1 = nfloat_double(n1);
            }
            else
            {
                scanf("%f %f", &f1, &f2);
                n1 = float_to_nfloat(f1);
                n2 = float_to_nfloat(f2);
                n1 = nfloat_add(n1, n2);
            }

            nfloat_print(n1);
        }
    }
}
