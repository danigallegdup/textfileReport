/* Daniela Gallegos Dupuis : V00942356
 * process_cal3.c
 * Starter file provided to students for Assignment #3, SENG 265,
 * Fall 2021.

    gcc d_a3.c -o d_a3 
    ./d_a3 --start=1234/01/12 --end=2134/12/11 --file=ics/one.ics
     valgrind --leak-check=yes ./d_a3  --start=1234/01/12 --end=2134/12/11 --file=ics/one.ics 2&> temp.txt


 valgrind --leak-check=yes ./d_a3 --start=2021/4/18 --end=2021/4/21 --file=ics/diana-devops.ics 2&> temp.txt
     
    It is very unpredictable!!! 
    conclusion I do not know what I am doing : I do not know enough to know why this fails

    - labs 8
    - lab 7

    learn form them: 
    - 

 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emalloc.h"
#include "ics.h"
#include "listy.h"
#include <time.h>

struct recipes{
    char event[300];
    char  date[50];
    char  time[50];
    char  summary[50];
    char  location[50];
    char  rule[200];
    int start_time; //
    long int  start_date; // 20210304 
    long int until;
}recipes;

struct Node {
    struct Node* first;
    struct Node* last;
	struct recipes data;
	struct Node* next;
};

//part 1 : given code: finds filename and calls IntoString
int sortArugments(int argc, char *argv[], char *buffer);
int intoString (char *filename, char *buffer);

void printList(struct Node* n);
struct recipes create_recipes(char *buffer, int startingindex, int i); 
void push(struct Node * head, struct recipes one);
int linkedlist(char *buffer, int noc, struct Node* head);
void fill_out_ll(struct Node* head);

int find_key_words(char *event, char *word, char *newWord);
void summary(struct Node *head);
void location(struct Node *head);
void date(struct Node * head);
void start_date(struct Node * head);
void get_time(struct Node * head);
void convertTime(struct Node * head);
void to_standard_time(int military_time,   char *start_time);
void fill_properties(struct Node * head);
int rrule( struct Node * head);
int isRule(char *word,char *buffer);
long int until_date(struct Node * head);

void dt_increment(char *after, const char *before, int const num_days);
int manage_rule(struct Node * head, struct recipes rep);
struct recipes create_recurring_event( struct Node * head,  struct recipes rep);
long int start_date_atoi(char *date );
struct Node pushRR(struct Node * head, struct recipes val) ;

void bubbleSort(struct Node * head, int n);
void swap(struct recipes *xp, struct recipes *yp);
long int  arg__end(int argc, char *argv[]);
long int  arg__start(int argc, char *argv[]);

void dt_format(char *formatted_time, const char *dt_time, const int len);
void printEvent(struct Node * head);
void manage_printing(struct Node * head, int newE, long int start_range, long int end_range);
void printEvent_same_day(struct Node * head);

int main(int argc, char *argv[]){
    // file content into buffer
    char buffer[3500];
    int numofchar;
    numofchar = sortArugments(argc, argv, buffer);
    //printf("\n%s\n",buffer);

    struct Node* head = NULL;
    struct Node* temp = NULL;
    head = (struct Node*)malloc(sizeof(struct Node));
    head->next = NULL;

    int events = linkedlist(buffer, numofchar, head);
    //push(head, head->data );
    temp = head;
    head = head->next;
    free(temp);


    // the very end
    long int arg_start=  arg__start(argc, argv);
    long int arg_end = arg__end(argc, argv);
    //printf("%ld %ld\n", arg_start, arg_end);
    
  


    struct Node* n = head;
    int i =0;
	while (n != NULL) {
        if(n->data.until > 0){
             manage_rule(n, n->data);
        }
		n = n->next;    
        i++;
	}

    bubbleSort(head,i);
     manage_printing( head, 2, arg_start, arg_end);


    struct Node *temp_n = NULL;
    for(; head!= NULL; head = temp_n){
        temp_n = head->next;
        free(head);

    } 

   
    exit(0);
}


///////////////////// Part 7: formatting date and printing calender as assignment instucted ////////////////////

/*
* in:  all, newE, start_range,  end_range
* out: if new day - print with date - if same dat without date
* calls: printEvent(struct recipes event, printEvent_same_day(struct recipes event)
* purpose: prints out properly

store the previous date!!!
*/
void manage_printing(struct Node * head, int newE, long int start_range, long int end_range){
   int first = 0;
   struct Node* n = head;
	while (n != NULL) {  
        //printf("%ld, %ld\n",n->data.start_date, start_range);
        if(n->data.start_date >= start_range && n->data.start_date<= end_range){
         if (first > 0){
            printf("\n");
         }   
         first++;
		 dt_format(n->data.date, n->data.date, 50); 
         printEvent(n);
        }
		n = n->next;   
       
	}
}

