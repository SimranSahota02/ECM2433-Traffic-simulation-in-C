#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

/*Structure to contain results*/
struct Results{
   int lIterTotal;
   int rIterTotal;
   int lIterMax;
   int rIterMax;
   int lCarTotal;
   int rCarTotal;
   int lToClear;
   int rToClear;
};
/*Structure for Car with pointer to next car in queue*/
struct Car{
   int iterationCount;
   struct Car* next;
};
/*Structure for Car queues*/
struct Queue{
   struct Car *front, *rear;
};
/*Function prototypes*/
struct Results runOneSimulation(int lLightTime, int rLightTime, int lTrafficRate, int rTrafficRate);
void runSimulations(int lLightTime, int rLightTime, int lTrafficRate, int rTrafficRate);

/*main function*/
int main(){
   /*set varibles*/
   int correctInputs=0;
   int inp1;
   int inp2;
   int inp3;
   int inp4;
   correctInputs=0;
   fprintf(stdout,"Welcome to the traffic simulation program\n");
   fprintf(stdout,"You will be prompted four integer arguments between 0 and 100, for the left traffic rate and light period, followed by right traffic rate and light period\n");
   /*use conditionals to check if each input is good*/
   fprintf(stdout,"Left traffic rate: ");
   scanf("%d", &inp1);
   if(inp1<101){
      if(inp1>0){
         correctInputs++;
      }
   }
   fprintf(stdout,"\nLeft light period: ");
   scanf("%d", &inp2);
   if(inp2<101){
      if(inp2>0){
        correctInputs++;
      }
   }
   fprintf(stdout,"\nRight traffic rate: ");
   scanf("%d", &inp3);
   if(inp3<101){
      if(inp3>0){
        correctInputs++;
      }
   }
   fprintf(stdout,"\nRight light period: ");
   scanf("%d", &inp4);
   if(inp4<101){
      if(inp4>0){
        correctInputs++;
      }
   }
   if(correctInputs!=4){
      fprintf(stdout,"\nIncorrect inputs\n");
   } else{  
   runSimulations(inp2,inp4,inp1,inp3);
   }
   return 0;
}

/*runSimulations function*/
void runSimulations(int lLightTime, int rLightTime, int lTrafficRate, int rTrafficRate){
   /*set results variables*/
   int lTotalCars =0;
   int rTotalCars =0;
   int lAverageIterationCount =0;
   int rAverageIterationCount =0;
   int lMaxIterationCount =0;
   int rMaxIterationCount =0;
   int lClearance =0;
   int rClearance =0;
   /* run one sim 100 times*/
   int i;
   for(i=0;i<100;++i){
      struct Results resultsQ = runOneSimulation(lLightTime, rLightTime, lTrafficRate, rTrafficRate);
      /*read results from sim*/
      lTotalCars += resultsQ.lCarTotal;
      rTotalCars += resultsQ.rCarTotal;
      lAverageIterationCount += (int)(resultsQ.lIterTotal / resultsQ.lCarTotal);
      rAverageIterationCount += (int)(resultsQ.rIterTotal / resultsQ.rCarTotal);
      lMaxIterationCount += resultsQ.lIterMax;
      rMaxIterationCount += resultsQ.rIterMax;
      lClearance += resultsQ.lToClear;
      rClearance += resultsQ.rToClear;
   }
   /*print results*/
   fprintf(stdout,"Parameter values:\n");
   fprintf(stdout,"   from left:\n");
   fprintf(stdout,"      traffic arrival rate: %d\n",lTrafficRate);
   fprintf(stdout,"      traffic light period: %d\n",lLightTime);
   fprintf(stdout,"   from right:\n");
   fprintf(stdout,"      traffic arrival rate: %d\n", rTrafficRate);
   fprintf(stdout,"      traffic light period: %d\n", rLightTime);
   fprintf(stdout,"Results (averaged over 100 runs):\n");
   fprintf(stdout,"   from left:\n");
   fprintf(stdout,"      number of vehicles: %d\n",lTotalCars /100);
   fprintf(stdout,"      average waiting time: %d\n", lAverageIterationCount);
   fprintf(stdout,"      maximum waiting time: %d\n", lMaxIterationCount / 100);
   fprintf(stdout,"      clearance time: %d\n", lClearance /100);
   fprintf(stdout,"   from right:\n");
   fprintf(stdout,"      number of vehicles: %d\n",rTotalCars /100);
   fprintf(stdout,"      average waiting time: %d\n", rAverageIterationCount);
   fprintf(stdout,"      maxiumum waiting time: %d\n", rMaxIterationCount /100);
   fprintf(stdout,"      clearance time: %d\n", rClearance /100);
   return;
}

