#include "compute.h"
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>


/* A) Additions part */

static int middleNumSymb (Tool *t)
{
    if (t->len <= 1)
        return 0;

    size_t i;
    int score = 0;
    /*we just look for all middle symbols or numbers and count them */

    for (i = 1; i < t->len-1; i++)
    {
        if (isdigit(t->password[i]) || ispunct(t->password[i]))
            score++;
    }
    return (2 * score);
}



static void computeAdditions (Tool *t)
{
    if(t->len < MIN_LENGTH || t->nrequir < MIN_REQUIREMENTS)
        t->requierement_done = 0; /*KO*/
    else
        t->requierement_done = 1; /*Ok*/

    /*adding Flat */
    t->score += (t->nrequir*2) * t->requierement_done + (t->len*4) + (t->nsymbl*6);
    t->score +=  middleNumSymb(t);

    /*adding cond */
    if (t->nuppr > 0)
        t->score +=  ((t->len-t->nuppr) * 2);
    if (t->nlowr > 0)
        t->score +=  ((t->len-t->nlowr) * 2);
    if (t->ndigit > 0)
        t->score +=  (t->ndigit*4);
}




/* B) Deductions part */

static inline int onlyLetters (Tool *t)
{
    return ((t->nlowr+t->nuppr) == t->len) ? (-t->len) : 0;
}



static inline int onlyDigits (Tool *t)
{
    return (t->ndigit == t->len) ? (-t->len) : 0;
}



static int repeatChars (Tool *t)
{
    /* counter to store repetition of all alphabet chars */
    unsigned char cpa[ALPHABET] = {0}, cpd[DIGITS] = {0};
    size_t index, i;
    int score = 0;

    if (t->len <= 1)
        return 0;

    for (i = 0; t->password[i]; i++)
    {
        if (isalpha(t->password[i]))
        {
            index = (size_t)(tolower(t->password[i]) - 'a');
            cpa[index]++;
        }
        else if (isdigit(t->password[i]))
        {
            index = (size_t)(t->password[i] - '0');
            cpd[index]++;
        }
    }
    /*if the number of repetition of some 'character' is >= MIN_REPCHAR
    * we add it to the negative score */
    for (index = 0; index < ALPHABET; index++)
        if (cpa[index] >= MIN_REPCHAR)
            score += (cpa[index] * cpa[index]);
    for (index = 0; index < DIGITS; index++)
        if (cpd[index] >= MIN_REPCHAR)
            score += (cpd[index] * cpd[index]);
    return (-score);
}


/* Consecutive Uppercase Letters and lower (case insensitive) we start
 * the count after the 2nr consecutive character in the string.*/
static int consecLetters (Tool *t)
{
    size_t cp = 0, i;
    int score = 0;

    if (t->len <= 1)
        return 0;

    for (i = 0; t->password[i]; i++)
    {
        if (isalpha(t->password[i]))
        {
            cp++;
            if (cp >= MIN_CONSECHAR)
                score ++;
        }
        else
            cp = 0;
    }
    return (-2 * score);
}



/* Consecutive numbers, we start the count after the MIN_CONSEDIGIT
 * consecutive number in the string. (see the header file for macros)
 * Nota bene that functions  consecLetters & consecDigitscould be
 * combined as one fucntion, we just split them for more reading readability of code.*/
static int consecDigits (Tool *t)
{
    size_t cp = 0, i;
    int score = 0;

    if (t->len <= 1)
        return 0;

    for (i = 0; t->password[i]; i++)
    {
        if (isdigit(t->password[i]))
        {
            cp++;
            if (cp >= MIN_CONSEDIGIT)
                score ++;
        }
        else
            cp = 0;
    }
    return (-2 * score);
}



/* check for ascending /descending sequentials Letters & digits;
 * start when the counter is >=: MIN_SEQUENTIAL   */
static int checkSequential (Tool *t)
{
    if (t->len <= 1)
        return 0;

    size_t i, cpAsc_seq = 0, cpDec_seq = 0;
    int score = 0;

    for (i=0;  i < t->len-1; i++)
    {
        /*check for ascending & descending sequential characters & numbers */
        if ((t->password[i]+1) == t->password[i+1])
        {
            cpAsc_seq++;
            /*  3 is the minimum sequential number of letters & digits to start add to score. */
            if (cpAsc_seq >= MIN_SEQUENTIAL)
                score ++;
        }
        else if ((t->password[i]) == (t->password[i+1]+1))
        {
            cpDec_seq++;
            if (cpDec_seq >= MIN_SEQUENTIAL)
                score++;
        }
        else
        {
            cpAsc_seq = 0;
            cpDec_seq = 0;
        }
    }
    return (-3 * score);
}




