/*****************************************************************************/
/*  COMP30023 Assignment 1
/*  Eishitha Galpayage Don
/*  993413
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "queue.h"

int main(int argc, char *argv[]){

    /*  Parse command line args */
    int option;
    char *filename = NULL;
    int num_processors;
    while ((option = getopt(argc, argv, "f:p:c")) != -1){
        switch (option){
            case 'f':
                filename = optarg;
                break;
            case 'p':
                num_processors = atoi(optarg);
                break;
            case 'c':
                break;
            case '?':
                return 1;
        }
    }
    
}