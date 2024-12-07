// sample input for a mealy machine that returns 1s compilment
// 2 0      //nStates   //initialState
// 2 0 1    //nSymbols  //symbol[]
// 0 0 1    //state[]   //transition[][]
// 1 1 0
// 0 1 0    //state[]   //output[][]
// 1 1 0

#include <bits/stdc++.h>
using namespace std;

int getSymbolIndex(char sym, char symbol[], int nSymbols)
{
    for (int i = 0; i < nSymbols; i++)
    {
        if (sym == symbol[i])
        {
            return i;
        }
    }
    return -1;
}
int main()
{
    // characteristics of a moore machine
    int nStates, initialState, nSymbols;
    char symbol[32];
    int transition[64][32];
    char outputSymbol[64][32][16]; // 3d char array i.e. matrix of char arrays i.e. matrix of strings
    // auxillary variables
    int i, j, index;
    // fetching input from the file
    FILE *filePointer = fopen("2_MEALY.txt", "r");

    fscanf(filePointer, "%d %d", &nStates, &initialState);
    fscanf(filePointer, "%d", &nSymbols);
    for (i = 0; i < nSymbols; i++)
    {
        fscanf(filePointer, "%*c %c", &symbol[i]);
    }
    for (i = 0; i < nStates; i++)
    {
        fscanf(filePointer, "%d", &index);
        for (j = 0; j < nSymbols; j++)
        {
            fscanf(filePointer, "%d", &transition[index][j]);
        }
    }
    for (i = 0; i < nStates; i++)
    {
        fscanf(filePointer, "%d", &index);
        for (j = 0; j < nSymbols; j++)
        {
            fscanf(filePointer, "%*c %s", outputSymbol[index][j]);
        }
    }
    // automata setup complete
    while (1)
    {
        string input, output;
        printf("\nInput String: ");
        getline(cin, input);
        if (input[0] == '!')
        {
            return 0;
        }
        int len = input.length();
        // String traversal starts here
        int currentState = initialState;
        for (int i = 0; i < len; i++)
        {
            printf("State q%d on input %c leads us to ", currentState, input[i]);
            int currentSymbol = getSymbolIndex(input[i], symbol, nSymbols);
            if (currentSymbol < 0) // if the symbol is not defined
            {
                currentState = -1;
                printf("state q%d", currentState);
                break;
            }
            currentState = transition[currentState][currentSymbol];
            if (currentState < 0) // if there is no transition possible
            {
                printf("state q%d", currentState);
                break;
            }
            output += outputSymbol[currentState][currentSymbol];
            printf("state q%d and gives output %s\n", currentState, outputSymbol[currentState][currentSymbol]);
        }
        printf("\nOutput: ");
        cout << output << endl;
    }
}