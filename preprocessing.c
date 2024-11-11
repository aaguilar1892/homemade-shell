#include "preprocessing.h"

char* preprocessing(char* cmd){

            //Delete leading whitespace
            for(int i = 0; i < strlen(cmd); ++i){
                    if(!isspace(cmd[i])){
                            strncpy(cmd, cmd + i, strlen(cmd));
                            break;
                    }
            }

            //Delete trailing whitespace
            for(int i = strlen(cmd) - 1; i >= 0 && isspace(cmd[i]); --i){
                   cmd[i] = '\0';
            }

            //Delete extra whitespace in the middle...
            bool currSpace = false;
            bool nextSpace = false;

            for(int i = 0; i < strlen(cmd); ++i){

                    currSpace = isspace(cmd[i]);
                    nextSpace = isspace(cmd[i+1]);

                    if(currSpace){
                        if(nextSpace){
                            for(int j = i; j < strlen(cmd); ++j){
                                    cmd[j] = cmd[j+1];
                            }
                            --i;
                        }
                    }
            }

            //Return processed command
            return cmd;
}
