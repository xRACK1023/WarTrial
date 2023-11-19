#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//the struct represent the card stacks.
struct cardstack {
    int card[52];
    int suit[52];
};

void createNewDeck(cardstack* deck) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 13; j++) {
            deck->card[i * 13 + j] = j;
            deck->suit[i * 13 + j] = i;
            //printf("card %i, suit %i\n", fullDeck.card[i * 13 + j], fullDeck.suit[i * 13 + j]);
        }
    }

//function that moves cards from both player and machine to and from the discard pile
void moveCard(struct cardstack* toStack, int* toDepth, struct cardstack* fromStack, int* fromDepth) {
    //toStack is the discard pile of cards from players
    //toDepth how many cards are in the discard pile
    //fromStack is the cards from the players
    //fromDepth is how many cards are in the players pile
    toStack->card[*toDepth] = fromStack->card[0];
    toStack->suit[*toDepth] = fromStack->suit[0];
    *toDepth = *toDepth + 1;    
    for (int i = 0; i < *fromDepth; i++) {
      fromStack->card[i] = fromStack->card[i + 1];
      fromStack->suit[i] = fromStack->suit[i + 1];
    }
    *fromDepth = *fromDepth - 1;
    return;
}


//function that prints out the card value and face for user and machine
void showCard(int card, int suit) {
    switch (card) {
    case 0: printf("Deuce of ");
        break;
    case 1: printf("Three of ");
        break;
    case 2: printf("Four of ");
        break;
    case 3: printf("Five of ");
        break;
    case 4: printf("Six of ");
        break;
    case 5: printf("Seven of ");
        break;
    case 6: printf("Eight of ");
        break;
    case 7: printf("Nine of ");
        break;
    case 8: printf("Ten of ");
        break;
    case 9: printf("Jack of ");
        break;
    case 10: printf("Queen of ");
        break;
    case 11: printf("King of ");
        break;
    case 12: printf("Ace of ");
        break;
    }
    switch (suit) {
    case 0: printf("Clubs");
        break;
    case 1: printf("Spades");
        break;
    case 2: printf("Hearts");
        break;
    case 3: printf("Diamonds");
        break;
    }
}
 
