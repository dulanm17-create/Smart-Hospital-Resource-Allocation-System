#include <stdio.h>
#include <stdlib.h>

int specialtyID[4] = {1, 2, 3, 4};

char specialtyName[4][30] = {
    "General Practice",
    "Paediatrics",
    "Cardiology",
    "Neurology"
};

float consultationFee[4] = {
    1500.00,
    2500.00,
    4500.00,
    5000.00
};

int consultationTime[4] = {
    15, 20, 30, 30
};
int main()
{
    printf("SMART HOSPITAL SYSTEM\n");


printf("\nDoctor Specialties\n");
printf("-----------------------------\n");

for (int i = 0; i < 4; i++)
{
    printf("%d. %s - LKR %.2f - %d mins\n",
           specialtyID[i],
           specialtyName[i],
           consultationFee[i],
           consultationTime[i]);
}


    return 0;
}
