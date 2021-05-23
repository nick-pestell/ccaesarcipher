/* caeser_modules.h 
 * author: nick pestell
 * date: 14/08/2020
 */

struct wnode{
    char *word;
    int count;
    struct wnode *less;
};

char shift(char in, int rot_value);
char *cipher(int mode, char *input, int rot_value);
int mode_select(void);
int auto_select(void);
int rot_select(void);
char *file_select(int in_tty);
float likelihood(char *cand);
int find_most_likli(char *input);
struct wnode *ten_most_common(char *msg);
struct wnode *add_word(struct wnode *p, char *word);
struct wnode *sort(struct wnode *p);