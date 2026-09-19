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

int specialtyQueue[4] = {0};

// ==========================================
// 3. FUNCTION DECLARATIONS & IMPLEMENTATION
// ==========================================

// Patient Registration Function
void registerPatient() {
    if (patientCount >= 10) {
        printf("\nError: System memory full! Cannot register more patients.\n");
        return;
    }

    printf("\n--- PATIENT REGISTRATION ---\n");

    // Patient Name
    printf("Enter Patient Name: ");
    getchar(); // Clear buffer
    fgets(patientName[patientCount], 50, stdin);
    patientName[patientCount][strcspn(patientName[patientCount], "\n")] = 0;

    // Patient Age
    printf("Enter Patient Age: ");
    scanf("%d", &patientAge[patientCount]);

    // Urgency Level
    printf("\nUrgency/Triage Level:\n");
    printf("1. Normal\n2. Urgent\n3. Critical\n");
    printf("Select Urgency Level (1-3): ");
    scanf("%d", &urgencyLevel[patientCount]);

    // Doctor Specialty Selection
    printf("\nDoctor Specialties:\n");
    for (int i = 0; i < 4; i++) {
        printf("%d. %s (Fee: LKR %.2f)\n", specialtyID[i], specialtyName[i], consultationFee[i]);
    }
    printf("Select Specialty ID (1-4): ");
    scanf("%d", &patientSpecialty[patientCount]);

    // Update Queue
    int specIndex = patientSpecialty[patientCount] - 1;
    specialtyQueue[specIndex]++;

    // Ward Admission Details
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

        // Find available bed in 2D Bed Occupancy Matrix
        int allocated = 0;
        for (int b = 0; b < bedCapacity[wIndex]; b++) {
            if (bedOccupancy[wIndex][b] == 0) {
                bedOccupancy[wIndex][b] = 1; // Mark bed as occupied
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

    printf("\nPatient %s registered successfully!\n", patientName[patientCount]);
    patientCount++;
}

// ==========================================
// 4. MAIN FUNCTION
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
                printf("\n[ Triage Priority Display Coming Soon ]\n");
                break;
            case 3:
                printf("\n[ Bed Status Feature Coming Soon ]\n");
                break;
            case 4:
                printf("\n[ Analytics Report Feature Coming Soon ]\n");
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
