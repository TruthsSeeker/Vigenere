#include <stdio.h>
#include <stdlib.h>
#include <string.h>




int main(int argc, char *argv[])
{
    int EncryptOrDecrypt; //0 for encrypt, 1 for decrypt
    int InputOrFile; // same logic but for string input
    int OutputOrFile; // idem for output
    const unsigned int o = ' '; //Initial offset
    char Table[95][95];
    char key[]= "Musique";
    char input[] = "w|US_[MbUi_kA_e_W";
    char *keyp, *inputp; //trying stuff with pointers to get around the clusterfuck of dynamic size array creation



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
            for (i=0; i<=InputLength; i++){
                printf("%c", crypter(key[i%KeyLength], input[i]));
            }
        }

        else if (EncryptOrDecrypt==1){
            for (i=0; i<=InputLength; i++){

                printf("%c", decrypter(key[i%KeyLength], input[i]));

            }
        }
        printf("\n");
    }

    void fileVigenerisation (char key[], char input[]){
        int i;
        FILE *ResultFile;
        ResultFile=fopen("result.txt", "w");
        if (EncryptOrDecrypt==0){
            for (i=0; i<strlen(input)-1;i++){
                fputc(crypter(key[i%(strlen(key)-1)], input[i]), ResultFile);
                printf("%d", i);
            }
            fclose(ResultFile);
        }

        else if (EncryptOrDecrypt==1){
            for (i=0; i<strlen(input)-1;i++){
                fputc(decrypter(key[i%(strlen(key)-1)], input[i]), ResultFile);
            }
            fclose(ResultFile);
        }
    }

    char *getFileContent (const char* FileAdress){
        FILE *fp=fopen(FileAdress, "r");
        fseek(fp, 0, SEEK_END);
        int FLen=ftell(fp);
        rewind(fp);
        char* Buffer= malloc(FLen);
        for (int i=0; i<FLen; i++){
            Buffer[i]=fgetc(fp);
        }
        fclose(fp);
        printf("%d", strlen(Buffer));
        return Buffer;

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

            int k;
            for(k=0; k<argc; k++){
                if (strcmp(argv[k], "-h") == 0){
                    helper();
                }

                else if (strcmp(argv[k], "-e") == 0){
                    EncryptOrDecrypt=0;
                    keyp=argv[k+1];
                    k++;
                }

                else if (strcmp(argv[k], "-d") == 0){
                    EncryptOrDecrypt=1;
                    keyp=argv[k+1];
                    k++;
                }

                else if (strcmp(argv[k], "-fi") == 0){
                    InputOrFile=1;
                    inputp=argv[k+1];
                    k++;
                }

                else if (strcmp(argv[k], "-ki") == 0){
                    InputOrFile=0;
                    inputp=argv[k+1];
                    k++;
                }
                else if (strcmp(argv[k], "-fo") == 0){
                    OutputOrFile=1;
                }
                else if(strcmp(argv[k], "-so") == 0){
                    OutputOrFile=0;
                }
            }
        }
    }



    initTable();
    EncryptOrDecrypt=0;
    fileVigenerisation(key, getFileContent("test.txt"));
//    argParser();
//    printf("%s", inputp);

    return 0;
}
