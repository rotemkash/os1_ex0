/*
Ex #0b: Split sentences to words:
The program receives the name of a file using the arguments vector.
The file will include a natural number indicating how many sentences 
the file includes, and then the series of sentences and dividing the sentences 
into words with each word in a line alone
Written by: Rotem Kashani, Mahmood Jazmawy
Id: 209073352, 211345277
Login: rotemkash, Mahmoodja
*/

//----------------------includes and using---------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//---------------------Consts and enums------------------------------------
#define MAX_STR_LEN 1000 //max sentence length

size_t MAX_LINE_LEN = 1000;

//--------------------STRUCTS-------------------------------
struct Sentences {
    char **_data;
    int _num_of_sentences;
};

//---------------------Prototypes------------------------------------------

int readData(struct Sentences *split, FILE *inFile);

struct Sentences *splitToWords(struct Sentences *sentArray, int numOfWords);

void printToFile(struct Sentences *wordsArr, FILE *outFile);

void freeStruct(struct Sentences *structToFree);

//---------------------Main Function---------------------------------------

int main(int argc, const char *argv[]) {
    struct Sentences *sentencesArray = (struct Sentences *) malloc(sizeof(struct Sentences));
    // TODO: check alloc and exit if failed no need to free anything
    int numOfWords = 0;

    FILE *inFile, *outFile;
    if (argc != 3) {
        fprintf(stderr, "Usage:%s<inp file> <out file>\n", argv[0]);
        //TODO: free prev struct
        exit(EXIT_FAILURE);
    }

    inFile = fopen(argv[1], "r");
    outFile = fopen(argv[2], "a");

    if (inFile == NULL || outFile == NULL) {
        fputs("Cannot open files\n", stderr);
        //TODO: free prev struct
        exit(EXIT_FAILURE);
    }

    // rad the file into and array of sentences
    numOfWords = readData(sentencesArray, inFile);


    // create a new struct with each word in it's own row
    struct Sentences *newStruct = splitToWords(sentencesArray, numOfWords);

    //print the words into a file
    printToFile(newStruct, outFile);

    // free the data structures
    freeStruct(sentencesArray);
    freeStruct(newStruct);
    free(sentencesArray);
    free(newStruct);

    // close the files
    fclose(inFile);
    fclose(outFile);
    return EXIT_SUCCESS;
}

//======================IMPLEMENTATION==================================
/**
 * This function receives a Sentence struct and a file.
 * it reads the lines from the file and puts each line in a row
 * of the array in the sentence->data
 * then it returns the number of line in the file to use later
 */
int readData(struct Sentences *split, FILE *inFile) {
    int numOfSentences, numOfWords = 0;
    fscanf(inFile, "%d", &numOfSentences);

    split->_data = (char **) malloc(numOfSentences * sizeof(char *));
    if (!split->_data) {
        printf("cannot allocate memory\n");
        exit(EXIT_FAILURE);
    }

    split->_num_of_sentences = numOfSentences;
    char *currSentence = NULL;

    fgetc(inFile);// deletes the '\n'
    // converts the file into the data structure
    for (int i = 0; i < split->_num_of_sentences; i++) {
        getline(&currSentence, &MAX_LINE_LEN, inFile);
        (split->_data)[i] = (char *) malloc((strlen(currSentence)) * sizeof(char) + 1);
        if (!split->_data[i]) {
            printf("cannot allocate memory\n");
            freeStruct(split);
            free(split);
            exit(EXIT_FAILURE);
        }
        strcpy(split->_data[i], currSentence); //copy the string to the array
    }
    free(currSentence);

    // goes over all the line in the data structs
    for (int sentence = 0; sentence < (split->_num_of_sentences); sentence++) {
        if (split->_data[sentence] == NULL) { break; }
        // counts the words in the k-th line
        for (int word = 0; word < MAX_LINE_LEN; word++) {
            if ((isspace(split->_data[sentence][word]) != 0) || split->_data[sentence][word] == '\0') {
                numOfWords++; //end of word
            }
            if (split->_data[sentence][word] == '\n') { break; }
        }
    }
    return numOfWords;
}


/**
 * This function receives the struct that has each line in a row.
 * then is puts each word in it's own line in a new struct
 * and after it's done it returns the new struct with each word
 * in it's own line.
 */
struct Sentences *splitToWords(struct Sentences *sentArray, int numOfWords) {
    int tmpIndex;
    char tmpString[MAX_STR_LEN] = {'\0'};
    struct Sentences *wordsArray = (struct Sentences *) malloc(sizeof(struct Sentences));
    if (!wordsArray) { // check allocation
        printf("cannot allocate memory\n");
        freeStruct(sentArray);
        free(sentArray);
        exit(EXIT_FAILURE);
    }

    wordsArray->_data = (char **) malloc(numOfWords * sizeof(char *));
    if (!wordsArray->_data) { // check allocation of the data
        printf("cannot allocate memory\n");
        free(wordsArray);
        freeStruct(sentArray);
        free(sentArray);
        exit(EXIT_FAILURE);
    }

    wordsArray->_num_of_sentences = 0;
    // go over the lines in the old struct we created before
    for (int oldRow = 0; oldRow < sentArray->_num_of_sentences; ++oldRow) {
        tmpIndex = 0;
        // go over the letters in the current line
        for (int letter = 0; letter < MAX_STR_LEN; ++letter) {
            // if a space or the end of a string or a new-line was reached
            if (isspace(sentArray->_data[oldRow][letter]) ||
                (sentArray->_data[oldRow][letter] == '\0')) {

                wordsArray->_data[wordsArray->_num_of_sentences] = (char *) malloc(
                        ((strlen(tmpString) + 1) * sizeof(char)));
                // check allocation and free previous memory if failed
                if (!wordsArray->_data[wordsArray->_num_of_sentences]) {
                    printf("cannot allocate memory\n");
                    freeStruct(wordsArray);
                    free(wordsArray);
                    freeStruct(sentArray);
                    free(sentArray);
                    exit(EXIT_FAILURE);
                }
                //copy the word to it's own row
                strcpy(wordsArray->_data[wordsArray->_num_of_sentences++], tmpString);

                // reset the tmpString
                memset(tmpString, '\0', strlen(tmpString) + 1);
                tmpIndex = 0;
            } else { // else if we still didn't reach the end of a word
                tmpString[tmpIndex++] = sentArray->_data[oldRow][letter];
            }

            if (sentArray->_data[oldRow][letter] == '\n') { break; }
        }
    }
    return wordsArray;
}


/**
 * This function receives a struct and a file.
 * then it prints each row of the struct into a row in the file
 */
void printToFile(struct Sentences *wordsArr, FILE *outFile) {
    for (int word = 0; word < wordsArr->_num_of_sentences; ++word) {
        fprintf(outFile, "%s", wordsArr->_data[word]);
        fprintf(outFile, "\n");
    }
}


/**
 * This function receives a struct and frees the memory allocated for it
 */
void freeStruct(struct Sentences *structToFree) {
    for (int row = 0; row < structToFree->_num_of_sentences; ++row) {
        free((structToFree->_data[row]));
    }
    free(structToFree->_data);
}
