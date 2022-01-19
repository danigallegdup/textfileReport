/*
Daniela Gallegos Dupuis
V00942356

my very best shot:
- no global variables
- written as neatly as I could
- passed every test: one, two, many, diana-devops

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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


//part 1 : given code: finds filename and calls IntoString
int sortArugments(int argc, char *argv[], char *buffer);
int intoString (char *filename, char *buffer);
// part 2: From buffer: this finds all the events and puts it into recipes
int find_separte_events( char *word,char *buffer ,int noc, struct recipes *all);
struct recipes create_recipes(char *buffer, int startingindex, int i); 
// Part 3: find and stor key information into recipes
void fill_event_properties(struct recipes *all, int num);
int find_key_words(char *buffer, char *word, char *newWord);
void summary(struct recipes *all, int i);
void location(struct recipes *all, int i);
void date(struct recipes *all, int i);
void start_date(struct recipes* all, int index);
void get_time(struct recipes *all, int i);
//Part 3.2: converting miltary to standard time
void convertTime(struct recipes *e, int index);
void to_standard_time(int military_time,   char *start_time);
//Part 4: frequency - rule
int manage_rule(struct recipes* event, int num, struct recipes replicate);
long int start_date_atoi(char *date );
int rrule( struct recipes* event, int i);
int isRule(char *word,char *buffer ,int noc, struct recipes *all);
long int until_date(struct recipes* event, int i);
struct recipes create_recurring_event( struct recipes* all, struct recipes replicate, int totalnum);
void dt_increment(char *after, const char *before, int const num_days);
// Part 5: sorting by date:
void swap(struct recipes *xp, struct recipes *yp);
void bubbleSort(struct recipes *all, int n);
//Part 6: getting  printing in Range
long int arg__start(int argc, char *argv[]);
long int arg__end(int argc, char *argv[]);
// Part 7: print fix date and print event
void printEvent(struct recipes event);
void printEvent_same_day(struct recipes event);
void dt_format(char *formatted_time, const char *dt_time, const int len);
void manage_printing(struct recipes *all, int newE, long int start_range, long int end_range);

int main(int argc, char *argv[]){

    /* get file into array */
    char buffer[3500];
    int numofchar;
    //part 1:
    numofchar = sortArugments(argc, argv, buffer);
    //printf("\n%s\n", buffer); 
    //part 2:
    struct recipes all[200];
    int num = find_separte_events("END:VEVENT\n", buffer, numofchar, all);
    //printf("the number of events %d\n", num);
    //part 3:
    fill_event_properties(all, num);
    //part 4:
    int newE= num;
    for(int i =0; i < num; i++){
        if (rrule(all,i) ==1){
            long int end_date =  until_date(all, i);
              newE = manage_rule(all, newE , all[i] ) + newE;
        }
    } 
    //part 5: sort by date
    bubbleSort(all,newE );
    //in range of dates passed
    long int start_range = arg__start( argc, argv);
    //printf("%ld\n", start_range );
    long int end_range = arg__end(argc,argv);
    //printf("%ld\n", end_range );
    // part 6: fix date and print
      for(int i =0; i < newE; i++){
           dt_format(all[i].date, all[i].date, 50); 
           //printEvent(all[i]);
        }   
     
    manage_printing(all, newE, start_range,  end_range); 
     //printf("the number of events %d\n", newE); 
}

///////////////////// Part 7: formatting date and printing calender as assignment instucted ////////////////////
/*
* in:  all[i]
* out: printed exactly as assignment specifies
* calls: --
* purpose: show program works perfectly
*/

/*
* in:  all, newE, start_range,  end_range
* out: if new day - print with date - if same dat without date
* calls: printEvent(struct recipes event, printEvent_same_day(struct recipes event)
* purpose: prints out properly
*/
void manage_printing(struct recipes *all, int newE, long int start_range, long int end_range){
     int printed = 0;
     if(all[0].start_date >=  start_range && all[0].start_date <= end_range){
        printEvent(all[0]);
        printed++;
        }

    for(int i =1; i < newE; i++){
        if(all[i].start_date >=  start_range && all[i].start_date <= end_range){
            if( strcmp(all[i-1].date,all[i].date)== 0){
                 printEvent_same_day(all[i]);
                    printed++;
            }else{
                if(printed>0){
                    printf("\n");
                }
                printEvent(all[i]);
                   printed++;
            }
        }   
    } 
}