/*
* in:  struct recipes event - all[i]
* out: date - time and discription
* calls: ---
* purpose: format date properly
*/
void printEvent(struct Node * head){
    int len = strlen(head->data.date);
    len = len;
    //printf("%d", len);

    char line[len];
    memset(line,'\0',30);
    for(int i =0; i < len; i++){
        line[i] = '-';
    }
    // printf("\n%s\n %ld\n\n",head->data.event, head->data.start_date);
    printf("%s\n%s\n",head->data.date,line);
    printf("%s %s {{%s}}\n",head->data.time,head->data.summary,head->data.location); 
}

/*
* in:  all[i].date, all[i].date, 50)
* out: DTSTART:20210214T180000 -> February 14, 2021 (Sun) 
* calls: ---
* purpose: format date properly
*/
void dt_format(char *formatted_time, const char *dt_time, const int len){
    struct tm temp_time;
    time_t    full_time;

    memset(&temp_time, 0, sizeof(struct tm));
    sscanf(dt_time, "%4d%2d%2d",
        &temp_time.tm_year, &temp_time.tm_mon, &temp_time.tm_mday);
    temp_time.tm_year -= 1900;
    temp_time.tm_mon -= 1;
    full_time = mktime(&temp_time);
    strftime(formatted_time, len, "%B %d, %Y (%a)", 
        localtime(&full_time));
}


/*
* in:  struct recipes event - all[i]
* out: -time and discription
* calls: ---
* purpose: format date properly
*/
void printEvent_same_day(struct Node * head){
    printf("%s %s {{%s}}\n",
    head->data.time,
    head->data.summary,
    head->data.location); 
}


///////////////////// Part 6:range dates start and end//// //////////////////////////////////////////////////
/*
* in:  int argc, char *argv[])
* out:  --- date specified in args - long int  - similar to  start_date
* calls:  ---
* purpose: get start_range
*/
long int  arg__start(int argc, char *argv[]){
    long int start_date = 0;
    int from_y = 0, from_m = 0, from_d = 0;
     
    for (int i = 0; i < argc; i++) {
        if (strncmp(argv[i], "--start=", 8) == 0) {
            sscanf(argv[i], "--start=%d/%d/%d", &from_y, &from_m, &from_d);
        } 
    } 
    start_date = (from_y* 10000) +start_date;
    start_date = start_date + (from_m*100);
    start_date =start_date+ from_d;
    
    return start_date;  
}

/*
* in:  int argc, char *argv[]
* out:  --- date specified in args - long int  - similar to  start_date
* calls: --
* purpose: get end_range
*/
long int  arg__end(int argc, char *argv[]){
    long int end_date=0;
    int to_y = 0, to_m = 0, to_d = 0;
    for (int i = 0; i < argc; i++) {
        if (strncmp(argv[i], "--end=", 6) == 0) {
            sscanf(argv[i], "--end=%d/%d/%d", &to_y, &to_m, &to_d);
        } 
    } 

     end_date = (to_y* 10000) +end_date;
    end_date = end_date + (to_m*100);
    end_date =end_date+ to_d;
    
    // printf("%ls %d%d%d \n", to_y, to_m, to_d);
      return end_date;
}

///////////////////// Part 5: sorting array all by date and time is earliest to latest /////////////////////////////////////


