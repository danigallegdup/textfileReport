//break it down one at a time program
// assign1.c

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
    long int  start_date; // 20210304 
    long int until;
}recipes;

// given code: finds filename and calls IntoString
int sortArugments(int argc, char *argv[], char *buffer);
int intoString (char *filename, char *buffer);
/*given file:
                Part One: Buffer
BEGIN:VCALENDAR   
BEGIN:VEVENT
DTSTART:20210119T103000
DTEND:20210119T113000
RRULE:FREQ=WEEKLY;UNTIL=20210412T235959;BYDAY=TU
LOCATION:DSB C112
SUMMARY:ECON 104
END:VEVENT
BEGIN:VEVENT
DTSTART:20210216T080000
DTEND:20210216T090000
LOCATION:Burger King
SUMMARY:Call Mom on her birthday
END:VEVENT
END:VCALENDAR
*/

// From buffer: this finds all the events and puts it into recipes
int find_separte_events( char *word,char *buffer ,int noc, struct recipes *all);
struct recipes create_recipes(char *buffer, int startingindex, int i); 
/* array of structs with filled out recipes:
            Part 2. Find Unique events and store them
all[0].event:          
    BEGIN:VCALENDAR   
    BEGIN:VEVENT
    DTSTART:20210119T103000
    DTEND:20210119T113000
    RRULE:FREQ=WEEKLY;UNTIL=20210412T235959;BYDAY=TU
    LOCATION:DSB C112
    SUMMARY:ECON 104
    END:VEVENT

all[1].event: 
    BEGIN:VEVENT
    DTSTART:20210216T080000
    DTEND:20210216T090000
    LOCATION:Burger King
    SUMMARY:Call Mom on her birthday
    END:VEVENT
*/

// find and stor key information into recipes
void fill_event_properties(struct recipes *all, int num);
int find_key_words(char *buffer, char *word, char *newWord);
void summary(struct recipes *all, int i);
void location(struct recipes *all, int i);
void date(struct recipes *all, int i);
void start_date(struct recipes* all, int index);
void get_time(struct recipes *all, int i);
/*
            Part 3. extract and store important information
start date int: 20210119
date to converted: 20210119T103000
location: DSB C112
summary: ECON 104
date: 103000113000

start date int: 20210216
date to converted: 20210216T080000
location: Burger King
summary: Call Mom on her birthday
date: 080000090000
*/

// converting miltary to standard time
void convertTime(struct recipes *e, int index);
void to_standard_time(int military_time,   char *start_time);
/*
            Part 3.2 converting miltary to standard time

date: 103000113000 -> date: 10:30 AM to 11:30 AM:
date: 080000090000 -> date: 8:00 AM to 9:00 AM:
*/

//frequency - rule
int manage_rule(struct recipes* event, int num, struct recipes replicate);
long int start_date_atoi(char *date );
int rrule( struct recipes* event, int i);
int isRule(char *word,char *buffer ,int noc, struct recipes *all);
long int until_date(struct recipes* event, int i);
struct recipes create_recurring_event( struct recipes* all, struct recipes replicate, int totalnum);
void dt_increment(char *after, const char *before, int const num_days);
/*
    Part 4: generate all reacurring and know total number of events

    event index: 2  until: 20210412 start: 20210119 
    event index: 3  until: 20210412 start: 20210202 
    event index: 4  until: 20210412 start: 20210209 
    event index: 5  until: 20210412 start: 20210216 
    event index: 6  until: 20210412 start: 20210223 
    event index: 7  until: 20210412 start: 20210302 
    event index: 8  until: 20210412 start: 20210309 
    event index: 9  until: 20210412 start: 20210316 
    event index: 10  until: 20210412 start: 20210323 
    event index: 11  until: 20210412 start: 20210330 
    event index: 12  until: 20210412 start: 20210406 
    the number of events 13
*/

//sorting by date:
void swap(struct recipes *xp, struct recipes *yp);
void bubbleSort(struct recipes *all, int n);

