#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 6 // Size for max logs 


// queue for delivery requests
char queue[SIZE][20]; 
int f = -1, r = -1;    


// stack for urgent dispatches
char stack[SIZE][20];
int top = -1;         


// add item to queue
void enqueue(char item[20]) {
    if (r == SIZE - 1) {
        printf("Queue Overflow\n");
    } else if (f == -1 && r == -1) {
        f = r = 0;
        strcpy(queue[r], item);
    } else {
        strcpy(queue[++r], item);
    }
}

// remove item from queue
void dequeue(char item[20]) {
    if (f == -1 && r == -1) {
        printf("Queue is empty");
    } else if (f == r) {
        strcpy(item, queue[f]);
        f = r = -1;
    } else {
        strcpy(item, queue[f++]);
    }
}


// push item on stack
void push(char item[20]) {
    if (top == SIZE - 1) {
        printf("Stack Overflow\n");
        return;
    }
    strcpy(stack[++top], item);
}

// pop item from stack
void pop(char item[20]) {
    if (top == -1) {
        printf("Stack Underflow\n");
        return;
    }
    strcpy(item, stack[top--]);
}


// Flight Log:
char flog[SIZE][20]; // for storing last 6 completed deliveries
int logcount = 0;    

// add delivery to log and overwrite in case of overflow
void logDelivery(char delivery[20]) {
    if (logcount < SIZE) {
        strcpy(flog[logcount++], delivery);
    } else {
        // shift logs to the left
        for (int i = 1; i < SIZE; i++) {
            strcpy(flog[i - 1], flog[i]);
        }
        // add delivery
        strcpy(flog[SIZE - 1], delivery);
    }
}


//Overloaded Drones
typedef struct snode {
    char name[20];
    struct snode* next;
} snode;

snode* overhead = NULL; // head

// insert at front of list
void insertOverloaded(char name[20]) {
    snode* newNode = (snode*)malloc(sizeof(snode));
    strcpy(newNode->name, name);
    newNode->next = overhead;
    overhead = newNode;
}


//Serveiced Drones
typedef struct dnode {
    char name[20];
    struct dnode* prev;
    struct dnode* next;
} dnode;

dnode* servhead = NULL; // head

// move from overloaded to serveiced
void moveToServiced(char name[20]) {
    snode *temp = overhead, *prev = NULL;

    // finding in overloaded lost
    while (temp != NULL && strcmp(temp->name, name) != 0) {
        prev = temp;
        temp = temp->next;
    }

    // removing from overloaded and add to serveiced
    if (temp != NULL) {
        if (prev == NULL) overhead = temp->next;
        else prev->next = temp->next;

        dnode* newNode = (dnode*)malloc(sizeof(dnode));
        strcpy(newNode->name, name);
        newNode->prev = NULL;
        newNode->next = servhead;
        if (servhead != NULL) servhead->prev = newNode;
        servhead = newNode;

        free(temp); // freeing memory
    }
}

// traversing through the list in forward direction
void traverseServicedForward() {
    dnode* temp = servhead;
    printf("Serviced Drones (Forward): ");
    while (temp != NULL) {
        printf("%s -> ", temp->name);
        temp = temp->next;
    }
    printf("NULL\n");
}

// traversing through the list in backward direction
void traverseServicedBackward() {
    dnode* temp = servhead;
    while (temp && temp->next) temp = temp->next;

    printf("Serviced Drones (Backward): ");
    while (temp) {
        printf("%s -> ", temp->name);
        temp = temp->prev;
    }
    printf("NULL\n");
}


//Emergency Rerouting
typedef struct cnode {
    char name[20];
    struct cnode* next;
} cnode;

cnode* tail = NULL; //tail

//insert drone into circular list
void insertEmergency(char name[20]) {
    cnode* newNode = (cnode*)malloc(sizeof(cnode));
    strcpy(newNode->name, name);
    if (tail == NULL) {
        newNode->next = newNode;
        tail = newNode;
    } else {
        newNode->next = tail->next;
        tail->next = newNode;
        tail = newNode;
    }
}

//traversing through the circular list twice
void traverseEmergencyTwice() {
    if (!tail) return;

    cnode* start = tail->next;
    cnode* temp = start;
    int count = 0;

    printf("Emergency Reroute (2 Cycles): ");
    do {
        printf("%s -> ", temp->name);
        temp = temp->next;
        if (temp == start) count++;
    } while (count < 2);
    printf("\n");
}


int main() {
    int choice;
    char temp[20];

    while (1) {
        printf("\nCargo Drone Traffic Controller\n");
        printf("1. Enqueue Delivery Request\n");
        printf("2. Dispatch Drones (LIFO)\n");
        printf("3. Log Delivery\n");
        printf("4. Add Overloaded Drone\n");
        printf("5. Move Drone to Serviced\n");
        printf("6. Show Serviced Drones (Forward)\n");
        printf("7. Show Serviced Drones (Backward)\n");
        printf("8. Add Emergency Reroute Drone\n");
        printf("9. Show Emergency Reroutes (2 Cycles)\n");
        printf("10. Show Flight Log\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter delivery item: ");
                fgets(temp, sizeof(temp), stdin);
                temp[strcspn(temp, "\n")] = '\0';
                enqueue(temp);
                break;

            case 2:
                printf("Dispatching (LIFO): ");
                // move an element from the queue to stack
                while (f != -1) {
                    dequeue(temp);
                    push(temp);
                }
                // popping element from the stack
                while (top != -1) {
                    pop(temp);
                    printf("%s -> ", temp);
                }
                printf("NULL\n");
                break;

            case 3:
                printf("Enter delivery log: ");
                fgets(temp, sizeof(temp), stdin);
                temp[strcspn(temp, "\n")] = '\0';
                logDelivery(temp);
                printf("Delivery logged.\n");
                break;

            case 4:
                printf("Enter overloaded drone name: ");
                fgets(temp, sizeof(temp), stdin);
                temp[strcspn(temp, "\n")] = '\0';
                insertOverloaded(temp);
                printf("Drone added to overloaded list.\n");
                break;

            case 5:
                printf("Enter drone name to service: ");
                fgets(temp, sizeof(temp), stdin);
                temp[strcspn(temp, "\n")] = '\0';
                moveToServiced(temp);
                break;

            case 6:
                traverseServicedForward();
                break;

            case 7:
                traverseServicedBackward();
                break;

            case 8:
                printf("Enter emergency drone name: ");
                fgets(temp, sizeof(temp), stdin);
                temp[strcspn(temp, "\n")] = '\0';
                insertEmergency(temp);
                printf("Drone added to emergency reroute.\n");
                break;

            case 9:
                traverseEmergencyTwice();
                break;

            case 10:
                printf("Flight Log:\n");
                for (int i = 0; i < logcount; i++) {
                    printf("%s ", flog[i]);
                }
                printf("\n");
                break;

            case 0:
                printf("Exiting program.\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}



/*Creativity Bonus Section:
a)Why LIFO suits urgency:
    It is ideal because it prioritizes the most recent request. If for example Fuel is entered last then that means that a drone needs immediate refuelling,
    so then it the most urgent task and will be carried out first
    
b)Why should we archive logs?
    Generally storing old logs is necessary to keep track of what is going on in the company and moreover it will be useful for future analysis.
    
c)Overload cause and solution:
    Example: Drone 5 was carrying excess weight in the form of toolkits in a certain construction zone, thus damaging the drone's rotors. The technicians 
    changed the rotors to a new one and update the program of the drone to not lift beyond a certain weight limit
*/