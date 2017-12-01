#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define CLOCK_SIZE 4

struct record {
  int frame;
  int page;
  int use;
  int modify;
};

int pointer;

// Change everything to fprintf(outFilePtr, ...)

void writeClockToFile(FILE* filePtr, int page, char* operation, struct record* records) {
      int i = 0;
      fprintf(filePtr, " FRAME        PAGE       USE        MODIFY\n");

      // algorithm
      int p = pointer;
      int running = 1;
      // first pass
      while (running) {
        for (int i = 0; i < CLOCK_SIZE; i++) {

            if (records[i].page == page) {
              records[i].modify = 1;
              running = 0;
              break;
            }
        }
        if (running == 0) break;

        for (int i = 0; i < CLOCK_SIZE; i++) {
            int index = (p + i) % CLOCK_SIZE;
            if (records[index].use == 0 && records[index].modify == 0) {
              records[index].page = page;
              records[index].use = 1;
              if (operation == 'w') records[index].modify = 1;
              else records[index].modify = 0;
              pointer = (index + 1) % CLOCK_SIZE;
              running = 0;
              break;
            }
        }
        if (running == 0) break;

        // If step 3 fails, scan the clock again.  The first frame encountered with (u = 0; m = 1)
        // is selected for replacement.  During this scan, set the use bit to 0 for each frame that is bypassed.
        for (int i = 0; i < CLOCK_SIZE; i++) {
            int index = (p + i) % CLOCK_SIZE;
            if (records[index].use == 0 && records[index].modify == 1) {
              records[index].page = page;
              records[index].use = 1;
              if (operation == 'w') records[index].modify = 1;
              else records[index].modify = 0;
              pointer = (index + 1) % CLOCK_SIZE;
              running = 0;
              break;
            } else {
              records[index].use = 0;
            }
        }
        if (running == false) break;

        for (int i = 0; i < CLOCK_SIZE; i++) {
            int index = (p + i) % CLOCK_SIZE;
            if (records[index].use == 0 && records[index].modify == 0) {
              records[index].page = page;
              records[index].use = 1;
              if (operation == 'w') records[index].modify = 1;
              else records[index].modify = 0;
              pointer = (index + 1) % CLOCK_SIZE;
              running = 0;
              break;
            }
        }
        if (running == 0) break;

        // If step 3 fails, scan the clock again.  The first frame encountered with (u = 0; m = 1)
        // is selected for replacement.  During this scan, set the use bit to 0 for each frame that is bypassed.
        for (int i = 0; i < CLOCK_SIZE; i++) {
            int index = (p + i) % CLOCK_SIZE;
            if (records[index].use == 0 && records[index].modify == 1) {
              records[index].page = page;
              records[index].use = 1;
              if (operation == 'w') records[index].modify = 1;
              else records[index].modify = 0;
              pointer = (index + 1) % CLOCK_SIZE;
              running = 0;
              break;
            } else {
              records[index].use = 0;
            }
        }
        if (running == false) break;

        break;
      }


      for (  ; i < CLOCK_SIZE; i++) {
        if (pointer == i) {
            fprintf(filePtr, " %d            %d          %d           %d  <- next frame\n", records[i].frame, records[i].page, records[i].use, records[i].modify);
        } else {
            fprintf(filePtr, " %d            %d          %d           %d\n", records[i].frame, records[i].page, records[i].use, records[i].modify);
        }

      }
      fprintf(filePtr, " %s\n","");
};

int main()  {

      char inFileName[ ] = "testdata.txt";
      FILE *inFilePtr = fopen(inFileName, "r");
      if(inFilePtr == NULL) {
            printf("File %s could not be opened.\n", inFileName);
            exit(1);
      }

      char outFileName[ ] = "results.txt";
      FILE *outFilePtr = fopen(outFileName, "w");
      if(outFilePtr == NULL) {
            printf("File %s could not be opened.\n", outFileName);
            exit(1);
      }

      char *page;
      char *operation;
      struct record records[CLOCK_SIZE];
      pointer = 0;

      for (int i = 0; i < CLOCK_SIZE; i++) {
        records[i].frame = i;
        records[i].page = -1;
        records[i].use = 0;
        records[i].modify = 0;
      }

      fscanf(inFilePtr, "%d%c", &page, &operation);
      while(!feof(inFilePtr)) {
            fprintf(outFilePtr, "Page referenced: %d %c\n", page, operation);




            int page_int = page;
            writeClockToFile(outFilePtr, page_int, operation, records);
            fscanf(inFilePtr, "%d%c", &page, &operation);
      }//end while

      fclose(inFilePtr);
      fclose(outFilePtr);
      return 0;
};
