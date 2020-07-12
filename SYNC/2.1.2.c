#include <pthread.h>
#include <stdio.h>

#include <unistd.h>

#include "stdbool.h"
#include <semaphore.h>





#define N 5
#define EATING 0

#define L (phnum + 4) % N

#define R (phnum + 1) % N

#define EVEN 2

#define THINKING 2

#define HUNGRY 1





bool avl_shani[N] = {false};

int state[N];

int phil[N] = { 0, 1, 2, 3, 4 };



bool free = false, not_free = true;

sem_t mutex;

sem_t S[N];

void t(int phnum)

{

    if(state[phnum] == HUNGRY && (avl_shani[L] == free || avl_shani[R] == free))

    {

        if(avl_shani[L] == free)

            avl_shani[L] = not_free;



        else if(avl_shani[R] == free)

            avl_shani[R] = not_free;



        if(avl_shani[L] == not_free && avl_shani[R] == not_free)

        {

            // state that eating

            state[phnum] = EATING;

            sleep(2);



            printf("Philosopher %d takes fork %d and %d\n",

                   phnum + 1, L + 1, phnum + 1);



            printf("Philosopher %d is Eating\n", phnum + 1);



            // sem_post(&S[phnum]) has no effect

            // during takefork

            // used to wake up hungry philosophers

            // during putfork

            sem_post(&S[phnum]);

        }

        sem_post(&S[phnum]);

    }

}



// take up chopsticks

void take_fork(int phnum)

{



    sem_wait(&mutex);



    // state that hungry

    state[phnum] = HUNGRY;



    printf("Philosopher %d is Hungry\n", phnum + 1);



    // eat if neighbours are not eating

    t(phnum);



    sem_post(&mutex);



    // if unable to eat wait to be signalled

    sem_wait(&S[phnum]);



    sleep(1);

}



// put down chopsticks 

void put_fork(int phnum)

{

    sem_wait(&mutex);



    // state that thinking

    state[phnum] = THINKING;



    printf("Philosopher %d putting fork %d and %d down\n",

           phnum + 1, L + 1, phnum + 1);

    printf("Philosopher %d is thinking\n", phnum + 1);



    avl_shani[phnum+1] = free;

    avl_shani[L+1] = free;



    t(L);

    t(R);



    sem_post(&mutex);

}



void* philospher(void* num)

{



    while (1) {



        int* i = num;



        sleep(1);



        take_fork(*i);



        sleep(2);



        put_fork(*i);

    }

}



int main()

{



    int i;

    pthread_t thread_id[N];



    // initialize the mutexes

    sem_init(&mutex, 0, 1);



    for (i = 0; i < N; i++)



        sem_init(&S[i], 0, 0);



    for (i = 0; i < N; i++) {



        // create philosopher processes

        pthread_create(&thread_id[i], NULL,

                       philospher, &phil[i]);



        printf("Philosopher %d is thinking\n", i + 1);

    }



    for (i = 0; i < N; i++)



        pthread_join(thread_id[i], NULL);

} 