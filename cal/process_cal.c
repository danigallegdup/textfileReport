#include <stdlib.h>
#include <stdio.h>
#include <string.h>





/*
BEGIN:VCALENDAR
VERSION:A
BEGIN:VEVENT
DTSTART:20210102T111500
DTEND: 20210102T123000
RRULE:FREQ=WEEKLY;WKST=MO;UNTIL=20211002T235959;BYDAY=SA
LOCATION:The Bumptious Barista
SUMMARY:Coffee with Pat
END:VEVENT
END:VCALENDAR


 */

struct Event{
    char  date[100];
    char  weekday[100];
    char  time[100];
    char  summary[100];
    char  location[100];
    char  rule[200];
}Event;  


void add_event(struct Event *all_events,int currentIndex,struct Event event);
void firstphase();
void createEvents( char *date, char *weekday,char *time,char *summary,char *location, char *rule,
                             struct Event *all_events, int currentIndex );
                
void printEvent(struct Event event);

//void createEvent

int main(int argc, char **argv){
firstphase();
   
	return 0;
}


void firstphase(){

 struct Event all_events[1000];


    char  date[100] = "February 14, 2021";
    char  weekday[100] = "Sun";
    char  time[100] ="6:00 PM to 9:00 PM:";
    char  summary[100] = "Romantic dinner with Chris";
    char  location[100] = "Burger King";
    char  rule[200] = "RRULE:FREQ=WEEKLY;WKST=MO;UNTIL=2021 10 02 T 23:59:59;BYDAY=SA";



    createEvents("20210214-20210214", "UD","T180000-T210000",summary,location,rule, all_events, 0); 
    createEvents("2021/02/14","1","18:00:00-21:00:00",summary,location,rule, all_events, 1); //sort here
    createEvents(date, weekday,time,summary,location,rule, all_events, 2);


    printEvent(all_events[0]);
    printEvent(all_events[1]);
    printEvent(all_events[2]);
    printf("\n");

}

void add_event(struct Event *all_events,int currentIndex, struct Event newEvent){
    all_events[currentIndex]=newEvent ;
}

/*
with all needed material creates and returns event
*/
void createEvents(char *date, char *weekday,char *time,char *summary,char *location, char *rule,
                        struct Event *all_events, int currentIndex ){

    struct Event event;
        strcpy(event.date,date);
        strcpy(event.weekday, weekday);
        strcpy(event.time, time);
        strcpy(event.summary, summary );
        strcpy(event.location, location);
        strcpy(event.rule, rule);

    add_event( all_events, currentIndex, event);
}

/*
prints one event that is passed through param
*/
void printEvent(struct Event event){
     char line[100] ="\n\t‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐\n\t";
    printf("\n\t%s (%s)%s%s %s {{%s}}\n",
    event.date,
    event.weekday,
    line,
    event.time,
    event.summary,
    event.location); 
}
    









