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
        char* Buffer= malloc(FLen+1);
        for (int i=0; i<FLen; i++){// Replace with fread once understood
            Buffer[i]=fgetc(fp);
        }
        Buffer[FLen+1]='\0';
        fclose(fp);
        return Buffer;

    }

    void interactiveMode(){
        char c;
        char InteractiveKey[100];
        char InteractiveInput[255];

        void setEncryptOrDecrypt(){
            printf("Do you wish to encrypt or decrypt something? e/d\n");
            c= getchar();
            fflush(stdin);
            if (c=='e'){
                c=0;
                EncryptOrDecrypt=0;
                printf("Please enter the key you wish to use (max 100 characters):\n ");
                keyp=gets(InteractiveKey);
                fflush(stdin);
            }
            else if (c=='d') {
                c=0;
                EncryptOrDecrypt=1;
                printf("Please enter the key you wish to use (max 100 characters):\n ");
                keyp=gets(InteractiveKey);
                fflush(stdin);
            }
            else {
                printf("I didn't get that\n\n");
                setEncryptOrDecrypt();
            }
        }

        void setInputOrFile(){
            printf("Do you wish to encode an input or a file? i/f\n");
            c= getchar();
            fflush(stdin);
            if (c=='i'){
                c=0;
                InputOrFile==0;
                printf("Please input the text you wish to use (Max 255 characters) :\n");
                inputp=gets(InteractiveInput);
                fflush(stdin);
            }
            else if (c=='f'){
                c=0;
                InputOrFile==1;
                printf("Please input the adress of the file you wish to use (Max 255 characters) :\n");
                inputp=gets(InteractiveInput);
                fflush(stdin);
            }
            else {
                printf("I didn't get that\n\n");
                setInputOrFile();
            }

        }

        void setOutputOrFile(){
            printf("Do you wish to get the result in the console or in a file? c/f\n");
            c= getchar();
            fflush(stdin);
            if (c=='c'){
                c=0;
                OutputOrFile=0;
            }
            else if (c=='f'){
                c=0;
                OutputOrFile=1;
            }
            else {
                printf("I didn't get that\n\n");
                setOutputOrFile();
            }
        }

        printf("Welcome to the simple vigenere cypher interactive mode!\n");
        setEncryptOrDecrypt();
        setInputOrFile();
        setOutputOrFile();
    }

    void helper(){
        printf("Simple Vigenere key based encrypter and decrypter\n\n"
        "    May be used interactively or via command line arguments\n"
        "    If no arguments are provided, program will start in interactive mode\n\n"
        "Argument list:\n"
        "   -h : displays helper\n"
        "   -e \"KEY\": sets program to encrypt a string with KEY. KEY must be provided directly after this argument and must be a string\n"
        "   -d \"KEY\": sets program to decrypt a string with KEY. KEY must be provided directly after this argument and must be a string\n"
        "   -fi \"FILE_ADRESS\": sets program to accept a txt file as input. FILE_ADRESSS must be provided directly after this argument\n"
        "   -ki \"STRING\": sets program to accept a string as in input. STRING must be provided directly after this argument\n"
        "   -so : sets program to output results to console screen. Default Setting\n"
        "   -fo : sets program to output results to results.txt file\n");

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
    argParser();
    if (InputOrFile==0){
        if (OutputOrFile==0){
            vigenerisation(keyp, inputp);
        }
        else if (OutputOrFile==1){
            fileVigenerisation(keyp, inputp);
        }
    }

    else if (InputOrFile==1){
        if (OutputOrFile==0){
            vigenerisation(keyp, getFileContent(inputp));
        }
        else if (OutputOrFile==1){
            fileVigenerisation(keyp, getFileContent(inputp));
        }
    }

    return 0;
}
