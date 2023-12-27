#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INITIAL_SIZE 1

// base structure of a dynamic array
typedef struct {
    size_t size;
    size_t capacity;
    char** array;
}Dynamic_Array;

// function to initiate a dynamic array
void arrayMake(Dynamic_Array **arr_ptr) {
    Dynamic_Array *container;
    container = (Dynamic_Array *) malloc(sizeof(Dynamic_Array));
    container->size = 0;
    container->capacity = INITIAL_SIZE;
    container->array = (char** ) malloc(INITIAL_SIZE * sizeof(char*));
    *arr_ptr = container;
}

// function to insert a given item into a dynamic array
void insertItem(Dynamic_Array* container, char* item) {
    if (container->size == container->capacity) {
        container->capacity <<= 1;
        container->array = realloc(container->array, container->capacity * sizeof(char*));
    }
    container->array[container->size++] = item;
}

// function to delete a specific item from a dynamic array
void deleteItem(Dynamic_Array* container, char* item) {
    // first, find the index of the item to be deleted
    int index;
    for (int i = 0; i < container->size; i++) {
        if (strcmp(container->array[i], item)==0){
            index = i;
        }
    }
    // if it's the first element, free the array
    if (index == 0) {
        free(container->array);
    } else {
        // else, update it with the next item and reduce the size of the array by 1
        for (int i = index; i < container->size; i++) {
            container->array[i] = container->array[i + 1];
        }
    }
    container->size--;
}

// function to check the existence of item in a dynamic array
bool hasItem(Dynamic_Array* container, char* item) {
    for (int i = 0; i < container->size; i++) {
        if (strcmp(container->array[i], item) == 0){
            return true;
        }
    }
    return false;
}

// function to print out a dynamic array
void printArray(Dynamic_Array* container) {
    if (container->size > 1) {
        for (int i = 0; i < container->size - 1; i++) {
            printf("%s, ", container->array[i]);
        }
    }
    printf("%s\n", container->array[container->size - 1]);
}

// function to free the array and the memory
void freeArray(Dynamic_Array* container)
{
    free(container->array);
    free(container);
}


// MAIN PROGRAM
int main(int argc, char *argv[]) {

    // initiate keys
    Dynamic_Array *keys_input;
    arrayMake(&keys_input);
    for (int i = 2; i < argc; i++) {
        insertItem(keys_input, argv[i]);
    }
    char *fireKey = "FIREFIGHTER_SECRET_KEY";

    // initiate owner
    char *owner = argv[1];

    // initiate the successful key checker
    bool isInside = false;

    // initiate an empty array of house members
    Dynamic_Array *houseMembers;
    arrayMake(&houseMembers);
    // initiate an inserted key placeholder

    char *insertedKey;
    // initiate a current username placeholder
    char *attemptUser;
    // initiate a placeholder for user input
    char temp_input[100];

    // breakable while-loop for indefinite user inputs
    while (fgets(temp_input, 100, stdin) != NULL) {

        // if enter key is pressed, break
        if (strcmp(temp_input, "\n") == 0) {
            exit(1);
        }

        // tokenize/parse the inputted string and store the values in a dynamic array
        temp_input[strcspn(temp_input, "\n")] = 0;
        Dynamic_Array *splitLine;
        arrayMake(&splitLine);
        char *token = strtok(temp_input, " ");
        while (token != NULL) {
            insertItem(splitLine, token);
            token = strtok(NULL, " ");
        }

        // since the min length of acceptable inputs is 2, check
        if (splitLine->size > 1) {

            // if INSERT KEY <user_name> <key>
            if (strcmp(splitLine->array[0], "INSERT") == 0 && strcmp(splitLine->array[1], "KEY")==0 && splitLine->size == 4) {
                // store the duplicates of the input accordingly for the username and inserted key
                attemptUser = strdup(splitLine->array[2]);
                insertedKey = strdup(splitLine->array[3]);
                // constant output
                printf("KEY %s INSERTED BY %s\n", insertedKey, attemptUser);
            }

            // else if TURN KEY <user_name>
            else if (strcmp(splitLine->array[0], "TURN") == 0 && strcmp(splitLine->array[1], "KEY")==0 && splitLine->size == 3) {
                // update the username only
                attemptUser = strdup(splitLine->array[2]);
                // if the previously inserted key is correct, turn is possible
                if ((hasItem(keys_input, insertedKey) || strcmp(insertedKey, fireKey) == 0)) {
                    printf("SUCCESS %s TURNS KEY %s\n", attemptUser, insertedKey);
                    // now possible to enter
                    isInside = true;
                } else {
                    // otherwise unsuccessful
                    printf("FAILURE %s UNABLE TO TURN KEY %s\n", attemptUser, insertedKey);
                }
            }

            // else if ENTER HOUSE <user_name>
            else if (strcmp(splitLine->array[0], "ENTER") == 0 && strcmp(splitLine->array[1], "HOUSE")==0 && splitLine->size == 3) {
                // update the username only
                attemptUser = strdup(splitLine->array[2]);
                // if the key is already turned, access the house and update house members
                if (isInside){
                    // update the house members array
                    insertItem(houseMembers, attemptUser);
                    printf("ACCESS ALLOWED\n");
                } else {
                    // otherwise deny
                    printf("ACCESS DENIED\n");
                }
                // lock the key
                isInside = false;
            }

            // else if WHO'S INSIDE?
            else if (strcmp(splitLine->array[0], "WHO'S") == 0 && strcmp(splitLine->array[1], "INSIDE?")==0 && splitLine->size == 2) {
                // output the error message of the list of people inside
                if (houseMembers->size == 0) {
                    printf("NOBODY HOME\n");
                } else {
                    printArray(houseMembers);
                }
            }

            // else if CHANGE LOCKS <user_name> <key_1> <key_2> ... <key_n>
            else if (strcmp(splitLine->array[0], "CHANGE") == 0 && strcmp(splitLine->array[1], "LOCKS")==0 ) {
                attemptUser = strdup(splitLine->array[2]);
                // conduct a re-key operation is the username is the owner and if the owner is inside
                if (strcmp(attemptUser, owner) == 0 && hasItem(houseMembers, owner)){
                    // free and re-start an array of key inputs
                    freeArray(keys_input);
                    arrayMake(&keys_input);
                    // add new keys
                    for(int i = 3; i < splitLine->size; i++){
                        insertItem(keys_input, strdup(splitLine->array[i]));
                    }
                    printf("LOCK CHANGED\n");
                    // lock the key
                    isInside = false;
                } else {
                    printf("LOCK CHANGE DENIED\n");
                }
            }

            // else if LEAVE HOUSE <user_name>
            else if (strcmp(splitLine->array[0], "LEAVE") == 0 && strcmp(splitLine->array[1], "HOUSE")==0 && splitLine->size == 3) {
                // update the username only
                attemptUser = strdup(splitLine->array[2]);
                // if the username is inside the house, delete
                if (hasItem(houseMembers, attemptUser)){
                    deleteItem(houseMembers, attemptUser);
                    printf("%s LEFT\n", attemptUser);
                } else {
                    // otherwise print an error message
                    printf("%s NOT HERE\n", attemptUser);
                }
            }

            else{
                // if any other input is entered, print an error message
                printf("ERROR\n");
            }
        } else{
            // if any other input is entered, print an error message
            printf("ERROR\n");
        }
        // free the parsed array to re-initiate in in the next loop
        freeArray(splitLine);
    }
    return 0;
}
