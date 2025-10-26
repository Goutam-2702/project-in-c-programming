#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    int roll;
    char name[50];
    float marks;
};

void addStudent();
void displayStudents();
void searchStudent();
void deleteStudent();
void updateStudent();

int main() {
    int choice;
    while (1) {
        printf("\n===== STUDENT MANAGEMENT SYSTEM =====\n");
        printf("1. Add Student Record\n");
        printf("2. Display All Records\n");
        printf("3. Search Student\n");
        printf("4. Update Student Record\n");
        printf("5. Delete Student Record\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // clear input buffer

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: 
                printf("Exiting program...\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

void addStudent() {
    FILE *fp;
    struct Student s;
    fp = fopen("students.txt", "a");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Roll Number: ");
    scanf("%d", &s.roll);
    getchar();
    printf("Enter Name: ");
    fgets(s.name, 50, stdin);
    s.name[strcspn(s.name, "\n")] = '\0';  // remove newline
    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fprintf(fp, "%d,%s,%.2f\n", s.roll, s.name, s.marks);
    fclose(fp);

    printf("✅ Record added successfully!\n");
}

void displayStudents() {
    FILE *fp;
    struct Student s;
    fp = fopen("students.txt", "r");
    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("\n---- Student Records ----\n");
    printf("%-10s %-20s %-10s\n", "Roll", "Name", "Marks");
    printf("---------------------------------------------\n");

    while (fscanf(fp, "%d,%49[^,],%f\n", &s.roll, s.name, &s.marks) == 3) {
        printf("%-10d %-20s %-10.2f\n", s.roll, s.name, s.marks);
    }
    fclose(fp);
}

void searchStudent() {
    FILE *fp;
    struct Student s;
    int roll, found = 0;

    fp = fopen("students.txt", "r");
    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("Enter Roll Number to Search: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d,%49[^,],%f\n", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == roll) {
            printf("\nRecord Found:\n");
            printf("Roll: %d\nName: %s\nMarks: %.2f\n", s.roll, s.name, s.marks);
            found = 1;
            break;
        }
    }
    if (!found)
        printf("❌ Record not found!\n");

    fclose(fp);
}

void deleteStudent() {
    FILE *fp, *temp;
    struct Student s;
    int roll, found = 0;

    fp = fopen("students.txt", "r");
    temp = fopen("temp.txt", "w");

    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("Enter Roll Number to Delete: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d,%49[^,],%f\n", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll != roll) {
            fprintf(temp, "%d,%s,%.2f\n", s.roll, s.name, s.marks);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove("students.txt");
    rename("temp.txt", "students.txt");

    if (found)
        printf("✅ Record deleted successfully!\n");
    else
        printf("❌ Record not found!\n");
}

void updateStudent() {
    FILE *fp, *temp;
    struct Student s;
    int roll, found = 0;

    fp = fopen("students.txt", "r");
    temp = fopen("temp.txt", "w");

    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("Enter Roll Number to Update: ");
    scanf("%d", &roll);
    getchar();

    while (fscanf(fp, "%d,%49[^,],%f\n", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == roll) {
            found = 1;
            printf("Enter New Name: ");
            fgets(s.name, 50, stdin);
            s.name[strcspn(s.name, "\n")] = '\0';
            printf("Enter New Marks: ");
            scanf("%f", &s.marks);
        }
        fprintf(temp, "%d,%s,%.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    remove("students.txt");
    rename("temp.txt", "students.txt");

    if (found)
        printf("✅ Record updated successfully!\n");
    else
        printf("❌ Record not found!\n");
}