// print fix date and print event
void printEvent(struct recipes event);
void dt_format(char *formatted_time, const char *dt_time, const int len);
/*          
        Final Product: part 6:

    January 19, 2021 (Tue) 
	‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
	10:30 AM to 11:30 AM: ECON 104 {{DSB C112}}

	January 26, 2021 (Tue) 
	‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
	10:30 AM to 11:30 AM: ECON 104 {{DSB C112}}

	February 02, 2021 (Tue) 
	‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
	10:30 AM to 11:30 AM: ECON 104 {{DSB C112}}

	February 09, 2021 (Tue) 
	‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
	10:30 AM to 11:30 AM: ECON 104 {{DSB C112}}

	February 16, 2021 (Tue) 
	‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
	8:00 AM to 9:00 AM: Call Mom on her birthday {{Burger King}}

	February 16, 2021 (Tue) 
	‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
	10:30 AM to 11:30 AM: ECON 104 {{DSB C112}}

	February 23, 2021 (Tue) 
	‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
	10:30 AM to 11:30 AM: ECON 104 {{DSB C112}}

	March 02, 2021 (Tue) 
	‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
	10:30 AM to 11:30 AM: ECON 104 {{DSB C112}}

	March 09, 2021 (Tue) 
	‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
	10:30 AM to 11:30 AM: ECON 104 {{DSB C112}}

	March 16, 2021 (Tue) 
	‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
	10:30 AM to 11:30 AM: ECON 104 {{DSB C112}}

	March 23, 2021 (Tue) 
	‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
	10:30 AM to 11:30 AM: ECON 104 {{DSB C112}}

	March 30, 2021 (Tue) 
	‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
	10:30 AM to 11:30 AM: ECON 104 {{DSB C112}}

	April 06, 2021 (Tue) 
	‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
	10:30 AM to 11:30 AM: ECON 104 {{DSB C112}}
*/

int main(int argc, char *argv[]){

    /* get file into array */
    char buffer[5500];
    int numofchar;
//part 1:
    numofchar = sortArugments(argc, argv, buffer);
    //printf("\n%s\n", buffer); // with everthing works without illigal insteuctions

//part 2:
    struct recipes all[250];
    int num = find_separte_events("END:VEVENT\n", buffer, numofchar, all);
    printf("the number of events %d\n", num);

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
      printf("\nthe number of events %d\n", newE);

// part 5: sort by date
        bubbleSort(all,newE );

// part 6: fix date and print
    for(int i =0; i < newE; i++){
         dt_format(all[i].date, all[i].date, 50);
        printEvent(all[i]);
    } 

/**/
}

///////////////////// formatting date and printing calender as assignment instucted ////////////////////

/*
* in:  
* out: 
* calls: 
* purpose: 
*/
void printEvent(struct recipes event){
     char line[100] ="\n\t‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐\n\t";
    printf("\n\t%s %s%s %s {{%s}}\n",
    event.date,
    line,
    event.time,
    event.summary,
    event.location); 
}

/*
* in:  
* out: 
* calls: 
* purpose: 
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

///////////////////// sorting array all by dates earliest to latest /////////////////////////////////////

/*
* in:  
* out: 
* calls: 
* purpose: 
* acknowledgment that this is not the best sorting aglorthmn but the easiest to implement
*/
void bubbleSort(struct recipes *all, int n){
   int i, j;
   for (i = 0; i < n-1; i++){
       for (j = 0; j < n-i-1; j++){
            if (all[j].start_date > all[j+1].start_date){
                swap(&all[j], &all[j+1]);
            }      
        }       
    }            
}

/*
* in:  
* out: 
* calls: 
* purpose: 
*/
void swap(struct recipes *xp, struct recipes *yp){
    struct recipes temp = *xp;
    *xp = *yp;
    *yp = temp;
}
 
///////////////////// Rule Frequency - adding to all - weekly til until_date ///////////////////////////////

