/*
Ex #0a: Merge two files
This program will take read 3 files from the user by the following order :
first file - <input file1>, second file - <input file2>, third file -
<output file>.the program than will merge the two input files and print the
names with the grades given in both files in alphabetic order
into the given output file.
Written by: Rotem Kashani, Mahmood Jazmawy
        Id: 209073352, 211345277
Login: rotemkash, Mahmoodja

*/
//----------------------includes and using---------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

//---------------------Consts and enums------------------------------------

#define NAME_LEN 50
size_t LINE_LEN = 1000;

//---------------------Prototypes------------------------------------------

void openFiles(const char *argv[], FILE **file_1, FILE **file_2, FILE **out_file);

void readFromFile(FILE *file, char name[NAME_LEN], char prev_name[NAME_LEN], char **grades);

void printToFile(FILE *outFile, const char name[NAME_LEN], const char grades[NAME_LEN]);

//---------------------Main Function---------------------------------------

int main(int argc, const char *argv[]) {
    FILE *file_1, *file_2, *outFile;


    if (argc != 4) {
        fprintf(stderr, "Invalid arguments number entered\n");
        exit(EXIT_FAILURE);
    }
    openFiles(argv, &file_1, &file_2, &outFile);

    char name1[NAME_LEN] = {'\0'}, prevName1[NAME_LEN] = {'\0'},
            name2[NAME_LEN] = {'\0'}, prevName2[NAME_LEN] = {'\0'};

    char *grades_1 = NULL, *grades_2 = NULL;

    readFromFile(file_1, name1, prevName1, &grades_1);
    readFromFile(file_2, name2, prevName2, &grades_2);
    strcpy(prevName1, name1);
    strcpy(prevName2, name2);


    // go over the first file until we reach it's end
    while (!feof(file_1)) {
        // if the files are unsorted delete the output and exit
        if ((strcmp(name1, prevName1) < 0) || (strcmp(name2, prevName2) < 0)) {
            unlink(argv[3]);
            fprintf(stderr, "Error: File not sorted");
            return EXIT_FAILURE;
        }

        if (strcmp(name1, name2) == 0) { // if name1 == name2
            grades_1[strlen(grades_1) - 1] = '\0';
            printToFile(outFile, name1, grades_1);
            fprintf(outFile, "%s", grades_2);

            readFromFile(file_1, name1, prevName1, &grades_1);
            readFromFile(file_2, name2, prevName2, &grades_2);
            continue;

        } else if (strcmp(name1, name2) < 0) { // if name < name2
            printToFile(outFile, name1, grades_1);
            readFromFile(file_1, name1, prevName1, &grades_1);
            continue;

        } else { // else if name1 > name2
            // go over the second file until we reach a word
            //that's greater than the word in file_1
            while (strcmp(name1, name2) > 0) {
                printToFile(outFile, name2, grades_2);
                readFromFile(file_2, name2, prevName2, &grades_2);

                // if file_2 is unsorted delete the ouput and exit
                if ((strcmp(name2, prevName2) < 0)) {
                    unlink(argv[3]);
                    fprintf(stderr, "Error: File not sorted");
                    return EXIT_FAILURE;
                }
            }
        }
    }

    // print any remaining names in the second file
    while (!feof(file_2)) {
        if ((strcmp(name2, prevName2) < 0)) {
            unlink(argv[3]);
            fprintf(stderr, "Error: File not sorted");
            return EXIT_FAILURE;
        }

        printToFile(outFile, name2, grades_2);
        readFromFile(file_2, name2, prevName2, &grades_2);
    }
    return EXIT_SUCCESS;
}


/**
 * This function receives a file two strings,
 * then it prints those two strings into the file
 */
void printToFile(FILE *outFile, const char name[NAME_LEN], const char grades[NAME_LEN]) {
    fprintf(outFile, "%s", name);
    fprintf(outFile, "%s", grades);
}


/**
 * This function receives a file and 3 strings.
 * it copies the last name we had into prev_name.
 * it read a new name and the grades next to it
 */
void readFromFile(FILE *file, char name[NAME_LEN], char prev_name[NAME_LEN], char **grades) {
    strcpy(prev_name, name);
    fscanf(file, "%s", name);
    getline(&(*grades), &LINE_LEN, file);
}


/**
 * This function receives an array of files names and it tries to open them.
 */
void openFiles(const char *argv[], FILE **file_1, FILE **file_2, FILE **out_file) {

    if ((*file_1 = fopen(argv[1], "r")) == NULL) {
        fputs("Couldn't open the first file\n", stderr);
        exit(EXIT_FAILURE);
    }

    if ((*file_2 = fopen(argv[2], "r")) == NULL) {
        fputs("Couldn't open the second file\n", stderr);
        exit(EXIT_FAILURE);
    }

    if ((*out_file = fopen(argv[3], "a")) == NULL) {
        fputs("Couldn't open the third file\n", stderr);
        exit(EXIT_FAILURE);
    }
}
