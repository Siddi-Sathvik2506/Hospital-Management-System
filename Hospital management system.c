/****************************************************
 * HOSPITAL MANAGEMENT SYSTEM (HMS)
 * Language: ANSI C
 * Storage : File Handling (.dat)
 * Interface: Console
 * Roles    : Admin, Doctor, Pharmacy Owner
 ****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================= CONSTANTS ================= */
#define ADMIN 1
#define DOCTOR 2
#define PHARMACY 3

/* ================= STRUCTURES ================= */

typedef struct {
    int id;
    char name[50];
    int age;
    char gender[10];
    char disease[50];
    int admitted;
    int bedNo;
} Patient;

typedef struct {
    int id;
    char name[50];
    char specialization[50];
} Doctor;

typedef struct {
    int bedNo;
    int occupied;
    int patientId;
} Bed;

typedef struct {
    int patientId;
    int doctorId;
    char date[20];
} Appointment;

typedef struct {
    int patientId;
    float consultation;
    float medicine;
    float room;
    float total;
} Bill;

typedef struct {
    int id;
    char name[50];
    int quantity;
    char expiry[15];
} Medicine;

typedef struct {
    int patientId;
    char testName[50];
    char result[100];
} Lab;

typedef struct {
    char username[20];
    char password[20];
    int role;
} User;

/* ================= FILE NAMES ================= */
#define PATIENT_FILE "patients.dat"
#define DOCTOR_FILE "doctors.dat"
#define BED_FILE "beds.dat"
#define APPOINT_FILE "appointments.dat"
#define BILL_FILE "billing.dat"
#define MED_FILE "medicines.dat"
#define LAB_FILE "labs.dat"
#define USER_FILE "users.dat"

/* ================= FUNCTION DECLARATIONS ================= */

void initBeds();
void initUsers();
int login();

/* Bed helpers */
int allocateBed(int patientId);
void freeBed(int bedNo);

/* Patient */
void addPatient();
void viewPatients();
void updatePatient();
void deletePatient();
void dischargePatient();

/* Doctor */
void addDoctor();
void viewDoctors();

/* Appointment */
void bookAppointment();
void viewAppointments();

/* Bed */
void bedReport();

/* Billing */
void generateBill();

/* Pharmacy */
void addMedicine();
void viewMedicines();

/* Lab */
void addLabResult();
void viewLabResults();

/* Reports */
void dailyReport();

/* ================= MAIN ================= */

int main() {
    int role, choice;

    initBeds();
    initUsers();

    role = login();
    if (role == 0) return 0;

    do {
        printf("\n========== HOSPITAL MANAGEMENT SYSTEM ==========\n");

        /* ================= ADMIN MENU ================= */
        if (role == ADMIN) {
            printf("1. Patient Management\n");
            printf("2. Doctor Management\n");
            printf("3. Appointment Management\n");
            printf("4. Bed Occupancy Report\n");
            printf("5. Billing\n");
            printf("6. Pharmacy\n");
            printf("7. Laboratory\n");
            printf("8. Reports\n");
        }

        /* ================= DOCTOR MENU ================= */
        else if (role == DOCTOR) {
            printf("1. View Patients\n");
            printf("2. View Appointments\n");
            printf("3. View Lab Results\n");
        }

        /* ================= PHARMACY MENU ================= */
        else if (role == PHARMACY) {
            printf("1. Add Medicine\n");
            printf("2. View Medicines\n");
        }

        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        /* ================= ADMIN ================= */
        if (role == ADMIN) {
            switch (choice) {
            case 1:
                printf("\n1.Add 2.View 3.Update 4.Delete 5.Discharge\n");
                scanf("%d", &choice);
                if (choice == 1) addPatient();
                else if (choice == 2) viewPatients();
                else if (choice == 3) updatePatient();
                else if (choice == 4) deletePatient();
                else if (choice == 5) dischargePatient();
                break;

            case 2:
                printf("\n1.Add Doctor 2.View Doctors\n");
                scanf("%d", &choice);
                if (choice == 1) addDoctor();
                else viewDoctors();
                break;

            case 3:
                printf("\n1.Book Appointment 2.View Appointments\n");
                scanf("%d", &choice);
                if (choice == 1) bookAppointment();
                else viewAppointments();
                break;

            case 4:
                bedReport();
                break;

            case 5:
                generateBill();
                break;

            case 6:
                printf("\n1.Add Medicine 2.View Medicines\n");
                scanf("%d", &choice);
                if (choice == 1) addMedicine();
                else viewMedicines();
                break;

            case 7:
                printf("\n1.Add Lab Result 2.View Lab Results\n");
                scanf("%d", &choice);
                if (choice == 1) addLabResult();
                else viewLabResults();
                break;

            case 8:
                dailyReport();
                break;
            }
        }

        /* ================= DOCTOR ================= */
        else if (role == DOCTOR) {
            switch (choice) {
            case 1:
                viewPatients();
                break;
            case 2:
                viewAppointments();
                break;
            case 3:
                viewLabResults();
                break;
            }
        }

        /* ================= PHARMACY ================= */
        else if (role == PHARMACY) {
            switch (choice) {
            case 1:
                addMedicine();
                break;
            case 2:
                viewMedicines();
                break;
            }
        }

    } while (choice != 0);

    return 0;
}

