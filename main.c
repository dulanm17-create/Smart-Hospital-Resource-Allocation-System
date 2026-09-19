#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================================
// 1. LOOKUP DATA & CONSTANTS
// ==========================================
int specialtyID[4] = {1, 2, 3, 4};
char specialtyName[4][30] = {
    "General Practice",
    "Paediatrics",
    "Cardiology",
    "Neurology"
};
float consultationFee[4] = {1500.00, 2500.00, 4500.00, 5000.00};
int consultationTime[4] = {15, 20, 30, 30};

int wardID[4] = {1, 2, 3, 4};
char wardName[4][30] = {
    "General Ward",
    "Paediatric Ward",
    "Surgical Ward",
    "ICU"
};
float wardDailyRate[4] = {3000.00, 6000.00, 12000.00, 25000.00};
int bedCapacity[4] = {20, 10, 10, 5};
int bedOccupancy[4][20] = {0}; // 0 = Available, 1 = Occupied

// ==========================================
// 2. PATIENT DATA & QUEUE TRACKING ARRAYS
// ==========================================
int patientCount = 0;
char patientName[10][50];
int patientAge[10];
int urgencyLevel[10];
int patientSpecialty[10];
int admitted[10];
int patientWard[10];
int assignedBed[10];
int daysAdmitted[10];
int originalID[10];

int specialtyQueue[4] = {0};

// ==========================================
// 3. CALCULATION FUNCTIONS (REQUIREMENT 3)
// ==========================================

float calculateWaitingTime(int specIndex) {
    return (float)(specialtyQueue[specIndex] * consultationTime[specIndex]);
}

float calculateSurcharge(float baseFee, int urgency) {
    if (urgency == 2) {
        return baseFee * 0.20f;
    } else if (urgency == 3) {
        return baseFee * 0.50f;
    }
    return 0.0f;
}

float calculateWardCost(int isAdmitted, int wIndex, int days) {
    if (isAdmitted == 1 && wIndex >= 0 && wIndex < 4) {
        return days * wardDailyRate[wIndex];
    }
    return 0.0f;
}

float calculateDiscount(int age, float grossTotal) {
    if (age < 5 || age > 65) {
        return grossTotal * 0.15f;
    }
    return 0.0f;
}

// ==========================================
// 4. FILE HANDLING FUNCTIONS (BONUS REQUIREMENT)
// ==========================================

void savePatientToLog(int index) {
    FILE *file = fopen("patient_log.txt", "a");
    if (file == NULL) {
        printf("Error: Could not open patient_log.txt for writing.\n");
        return;
    }

    int sIdx = patientSpecialty[index] - 1;
    int wIdx = patientWard[index] - 1;

    float baseFee = consultationFee[sIdx];
    float surcharge = calculateSurcharge(baseFee, urgencyLevel[index]);
    float wardCost = calculateWardCost(admitted[index], wIdx, daysAdmitted[index]);
    float grossTotal = baseFee + surcharge + wardCost;
    float discount = calculateDiscount(patientAge[index], grossTotal);
    float finalPayable = grossTotal - discount;

    fprintf(file, "ID: PAT-%d | Name: %s | Age: %d | Urgency: Level %d | Specialty: %s | Admitted: %s | Final Bill: LKR %.2f\n",
            originalID[index],
            patientName[index],
            patientAge[index],
            urgencyLevel[index],
            specialtyName[sIdx],
            (admitted[index] == 1) ? "Yes" : "No",
            finalPayable);

    fclose(file);
}

void saveBedStatusToFile() {
    FILE *file = fopen("bed_status.txt", "w");
    if (file == NULL) {
        printf("Error: Could not open bed_status.txt for writing.\n");
        return;
    }

    fprintf(file, "=========================================================\n");
    fprintf(file, "             HOSPITAL WARDS & BED STATUS                 \n");
    fprintf(file, "=========================================================\n");

    for (int w = 0; w < 4; w++) {
        int occupiedCount = 0;
        for (int b = 0; b < bedCapacity[w]; b++) {
            if (bedOccupancy[w][b] == 1) occupiedCount++;
        }

        fprintf(file, "\nWard %d: %s | Occupancy: %d/%d beds\n", wardID[w], wardName[w], occupiedCount, bedCapacity[w]);
        for (int b = 0; b < bedCapacity[w]; b++) {
            if (bedOccupancy[w][b] == 1) {
                fprintf(file, "[X] B%02d ", b + 1);
            } else {
                fprintf(file, "[ ] B%02d ", b + 1);
            }

            if ((b + 1) % 5 == 0) {
                fprintf(file, "\n");
            }
        }
        fprintf(file, "\n---------------------------------------------------------\n");
    }

    fclose(file);
}

// ==========================================
// 5. BILL DISPLAY FUNCTION (REQUIREMENT 5)
// ==========================================

