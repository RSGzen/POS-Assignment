#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Mutex represents a single dorm
// Only a student or a repair worker can have rights to here at a same time
pthread_mutex_t single_dorm;

// Declare and initialize total number of active threads running at the same time as 0
// Each student count as a thread and the repair worker also count as a single thread
// Active thread can be referred as total number of person in the room
int num_people_in_dorm = 0;

// Iitialize and set current year to 2025
int current_year = 2025;

// Initialize and set the years of studies left (YOSL) for each student
int studentA_yosl = 3;
int studentB_yosl = 4;

// Intialize and set the years of rennovation left (YORL) for repair worker
int worker_yorl = 1;

void* single_dorm_operation(void* arg)
{   
    // Lock mutex
    pthread_mutex_lock(&single_dorm);

    // Number of people in the dorm plus 1
    num_people_in_dorm++;

     // Print current active threads
    printf("\n| Current number of person in dorm room: %d |\n", num_people_in_dorm);

    // Cast to int data type
    int *repair_check = (int *)arg; 
    
    // Just for checking purposes
    int number0 = 0; // Represent worker
    int number1 = 1; // Represent student A
    int number2 = 2; // Represent student B

    // Function for repair worker rennovation
    if (*repair_check == number0)
    {
        printf("\nRepair worker is renovating the single dorm at year %d", current_year);

        // Adjust the year to after renovation
        current_year += worker_yorl;

        // Simulate the time required for renovation
        sleep(1); 
        
        printf("\nRepair worker completed renovation at year %d.\n", current_year);
    }

    // Function for student A
    if (*repair_check == number1)
    {
        printf("\nStudent A is studying in the single dorm at year %d\n", current_year);

        // Decrease years of studies left for student A year by year
        for (int i = 0; i < studentA_yosl; i++)
        {
            // Increment current year counter
            current_year++;

            // Simulate time passed per year while studying
            sleep(1);

            printf("\nAfter %d year of study, it is now year %d", i+1, current_year);
        }

        studentA_yosl -= studentA_yosl;

        printf("\n\nStudent A completed degreee at year %d\n", current_year);
    }

    // Function for student B
    if (*repair_check == number2)
    {
        printf("\nStudent B is studying in the single dorm at year %d\n", current_year);

        // Decrease years of studies left for student A year by year
        for (int i = 0; i < studentB_yosl; i++)
        {
            // Increment current year counter
            current_year++;

            // Simulate time passed per year while studying
            sleep(1);

            printf("\nAfter %d year of study, it is now year %d", i+1, current_year);
        }

        studentB_yosl -= studentB_yosl;

        printf("\n\nStudent B completed degreee at year %d\n", current_year);
    }

    // Unlock mutex
    pthread_mutex_unlock(&single_dorm); 

    // Number of person in room minus 1
    num_people_in_dorm--;

    // Print current number of person in room
    printf("\n| Current number of person in dorm room: %d |\n", num_people_in_dorm);

    return NULL;
}

int main()
{
    pthread_t repair_worker, student_A, student_B;

    printf("\n===================================================\n");
    
    // Initialize mutex (single dorm)
    pthread_mutex_init(&single_dorm, NULL);

    // Create flags for 3 threads
    int flag_repairworker = 0;
    int flag_studentA = 1;
    int flag_studentB = 2;

    // Create two threads
    pthread_create(&repair_worker, NULL, single_dorm_operation, &flag_repairworker);
    pthread_create(&student_A, NULL, single_dorm_operation, &flag_studentA);
    pthread_create(&student_B, NULL, single_dorm_operation, &flag_studentB);

    // Wait for threads to finish
    pthread_join(student_A, NULL); // Student A gets to study until degree finishes
    pthread_join(repair_worker, NULL); // Repair worker can renovate after student A graduates
    pthread_join(student_B, NULL); // Student B gets to study after rennovation completed

    // Destroy mutex (single dorm)
    pthread_mutex_destroy(&single_dorm);
    
    printf("\n===================================================\n");
}