#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){

    //Interactive mode
    if(argc == 1){

        //Until user exits shell
        while(1 == 1){
            
            //Get user input

            //If user input contains multiple commands, separate at ';' and execute sequentially

        }
    }

    //Batch mode
    if(argc == 2){

    }

    //Print error message if incorrect # of command line arguments
    if(argc != 1 && argc != 2){
            fprintf(stderr, "\n\t --- ERROR: Incorrect number of command line arguments ---\n\n");
            fprintf(stderr, "\tInteractive Mode: ./a.out\n");
            fprintf(stderr, "\tBatch Mode: ./a.out <batch_file>\n\n");
            exit(1);
    }

    return 0;
}
