#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_RECORDS 100
#define MAX_LEN 50

/* Parallel arrays: index i of each array belongs to the same medicine */
char names[MAX_RECORDS][MAX_LEN];
char brands[MAX_RECORDS][MAX_LEN];
int quantities[MAX_RECORDS];
int count = 0;

/* Read a line safely and remove the trailing newline */
void read_line(const char *prompt, char *buffer, int size)
{
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) == NULL)
    {
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}

/* Convert a string to lowercase (in place) */
void to_lower(char *s)
{
    int i;
    for (i = 0; s[i] != '\0'; i++)
    {
        s[i] = (char)tolower((unsigned char)s[i]);
    }
}

/* Return 1 if text is a whole non-negative number, otherwise 0 */
int is_number(const char *s)
{
    int i;
    if (s[0] == '\0')
        return 0;
    for (i = 0; s[i] != '\0'; i++)
    {
        if (!isdigit((unsigned char)s[i]))
            return 0;
    }
    return 1;
}

void print_header(void)
{
    printf("\n%-4s %-25s %-20s %8s\n", "No.", "Medicine Name", "Brand", "Quantity");
    printf("------------------------------------------------------------\n");
}

void add_medicine(void)
{
    char name[MAX_LEN], brand[MAX_LEN], qty[MAX_LEN];

    if (count >= MAX_RECORDS)
    {
        printf("Stock list is full.\n");
        return;
    }

    read_line("Medicine name: ", name, MAX_LEN);
    read_line("Brand: ", brand, MAX_LEN);
    read_line("Quantity: ", qty, MAX_LEN);

    if (name[0] == '\0' || brand[0] == '\0' || !is_number(qty))
    {
        printf("Invalid input. Name and brand are required; quantity must be a whole number.\n");
        return;
    }

    strcpy(names[count], name);
    strcpy(brands[count], brand);
    quantities[count] = atoi(qty);
    count++;
    printf("Medicine added!\n");
}

void view_all(void)
{
    int i;

    if (count == 0)
    {
        printf("No records yet.\n");
        return;
    }

    print_header();
    for (i = 0; i < count; i++)
    {
        printf("%-4d %-25s %-20s %8d\n", i + 1, names[i], brands[i], quantities[i]);
    }
}

void search_by_partial_name(void)
{
    char keyword[MAX_LEN], lowered[MAX_LEN];
    int i, found = 0;

    read_line("Enter part of the medicine name: ", keyword, MAX_LEN);

    if (keyword[0] == '\0')
    {
        printf("Please type at least one letter to search.\n");
        return;
    }

    to_lower(keyword);

    for (i = 0; i < count; i++)
    {
        strcpy(lowered, names[i]);
        to_lower(lowered);

        if (strstr(lowered, keyword) != NULL)   /* partial, case-insensitive match */
        {
            if (!found)                          /* print header once, on the first match */
                print_header();
            printf("%-4d %-25s %-20s %8d\n", i + 1, names[i], brands[i], quantities[i]);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No medicine found containing '%s'.\n", keyword);
    }
}

void update_quantity(void)
{
    char num[MAX_LEN], qty[MAX_LEN];
    int choice;

    if (count == 0)
    {
        printf("No records yet.\n");
        return;
    }

    view_all();
    read_line("Enter the No. to update: ", num, MAX_LEN);

    if (!is_number(num) || atoi(num) < 1 || atoi(num) > count)
    {
        printf("Invalid number.\n");
        return;
    }
    choice = atoi(num) - 1;

    read_line("New quantity: ", qty, MAX_LEN);
    if (!is_number(qty))
    {
        printf("Quantity must be a whole number.\n");
        return;
    }

    quantities[choice] = atoi(qty);
    printf("Quantity updated!\n");
}

int main(void)
{
    char choice[MAX_LEN] = "";

    while (strcmp(choice, "5") != 0)
    {
        printf("\n===== PHARMACY STOCK LIST =====\n");
        printf("1. Add medicine\n");
        printf("2. View all stock\n");
        printf("3. Search by medicine name (partial)\n");
        printf("4. Update quantity\n");
        printf("5. Exit\n");
        read_line("Choose an option (1-5): ", choice, MAX_LEN);

        if (strcmp(choice, "1") == 0)
            add_medicine();
        else if (strcmp(choice, "2") == 0)
            view_all();
        else if (strcmp(choice, "3") == 0)
            search_by_partial_name();
        else if (strcmp(choice, "4") == 0)
            update_quantity();
        else if (strcmp(choice, "5") == 0)
            printf("Goodbye!\n");
        else
            printf("Invalid option. Please enter a number from 1 to 5.\n");
    }

    return 0;
}