#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int regID;
    char name[20];
    char surname[20];
    char status[20];
    int year;
    char macAddress[20];
}Registration;


Registration* Load_RegistrationData(char *filePath, int *recordSize);
void Display_RegistrationData(Registration *regList, int size);
void Add_Registration(Registration **regList, int *recordSize);
void Sort(Registration **regList, int size);
void Statistics(Registration *regList, int size);
void Menu();

int main(int argc, char *argv[]) {

    FILE *fileRead = NULL;
    char filePath[100];

    if (argc == 2) {
        strcpy(filePath, argv[1]);
        fileRead = fopen(filePath, "r");
    }

    while (fileRead == NULL) {
        printf("This file does not exist. Please enter again: ");
        fgets(filePath, 100, stdin);

        fileRead = fopen(filePath, "r");
        if (fileRead != NULL) {
            fclose(fileRead);
        }
    }
    printf("The registration records file %s has been successfully loaded\n",filePath);
    printf("The following records have been loaded:\n");

    int recordSize = 0;
    Registration *RegistrationData = Load_RegistrationData(filePath, &recordSize);

    Display_RegistrationData(RegistrationData, recordSize);


    int isOpen = 1;
    //Main loop
    while (isOpen)
    {
        int validInput = 0;
        char input;
        while (!validInput) {
            Menu();
            printf("Enter your choice: ");
            scanf(" %c", &input);

            validInput = input == '1' || input == '2' || input == '3' || input == '4';
            if (!validInput)
                printf("Wrong input!\n");


        }

        //operations
        switch (input) {
            case '1':
                //Add
                Add_Registration(&RegistrationData, &recordSize);
                break;
            case '2':
                //Sort
                Sort(&RegistrationData, recordSize);
                Display_RegistrationData(RegistrationData, recordSize);
                break;
            case '3':
                //statistic
                Statistics(RegistrationData, recordSize);
                break;
            case '4':
                //exit
                isOpen = 0;
                break;
            default:
                printf("Invalid input!\n");
        }
    }


    //Cleanup
    free(RegistrationData);
    fclose(fileRead);
    return 0;
}

Registration* Load_RegistrationData(char *filePath, int *recordSize) {
    Registration *RegistrationData = NULL;

    FILE *file = fopen(filePath, "r");

    int regID;
    char name[20];
    char surname[20];
    char status[20];
    int year;
    char macAddress[20];

    //Skip the first line
    int ch = 0;
    while ((ch = fgetc(file)) != '\n' && ch != EOF)
        ;


    int i = 0;
    while (fscanf(file, "%d %s %s %s %d %s\n",&regID, name, surname, status, &year, macAddress ) != EOF) {

        Registration *temp = realloc(RegistrationData,(i + 1) * sizeof(Registration));
        if (temp == NULL) {
            printf("Memory error\n");
            exit(1);
        }
        RegistrationData = temp;
        RegistrationData[i].regID = regID;
        RegistrationData[i].year = year;
        strcpy(RegistrationData[i].name, name);
        strcpy(RegistrationData[i].surname, surname);
        strcpy(RegistrationData[i].status, status);
        strcpy(RegistrationData[i].macAddress, macAddress);


        i++;
    }
    *recordSize = i;

    return RegistrationData;
}

void Display_RegistrationData(Registration *regList, int size) {

    if (regList == NULL) {
        printf("Data couldn't be loaded\n");
        return;
    }

    printf("RegID    NAME       SURNAME      STATUS     YEAR    DeviceMACAddress\n");

    for (int i = 0; i < size; i++) {
        Registration reg = regList[i];
        printf("%d\t %-10s %-12s %-10s %d    %-15s\n",
            reg.regID, reg.name, reg.surname, reg.status, reg.year, reg.macAddress );
    }
}

