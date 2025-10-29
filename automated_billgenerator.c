#include <stdio.h>
#include <string.h>

// Structure to store details of each item
struct Item {
    char name[50];    // Item name
    int quantity;     // Quantity purchased
    float price;      // Price per unit
    float total;      // Total = quantity * price
};

int main() {
    struct Item items[50];  // Array of items
    int n, i;
    float subtotal = 0, gst, grandTotal;
    const float GST_RATE = 0.18;  // 18% GST

    // Strings to store shop, employee, and customer names
    char shopName[100], employeeName[50], customerName[50];

    // Input shop and customer details
    printf("=====================================\n");
    printf("        AUTOMATED BILL GENERATOR     \n");
    printf("=====================================\n");

    printf("Enter Shop Name: ");
    fgets(shopName, sizeof(shopName), stdin);
    shopName[strcspn(shopName, "\n")] = '\0'; // Remove newline

    printf("Enter Employee Name: ");
    fgets(employeeName, sizeof(employeeName), stdin);
    employeeName[strcspn(employeeName, "\n")] = '\0'; // Remove newline

    printf("Enter Customer Name: ");
    fgets(customerName, sizeof(customerName), stdin);
    customerName[strcspn(customerName, "\n")] = '\0'; // Remove newline

    // Number of items
    printf("\nEnter number of items: ");
    scanf("%d", &n);
    getchar(); // Clear buffer

    // Input item details
    for (i = 0; i < n; i++) {
        printf("\nEnter details of item %d\n", i + 1);
        printf("Item name: ");
        fgets(items[i].name, sizeof(items[i].name), stdin);
        items[i].name[strcspn(items[i].name, "\n")] = '\0'; // Remove newline

        printf("Quantity: ");
        scanf("%d", &items[i].quantity);
        printf("Price per item: ");
        scanf("%f", &items[i].price);
        getchar(); // Clear buffer

        items[i].total = items[i].quantity * items[i].price;
        subtotal += items[i].total; // Add to subtotal
    }

    // Calculate GST and final total
    gst = subtotal * GST_RATE;
    grandTotal = subtotal + gst;

    // Print bill
    printf("\n=====================================\n");
    printf("               BILL                  \n");
    printf("=====================================\n");

    printf("Shop Name     : %s\n", shopName);
    printf("Employee Name : %s\n", employeeName);
    printf("Customer Name : %s\n", customerName);
    printf("=====================================\n");

    // Print table headers
    printf("%-20s %-10s %-10s %-10s\n", "Item", "Qty", "Price", "Total");
    printf("-------------------------------------\n");

    // Print item details
    for (i = 0; i < n; i++) {
        printf("%-20s %-10d %-10.2f %-10.2f\n",
               items[i].name, items[i].quantity, items[i].price, items[i].total);
    }

    // Print totals
    printf("-------------------------------------\n");
    printf("Subtotal      : %.2f\n", subtotal);
    printf("GST (18%%)     : %.2f\n", gst);
    printf("Grand Total   : %.2f\n", grandTotal);
    printf("=====================================\n");
    printf("   Thank You for Shopping with Us!   \n");
    printf("=====================================\n");

    return 0;
}