/*
* in:  all, int i 
* out: sortts all[] ealiest to latest
* calls:  swap(struct recipes *xp, struct recipes *yp)
* purpose: acknowledgment that this is not the best sorting aglorthmn but the easiest to implement

- *** find the length of list: make a function that can do that
node for i 
node for j 

    refresh after inner for loop

*/
void bubbleSort(struct Node * head, int n){
    int i, j;
    struct Node * jay = head;
    for (i = 0; i < n-1; i++){
       for (j = 0; j < n-i-1; j++){
            if (jay->data.start_date > jay->next->data.start_date){
                swap(&jay->data, &jay->next->data);
            }
            if(jay->data.start_date == jay->next->data.start_date &&
                 jay->data.start_time > jay->next->data.start_time){
                
                swap(&jay->data,&jay->next->data);
            } 
            jay = jay->next;
        }  
       jay= head;
    }            
}

/*
* in: 2 pointer variables holding struct recipes adresses
* out: addresses swaps - so that the values are swaps
* called by: void bubbleSort(struct recipes *all, int n)
* purpose: to swap
*/
void swap(struct recipes *xp, struct recipes *yp){
    struct recipes temp = *xp;
    *xp = *yp;
    *yp = temp;
}

///////////////////////////*PART 4:  RULE  *////////////////////////////////////////////////////////////////////////////////////////

/*
* in:  all, index i
* out: fills out until date - filles out Rule(entire line) - 1 if it has rule 0 if it does not
* calls: isRule("RRULE:",all[i].event ,100, all);  until_date(all,i); 
* purpose: filter the events with a rule and without a rule
*/
int rrule( struct Node * head){
    int y =  isRule("RRULE:",head->data.event);
    if(y > 0){
         int len;
        char word3[8];
        strncpy(word3, "RRULE:", 7 );
        char RRULE[50];
        len = find_key_words(head->data.event, word3, RRULE);
        memset(head->data.rule,'\0',len+5);
        strncpy(head->data.rule, RRULE, len );
       int until = until_date(head); 
        //manage_rule(head, head->data);
        //printf("%ld\n", head->data.until); 
        return 1;
    }
     head->data.until = 0;
     //printf("%ld\n", head->data.until); 
    return 0;
}

/*
* in:  all i 
* out: "RRULE:FREQ=WEEKLY;UNTIL=20210418T235959;BYDAY=MO" = 20210418
* called by: int rrule( struct recipes* all, int i){ 
* purpose: extract from all[i].rule - until_date and turn into long int\
*/
long int until_date(struct Node * head){
    long int date = 0;
    int len = strlen(head->data.rule);

    char *ret;
    ret = strstr(head->data.rule, "TIL=");
    //printf("%s\n",ret);
    char until_char[8];
    //printf("\n\n%ld,%d,%s \n",date, len, event[index].rule);
     //25 - 34
     int uci=0;
    for(int i = 4; i< 12; i++){
        until_char[uci] = ret[i];
       // printf("%c",  until_char[uci] );
        uci++;
    }
 
    date =atoi(until_char);
     //printf("\n\n%ld,%d,%s \n",date, len, until_char);
     head->data.until = date;
    // printf("\n the date is %ld\n",date);
    return date;
}


/*
* in: "RRULE:",all[i].event ,200, all
* out: if RULE is found witin the first 200 chars of an event: returns one
* called by: int rrule( struct recipes* all, int i){
* purpose: acts like a boolean - if there is a rule if not:
*/
int isRule(char *word,char *buffer){
    int wl = strlen(word);
    int bl = strlen(buffer);
    int found = 0;
    int match =0;
    int wordindex = 0; 
    for(int i = 0 ; i < bl; i++){
        if(buffer[i] == word[wordindex]){
            wordindex++;
            match++;
            if(match == wl){
                found = found +7;
                break;
            }
          
        }else{
            match= 0;
            wordindex = 0; 
        }
    }
    return found;
}