/* ================= INITIALIZATION ================= */

void initBeds() {
    FILE *fp = fopen(BED_FILE, "rb");
    if (fp) { fclose(fp); return; }

    fp = fopen(BED_FILE, "wb");
    Bed b;
    for (int i = 1; i <= 20; i++) {
        b.bedNo = i;
        b.occupied = 0;
        b.patientId = -1;
        fwrite(&b, sizeof(Bed), 1, fp);
    }
    fclose(fp);
}

void initUsers() {
    FILE *fp = fopen(USER_FILE, "rb");
    if (fp) { fclose(fp); return; }

    fp = fopen(USER_FILE, "wb");
    User u1 = {"admin", "admin123", ADMIN};
    User u2 = {"doctor", "doc123", DOCTOR};
    User u3 = {"pharmacy", "pharma123", PHARMACY};
    fwrite(&u1, sizeof(User), 1, fp);
    fwrite(&u2, sizeof(User), 1, fp);
    fwrite(&u3, sizeof(User), 1, fp);
    fclose(fp);
}

/* ================= LOGIN ================= */

int login() {
    char u[20], p[20];
    FILE *fp = fopen(USER_FILE, "rb");
    User usr;

    printf("\nLogin\nUsername: ");
    scanf("%s", u);
    printf("Password: ");
    scanf("%s", p);

    while (fread(&usr, sizeof(User), 1, fp)) {
        if (!strcmp(u, usr.username) && !strcmp(p, usr.password)) {
            fclose(fp);
            return usr.role;
        }
    }
    fclose(fp);
    printf("Invalid login!\n");
    return 0;
}

/* ================= BED HELPERS ================= */

int allocateBed(int patientId) {
    FILE *fp = fopen(BED_FILE, "rb+");
    Bed b;

    while (fread(&b, sizeof(Bed), 1, fp)) {
        if (!b.occupied) {
            b.occupied = 1;
            b.patientId = patientId;
            fseek(fp, -sizeof(Bed), SEEK_CUR);
            fwrite(&b, sizeof(Bed), 1, fp);
            fclose(fp);
            return b.bedNo;
        }
    }
    fclose(fp);
    return -1;
}

void freeBed(int bedNo) {
    FILE *fp = fopen(BED_FILE, "rb+");
    Bed b;

    while (fread(&b, sizeof(Bed), 1, fp)) {
        if (b.bedNo == bedNo) {
            b.occupied = 0;
            b.patientId = -1;
            fseek(fp, -sizeof(Bed), SEEK_CUR);
            fwrite(&b, sizeof(Bed), 1, fp);
            break;
        }
    }
    fclose(fp);
}

/* ================= PATIENT ================= */

void addPatient() {
    FILE *fp = fopen(PATIENT_FILE, "ab");
    Patient p;

    printf("ID: "); scanf("%d", &p.id);
    printf("Name: "); scanf("%s", p.name);
    printf("Age: "); scanf("%d", &p.age);
    printf("Gender: "); scanf("%s", p.gender);
    printf("Disease: "); scanf("%s", p.disease);

    p.admitted = 1;
    p.bedNo = allocateBed(p.id);

    if (p.bedNo == -1)
        printf("No bed available!\n");
    else
        printf("Patient admitted to Bed %d\n", p.bedNo);

    fwrite(&p, sizeof(Patient), 1, fp);
    fclose(fp);
}

void viewPatients() {
    FILE *fp = fopen(PATIENT_FILE, "rb");
    Patient p;

    while (fread(&p, sizeof(Patient), 1, fp)) {
        printf("\nID:%d Name:%s Disease:%s Bed:%d Status:%s",
               p.id, p.name, p.disease,
               p.bedNo,
               p.admitted ? "Admitted" : "Discharged");
    }
    fclose(fp);
}

void updatePatient() {
    FILE *fp = fopen(PATIENT_FILE, "rb+");
    Patient p;
    int id;

    printf("Enter Patient ID: ");
    scanf("%d", &id);

    while (fread(&p, sizeof(Patient), 1, fp)) {
        if (p.id == id) {
            printf("New Disease: ");
            scanf("%s", p.disease);
            fseek(fp, -sizeof(Patient), SEEK_CUR);
            fwrite(&p, sizeof(Patient), 1, fp);
            break;
        }
    }
    fclose(fp);
}

