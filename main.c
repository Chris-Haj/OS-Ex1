#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define LENGTH 512
#define MAX_INPUT 510
#define FILENAME "history.txt"


void readHistory();
int operators(size_t i, int mode, FILE *file, char *line);
void loop();

int main() {
    loop();
    return 0;
}
//Main loop function to keep asking user for input and call other functions according to what is passed in to the string buffer
void loop() {
//    const char FILENAME[] = "history.txt";
    const char HISTORY[] = "history";
    const char EXIT[] = "exit";
    const size_t EXIT_LENGTH = strlen(EXIT);
    const size_t HISTORY_LENGTH = strlen(HISTORY);
    //Length is equal to 512 because the last index contains \0 and the before last index contains
    // \n from stdin so the input will fit exactly 510 characters!
    FILE *file = fopen(FILENAME, "a+");
    char input[LENGTH] = "";
    while (1) {
        printf("Enter a String or \"exit\" to end program:\n");
        fgets(input,MAX_INPUT,stdin);
        size_t i=0;
        while (input[i]==' ') i++;
        if (strncmp(&input[i], EXIT,EXIT_LENGTH) == 0) {
            i+=EXIT_LENGTH;
            if (operators(i,1,file,input)==1)
                break;
        }
        else if (strncmp(&input[i], HISTORY,HISTORY_LENGTH) == 0) {
            //if "history" is passed in we have to close the file, so it can save its contents, and they are able to be read.
            i+=HISTORY_LENGTH;
            operators(i,2,file,input);
        }
        else
            operators(i, 0, file, input);
    }
}
//the operators function is used to control everything related to counting the letters and the words of the input of the user and storing them in the history file.
int operators(size_t i, int mode, FILE *file, char *line) {
    int lettersAmount = 0;
    int wordAmount = 0;
    int exitOrHistory = 0;
    if(mode != 0){
        if(mode == 1)
            lettersAmount+=strlen("exit");
        else if(mode == 2)
            lettersAmount+= strlen("history");
    }
    size_t length = strlen(line);
    if (line[i] != '\n' && line[i] != '\0')
        wordAmount++;
    if (length == 0) {
        printf("%d words\n%d chars\n", 0, 0);
        return 0;
    }
    for (; i < length && line[i] != '\n'; i++) {
        if (line[i] != ' '){
            lettersAmount++;
            exitOrHistory=1;
        }
        else if (line[i] == ' ') {
            while (line[i + 1] == ' ')
                i++;
            wordAmount++;
        }
    }
    if(exitOrHistory == 0){
        if(mode == 1){
            printf("Program finished.");
            fclose(file);
            return 1;
        }
        if(mode==2){
            //if "history" is passed in we have to close the file, so it can save its contents, and they are able to be read.
            fclose(file);
            readHistory();
            //After reading the contents of the file, we reopen the file writer in append mode to continue writing from it finished.
            file = fopen(FILENAME, "a+");
            return 0;
        }
    }
    printf("%d words\n%d chars\n", line[i - 2] == ' ' ? wordAmount - 1 : wordAmount, lettersAmount);
    if(line[i]=='\n')
        fprintf(file,"%s",line);
    else
        fprintf(file,"%s\n",line);
    return 0;
}

//the readHistory function is simple function used to reopen the file in read mode and pass through all lines in the file while printing them to the terminal.
void readHistory(){
    FILE *file = fopen(FILENAME, "r");
    if(file != NULL) {
        char currentLine[511];
        int counter = 0;
        while (fgets(currentLine, 511, file)) {
            printf("%d: %s", counter++, currentLine);
        }
        fclose(file);
    }
}
