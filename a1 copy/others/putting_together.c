#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LINE_LEN 132
#define MAX_EVENTS 500

/*
Got where i wanted
**** memset storage -- everything---- before you use it
- fix the time function

RRULE and frequency:
- how many times do I repeat this event?
- what stays the same
- time and date: fix

*/

struct recipes{
    char event[500];
    char  date[50];
    char  time[50];
    char  summary[50];
    char  location[50];
    char  rule[200];
}recipes;

int sortArugments(int argc, char *argv[], char *buffer);
int IntoString (char *filename, char *buffer);

int find_recurances_of_word( char *word,char *buffer ,int noc, struct recipes *all);
struct recipes  store_until(char *buffer, int startingindex, int i); 
// change this one to a stuct (output a struct)

int find_key_words(char *buffer, char *word, char *newWord);
void summary(struct recipes *event, int i);
void location(struct recipes *event, int i);
void date(struct recipes *event, int i);
void dt_format(char *formatted_time, const char *dt_time, const int len);
void get_time(struct recipes *event, int i);
void addjustTime(struct recipes *e, int index);

void printEvent(struct recipes event);

int main(int argc, char *argv[]){

    /* get file into array */
    char buffer[5500];
    int numofchar;

    numofchar = sortArugments(argc, argv, buffer);
    //printf("\n%s\n", buffer);

    struct recipes all[50];
    int num = find_recurances_of_word("END:VEVENT\n", buffer, numofchar, all);
   
   
    for(int i =0; i < num; i++){
        //printf("%s\n",all[i].event );
        date( all, i);
        //printf("%d: %s\n", i, all[i].date);
        location( all, i);
        // printf("%d: %s\n", i, all[i].location);
        summary(all, i);
        // printf("%d: %s\n", i, all[i].summary);
        get_time(all,i);
         addjustTime(all,i);
        printEvent(all[i]);
       printf("\n");  
    }
 
     
//printf("%s",all[16].event);

    exit(0);
}

/* put into lines  scan for*/
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
filling out the pain proporties
*/
int find_key_words(char *buffer, char *word, char *newWord){
    char *ret;
    ret = strstr(buffer, word);
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

void summary(struct recipes* event, int i){
        //printf("%p", event);
        //printf("%s", event[i].event);
       
       int len;
        char word3[8];
        strncpy(word3, "SUMMARY:", 7 );
        char SUMMARY[50];
        len = find_key_words(event[i].event, word3, SUMMARY);
        memset(event[i].summary,'\0',len+5);
        strncpy(event[i].summary, SUMMARY, len );
    //printf("%s\n",event[i].summary);

}

void location(struct recipes *event, int i){
    int len;
    char word2[] = "LOCATION";
    char LOCATION[40];
    len = find_key_words(event[i].event, word2, LOCATION);
      memset(event[i].location,'\0',len+5);
    strncpy(event[i].location, LOCATION,len );
    //printf("%s , %s\n",event.location ,LOCATION);
}

void date(struct recipes *event, int i){
    int len;
    char word[] = "DTSTART";
    char DTSTART[50];
    len = find_key_words(event[i].event, word, DTSTART);
      memset(event[i].date,'\0',len+3);
    dt_format(event[i].date, DTSTART, 50);
     
    //printf("%s \n",event[i].date);
}

void dt_format(char *formatted_time, const char *dt_time, const int len)
{
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

void get_time(struct recipes *event, int i){
    int len;
    char word1[] = "DTEND";
    char DTEND[50];
    char word[] = "DTSTART";
    char DTSTART[50];
    len = find_key_words(event[i].event, word, DTSTART);
    len = find_key_words(event[i].event, word1, DTEND);
        char *ret;
        ret = strchr(DTSTART, 'T');
        ret++;
        strncpy(DTSTART, ret, 8);
        ret = strchr(DTEND, 'T');
        ret++;
           memset(DTEND,'\0',len-8);
        strncpy(DTEND, ret, 8);

    strncat(DTSTART, DTEND, 10);
      memset(event[i].time,'\0',len+3);
    strncpy(event[i].time,DTSTART, len-3);
    //printf("%s , %s\n",event[i].time ,DTSTART);

}



/*
store all recipes 
*/
int find_recurances_of_word( char *word,char *buffer ,int noc, struct recipes *all){
    int wl = strlen(word);
    int startingindex = 0;
    int found = 0;
    int match =0;
    int wordindex = 0; 
    for(int i = 0 ; i < noc; i++){
        if(buffer[i] == word[wordindex]){
            wordindex++;
            match++;
            if(match == wl){
                all[found] = store_until(buffer, startingindex, i);
                startingindex = i;
                found++;
            }
          
        }else{
            match= 0;
            wordindex = 0; 
        }
    }
    return found;
}

struct recipes store_until(char *buffer, int startingindex, int i){
    struct recipes new;
        int length = i  - startingindex;
        char *event2 = &buffer[startingindex];
        memset(new.event,'\0',length+5);
        strncpy(new.event,event2,length);
    return new;
                 
 }

/*
take filename and puts it into bufffer
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
    numofchar = IntoString(filename, buffer);
    
    return numofchar;
}

int IntoString( char *filename, char *buffer){
    char c;  FILE *fptr;
    int numOfchar=1;

    // Open file
    fptr = fopen(filename, "r");
    if (fptr == NULL){
        printf("\n Cannot open file \n");
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
   (input) e[index].time  = 1800 00 2100 00 
   (output) e[index].time =  6:00 PM to 9:00 PM

   1. separates start and end - 180000 210000 
   2. change to integers
   3. change to standard
   4. add strings together

   Still needs to be able do minuites: make a mini for loop for those indexes

*/
void addjustTime(struct recipes *e, int index){
    char model[50]= "";
    char start[10];

    // convertions start and end time
   // printf("%s\n",e[index].time );
    strncpy(start, e[index].time, 6 );
   
    int military_start = atoi(start);
    char end[5];
    int ni = 0;
    for(int i = 6; i <11;i++){
        end[ni] = e[index].time[i];
        //printf("%c",end[ni]);
        ni++;
    }
     //printf("  this is end %s\n",end);
    int military_end = atoi(end);
   // printf("%d",military_end);

    int hour_start = military_start / 10000;
    int start_minute = military_start / 100;
    int hour_end = military_end / 1000;
    int end_minute = military_end / 100;
        
    char start_time[20];
    char zeros[20] = ":00";
    char semicolen[2] = ":";
    char to[5] = " to ";
    char am[5] = " AM";
    char pm[5] = " PM";
    int am_pm = 0;

    if( hour_start > 11){
        if(hour_start > 12){
        hour_start = hour_start -12 ; // changes to standard   
        }      
        am_pm = 1;
     }

    sprintf(start_time, "%d", hour_start);
    strcat(start_time, zeros );
        if( am_pm == 1){
            strcat(start_time, pm );
        }else{
            strcat(start_time, am );
        }
    strcat(start_time, to);
    strcat(model, start_time);
        
    am_pm =0;
    //printf("%d\n",hour_end);
    if (hour_end > 11){
            if(hour_end > 12){
                hour_end =  hour_end -12 ; // changes to standard   
                //printf("%d\n",hour_end);
            }      
            am_pm = 1;
    }
    
    char end_time[20];
    sprintf(end_time, "%d", hour_end);
    strcat(end_time, zeros );

        if( am_pm == 1){
            strcat(end_time, pm );
        }else{
            strcat(end_time, am );
        }
    strcat(model, end_time);
    strcat(model, semicolen);
        memset(e[index].time,'\0',27);  
    strncpy(e[index].time, model, 21 );
    
}
