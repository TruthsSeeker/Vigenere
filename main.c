#include <stdio.h>
#include <stdlib.h>
#include <string.h>




int main(int argc, char *argv[])
{
    int EncryptOrDecrypt; //0 for encrypt, 1 for decrypt
    int InputOrFile; // same logic but for string input
    int OutputOrFile; // idem for output
    int INeedSomeBody;//1 for help
    const unsigned int o = ' '; //Initial offset
    char Table[95][95];
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

    char crypter(char key, char value){ //encrypts a single character using the table.
        if (key < o|| value < o){//abort if variable isn't in characters table because it's too small to be the first accepted character
            return 0;
        }

        unsigned int a = key - o; //remove offset to use char as index
        unsigned int b = value - o; // idem
        char c = Table[a][b]; // access table to determine encoded character
        return c;

    }

    char decrypter(char key, char value){//decrypts a single character using the table
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
        if(EncryptOrDecrypt==0){//if program set to encrypt
            for (i=0; i<=InputLength; i++){//for each character in input
                printf("%c", crypter(key[i%KeyLength], input[i]));//print encrypted character. i%KeyLength to start over once KeyLength is reached
            }
        }

        else if (EncryptOrDecrypt==1){//if program set to decrypt
            for (i=0; i<=InputLength; i++){//for each character in input

                printf("%c", decrypter(key[i%KeyLength], input[i]));//print encrypted character. i%KeyLength to start over once KeyLength is reached

            }
        }
        printf("\n");
    }

    void fileVigenerisation (char key[], char *input){
        int i;
        FILE *ResultFile;
        ResultFile=fopen("result.txt", "w");//opens or creates result.txt in writing mode
        if (EncryptOrDecrypt==0){//if program set to encrypt
            for (i=0; i<strlen(input)-1;i++){//for each character in input
                fputc(crypter(key[i%(strlen(key)-1)], input[i]), ResultFile);//write character in opened file

            }
            fclose(ResultFile);//close file
        }

        else if (EncryptOrDecrypt==1){//if program set to decrypt
            for (i=0; i<strlen(input)-1;i++){//for each character in input
                fputc(decrypter(key[i%(strlen(key)-1)], input[i]), ResultFile);//write character in opened file
            }
            fclose(ResultFile);// close file
        }
    }

    char *getFileContent ( char* FileAdress){
        FILE *fp=fopen(FileAdress, "r");//opens file from adress in reading mode
        fseek(fp, 0, SEEK_END);//find last position of file
        int FLen=ftell(fp);//ftell last position to determine file size
        rewind(fp);//rewind cursor to beginning
        char* Buffer= malloc(FLen+1);//allocate enough memory for file
        for (int i=0; i<FLen; i++){// Replace with fread once understood
            Buffer[i]=fgetc(fp);
        }
        Buffer[FLen+1]='\0';//add sentinel
        fclose(fp);//close file
        return Buffer;//return array with file contents

    }

    void interactiveMode(){
        char c;
        char InteractiveKey[100], InteractiveInput[255];

        void setEncryptOrDecrypt(){//Function to set program to encrypt or decrypt
            printf("Do you wish to encrypt or decrypt something? e/d\n");
            c= getchar();//get input
            fflush(stdin);//flush buffer
            if (c=='e'){//compare c to parameter
                EncryptOrDecrypt=0;//flag encrypt mode
                printf("Please enter the key you wish to use (max 100 characters):\n ");
                keyp=gets(InteractiveKey);//get key from input
                fflush(stdin);//flush buffer
            }
            else if (c=='d') {//compare c to parameter
                EncryptOrDecrypt=1;//flag decrypt mode
                printf("Please enter the key you wish to use (max 100 characters):\n ");
                keyp=gets(InteractiveKey);//get key from input
                fflush(stdin);//flush buffer

            }
            else {
                printf("I didn't get that\n\n");//calls function again if parameter isn't recognized
                setEncryptOrDecrypt();
            }
        }

        void setInputOrFile(){//function to set program to accept input from stdin or file
            printf("Do you wish to encode an input or a file? i/f\n");
            c= getchar();//get input
            fflush(stdin);//flush buffer
            if (c=='i'){//compare c to parameter
                InputOrFile=0;//flag stdin mode
                printf("Please input the text you wish to use (Max 255 characters) :\n");
                inputp=gets(InteractiveInput);//get input
                fflush(stdin);//flush buffer
            }
            else if (c=='f'){//compare c to parameter
                InputOrFile=1;//flag file input mode
                printf("Please input the adress of the file you wish to use (Max 255 characters) :\n");
                inputp=gets(InteractiveInput);//get adress of file
                fflush(stdin);//flush buffer

            }
            else {
                printf("I didn't get that\n\n");//call function again if parameter wasn't recognized
                setInputOrFile();
            }

        }

        void setOutputOrFile(){//function to set program to output to console or to a file
            printf("Do you wish to get the result in the console or in a file? c/f\n");
            c= getchar();//get input
            fflush(stdin);//flush buffer
            if (c=='c'){//compare c to parameter
                OutputOrFile=0;//flag console output mode
            }
            else if (c=='f'){//compare c to parameter
                OutputOrFile=1;//flag file output mode
            }
            else {//call function again if parameter wasn't recognized
                printf("I didn't get that\n\n");
                setOutputOrFile();
            }
        }

        printf("Welcome to the simple vigenere cypher interactive mode!\n");//successive function calls to set necessary flags
        setEncryptOrDecrypt();
        setInputOrFile();
        setOutputOrFile();
    }

    void helper(){//prints help
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

    void argParser(){// function to parse command line arguments and set appropriate flags
        if (argc<=1){// if no argument is provided
            interactiveMode();//start interactive mode
        }
        else{

            int k;
            for(k=0; k<argc; k++){// for each argument
                if (strcmp(argv[k], "-h") == 0){//compare argument to a parameter
                    INeedSomeBody=1;//flag for help
                }

                else if (strcmp(argv[k], "-e") == 0){
                    EncryptOrDecrypt=0;//set program to encrypt
                    keyp=argv[k+1];//next argument needs to be the key
                    k++;//no need to check the next one, we've done what we need to with it already
                }

                else if (strcmp(argv[k], "-d") == 0){
                    EncryptOrDecrypt=1;//set program to decrypt
                    keyp=argv[k+1];//next argument needs to be the key
                    k++;//no need to check the next one, we've done what we need to with it already
                }

                else if (strcmp(argv[k], "-fi") == 0){
                    InputOrFile=1;//set program to accept file input
                    inputp=argv[k+1];//next argument needs to be file adress
                    k++;//no need to check the next one, we've done what we need to with it already
                }

                else if (strcmp(argv[k], "-ki") == 0){
                    InputOrFile=0;//set program to accept stdin
                    inputp=argv[k+1];//next argument needs to be the input
                    k++;//no need to check the next one, we've done what we need to with it already
                }
                else if (strcmp(argv[k], "-fo") == 0){
                    OutputOrFile=1;//set program to output a file
                }
                else if(strcmp(argv[k], "-so") == 0){
                    OutputOrFile=0;//set program to output to screen
                }
            }
        }
    }





    initTable();//calls table initialization function
    argParser();//parse arguments
    if (INeedSomeBody==1){//if help is required
        helper();// help is provided
    }
    else{
        if (InputOrFile==0){//do stuff depending on flags
            if (OutputOrFile==0){
                vigenerisation(keyp, inputp);//if input= stdin && output=console call vigenerisation
            }
            else if (OutputOrFile==1){
                fileVigenerisation(keyp, inputp);//if input=stdin && output=file call fileVigenerisation
            }
        }

        else if (InputOrFile==1){
            if (OutputOrFile==0){
                vigenerisation(keyp, getFileContent(inputp));//if input=file && output=console call vigenerisation with getFileContent(inputp) as input
            }
            else if (OutputOrFile==1){
                fileVigenerisation(keyp, getFileContent(inputp));//if input=file && output=file call fileVigenerisation with getFileContent(inputp) as input

            }
        }
    }

    return 0;
}
