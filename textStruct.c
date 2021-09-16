#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_STRING_SIZE 4000

/*
needes things:
  find word- split into 2 functions
  add a space with words with apostrophies
  have only print function be voids
  nicely commented: include a flow chart and picture of different outputs
  make it have a command line - to specify output: anything : letter, full report, half report, letter a, search--word 
*/

struct Report {
   char  word[50];
    int line_infile;
    int wordlength;
    int repetition;
} Report;  

void printReport(struct Report *all_words, int sizeA);
int IntoString( char *filename, char *buffer);
struct Report createWord(char word[], int line, int length, int rep);
void findWords(char *buffer, int numofchar);
void sortApha(struct Report *all_words, int size);
void sortLength(struct Report *all_words, int size);
void Repetitions(struct Report *all_words, int size);

void startWith(char letter); // print out numbers
void quickRecap(int numOfchar, int numofWords, struct Report mostFrequent, struct Report longest_word); // prints before

int main(int argc, char **argv){

  if (argc < 2) {
        fprintf(stderr, "usage: %s filename \n", argv[0]); exit(1);
        }

       // printf ("opening file name: %s\n",argv[1]);
        char buffer[MAX_STRING_SIZE];
        int numofchar;

       numofchar = IntoString(argv[1], buffer);

  //printf("\n%s\n", buffer);
  findWords(buffer,numofchar );

   
	return 0;
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


void findWords(char *buffer, int numofChar){
  
  // struct array
  struct Report all_words[1000];
  int ReportIndex = 0; // number of structs in array
  
  // reused through iteration to make structs
  char word[50]; //  word
  int line= 1; //  line
  int wordIndex = 0; // length

// itterate through text
  for(int i = 0 ;i<numofChar+1; i++ ){
    if(buffer[i] != '\n' && buffer[i] != ' ' && buffer[i] != '.' && buffer[i] != ','  && buffer[i] != ':'  ) {
        word[wordIndex]= buffer[i]; // adding all joint letters to make a word
        wordIndex++;

    }else{ // assuption that there extranious spaces (if yes skip of wordindex > 2)
        
         for(int i = wordIndex; i < 20; i++){
            word[i] = ' ';
          }

        // create and add to struct
        if(wordIndex> 0){
          struct Report NewWord;
          NewWord= createWord(word,line, wordIndex, 0);
          all_words[ReportIndex] = NewWord;
          ReportIndex++;
        }
       
        // updates for next itteration
          if(buffer[i]== '\n'){
            line++;
          }
         wordIndex= 0;
    } 
  }
    sortApha(all_words, ReportIndex);
    //sortLength(all_words,  ReportIndex);

 
   

}

/*
creates word
*/
struct Report createWord(char word[], int line, int length, int rep ){
    struct Report NewWord;
    strcpy(NewWord.word, word );
    NewWord.line_infile = line;
    NewWord.wordlength = length;
    NewWord.repetition = 1;
   return NewWord;
}

/* takes in struct: sorts aphabetically by literally bubble sort */ 
void sortApha(struct Report *all_words, int size){
     struct Report NewWord;
    for(int i =0;i<size;i++)
    { for(int j=i+1;j< size ;j++)
        {if(strcmp(all_words[i].word,all_words[j].word)>0)
          {
              NewWord =  all_words[i];
              all_words[i] =all_words[j];
              all_words[j]= NewWord;
          }
        }
   }

  Repetitions(all_words, size);
  printReport(all_words, size);
}


void sortLength(struct Report *all_words, int size){
     struct Report NewWord;
    char s[100];
    for(int i =0;i<size;i++)
    { for(int j=i+1;j< size ;j++)
        {if(all_words[i].wordlength > all_words[j].wordlength)
          {
              NewWord =  all_words[i];
              all_words[i] =all_words[j];
              all_words[j]= NewWord;
          }
        }
   }

  //Repetitions(all_words, size);
  printReport(all_words, size);

}


void Repetitions(struct Report *all_words, int size){
    for(int i =0;i<size;i++)
    { for(int j=i+1;j< size ;j++)
        {if(strcmp(all_words[i].word,all_words[j].word) == 0)
          {
               all_words[i].repetition = all_words[i].repetition+ 1 ;
               all_words[j].repetition = all_words[i].repetition;
          }
        }
   }

}

/*
Prints out array of Reports - takes in number of words
          ONLY AFTER ALPHA
*/
void printReport(struct Report *all_words, int size){
   	printf("\n\t\t\t%s\n" ,"REPORT");
    for(int i =0; i< size; i++){
      if( all_words[i].repetition > 1){
         printf("\t%s: length: %d repetition: %d line: [%d",
         all_words[i].word,
         all_words[i].wordlength, 
        all_words[i].repetition,
        all_words[i].line_infile);
        for(int r = 1; r< all_words[i].repetition; r++ ){
          printf(",%d",all_words[r+all_words[i].repetition].line_infile);
        }
        printf("]\n");
        i = all_words[i].repetition +i;
      }

      printf("\t%s: length: %d repetition: %d line: [%d]\n",
      all_words[i].word,
      all_words[i].wordlength, 
      all_words[i].repetition,
      all_words[i].line_infile);
  }
	printf("\n" );
}
