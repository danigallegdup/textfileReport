#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
make a function that can print the results out perfectly
*/
 
 /*
    BEGIN:VCALENDAR
    BEGIN:VEVENT
    DTSTART:20210214T180000
    DTEND:20210214T210000
    LOCATION:Burger King
    SUMMARY:Romantic dinner with Chris
    END:VEVENT
    END:VCALENDAR
 */


/*


*/

struct Event{
    char  date[100];
    char  weekday[100];
    char  time[100];
    char  summary[100];
    char  location[100];
}Event;  


int IntoString( char *filename, char *buffer);
void filestuff(int argc, char **argv);
struct Event createEvents( char *date, char *weekday,char *time,char *summary,char *location);
void printEvent(struct Event event);

//void createEvent

int main(int argc, char **argv){

    char  date[100] = "February 14, 2021";
    char  weekday[100] = "Sun";
    char  time[100] ="6:00 PM to  9:00 PM:";
    char  summary[100] = "Romantic dinner with Chris";
    char  location[100] = "Burger king";

struct Event event;
event = createEvents(date, weekday,time,summary,location);
printEvent(event);
printf("\n");
   
       
  

/*
February 14, 2021 (Sun)
‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐ 
6:00 PM to  9:00 PM: Romantic dinner with Chris {{Burger king}} 

*/   
	return 0;
}


void filestuff(int argc, char **argv){
      if (argc < 2) {
        fprintf(stderr, "usage: %s filename \n", argv[0]); exit(1);
        }

        printf ("opening file name: %s\n",argv[1]);
        char buffer[4000];
        int numofchar;

       numofchar = IntoString(argv[1], buffer);
        //printf("\n%s\n", buffer);

}
/*
take filename and puts it into bufffer
*/
int IntoString( char *filename, char *buffer){
    char c;  FILE *fptr;
    int numOfchar=1;

    // Open file
    fptr = fopen(filename, "r");
    if (fptr == NULL){
        printf("Cannot open file \n");
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

/*
assume that it comes in lines: if 
*/
struct Event createEvents(char *date, char *weekday,char *time,char *summary,char *location){

    struct Event event;
        strcpy(event.date,date);
        strcpy(event.weekday, weekday);
        strcpy(event.time, time);
        strcpy(event.summary, summary );
        strcpy(event.location, location);
    return event;
}


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
    







