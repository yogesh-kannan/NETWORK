#include <stdio.h>

#define MAX 10
#define INF 999

/* -------------------------------------------------
   FUNCTION 1: DISPLAY COST MATRIX
   ------------------------------------------------- */

void displayMatrix(int n, int graph[MAX][MAX])
{
    int i, j;

    printf("\n------COST MATRIX------\n");

    printf("\n\t");

    for (i = 0; i < n; i++)
        printf("R%d\t", i + 1);

    printf("\n");

    for (i = 0; i < n; i++)
    {
        printf("R%d\t", i + 1);

        for (j = 0; j < n; j++)
        {
            if (graph[i][j] == INF)
                printf("INF\t");
            else
                printf("%d\t", graph[i][j]);
        }

        printf("\n");
    }
}


/* -------------------------------------------------
   FUNCTION 2: DISTANCE VECTOR ROUTING
   ------------------------------------------------- */

void distanceVector(int n, int cost[MAX][MAX])
{
    int distance[MAX][MAX];
    int nextHop[MAX][MAX];

    int i, j, k;
    int updated;
    int iteration = 0;

    /* Initialize routing table */

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            distance[i][j] = cost[i][j];

            if (cost[i][j] != INF && i != j)
                nextHop[i][j] = j;
            else
                nextHop[i][j] = -1;
        }
    }

    /* Distance Vector Algorithm */

    do
    {
        updated = 0;
        iteration++;

        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                for (k = 0; k < n; k++)
                {
                    if (distance[i][k] != INF &&
                        distance[k][j] != INF)
                    {
                        if (distance[i][k] +
                            distance[k][j] <
                            distance[i][j])
                        {
                            distance[i][j] =
                                distance[i][k] +
                                distance[k][j];

                            nextHop[i][j] =
                                nextHop[i][k];

                            updated = 1;
                        }
                    }
                }
            }
        }

    } while (updated);


    printf("\n------DISTANCE VECTOR ROUTING------\n");

    printf("\nNumber of iterations = %d\n", iteration);

    for (i = 0; i < n; i++)
    {
        printf("\nRouting Table of Router R%d\n", i + 1);

        printf("--------------------------------------------\n");

        printf("Destination\tNext Hop\tDistance\n");

        for (j = 0; j < n; j++)
        {
            printf("R%d\t\t", j + 1);

            if (i == j)
            {
                printf("-\t\t0\n");
            }
            else if (distance[i][j] == INF)
            {
                printf("-\t\tINF\n");
            }
            else
            {
                printf("R%d\t\t%d\n",
                       nextHop[i][j] + 1,
                       distance[i][j]);
            }
        }
    }
}


/* -------------------------------------------------
   FUNCTION 3: FIND MINIMUM DISTANCE
   For Link State Routing
   ------------------------------------------------- */

int findMinimum(int distance[MAX],
                int visited[MAX],
                int n)
{
    int min = INF;
    int index = -1;

    int i;

    for (i = 0; i < n; i++)
    {
        if (!visited[i] &&
            distance[i] < min)
        {
            min = distance[i];
            index = i;
        }
    }

    return index;
}


/* -------------------------------------------------
   FUNCTION 4: DISPLAY PATH
   ------------------------------------------------- */

void displayPath(int parent[MAX],
                 int destination)
{
    if (parent[destination] == -1)
    {
        printf("R%d", destination + 1);
        return;
    }

    displayPath(parent, parent[destination]);

    printf(" -> R%d", destination + 1);
}


/* -------------------------------------------------
   FUNCTION 5: LINK STATE ROUTING
   Using DIJKSTRA'S ALGORITHM
   ------------------------------------------------- */

