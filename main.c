#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void readHistory(const char *FILENAME);
void operators(char *line, FILE *file);
void loop();
void memoryPrint(char *line, int start, int end, FILE *file);

int main() {
    loop();
    return 0;
}
//Main loop function to keep asking user for input and call other functions according to what is passed in to the string buffer
void loop() {
    const char FILENAME[] = "history.txt";
    const char HISTORY[] = "history";
    const char EXIT[] = "exit";

    FILE *file = fopen(FILENAME, "a+");
    char input[511] = "";
    while (1) {
        printf("Enter a String or \"%s\" to end program:\n", EXIT);
        gets(input);
        if (strcmp(input, EXIT) == 0) {
            printf("Program finished.");
            fclose(file);
            return;
        }

        if (strcmp(input, HISTORY) == 0) {
            //if "history" is passed in we have to close the file, so it can save its contents, and they are able to be read.
            fclose(file);
            readHistory(FILENAME);
            //After reading the contents of the file, we reopen the file writer in append mode to continue writing from it finished.
            file = fopen(FILENAME, "a+");
        }
        else
            operators(input, file);
    }
}
//the operators function is used to control everything related to counting the letters and the words of the input of the user and storing them in the history file.
void operators(char *line, FILE *file) {
    int lettersAmount = 0;
    int wordAmount = 1;
    if (strlen(line) == 0) {
        printf("%d words\n%d chars\n", 0, 0);
        return;
    }
    int end = 0;
    int i = 0;
    //If there are spaces at the start of the input we skip them until we reach a non-space character.
    while (line[i] == ' ') i++;
    int start = i;
    for (; i < strlen(line); i++) {
        end = i;
        /*If a space is found at index i and the next index also contains a space, we call the memoryPrint with the indexes of the first index
         * and the index of the first space we passed of the last word encountered.
         */
        if (i < strlen(line) - 1 && line[i] == ' ' && line[i + 1] == ' ') {
            memoryPrint(line, start, end, file);
            wordAmount++;
            //Since we found two/more spaces next to each other, a while loop is used to skip all spaces until we reach a non-space character,start and end are both updated.
            while (line[i] == ' '){
                i++;
                start=i;
            }
            if (start==strlen(line)){
                wordAmount--;
                lettersAmount--;
            }
            end = i;
        }
        //If a space is found by itself then we simply call the memoryPrint function with the indexes of the first index and the index of the space we encountered.
        if (line[i] == ' ') {
            memoryPrint(line, start, end, file);
            start=end+1; //start is updated to the index of the index after the space.
            wordAmount++;
        } else if (line[i] != ' ')
            lettersAmount++;
    }
    if (line[i] == '\0') /* In case the last index of the input was not a space, the last word would not
                          * be passed to the memoryPrint function, the start index is the first index of the last word and end is the index of the terminating "\0"
                          * character.*/
        memoryPrint(line, start, end + 1, file);
    printf("%d words\n%d chars\n", line[i - 1] == ' ' ? wordAmount - 1 : wordAmount, lettersAmount);
    fprintf(file,"\n");
}

//the memoryPrint function receives the whole input line by the user, the starting index and the index of the first space after the word that was last encountered.
void memoryPrint(char *line, int start, int end, FILE *file) {
    int length = end - start;
    if(length==0)
        return;
    char *word = (char *) calloc(length + 1, sizeof(char)); //length+1 space is allocated to also fit the terminating "\0" character into the string.
    strncpy(word, &line[start], length); //strncpy is used to copy the input from where the first letter's index, and length is to tell it how many characters to copy
    fprintf(file,"%s ", word);
    free(word);
}

//the readHistory function is simple function used to reopen the file in read mode and pass through all lines in the file while printing them to the terminal.
void readHistory(const char *FILENAME) {
    FILE *file = fopen(FILENAME, "r");
    char currentLine[511];
    int counter = 0;
    while (fgets(currentLine, 511, file)) {
        printf("%d: %s", counter++, currentLine);
    }
    fclose(file);
}
