#include "solver.h"

extern "C"
{
    TexasSolver* create_solver()
    {
        return new TexasSolver();
    }

    void set_number_players(TexasSolver *foo, int n)
    {
        foo->set_number_players(n);
    }

    double get_prob(TexasSolver *foo, uint8_t *cards, int n)
    {
        if (n == 2) return 
            foo->prob_2card(cards[0], cards[1]);
        if (n == 5)
            return foo->prob_5card(cards[0], cards[1], cards[2], cards[3], cards[4]);
        if (n == 6)
            return foo->prob_6card(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5]);
        if (n == 7)
            return foo->prob_7card(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]);
        return 0.0f;
    }

    uint8_t card2id(TexasSolver *foo, char *x)
    {
        return foo->card2id(x);
    }
}