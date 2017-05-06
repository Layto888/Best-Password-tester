/**
 * @brief : password meter algo & best pass finder.
 * @author : A.Amine.
 * @date : 03/03/2017
 * Source inspired from (http://www.passwordmeter.com/) but the rating methode is not exactly the same.

* Flat: Rates that add/remove in non-changing increments.
• Incr: Rates that add/remove in adjusting increments.
• Cond: Rates that add/remove depending on additional factors.
• Comp: Rates that are too complex to summarize. See source code for details.
• the struct Tool: Refers to the total number of occurrences.
• len: Refers to the total password length.
• Additional bonus scores are given for increased character variety.
• Final score is a cumulative result of all bonuses minus deductions.
• Final score is capped with a minimum of 0 and a maximum of 100.
• Score and Complexity ratings are not conditional on meeting minimum requirements.
 *
 * See headers files for more description...
 */

#include <stdio.h>
#include <time.h>
#include <string.h>
#include "compute.h"
#include "bench.h"


int main(void)
{
    Tool t, best, worst;
    size_t x = NB_BENCH, my_sz;
    char rndchar[BUFF_PASS] = {'\0'};

    /*init */
    srand(time(NULL));
    memset(&best, 0, sizeof(Tool));
    memset(&worst, 0, sizeof(Tool));
    worst.ratio = MAX_SCORE;


    /* benchmark test */
    for (my_sz = 3; my_sz < (MIN_LENGTH + 4); my_sz++)
    {
        printf("\nTest with length = %d\n\n", my_sz);
        while(x--)
        {
            rndmPass(rndchar, my_sz);
            getGlobalScore(rndchar, &t);
            /*look for the best pass*/
            if (t.ratio > best.ratio)
            {
                memcpy(&best, &t, sizeof(Tool));
                showPartial(&t);
            }
            /* look for the worst pass*/
            if (t.ratio < worst.ratio)
                memcpy(&worst, &t, sizeof(Tool));
        }
        x = NB_BENCH;
    }

    showGlobal(&best);
    printf ("\nPs: the worst password was: %s  global score: %d (ratio %d)", worst.password, worst.score, worst.ratio -100);
    /*showGlobal(&worst);*/
    return 0;
}
