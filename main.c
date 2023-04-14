#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

//enum suitType with the different suits
enum suitType{hearts, diamonds, spades, clubs};

//struct nodes holds cardValue, if its visible, the suit, a pointer to the next card in the pile
struct node {
    int cardValue;
    bool visible;
    enum suitType suit;
    struct node *next;
};

//struct linkedList (holds a pointer to the head of a node(card)
struct linkedList{
    struct node *head;
};


//Prints list (for debugging)
void printLinkedList(struct linkedList list){
    struct node *el = list.head;
    char suitNames[4][10] = {"hearts","diamonds","spades","clubs"};
    while(el != NULL){
        printf("(%d, %s)\n", el->cardValue, suitNames[el->suit]);
        el = el->next;
    }
}

//Inserts element in the beginning
void insert(struct linkedList *list, int cardValue, enum suitType suit, bool visible){
    struct node *newNodeLink = (struct node*) malloc(sizeof (struct node));
    newNodeLink->cardValue = cardValue;
    newNodeLink->suit = suit;
    newNodeLink->next = list->head;
    newNodeLink->visible = visible;
    list->head = newNodeLink;
}

void insertLast(struct linkedList *list, int cardValue, enum suitType suit, bool visible){
    struct node *newNodeLink = (struct node*) malloc(sizeof (struct node));
    newNodeLink->cardValue = cardValue;
    newNodeLink->suit = suit;
    newNodeLink->next = NULL;
    newNodeLink->visible = visible;
    if(list->head == NULL){
        list->head = newNodeLink;
    } else{
        struct node *lastElement = list->head;
        while(lastElement->next != NULL){
            lastElement = lastElement->next;
        }
        lastElement->next = newNodeLink;
    }
}

//Delete first (youngest) element
struct node* deleteFirst(struct linkedList *list){
    struct node *deletedElement = list->head;
    list->head = list->head->next;
    return deletedElement;
}

//Returns pointer to node(card) that matches card value and suit
struct node* findElement(struct linkedList list, int cardValue, enum suitType suit){
    struct node* el = list.head;

    if(list.head == NULL){
        return NULL;
    }

    while (el->cardValue != cardValue || el->suit != suit){
        if(el->next == NULL){
            return NULL;
        } else{
            el = el->next;
        }
    }
    return el;
}

//Deletes element that matches card value and suit
struct node* deleteElement(struct linkedList *list, int cardValue, enum suitType suit){
    struct node* el = list->head;
    struct node* prev = NULL;
    if(list->head == NULL){
        return NULL;
    }

    while (el->cardValue != cardValue || el->suit != suit){
        if(el->next == NULL){
            return NULL;
        } else{
            prev = el;
            el = el->next;
        }
    }
    if(el == list->head){
        list->head = list->head->next;
    } else{
        prev->next = el->next;
    }
    return el;



}
//7 linked lists one for each column.
//Maybe it would be better to just have one??
// But then it might make some operations a bit harder idk
struct linkedList A;
struct linkedList C1;
struct linkedList C2;
struct linkedList C3;
struct linkedList C4;
struct linkedList C5;
struct linkedList C6;
struct linkedList C7;

enum gamePhase{STARTUP, PLAY};

enum gamePhase phase = STARTUP;

//last command
char lastCommand[20];

//Status message changed by the handleInput
char* status;

// Takes a node struct (card) and changes a str to match the representation of the card.
// str is the pointer to the string. E.g. 7 of spades becomes 7S.
void cardToString(char *str,struct node *card){
    if(card == NULL){
        return;
    }

    if(!card->visible){
        str[0]='[';
        str[1] = ']';
        return;
    }

    switch (card->cardValue) {
        case 1:
            str[0] = 'A';
            break;
        case 10:
            str[0] = 'T';
            break;
        case 11:
            str[0] = 'J';
            break;
        case 12:
            str[0] = 'Q';
            break;
        case 13:
            str[0] = 'K';
            break;
        default:
            str[0] = card->cardValue + '0';
            break;

    }

    char suitNames[4] = {'H','D','S','C'};
    str[1] = suitNames[card->suit];


}


