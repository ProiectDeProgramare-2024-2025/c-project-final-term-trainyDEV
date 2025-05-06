#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CVS 100
#define LINE_BUFFER_SIZE 600

// macro should help with scanf_s implementation, DO NOT TOUCH
#define _countof(arr) (sizeof(arr) / sizeof((arr)[0]))


// CV details
typedef struct {
    char name[50];
    char surname[50];
    char email[100];
    char phoneNumber[20];
    char previousPositions[500];
} CV;

CV cvs[MAX_CVS];
int cvCount = 0;

void clearInputBuffer() {
    while (getchar() != '\n');
}

void clearScreen() { //screen clear for each option
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Validation Functions start
int isValidName(const char* name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (name[i] >= '0' && name[i] <= '9') return 0;
    }
    return 1;
}

int isValidPhoneNumber(const char* phone) {
    if (phone[0] == '+') {
        for (int i = 1; phone[i] != '\0'; i++) {
            if (phone[i] < '0' || phone[i] > '9') return 0;
        }
        return 1;
    }
    else {
        for (int i = 0; phone[i] != '\0'; i++) {
            if (phone[i] < '0' || phone[i] > '9') return 0;
        }
        return 1;
    }
}

int isValidEmail(const char* email) {
    const char* at = strchr(email, '@');
    if (!at || at == email) return 0;

    const char* dot = strrchr(email, '.');
    if (!dot || dot < at || dot == email + strlen(email) - 1) return 0;

    return 1;
}

int isCancelCommand(const char* input) {
    return (strcmp(input, "cancel") == 0 || strcmp(input, "exit") == 0);
}
// Validation Functions end
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


// Read CVs from file
void loadCVsFromFile(const char* filename) {
    FILE* file = NULL;
    errno_t err = fopen_s(&file, filename, "r");
    if (err != 0 || file == NULL) {
        printf("\x1b[31mWarning: Could not open file '%s'. Starting with empty CV list.\x1b[0m\n", filename);
        return;
    }

    char line[LINE_BUFFER_SIZE];
    while (fgets(line, sizeof(line), file) && cvCount < MAX_CVS) {
        line[strcspn(line, "\n")] = '\0';

        char* token = NULL;
        char* context = NULL;
        CV cv;

        token = strtok_s(line, "|", &context);
        if (!token) continue;
        strncpy_s(cv.name, sizeof(cv.name), token, _TRUNCATE);

        token = strtok_s(NULL, "|", &context);
        if (!token) continue;
        strncpy_s(cv.surname, sizeof(cv.surname), token, _TRUNCATE);

        token = strtok_s(NULL, "|", &context);
        if (!token) continue;
        strncpy_s(cv.email, sizeof(cv.email), token, _TRUNCATE);

        token = strtok_s(NULL, "|", &context);
        if (!token) continue;
        strncpy_s(cv.phoneNumber, sizeof(cv.phoneNumber), token, _TRUNCATE);

        token = strtok_s(NULL, "|", &context);
        if (!token) continue;
        strncpy_s(cv.previousPositions, sizeof(cv.previousPositions), token, _TRUNCATE);

        cvs[cvCount++] = cv;
    }

    fclose(file);
}

// Save CVs to file
void saveCVsToFile(const char* filename) {
    FILE* file = NULL;
    errno_t err = fopen_s(&file, filename, "w");
    if (err != 0 || file == NULL) {
        printf("\x1b[31mError: Could not open file '%s' for writing.\x1b[0m\n", filename);
        return;
    }

    for (int i = 0; i < cvCount; i++) {
        fprintf(file, "%s|%s|%s|%s|%s\n",
            cvs[i].name,
            cvs[i].surname,
            cvs[i].email,
            cvs[i].phoneNumber,
            cvs[i].previousPositions);
    }

    fclose(file);
}