/*
* in:  head , event to repeat
* out: all the reps needed within the array and complete: returns number of events added
* calls: start_date_atoi(all[num+j].date);   create_recurring_event( all, rep, num+j);
* purpose: managed the rule: makes sure all[] indexes are appropriate
*/
int manage_rule(struct Node * head, struct recipes rp){
    int rep = 1;
    long int compare = 0;
    char hold[50] ;
    struct Node* r = head;
	while (head->data.until > compare) {
        push(r, r->data);
        memset(r->next->data.event,'\0',300);
        strncpy(r->next->data.event, r->data.event, 300);
        
        fill_properties(r->next);
        dt_increment(r->next->data.date,r->next->data.date, 7*rep);
        r->next->data.start_date = start_date_atoi(r->next->data.date);
        r->next->data.until = 0;
        dt_increment(hold,r->next->data.date, 7);
        compare = start_date_atoi(hold);
        r = r->next;    
        rep++;
	}
          
    return rep;

}

/*
* in: all, rep = one to repeat and add a week , num+j = index
* out: new recipes identical to rep but a week later- at all[num+j]
* calls: dt_increment(repeat.date, rep.date, 7);
* purpose: create recurring event
*/
struct recipes create_recurring_event( struct Node * head,  struct recipes rep){
        struct recipes repeat;     

        printf("\n%s,  %s\n", head->data.event, head->data.summary);
        memset(repeat.date,'\0',50);
        printf("Last week Event %s\n",head->data.date);
        dt_increment(repeat.date, head->data.date, 7);
        printf("Week later Next Event date %s\n\n",repeat.date );
      
 
        //strncpy(repeat.date,all[i].date,50);
        memset(repeat.summary,'\0',50);
        strncpy(repeat.summary,head->data.summary, 50);
        memset(repeat.location,'\0',50);
        strncpy(repeat.location,head->data.location,50);
        memset(repeat.time,'\0',50);
        strncpy(repeat.time,head->data.time, 50);
        repeat.start_date = start_date_atoi(repeat.date);

        printf("%s", repeat.summary); 
       printf("%s\n", repeat.location); 
      printf("%s\n", repeat.date); 
     printf("%ld\n", repeat.start_date); 
     printf("%s\n\n", repeat.time); 
        
        
     return repeat;
}

struct Node pushRR(struct Node * head, struct recipes val) {

    struct Node * current = head;
    while (current->next != NULL) {
        current = current->next;
    }
      /* now we can add a new variable */
    current->next = (struct Node*)malloc(sizeof(struct Node));
    current->next->data = val;
    current->next->next = NULL;
   
return *current->next;

}


/*
* in:  date: 20210213T103000 -- 20210213
* out: returns the int version of the starting dat of a all.[date] -
* called by:  manage_rule(struct recipes* all, int num, struct recipes rep)
* purpose: to make the termation of the while loop in 
    manag_rule as independent as possible to create_recurring_event
*/
long int start_date_atoi(char *date ){
    long int start = 0;
    //int len = strlen(date);
    char until_char[8];
    //printf("\n\n input date %s \nstat_d:for",date);
     //25 - 34
     int uci=0;
    for(int i = 0; i< 8; i++){
        until_char[uci] = date[i];
        //printf("%c",  until_char[uci] );
        uci++;
    }

   start =atoi(until_char);
    //printf("\n\n%ld %s \n",start,until_char );
    return start;
}

/*
* in: 20210220T103000
* out: 20210227T103000
* callsed by:struct recipes create_recurring_event( struct recipes* all, struct recipes rep, int totalnum)
* purpose: date  - one week later -
*/
void dt_increment(char *after, const char *before, int const num_days){
   // printf(" after %s, before %s: \t", after, before);
    struct tm temp_time, *p_temp_time;
    time_t    full_time;
    char      temp[10] = "T103000";// this does not matter for date formatting


    memset(&temp_time, 0, sizeof(struct tm));
    sscanf(before, "%4d%2d%2d", &temp_time.tm_year,
        &temp_time.tm_mon, &temp_time.tm_mday);
    temp_time.tm_year -= 1900;
    temp_time.tm_mon -= 1;
    temp_time.tm_mday += num_days;

    full_time = mktime(&temp_time);
      //  printf(" fulltime %ld:::", full_time);  
    after[0] = '\0';
    strftime(after, 9, "%Y%m%d", localtime(&full_time));
      //printf(" after %s:::", after);  
      strcat(after, temp);
       //printf(" after %s:::", after);  
         //printf(" before %s:::", before);  
        //printf(" \n\t\t\t\t\t after %s, before %s", after, before);
       
}


