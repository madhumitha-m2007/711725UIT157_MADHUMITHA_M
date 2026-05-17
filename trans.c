
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100
#define PASSWORD "admin123"

struct clientData
{
    int acctNum;
    char firstName[30];
    char lastName[30];
    char gender[10];
    char phone[15];
    char address[100];
    char accountType[20];
    char password[20];
    double balance;
};

void createFile();
void login();
int menu();
void addAccount(FILE *);
void displayAccounts(FILE *);
void searchAccount(FILE *);
void updateAccount(FILE *);
void deleteAccount(FILE *);
void depositMoney(FILE *);
void withdrawMoney(FILE *);
void transferMoney(FILE *);
void calculateInterest(FILE *);
void transactionHistory(char []);
void exportTextFile(FILE *);
void clearInputBuffer();
void pauseScreen();
void accountStatistics(FILE *);
void sortAccounts(FILE *);
void changePassword(FILE *);
void miniStatement(FILE *);
void lowBalanceAccounts(FILE *);
void accountTypeSummary(FILE *);
void welcome();

int main()
{
    FILE *cfPtr;

    welcome();

    createFile();

    login();

    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("Unable to open file.\n");
        return 1;
    }

    int choice;

    while ((choice = menu()) != 15)
    {
        switch (choice)
        {
            case 1:
                addAccount(cfPtr);
                break;

            case 2:
                displayAccounts(cfPtr);
                break;

            case 3:
                searchAccount(cfPtr);
                break;

            case 4:
                updateAccount(cfPtr);
                break;

            case 5:
                deleteAccount(cfPtr);
                break;

            case 6:
                depositMoney(cfPtr);
                break;

            case 7:
                withdrawMoney(cfPtr);
                break;

            case 8:
                transferMoney(cfPtr);
                break;

            case 9:
                calculateInterest(cfPtr);
                break;

            case 10:
                exportTextFile(cfPtr);
                break;

            case 11:
                accountStatistics(cfPtr);
                break;

            case 12:
                sortAccounts(cfPtr);
                break;

            case 13:
                changePassword(cfPtr);
                break;

            case 14:
                miniStatement(cfPtr);
                break;

            default:
                printf("Invalid Choice\n");
        }

        pauseScreen();
    }

    fclose(cfPtr);

    printf("\nThank You For Using Bank Management System\n");

    return 0;
}

void welcome()
{
    printf("============================================\n");
    printf("      BANK MANAGEMENT SYSTEM PROJECT        \n");
    printf("============================================\n");
}

void login()
{
    char pass[20];

    printf("Enter Admin Password: ");
    scanf("%19s", pass);

    if (strcmp(pass, PASSWORD) != 0)
    {
        printf("Wrong Password\n");
        exit(0);
    }

    printf("Login Successful\n");
}

void createFile()
{
    FILE *fPtr;

    struct clientData blank = {0, "", "", "", "", "", "", "", 0.0};

    if ((fPtr = fopen("credit.dat", "rb")) == NULL)
    {
        fPtr = fopen("credit.dat", "wb");

        for (int i = 0; i < MAX; i++)
        {
            fwrite(&blank, sizeof(struct clientData), 1, fPtr);
        }

        fclose(fPtr);
    }
}

int menu()
{
    int choice;

    printf("\n========== MAIN MENU ==========\n");
    printf("1. Add New Account\n");
    printf("2. Display All Accounts\n");
    printf("3. Search Account\n");
    printf("4. Update Account\n");
    printf("5. Delete Account\n");
    printf("6. Deposit Money\n");
    printf("7. Withdraw Money\n");
    printf("8. Transfer Money\n");
    printf("9. Calculate Interest\n");
    printf("10. Export Accounts to Text File\n");
    printf("11. Account Statistics\n");
    printf("12. Sort Accounts\n");
    printf("13. Change Account Password\n");
    printf("14. Mini Statement\n");
    printf("15. Exit\n");

    printf("Enter Choice: ");
    scanf("%d", &choice);

    return choice;
}

void addAccount(FILE *fPtr)
{
    struct clientData client = {0};

    int account;

    printf("Enter Account Number (1-100): ");
    scanf("%d", &account);

    if (account < 1 || account > 100)
    {
        printf("Invalid Account Number\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account Already Exists\n");
        return;
    }

    client.acctNum = account;

    printf("Enter First Name: ");
    scanf("%29s", client.firstName);

    printf("Enter Last Name: ");
    scanf("%29s", client.lastName);

    printf("Enter Gender: ");
    scanf("%9s", client.gender);

    printf("Enter Phone Number: ");
    scanf("%14s", client.phone);

    clearInputBuffer();

    printf("Enter Address: ");
    fgets(client.address, sizeof(client.address), stdin);
    client.address[strcspn(client.address, "\n")] = '\0';

    printf("Enter Account Type: ");
    scanf("%19s", client.accountType);

    printf("Create Password: ");
    scanf("%19s", client.password);

    printf("Enter Initial Balance: ");
    scanf("%lf", &client.balance);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Account Created Successfully\n");

    transactionHistory("New Account Added");
}