//Prints the game like in the project 2 pdf description.
void printGameBoard(){
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n");
    bool hasMoreCards = true;
    struct node *column1 = C1.head;
    struct node *column2 = C2.head;
    struct node *column3 = C3.head;
    struct node *column4 = C4.head;
    struct node *column5 = C5.head;
    struct node *column6 = C6.head;
    struct node *column7 = C7.head;
    int index = 0;

    while(hasMoreCards || index < 4){
        hasMoreCards = false;

        char c1txt[] = "  ";
        char c2txt[] = "  ";
        char c3txt[] = "  ";
        char c4txt[] = "  ";
        char c5txt[] = "  ";
        char c6txt[] = "  ";
        char c7txt[] = "  ";

        cardToString(&c1txt, column1);
        cardToString(&c2txt, column2);
        cardToString(&c3txt, column3);
        cardToString(&c4txt, column4);
        cardToString(&c5txt, column5);
        cardToString(&c6txt, column6);
        cardToString(&c7txt, column7);



        column1 = column1 == NULL ? NULL : column1->next;
        column2 = column2 == NULL ? NULL : column2->next;
        column3 = column3 == NULL ? NULL : column3->next;
        column4 = column4 == NULL ? NULL : column4->next;
        column5 = column5 == NULL ? NULL : column5->next;
        column6 = column6 == NULL ? NULL : column6->next;
        column7 = column7 == NULL ? NULL : column7->next;

        printf("%s\t%s\t%s\t%s\t%s\t%s\t%s", c1txt,c2txt,c3txt,c4txt,c5txt,c6txt,c7txt);
        if(index == 0){
            printf("\t\t[]\tF1");
        } else if(index == 1){
            printf("\t\t[]\tF2");
        } else if(index == 2){
            printf("\t\t[]\tF3");
        } else if(index == 3){
            printf("\t\t[]\tF3");
        }

        printf("\n");

        if(column1 != NULL ||
           column2 != NULL ||
           column3 != NULL ||
           column4 != NULL ||
           column5 != NULL ||
           column6 != NULL ||
           column7 != NULL
                ){hasMoreCards = true;}

        index++;
    }

    printf("\nLAST Command: %s\nMessage: %s", lastCommand, status);


}

//Reveals all cards in a column
void showColumn(struct linkedList *col){
    struct node *el = col->head;
    while (el != NULL){
        el->visible = true;
        el = el->next;
    }
}

//Reveals all cards
char *showCards(){
    showColumn(&C1);
    showColumn(&C2);
    showColumn(&C3);
    showColumn(&C4);
    showColumn(&C5);
    showColumn(&C6);
    showColumn(&C7);
    return "OK";
}


// Loads a file and puts the cards in the columns. First horizontal from left to right and then vertical from top to bottom.
// Right now they get inserted in reversed order (can be fixed by changing the insert() method)
char* loadCardDeck(char* name){

    if (access(name, F_OK) != 0) {
        //Implement create new deck if file doesn't exists
        return "Error: File doesn't exist";
    }

    FILE *filePointer = fopen(name, "r");
    char line[256];
    int cardIndex = 1;

    struct node *current = A.head;

    while (fgets(line, sizeof (line), filePointer)){
        struct node *newCard = (struct node*) malloc(sizeof (struct node));
        switch (line[0]) {
            case 'A':
                newCard->cardValue = 1;
                break;
            case 'T':
                newCard->cardValue = 10;
                break;
            case 'J':
                newCard->cardValue = 11;
                break;
            case 'Q':
                newCard->cardValue=12;
                break;
            case 'K':
                newCard->cardValue = 13;
                break;
            default:
                newCard->cardValue = line[0]-'0';
                break;
        }


        switch (line[1]) {
            case 'H':
                newCard->suit = hearts;
                break;
            case 'D':
                newCard->suit = diamonds;
                break;
            case 'S':
                newCard->suit = spades;
                break;
            case 'C':
                newCard->suit = clubs;
                break;
        }

        insertLast(&A, newCard->cardValue,newCard->suit, false);

        switch (cardIndex%7) {
            case 1:
                insertLast(&C1, newCard->cardValue, newCard->suit, false);
                break;
            case 2:
                insertLast(&C2, newCard->cardValue, newCard->suit, false);
                break;
            case 3:
                insertLast(&C3, newCard->cardValue, newCard->suit, false);
                break;
            case 4:
                insertLast(&C4, newCard->cardValue, newCard->suit, false);
                break;
            case 5:
                insertLast(&C5, newCard->cardValue, newCard->suit, false);
                break;
            case 6:
                insertLast(&C6, newCard->cardValue, newCard->suit, false);
                break;
            default:
                insertLast(&C7, newCard->cardValue, newCard->suit, false);
                break;

        }
        cardIndex += 1;

    }
    fclose(filePointer);
    return "OK";
}