/*compute the general deduction B score */
static void computeDeductions (Tool *t)
{
    t->score += onlyLetters(t);
    t->score += onlyDigits(t);
    t->score += repeatChars(t);
    t->score += consecLetters(t);
    t->score += consecDigits(t);
    t->score += checkSequential(t);
}


/* ALL EXTERNAL functions must be implemented here
about the warning format %u For C89, use %lu and cast
the value to unsigned long, For C99 and later, use %zu
 */
extern void showPartial(Tool *t)
{
    printf ("Best password found:   %s   global score: %d(ratio %d)\n",t->password, t->score, t->ratio - 100);
}




/* the final score is computed A+B :
   final score = addition score A + deduction score B */
extern void getGlobalScore (char const * const password, Tool *t)
{
    size_t i;

    memset(t, 0, sizeof(Tool));
    t->len = strlen(password);
    memcpy(t->password, password, t->len);

    for (i = 0; t->password[i]; i++)
    {
        if(islower(t->password[i]))      t->nlowr++;
        else if(isupper(t->password[i])) t->nuppr++;
        else if(isdigit(t->password[i])) t->ndigit++;
        else if(ispunct(t->password[i])) t->nsymbl++;
    }

    /*alors the fucntion check for default minimum
     *requirements by incrementing l->nrequir for each case bellow:
    - Minimum  MIN_LENGTH characters in length
    - Contains 3/4 of the following items + the min length.:
    - Uppercase Letters  MIN_LOWUP
    - Lowercase Letters MIN_LOWUP
    - Numbers MIN_DIGITS
    - Symbols  MIN_PUNCT */
    if(t->len > MIN_LENGTH)     t->nrequir++;
    if(t->nlowr >= MIN_LOWUP)   t->nrequir++;
    if(t->nuppr >= MIN_LOWUP)   t->nrequir++;
    if(t->ndigit >= MIN_DIGITS) t->nrequir++;
    if(t->nsymbl >= MIN_PUNCT)  t->nrequir++;

    /*get the first addition score */
    computeAdditions (t);
    /*add deduction score to final score */
    computeDeductions (t);
    /* limit the range % of final score & get the ratio */
    t->ratio = t->score;
    if (t->score > MAX_SCORE) t->score = MAX_SCORE;
    else if (t->score < MIN_SCORE) t->score = MIN_SCORE;
}


/*printing summary */
extern void showGlobal (Tool *t)
{
    printf("\n======== Summary ======== \n\n");
    printf ("Password: %s\n\n",t->password);
    printf ("Number of Characters:(%u) Bonus: %u\n", t->len , t->len * 4);
    if (t->nuppr > 0)
        printf ("Upper Letters:(%d)  Bonus: %d\n",t->nuppr,(t->len - t->nuppr)*2);
    else
        printf ("Upper Letters:(%d)  Bonus: %d\n",t->nuppr , 0);
    if (t->nlowr > 0)
        printf ("Lower Letters:(%d)  Bonus: %d\n",t->nlowr,(t->len - t->nlowr)*2);
    else
        printf ("Lower Letters:(%d)  Bonus: %d\n",t->nlowr , 0);
    printf ("Numbers:(%d)  Bonus: %d\n",t->ndigit, t->ndigit*4);
    printf ("Symbols:(%d)  Bonus: %d\n",t->nsymbl,t->nsymbl*6);
    printf ("middle Num/Symb Bonus: %d\n", middleNumSymb(t));
    printf ("Requierements: (%d/5) Bonus: %d\n", t->nrequir, (t->nrequir * 2)* t->requierement_done);
    printf ("only Letters Bonus: %d\n",onlyLetters(t));
    printf ("only Digits Bonus: %d\n", onlyDigits(t));
    printf ("Repeat Chars Bonus: %d\n", repeatChars(t));
    printf ("consecutive Letters: %d\n", consecLetters(t));
    printf ("consecutive Digits: %d\n", consecDigits(t));
    printf ("Sequential (Letters/Digits/Symbols) Bonus: %d\n", checkSequential(t));
    printf ("\nYour global score: %d %%\n", t->score);
}