void linkState(int n, int graph[MAX][MAX])
{
    int distance[MAX];
    int visited[MAX];
    int parent[MAX];

    int source;
    int i, count;
    int current;

    printf("\nEnter source router (1-%d): ", n);
    scanf("%d", &source);

    if (source < 1 || source > n)
    {
        printf("\nInvalid source router!\n");
        return;
    }

    source--;

    /* Initialization */

    for (i = 0; i < n; i++)
    {
        distance[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }

    distance[source] = 0;


    /* Dijkstra Algorithm */

    for (count = 0; count < n - 1; count++)
    {
        current =
            findMinimum(distance,
                        visited,
                        n);

        if (current == -1)
            break;

        visited[current] = 1;

        for (i = 0; i < n; i++)
        {
            if (!visited[i] &&
                graph[current][i] != INF &&
                distance[current] != INF)
            {
                int newDistance =
                    distance[current] +
                    graph[current][i];

                if (newDistance < distance[i])
                {
                    distance[i] = newDistance;

                    parent[i] = current;
                }
            }
        }
    }

    printf("\n------LINK STATE ROUTING------\n");

    printf("\nSource Router: R%d\n", source + 1);

    printf("\nDestination\tCost\tShortest Path\n");

    printf("--------------------------------------------\n");

    for (i = 0; i < n; i++)
    {
        printf("R%d\t\t", i + 1);

        if (distance[i] == INF)
        {
            printf("INF\tNo Path\n");
        }
        else
        {
            printf("%d\t", distance[i]);

            displayPath(parent, i);

            printf("\n");
        }
    }
}
/* -------------------------------------------------
 FUNCTION 6: CHANGE LINK COST
 Updates the bidirectional cost between two routers
 ------------------------------------------------- */
void changeCost(int n, int graph[MAX][MAX], int startRouter, int endRouter, int newCost)
{
    // Convert 1-based router input (R1, R2) to 0-based array indices
    int start = startRouter - 1;
    int end = endRouter - 1;

    // Boundary check to prevent array index out of bounds crashes
    if (start < 0 || start >= n || end < 0 || end >= n)
    {
        printf("\nError: Invalid router selection! Must be between 1 and %d.\n", n);
        return;
    }

    // A router's cost to itself must always remain 0
    if (start == end)
    {
        printf("\nWarning: Cost from a router to itself cannot be changed from 0.\n");
        return;
    }

    // Update the cost symmetrically for bidirectional network graph linkage
    graph[start][end] = newCost;
    graph[end][start] = newCost;

    printf("\nSuccessfully updated link R%d <-> R%d to Cost: ", startRouter, endRouter);
    if (newCost == INF)
    {
        printf("INF (Link Down)\n");
    }
    else
    {
        printf("%d\n", newCost);
    }
}
/* -------------------------------------------------
 FUNCTION 7: FIND MINIMUM COST PATH (PAIRWISE)
 Computes and prints the shortest path between two specific routers
 ------------------------------------------------- */
void findMinCostPath(int n, int graph[MAX][MAX], int startRouter, int endRouter)
{
    int distance[MAX];
    int visited[MAX];
    int parent[MAX];
    int i, count, current;

    // Convert 1-based user input to 0-based array index keys
    int source = startRouter - 1;
    int target = endRouter - 1;

    // Bounds checking
    if (source < 0 || source >= n || target < 0 || target >= n)
    {
        printf("\nError: Invalid router bounds selection!\n");
        return;
    }

    // Initialize arrays
    for (i = 0; i < n; i++)
    {
        distance[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }
    distance[source] = 0;

    // Dijkstra's Algorithm execution loop
    for (count = 0; count < n - 1; count++)
    {
        current = findMinimum(distance, visited, n); // Reuses Function 3 from your code

        if (current == -1)
            break;

        visited[current] = 1;

        // Early exit: optimization if we successfully reached our specific destination
        if (current == target)
            break;

        for (i = 0; i < n; i++)
        {
            if (!visited[i] && graph[current][i] != INF && distance[current] != INF)
            {
                int newDistance = distance[current] + graph[current][i];
                if (newDistance < distance[i])
                {
                    distance[i] = newDistance;
                    parent[i] = current;
                }
            }
        }
    }

    // Output formatting block
    printf("\n------ PATH ANALYSIS: R%d to R%d ------\n", startRouter, endRouter);
    if (distance[target] == INF)
    {
        printf("Status: NO PATH EXISTS (The destination is completely isolated)\n");
    }
    else
    {
        printf("Absolute Minimum Cost: %d\n", distance[target]);
        printf("Shortest Router Path: ");
        displayPath(parent, target); // Reuses Function 4 from your code to auto-unwrap string traces
        printf("\n");
    }
}


/* -------------------------------------------------
   MAIN FUNCTION
   ------------------------------------------------- */

int main()
{
    int n;
    int graph[MAX][MAX];

    int i, j;
    int choice;
while(1){
    printf("\n------ROUTING ALGORITHM SIMULATOR MENU------\n");

    printf("\n1. Enter cost matrix");
    printf("\n2. Distance Vector Routing");
    printf("\n3. Link State Routing");
    printf("\n4. Run Both Algorithms");
    printf("\n5. Change Cost");
    printf("\n6. Find minimum cost path");
    printf("\n7. Exit");

    printf("\n\nEnter your choice: ");
    scanf("%d", &choice);

    int sRouter, eRouter, uCost;
    switch (choice)
    {
        case 1:
            printf("\nEnter number of routers (maximum %d): ",MAX);
            scanf("%d", &n);
            if (n <= 0 || n > MAX)
            {
                printf("\nInvalid number of routers!\n");
                return 0;
            }

            printf("\nEnter the cost matrix.\n");
            printf("Enter %d for no direct connection.\n\n",INF);

            for (i = 0; i < n; i++)
            {
                for (j = 0; j < n; j++)
                {
                    printf("Cost R%d -> R%d: ",i + 1,j + 1);
                    scanf("%d", &graph[i][j]);
                    /* Cost from router to itself */
                    if (i == j)
                        graph[i][j] = 0;
                }
            }

            displayMatrix(n, graph);
            break;
        case 2:
            distanceVector(n, graph);
            break;
        case 3:
            linkState(n, graph);
            break;
        case 4:
            printf("\n\nRunning Distance Vector...");
            distanceVector(n, graph);
            printf("\n\nRunning Link State...");
            linkState(n, graph);
            break;
        case 5: // <-- Inserted Case logic
            printf("\nEnter starting router (1-%d): ", n);
            scanf("%d", &sRouter);
            printf("Enter ending router (1-%d): ", n);
            scanf("%d", &eRouter);
            printf("Enter new link cost (Use 999 for INF/Link Down): ");
            scanf("%d", &uCost);

            changeCost(n, graph, sRouter, eRouter, uCost);

            printf("\nUpdated topology matrix view:\n");
            displayMatrix(n, graph); // Display the updated matrix to verify changes
            break;
        case 6:
            printf("\nEnter starting router (1-%d): ", n);
            scanf("%d", &sRouter);
            printf("Enter target destination router (1-%d): ", n);
            scanf("%d", &eRouter);

            findMinCostPath(n, graph, sRouter, eRouter);
            break;
        case 7:
            printf("\nExiting...\n");
            return 0;
        default:
            printf("\nInvlid choice!Try again.\n");
            break;
    }
    }
}
/*[24bcs063@mepcolinux ex4]$cc p1.c -o p1
[24bcs063@mepcolinux ex4]$./p1
------ROUTING ALGORITHM SIMULATOR MENU------

1. Enter cost matrix
2. Distance Vector Routing
3. Link State Routing
4. Run Both Algorithms
5. Change Cost
6. Find minimum cost path
7. Exit

Enter your choice: 1

Enter number of routers (maximum 10): 5

Enter the cost matrix.
Enter 999 for no direct connection.

Cost R1 -> R1: 0
Cost R1 -> R2: 2
Cost R1 -> R3: 999
Cost R1 -> R4: 1
Cost R1 -> R5: 999
Cost R2 -> R1: 2
Cost R2 -> R2: 0
Cost R2 -> R3: 3
Cost R2 -> R4: 999
Cost R2 -> R5: 7
Cost R3 -> R1: 999
Cost R3 -> R2: 3
Cost R3 -> R3: 0
Cost R3 -> R4: 4
Cost R3 -> R5: 2
Cost R4 -> R1: 1
Cost R4 -> R2: 999
Cost R4 -> R3: 4
Cost R4 -> R4: 0
Cost R4 -> R5: 999
Cost R5 -> R1: 999
Cost R5 -> R2: 7
Cost R5 -> R3: 2
Cost R5 -> R4: 999
Cost R5 -> R5: 0

------COST MATRIX------

        R1      R2      R3      R4      R5
R1      0       2       INF     1       INF
R2      2       0       3       INF     7
R3      INF     3       0       4       2
R4      1       INF     4       0       INF
R5      INF     7       2       INF     0

------ROUTING ALGORITHM SIMULATOR MENU------

1. Enter cost matrix
2. Distance Vector Routing
3. Link State Routing
4. Run Both Algorithms
5. Change Cost
6. Find minimum cost path
7. Exit

Enter your choice: 2

------DISTANCE VECTOR ROUTING------

Number of iterations = 2

Routing Table of Router R1
--------------------------------------------
Destination     Next Hop        Distance
R1              -               0
R2              R2              2
R3              R2              5
R4              R4              1
R5              R2              7

Routing Table of Router R2
--------------------------------------------
Destination     Next Hop        Distance
R1              R1              2
R2              -               0
R3              R3              3
R4              R1              3
R5              R3              5

Routing Table of Router R3
--------------------------------------------
Destination     Next Hop        Distance
R1              R2              5
R2              R2              3
R3              -               0
R4              R4              4
R5              R5              2

Routing Table of Router R4
--------------------------------------------
Destination     Next Hop        Distance
R1              R1              1
R2              R1              3
R3              R3              4
R4              -               0
R5              R3              6

Routing Table of Router R5
--------------------------------------------
Destination     Next Hop        Distance
R1              R3              7
R2              R3              5
R3              R3              2
R4              R3              6
R5              -               0

------ROUTING ALGORITHM SIMULATOR MENU------

1. Enter cost matrix
2. Distance Vector Routing
3. Link State Routing
4. Run Both Algorithms
5. Change Cost
6. Find minimum cost path
7. Exit

Enter your choice: 3

Enter source router (1-5): 1

------LINK STATE ROUTING------

Source Router: R1

Destination     Cost    Shortest Path
--------------------------------------------
R1              0       R1
R2              2       R1 -> R2
R3              5       R1 -> R4 -> R3
R4              1       R1 -> R4
R5              7       R1 -> R4 -> R3 -> R5

------ROUTING ALGORITHM SIMULATOR MENU------

1. Enter cost matrix
2. Distance Vector Routing
3. Link State Routing
4. Run Both Algorithms
5. Change Cost
6. Find minimum cost path
7. Exit

Enter your choice: 6

Enter starting router (1-5): 1
Enter target destination router (1-5): 5

------ PATH ANALYSIS: R1 to R5 ------

Absolute Minimum Cost: 7
Shortest Router Path: R1 -> R4 -> R3 -> R5

------ROUTING ALGORITHM SIMULATOR MENU------

1. Enter cost matrix
2. Distance Vector Routing
3. Link State Routing
4. Run Both Algorithms
5. Change Cost
6. Find minimum cost path
7. Exit

Enter your choice: 5

Enter starting router (1-5): 1
Enter ending router (1-5): 5
Enter new link cost (Use 999 for INF/Link Down): 2

Successfully updated link R1 <-> R5 to Cost: 2

Updated topology matrix view:

------COST MATRIX------

        R1      R2      R3      R4      R5
R1      0       2       INF     1       2
R2      2       0       3       INF     7
R3      INF     3       0       4       2
R4      1       INF     4       0       INF
R5      2       7       2       INF     0

------ROUTING ALGORITHM SIMULATOR MENU------

1. Enter cost matrix
2. Distance Vector Routing
3. Link State Routing
4. Run Both Algorithms
5. Change Cost
6. Find minimum cost path
7. Exit

Enter your choice: 4

running distance vector...
------DISTANCE VECTOR ROUTING------

Number of iterations = 2

Routing Table of Router R1
--------------------------------------------
Destination     Next Hop        Distance
R1              -               0
R2              R2              2
R3              R5              4
R4              R4              1
R5              R5              2

Routing Table of Router R2
--------------------------------------------
Destination     Next Hop        Distance
R1              R1              2
R2              -               0
R3              R3              3
R4              R1              3
R5              R1              4

Routing Table of Router R3
--------------------------------------------
Destination     Next Hop        Distance
R1              R5              4
R2              R2              3
R3              -               0
R4              R4              4
R5              R5              2

Routing Table of Router R4
--------------------------------------------
Destination     Next Hop        Distance
R1              R1              1
R2              R1              3
R3              R3              4
R4              -               0
R5              R1              3

Routing Table of Router R5
--------------------------------------------
Destination     Next Hop        Distance
R1              R1              2
R2              R1              4
R3              R3              2
R4              R1              3
R5              -               0


Running Link State...
Enter source router (1-5): 1

------LINK STATE ROUTING------

Source Router: R1

Destination     Cost    Shortest Path
--------------------------------------------
R1              0       R1
R2              2       R1 -> R2
R3              4       R1 -> R5 -> R3
R4              1       R1 -> R4
R5              2       R1 -> R5

------ROUTING ALGORITHM SIMULATOR MENU------

1. Enter cost matrix
2. Distance Vector Routing
3. Link State Routing
4. Run Both Algorithms
5. Change Cost
6. Find minimum cost path
7. Exit

Enter your choice: 6

Enter starting router (1-5): 2
Enter target destination router (1-5): 5

------ PATH ANALYSIS: R2 to R5 ------

Absolute Minimum Cost: 4
Shortest Router Path: R2 -> R1 -> R5

------ROUTING ALGORITHM SIMULATOR MENU------

1. Enter cost matrix
2. Distance Vector Routing
3. Link State Routing
4. Run Both Algorithms
5. Change Cost
6. Find minimum cost path
7. Exit

Enter your choice: 8

Invlid choice!Try again.

------ROUTING ALGORITHM SIMULATOR MENU------

1. Enter cost matrix
2. Distance Vector Routing
3. Link State Routing
4. Run Both Algorithms
5. Change Cost
6. Find minimum cost path
7. Exit

Enter your choice: 7

Exiting...*/
