#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SUBJECTS 50
#define MAX_SUBJECT_LENGTH 20
struct Item;
struct Subject;


char* current;
int isValid=1;
char *actions[10];
int curaction;

void actionInitialize(){
    actions[0]="and";
    actions[1]="buy";
    actions[2]="sell";
    actions[3]="go";
    actions[4]="has";
    actions[5]="from";
    actions[6]="to";

}
int findAction(char *word){
    for (int i = 0; i < 7; i++) {
        int action_length = strlen(actions[i]);
        if (strncmp(actions[i], word, action_length) == 0) {
            if (word[action_length] == ' ') {
                return i;
            }
        }
    }
    return -1; 
}



char *nextWord(char * cur){
    while (*cur==' ')
    cur++;
    return cur;
}

int isValidChar(char c) {
    return isalpha(c) || c == '_';
}

struct subject* singleSubjectFinder(char* cur){
    int lengt = 0;


    while (cur[lengt] != '\0') {
        if (isValidChar(cur[lengt])) {         
            lengt++;
        }else if (cur[lengt]==' ' || cur[lengt]=='\0') {
            struct subject* newSubject;
            newSubject->length=lengt;
            newSubject->name = &cur[i-lengt];
            newSubject->next=malloc(sizeof(struct subject));

        }
    }


void sentenceFinder(char* sentence){
    char* current=sentence;
    curaction=-1;
    struct subject* subjects=parseSubjects(sentence,subjects);
    if(curaction==-1){
        return;
    }
    else if (curaction==1){
        struct item* items= parseItems();
        if(curaction==5){

        }

    }
    

}

struct item* parseItems();


struct subject* parseSubjects(char *sentence, struct subject* newSubject) {
    int i = 0;
    int lengt = 0;


    while (sentence[i] != '\0') {
        if (isValidChar(sentence[i])) {         
            lengt++;
            i++;
        } else if (sentence[i]==' ') {
            newSubject->length=lengt;
            newSubject->name = &sentence[i-lengt];
            newSubject->next=malloc(sizeof(struct subject));
            char* cur=nextWord(&sentence[i]);
            
            int act=findAction(cur);
            cur+=strlen(actions[act]);
            cur=nextWord(cur);

            if(act==0){
            newSubject->next = parseSubjects(cur,newSubject->next);

            return newSubject;
            }
            else if (act==-1)
            {
             isValid=0;
             return NULL;
            }
            else {
                curaction=cur;
                return newSubject;
            }
            

        }else
        {
            isValid=0;
            
        }
        
        
    }


    return 0;
}

int main() {
    actionInitialize();
    char sentence[] = "Gandalf and Aragon and Merlin buy 5 breads";
  
    struct subject* start = (struct subject*)malloc(sizeof(struct subject));






    start=parseSubjects(sentence,start);

    while(start!=NULL){
        for(int i=0 ; i<start->length;i++){
        printf("%c",*((start->name)+i));
        }
        start=start->next;
        printf("\n");

    }

    return 0;
}