void displayAccounts(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n%-6s %-15s %-15s %-10s %-15s %-10s\n",
           "ID",
           "First Name",
           "Last Name",
           "Gender",
           "Phone",
           "Balance");

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%-6d %-15s %-15s %-10s %-15s %.2lf\n",
                   client.acctNum,
                   client.firstName,
                   client.lastName,
                   client.gender,
                   client.phone,
                   client.balance);
        }
    }
}

void searchAccount(FILE *fPtr)
{
    struct clientData client;

    int account;

    printf("Enter Account Number: ");
    scanf("%d", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account Not Found\n");
        return;
    }

    printf("\n========== ACCOUNT DETAILS ==========\n");
    printf("Account Number : %d\n", client.acctNum);
    printf("First Name     : %s\n", client.firstName);
    printf("Last Name      : %s\n", client.lastName);
    printf("Gender         : %s\n", client.gender);
    printf("Phone          : %s\n", client.phone);
    printf("Address        : %s\n", client.address);
    printf("Account Type   : %s\n", client.accountType);
    printf("Balance        : %.2lf\n", client.balance);
}

void updateAccount(FILE *fPtr)
{
    struct clientData client;

    int account;

    printf("Enter Account Number: ");
    scanf("%d", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account Does Not Exist\n");
        return;
    }

    printf("Enter New Phone Number: ");
    scanf("%14s", client.phone);

    clearInputBuffer();

    printf("Enter New Address: ");
    fgets(client.address, sizeof(client.address), stdin);
    client.address[strcspn(client.address, "\n")] = '\0';

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Account Updated Successfully\n");

    transactionHistory("Account Updated");
}

void deleteAccount(FILE *fPtr)
{
    struct clientData client;
    struct clientData blank = {0};

    int account;

    printf("Enter Account Number: ");
    scanf("%d", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account Not Found\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fwrite(&blank, sizeof(struct clientData), 1, fPtr);

    printf("Account Deleted Successfully\n");

    transactionHistory("Account Deleted");
}

void depositMoney(FILE *fPtr)
{
    struct clientData client;

    int account;
    double amount;

    printf("Enter Account Number: ");
    scanf("%d", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account Not Found\n");
        return;
    }

    printf("Enter Deposit Amount: ");
    scanf("%lf", &amount);

    if (amount <= 0)
    {
        printf("Invalid Amount\n");
        return;
    }

    client.balance += amount;

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Deposit Successful\n");

    transactionHistory("Money Deposited");
}

void withdrawMoney(FILE *fPtr)
{
    struct clientData client;

    int account;
    double amount;

    printf("Enter Account Number: ");
    scanf("%d", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account Not Found\n");
        return;
    }

    printf("Enter Withdraw Amount: ");
    scanf("%lf", &amount);

    if (amount > client.balance)
    {
        printf("Insufficient Balance\n");
        return;
    }

    client.balance -= amount;

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Withdrawal Successful\n");

    transactionHistory("Money Withdrawn");
}

void transferMoney(FILE *fPtr)
{
    struct clientData sender;
    struct clientData receiver;

    int senderAcc;
    int receiverAcc;

    double amount;

    printf("Enter Sender Account Number: ");
    scanf("%d", &senderAcc);

    printf("Enter Receiver Account Number: ");
    scanf("%d", &receiverAcc);

    printf("Enter Amount: ");
    scanf("%lf", &amount);

    fseek(fPtr, (senderAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&sender, sizeof(struct clientData), 1, fPtr);

    fseek(fPtr, (receiverAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&receiver, sizeof(struct clientData), 1, fPtr);

    if (sender.acctNum == 0 || receiver.acctNum == 0)
    {
        printf("Invalid Accounts\n");
        return;
    }

    if (sender.balance < amount)
    {
        printf("Insufficient Balance\n");
        return;
    }

    sender.balance -= amount;
    receiver.balance += amount;

    fseek(fPtr, (senderAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&sender, sizeof(struct clientData), 1, fPtr);

    fseek(fPtr, (receiverAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&receiver, sizeof(struct clientData), 1, fPtr);

    printf("Transfer Successful\n");

    transactionHistory("Money Transferred");
}

void calculateInterest(FILE *fPtr)
{
    struct clientData client;

    int account;
    double rate;
    double interest;

    printf("Enter Account Number: ");
    scanf("%d", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account Not Found\n");
        return;
    }

    printf("Enter Interest Rate: ");
    scanf("%lf", &rate);

    interest = (client.balance * rate) / 100;

    printf("Interest Amount: %.2lf\n", interest);

    client.balance += interest;

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Interest Added Successfully\n");

    transactionHistory("Interest Added");
}

void exportTextFile(FILE *fPtr)
{
    FILE *txtPtr;

    struct clientData client;

    txtPtr = fopen("accounts.txt", "w");

    if (txtPtr == NULL)
    {
        printf("Unable To Create Text File\n");
        return;
    }

    rewind(fPtr);

    fprintf(txtPtr,
            "%-6s %-15s %-15s %-10s %-15s %-10s\n",
            "ID",
            "First",
            "Last",
            "Gender",
            "Phone",
            "Balance");

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            fprintf(txtPtr,
                    "%-6d %-15s %-15s %-10s %-15s %.2lf\n",
                    client.acctNum,
                    client.firstName,
                    client.lastName,
                    client.gender,
                    client.phone,
                    client.balance);
        }
    }

    fclose(txtPtr);

    printf("Accounts Exported Successfully\n");
}

void accountStatistics(FILE *fPtr)
{
    struct clientData client;

    int totalAccounts = 0;
    double totalBalance = 0;
    double highest = 0;

    rewind(fPtr);

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            totalAccounts++;
            totalBalance += client.balance;

            if (client.balance > highest)
            {
                highest = client.balance;
            }
        }
    }

    printf("\n========== STATISTICS ==========\n");
    printf("Total Accounts : %d\n", totalAccounts);
    printf("Total Balance  : %.2lf\n", totalBalance);
    printf("Highest Balance: %.2lf\n", highest);
}

void sortAccounts(FILE *fPtr)
{
    struct clientData clients[MAX];

    int count = 0;

    rewind(fPtr);

    while (fread(&clients[count], sizeof(struct clientData), 1, fPtr))
    {
        if (clients[count].acctNum != 0)
        {
            count++;
        }
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (clients[i].balance < clients[j].balance)
            {
                struct clientData temp = clients[i];
                clients[i] = clients[j];
                clients[j] = temp;
            }
        }
    }

    printf("\nAccounts Sorted By Balance\n");

    for (int i = 0; i < count; i++)
    {
        printf("%d %s %.2lf\n",
               clients[i].acctNum,
               clients[i].firstName,
               clients[i].balance);
    }
}

