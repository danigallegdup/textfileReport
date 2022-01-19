/* Daniela Gallegos Dupuis : V00942356
 * process_cal3.c
 * Starter file provided to students for Assignment #3, SENG 265,
 * Fall 2021.

    gcc process_cal3.c -o process_cal3
    ./process_cal3 --start=1234/01/12 --end=2134/12/11 --file=ics/one.ics
   
 - uses linked lists
 - dynamic memory allocation
 - 


 valgrind --leak-check=yes ./process_cal3 --start=2021/4/18 --end=2021/4/21 --file=ics/diana-devops.ics 2&> temp.txt
    ==7168== HEAP SUMMARY:
    ==7168==  in use at exit: 0 bytes in 0 blocks
      total heap usage: 782 allocs, 782 frees, 157,933 bytes allocated

      process_cal3.c, emalloc.c, emalloc.h, ics.h, listy.c, listy.h, makefile, email comfirmation that I got an extention

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
    int start_time; 
    long int  start_date; 
    long int until;
}recipes;

struct Node {
    struct Node* first;
    struct Node* last;
	struct recipes data;
	struct Node* next;
};

// file information to program information : stored in main
int sortArugments(int argc, char *argv[], char *buffer);
int intoString (char *filename, char *buffer);
long int  arg__end(int argc, char *argv[]);
long int  arg__start(int argc, char *argv[]);

// linked list functions
void printList(struct Node* n);
struct recipes create_recipes(char *buffer, int startingindex, int i); 
void push(struct Node * head, struct recipes one);
int linkedlist(char *buffer, int noc, struct Node* head);
void fill_out_ll(struct Node* head);

// filling out all information functions
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

// rrule functions
void dt_increment(char *after, const char *before, int const num_days);
int manage_rule(struct Node * head);
long int start_date_atoi(char *date );

// sort
void bubbleSort(struct Node * head, int n);
void swap(struct recipes *xp, struct recipes *yp);

// print :)
void dt_format(char *formatted_time, const char *dt_time, const int len);
void printEvent(struct Node * head);
void manage_printing(struct Node * head, int newE, long int start_range, long int end_range);
void printEvent_same_day(struct Node * head);

int main(int argc, char *argv[]){
// get all information from command line aruguments
    long int arg_start=  arg__start(argc, argv);
    long int arg_end = arg__end(argc, argv);
    char buffer[43500];
    int numofchar;
    numofchar = sortArugments(argc, argv, buffer);

// make linked list
    struct Node* head = NULL;
    struct Node* temp = NULL;
    head = (struct Node*)malloc(sizeof(struct Node));
    head->next = NULL;
    linkedlist(buffer, numofchar, head);
    temp = head;
    head = head->next;
    free(temp);

 // count num of events and takes care of rrule   
    struct Node* n = head;
    int i =0;
	while (n != NULL) {
        if(n->data.until > 0){
             manage_rule(n);
        }
		n = n->next;    
        i++;
	}

// sort and print
    bubbleSort(head,i);
    manage_printing( head, 2, arg_start, arg_end);

// Free all allocated memeory
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
   long int prev =0;
	while (n != NULL) {  
        //printf("%ld, %ld\n",n->data.start_date, start_range);
        if(n->data.start_date >= start_range && n->data.start_date<= end_range){
         if(prev ==n->data.start_date){
             printEvent_same_day(n);
         }else{
            if (first > 0){
            printf("\n");
            }   
            first++;
		    dt_format(n->data.date, n->data.date, 50); 
            printEvent(n);
         }
         
        }
        prev = n->data.start_date;
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

///////////////////// Part 5: sorting linked list by date and time is earliest to latest /////////////////////////////////////


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
        until_date(head); 
        return 1;
    }
     head->data.until = 0;
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

    char *ret;
    ret = strstr(head->data.rule, "TIL=");
    char until_char[8];
     int uci=0;
    for(int i = 4; i< 12; i++){
        until_char[uci] = ret[i];
        uci++;
    }

    date =atoi(until_char);
    head->data.until = date;
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
int manage_rule(struct Node * head){
    struct Node* end_node = head;
    int i =0;
	while (end_node->next != NULL) {
		end_node= end_node->next;    
        i++;
	}    
    
    int rep = 1;
    long int compare = 0;
    char hold[50] ;
    struct Node* replicate_node = head;
	while (head->data.until > compare) {
        push(head, head->data); 
        memset(end_node->next->data.event,'\0',300);
        strncpy(end_node->next->data.event, replicate_node->data.event, 300);
        
        fill_properties(end_node->next);
        dt_increment(end_node->next->data.date,end_node->next->data.date, 7*rep);
        end_node->next->data.start_date = start_date_atoi(end_node->next->data.date);
        end_node->next->data.until = 0;
        
        dt_increment(hold,end_node->next->data.date, 7);
        compare = start_date_atoi(hold);
        end_node = end_node->next;    
        rep++;
	}
          
    return rep;

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
    char until_char[8];
    int uci=0;
    for(int i = 0; i< 8; i++){
        until_char[uci] = date[i];
        uci++;
    }
   start =atoi(until_char);
   return start;
}

/*
* in: 20210220T103000
* out: 20210227T103000
* callsed by:struct recipes create_recurring_event( struct recipes* all, struct recipes rep, int totalnum)
* purpose: date  - one week later -
*/
void dt_increment(char *after, const char *before, int const num_days){
    struct tm temp_time;
    time_t    full_time;
    char      temp[10] = "T103000";

    memset(&temp_time, 0, sizeof(struct tm));
    sscanf(before, "%4d%2d%2d", &temp_time.tm_year,
        &temp_time.tm_mon, &temp_time.tm_mday);
    temp_time.tm_year -= 1900;
    temp_time.tm_mon -= 1;
    temp_time.tm_mday += num_days;

    full_time = mktime(&temp_time);
    after[0] = '\0';
    strftime(after, 9, "%Y%m%d", localtime(&full_time)); 
    strcat(after, temp);
    
       
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

    if(strcmp(head->data.location,head->data.summary )==0){
        memset(head->data.location,'\0',40+5);
    }

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
      
    int ni = 0;
    for(int i = 6; i <10;i++){
        military_t[ni] = head->data.time[i];
        ni++;
    }
  
    military_int = atoi(military_t);
    to_standard_time(military_int, end_time);

    strcat(start_time, to);
    strcat(model, start_time);
    strcat(model, end_time);
    strcat(model, semicolen);
    memset(head->data.time,'\0',27);  
    strncpy(head->data.time, model, 21 );

    
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
        hour_start = hour_start -12 ;  
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
    len = find_key_words(head->data.event, word1, DTEND);
        char *ret;
        ret = strchr(DTSTART, 'T');
        ret++;
        strncpy(DTSTART, ret, 8);
        ret = strchr(DTEND, 'T');
        ret++;
        memset(DTEND,'\0',len-8);
        strncpy(DTEND, ret, 8);

    strncat(DTSTART, DTEND, 10);
      memset(head->data.time,'\0',len+3);
    strncpy(head->data.time,DTSTART, len-3);
}