////////////////////////////// PART 3:  fill out keyWord //////////////////////////////////////////////////////////////////////////////
void fill_properties(struct Node * head){
    summary(head);
    location(head);
    date(head);
    start_date(head);
    get_time(head);
    convertTime(head);
    rrule(head);
    

    //printf("%s", head->data.summary); 
    //printf("%s\n", head->data.location); 
    //printf("%s\n", head->data.date); 
    //printf("%ld\n", head->data.start_date); 
    //printf("%s\n\n", head->data.time); 

}

/*
* in: all and index
* out: modifies all[i]
* calls: to_standard_time(military_int, start_time);
* purpose: 180000210000  --- 6:00 PM to 9:00 PM:
*/
void convertTime(struct Node * head){
    char military_t[10];
    char model[50]= "";
    char start_time[20];
    char to[5] = " to ";
    char end_time[20]; 
    char semicolen[2] = ":";
   
    strncpy(military_t, head->data.time, 4 );
    int military_int = atoi(military_t);
    head->data.start_time = military_int;
    to_standard_time(military_int, start_time);
        // printf("%s\n",e[index].time );
        //printf("military End %s\n",military_t);  
        //printf("\nmilitary_start %d \n",military_int);
        //printf("IN FUNCTION %s\n",start_time);
      
    int ni = 0;
    for(int i = 6; i <10;i++){
        military_t[ni] = head->data.time[i];
        ni++;
               //printf("%c",end[ni]);
    }
  
    military_int = atoi(military_t);
    to_standard_time(military_int, end_time);

        // printf("military End %d\n",military_int);
        //printf("  this is end %s\n",military_t);
        //printf("IN FUNCTION %s\n",end_time);

    strcat(start_time, to);
    strcat(model, start_time);
    strcat(model, end_time);
    strcat(model, semicolen);
    memset(head->data.time,'\0',27);  
    strncpy(head->data.time, model, 21 );
     //printf("\n%s\n",e[index].time);
    
}

/*
* in: 1800 , blank char[]
* out: 6:00 PM - into start_time
* called by: void convertTime(struct recipes *all, int index)
* purpose: do the math part: converts the hour 
*/
void to_standard_time(int military_time,   char *start_time){
    int hour_start = military_time / 100;
    int start_minute = (military_time) -(hour_start*100 );
    char min[10]; char semicolen[2] = ":";  char zero[5] = "0";
    char am[5] = " AM"; char pm[5] = " PM"; int am_pm = 0;
    char space[20] = " ";

    if(hour_start > 11){
        if(hour_start > 12){
        hour_start = hour_start -12 ; // changes to standard   
        }      
        am_pm = 1;
     } else if(hour_start == 0){
         hour_start = 12;
     }
     
    sprintf(start_time, "%d", hour_start);
    sprintf(min, "%d", start_minute);
    strcat(start_time, semicolen);
        if(start_minute < 10){
           strcat(start_time, zero);
        }
    strcat(start_time, min );
  
    if( am_pm == 1){
        strcat(start_time, pm );
    }else{
        strcat(start_time, am );
    }

    if(hour_start < 10){
            strcat(space,start_time);
            memset(start_time,'\0', 20);
            strncpy(start_time, space, 20);
      }

   // printf("\n%s\n",start_time);
}