// CV Creation
void createCV() {
    clearScreen();
    printf("\x1b[36m=== Create New CV ===\x1b[0m\n\n");

    if (cvCount >= MAX_CVS) {
        printf("\x1b[31mCV limit reached!\x1b[0m\n");
        return;
    }

    CV newCV;
    char inputBuffer[LINE_BUFFER_SIZE];

    // Name Check
    do {
        printf("Enter name (no digits allowed) or type 'cancel' to exit: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        inputBuffer[strcspn(inputBuffer, "\n")] = '\0';

        if (isCancelCommand(inputBuffer)) return;

        strncpy_s(newCV.name, sizeof(newCV.name), inputBuffer, _TRUNCATE);
        if (!isValidName(newCV.name)) {
            printf("\x1b[31mName must not contain digits. Try again.\x1b[0m\n");
        }
    } while (!isValidName(newCV.name));

    // Surname Check
    do {
        printf("Enter surname (no digits allowed) or type 'cancel' to exit: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        inputBuffer[strcspn(inputBuffer, "\n")] = '\0';

        if (isCancelCommand(inputBuffer)) return;

        strncpy_s(newCV.surname, sizeof(newCV.surname), inputBuffer, _TRUNCATE);
        if (!isValidName(newCV.surname)) {
            printf("\x1b[31mSurname must not contain digits. Try again.\x1b[0m\n");
        }
    } while (!isValidName(newCV.surname));

    // Email Check
    do {
        printf("Enter email address (e.g. name@example.com) or type 'cancel' to exit: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        inputBuffer[strcspn(inputBuffer, "\n")] = '\0';

        if (isCancelCommand(inputBuffer)) return;

        strncpy_s(newCV.email, sizeof(newCV.email), inputBuffer, _TRUNCATE);
        if (!isValidEmail(newCV.email)) {
            printf("\x1b[31mInvalid email format. Try again.\x1b[0m\n");
        }
    } while (!isValidEmail(newCV.email));

    // Phone Number Check
    do {
        printf("Enter phone number (digits only or starting with '+') or type 'cancel' to exit: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        inputBuffer[strcspn(inputBuffer, "\n")] = '\0';

        if (isCancelCommand(inputBuffer)) return;

        strncpy_s(newCV.phoneNumber, sizeof(newCV.phoneNumber), inputBuffer, _TRUNCATE);
        if (!isValidPhoneNumber(newCV.phoneNumber)) {
            printf("\x1b[31mInvalid phone number. Use digits only, or start with a single '+'. Try again.\x1b[0m\n");
        }
    } while (!isValidPhoneNumber(newCV.phoneNumber));

    // Previous Positions Check
    printf("Enter previous positions (free text, max 500 characters) or type 'cancel' to exit:\n");
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    inputBuffer[strcspn(inputBuffer, "\n")] = '\0';

    if (isCancelCommand(inputBuffer)) return;

    strncpy_s(newCV.previousPositions, sizeof(newCV.previousPositions), inputBuffer, _TRUNCATE);

    // Save CV
    cvs[cvCount++] = newCV;
    saveCVsToFile("cv_data.txt");

    printf("\n\x1b[32mCV created successfully!\x1b[0m\n");
    printf("\nPress Enter to return to main menu...");
    getchar();
}

// View CVs function
void viewCVs() {
    clearScreen();
    printf("\x1b[36m=== View All CVs ===\x1b[0m\n\n");

    if (cvCount == 0) {
        printf("\x1b[31mNo CVs available.\x1b[0m\n");
    }
    else {
        for (int i = 0; i < cvCount; i++) {
            printf("CV %d:\n", i + 1);
            printf("Name: \x1b[32m%s %s\x1b[0m, Email: \x1b[34m%s\x1b[0m, Phone: \x1b[33m%s\x1b[0m\n",
                cvs[i].name, cvs[i].surname, cvs[i].email, cvs[i].phoneNumber);
            printf("Previous Positions: %s\n\n", cvs[i].previousPositions);
        }
    }

    printf("Press Enter to return to main menu...");
    getchar();
    getchar();
}

// Search function
void searchCVs() {
    clearScreen();
    printf("\x1b[36m=== Search CVs by Keyword ===\x1b[0m\n\n");

    if (cvCount == 0) {
        printf("\x1b[31mNo CVs available to search.\x1b[0m\n");
        printf("\nPress Enter to return to main menu...");
        getchar();
        getchar();
        return;
    }

    char keyword[100];
    printf("Enter keyword to search in previous positions: ");
    scanf_s("%s", keyword, (unsigned)_countof(keyword));

    int found = 0;
    for (int i = 0; i < cvCount; i++) {
        if (strstr(cvs[i].previousPositions, keyword) != NULL) {
            printf("Match in CV %d:\n", i + 1);
            printf("Name: \x1b[32m%s %s\x1b[0m, Email: \x1b[34m%s\x1b[0m, Phone: \x1b[33m%s\x1b[0m\n",
                cvs[i].name, cvs[i].surname, cvs[i].email, cvs[i].phoneNumber);
            printf("Previous Positions: %s\n\n", cvs[i].previousPositions);
            found = 1;
        }
    }

    if (!found) {
        printf("\x1b[31mNo CVs matched the keyword.\x1b[0m\n");
    }

    printf("\nPress Enter to return to main menu...");
    getchar();
    getchar();
}

// CV Deletion
void deleteCV() {
    clearScreen();
    printf("\x1b[36m=== Delete a CV ===\x1b[0m\n\n");

    if (cvCount == 0) {
        printf("\x1b[31mNo CVs available to delete.\x1b[0m\n");
        printf("\nPress Enter to return to main menu...");
        getchar();
        getchar();
        return;
    }

    for (int i = 0; i < cvCount; i++) {
        printf("%d. \x1b[32m%s %s\x1b[0m\n", i + 1, cvs[i].name, cvs[i].surname);
    }

    int index;
    printf("\nEnter the number of the CV to delete: ");
    scanf_s("%d", &index);

    if (index < 1 || index > cvCount) {
        printf("\x1b[31mInvalid selection.\x1b[0m\n");
        printf("\nPress Enter to return to main menu...");
        getchar();
        getchar();
        return;
    }

    printf("Are you sure you want to delete CV %d (\x1b[31m%s %s\x1b[0m)? (y/n): ", index, cvs[index - 1].name, cvs[index - 1].surname);

    getchar();
    char confirm = getchar();
    if (confirm != 'y' && confirm != 'Y') {
        printf("Deletion cancelled.\n");
        printf("\nPress Enter to return to main menu...");
        getchar();
        return;
    }

    // Shift all CVs up
    for (int i = index - 1; i < cvCount - 1; i++) {
        cvs[i] = cvs[i + 1];
    }
    cvCount--;

    saveCVsToFile("cv_data.txt");
    printf("\x1b[32mCV deleted successfully.\x1b[0m\n");

    printf("\nPress Enter to return to main menu...");
    getchar();
}

// Menu
int main() {
    loadCVsFromFile("cv_data.txt");  // Load saved CVs

    int choice;
    do {
        clearScreen();
        printf("\x1b[36m=== Job Social Network System ===\x1b[0m\n");
        printf("\x1b[32m1. Create\x1b[0m CV\n");
        printf("\x1b[34m2. View\x1b[0m CVs\n");
        printf("\x1b[33m3. Search\x1b[0m CVs\n");
        printf("\x1b[31m4. Delete\x1b[0m CV\n");
        printf("\x1b[90m5. Exit\x1b[0m\n");
        printf("Enter your choice: ");
        scanf_s("%d", &choice);
        clearInputBuffer(); // clear the input buffer after scanf_s

        switch (choice) {
        case 1: createCV(); break;
        case 2: viewCVs(); break;
        case 3: searchCVs(); break;
        case 4: deleteCV(); break;
        case 5: printf("\x1b[32mExiting...\x1b[0m\n"); break;
        default:
            printf("\x1b[31mInvalid choice, try again.\x1b[0m\n");
            printf("Press Enter to continue...");
            getchar();
            getchar();
        }

    } while (choice != 5);

    return 0;
}