void printPatientBill(int index) {
    int sIdx = patientSpecialty[index] - 1;
    int wIdx = patientWard[index] - 1;

    float baseFee = consultationFee[sIdx];
    float surcharge = calculateSurcharge(baseFee, urgencyLevel[index]);
    float wardCost = calculateWardCost(admitted[index], wIdx, daysAdmitted[index]);
    float grossTotal = baseFee + surcharge + wardCost;
    float discount = calculateDiscount(patientAge[index], grossTotal);
    float finalPayable = grossTotal - discount;
    float waitTime = calculateWaitingTime(sIdx);

    printf("\n==================================================\n");
    printf("         SMART HOSPITAL ADMISSION & BILL          \n");
    printf("==================================================\n");
    printf("Patient ID              : PAT-%d\n", originalID[index]);
    printf("Patient Name            : %s\n", patientName[index]);

    if (patientAge[index] < 5 || patientAge[index] > 65) {
        printf("Age                     : %d Years (15%% Subsidy Eligible)\n", patientAge[index]);
    } else {
        printf("Age                     : %d Years\n", patientAge[index]);
    }

    printf("Specialty               : %s\n", specialtyName[sIdx]);

    if (admitted[index] == 1) {
        printf("Assigned Ward           : %s (Bed #%02d)\n", wardName[wIdx], assignedBed[index]);
    } else {
        printf("Assigned Ward           : None (OPD Patient)\n");
    }

    if (urgencyLevel[index] == 3) {
        printf("Urgency Level           : Level 3 (Critical)\n");
    } else if (urgencyLevel[index] == 2) {
        printf("Urgency Level           : Level 2 (Urgent)\n");
    } else {
        printf("Urgency Level           : Level 1 (Normal)\n");
    }

    printf("--------------------------------------------------\n");
    printf("Base Consultation Fee   : LKR %.2f\n", baseFee);
    printf("Emergency Surcharge     : LKR %.2f\n", surcharge);
    printf("Ward Stay Cost (%d Days) : LKR %.2f\n", daysAdmitted[index], wardCost);
    printf("Gross Total Bill        : LKR %.2f\n", grossTotal);
    printf("Age Subsidy Discount    : LKR -%.2f\n", discount);
    printf("--------------------------------------------------\n");
    printf("Final Payable Amount    : LKR %.2f\n", finalPayable);

    if (urgencyLevel[index] == 3) {
        printf("Estimated Waiting Time  : 0.00 mins (Immediate Attention)\n");
    } else {
        printf("Estimated Waiting Time  : %.2f mins\n", waitTime);
    }
    printf("==================================================\n\n");
}

// ==========================================
// 6. REGISTRATION FUNCTION
// ==========================================

void registerPatient() {
    if (patientCount >= 10) {
        printf("\nError: System memory full! Cannot register more patients.\n");
        return;
    }

    printf("\n--- PATIENT REGISTRATION ---\n");

    originalID[patientCount] = 1001 + patientCount;

    printf("Enter Patient Name: ");
    getchar();
    fgets(patientName[patientCount], 50, stdin);
    patientName[patientCount][strcspn(patientName[patientCount], "\n")] = 0;

    printf("Enter Patient Age: ");
    scanf("%d", &patientAge[patientCount]);

    printf("\nUrgency/Triage Level:\n");
    printf("1. Normal\n2. Urgent\n3. Critical\n");
    printf("Select Urgency Level (1-3): ");
    scanf("%d", &urgencyLevel[patientCount]);

    printf("\nDoctor Specialties:\n");
    for (int i = 0; i < 4; i++) {
        printf("%d. %s (Fee: LKR %.2f)\n", specialtyID[i], specialtyName[i], consultationFee[i]);
    }
    printf("Select Specialty ID (1-4): ");
    scanf("%d", &patientSpecialty[patientCount]);

    int specIndex = patientSpecialty[patientCount] - 1;

    printf("\nIs Admitted to Ward? (1 = Yes, 0 = No): ");
    scanf("%d", &admitted[patientCount]);

    if (admitted[patientCount] == 1) {
        printf("\nHospital Wards:\n");
        for (int i = 0; i < 4; i++) {
            printf("%d. %s - LKR %.2f/day (Beds: %d)\n", wardID[i], wardName[i], wardDailyRate[i], bedCapacity[i]);
        }
        printf("Select Ward ID (1-4): ");
        scanf("%d", &patientWard[patientCount]);

        int wIndex = patientWard[patientCount] - 1;

        int allocated = 0;
        for (int b = 0; b < bedCapacity[wIndex]; b++) {
            if (bedOccupancy[wIndex][b] == 0) {
                bedOccupancy[wIndex][b] = 1;
                assignedBed[patientCount] = b + 1;
                allocated = 1;
                break;
            }
        }

        if (!allocated) {
            printf("Warning: Selected Ward is full! Patient registered as OPD.\n");
            admitted[patientCount] = 0;
            patientWard[patientCount] = 0;
            assignedBed[patientCount] = 0;
            daysAdmitted[patientCount] = 0;
        } else {
            printf("Enter Days Admitted: ");
            scanf("%d", &daysAdmitted[patientCount]);
            printf("Bed #%02d in %s allocated successfully!\n", assignedBed[patientCount], wardName[wIndex]);
        }
    } else {
        patientWard[patientCount] = 0;
        assignedBed[patientCount] = 0;
        daysAdmitted[patientCount] = 0;
    }

    // Print Receipt
    printPatientBill(patientCount);

    // Save Data to Files
    savePatientToLog(patientCount);
    saveBedStatusToFile();
    printf("[Data successfully saved to patient_log.txt and bed_status.txt]\n");

    specialtyQueue[specIndex]++;

    patientCount++;
}