/*
* in: all and index i
* out: filled out starting_date for all[i].starting_date
* calls: find_key_words
* purpose: find and fill starting_date of event
*/
void start_date(struct Node * head){
    long int date = 0;
    char until_char[8];
    int uci=0;
    for(int i = 0; i< 8; i++){
        until_char[uci] = head->data.date[i];
        uci++;
    }

    date =atoi(until_char);
    head->data.start_date = date;
}

/*
* in: all and index i
* out: filled out date for all[i].date
* calls: find_key_words
* purpose: find and fill date of event
*/
void date(struct Node * head){
    char word[] = "DTSTART";
    char DTSTART[50];
    memset(DTSTART,'\0',50);
    find_key_words(head->data.event, word, DTSTART);
    memset(head->data.date,'\0',50);
    strncpy(head->data.date,DTSTART, 50 );
}

/*
* in: struct Node * head
* out: filled out head->data.summary
* calls: find_key_words
* purpose: find and fill summmary of event
*/
void summary(struct Node * head){
    int len;
    char word3[] = "SUMMARY:";
    char SUMMARY[50];
    len = find_key_words(head->data.event, word3, SUMMARY);

    memset(head->data.summary,'\0',len+5);
    strncpy(head->data.summary, SUMMARY , len);        
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
            ni++;
        }
        i++;
    }
    return ni;
}

///////////////////////////*PART 2:  argc -  make a recipe and set up linked list *///////////////////////////////////////////

void fill_out_ll(struct Node* n){
    n = n->next;
	while (n != NULL) {
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

    for (i = 0; i < argc; i++) {
        if (strncmp(argv[i], "--start=", 8) == 0) {
            sscanf(argv[i], "--start=%d/%d/%d", &from_y, &from_m, &from_d);
        } else if (strncmp(argv[i], "--end=", 6) == 0) {
            sscanf(argv[i], "--end=%d/%d/%d", &to_y, &to_m, &to_d);
        } else if (strncmp(argv[i], "--file=", 7) == 0) {
            filename = argv[i]+7;
        }
    }

    if (from_y == 0 || to_y == 0 || filename == NULL) {
        fprintf(stderr, 
            "usage: %s ./process_cal --start=1234/01/12 --end=2134/12/11 --file=ics/one.ics\n",
            argv[0]);
        exit(1);
    }
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
