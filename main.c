#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ACCOUNT_FILE "account.dat"
#define LOG_FILE "transactions.log"

typedef struct {
    char name[50];
    int acc_no;
    float balance;
    int pin;
} Account;

void log_transaction(const char *message) {
    FILE *logfile = fopen(LOG_FILE, "a");
    if (logfile == NULL) return;

    time_t now = time(NULL);
    fprintf(logfile, "[%s] %s\n", ctime(&now), message);
    fclose(logfile);
}

void create_account() {
    FILE *file = fopen(ACCOUNT_FILE, "rb+");
    if (file == NULL) {
        // If file doesn’t exist, create it
        file = fopen(ACCOUNT_FILE, "wb+");
        if (file == NULL) {
            printf("\nUnable to create account file!!\n");
            return;
        }
    }

    Account acc;
    printf("Enter your name: ");
    getchar(); // clear newline
    fgets(acc.name, sizeof(acc.name), stdin);
    acc.name[strcspn(acc.name, "\n")] = '\0'; // remove newline

    printf("Enter account number: ");
    scanf("%d", &acc.acc_no);

    // Before writing new account, check for duplicate
    rewind(file);
    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no == acc.acc_no) {
            printf("Account number already exists!\n");
            fclose(file);
            return;
        }
    }

    printf("Enter initial balance: ");
    scanf("%f", &acc.balance);

    printf("Set 4-digit PIN: ");
    scanf("%d", &acc.pin);

    fwrite(&acc, sizeof(acc), 1, file);
    fclose(file);

    char msg[200];
    sprintf(msg, "Account created: %s (%d)", acc.name, acc.acc_no);
    log_transaction(msg);

    printf("\nAccount created successfully!\n");
}

void deposit_money() {
    FILE *file = fopen(ACCOUNT_FILE, "rb+");
    if (file == NULL) {
        printf("\nUnable to open file!!\n");
        return;
    }

    int acc_no, pin;
    float amount;
    Account acc;
    printf("Enter account number: ");
    scanf("%d", &acc_no);
    printf("Enter PIN: ");
    scanf("%d", &pin);
    printf("Enter deposit amount: ");
    scanf("%f", &amount);

    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no == acc_no && acc.pin == pin) {
            acc.balance += amount;
            fseek(file, -sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, file);
            fclose(file);

            char msg[200];
            sprintf(msg, "Deposited Rs.%.2f into %d (Balance: Rs.%.2f)", amount, acc_no, acc.balance);
            log_transaction(msg);

            printf("\nDeposit successful. Balance: Rs.%.2f\n", acc.balance);
            return;
        }
    }
    printf("\nAccount not found or invalid PIN!\n");
    fclose(file);
}

void withdraw_money() {
    FILE *file = fopen(ACCOUNT_FILE, "rb+");
    if (file == NULL) {
        printf("\nUnable to open the account file!\n");
        return;
    }

    int acc_no, pin;
    float money;
    Account acc;
    printf("Enter your account number: ");
    scanf("%d", &acc_no);
    printf("Enter PIN: ");
    scanf("%d", &pin);
    printf("Enter the amount you wish to withdraw: ");
    scanf("%f", &money);

    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no == acc_no && acc.pin == pin) {
            if (acc.balance >= money) {
                acc.balance -= money;
                fseek(file, -sizeof(acc), SEEK_CUR);
                fwrite(&acc, sizeof(acc), 1, file);
                fclose(file);

                char msg[200];
                sprintf(msg, "Withdrawn Rs.%.2f from %d (Balance: Rs.%.2f)", money, acc_no, acc.balance);
                log_transaction(msg);

                printf("\nWithdraw successful. Balance: Rs.%.2f\n", acc.balance);
            } else {
                printf("\nInsufficient balance!\n");
            }
            return;
        }
    }
    printf("\nAccount not found or invalid PIN!\n");
    fclose(file);
}

void check_balance() {
    FILE *file = fopen(ACCOUNT_FILE, "rb");
    if (file == NULL) {
        printf("\nUnable to open file!\n");
        return;
    }

    int acc_no, pin;
    Account acc;
    printf("Enter account number: ");
    scanf("%d", &acc_no);
    printf("Enter PIN: ");
    scanf("%d", &pin);

    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no == acc_no && acc.pin == pin) {
            printf("\nAccount: %s | Balance: Rs.%.2f\n", acc.name, acc.balance);
            fclose(file);
            return;
        }
    }
    printf("\nAccount not found or invalid PIN!\n");
    fclose(file);
}

void list_accounts() {
    FILE *file = fopen(ACCOUNT_FILE, "rb");
    if (file == NULL) {
        printf("\nUnable to open file!\n");
        return;
    }

    Account acc;
    int found = 0;

    printf("\n--- Account List ---\n");

    while (fread(&acc, sizeof(acc), 1, file) == 1) {
        printf("Name: %s | AccNo: %d | Balance: Rs.%.2f\n",
               acc.name, acc.acc_no, acc.balance);
        found = 1;
    }

    if (!found) {
        printf("No accounts found.\n");
    }

    fclose(file);
}




void export_accounts_csv() {
    FILE *file = fopen(ACCOUNT_FILE, "rb");
    FILE *csv = fopen("accounts.csv", "w");
    if (file == NULL || csv == NULL) {
        printf("\nError opening files!\n");
        return;
    }

    fprintf(csv, "Name,Account Number,Balance\n");
    Account acc;
    while (fread(&acc, sizeof(acc), 1, file)) {
        fprintf(csv, "%s,%d,%.2f\n", acc.name, acc.acc_no, acc.balance);
    }
    fclose(file);
    fclose(csv);

    log_transaction("Exported accounts to CSV");
    printf("\nAccounts exported to accounts.csv\n");
}
void delete_account() {
    FILE *file = fopen(ACCOUNT_FILE, "rb");
    if (file == NULL) {
        printf("\nUnable to open account file!!!\n");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");
    if (temp == NULL) {
        printf("\nUnable to create temporary file!!!\n");
        fclose(file);
        return;
    }

    int acc_no;
    Account acc_r;
    int found = 0;

    printf("Enter account number to delete: ");
    scanf("%d", &acc_no);

    while (fread(&acc_r, sizeof(acc_r), 1, file) == 1) {
        if (acc_r.acc_no == acc_no) {
            found = 1;
            printf("\nAccount No:%d belonging to %s has been deleted.\n", acc_r.acc_no, acc_r.name);
            // Skip writing this record (effectively deleting)
        } else {
            fwrite(&acc_r, sizeof(acc_r), 1, temp);
        }
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove(ACCOUNT_FILE);             // delete original file
        rename("temp.dat", ACCOUNT_FILE); // rename temp file to original
    } else {
        remove("temp.dat"); // cleanup
        printf("\nAccount No:%d was not found.\n", acc_no);
    }
}
int main() {
    int choice;
    while (1) {
        printf("\n--- Banking System Menu ---\n");
        printf("1. Create Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Check Balance\n");
        printf("5. List All Accounts\n");
        printf("6. Export Accounts (CSV)\n");
        printf("7. Delete Account\n");
        printf("8. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: create_account(); break;
            case 2: deposit_money(); break;
            case 3: withdraw_money(); break;
            case 4: check_balance(); break;
            case 5: list_accounts(); break;
            case 6: export_accounts_csv(); break;
            case 7: delete_account(); break;
            case 8: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
}