// ==========================================
// 7. TRIAGE PRIORITY SORTING (REQUIREMENT 4)
// ==========================================

void displayPatientsTriage() {
    if (patientCount == 0) {
        printf("\nNo registered patients in the system.\n");
        return;
    }

    char tempName[10][50];
    int tempAge[10], tempUrgency[10], tempSpecialty[10], tempAdmitted[10], tempWard[10], tempBed[10], tempDays[10], tempID[10];

    for (int i = 0; i < patientCount; i++) {
        strcpy(tempName[i], patientName[i]);
        tempAge[i] = patientAge[i];
        tempUrgency[i] = urgencyLevel[i];
        tempSpecialty[i] = patientSpecialty[i];
        tempAdmitted[i] = admitted[i];
        tempWard[i] = patientWard[i];
        tempBed[i] = assignedBed[i];
        tempDays[i] = daysAdmitted[i];
        tempID[i] = originalID[i];
    }

    for (int i = 0; i < patientCount - 1; i++) {
        for (int j = 0; j < patientCount - i - 1; j++) {
            if (tempUrgency[j] < tempUrgency[j + 1]) {
                int tU = tempUrgency[j]; tempUrgency[j] = tempUrgency[j + 1]; tempUrgency[j + 1] = tU;
                char tN[50]; strcpy(tN, tempName[j]); strcpy(tempName[j], tempName[j + 1]); strcpy(tempName[j + 1], tN);
                int tA = tempAge[j]; tempAge[j] = tempAge[j + 1]; tempAge[j + 1] = tA;
                int tS = tempSpecialty[j]; tempSpecialty[j] = tempSpecialty[j + 1]; tempSpecialty[j + 1] = tS;
                int tAd = tempAdmitted[j]; tempAdmitted[j] = tempAdmitted[j + 1]; tempAdmitted[j + 1] = tAd;
                int tW = tempWard[j]; tempWard[j] = tempWard[j + 1]; tempWard[j + 1] = tW;
                int tB = tempBed[j]; tempBed[j] = tempBed[j + 1]; tempBed[j + 1] = tB;
                int tD = tempDays[j]; tempDays[j] = tempDays[j + 1]; tempDays[j + 1] = tD;
                int tI = tempID[j]; tempID[j] = tempID[j + 1]; tempID[j + 1] = tI;
            }
        }
    }

    printf("\n=========================================================================\n");
    printf("             REGISTERED PATIENTS - TRIAGE PRIORITY ORDER                  \n");
    printf("=========================================================================\n");
    printf("%-10s %-20s %-5s %-12s %-18s %-10s\n", "ID", "Name", "Age", "Urgency", "Specialty", "Status");
    printf("-------------------------------------------------------------------------\n");

    for (int i = 0; i < patientCount; i++) {
        char urgStr[15];
        if (tempUrgency[i] == 3) strcpy(urgStr, "3-Critical");
        else if (tempUrgency[i] == 2) strcpy(urgStr, "2-Urgent");
        else strcpy(urgStr, "1-Normal");

        char statusStr[20];
        if (tempAdmitted[i] == 1) sprintf(statusStr, "Ward %d (B#%d)", tempWard[i], tempBed[i]);
        else strcpy(statusStr, "OPD");

        printf("PAT-%-6d %-20s %-5d %-12s %-18s %-10s\n",
               tempID[i], tempName[i], tempAge[i], urgStr, specialtyName[tempSpecialty[i] - 1], statusStr);
    }
    printf("=========================================================================\n");
}

// ==========================================
// 8. BED STATUS VISUALIZER (REQUIREMENT 1)
// ==========================================

