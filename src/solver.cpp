#include "solver.h"

TexasSolver::TexasSolver()
{
    memset(mask, 0, 52);
};

void TexasSolver::set_number_players(int n)
{
    if (n < 2)
    {
        printf("Warning: the number of players should be between 2 to 6!\n");
        n_players = 2;
    } else
    {
        if (n > 6)
        {
            printf("Warning: the number of players should be between 2 to 6!\n");
            n_players = 6;
        } else
        {
            n_players = n;
        }
    }
};

int TexasSolver::hash_quinary(const unsigned char q[], int k)
{
    int sum = 0;
    for (int i = 0; i < 13; i++)
    {
        sum += rank_dp[q[i]][13 - i - 1][k];
        k -= q[i];
        if (k <= 0)
            break;
    }
    return sum;
}

int TexasSolver::rank_7card(uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4, uint8_t c5, uint8_t c6, uint8_t c7)
{
    int suit_hash = 0;
    suit_hash += suitbit_by_id[c1];
    suit_hash += suitbit_by_id[c2];
    suit_hash += suitbit_by_id[c3];
    suit_hash += suitbit_by_id[c4];
    suit_hash += suitbit_by_id[c5];
    suit_hash += suitbit_by_id[c6];
    suit_hash += suitbit_by_id[c7];

    if (suits[suit_hash])
    {
        int suit_binary[4] = {0};
        suit_binary[c1 & 0x3] |= binaries_by_id[c1];
        suit_binary[c2 & 0x3] |= binaries_by_id[c2];
        suit_binary[c3 & 0x3] |= binaries_by_id[c3];
        suit_binary[c4 & 0x3] |= binaries_by_id[c4];
        suit_binary[c5 & 0x3] |= binaries_by_id[c5];
        suit_binary[c6 & 0x3] |= binaries_by_id[c6];
        suit_binary[c7 & 0x3] |= binaries_by_id[c7];

        return flush[suit_binary[suits[suit_hash] - 1]];
    }

    unsigned char quinary[13] = {0};
    ++quinary[(c1 >> 2)];
    ++quinary[(c2 >> 2)];
    ++quinary[(c3 >> 2)];
    ++quinary[(c4 >> 2)];
    ++quinary[(c5 >> 2)];
    ++quinary[(c6 >> 2)];
    ++quinary[(c7 >> 2)];
    const int hash = hash_quinary(quinary, 7);

    return no_flush7[hash];
}