/*
* in: all and index i
* out: filled out summarry for all[i].summary
* calls: find_key_words
* purpose: find and fill time of event

20210214T180000 -20210214T210000 == 180000210000 
*/
void get_time(struct Node * head){
    int len;
    char word1[] = "DTEND";
    char DTEND[50];
    memset(DTEND,'\0',50);
    char word[] = "DTSTART";
    char DTSTART[50];
          memset(DTSTART,'\0',50);
    len = find_key_words(head->data.event, word, DTSTART);
    //printf("DSTART %s\n",DTSTART);
    len = find_key_words(head->data.event, word1, DTEND);
    //printf("DTEND %s \n",DTEND);
        char *ret;
        ret = strchr(DTSTART, 'T');
        ret++;
        strncpy(DTSTART, ret, 8);
         //printf("DSTART %s\n",DTSTART);
        ret = strchr(DTEND, 'T');
        ret++;
        memset(DTEND,'\0',len-8);
        strncpy(DTEND, ret, 8);
        // printf("DTEND %s \n",DTEND);

    strncat(DTSTART, DTEND, 10);
      memset(head->data.time,'\0',len+3);
    strncpy(head->data.time,DTSTART, len-3);
    //printf("%s , %s\n",all[i].time ,DTSTART);
}

/*
* in: all and index i
* out: filled out starting_date for all[i].starting_date
* calls: find_key_words
* purpose: find and fill starting_date of event
*/
void start_date(struct Node * head){
    long int date = 0;
    int len = strlen(head->data.date);
    char until_char[8];
    //printf("\n\n%ld,%d,%s \n",date, len,event[index].date );
     //25 - 34
     int uci=0;
    for(int i = 0; i< 8; i++){
        until_char[uci] = head->data.date[i];
        //printf("%c",  until_char[uci] );
        uci++;
    }

    date =atoi(until_char);
    head->data.start_date = date;
    //printf("\n\n%ld %s, %d\n",date, event[index].date, index);
}

/*
* in: all and index i
* out: filled out date for all[i].date
* calls: find_key_words
* purpose: find and fill date of event
*/
void date(struct Node * head){
    int len;
    char word[] = "DTSTART";
    char DTSTART[50];
    memset(DTSTART,'\0',50);
    len = find_key_words(head->data.event, word, DTSTART);
    memset(head->data.date,'\0',50);
    strncpy(head->data.date,DTSTART, 50 );
    //printf("%s \n",all[i].date);
}

/*
* in: struct Node * head
* out: filled out head->data.summary
* calls: find_key_words
* purpose: find and fill summmary of event
*/
void summary(struct Node * head){
    int len;
    char word3[8];
    strncpy(word3, "SUMMARY:", 7 );
    char SUMMARY[50];
    len = find_key_words(head->data.event, word3, SUMMARY);

    memset(head->data.summary,'\0',len+5);
    strncpy(head->data.summary, SUMMARY , len);     
    //printf("%s", head->data.summary);   
}

/*
* in: struct Node * head
* out: filled out location for head->data.location
* calls: find_key_words
* purpose: find and fill location of event
*/
void location(struct Node * head){
    int len;
    char word2[] = "LOCATION";
    char LOCATION[40];
    len = find_key_words(head->data.event, word2, LOCATION);
    memset(head->data.location,'\0',len+5);
    strncpy(head->data.location, LOCATION,len);  
    //printf("%s", head->data.location); 
}

/*
* in: all[i].event (paragraph) - keyWord: to find  - Store word
* out: int  = length of filled out word and filled out NewWord: 
* called by:  date( all, i); location( all, i); summary(all, i); get_time(all,i); 
                convertTime(all,i); start_date(all,i);
* purpose: pointer to word index - copy after ':' into newWord and end at '\n' 
*/
int find_key_words(char *event, char *word, char *newWord){
    char *ret;
    ret = strstr(event, word);
    int i =0;
    int ni= -1;
    while(ret[i]!= '\n'){
        if(ret[i] == ':'){
            ni = 0;
            i++;
        }
        if (ni >= 0){
            newWord[ni] = ret[i];
            //printf("%c",newWord[ni]);
            ni++;
        }
        i++;
    }
    //printf("%s, %d\n",newWord, ni);
    return ni;
}

///////////////////////////*PART 2:  argc -  make a recipe and set up linked list *///////////////////////////////////////////

void fill_out_ll(struct Node* n){
    n = n->next;
	while (n != NULL) {
        //printf("%s\n", n->data.event);
       fill_properties(n);
		n = n->next;
	}
}