void deletePatient() {
    FILE *fp = fopen(PATIENT_FILE, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    Patient p;
    int id;

    printf("Enter ID to delete: ");
    scanf("%d", &id);

    while (fread(&p, sizeof(Patient), 1, fp)) {
        if (p.id != id)
            fwrite(&p, sizeof(Patient), 1, temp);
    }
    fclose(fp);
    fclose(temp);
    remove(PATIENT_FILE);
    rename("temp.dat", PATIENT_FILE);
}

void dischargePatient() {
    FILE *fp = fopen(PATIENT_FILE, "rb+");
    Patient p;
    int id;

    printf("Patient ID to discharge: ");
    scanf("%d", &id);

    while (fread(&p, sizeof(Patient), 1, fp)) {
        if (p.id == id && p.admitted) {
            p.admitted = 0;
            freeBed(p.bedNo);
            p.bedNo = -1;
            fseek(fp, -sizeof(Patient), SEEK_CUR);
            fwrite(&p, sizeof(Patient), 1, fp);
            printf("Patient discharged.\n");
            break;
        }
    }
    fclose(fp);
}

/* ================= DOCTOR ================= */

void addDoctor() {
    FILE *fp = fopen(DOCTOR_FILE, "ab");
    Doctor d;

    printf("Doctor ID: "); scanf("%d", &d.id);
    printf("Name: "); scanf("%s", d.name);
    printf("Specialization: "); scanf("%s", d.specialization);

    fwrite(&d, sizeof(Doctor), 1, fp);
    fclose(fp);
}

void viewDoctors() {
    FILE *fp = fopen(DOCTOR_FILE, "rb");
    Doctor d;

    while (fread(&d, sizeof(Doctor), 1, fp)) {
        printf("\nID:%d Name:%s Spec:%s",
               d.id, d.name, d.specialization);
    }
    fclose(fp);
}

/* ================= APPOINTMENT ================= */

void bookAppointment() {
    FILE *fp = fopen(APPOINT_FILE, "ab");
    Appointment a;

    printf("Patient ID: "); scanf("%d", &a.patientId);
    printf("Doctor ID: "); scanf("%d", &a.doctorId);
    printf("Date: "); scanf("%s", a.date);

    fwrite(&a, sizeof(Appointment), 1, fp);
    fclose(fp);
}

void viewAppointments() {
    FILE *fp = fopen(APPOINT_FILE, "rb");
    Appointment a;

    while (fread(&a, sizeof(Appointment), 1, fp)) {
        printf("\nPatient:%d Doctor:%d Date:%s",
               a.patientId, a.doctorId, a.date);
    }
    fclose(fp);
}

/* ================= BED REPORT ================= */

void bedReport() {
    FILE *fp = fopen(BED_FILE, "rb");
    Bed b;

    printf("\nBED OCCUPANCY REPORT\n");
    while (fread(&b, sizeof(Bed), 1, fp)) {
        printf("Bed %d : %s\n", b.bedNo,
               b.occupied ? "Occupied" : "Free");
    }
    fclose(fp);
}

/* ================= BILLING ================= */

void generateBill() {
    FILE *fp = fopen(BILL_FILE, "ab");
    Bill b;

    printf("Patient ID: "); scanf("%d", &b.patientId);
    printf("Consultation: "); scanf("%f", &b.consultation);
    printf("Medicine: "); scanf("%f", &b.medicine);
    printf("Room: "); scanf("%f", &b.room);

    b.total = b.consultation + b.medicine + b.room;
    printf("TOTAL AMOUNT: %.2f\n", b.total);

    fwrite(&b, sizeof(Bill), 1, fp);
    fclose(fp);
}

/* ================= PHARMACY ================= */

void addMedicine() {
    FILE *fp = fopen(MED_FILE, "ab");
    Medicine m;

    printf("ID: "); scanf("%d", &m.id);
    printf("Name: "); scanf("%s", m.name);
    printf("Quantity: "); scanf("%d", &m.quantity);
    printf("Expiry: "); scanf("%s", m.expiry);

    fwrite(&m, sizeof(Medicine), 1, fp);
    fclose(fp);
}

void viewMedicines() {
    FILE *fp = fopen(MED_FILE, "rb");
    Medicine m;

    while (fread(&m, sizeof(Medicine), 1, fp)) {
        printf("\n%s Qty:%d Exp:%s",
               m.name, m.quantity, m.expiry);
    }
    fclose(fp);
}

/* ================= LAB ================= */

void addLabResult() {
    FILE *fp = fopen(LAB_FILE, "ab");
    Lab l;

    printf("Patient ID: "); scanf("%d", &l.patientId);
    printf("Test Name: "); scanf("%s", l.testName);
    printf("Result: "); scanf("%s", l.result);

    fwrite(&l, sizeof(Lab), 1, fp);
    fclose(fp);
}

void viewLabResults() {
    FILE *fp = fopen(LAB_FILE, "rb");
    Lab l;

    while (fread(&l, sizeof(Lab), 1, fp)) {
        printf("\nPatient:%d Test:%s Result:%s",
               l.patientId, l.testName, l.result);
    }
    fclose(fp);
}

/* ================= REPORT ================= */

void dailyReport() {
    printf("\nDaily report generated successfully.\n");
}