/*runOneSimulation function returns Results struct*/
struct Results runOneSimulation(int lLightTime, int rLightTime, int lTrafficRate, int rTrafficRate){
   /*create traffic Queues*/
   struct Queue* leftQ = (struct Queue*)malloc(sizeof(struct Queue));
   leftQ->front = leftQ->rear = NULL;
   struct Queue* rightQ = (struct Queue*)malloc(sizeof(struct Queue));
   rightQ->front = rightQ->rear = NULL;
   /*initialise other variables*/
   struct Results resultsQ;
   int carCount=0;
   int currentLightTime=0;
   int lCarTotal=0;
   int rCarTotal=0;
   int currentGreen=0;
   int lIterTotal=0;
   int rIterTotal=0;
   int lIterMax=0;
   int rIterMax=0;
   /*set up random generator*/
   const gsl_rng_type *T;
   gsl_rng *r;
   T = gsl_rng_default;
   r = gsl_rng_alloc(T);
   gsl_rng_set(r,time(0));
   /*loop 500 iterations where car can be added*/
   int i;
   for(i=0; i<500; ++i){
      /*swap which light is green*/
      currentLightTime++;
      int lightChanged=0; 
      if(currentLightTime == lLightTime){
         if(currentGreen==0){
            currentGreen = 1;
            lightChanged = 1;
         }
      }
      if(currentLightTime == rLightTime){
         if(currentGreen==1){
            currentGreen = 0;
            lightChanged = 1;
         }
      }
      if(lightChanged==1){
         currentLightTime = 0;
      }
      else{
         /*add car to queue*/
         int randomL = (int)gsl_ran_flat(r,1,100);
         if(randomL<lTrafficRate){
            struct Car* temp = (struct Car*)malloc(sizeof(struct Car));
            temp->iterationCount = 0;
         
            if(leftQ->rear == NULL){
            leftQ->front = leftQ->rear = temp;
            }
         leftQ->rear->next = temp;
         leftQ->rear = temp;
         carCount++;
         lCarTotal++;
         }
         int randomR = (int)gsl_ran_flat(r,1,100);
         if(randomR<rTrafficRate){
            struct Car* temp = (struct Car*)malloc(sizeof(struct Car));
            temp->iterationCount = 0;
   
            if(rightQ->rear == NULL){
            rightQ->front = rightQ->rear = temp;
            }
         rightQ->rear->next = temp;
         rightQ->rear = temp; 
         carCount++;
         rCarTotal++;
         }
         /*remove car from queue*/
         int randomPass = (int)gsl_ran_flat(r,0,10);
         if(randomPass<5){
            carCount--;
            if(currentGreen==0){
              if(leftQ->front == NULL){
               } else{
                  struct Car* temp = leftQ->front;
                  lIterTotal+=(leftQ->front->iterationCount);
                  if((leftQ->front->iterationCount)>lIterMax){
                     lIterMax=(leftQ->front->iterationCount);
                  }
                  leftQ->front = leftQ->front->next;
                  if(leftQ->front == NULL){
                  leftQ->rear = NULL;
                  }
                  free(temp);
               }
            } else{
              if(rightQ->front == NULL){
               } else{
                  struct Car* temp = rightQ->front;
                  rIterTotal+=(rightQ->front->iterationCount);
                  if((rightQ->front->iterationCount)>rIterMax){
                     rIterMax=(rightQ->front->iterationCount);
                  }
                  rightQ->front = rightQ->front->next;
                  
                  if(rightQ->front == NULL){
                  rightQ->rear = NULL;
                  }
                  free(temp);
               }
            }
         }
      }
   }
   /*iterate with no new cars until clear*/
   int lToClear=0;
   int rToClear=0;
   while(carCount!=0 ){
      /*same actions as iterating with adding car*/
      currentLightTime++;
      if(currentGreen==0){
         lToClear++;
      } else{
      rToClear++;
      }
      int lightChanged=0;
      if(currentLightTime==lLightTime){
         if(currentGreen==0){
            currentGreen=1;
            lightChanged = 1;
         }
      }
      if(currentLightTime==rLightTime){
         if(currentGreen==1){
            currentGreen=0;
            lightChanged = 1;
         }
      }
      if(lightChanged==1){
         currentLightTime = 0;
      }
      else{
         int randomPass = (int)gsl_ran_flat(r,0,10);
         if(randomPass<5){
            carCount--;
            if(currentGreen==0){
               if(leftQ->front == NULL){
               } else{
                  struct Car* temp = leftQ->front;
                  lIterTotal+=(leftQ->front->iterationCount);
                  if((leftQ->front->iterationCount)>lIterMax){
                     lIterMax=(leftQ->front->iterationCount);
                  }
                  leftQ->front = leftQ->front->next;

                  if(leftQ->front == NULL){
                  leftQ->rear = NULL;
                  }
                  free(temp);
               }
            } else{
               if(rightQ->front == NULL){
               } else{
                  struct Car* temp = rightQ->front;
                  rIterTotal+=(rightQ->front->iterationCount);
                  if((rightQ->front->iterationCount)>rIterMax){
                     rIterMax=(rightQ->front->iterationCount);
                  }
                  rightQ->front = rightQ->front->next;
                  
                  if(rightQ->front == NULL){
                  rightQ->rear = NULL;
                  }
                  free(temp);
               }
            }
         }
      }
   }
   resultsQ.lIterMax = lIterMax;
   resultsQ.rIterMax = rIterMax;
   resultsQ.lIterTotal = lIterTotal;
   resultsQ.rIterTotal = rIterTotal;
   resultsQ.lToClear = lToClear;
   resultsQ.rToClear = rToClear;
   resultsQ.lCarTotal = lCarTotal;
   resultsQ.rCarTotal = rCarTotal;
   return resultsQ;
}