/*
* in: char *buffer, int startingindex, int i
* out: struct recipes new = with a filled out event: 
* called by: int find_separte_events( char *word,char *buffer ,int noc, struct recipes *all);
* purpose: stores every unique event and contains all there properties
*/
struct recipes create_recipes(char *buffer, int startingindex, int i){
    struct recipes new;
        int length = i  - startingindex;
        char *event2 = &buffer[startingindex];
        memset(new.event,'\0',length+5);
        strncpy(new.event,event2,length);
        //printf("%s\n", new.event);
    return new;
                 
 }

int linkedlist(char *buffer, int noc, struct Node* head){
    char word[50] = "END:VEVENT\n";
    int wl = strlen(word);  // length of given word
    int startingindex = 0; // for: create_recipes
    int found =0; // number of unique events found
    int match =0;  // the number of consecutive similar letters that match 
    int wordindex = 0;  // index of given word
    for(int i = 0 ; i < noc; i++){
        if(buffer[i] == word[wordindex]){ 
            wordindex++;
            match++;
            if(match == wl){
                struct recipes two = create_recipes(buffer, startingindex, i ); 
                push(head,two);
                fill_out_ll(head);
                
                startingindex = i;
                found++;
            }
          
        }else{
            match =0; 
            wordindex = 0; 
        }
    }
    return found;

}

void printList(struct Node* n){
    	n = n->next;
	while (n != NULL) {
		printf("\n%s", n->data.event);
		n = n->next;
         printf("\n");
	}
    printf("\n");
}

void push(struct Node * head, struct recipes val) {

    struct Node * current = head;
    while (current->next != NULL) {
        current = current->next;
    }
      /* now we can add a new variable */
    current->next = (struct Node*)malloc(sizeof(struct Node));
    current->next->data = val;
    current->next->next = NULL;
}

///////////////////////////*PART 1:  argc - filename - file into buffer *///////////////////////////////////////////
/*
In: int argc, char *argv[], char *buffer
Out: int numofchar within file
calls: IntoString(filename, buffer);
Puropse: sort agruments form passed from command line and locate filename
*/
int sortArugments(int argc, char *argv[], char *buffer){
    int from_y = 0, from_m = 0, from_d = 0;
    int to_y = 0, to_m = 0, to_d = 0;
    char *filename = NULL;
    int i; 

    //given code 
    for (i = 0; i < argc; i++) {
        if (strncmp(argv[i], "--start=", 8) == 0) {
            sscanf(argv[i], "--start=%d/%d/%d", &from_y, &from_m, &from_d);
        } else if (strncmp(argv[i], "--end=", 6) == 0) {
            sscanf(argv[i], "--end=%d/%d/%d", &to_y, &to_m, &to_d);
        } else if (strncmp(argv[i], "--file=", 7) == 0) {
            filename = argv[i]+7;
             //printf("\n%s\n",filename);
        }
    }

    if (from_y == 0 || to_y == 0 || filename == NULL) {
        fprintf(stderr, 
            "usage: %s ./process_cal --start=1234/01/12 --end=2134/12/11 --file=ics/one.ics\n",
            argv[0]);
        exit(1);
    }

    //^ give code || v my code
    int numofchar;
    numofchar = intoString(filename, buffer);
    
    return numofchar;
}

/*
* in: char *filename, char *buffer <-buffer is an empty array 
* out: numofchar within file
* called by: sortArugments
* purpose: obtain an array holding file contents
*/
int intoString( char *filename, char *buffer){
    char c;  FILE *fptr;
    int numOfchar=1;

    // Open file
    fptr = fopen(filename, "r");
    if (fptr == NULL){
        printf("\n Cannot open file %s \n", filename);
        exit(0);
    }

    // Read contents from file
    c = fgetc(fptr);
    int wordindex; 
    wordindex = 0;
    
    while (c != EOF){
        buffer[wordindex]= c;
        numOfchar++;
        wordindex++; 
        c = fgetc(fptr);
    }
   
    fclose(fptr);
    return numOfchar;
}
