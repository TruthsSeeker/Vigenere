#include <stdio.h>
#include <stdlib.h>
#include <string.h>




int main(int argc, char *argv[])
{
    int EncryptOrDecrypt; //0 for encrypt, 1 for decrypt
    const unsigned int o = ' '; //Initial offset
    char Table[95][95];



    void initTable(){ // fills the vigenere table with the appropriate character in each case
        unsigned int v;
        const int length = '~' - ' '+1;
        for(unsigned int i = 0; i<length; i++){
            for(unsigned int j=0; j<length; j++){ // Two nested "for"s ensure iteration over the whole table, i for rows, j for columns
                v = o+i+j; // Each character in a vigenere table is conveniently represented by the sum of the value of the character to encrypt and of the character to decrypt (including the necessary offset)
                if (v>'~'){ // This makes sure that if the value of the character was to go over the limit of the included characters, it would start over from the first character
                    Table[i][j]= v - length; //There are 95 characters in the table, could be replaced by a variable to include more characters if needed
                }
                else{
                    Table[i][j]=v;
                }
            }
        }


    }

    char crypter(char key, char value){ //encrypts a single character using the table. Sanitize input to ensure no LF is left inside
        if (key < o|| value < o){//abort if variable isn't in characters table because it's too small to be the first accepted character
            return 0;
        }

        unsigned int a = key - o; //remove offset to use char as index
        unsigned int b = value - o; // idem
        char c = Table[a][b]; // access table to determine encoded character
        return c;

    }

    char decrypter(char key, char value){
        if (key < o|| value < o){//abort if variable isn't in characters table because it's too small to be the first accepted character
            return 0;
        }
        unsigned int i = 0;
        unsigned int a = key-o;

        while(Table[a][i]!= value){ // Iterate over appropriate row to find the corresponding character
            i++;
        }
        char c = i+o; // Column index + offset gives decoded character
        return c;

    }

    void vigenerisation(char key[], char input[]){

        unsigned int KeyLength = strlen(key)-1;// -1 to convert length to index
        unsigned int InputLength= strlen(input)-1;
        int i;
        if(EncryptOrDecrypt==0){
            for (i=0; i<InputLength; i++){

                printf("%c", crypter(key[i%KeyLength], input[i]));


            }
        }
        else if (EncryptOrDecrypt==1){
            for (i=0; i<InputLength; i++){

                printf("%c", decrypter(key[i%KeyLength], input[i]));

            }
        }
        printf("\n");
    }

    void interactiveMode(){
        printf("Interactive mode!");
    }

    void helper(){
        printf("Display Help!");
    }

    void argParser(){
        if (argc<=1){
            interactiveMode();
        }
        else{
            int ArgNumber = strlen(argv);
            int k;
            for(k=1; k<ArgNumber; k++){
                if (argv[k]== "-h"){
                    helper();

                }
                else if (argv[k]== "-e"){
                    EncryptOrDecrypt=0;

                }
                else if (argv[k]== "-d"){
                    EncryptOrDecrypt=1;
                }
            }
        }

    }



    initTable();
    char key[]= "Musique";
    char input[] = "J'aime la musique";
    vigenerisation(key,input);
    argParser();

    return 0;
}
