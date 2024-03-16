#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define st struct


// necessary data types

int isValid = 1;
char* position;


char **itemList;
int maxItemCount = 100;
int itemCount = 0;

char **locationList;
int maxLocationCount = 100;
int locationCount = 1;


char **subjectList;
int maxSubjectCount = 100;
int subjectCount = 0;

typedef st Item{
    char* start;
    int length;
    char* quantity;
    int quantityLength;
    st Item* next;
};


typedef st Subject{
    char* start;
    int length;
    st Subject* next;   

};

typedef st Action{
    int actionType;

    int* subjectList;
    int locationIndex;
    st Item* itemList;
    st Subject* objectList;

    
};


typedef st Condition{
    int conditionType;

    int* subjects;
    int locationIndex;
    st Item* itemList;

    st Condition* next;
};


typedef st Sentence{
    st Action* actionList;
    st Condition* conditionList;
};


// basic functions

int findItemIndex(char* name){
    for (int i = 0; i < itemCount; i++){
        if (strcmp(name, itemList[i]) == 0){
            return i;
        }
    }

    itemList[itemCount] = name;
    itemCount++;

    return itemCount-1;
}

int findLocationIndex(char* name){
    for (int i = 0; i < maxLocationCount; i++){
        if (strcmp(name, *(locationList + i)) == 0){
            return i;
        }
    }

    return -1;
}

int findSubjectIndex(char* name){
    for (int i = 0; i < maxSubjectCount; i++){
        if (strcmp(name, *(subjectList + i)) == 0){
            return i;
        }
    }

    return -1;
}


st Item *constructItem(char* start, int length, char* quantity, int quantityLength){
    st Item *item = (st Item*) malloc(sizeof(st Item));
    item->start = start;
    item->length = length;
    item->quantity = quantity;
    item->quantityLength = quantityLength;
    item->next = NULL;

    return item;
}

st Subject *constructSubject(char* start, int length){
    st Subject *subject = (st Subject*) malloc(sizeof(st Subject));
    subject->start = start;
    subject->length = length;

    return subject;
}


st Action *constructAction(int actionType, int* subjectList, int locationIndex, st Item* itemList){
    st Action *action = (st Action*) malloc(sizeof(st Action));
    action->actionType = actionType;
    action->subjectList = subjectList;
    action->locationIndex = locationIndex;
    action->itemList = itemList;

    return action;
}

st Condition *constructCondition(int condiitonType, int* subjects, int locationIndex, st Item* itemList){
    st Condition *condition = (st Condition*) malloc(sizeof(st Condition));
    condition->conditionType = condiitonType;
    condition->subjects = subjects;
    condition->locationIndex = locationIndex;
    condition->itemList = itemList;

    return condition;
}

st Sentence *constructSentence(st Action* actionList, st Condition* conditionList){
    st Sentence *sentence = (st Sentence*) malloc(sizeof(st Sentence));
    sentence->actionList = actionList;
    sentence->conditionList = conditionList;

    return sentence;
}


// complex functions

int isValidChar(char c) {
    return isalpha(c) || c == '_';
}

int isValidWord(char* word);

void nextWord(){
    while (*(position) == ' '){
        position++;
    }
}

int getWord(int type){ // 0 check, 1 integer
    int i = 0;
    while (*(position + i) != ' ' && *(position + i) != '\0' ){
        char c = *(position + i);
        if ((type == 0 && isValidChar(c) == 0) || (type == 3 && isdigit(c) == 0)){
            isValid = 0;
            return -1;
            
        }
        i++;
    }

    return i;
}

st Subject *parseSubject();

st Item *parseItem(){
    nextWord();

    char* quantity = position;
    int quantityLength = getWord(1);
    if (quantityLength == -1) { return NULL; }
    position += quantityLength;

    nextWord();
    char* start = position;
    int length = getWord(0);
    // isValidWord(name);
    position += length;
    
    st Item *item = constructItem(start, length, quantity, quantityLength);
    
    if (*position == '\0') { return item; }

    nextWord();
    int check = getWord(0);


    if (strncmp("and", position, check) == 0){
        position += check;
        nextWord();

        item->next = itemParser();
    }

    return item;

}

st Action *parseAction();

st Condition *parseCondition(){
    nextWord();

    st Condition* condition;
    
    st Subject* subjects = parseSubject();

    nextWord();
    int clause = getWord(0);
    if (clause == -1) {
        isValid = 0;
        return NULL; 
    }

    int conditionType = -1;
    if (strncmp("at", position, clause) == 0){
        position += clause;
        nextWord();
        int location = getWord(0);
        if (location == -1) {
            isValid = 0;
            return NULL;
        }
        position += location;

        st Condition* condition = constructCondition(0, subjects, findLocationIndex(location), NULL);
    }
    else if (strcmp(clause, "has") == 0){
        position += clause;
        nextWord();

        int conditionType = 1;

        int otherWord = getWord(0);
        if (strncmp("less", position, otherWord) == 0){
            conditionType = 2;
            position += otherWord;
            nextWord();

            int than = getWord(0);
            if (strncmp("than", position, than) != 0){
                isValid = 0;
                return NULL;
            }
            position += than;
        }
        else if (strncmp("more", position, otherWord) == 0){
            conditionType = 3;
            position += otherWord;
            nextWord();

            int than = getWord(0);
            if (strncmp("than", position, than) != 0){
                isValid = 0;
                return NULL;
            }
            position += than;
        }

        st Item* items = parseItem();
        if (items == NULL){
            isValid = 0;
            return NULL;
        }

        st Condition* condition = constructCondition(conditionType, subjects, -1, items);
    }

    nextWord();
    int and = getWord(0);
    if (and == -1 || strncmp("and", position, and) != 0){
        isValid = 0;
        return NULL;
    }

    char* initialPosition = position; 
    parseSubject();
    nextWord();

    int temp = getWord(0);
    if (temp == -1){
        isValid = 0;
        return NULL;
    }
    
    if (strncmp("at", position, temp) == 0 || trncmp("has", position, temp) == 0){
        position = initialPosition;

        condition->next = parseCondition();
    }

    return condition;
    
}




int main(){

    itemList = (char**) malloc(maxItemCount*sizeof(char*));
    subjectList = (char**) malloc(maxSubjectCount*sizeof(char*));
    locationList = (char**) malloc(maxLocationCount*sizeof(char*));
    strcpy(locationList, "NOWHERE");

    char* string = "Osman has 10 ekmek";
    position = string;
    
    st Condition* condition = parseCondition();

    if (condition == NULL){
        printf("%s\n", "INVALID");
    }

    else {
        while (condition != NULL){
            st Subject* subjects = condition->subjects;
        }
    } 
    


   

    return 0;

}



