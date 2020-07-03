#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"
#define REVERSE_ALPHABET "zyxwvutsrqponmlkjihgfedcba"

unsigned short state = 0;
char r1[27] = "", r2[27] = "", r3[27] = "", rotor[27] = "";
FILE *file;

// read 3 rotors from .enigma file
void ReadRotors() {
    if(access(".enigma", F_OK) == -1) {
        printf("before run Enigma.out --new for write rotors\n");
        exit(1);
    }
    FILE *FRotors = fopen(".enigma", "rb");
		fscanf(FRotors, "%s %s %s", r1, r2, r3);
    fclose(FRoters);
}

char FindIndex(const char *arr, char search) {
    for (register char i = 0; i < strlen(arr); i++)
        if (search == arr[i]) return i;
		return -1;
}

// Enigma function convert q character to a enigma character
char Enigma(char character) {
    char c1 = r1[character - 97];
    char c2 = r2[c1 - 97];
    char c3 = r3[c2 - 97];
    char reflect = REVERSE_ALPHABET[c3 - 97];
    c3 = ALPHABET[FindIndex(r3, reflect)];
    c2 = ALPHABET[FindIndex(r2, c3)];
    c1 = ALPHABET[FindIndex(r1, c2)];
    return c1;
}

//shift array convert 'abcdef' to 'bcdefa'
void ShiftArray(char *r) {
    char temp[27];
    strcpy(temp, r);
    char tmp = r[0];
    for (register char i = strlen(r) - 1; i > 0; i--)
        r[i - 1] = temp[i];
    r[strlen(r) - 1] = tmp;
}

void RotateRotors() {
    ShiftArray(r1);
    if (state % 24 == 0) ShiftArray(r2);
    if (state % (24 * 24) == 0) ShiftArray(r3);
}

// write new rotors in .enigma
void WriteToFile(const char *format) {
    bool CanAdd = true;
    register char i = 0;
    for (i = 0; i < 26; i++) {
        char random = (rand() % ('z' - 'a' + 1)) + 'a';
        for (register char j = 0; j < i; j++)
            if (random == rotor[j]) {
                CanAdd = false;
                break;
            }
        if (CanAdd == true) rotor[i] = random;
        else i--;
        CanAdd = true;
    }
    rotor[i + 1] = '\0';
    fprintf(file, format, rotor);
}

int main(int argc, char **argv) {
    if (argc == 2) {
        if (!strcmp(argv[1], "--new")) {
            srand(time(NULL));
            file = fopen(".enigma", "wb");
            WriteToFile("%s");
            WriteToFile(" %s ");
            WriteToFile("%s");
            fclose(file);
            return 0;
        }
        else {
            ReadRotors();
            for (register unsigned short i = 0; i < strlen(argv[1]); i++) {
                state++;
                putchar(Enigma(argv[1][i]));
                RotateRotors();
            }
            putchar('\n');
        }
        return 0;
    }
    printf("Usage:\n   ./Enigma.out word\ncommands:\n   --new: write new rotors\n");
    return 0;
}
