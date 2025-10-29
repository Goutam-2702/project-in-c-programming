#include <stdio.h>
#include <string.h>

// Structure to store booking details
struct Booking {
    char customerName[50];   // Name of the customer
    char from[50];           // Starting location
    char to[50];             // Destination location
    int days;                // Number of travel days
    float pricePerDay;       // Cost per day
    float total;             // Total = days * pricePerDay
};

int main() {
    struct Booking bookings[50];  // Array to store up to 50 bookings
    int n, i;
    float subtotal = 0, tax, grandTotal;
    const float TAX_RATE = 0.10;  // 10% service tax

    char agencyName[100], employeeName[50];

    // Display header
    printf("===========================================\n");
    printf("         TRAVEL AGENCY MANAGER SYSTEM       \n");
    printf("===========================================\n");

    // Input agency and employee details
    printf("Enter Travel Agency Name: ");
    fgets(agencyName, sizeof(agencyName), stdin);
    agencyName[strcspn(agencyName, "\n")] = '\0'; // Remove newline

    printf("Enter Employee Name: ");
    fgets(employeeName, sizeof(employeeName), stdin);
    employeeName[strcspn(employeeName, "\n")] = '\0'; // Remove newline

    // Input number of bookings
    printf("\nEnter number of bookings today: ");
    scanf("%d", &n);
    getchar(); // clear buffer

    // Input booking details for each customer
    for (i = 0; i < n; i++) {
        printf("\n--- Booking %d ---\n", i + 1);

        printf("Customer Name: ");
        fgets(bookings[i].customerName, sizeof(bookings[i].customerName), stdin);
        bookings[i].customerName[strcspn(bookings[i].customerName, "\n")] = '\0';

        printf("From (Source): ");
        fgets(bookings[i].from, sizeof(bookings[i].from), stdin);
        bookings[i].from[strcspn(bookings[i].from, "\n")] = '\0';

        printf("To (Destination): ");
        fgets(bookings[i].to, sizeof(bookings[i].to), stdin);
        bookings[i].to[strcspn(bookings[i].to, "\n")] = '\0';

        printf("Number of Days: ");
        scanf("%d", &bookings[i].days);

        printf("Price per Day (INR): ");
        scanf("%f", &bookings[i].pricePerDay);
        getchar(); // clear buffer

        // Calculate total cost for this booking
        bookings[i].total = bookings[i].days * bookings[i].pricePerDay;
        subtotal += bookings[i].total;
    }

    // Calculate tax and total amount
    tax = subtotal * TAX_RATE;
    grandTotal = subtotal + tax;

    // Print receipt
    printf("\n===========================================\n");
    printf("              TRAVEL BILL RECEIPT          \n");
    printf("===========================================\n");
    printf("Agency Name   : %s\n", agencyName);
    printf("Employee Name : %s\n", employeeName);
    printf("===========================================\n");
    printf("%-15s %-12s %-12s %-6s %-10s %-10s\n",
           "Customer", "From", "To", "Days", "Rate", "Total");
    printf("-------------------------------------------\n");

    // Print each booking record
    for (i = 0; i < n; i++) {
        printf("%-15s %-12s %-12s %-6d %-10.2f %-10.2f\n",
               bookings[i].customerName,
               bookings[i].from,
               bookings[i].to,
               bookings[i].days,
               bookings[i].pricePerDay,
               bookings[i].total);
    }

    // Print summary
    printf("-------------------------------------------\n");
    printf("Subtotal      : %.2f\n", subtotal);
    printf("Service Tax(10%%): %.2f\n", tax);
    printf("Grand Total   : %.2f\n", grandTotal);
    printf("===========================================\n");
    printf("   Thank You for Choosing %s!   \n", agencyName);
    printf("===========================================\n");

    return 0;
}
