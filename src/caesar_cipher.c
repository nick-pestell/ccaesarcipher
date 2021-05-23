/* 
 * caeser cipher 
 * author: nick pestell
 * date: 14/08/2020 
 */

# include <stdio.h>
# include <ctype.h>
# include <string.h>
#include <unistd.h>
#include <stdlib.h>
# include "../include/caesar_modules.h"

int main(int argc, char *argv[]){   

    /* if no extra arguments */
    if(argc <= 3){
        
        int stop = 0;
        int mode = 0;
        int auto_flag = 0;
        char *msg = NULL;
        int rot_val = 0;
        
        /* read input message from file */
        msg = file_select(isatty(STDIN_FILENO));
        
        /* if no command line arguments then ask for input */
        if(argc == 1){
            mode = mode_select();
            if(mode == -1){
                if((int)auto_select() == 121){
                    auto_flag = 1;
                } else {
                    rot_val = rot_select();
                }
            } else {
                rot_val = rot_select();
            }
        /* otherwise use command line inputs*/
        } else {
            if(strcmp(argv[1], "encrypt") == 0){
                mode = 1;
            } else if(strcmp(argv[1], "decrypt") == 0) {
                mode = -1;
            } else {
                printf("please provide mode (encrypt or "
                "decrypt) followed by rotation value or type " 
                "'auto' for auto decryption\n");
                exit(0);
            }
            if(isdigit(*argv[2])){
                rot_val = atoi(argv[2]);
            } else if(strcmp(argv[2], "auto") == 0 && mode == 1){
                printf("only use 'auto' with decryption\n");
                exit(0);
            } else if(strcmp(argv[2], "auto") == 0) {
                auto_flag = 1;                
            } else {
                printf("please provide mode (encrypt or "
                "decrypt) followed by rotation value or type "
                "'auto' for auto decryption x\n");
                exit(0);                
            }
        }       

        /* if decryption */
        if(mode == -1){
            /* if auto decryption */
            if(auto_flag){
                /* performs auto decryption */
                char *output = cipher(mode, msg, 
                                        find_most_likli(msg));
                printf("\n%s\n\n", output);
                /* finds the most common words */
                struct wnode *start = ten_most_common(output);
                struct wnode *temp = start;
                printf("the ten most common words are ...\n");
                for (int i = 0; i < 10; i++){
                    printf("%s: %d\n", start->word, 
                            start->count);
                    start = start->less;
                }
            /* if manual decryption */
            } else {
                /* calls cipher function with specified 
                parameters */
                char *output = cipher(mode, msg, rot_val);
                printf("\n%s\n\n", output);
                /* finds the most common words */
                struct wnode *start = ten_most_common(output);
                struct wnode *temp = start;
                printf("the ten most common words are ...\n");
                for (int i = 0; i < 10; i++){
                    printf("%s: %d\n", start->word, 
                            start->count);
                    start = start->less;
                }
            }
        /* if encryption */
        } else {
            /* calls cipher function with specified parameters */
            printf("\n%s\n\n", cipher(mode, msg, rot_val));
        }
    /* if too many arguments */
    } else {
        printf("1st argument must be encrypt or decrypt and the "
               "2nd argument must be a rotation value or 'auto' " 
               "for auto decryption\n");
    }

    return 0; 
}
