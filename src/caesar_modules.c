/* 
 * caeser modules
 * author: nick pestell
 * date: 14/08/2020 
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include "../include/caesar_modules.h"

#define ENCR "encrypt"
#define DECR "decrypt"

/* 
 * function: shift
 * --------------------------------------------------------------- 
 * shifts input charcter by specified rotation value for 
 * encryption or decryption
 * 
 * in: input letter of original message
 * rot_value: rotation value to shift letter by 
 * 
 * returns: a charecter describing the enecrypted or decrypted 
 * letter
 */
char shift(char in, int rot_value){

    char out;

    /* if rotation shifts to after z */
    if(in+rot_value > 'z') {
        out = in + (char)rot_value - 'z' + 'a' - (char)1;
    /* if rotation shifts to before a */
    } else if(in+rot_value < 'a') {
        out = 'z' +(char)1 - 'a' + in + (char)rot_value;
    } else {
        out = in + (char)rot_value;
    }

    return out; 
}

/* 
 * function: cipher
 * --------------------------------------------------------------- 
 * encrypts or decrypts the message contained in file by calling 
 * shift for each charecter read from the FILE stream 
 * 
 * mode: specifies encryption (1) or decryption (-1) 
 * input: input message
 * rot_value: rotation value for encryption or decryption 
 * 
 * returns: a sting containing the output message
 */
char *cipher(int mode, char *input, int rot_value){

    char *output = (char*)calloc(1, sizeof(char));
    
    /* if input redirection used */
    if(input == NULL){
        char c;
        int n = 1;
        while((c = getchar()) != EOF){
            if(isalpha(c)){
                output[n-1] = shift(c, rot_value*mode);
            } else {
                output[n-1] = c;
            }
            n++;
            output = realloc(output, (n)*sizeof(char));
        }
        output[n-1] = '\0';
    }
    else
    {
        /* initialises output string of the same length as the 
        input */
        output = realloc(output, (strlen(input))*sizeof(char));

        int i=0;
        /* loop through input charecters */
        while(input[i] != '\0') {
            if(isalpha(input[i])){
                /* shift the input if letter*/
                output[i] = shift(input[i], rot_value*mode);
            } else {
                output[i] = input[i];
            }
            i++;         
        }
        output[i] = '\0';
    }

    return output;
}

/* 
 * function: mode_select
 * --------------------------------------------------------------- 
 * asks user to specify the cipher mode (encrypt/decrypt). reads 
 * the input stream using getchar. compares input to ENCR and DECR
 * at sets the mode accordingly
 * 
 * returns: an integer describing the mode: 1 for encrypt and -1 
 * for decrypt
 */
int mode_select(void){
    
    char s;
    int mode = 0;
    char *in_mode = (char*)calloc(1, sizeof(char));

    printf("type encrypt or decrypt, followed by ENTER\n");
    while(1) {
        /* dynamically allocate memory and read in user input */
        int i = 0;
        while((s = getchar())!= '\n'){
            i++;
            in_mode = realloc(in_mode, (i+1)*sizeof(char));
            in_mode[i-1] = s;
        }
        in_mode[i] = '\0'; 
        /* check if correct format */
        if(strcmp(in_mode, ENCR) == 0){
            mode = 1;
            break;
        } else if(strcmp(in_mode, DECR) == 0) {
            mode = -1;
            break;
        } else {
            printf("incorrect input, please type encrypt or "
                   "decrypt\n");
        } 
    }

    return mode;
}

/* 
 * function: auto_select
 * --------------------------------------------------------------- 
 * asks the user if they would like to automatically decrypt
 * 
 * returns: integer value. 121 for yes. 110 for no.  
 */
int auto_select(void){

    char s;
    char auto_flag;
    int i = 0;

    printf("would you like to automatically decrypt (y/n)?\n");
    while(1) {
        /* waits for end of line */
        while((s = getchar())!= '\n'){
            auto_flag = s;
            i++;
        }
        /* check if input is 'y' or 'n' */
        if(i == 1 && auto_flag == 'y' || i == 1 && auto_flag == 
           'n') {
            break;
        } else {
            printf("please enter 'y' for yes or 'n' for no\n");
            i = 0; 
        }
    }

    return (int)auto_flag;
}