void displayBedStatus() {
    printf("\n=========================================================\n");
    printf("             HOSPITAL WARDS & BED STATUS                 \n");
    printf("=========================================================\n");
    printf(" Legend: [X] = Occupied | [ ] = Available                \n");
    printf("---------------------------------------------------------\n");

    for (int w = 0; w < 4; w++) {
        int occupiedCount = 0;
        for (int b = 0; b < bedCapacity[w]; b++) {
            if (bedOccupancy[w][b] == 1) occupiedCount++;
        }

        printf("\nWard %d: %-18s (Rate: LKR %.2f/day)\n", wardID[w], wardName[w], wardDailyRate[w]);
        printf("Occupancy: %d/%d beds\n", occupiedCount, bedCapacity[w]);
        printf("Bed Layout:\n");

        for (int b = 0; b < bedCapacity[w]; b++) {
            if (bedOccupancy[w][b] == 1) {
                printf("[X] B%02d ", b + 1);
            } else {
                printf("[ ] B%02d ", b + 1);
            }

            if ((b + 1) % 5 == 0) {
                printf("\n");
            }
        }
        printf("---------------------------------------------------------\n");
    }
}

// ==========================================
// 9. SUMMARY REPORTS & ANALYTICS (REQUIREMENT 6)
// ==========================================

void generateAnalyticsReport() {
    printf("\n=========================================================\n");
    printf("         SMART HOSPITAL PERFORMANCE & ANALYTICS          \n");
    printf("=========================================================\n");

    if (patientCount == 0) {
        printf("No patient data available for analytics.\n");
        printf("=========================================================\n");
        return;
    }

    int lvl1 = 0, lvl2 = 0, lvl3 = 0;
    float totalRevenue = 0.0f;
    float totalDiscounts = 0.0f;

    float maxBill = -1.0f;
    char highestPayerName[50] = "None";

    for (int i = 0; i < patientCount; i++) {
        if (urgencyLevel[i] == 1) lvl1++;
        else if (urgencyLevel[i] == 2) lvl2++;
        else if (urgencyLevel[i] == 3) lvl3++;

        int sIdx = patientSpecialty[i] - 1;
        int wIdx = patientWard[i] - 1;

        float baseFee = consultationFee[sIdx];
        float surcharge = calculateSurcharge(baseFee, urgencyLevel[i]);
        float wardCost = calculateWardCost(admitted[i], wIdx, daysAdmitted[i]);
        float grossTotal = baseFee + surcharge + wardCost;
        float discount = calculateDiscount(patientAge[i], grossTotal);
        float finalPayable = grossTotal - discount;

        totalRevenue += finalPayable;
        totalDiscounts += discount;

        if (finalPayable > maxBill) {
            maxBill = finalPayable;
            strcpy(highestPayerName, patientName[i]);
        }
    }

    printf("1. Patient Intake Breakdown:\n");
    printf("   - Total Registered Patients : %d\n", patientCount);
    printf("   - Level 1 (Normal)          : %d\n", lvl1);
    printf("   - Level 2 (Urgent)          : %d\n", lvl2);
    printf("   - Level 3 (Critical)        : %d\n", lvl3);

    printf("\n2. Financial Summary:\n");
    printf("   - Total Revenue Earned      : LKR %.2f\n", totalRevenue);
    printf("   - Total Discounts Granted   : LKR %.2f\n", totalDiscounts);

    printf("\n3. Ward Bed Occupancy Rate:\n");
    for (int w = 0; w < 4; w++) {
        int occ = 0;
        for (int b = 0; b < bedCapacity[w]; b++) {
            if (bedOccupancy[w][b] == 1) occ++;
        }
        float percentage = ((float)occ / bedCapacity[w]) * 100.0f;
        printf("   - %-18s : %.1f%% (%d/%d beds)\n", wardName[w], percentage, occ, bedCapacity[w]);
    }

    printf("\n4. Highest Paying Patient:\n");
    printf("   - Name       : %s\n", highestPayerName);
    printf("   - Total Bill : LKR %.2f\n", maxBill);
    printf("=========================================================\n");
}

// ==========================================
// 10. MAIN FUNCTION
// ==========================================
int main()
{
    int choice;

    do {
        printf("\n=========================================\n");
        printf("    SMART HOSPITAL MANAGEMENT SYSTEM     \n");
        printf("=========================================\n");
        printf("1. Register New Patient\n");
        printf("2. Display All Patients (Triage Priority)\n");
        printf("3. Hospital Wards & Bed Status\n");
        printf("4. Generate Hospital Analytics Report\n");
        printf("5. Exit\n");
        printf("-----------------------------------------\n");
        printf("Enter your choice (1-5): ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                registerPatient();
                break;
            case 2:
                displayPatientsTriage();
                break;
            case 3:
                displayBedStatus();
                break;
            case 4:
                generateAnalyticsReport();
                break;
            case 5:
                printf("\nExiting System. Thank you!\n");
                break;
            default:
                printf("\nInvalid Choice! Please enter an option between 1-5.\n");
        }
    } while (choice != 5);

    return 0;
}
