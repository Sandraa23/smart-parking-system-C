#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SPACES 8
#define QUEUE_SIZE 5// Changed from 20 to 10


typedef struct {
    int space_id;
    int occupied;
    char vehicle_id[10];
} ParkingSpace;

ParkingSpace parking_lot[MAX_SPACES];


typedef struct {
    char vehicle_ids[QUEUE_SIZE][10];
    int front;
    int rear;
    int count;
} Queue;

Queue waiting_queue;

// Function prototypes
void init_queue(void);
int is_queue_full(void);
int is_queue_empty(void);
void enqueue(char* vehicle_id);
void dequeue(char* vehicle_id);

void init_parking_lot() {
    for (int i = 0; i < MAX_SPACES; i++) {
        parking_lot[i].space_id = i + 1;
        parking_lot[i].occupied = 0;
    }
}

int find_empty_space() {
    for (int i = 0; i < MAX_SPACES; i++) {
        if (!parking_lot[i].occupied) {
            return i;
        }
    }
    return -1;
}

void park_vehicle(char* vehicle_id) {
    int index = find_empty_space();
    if (index != -1) {
        parking_lot[index].occupied = 1;
        strcpy(parking_lot[index].vehicle_id, vehicle_id);
        printf("Vehicle %s parked at space %d\n", vehicle_id, index + 1);
    } else {
        if (!is_queue_full()) {
            enqueue(vehicle_id);
            printf("Parking lot full. Vehicle %s added to waiting queue\n", vehicle_id);
        } else {
            printf("Waiting queue is full. Cannot add vehicle %s\n", vehicle_id);
        }
    }
}

void leave_parking(int space_id) {
    if (space_id >= 1 && space_id <= MAX_SPACES) {
        int index = space_id - 1;
        if (parking_lot[index].occupied) {
            printf("Vehicle %s left from space %d\n", parking_lot[index].vehicle_id, space_id);
            parking_lot[index].occupied = 0;
            
            if (!is_queue_empty()) {
                char vehicle_id[10];
                dequeue(vehicle_id);
                park_vehicle(vehicle_id);
            }
        } else {
            printf("Space %d is already empty\n", space_id);
        }
    } else {
        printf("Invalid space ID. Please enter a number between 1 and %d\n", MAX_SPACES);
    }
}

void display_status() {
    printf("\nParking Lot Status:\n");
    for (int i = 0; i < MAX_SPACES; i++) {
        if (parking_lot[i].occupied) {
            printf("Space %d: Occupied (Vehicle: %s)\n", i + 1, parking_lot[i].vehicle_id);
        } else {
            printf("Space %d: Empty\n", i + 1);
        }
    }

    printf("\nWaiting Queue (%d vehicles):\n", waiting_queue.count);
    if (!is_queue_empty()) {
        int current = waiting_queue.front;
        for (int i = 0; i < waiting_queue.count; i++) {
            printf("Vehicle: %s\n", waiting_queue.vehicle_ids[current]);
            current = (current + 1) % QUEUE_SIZE;
        }
    } else {
        printf("Queue is empty.\n");
    }
}

// Queue functions
void init_queue() {
    waiting_queue.front = 0;
    waiting_queue.rear = -1;
    waiting_queue.count = 0;
}

int is_queue_full() {
    return waiting_queue.count == QUEUE_SIZE;
}

int is_queue_empty() {
    return waiting_queue.count == 0;
}

void enqueue(char* vehicle_id) {
    if (!is_queue_full()) {
        waiting_queue.rear = (waiting_queue.rear + 1) % QUEUE_SIZE;
        strcpy(waiting_queue.vehicle_ids[waiting_queue.rear], vehicle_id);
        waiting_queue.count++;
    }
}

void dequeue(char* vehicle_id) {
    if (!is_queue_empty()) {
        strcpy(vehicle_id, waiting_queue.vehicle_ids[waiting_queue.front]);
        waiting_queue.front = (waiting_queue.front + 1) % QUEUE_SIZE;
        waiting_queue.count--;
    }
}

// Main
int main() {
    init_parking_lot();
    init_queue();
    
    int choice;
    char vehicle_id[10];
    int space_id;
    
    while (1) {
        printf("\nSmart Parking System Menu:\n");
        printf("1. Park a vehicle\n");
        printf("2. Remove a vehicle\n");
        printf("3. Display parking status\n");
        printf("4. Exit\n");
        printf("Enter your choice (1-4): ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); 
            continue;
        }
        getchar(); 

        switch (choice) {
            case 1:
                printf("Enter vehicle ID (max 9 characters): ");
                fgets(vehicle_id, sizeof(vehicle_id), stdin);
                vehicle_id[strcspn(vehicle_id, "\n")] = 0; // remove newline
                if (strlen(vehicle_id) == 0) {
                    printf("Vehicle ID cannot be empty.\n");
                    break;
                }
                park_vehicle(vehicle_id);
                break;

            case 2:
                printf("Enter space ID (1-%d): ", MAX_SPACES);
                if (scanf("%d", &space_id) != 1) {
                    printf("Invalid input. Please enter a number.\n");
                    while (getchar() != '\n'); // clear buffer
                    break;
                }
                getchar(); 
                leave_parking(space_id);
                break;

            case 3:
                display_status();
                break;

            case 4:
                printf("Exiting program.\n");
                return 0;

            default:
                printf("Invalid choice. Please enter a number between 1 and 4.\n");
        }
    }

    return 0;
}