/* 
 * function: rot_select
 * --------------------------------------------------------------- 
 * asks user to specify the rotation value. reads the input stream 
 * using getchar. checks if each charecter is numeric. transforms 
 * the rotation value to lie between 0 and 26
 * 
 * returns: rotation value as an integer
 */
int rot_select(void){

    int rot;
    char s;
    char *in_rot = (char*)calloc(1, sizeof(char));

    printf("type a positive valued integer rotation value, "
           "followed by "
           "ENTER\n");
    while(1){
        /* dynamically allocate memory and read in user input */
        int i = 0;
        while((s = getchar())!= '\n'){
            i++;
            in_rot = realloc(in_rot, (i+1)*sizeof(char));
            in_rot[i-1] = s;
        }
        in_rot[i] = '\0'; 
        /* check if correct format */
        int num_check = 1;
        for(int j = 0; j<i; j++) {
            if(isdigit(in_rot[j]) == 0){
                printf("incorrect input, enter a number\n");
                num_check = 0;
                break;
            }
        }
        if(num_check){
            rot = atoi(in_rot);
            break;
        } 
    }
    /* transform rotation value to be between 0 and 26 */
    if(rot > 26) {
    rot = rot-26;
    }

    return rot;
}

/* 
 * function: file_select
 * --------------------------------------------------------------- 
 * if not using file redirection: asks user to provide a 
 * valid .txt file for encryption or decryption and reads the 
 * input stream using getchar. attempts to open a file with name 
 * provided by the user using fopen and reads the file into a 
 * character array
 * if using file redirection: reads the contents of stdin into a
 * character array. stdin is provided by the user when running
 * caeser_modules.exe via file redirection (<) 
 * 
 * in_tty: intiger flag for input_redirection
 * 
 * returns: pointer to character array  
 */
char *file_select(int in_tty){

    FILE *fp;
    char s;
    char *in_fp = (char*)calloc(1, sizeof(char)); 
    char in_char;
    char *input = (char*)calloc(1, sizeof(char));
    
    /* if not using input redirection */
    if(in_tty){
        printf("enter a filename for encryption/decryption\n");
        while(1){
            /* dynamically allocate memory and read in user input */
            int i = 0;
            while ((s = getchar())!= '\n'){
                i++;
                in_fp = realloc(in_fp, (i+1)*sizeof(char));
                in_fp[i-1] = s;
            }
            in_fp[i] = '\0'; 
            fp = fopen(in_fp, "r"); 
            /* check if file was correctly opened */
            if(!fp) {
                printf("could not find file, please enter a valid"
                       " filename\n");
            } else {
                break;
            }
        }
    }

    int i = 0;
    while(1) {
        i ++;
        /* dynamically allocate memory for each charecter read 
        from file */
        input = realloc(input, (i+1)*sizeof(char));
        /* convert each charecter to lowercase */
        if(!in_tty){
            in_char = tolower(getchar());
        }
        else{
            in_char = tolower((char)fgetc(fp));
        }
        if(in_char == EOF){
            break;
        } else if(isalpha(in_char)){
            /* shift the input */
            input[i-1] = in_char;
        } else {
            input[i-1] = in_char;
        }         
    }
    /* change the standard input stream back to the keyboard*/
    if (!freopen("/dev/tty", "r", stdin)) {
        perror("/dev/tty");
    exit(1);
    }

    return input;
}

/* 
 * function: likelihood
 * --------------------------------------------------------------- 
 * calculates the likelihood of the candidate decryption message 
 * based on letter
 * frequency in the english language
 * 
 * cand: the candidate message 
 *  
 * returns: likelihood  
 */
float likelihood(char *cand){

    /* frequencies of each letter (a-z) in the english language */
    float freqs[] = {0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 
    0.02228, 0.02015, 0.06094, 0.06966, 0.00153, 0.00772, 0.04025, 
    0.02406, 0.06749, 0.07507, 0.01929, 0.00095, 0.05987, 0.06327,
    0.09056, 0.02758, 0.00978, 0.02360, 0.00150, 0.01974, 
    0.00074};
    
    float likli = 0;

    /* itterate through the candidate decrypted message */
    for(int i=0; i<strlen(cand); i++){
        if(isalpha(cand[i])){
            /* update liklihood */
            likli += freqs[(int)cand[i]-97];
        }
    }

    return likli;
}