// Should save the cards in the columns to a file
char* saveCardDeck(char* filename){
    return "NOT IMPLEMENTED";
}

char* splitCards(char* splitLine){
    int line;
    sscanf(splitLine, "%d", &line);
    for (int i = 0; i < line; ++i) {

    }
    return "test";
}

int columnIndexCalculation(int cardIndex){
    int index = cardIndex%7;
    if(index == 0){
        index = 6;
    } else{
        index-=1;
    }

    return index;
}

char* startPlayPhase(){


    struct node *newCard = A.head;
    if(newCard == NULL){
        return "Error: No deck loaded";
    }

    phase = PLAY;
    C1.head = NULL;
    C2.head = NULL;
    C3.head = NULL;
    C4.head = NULL;
    C5.head = NULL;
    C6.head = NULL;
    C7.head = NULL;

    int caps[] = {1,6,7,8,9,10,11};
    int cardAmounts[] = {0,0,0,0,0,0,0};

    int cardIndex = 1;
    do{
        if(cardIndex > 1){
            newCard= newCard->next;
        }
        while(cardAmounts[columnIndexCalculation(cardIndex)] >= caps[columnIndexCalculation(cardIndex)]){
            cardIndex +=1;
        }

        int columnIndex = columnIndexCalculation(cardIndex);

        switch (columnIndex) {
            case 0:
                insertLast(&C1, newCard->cardValue, newCard->suit, (cardAmounts[columnIndex] >= columnIndex));
                break;
            case 1:
                insertLast(&C2, newCard->cardValue, newCard->suit, (cardAmounts[columnIndex] >= columnIndex));
                break;
            case 2:
                insertLast(&C3, newCard->cardValue, newCard->suit, (cardAmounts[columnIndex] >= columnIndex));
                break;
            case 3:
                insertLast(&C4, newCard->cardValue, newCard->suit, (cardAmounts[columnIndex] >= columnIndex));
                break;
            case 4:
                insertLast(&C5, newCard->cardValue, newCard->suit, (cardAmounts[columnIndex] >= columnIndex));
                break;
            case 5:
                insertLast(&C6, newCard->cardValue, newCard->suit, (cardAmounts[columnIndex] >= columnIndex));
                break;
            default:
                insertLast(&C7, newCard->cardValue, newCard->suit, (cardAmounts[columnIndex] >= columnIndex));
                break;

        }
        cardAmounts[columnIndex] +=1;
        cardIndex += 1;

    } while (newCard->next != NULL);


    return "Game started";
}

// Ask user for command and handles (some of it)
int handleInput(){
    printf("\nINPUT > ");
    char in[20];
    gets(in);
    strcpy(lastCommand, in);
    char* comm = strtok(in, " ");


    if(strcmp(comm, "LD") == 0 && phase==STARTUP){
        status = loadCardDeck(strtok(NULL, " "));
    } else if(strcmp(comm, "SD") == 0 && phase==STARTUP){
        status = saveCardDeck(strtok(NULL, " "));
    } else if(strcmp(comm, "SW") == 0){
        status = showCards();
    }else if(strcmp(comm, "QQ") == 0 && phase==STARTUP){
        return  1;
    } else if(strcmp(comm, "P") == 0){
        status = startPlayPhase();
    }else if (strcmp(comm, "SL") == 0){
        status = splitCards(strtok(NULL, " "));
    } else{
        status = "Unknown command";
    }
    return 0;
}

void commandLoop(){
    bool quit = 0;
    while (!quit){
        printGameBoard();
        quit = handleInput();
    }


}


int main() {
    commandLoop();

    return 0;
}