void Add_Registration(Registration **regList, int *recordSize) {
    Registration newReg;

    printf("Please enter the name: ");
    scanf("%s", newReg.name);

    printf("Please enter the surname: ");
    scanf("%s", newReg.surname);


    int correctStatus = 0;
    do {
        printf("Please enter the status: ");
        scanf("%s", newReg.status);

        correctStatus = strcmp(newReg.status,"Approved") == 0 || strcmp(newReg.status,"Blocked") == 0 ||
            strcmp(newReg.status,"Declined") == 0;

        if (!correctStatus)
            printf("This is not valid!\n");

    } while (!correctStatus);


    printf("Please enter the year: ");
    scanf("%d", &newReg.year);

    int correctMac = 0;
    do {
        printf("Please enter the mac address: ");
        scanf("%17s", newReg.macAddress);
        printf("Read: %s\n", newReg.macAddress);
        unsigned int a, b, c, d, e, f;
        char extra;

        int count = sscanf(newReg.macAddress, "%2x:%2x:%2x:%2x:%2x:%2x%c\n",
                           &a, &b, &c, &d, &e, &f, &extra);
        printf("%d\n",count);
        correctMac = (count == 6);

        if (!correctMac) {
            printf("Mac address is invalid!\n");
        }

    } while (!correctMac);

    if (*recordSize > 0) {

        //Find the highest Registration ID
        int highestID = (*regList)[0].regID;
        for (int i = 0; i < *recordSize; ++i) {
            if ((*regList)[i].regID > highestID) {
                highestID = (*regList)[i].regID;
            }
        }

        newReg.regID = highestID + 1;
    }
    else {
        newReg.regID = 1;
    }



    Registration *temp = realloc(*regList, (*recordSize + 1) * sizeof(Registration));
    if (temp == NULL) {
        printf("Memory error\n");
        exit(1);
    }
    *regList = temp;
    (*regList)[*recordSize] = newReg;
    (*recordSize)++;

    printf("\nIt is recorded successfully!\n");
}

void Sort(Registration **regList, int size) {

    char input;
    int validInput = 0;
    while (!validInput) {
        printf("Which column (1: year, 2: surname)?: ");
        scanf(" %c",&input);
        validInput = input == '1' || input == '2';
        if (!validInput)
            printf("Invalid input!\n");
    }


    switch (input) {
        case '1':
            //Sort by year
            for (int i = 0; i < size - 1; ++i) {
                for (int j = 0; j < size - 1 - i ; ++j) {

                    if ((*regList)[j].year > (*regList)[j + 1].year) {
                        Registration temp = (*regList)[j];
                        (*regList)[j] = (*regList)[j + 1];
                        (*regList)[j + 1] = temp;
                    }
                }
            }
            break;

        case '2':
            //Sort by surname
            for (int i = 0; i < size - 1; ++i) {
                for (int j = 0; j < size - 1 - i ; ++j) {

                    //maybe add case insensitivity
                    if (strcmp((*regList)[j].surname , (*regList)[j + 1].surname) > 0) {
                        Registration temp = (*regList)[j];
                        (*regList)[j] = (*regList)[j + 1];
                        (*regList)[j + 1] = temp;
                    }
                }
            }
            break;
        default:
            printf("Invalid input!\n");
    }

}

void Statistics(Registration *regList, int size) {
    int approved = 0;
    int blocked = 0;
    int declined = 0;

    for (int i = 0; i < size; i++) {
        char status[20];
        strcpy(status, regList[i].status);
        if (strcmp(status,"Approved") == 0)
        {
            approved++;
        }
        else if (strcmp(status,"Blocked") == 0) {
            blocked++;
        }
        else if (strcmp(status,"Declined") == 0) {
            declined++;
        }
    }
    printf("Summary:\n");
    printf("%d Blocked\n", blocked);
    printf("%d Approved \n", approved);
    printf("%d Declined \n", declined);
}

void Menu() {
    printf("What would you like to do?\n");
    printf("1 - add\n");
    printf("2 - sort\n");
    printf("3 - statistics\n");
    printf("4 - exit\n");
}