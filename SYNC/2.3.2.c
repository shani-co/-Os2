#include <pthread.h>

#include <semaphore.h>

#include <stdio.h>

#include <unistd.h>



#define N 5

#define THINKING 2

#define HUNGRY 1

#define EATING 0

#define L (phnum + 4) % N

#define R (phnum + 1) % N



int state[N];

int phil[N] = { 0, 1, 2, 3, 4 };



int fork_arr1[N] = {0};

sem_t mutex;

sem_t S[N];



void TEST(int phnum)

{

    if(fork_arr1[L] == EATING || fork_arr1[R] == EATING)

    {

        if (state[phnum] == HUNGRY)

        {

            if (fork_arr1[R] == EATING)

                fork_arr1[R] = phnum + 1;



            else if (fork_arr1[L] == EATING)

                fork_arr1[L] = phnum + 1;



            if (fork_arr1[R] == phnum + 1)

            {

                if (fork_arr1[L] == phnum + 1)

                {

                    // state that eating

                    state[phnum] = EATING;

                    sleep(2);



                    printf("Philosopher %d takes fork %d and %d\n",

                           phnum + 1, L + 1, phnum + 1);



                    printf("Philosopher %d is Eating\n", phnum + 1);


                    sem_post(&S[phnum]);

                }

            }

            sem_post(&S[phnum]);

        }

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

    TEST(phnum);



    sem_post(&mutex);



    // if unable to eat wait to be signalled

    sem_wait(&S[phnum]);



    sleep(1);

}



// put down chopsticks

void put_fork1(int phnum)

{



    sem_wait(&mutex);



    // state that thinking

    state[phnum] = THINKING;



    printf("Philosopher %d putting fork %d and %d down\n",

           phnum + 1, L + 1, phnum + 1);

    printf("Philosopher %d is thinking\n", phnum + 1);



    // change here

    fork_arr1[L+1] = EATING;

    fork_arr1[phnum+1] = EATING;

    //



    TEST(L);

    TEST(R);



    sem_post(&mutex);

}



void* philospher(void* num)

{



    while (1) {



        int* i = num;



        sleep(1);



        take_fork(*i);



        sleep(0);



        put_fork1(*i);

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