// sample input for DFA that accepts strings having odd 0s and odd 1s
// 4 0      //nStates       //initialState
// 1 2      //nFinalStates  //finalStates[]
// 2 0 1    //nSymbols      //symbol[]
// 0 1 3    //state[]       //transition[][]
// 1 0 2
// 2 3 1
// 3 2 0

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
    // characteristics of a dfa
    int nStates, initialState, nFinalStates, nSymbols;
    bool isFinal[64];
    char symbol[32];
    int transition[64][32];
    // auxillary variables
    int i, j, index;
    // fetching input from the file
    FILE *filePointer = fopen("1_DFA.txt", "r");

    fscanf(filePointer, "%d %d", &nStates, &initialState);
    fscanf(filePointer, "%d", &nFinalStates);
    for (i = 0; i < nStates; i++)
    {
        isFinal[i] = 0;
    }
    for (i = 0; i < nFinalStates; i++)
    {
        fscanf(filePointer, "%d", &index);
        if (index >= 0 && index < nStates)
            isFinal[index] = 1;
    }
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
    // automata setup complete
    while (1)
    {
        string input;
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
                printf("state q%d\n", currentState);
                break;
            }
            currentState = transition[currentState][currentSymbol];
            printf("state q%d\n", currentState);
            if (currentState < 0) // if there is no transition possible
            {
                break;
            }
        }
        if (currentState >= 0 && isFinal[currentState] == 1)
        { // string ends on a valid final state
            printf("\nThe input string is accepted\n ");
        }
        else
        {
            printf("\nThe input string is rejected\n ");
        }
    }
}