/*
* in:  
* out: 
* calls: 
* purpose: 
*/
int rrule( struct recipes* event, int i){
    int y =  isRule("RRULE:",event[i].event ,100, event);
    if(y > 0){
         int len;
        char word3[8];
        strncpy(word3, "RRULE:", 7 );
        char RRULE[50];
        len = find_key_words(event[i].event, word3, RRULE);
        memset(event[i].rule,'\0',len+5);
        strncpy(event[i].rule, RRULE, len );
       int until = until_date(event,i); 
        return 1;
    }
    return 0;
}

/*
* in:  
* out: 
* calls: 
* purpose: 
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
            }
          
        }else{
            match= 0;
            wordindex = 0; 
        }
    }
    return found;
}

/*
* in:  
* out: 
* calls: 
* purpose: 
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
        //printf("%c",  until_char[uci] );
        uci++;
    }
 
    date =atoi(until_char);
   //  printf("\n\n%ld,%d,%s \n",date, len, until_char);
     event[index].until = date;
     //printf("\n the date is %ld\n",date);
    return date;
}

/*
* in:  
* out: 
* calls: 
* purpose: 
*/
int manage_rule(struct recipes* all, int num, struct recipes rep){
     //printf("\nevent index: %d  rule: %s \n\t start_date is: %ld end_date: %ld \n",
             //num, rep.rule, rep.start_date, rep.until );
            long int start = rep.start_date;
            long int until = rep.until;
          
        int j = 0;
             struct recipes new=  create_recurring_event( all, rep, num+j);
            while( start < until){  
                //printf("\nevent index: %d  until: %ld start: %ld ",
                //num+j, until, start);
                 j++;
                 new =  create_recurring_event( all, new, num+j);
                 start = start_date_atoi(all[num+j].date);
                }
          
    return j;

}

/*
* in:  
* out: 
* calls: 
* purpose: 
*/
struct recipes create_recurring_event( struct recipes* all,  struct recipes rep, int totalnum){
        struct recipes repeat;     