int main() {
    int i, j, pointer, userStackDepth = 0, machineStackDepth = 0, pileStackDepth = 0;
    int tempCard, tempSuit, done;
    
    char temp, userReply;

    const int True = 1, False = 0;

    //structures the card deck. numbers 0-12 represent vales of card. Jack, Queen and King are numbers 10, 11, 12.
    //suits represent 0-3. hearts = 0, diamonds = 1, spades = 2, clubs = 3
    //making one for the user, maching, deck of cards, and the comparison pile
    cardstack userStack, machineStack, pileStack, fullDeck;

    printf("hello, lets play war\n");
    //loads new deck of cards into the game
    createNewDeck(&fullDeck);

    //lets shuffle, Pulls random card from new deck in to random spot in the deck.
    //card gets pulled using pointer in to a temp slot, from there random card gets
    //inserted to location in deck (i). This is reapeated for all 52 cards, 10 times. 
    srand(time(0));
    for (j = 0; j < 10; j++) { 
        for (i = 0; i < 52; i++) {
            pointer = rand() % 52;

            tempCard = fullDeck.card[pointer];
            tempSuit = fullDeck.suit[pointer];

            fullDeck.card[pointer] = fullDeck.card[i];
            fullDeck.suit[pointer] = fullDeck.suit[i];

            fullDeck.card[i] = tempCard;
            fullDeck.suit[i] = tempSuit;
        }
    }

    //Dealing. Card from the shuffled full deck is pulled and placed into either the user or machine
    //deck. User and machine alternates getting cards from shuffled deck. 
    for (i = 0; i < 26; i++) {
        userStack.card[i] = fullDeck.card[i * 2];
        userStack.suit[i] = fullDeck.suit[i * 2];
        machineStack.card[i] = fullDeck.card[(i * 2) + 1];
        machineStack.suit[i] = fullDeck.suit[(i * 2) + 1];
    }
    userStackDepth = 26;
    machineStackDepth = 26;

    // prints out all cards both user and machine have, debug
    /*for (i = 0; i < userStackDepth; i++)
        printf("card %i, suit %i\n", userStack.card[i], userStack.suit[i]);
        */

    //play game

    done = False; //loop keeps going until done playing game

    while (done == False) {
        printf("The user has: %i cards, and the machine has %i cards\n", userStackDepth, machineStackDepth);
        //gets top card from user, and places card into pile
        printf("Player card is:  "); showCard(userStack.card[0], userStack.suit[0]);
        printf("\n");
        moveCard(&pileStack, &pileStackDepth, &userStack, &userStackDepth);

        //gets top card from machine and places card in to pile
        printf("machine card is:  "); showCard(machineStack.card[0], machineStack.suit[0]);
        printf("\n");
        moveCard(&pileStack, &pileStackDepth, &machineStack, &machineStackDepth);

        //compares both user and machine card to see which one is higher, card value only
        if (pileStack.card[pileStackDepth - 1] > pileStack.card[pileStackDepth - 2]) {
            //Machine won, moves cards in pile to the bottom of machines deck
            printf("Machine won\n\n");
            for (i = 0; i < pileStackDepth; i++) {
                machineStack.card[machineStackDepth] = pileStack.card[i];
                machineStack.suit[machineStackDepth] = pileStack.suit[i];
                machineStackDepth++;
            }
            pileStackDepth = 0;
        }
        else if (pileStack.card[pileStackDepth - 1] < pileStack.card[pileStackDepth - 2]) {
            printf("Player won\n\n");
            //player won, moves card into pile to the bottom of players deck
            for (i = 0; i < pileStackDepth; i++) {
                userStack.card[userStackDepth] = pileStack.card[i];
                userStack.suit[userStackDepth] = pileStack.suit[i];
                userStackDepth++;
            }
            pileStackDepth = 0;
        }
        else {
            //both player and machine cards are equal
            printf("Tie\n\n");
            if (userStackDepth < 4) {
                //if in a tie, player has less than 4 cards, can not compete in "war" machine wins
                printf("MACHINE WINS THE GAME!!\n\n");
                done = True;
            }
            else if (machineStackDepth < 4) {
                //if in a tie, machine has less than 4 cards, can not compete in "war" player wins
                printf("PLAYER WINS THE GAME!!\n\n");
                done = True;
            }
            else {
                //moves 3 cards from the user stack of cards in to the pile stack.
                for (j = 0; j < 3; j++) {
                    pileStack.card[pileStackDepth] = userStack.card[0];
                    pileStack.suit[pileStackDepth] = userStack.suit[0];
                    pileStackDepth++;
                    //moves up all cards in the players hand. 
                    for (i = 0; i < userStackDepth; i++) {
                        userStack.card[i] = userStack.card[i + 1];
                        userStack.suit[i] = userStack.suit[i + 1];
                    }
                    userStackDepth--;
                    //moves 3 cards from the machine stack of cards in to the pile stack
                    pileStack.card[pileStackDepth] = machineStack.card[0];
                    pileStack.suit[pileStackDepth] = machineStack.suit[0];
                    pileStackDepth++;
                    //moves up all cards in the machines hand
                    for (i = 0; i < machineStackDepth; i++) {
                        machineStack.card[i] = machineStack.card[i + 1];
                        machineStack.suit[i] = machineStack.suit[i + 1];
                    }
                    machineStackDepth--;
                }
            }
        }
        //when either player or machine stack as reached 0, game over
        if (userStackDepth == 0 || machineStackDepth == 0)
            done = True;

        if (done == False){
            printf("Play another round? (Y/N)\n");
            scanf(" %c", &userReply);
            
        }
    }
    printf("goodbye\n");

    return 0;
}