/* 
 * function: find_most_likli
 * --------------------------------------------------------------- 
 * itterates through possible decryption rotation values performs 
 * decryption and calls likelihood function for each decrypted 
 * message 
 * 
 * input: the encrypted message 
 *  
 * returns: the most likely rotattin value  
 */
int find_most_likli(char *input){

    float likli;
    float most_likli = 0;
    int rot_val = 0;

    /* itterate through possible rotation values */
    for(int i = 0; i<27; i++){
        /* calculate liklihood */
        likli = likelihood(cipher(-1, input, i));
        /* update most_likli */
        if(likli > most_likli){
            most_likli = likli;
            rot_val = i;
        }
    }

    return rot_val;
}

/* 
 * function: ten_most_common
 * --------------------------------------------------------------- 
 * seperates the decrypted message into strings of individual 
 * words by upon finding a word in the input message the word is 
 * added to a linked list which is ordered by frequency of word 
 * occurance by calling function: add_word 
 * 
 * msg: the decrypted message 
 *  
 * returns: a pointer to the head of the linked list of word 
 * frequency occurance  
 */
struct wnode *ten_most_common(char *msg){ 

    /* creates the linked list head node */
    struct wnode *head;
    char word[20];
    int n = 0;

    /* initialise the head node to NULL */
    head = NULL;
    /* loops through characters in the decrypted message */
    for(int i = 0; i<strlen(msg); i++){
        /* if complete word */
        if((msg[i] == ' ')||(msg[i] == '\n')||(msg[i] == '\0')){
            word[n] = '\0'; /* add null character */
            n = 0;
            /* add the word to the linked list - calls add_node 
            with the first working node as the head of the linked 
            list*/
            head = add_word(head, word);
        }
        /* if normal letter */
        else if isalpha(msg[i]){
            /* add the ltter to the word */
            word[n] = msg[i];
            n++;
        }
    }

    return head;
}

/* 
 * function: add_word
 * --------------------------------------------------------------- 
 * adds words from the decrypted message to the linked list. novel
 * words add new nodes to the end of the linked list. for repeated
 * words the count attribute of that word is increased by 1. the 
 * linked list is sorted by word frequency  
 * 
 * p: a pointer to the working node in the linked list
 * w: a string for current word  
 *  
 * returns: a pointer to the working node 
 * 
 * code explanation: the program moves down through the 
 * existing linked list by a recursive call to itself wih the 
 * argument p pointing to the next node in the list. the function 
 * returns when either p == NULL (novel word) or w = p->word (the 
 * word matches that of the working node). at this point the 
 * program returns the current working node from each recursive 
 * call to add_node and assigns it to the p->less of the node one 
 * up from itself thus moving back up through the linked list 
 * towards the head. inbetween each return, the program checks if 
 * the working node and the previous node are in the correct order
 * and reverses their positions in the linked list if necesary 
 * with a call to the sort function. the last call to add_word 
 * returns the head of the linked list. 
 */
struct wnode *add_word(struct wnode *p, char *w){

    /* if novel word - for novel words p is always NULL prior to 
    the node being filled */
    if (p == NULL){
        /* allocated memory for the new node on the heap */
        p = malloc(sizeof(struct wnode));
        p->word = strdup(w);
        p->count = 1;
        /* assigns the pointer to the next node to NULL - the next
        node is novel */
        p->less = NULL;
    }
    /* if word matches the working node */
    else if (strcmp(w, p->word) == 0){
        p->count++;
    }
    /* word does not match the current working node */
    else{
        /* recursive call to the add_word function where the 
        working node is the next node in the linked list */
        p->less = add_word(p->less, w); 
        p = sort(p);
    }

    return p;
}

/* 
 * function: sort
 * --------------------------------------------------------------- 
 * checks if working node and the next node down in the linked 
 * list are in the correct order and switches their positions if 
 * necesary  
 * 
 * p: a pointer to the working node in the linked list
 *  
 * returns: a pointer to the working node 
 */
struct wnode *sort(struct wnode *p){

    if (p->less->count > p->count){
        struct wnode *temp_1 = p->less;
        struct wnode *temp_2 = p->less->less;
        p->less->less = p;
        p->less = temp_2;
        p = temp_1;
    }

    return p;
}