double TexasSolver::prob_7card(uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4, uint8_t c5, uint8_t c6, uint8_t c7)
{
    memset(mask, 0, 52);
    mask[c1] = true;
    mask[c2] = true;
    mask[c3] = true;
    mask[c4] = true;
    mask[c5] = true;
    mask[c6] = true;
    mask[c7] = true;

    std::vector<uint8_t> numbers;
    for (int i = 0; i < 52; ++i)
    {
        if (!mask[i])
            numbers.push_back(i);
    }

    int rank_hand = rank_7card(c1, c2, c3, c4, c5, c6, c7);

    int suit_hash_public = 0;
    suit_hash_public += suitbit_by_id[c3];
    suit_hash_public += suitbit_by_id[c4];
    suit_hash_public += suitbit_by_id[c5];
    suit_hash_public += suitbit_by_id[c6];
    suit_hash_public += suitbit_by_id[c7];

    int suit_binary_public[4] = {0};
    suit_binary_public[c3 & 0x3] |= binaries_by_id[c3];
    suit_binary_public[c4 & 0x3] |= binaries_by_id[c4];
    suit_binary_public[c5 & 0x3] |= binaries_by_id[c5];
    suit_binary_public[c6 & 0x3] |= binaries_by_id[c6];
    suit_binary_public[c7 & 0x3] |= binaries_by_id[c7];

    unsigned char quinary_public[13] = {0};
    ++quinary_public[(c3 >> 2)];
    ++quinary_public[(c4 >> 2)];
    ++quinary_public[(c5 >> 2)];
    ++quinary_public[(c6 >> 2)];
    ++quinary_public[(c7 >> 2)];

    int n_win = 0, n_tie = 0;
    for (int i = 0; i < 45; ++i)
    {
        int c8 = numbers[i];
        int suit_hash = suit_hash_public + suitbit_by_id[c8];

        int suit_binary[4];
        for (int k = 0; k < 4; ++k)
            suit_binary[k] = suit_binary_public[k];
        suit_binary[c8 & 0x3] |= binaries_by_id[c8];

        unsigned char quinary[13];
        for (int k = 0; k < 13; ++k)
            quinary[k] = quinary_public[k];
        ++quinary[(c8 >> 2)];

        for (int j = i + 1; j < 45; ++j)
        {
            int c9 = numbers[j];
            suit_hash += suitbit_by_id[c9];
            int suit_binary_tmp = suit_binary[c9 & 0x3];
            suit_binary[c9 & 0x3] |= binaries_by_id[c9];
            ++quinary[(c9 >> 2)];

            int rank;
            if (suits[suit_hash])
            {
                rank = flush[suit_binary[suits[suit_hash] - 1]];
            }
            else
            {
                const int hash = hash_quinary(quinary, 7);
                rank = no_flush7[hash];
            }
            if (rank_hand < rank)
                ++n_win;
            if (rank_hand == rank)
                ++n_tie;

            suit_hash -= suitbit_by_id[c9];
            suit_binary[c9 & 0x3] = suit_binary_tmp;
            --quinary[(c9 >> 2)];
        }
    }

    return get_prob(n_win, n_tie);
}

double TexasSolver::prob_6card(uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4, uint8_t c5, uint8_t c6)
{
    memset(mask, 0, 52);
    mask[c1] = true;
    mask[c2] = true;
    mask[c3] = true;
    mask[c4] = true;
    mask[c5] = true;
    mask[c6] = true;

    double prob = 0.0f;
    for (uint8_t card = 0; card < 52; ++card)
    {
        if (!mask[card])
        {
            prob += prob_7card(c1, c2, c3, c4, c5, c6, card);
        }
    }
    return prob / 46;
}

double TexasSolver::prob_5card(uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4, uint8_t c5)
{
    memset(mask, 0, 52);
    mask[c1] = true;
    mask[c2] = true;
    mask[c3] = true;
    mask[c4] = true;
    mask[c5] = true;
    std::vector<uint8_t> numbers;
    for (uint8_t k = 0; k < 52; ++k)
    {
        if (!mask[k])
            numbers.push_back(k);
    }
    double prob = 0.0f;
    for (int i = 0; i < 47; ++i)
    {
        for (int j = i + 1; j < 47; ++j)
        {
            prob += prob_7card(c1, c2, c3, c4, c5, numbers[i], numbers[j]);
        }
    }
    return prob / 1081;
}

double TexasSolver::prob_2card(uint8_t c1, uint8_t c2)
{
    if ((c1 & 0x3) == (c2 & 0x3))
    {
        return flush2[n_players - 2][c1 >> 2][c2 >> 2];
    }
    return no_flush2[n_players - 2][c1 >> 2][c2 >> 2];
}

uint8_t TexasSolver::card2id(char *x)
{   
    std::string card = x;
    if (card.length() != 2)
    {
        printf("Invalid card! It should be two chars\n");
        return 53;
    }
    if (rankMap.find(card[0]) == rankMap.end())
    {
        printf("Invalid card! The first char should be in {23456789TJQKA}\n");
        return 53;
    }
    if (suitMap.find(card[1]) == suitMap.end())
    {
        printf("Invalid card! The second char should be in {CDHScdhs}\n");
        return 53;
    }
    return rankMap.at(card[0]) * 4 + suitMap.at(card[1]);
}