#include <stdio.h>

#define MAX 10
#define INF 999

/* -------------------------------------------------
   FUNCTION 1: DISPLAY COST MATRIX
   ------------------------------------------------- */

void displayMatrix(int n, int graph[MAX][MAX])
{
    int i, j;

    printf("\n============================================\n");
    printf("              COST MATRIX\n");
    printf("============================================\n");

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


    /* Display Distance Vector Table */

    printf("\n============================================\n");
    printf("          DISTANCE VECTOR ROUTING\n");
    printf("============================================\n");

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


    /* Display Link State Table */

    printf("\n============================================\n");
    printf("             LINK STATE ROUTING\n");
    printf("============================================\n");

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
   MAIN FUNCTION
   ------------------------------------------------- */

int main()
{
    int n;
    int graph[MAX][MAX];

    int i, j;
    int choice;

    printf("============================================\n");
    printf("       ROUTING ALGORITHM SIMULATOR\n");
    printf("============================================\n");

    printf("\n1. Distance Vector Routing");
    printf("\n2. Link State Routing");
    printf("\n3. Run Both Algorithms");
    printf("\n4. Exit");

    printf("\n\nEnter your choice: ");
    scanf("%d", &choice);


    /* Exit */

    if (choice == 4)
    {
        printf("\nProgram terminated.\n");
        return 0;
    }


    /* Validate choice */

    if (choice < 1 || choice > 4)
    {
        printf("\nInvalid choice!\n");
        return 0;
    }


    /* Input number of routers */

    printf("\nEnter number of routers (maximum %d): ",
           MAX);

    scanf("%d", &n);

    if (n <= 0 || n > MAX)
    {
        printf("\nInvalid number of routers!\n");
        return 0;
    }


    /* Input cost matrix */

    printf("\nEnter the cost matrix.\n");
    printf("Enter %d for no direct connection.\n\n",
           INF);

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("Cost R%d -> R%d: ",
                   i + 1,
                   j + 1);

            scanf("%d", &graph[i][j]);

            /* Cost from router to itself */

            if (i == j)
                graph[i][j] = 0;
        }
    }


    /* Display network */

    displayMatrix(n, graph);


    /* Select Algorithm */

    switch (choice)
    {
        case 1:

            distanceVector(n, graph);

            break;


        case 2:

            linkState(n, graph);

            break;


        case 3:

            printf("\n\nRunning Distance Vector...");
            distanceVector(n, graph);

            printf("\n\nRunning Link State...");
            linkState(n, graph);

            break;
    }


    printf("\n============================================\n");
    printf("          PROGRAM COMPLETED\n");
    printf("============================================\n");

    return 0;
}
