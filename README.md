# Texas Hold'em Poker Calculator

For Texas Hold'em poker, it is important to calculate the probability to win based on observed cards. This project

- calculates the exact probability of two player game.
- calculates the approximated probability of multiple player game.
- for any card type, the calculation is done in real time.

## Solution of two players

The game has 4 phases:
- $S_2$: 2 private cards
- $S_5$: 2 private cards and 3 public cards
- $S_6$: 2 private cards and 4 public cards
- $S_7$: 2 private cards and 5 public cards

Denote $p(S)$ as the probability to win given $S$: 
$$p = \frac{win + 0.5 \times tie}{total}$$
We can calculate $p(S_7)$ via brute force search very fast. Then we can calculate $p(S_5)$ and $p(S_6)$ based on the results of $p(S_7)$ in real time (about 0.01s). Calculating $p(S_2)$ takes several minutes. However, the number of card types of $S_2$ is small. So we can pre-compute and store $p(S_2)$ in a table.

## Solution of multiple players

For multiple players, it is difficult to calculate $p(S_7)$ via brute force search. Instead, this project treats whether to win each opponent as an independent event. 
$$\hat{p}(S_7) = p^{n-1}(S_7)$$
where $n$ is the number of players. $\hat{p}(S_7)$ is usually smaller than the true probability. This project adjusts the equation to 
$$\hat{p}(S_7) = p^{(n-1)^{0.85}}(S_7)$$ 
Then $\hat{p}(S_5)$ and $\hat{p}(S_6)$ are calculated based on it. Again, we pre-compute and store $\hat{p}(S_2)$ in a table.

## How to use
First compile the code
```
./make.sh
```
Then run the script
```
python3 solver.py
```
Here is an example
```
Input the number of players: 2
Input your cards one by one: As Ac 
Your probability to win is:  85.20%

Input the number of players: 2
Input your cards one by one: 9c 4c 4s 9d 4h 2c 9h 
Your probability to win is:  97.78%
```

Each card has two chars: 
- the first one for ranks: `{23456789TJQKA}`
- the second one for suits: `{cdhs}`.

The first two cards are private cards while the others are public cards.

## Thanks
This project is built at the top of [PokerHandEvaluator](https://github.com/HenryRLee/PokerHandEvaluator). It provides high performance code for 7 cards ranking 7.