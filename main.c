#include <stdio.h>
#include <string.h>

int countWords(const char *line);

int countLetters(const char *line);

void readHistory(const char *FILENAME);


int main() {
    const char FILENAME[] = "history.txt";
    const char HISTORY[] = "history";
    const char EXIT[] = "exit";

    FILE *file = fopen(FILENAME,"a+");
    char input[100] = "";
    while (1) {
        printf("Enter a String or \"%s\" to end program:\n", EXIT);
        gets(input);
        if (strcmp(input, EXIT) == 0){
            printf("Program finished.");
            fclose(file);
            return 0;
        }

        if (strcmp(input, HISTORY) == 0){
            fclose(file);
            readHistory(FILENAME);
            file = fopen(FILENAME,"a+");
        }

        else {
            printf("%d words\n%d chars\n", countWords(input), countLetters(input));
            fprintf(file,"%s\n",input);
        }
    }

}

int countWords(const char *line) {
    int counter = 1;
    for (int i = 0; i < strlen(line); i++) {
        if (line[i] == ' ')
            counter++;
    }
    return counter;
}

int countLetters(const char *line) {
    int counter = 0;
    for (int i = 0; i < strlen(line); i++) {
        if (line[i] != ' ')
            counter++;
    }
    return counter;
}

void readHistory(const char *FILENAME) {
    FILE *file = fopen(FILENAME,"r");
    char currentLine[100];
    int counter=0;
    while (fgets(currentLine,100,file)) {
        printf("%d: %s",counter++, currentLine);
    }
    fclose(file);
}
