// CS475W lab05 Mutual Exclusion
// Simulation of process for the classic Dinning Philosopher problem for 'x' amount of time
// Due time: 4/1/2021 Thursday
// Student name: Bishesh Tuladhar, Xavier Betancourt

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

// Global array to keep track of which chopsticks are available -- 1 means
// available, 0 means not. Philosopher i will eat with chopsticks i, and i+1
// (addition done modulo 5, so philosopher 4 will use 4 and 0.
int chopsticks[5] = {1,1,1,1,1};
pthread_mutex_t lock[5] = PTHREAD_MUTEX_INITIALIZER;

#define NUM_PHILOSOPHERS 5 // 5 philosophers, numbered 0 to 4

/*
    Each philosopher, based on the index (odd or even), will proceed to pick up chopstics.
    First, odd index philosophers get to pick up their left and then right chopstick. When they are
    done, even index philosphers get to pick their right and then left chopsticks. This way, we can also
    prevent a deadlock when proceeding sequentially.
*/
int pickUpChop(int philosopher_number);

/*
    Unlock each mutex for left or right-hand chopstick that a philosopher grabbed
*/
void putDownChop(int philosopher_number);


/*
  Our thread function -- seed the random number generator, and while
  (true), wait for 0-5 seconds (thinking), pickup chopsticks (if it returns
  true or 1, we exit), then wait again for 0-5 seconds (eating) and return
  them
*/
void *philosopher( void*);



int pickUpChop(int philosopher_number)
{

  printf("Philosopher %d has started his meal!\n",philosopher_number);
  //Checks if philosopher num is even or odd.
  if (philosopher_number % 2 != 0) { //Case where philosopher num is odd
    while(chopsticks[(philosopher_number+1)%5] < 1){
      //Spin
    }
    //pthread_mutex_lock(&lock[(philosopher_number+1)%5]);
    chopsticks[(philosopher_number+1)%5]--;

    sleep(rand() % 4 + 1);

    while(chopsticks[philosopher_number] < 1){
      //Spin
    }
    //pthread_mutex_lock(&lock[philosopher_number]);
    chopsticks[philosopher_number]--;
  }

  else { //Case where philosopher num is even
    while(chopsticks[philosopher_number] < 1){
      //Spin
    }

    //pthread_mutex_lock(&lock[philosopher_number]);
    chopsticks[philosopher_number]--;
    
    sleep(rand() % 4 + 1);

    while(chopsticks[(philosopher_number+1)%5] < 1){
      //Spin
    }
    
    //pthread_mutex_lock(&lock[(philosopher_number+1)%5]);
    chopsticks[(philosopher_number+1)%5]--;
  }

  return 0;
}


// Function to return the chopsticks
void putDownChop(int philosopher_number)
{
  printf("Philosopher %d has returned his chopsticks. \n",philosopher_number);
  chopsticks[(philosopher_number + 1)%5]++;
  pthread_mutex_unlock(&lock[(philosopher_number+1)%5]);
  chopsticks[philosopher_number]++;
  pthread_mutex_unlock(&lock[philosopher_number]);
}


void *Philosopher(void *num)
{
  // Here we cast the argument as an integer and save it, as before
  int philosopher_number = *((int *)num);


  // Create variables to keep track of time
  time_t current_time;
  time(&current_time);

  struct tm *mytime = localtime(&current_time);
  int startime = mytime->tm_sec;

  // Seed the random number generator
  srand(time(NULL)+philosopher_number);
  int count =0;

  while(count < 60)
  {
    count++;
    //Thinking time
    sleep(rand()%5);

    // If that function returned 1, then the philosopher couldn't pick up a fork.
    if(pickUpChop(philosopher_number))
      continue;

    sleep(rand()%5); //eating time

    //DEADLOCK CRETED HERE : The philosopher does not put down the chopstick if they do not finish the meal, and since they are waiting for 2 chopstick, all the philosopher are in deadlock

    //putDownChop(philosopher_number);
  
  }
}


int main()
{
  pthread_t philosophers[5];

  int i=0;
  int args[5];
  for(i=0;i<5;i++)
  {
    args[i] = i;
    pthread_create(&philosophers[i],NULL,Philosopher,(void*)&args[i]);
  }

  for(i=0;i<5;i++)
  {
    pthread_join(philosophers[i],NULL);
  }
  return 0;
}