        memset(repeat.date,'\0',50);
        dt_increment(repeat.date, rep.date, 7);
        //printf("\n%s ",all[i].date);
        // printf("%s\n",repeat.date);

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

long int start_date_atoi(char *date ){
    long int start = 0;
    int len = strlen(date);
    char until_char[8];
    //printf("\n\n%ld,%d,%s \n",date, len,event[index].date );
     //25 - 34
     int uci=0;
    for(int i = 0; i< 8; i++){
        until_char[uci] = date[i];
        //printf("%c",  until_char[uci] );
        uci++;
    }

   start =atoi(until_char);
     //printf("\n\n%ld %s, %d\n",date, event[index].date, index);
    return start;
}

/*
* in:  
* out: 
* calls: 
* purpose: 
*/
void dt_increment(char *after, const char *before, int const num_days){
    struct tm temp_time, *p_temp_time;
    time_t    full_time;
    char      temp[5];

    memset(&temp_time, 0, sizeof(struct tm));
    sscanf(before, "%4d%2d%2d", &temp_time.tm_year,
        &temp_time.tm_mon, &temp_time.tm_mday);
    temp_time.tm_year -= 1900;
    temp_time.tm_mon -= 1;
    temp_time.tm_mday += num_days;

    full_time = mktime(&temp_time);
    after[0] = '\0';
    strftime(after, 8, "%Y%m%d", localtime(&full_time));
    strncpy(after + 8, before + 8, 132- 8); 
    after[132 - 1] = '\0';
}

///////////////////// military time to standard time ///////////////////////////////////////////////////////

/*
* in:  
* out: 
* calls: 
* purpose: 
*/
void convertTime(struct recipes *e, int index){
    char military_t[10];
    char model[50]= "";
    char start_time[20];
    char to[5] = " to ";
    char end_time[20]; 
    char semicolen[2] = ":";
   
    strncpy(military_t, e[index].time, 4 );
    int military_int = atoi(military_t);
    to_standard_time(military_int, start_time);
        // printf("%s\n",e[index].time );
        //printf("military End %s\n",military_t);  
        //printf("\nmilitary_start %d \n",military_int);
        //printf("IN FUNCTION %s\n",start_time);
      
    int ni = 0;
    for(int i = 6; i <10;i++){
        military_t[ni] = e[index].time[i];
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
    memset(e[index].time,'\0',27);  
    strncpy(e[index].time, model, 21 );
     //printf("\n%s\n",e[index].time);
    
}

/*
* in:  
* out: 
* calls: 
* purpose: 
*/
void to_standard_time(int military_time,   char *start_time){
    int hour_start = military_time / 100;
    int start_minute = (military_time) -(hour_start*100 );
    char min[10]; char semicolen[2] = ":";  char zero[5] = "0";
    char am[5] = " AM"; char pm[5] = " PM"; int am_pm = 0;

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

   // printf("\n%s\n",start_time);
}

///////////////////// filling out basic properties: all except rule /////////////////////////////////////////

/*
* in:  
* out: 
* calls: 
* purpose: 
*/
void fill_event_properties(struct recipes *all, int num){
    for(int i =0; i < num; i++){
            date( all, i);
            location( all, i);
            summary(all, i);
            get_time(all,i);
            convertTime(all,i);
            start_date(all,i);
    
        //printf("\n");
        //printf("start date int: %ld\n", all[i].start_date);  
        //printf("date to converted: %s\n", all[i].date);
        //printf("location: %s\n", all[i].location);
        //printf("summary: %s\n", all[i].summary);
        //printf("date: %s\n", all[i].time);
    } 
}

/*
* in:  
* out: 
* calls: 
* purpose: 
*/
void get_time(struct recipes *all, int i){
    int len;
    char word1[] = "DTEND";
    char DTEND[50];
    char word[] = "DTSTART";
    char DTSTART[50];
    len = find_key_words(all[i].event, word, DTSTART);
    len = find_key_words(all[i].event, word1, DTEND);
        char *ret;
        ret = strchr(DTSTART, 'T');
        ret++;
        strncpy(DTSTART, ret, 8);
        ret = strchr(DTEND, 'T');
        ret++;
           memset(DTEND,'\0',len-8);
        strncpy(DTEND, ret, 8);

    strncat(DTSTART, DTEND, 10);
      memset(all[i].time,'\0',len+3);
    strncpy(all[i].time,DTSTART, len-3);
    //printf("%s , %s\n",event[i].time ,DTSTART);
}

/*
* in:  
* out: 
* calls: 
* purpose: 
*/
void summary(struct recipes* all, int i){
        //printf("%p", all);
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
* in:  
* out: 
* calls: 
* purpose: 
*/
void location(struct recipes *all, int i){
    int len;
    char word2[] = "LOCATION";
    char LOCATION[40];
    len = find_key_words(all[i].event, word2, LOCATION);
      memset(all[i].location,'\0',len+5);
    strncpy(all[i].location, LOCATION,len );
    //printf("%s , %s\n",all.location ,LOCATION);
}

/*
* in:  
* out: 
* calls: 
* purpose: 
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
* in:  
* out: 
* calls: 
* purpose: 
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
* in:  
* out: 
* calls: 
* purpose: 
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

//////////////////////////* fill out all.events- get number of unique events */////////////////////////////

/*
* in:  char *word,char *buffer ,int noc, struct recipes *all
* out: number of events = num
* calls: struct recipes create_recipes(char *buffer, int startingindex, int i)
* purpose: used the index of "END:EVENT\n" to find the length of all event recipes
*/
int find_separte_events( char *word,char *buffer ,int noc, struct recipes *all){
    int wl = strlen(word);  
    int startingindex = 0;
    int found =0;
    int match =0; 
    int wordindex = 0; 
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

///////////////////////////* argc - filename - file into buffer *///////////////////////////////////////////

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
             //printf("%s",filename);
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
