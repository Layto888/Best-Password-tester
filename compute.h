#include <stdlib.h>

/* # the default requirement values of a good password are defined like: */

#define MIN_LENGTH 8 /* Minimum length of a password that represents for us a very good score */
#define MIN_DIGITS 1 /* Minimum nb of digits */
#define MIN_LOWUP 1 /* the min values of lower & upper characters in the password */
#define MIN_PUNCT 2 /* Minimum punctuation */

#define MIN_REQUIREMENTS 4 /* If requiCount donsn't Contain 4/5 of the requierements criteria,
                             PLUS the minimum lenght of password, the score of requirement will be 0 */


/*All deduction values can be set right there */

#define MAX_SCORE 100
#define MIN_SCORE 0
#define MIN_REPCHAR 2
#define MIN_CONSECHAR 2
#define MIN_CONSEDIGIT 2
#define MIN_SEQUENTIAL 3

#define NB_BENCH 1000000 /* NB test per lenght */


/* CONSTANTS VALUE : don't edit */

#define ALPHABET 26
#define DIGITS 10
#define BUFF_PASS 64 /* The maximum length of password */



typedef struct
{
    /* The ratio is the same as the score except that it's not limited by
     * the 100% and 0% values ​​(used to find even more than an optimal pass when we reach the value 100%)
     */
    int  score, ratio;
    char password[BUFF_PASS];
    size_t requierement_done; /*used as bool type */
    size_t len,
           nuppr,
           nlowr,
           ndigit,
           nsymbl,
           nrequir;
} Tool;


extern void getGlobalScore (char const*, Tool*);
extern void showGlobal (Tool*);
extern void showPartial(Tool*);
