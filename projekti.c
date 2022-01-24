#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Game{
    char name[20];
    float price;
    float profit;
    int size;
}Game;

//Tarkistaa onko samanniminen peli jo olemassa
int uniqueName(Game* arr, int size, const char* name)
{
    for (int i = 0; i < size; i++)
    {
        if(strcmp(name, arr[i].name) == 0)
        {
            return 0;
        }
    }
    return 1;
}

Game* addGame(Game* arr, const char* name, float price, float profit, int size)
{
    arr = realloc(arr, (size+1) * sizeof(Game));
    Game uusi;
    strcpy(uusi.name, name);
    uusi.price = price;
    uusi.profit = profit;
    arr[size] = uusi;
    return arr;
}

int buyGame(Game* arr, const char* name, int n, int size)
{
    int i = 0;
    while (i < size)
    {
        if (strcmp(arr->name, name) == 0)
        {
            arr->profit += n * arr->price;
            return 1;
        }
        arr++;
        i++;
    }
    return 0;
}

//Avuksi printGames funktiolle
void swap(Game* a, Game* b)
{
    Game temp = *a;
    *a = *b;
    *b = temp;
}

//Järjestää ja tulostaa pelit
void printGames(Game* arr, int size)
{
    Game* sorted = malloc((size+1) * sizeof(Game));
    for (int i = 0; i < (size); i++)
    {
        sorted[i] = *arr;
        arr++;
    }
    for (int i = 0; i < (size-1); i++)
    {
        int max = i;
        for (int j = i + 1; j < size; j++)
        {
            if (sorted[i].profit < sorted[j].profit)
            {
                max = j;
            }
        }
        swap(&sorted[max], &sorted[i]);
    }
    for (int i = 0; i < size; i++)
    {
        printf("Name: %s    Price: %.2f    Profit: %.2f\n", sorted[i].name, sorted[i].price, sorted[i].profit);
    }
    free(sorted);
}

void writeFile(Game* arr, const char *filename, int size)
{
    FILE *f = fopen(filename, "w");
    for (int i = 0; i < size; i++)
    {
        fprintf(f, "%s %f %f\n", arr->name, arr->price, arr->profit);
        arr++;
    }
    fclose(f);
}

int main()
{
    printf("Available commands:\n");
    printf("A for adding a game\n");
    printf("B for buying games\n");
    printf("L for listing games in database\n");
    printf("W for saving data to a file\n");
    printf("O for loading data from a file\n");
    printf("Q for freeing memory and exiting the program\n");

    int size = 0;
    Game* arr = malloc(1);
    while (1)
    {
        char line[100];
        fgets(line, 99, stdin);
        int lenght = strlen(line);
        char cmd;
        char name[21];
        float n;
        int test = sscanf(line, "%c %s %f", &cmd, name, &n);
        if (lenght > 81)
        {
            printf("Max lenght for input is 80 characters. Try again\n");
            if (lenght == 99)
            {
                int temp = fgetc(stdin);
                while (temp != '\n')
                {
                    temp = fgetc(stdin);
                }
            }
        }


        else if (cmd == 'A')
        {
            if (test != 3)
            {
                printf("Incorrect arguments for A\n");
            }
            else if (strlen(name) > 20)
            {
                printf("Max lenght for name is 20 characters. Try again\n");
            }
            else if (n < 0)
            {
                printf("Price cannot be negative\n");
            }
            else if (n > 10000000)
            {
                printf("Calm down EA (max price is 10 million)\n");
            }
            else
            {
                if (uniqueName(arr, size, name))
                {
                    arr = addGame(arr, name, n, 0, size);
                    size++;
                    printf("Game '%s' added\n", arr[size-1].name);
                }
                else
                {
                    printf("Game with that name already exists\n");
                }  
            }
        }


        else if(cmd == 'B')
        {
            if (test != 3)
            {
                printf("Incorrect arguments for B\n");
            }
            else if ((int) n != n)
            {
                printf("Use a whole number\n");
            }
            else
            {
                //Hyväksyn tässää negatiiviset arvot pelien määräksi, että pelejä voi palauttaa
                if (buyGame(arr, name, (int) n, size))
                {
                    printf("%d games bought\n", (int) n);
                }
                else
                {
                    printf("Game not found\n");
                }
            }
        }


        else if (cmd == 'L')
        {
            printGames(arr, size);
        }
        else if (cmd == 'W')
        {
            if (test == 2)
            {
                writeFile(arr, name, size);
                printf("Games saved to file: %s\n", name);
            }
            else
            {
                printf("Incorrect arguments for W\n");
            }
        }


        else if (cmd == 'O')
        {
            if (test == 2)
            {
                FILE *f = fopen(name, "r");
                if (!f)
                {
                    printf("Error opening file: %s\n", name);
                }
                else
                {
                    free(arr);
                    arr = malloc(1);
                    size = 0;
                    char nimi[20];
                    float hinta;
                    float prof;
                    int a = fscanf(f, "%s %f %f", nimi, &hinta, &prof);
                    while (a == 3)
                    {
                        arr = addGame(arr, nimi, hinta, prof, size);
                        size++;
                        a = fscanf(f, "%s %f %f", nimi, &hinta, &prof);
                    }
                    printf("Games loaded from file: %s\n", name);
                    fclose(f);
                }
            }
            else
            {
                printf("Incorrect arguments for O\n");
            }
        }


        else if (cmd == 'Q')
        {
            free(arr);
            printf("Exiting program\n");
            break;
        }
        else
        {
            printf("Incorrect command\n");
        }
    }
    return 0;
}