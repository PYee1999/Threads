#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

static sem_t connected_lock;    // Lock thread
static sem_t operators;         // Operator thread
static sem_t caller_id;         // Caller ID thread
static int NUM_OPERATORS = 5;   // Number of operators
static int NUM_LINES = 8;       // Number of phone lines
static int connected = 0;       // Callers that are connected
int next_id = 0;                // Global Caller ID

void* phonecall(void* vargp);

int main(int argc, char *argv[]) // Main method: taking in number of phone calls as argument
{
    if (argc < 2)   // Check that user has input a number of phone calls
    {
        printf("Error: Did not input number of phone calls.\n");
        return -1;
    }
    
    int numPhoneCalls = atoi(argv[1]);      // Get number of phone calls from argument
    pthread_t phoneCallList[numPhoneCalls]; // Create list of phone calls
    
    sem_init(&operators, 0, NUM_OPERATORS);     // Set most number of operators allowed
    sem_init(&connected_lock, 0, 1);            // Set most number of connections allowed
    sem_init(&caller_id, 0, numPhoneCalls);     // Set most number of caller ids allowed
    
    for (int i = 0; i < numPhoneCalls; i++) // Create all of the phone call threads
    {
        pthread_create(&phoneCallList[i], NULL, phonecall, NULL);
    }
    
    for (int j = 0; j < numPhoneCalls; j++) // Join the list of phone calls; wait until all phone calls complete
    {
        pthread_join(phoneCallList[j], NULL);
    }
    
    if (sem_destroy(&operators) < 0)        // Destroy operators semaphore
    {
        printf("Error: Failed to destory 'operators' semaphore.\n");
        return -1;
    }
    
    if (sem_destroy(&connected_lock) < 0)   // Destroy connected_lock semaphore
    {
        printf("Error: Failed to destory 'connected_lock' semaphore.\n");
        return -1;
    }
    
    if (sem_destroy(&caller_id) < 0)        // Destroy caller_id semaphore
    {
        printf("Error: Failed to destory 'caller_id' semaphore.\n");
        return -1;
    }
  
    return 0; // End of program
}

void* phonecall(void* vargp) // Phone call method
{
    sem_wait(&caller_id);   // Create caller critical section
    int id = next_id++;     // Get caller id
    printf("Thread %i is trying to connect...\n", id);   // Print thread is attempting to connect
    sem_post(&caller_id);   // Exit caller critical section
    
    int connect_success = 0;
    while (connect_success == 0)    // If connection is successful, set to 1; otherwise, keep trying to connect
    {
        sem_wait(&connected_lock);  // Create connected lock critical section
        if (connected == NUM_LINES) // If line are busy, notify and skip to end
        {
            printf("Thread %i fails to connect, line is busy...\n", id);
            sem_post(&connected_lock);  // Exit connected lock critical section
            sleep(1);                   // Wait for a second before calling again
        }
        else                        // Otherwise, connection is successful
        {
            connected++;                // Increment number of lines connected
            sem_post(&connected_lock);  // Exit connected lock critical section
            printf("Thread %i connects to available line, call ringing...\n", id);
            
            sem_wait(&operators);       // Waiting for operator to be available
            printf("Thread %i is speaking to operator.\n", id);
            
            sleep(3);                   // Caller has ordered medicine
            sem_post(&operators);       // Order is completed
            printf("Thread %i has ordered a medicine! The operator has left...\n", id);

            sem_wait(&connected_lock);  // Create connected lock critical section again
            connected--;                // Decrement number of lines connected (Thread has hung up)
            sem_post(&connected_lock);  // Exit connected lock critical section
            printf("Thread %i has hung up!\n", id);
            
            connect_success = 1;        // End of call, exit out of loop
        }
    }
}
