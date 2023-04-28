#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

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

void updateCardPiles();
bool checkWinCon();


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

void insertRandom(struct linkedList *list, int cardValue, enum suitType suit, bool visible){
    int post = rand() %52;
    struct node *el= list->head;
    struct node* previous= NULL;
    for (int i = 0; i < post && el != NULL; ++i) {
        previous=el;
        el=el->next;
    }
    struct node *newNodeLink = (struct node*) malloc(sizeof (struct node));
    newNodeLink->cardValue = cardValue;
    newNodeLink->suit = suit;
    newNodeLink->next = NULL;
    newNodeLink->visible = visible;

    newNodeLink->next=el;
    if (previous==NULL){
        list->head=newNodeLink;
    } else{
        previous->next=newNodeLink;
    }
}

//Delete first (youngest) element
struct node* deleteFirst(struct linkedList *list){
    struct node *deletedElement = list->head;
    list->head = list->head->next;
    return deletedElement;
}



//Returns pointer to node(card) that matches card value and suit
struct node* findElement(struct linkedList *list, int cardValue, enum suitType suit){
    struct node* el = list->head;

    if(list->head == NULL){
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


struct node* findLastElement(struct linkedList *list){
    struct node* el = list->head;
    if(el == NULL){
        return NULL;
    }

    while (el->next != NULL){
        el = el->next;
    }
    return  el;
}


//Delete from element (the given element and all the following)
struct node* deleteFrom(struct linkedList *list, int cardValue, enum suitType suit){
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
        list->head = NULL;
    } else{
        prev->next = NULL;
    }
    if(prev != NULL){
        prev->visible = true;
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
struct  linkedList pile1;
struct  linkedList pile2;
struct linkedList splitPile;
struct linkedList shufflePile;
struct linkedList C1;
struct linkedList C2;
struct linkedList C3;
struct linkedList C4;
struct linkedList C5;
struct linkedList C6;
struct linkedList C7;

struct linkedList F1;
struct linkedList F2;
struct linkedList F3;
struct linkedList F4;

enum gamePhase{STARTUP, PLAY, WIN};

enum gamePhase phase = STARTUP;

//last command
char lastCommand[20];

//Status message changed by the handleInput
char* status = "Welcome to Yukon Solitare!";

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

    char suitNames[4] = {3,4,6,5};
    str[1] = suitNames[card->suit];


}


//Prints the game like in the project 2 pdf description.
void printGameBoard(){
    printf("\n");
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n");
    printf("\n");

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
            if(F1.head == NULL){
                printf("\t\t[]\tF1");
            } else{
                char ftxt[] = "  ";
                cardToString(&ftxt, findLastElement(&F1));
                printf("\t\t%s\tF1", ftxt);
            }

        } else if(index == 1){
            if(F2.head == NULL){
                printf("\t\t[]\tF2");
            }else{
                char ftxt[] = "  ";
                cardToString(&ftxt, findLastElement(&F2));
                printf("\t\t%s\tF2", ftxt);
            }
        } else if(index == 2){
            if(F3.head == NULL){
                printf("\t\t[]\tF3");
            }else{
                char ftxt[] = "  ";
                cardToString(&ftxt, findLastElement(&F3));
                printf("\t\t%s\tF3", ftxt);
            }
        } else if(index == 3){
            if(F4.head == NULL){
                printf("\t\t[]\tF4");
            }else{
                char ftxt[] = "  ";
                cardToString(&ftxt, findLastElement(&F4));
                printf("\t\t%s\tF€", ftxt);
            }
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
    if(A.head == NULL){
        return "ERROR: No deck loaded";
    }
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

    FILE *filePointer;

    if (access(name, F_OK) != 0) {
        filePointer = fopen("DefaultDeck.txt", "r");
    } else {
        filePointer = fopen(name, "r");
    }


    char line[256];
    int cardIndex = 1;

    A.head = NULL;
    C1.head = NULL;
    C2.head = NULL;
    C3.head = NULL;
    C4.head = NULL;
    C5.head = NULL;
    C6.head = NULL;
    C7.head = NULL;


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

    FILE *fb;

    if (access(filename, F_OK) != 0) {
        fb = fopen("card.txt", "w");
    } else {
        fb = fopen(filename, "w");
    }

    struct node *el = A.head;

    while (el != NULL){
        switch (el->cardValue) {
            case 1:
                putc('A', fb);
                break;
            case 10:
                putc('T', fb);
                break;
            case 11:
                putc('J', fb);
                break;
            case 12:
                putc('Q', fb);
                break;
            case 13:
                putc('K', fb);
                break;
            default:
                fprintf(fb, "%d", el->cardValue);
                break;
        }
        switch (el->suit) {
            case hearts:
                putc('H',fb);
                break;
            case diamonds:
                putc('D',fb);
                break;
            case spades:
                putc('S',fb);
                break;
            case clubs:
                putc('C',fb);
                break;
        }
        putc('\n', fb); // Add a newline character
        el = el->next;
    }
    fclose(fb);
    return "Game saved";
}

char* splitCards(char* splitLine){
    if(A.head == NULL){
        return "Error: No deck loaded";
    }
    int line;
    sscanf(splitLine, "%d", &line);
    if(line == NULL){
        srand(time(NULL));
        line = rand() % 52 + 1;
    }
    if(line >= 52 || line <= 0){
        return "Error: The line where to split the deck should be between 1 and 51";
    }

    struct node *el = A.head;
    pile1.head = NULL;
    pile2.head = NULL;
    splitPile.head = NULL;

    for (int i = 0; i < line; ++i) {
        insert(&pile1,el->cardValue,el->suit,el->visible);
        el = el->next;
    }
    while (el != NULL){
        insert(&pile2,el->cardValue,el->suit,el->visible);
        el = el->next;
    }

    struct node *newEl = pile1.head;
    struct node *newEl2 = pile2.head;
    bool pileOne = true;
    while (newEl != NULL && newEl2 != NULL) {
        if (pileOne == true) {
        insert(&splitPile, newEl->cardValue, newEl->suit, newEl->visible);
        newEl = newEl->next;
        if (newEl2 != NULL) {
            pileOne = false;
        }
        } else {
            insert(&splitPile, newEl2->cardValue, newEl2->suit, newEl2->visible);
            newEl2 = newEl2->next;
            if (newEl != NULL) {
                pileOne = true;
            }
        }
    }
    if (newEl != NULL){
        while (newEl != NULL){
            insertLast(&splitPile, newEl->cardValue, newEl->suit, newEl->visible);
            newEl = newEl->next;
        }
    } else if (newEl2 != NULL){
        while (newEl2 != NULL){
            insertLast(&splitPile, newEl2->cardValue, newEl2->suit, newEl2->visible);
            newEl2 = newEl2->next;
        }
    }

    A.head = splitPile.head;
    updateCardPiles();
    return "Ok";
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

bool isMoveCommand(char* cm){
    int l = strlen(cm);
    if(l != 6 && l != 9){
        return false;
    }
    if(cm[2] != '-' || cm[3] != '>'){
        if(l != 9){
            return false;
        }
        if(cm[5] != '-' || cm[6] != '>'){
            return false;
        }
        if(cm[2] != ':'){
            return false;
        }

    } else {
        if(l != 6){
            return false;
        }
    }

    if(l == 6){
        if(cm[0] ==  cm[4] ){
            return false;
        }
    }
    return true;
}



char* moveCards(char* cm){
    int l = strlen(cm);

    struct linkedList *sourcePile;
    struct linkedList *targetPile;
    int from = cm[1] - '0';
    int to = cm[5] - '0';
    if(l == 9) {
        from = cm[1] - '0';
        to = cm[8] - '0';
    }
    switch (from) {
        case 1:
            sourcePile = &C1;
            if(cm[0] == 'F'){
                sourcePile= &F1;
            }
            break;
        case 2:
            sourcePile = &C2;
            if(cm[0] == 'F'){
                sourcePile= &F2;
            }
            break;
        case 3:
            sourcePile = &C3;
            if(cm[0] == 'F'){
                sourcePile= &F3;
            }
            break;
        case 4:
            sourcePile = &C4;
            if(cm[0] == 'F'){
                sourcePile= &F4;
            }
            break;
        case 5:
            sourcePile = &C5;
            break;
        case 6:
            sourcePile = &C6;
            break;
        default:
            sourcePile = &C7;
            break;
    }

    switch (to) {
        case 1:
            targetPile = &C1;
            if(cm[4] == 'F'){
                targetPile= &F1;
            }
            break;
        case 2:
            targetPile = &C2;
            if(cm[4] == 'F'){
                targetPile= &F2;
            }
            break;
        case 3:
            targetPile = &C3;
            if(cm[4] == 'F'){
                targetPile= &F3;
            }
            break;
        case 4:
            targetPile = &C4;
            if(cm[4] == 'F'){
                targetPile= &F4;
            }
            break;
        case 5:
            targetPile = &C5;
            break;
        case 6:
            targetPile = &C6;
            break;
        default:
            targetPile = &C7;
            break;
    }


    char desiredCard[2];



    struct node* fromCard;
    if(l == 9){
        enum suitType st;
        int cardValue = 0;
        desiredCard[0] = cm[3];
        desiredCard[1] = cm[4];
        switch (desiredCard[1]) {
            case 'H':
                st=hearts;
                break;
            case 'D':
                st=diamonds;
                break;
            case 'C':
                st=clubs;
                break;
            case 'S':
                st=spades;
                break;
        }
        switch (desiredCard[0]) {
            case 'A':
                cardValue = 1;
                break;
            case 'T':
                cardValue = 10;
                break;
            case 'J':
                cardValue = 11;
                break;
            case 'Q':
                cardValue=12;
                break;
            case 'K':
                cardValue = 13;
                break;
            default:
                cardValue = desiredCard[0]-'0';
                break;
        }
        fromCard = findElement(sourcePile, cardValue, st);
    } else{
        fromCard = findLastElement(sourcePile);
    }


    struct node* toCard = findLastElement(targetPile);

    if(fromCard == NULL || !fromCard->visible ){
        return "No such card in source pile!";
    } else if(toCard != NULL){
        if(l == 9){
            if(fromCard->cardValue != toCard->cardValue-1){
                return "Source card must be 1 lower that target card";
            }
            if(fromCard->suit ==  toCard->suit) {
                return "Suit types must be different!";
            }
        } else {
            if(cm[4] == 'F' && toCard != NULL){
                if(fromCard->suit != toCard->suit){
                    return "All cards in a foundation must have the same suit!";
                }

                if(fromCard->cardValue != toCard->cardValue+1){
                    return "Cards in a foundation must increment by one in rising order!";
                }

            } else{
                if(sourcePile->head != fromCard){
                    return "You can only move from a foundation, if the foundation only HAS one card!";
                }

                if(fromCard->cardValue != toCard->cardValue-1){
                    return "Source card must be 1 lower that target card";
                }
                if(fromCard->suit ==  toCard->suit) {
                    return "Suit types must be different!";
                }
            }
        }



    }

    struct node* card = deleteFrom(sourcePile,fromCard->cardValue,fromCard->suit);
    if(targetPile->head == NULL){
        targetPile->head = card;
    } else{
        struct node *current = targetPile->head;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = card;
    }
    if(!checkWinCon()) {
        return "Moved cards";
    } else {
      return "You won!!!";
    }
}

bool checkWinCon(){
    if(C1.head == NULL && C2.head == NULL && C3.head == NULL && C4.head == NULL && C5.head == NULL && C6.head == NULL && C7.head == NULL){
        phase = WIN;
        return true;
    }
    return false;
}

char* shuffleRandom(){
    if(A.head == NULL){
        return "Error: No deck loaded";
    }
    struct node *el=A.head;
    shufflePile.head = NULL;
    while(el!= NULL) {
        insertRandom(&shufflePile,el->cardValue,el->suit,el->visible);
        el=el->next;
    }
    A.head=shufflePile.head;
    updateCardPiles();
    return "Ok";
}

void updateCardPiles(){
    C1.head = NULL;
    C2.head = NULL;
    C3.head = NULL;
    C4.head = NULL;
    C5.head = NULL;
    C6.head = NULL;
    C7.head = NULL;

    struct node *el = A.head;
    int cardIndex = 1;
    while (el != NULL){
        switch (cardIndex%7) {
            case 1:
                insertLast(&C1, el->cardValue, el->suit, false);
                break;
            case 2:
                insertLast(&C2, el->cardValue, el->suit, false);
                break;
            case 3:
                insertLast(&C3, el->cardValue, el->suit, false);
                break;
            case 4:
                insertLast(&C4, el->cardValue, el->suit, false);
                break;
            case 5:
                insertLast(&C5, el->cardValue, el->suit, false);
                break;
            case 6:
                insertLast(&C6, el->cardValue, el->suit, false);
                break;
            default:
                insertLast(&C7, el->cardValue, el->suit, false);
                break;
        }
        el = el->next;
        cardIndex += 1;
    }
}

// Ask user for command and handles (some of it)
int handleInput(){
    printf("\nINPUT > ");
    char in[20];
    gets(in);
    strcpy(lastCommand, in);


    // STARTUP COMMANDS
    if(phase == STARTUP){
        char* comm = strtok(in, " ");
        if(strcmp(comm, "LD") == 0 ){
            status = loadCardDeck(strtok(NULL, " "));
        } else if(strcmp(comm, "SD") == 0 ){
            status = saveCardDeck(strtok(NULL, " "));
        } else if(strcmp(comm, "SW") == 0){
            status = showCards();
        }else if(strcmp(comm, "QQ") == 0 ){
            return  1;
        } else if(strcmp(comm, "P") == 0){
            status = startPlayPhase();
        }else if (strcmp(comm, "SL") == 0){
            status = splitCards(strtok(NULL, " "));
        }else if(strcmp(comm,"SR")==0) {
            status = shuffleRandom();
        }
        else{
            status = "Error: Unknown command in startup phase";
        }
    } else if(phase == PLAY || phase == WIN){ //PLAY COMMANDS
        if(isMoveCommand(in) && phase == PLAY) {
            status = moveCards(in);
        } else if(strcmp(in, "QQ") == 0 ){
            return  1;
        }else if(strcmp(in, "Q") == 0 ){
            phase = STARTUP;
            status = "Exited current game";
        } else {
            if (phase == PLAY) {
                status = "Error: Unknown command in play phase";
            } else{
                status = "Error: Unknown command in win phase";
            }
        }
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
    srand(time(NULL));
    commandLoop();

    return 0;
}

