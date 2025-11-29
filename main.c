#include <stdio.h>
#include <stdlib.h>   
#include <time.h>

// Data Structure to represent a market queue
struct queue
{
    int *goods;    
    int maxSize;   
    int front;     
    int rear;      
    int size;      
};

struct queue* CreateMarketQueue(int queueMaxSize) {

    struct queue *pt = NULL;
    pt = (struct queue*)malloc(sizeof(struct queue));

    pt->goods = (int*)malloc(queueMaxSize * sizeof(int));
    pt->maxSize = queueMaxSize;
    pt->front = 0;
    pt->rear = -1;
    pt->size = 0;
    
    return pt;
}

int size(struct queue *pt) {
    return pt->size;
}

int isFull(struct queue *pt) {
    return pt->size == pt->maxSize;
}

int isEmpty(struct queue *pt) {
    return pt->size == 0;
}

int frontValue(struct queue *pt) {

    if(isEmpty(pt))
    {
        printf("There is no one in line\n");
        exit(EXIT_FAILURE);
    }

    return pt->goods[pt->front];
}

void enqueue(struct queue *pt, int x) {

    if(size(pt) == pt->maxSize)
    {
        printf("The queue is full.\n");
        exit(EXIT_FAILURE);
    }

    pt->rear = (pt->rear + 1) % pt->maxSize;
    pt->goods[pt->rear] = x;
    pt->size++;   
}

int dequeue(struct queue *pt) {

    if (isEmpty(pt))	
    {
        printf("There is nobody in the queue.\n");
        exit(EXIT_FAILURE);
    }

    printf("Removing %d\n", frontValue(pt));

    int x = pt->goods[pt->front];
    pt->front = (pt->front + 1) % pt->maxSize;
    pt->size--;

    return x;
}

int newCustomerArrived(double P) {
    double r = (double)rand() / RAND_MAX;
    return (r < P) ? 1 : 0;
}

void processGoodsInQueue(struct queue *pt) {

    if(isEmpty(pt))
    {
        return;
    }

    pt->goods[pt->front]--;

    if(pt->goods[pt->front] == 0)
    {
        printf("Customer finished goods and leaves the queue.\n");
        dequeue(pt);
    }
}


            /*  BONUS FUNCTIONS */
   

// choose the shorter queue

struct queue* chooseShorter(struct queue *q1, struct queue *q2)
{
    if(q1->size <= q2->size)
        return q1;
    return q2;
}

int main() {

    srand(time(NULL));

    int N;
    double P;
    int S;
    int T;

    printf("Enter the queue capacity for each cashier N : \n");
    scanf("%d", &N);

    printf("Enter the probability P (0.0 - 1.0)\n");
    scanf("%lf", &P);

    printf("Enter initial satisfaction (0 - 10)\n");
    scanf("%d", &S);

    printf("Enter total number of timesteps T: ");
    scanf("%d", &T);

    // TWO CASHIER QUEUES
    struct queue *c1 = CreateMarketQueue(N);
    struct queue *c2 = CreateMarketQueue(N);

    printf("\n--- SUPER MARKET SIMULATION (2 CASHIERS) STARTED ---\n");

    for(int t = 1; t <= T && S > 0; t++)
    {
        printf("\n ----- TIME STEP %d -----\n", t);

        // NEW CUSTOMER?
        if(newCustomerArrived(P))
        {
            int G = (rand() % 9) + 1;
            printf("New customer arrived with G = %d goods.\n", G);

            struct queue *target = chooseShorter(c1, c2);

            if(isFull(target))
            {
                printf("Both cashier queues FULL! Satisfaction decreased.\n");
                S--;
                if(S <= 0)
                {
                    printf("!!! Satisfaction dropped to 0. Simulation ended.\n");
                    break;
                }
            }
            else
            {
                enqueue(target, G);
                printf("Customer added to cashier %s.\n", (target == c1 ? "1" : "2"));
            }
        }

        // BOTH CASHIERS PROCESS GOODS
        processGoodsInQueue(c1);
        processGoodsInQueue(c2);

        printf("Cashier 1 queue size: %d\n", c1->size);
        printf("Cashier 2 queue size: %d\n", c2->size);
        printf("Satisfaction = %d\n", S);
    }

    if(S > 0)
        printf("\nSimulation finished normally after %d timesteps.\n", T);

    free(c1->goods);
    free(c2->goods);
    free(c1);
    free(c2);

    return 0;
}