void changePassword(FILE *fPtr)
{
    struct clientData client;

    int account;
    char oldPass[20];

    printf("Enter Account Number: ");
    scanf("%d", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account Not Found\n");
        return;
    }

    printf("Enter Old Password: ");
    scanf("%19s", oldPass);

    if (strcmp(oldPass, client.password) != 0)
    {
        printf("Incorrect Password\n");
        return;
    }

    printf("Enter New Password: ");
    scanf("%19s", client.password);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Password Changed Successfully\n");
}

void miniStatement(FILE *fPtr)
{
    struct clientData client;

    int account;

    printf("Enter Account Number: ");
    scanf("%d", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account Not Found\n");
        return;
    }

    printf("\n========== MINI STATEMENT ==========\n");
    printf("Account Number : %d\n", client.acctNum);
    printf("Customer Name  : %s %s\n",
           client.firstName,
           client.lastName);
    printf("Account Type   : %s\n", client.accountType);
    printf("Current Balance: %.2lf\n", client.balance);
}

void lowBalanceAccounts(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\nLow Balance Accounts (<1000)\n");

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if (client.acctNum != 0 && client.balance < 1000)
        {
            printf("%d %s %.2lf\n",
                   client.acctNum,
                   client.firstName,
                   client.balance);
        }
    }
}

void accountTypeSummary(FILE *fPtr)
{
    struct clientData client;

    int savings = 0;
    int current = 0;

    rewind(fPtr);

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            if (strcmp(client.accountType, "Savings") == 0)
            {
                savings++;
            }
            else
            {
                current++;
            }
        }
    }

    printf("\nSavings Accounts : %d\n", savings);
    printf("Current Accounts : %d\n", current);
}

void transactionHistory(char message[])
{
    FILE *history;

    time_t now;
    time(&now);

    history = fopen("history.txt", "a");

    if (history == NULL)
    {
        return;
    }

    fprintf(history,
            "%s : %s",
            message,
            ctime(&now));

    fclose(history);
}

void clearInputBuffer()
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

void pauseScreen()
{
    printf("\nPress Enter To Continue...");
    clearInputBuffer();
    getchar();
}