/*
* in:  struct recipes event - all[i]
* out: date - time and discription
* calls: ---
* purpose: format date properly
*/
void printEvent(struct recipes event){
    int len = 0;
    while(event.date[len] != ')'){
        len++;
    }
    len = len +1;
    //printf("%d", len);

    char line[len];
    memset(line,'\0',30);
    for(int i =0; i < len; i++){
        line[i] = '-';
    }
    printf("%s\n%s\n",event.date,line);
    printf("%s %s {{%s}}\n",event.time,event.summary,event.location); 
   
    
}

/*
* in:  struct recipes event - all[i]
* out: -time and discription
* calls: ---
* purpose: format date properly
*/
void printEvent_same_day(struct recipes event){
    printf("%s %s {{%s}}\n",
    event.time,
    event.summary,
    event.location); 
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
    char      temp[5];

    memset(&temp_time, 0, sizeof(struct tm));
    sscanf(dt_time, "%4d%2d%2d",
        &temp_time.tm_year, &temp_time.tm_mon, &temp_time.tm_mday);
    temp_time.tm_year -= 1900;
    temp_time.tm_mon -= 1;
    full_time = mktime(&temp_time);
    strftime(formatted_time, len, "%B %d, %Y (%a)", 
        localtime(&full_time));
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
*/
void bubbleSort(struct recipes *all, int n){
   int i, j;
   for (i = 0; i < n-1; i++){
       for (j = 0; j < n-i-1; j++){
            if (all[j].start_date > all[j+1].start_date){
                swap(&all[j], &all[j+1]);
            }
            if(all[j].start_date == all[j+1].start_date && all[j].start_time > all[j+1].start_time){
                swap(&all[j], &all[j+1]);
            }      
        }       
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


///////////////////// Part 4: Rule Frequency - adding to all - weekly til until_date ///////////////////////////////

/*
* in:  all, index i
* out: fills out until date - filles out Rule(entire line) - 1 if it has rule 0 if it does not
* calls: isRule("RRULE:",all[i].event ,100, all);  until_date(all,i); 
* purpose: filter the events with a rule and without a rule
*/
int rrule( struct recipes* all, int i){
    int y =  isRule("RRULE:",all[i].event ,200, all);
    if(y > 0){
         int len;
        char word3[8];
        strncpy(word3, "RRULE:", 7 );
        char RRULE[50];
        len = find_key_words(all[i].event, word3, RRULE);
        memset(all[i].rule,'\0',len+5);
        strncpy(all[i].rule, RRULE, len );
       int until = until_date(all,i); 
        return 1;
    }
    return 0;
}

/*
* in: "RRULE:",all[i].event ,200, all
* out: if RULE is found witin the first 200 chars of an event: returns one
* called by: int rrule( struct recipes* all, int i){
* purpose: acts like a boolean - if there is a rule if not:
*/
int isRule(char *word,char *buffer ,int noc, struct recipes *all){
    int wl = strlen(word);
    int found = 0;
    int match =0;
    int wordindex = 0; 
    for(int i = 0 ; i < noc; i++){
        if(buffer[i] == word[wordindex]){
            wordindex++;
            match++;
            if(match == wl){
                found++;
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
* in:  all i 
* out: "RRULE:FREQ=WEEKLY;UNTIL=20210418T235959;BYDAY=MO" = 20210418
* called by: int rrule( struct recipes* all, int i){ 
* purpose: extract from all[i].rule - until_date and turn into long int\
*/
long int until_date(struct recipes* event, int index){
    long int date = 0;
    int len = strlen(event[index].rule);

    char *ret;
    ret = strstr(event[index].rule, "TIL=");
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
     event[index].until = date;
    // printf("\n the date is %ld\n",date);
    return date;
}

/*
* in:  all ,i , event to repeat
* out: all the reps needed within the array and complete: returns number of events added
* calls: start_date_atoi(all[num+j].date);   create_recurring_event( all, rep, num+j);
* purpose: managed the rule: makes sure all[] indexes are appropriate
*/
int manage_rule(struct recipes* all, int num, struct recipes rep){
    // printf("\n\nevent index: %d  rule: %s \n\t start_date is: %ld end_date: %ld \n",
           //  num, rep.rule, rep.start_date, rep.until );
            long int start = rep.start_date;
           // printf("\nstart: %ld ", start);
            long int until = rep.until;
          
        int j = 0;
             struct recipes new=  create_recurring_event( all, rep, num+j);
            while( start < until ){  
               // printf("\nevent index: %d  until: %ld start: %ld ",
               // num+j, until, start);
                 j++;
                 new =  create_recurring_event( all, new, num+j);
                 start = start_date_atoi(all[num+j].date);
                }
          
    return j;

}

/*
* in: all, rep = one to repeat and add a week , num+j = index
* out: new recipes identical to rep but a week later- at all[num+j]
* calls: dt_increment(repeat.date, rep.date, 7);
* purpose: create recurring event
*/
struct recipes create_recurring_event( struct recipes* all,  struct recipes rep, int totalnum){
        struct recipes repeat;     

        memset(repeat.date,'\0',50);
        //printf("\n\n Last week Event %s\n ",rep.date);
        dt_increment(repeat.date, rep.date, 7);
       // printf("\n Week later Next Event date %s\n\n",repeat.date );
      
        repeat.start_time =  rep.start_time ;
        //strncpy(repeat.date,all[i].date,50);
            memset(repeat.summary,'\0',50);
        strncpy(repeat.summary,rep.summary, 50);
           memset(repeat.location,'\0',50);
        strncpy(repeat.location,rep.location,50);
          memset(repeat.time,'\0',50);
        strncpy(repeat.time,rep.time, 50);
        
       
        // printf("\t%d, ", totalnum);
         all[totalnum] = repeat;
        start_date(all, totalnum);
       //  printf("\t let us see %ld %ld\n, ", set, all[i].start_date );
      //printEvent(all[totalnum]);
     //dt_format(all[i].date, all[i].date, 50);
     return repeat;
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

/////////////////////PART 3.2: military time to standard time ///////////////////////////////////////////////////////

/*
* in: all and index
* out: modifies all[i]
* calls: to_standard_time(military_int, start_time);
* purpose: 180000210000  --- 6:00 PM to 9:00 PM:
*/
void convertTime(struct recipes *all, int index){
    char military_t[10];
    char model[50]= "";
    char start_time[20];
    char to[5] = " to ";
    char end_time[20]; 
    char semicolen[2] = ":";
   
    strncpy(military_t, all[index].time, 4 );
    int military_int = atoi(military_t);
    all[index].start_time = military_int;
    to_standard_time(military_int, start_time);
        // printf("%s\n",e[index].time );
        //printf("military End %s\n",military_t);  
        //printf("\nmilitary_start %d \n",military_int);
        //printf("IN FUNCTION %s\n",start_time);
      
    int ni = 0;
    for(int i = 6; i <10;i++){
        military_t[ni] = all[index].time[i];
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
    memset(all[index].time,'\0',27);  
    strncpy(all[index].time, model, 21 );
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

///////////////////// PART 3 :filling out basic properties: all except rule /////////////////////////////////////////

/*
* in:  all, num --> with all.event filled out
* out: everything needed to print and sort array of events
* calls: date( all, i); location( all, i); summary(all, i); get_time(all,i); 
                convertTime(all,i); start_date(all,i);
* purpose: fill all the properties out - makes it easy to isolate problems 
*/ 
void fill_event_properties(struct recipes *all, int num){
    for(int i =0; i < num; i++){
            date( all, i);
            location( all, i);
            summary(all, i);
            get_time(all,i);
            convertTime(all,i);
            start_date(all,i);

        if(strcmp (all[i].summary, all[i].location) == 0){
                memset(all[i].location,'\0',50);
        }
        //printf("\n");
        //printf("start date int: %ld\n", all[i].start_date);  
        //printf("date to converted: %s\n", all[i].date);
        //printf("location: %s\n", all[i].location);
        //printf("summary: %s\n", all[i].summary);
        // printf("time: %s\n", all[i].time);
    } 

}

/*
* in: all and index i
* out: filled out summarry for all[i].summary
* calls: find_key_words
* purpose: find and fill time of event

20210214T180000 -20210214T210000 == 180000210000 
*/
void get_time(struct recipes *all, int i){
    int len;
    char word1[] = "DTEND";
    char DTEND[50];
    memset(DTEND,'\0',50);
    char word[] = "DTSTART";
    char DTSTART[50];
          memset(DTSTART,'\0',50);
    len = find_key_words(all[i].event, word, DTSTART);
    //printf("DSTART %s\n",DTSTART);
    len = find_key_words(all[i].event, word1, DTEND);
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
      memset(all[i].time,'\0',len+3);
    strncpy(all[i].time,DTSTART, len-3);
    //printf("%s , %s\n",all[i].time ,DTSTART);
}

/*
* in: all and index i
* out: filled out summarry for all[i].summary
* calls: find_key_words
* purpose: find and fill summmary of event
*/
void summary(struct recipes* all, int i){
        //printf("%s", all[i].event);
        int len;
        char word3[8];
        strncpy(word3, "SUMMARY:", 7 );
        char SUMMARY[50];
        len = find_key_words(all[i].event, word3, SUMMARY);
        memset(all[i].summary,'\0',len+5);
        strncpy(all[i].summary, SUMMARY, len );
    //printf("%s\n",all[i].summary);

}

/*
* in: all and index i
* out: filled out location for all[i].location
* calls: find_key_words
* purpose: find and fill location of event
*/
void location(struct recipes *all, int i){
    int len;
    char word2[] = "LOCATION";
    char LOCATION[40];
    len = find_key_words(all[i].event, word2, LOCATION);
      memset(all[i].location,'\0',len+5);
    strncpy(all[i].location, LOCATION,len );
    //printf("%s , %s\n",all[i].location ,LOCATION);
}

/*
* in: all and index i
* out: filled out date for all[i].date
* calls: find_key_words
* purpose: find and fill date of event
*/
void date(struct recipes *all, int i){
    int len;
    char word[] = "DTSTART";
    char DTSTART[50];
    memset(DTSTART,'\0',50);
    len = find_key_words(all[i].event, word, DTSTART);
    memset(all[i].date,'\0',50);
    strncpy(all[i].date,DTSTART, 50 );
    //printf("%s \n",all[i].date);
}

/*
* in: all and index i
* out: filled out starting_date for all[i].starting_date
* calls: find_key_words
* purpose: find and fill starting_date of event
*/
void start_date(struct recipes* event, int index){
    long int date = 0;
    int len = strlen(event[index].date);
    char until_char[8];
    //printf("\n\n%ld,%d,%s \n",date, len,event[index].date );
     //25 - 34
     int uci=0;
    for(int i = 0; i< 8; i++){
        until_char[uci] = event[index].date[i];
        //printf("%c",  until_char[uci] );
        uci++;
    }

    date =atoi(until_char);
    event[index].start_date = date;
    //printf("\n\n%ld %s, %d\n",date, event[index].date, index);
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

//////////////////////////* PART 2 fill out all.events- get number of unique events */////////////////////////////

/*
* in: char *word,char *buffer ,int noc, struct recipes *all
* out: number of events = num
* calls: struct recipes create_recipes(char *buffer, int startingindex, int i)
* purpose: used the index of "END:EVENT\n" to find the length of all event recipes
*/
int find_separte_events( char *word,char *buffer ,int noc, struct recipes *all){
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
                all[found] = create_recipes(buffer, startingindex, i);
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

///////////////////////////*PART 1:  argc - filename - file into buffer *///////////////////////////////////////////

/*
In: int argc, char *argv[], char *buffer
Out: int numofchar within file
calls: IntoString(filename, buffer);
Puropse: sort agruments form passed from command line and locate filename
***- gonna need to store the begining and end dates of progran

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
