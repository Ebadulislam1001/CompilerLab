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

int getSymbolIndex(char sym, vector<char> &symbol, int nSymbols)
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
    // auxillary variables
    int i, j, index;
    // fetching input from the file
    FILE *filePointer = fopen("1_DFA.txt", "r");
    fscanf(filePointer, "%d %d", &nStates, &initialState);
    fscanf(filePointer, "%d", &nFinalStates);

    vector<bool> isFinal(nStates,0);
    vector<char> symbol;
    vector<vector<int>> transition(nStates);
    
    for (i = 0; i < nFinalStates; i++)
    {
        fscanf(filePointer, "%d", &index);
        if (index >= 0 && index < nStates)
            isFinal[index] = 1;
    }
    fscanf(filePointer, "%d", &nSymbols);
    for (i = 0; i < nSymbols; i++)
    {
        char temp;
        fscanf(filePointer, "%*c %c", &temp);
        symbol.push_back(temp);
    }
    for (i = 0; i < nStates; i++)
    {
        fscanf(filePointer, "%d", &index);
        for (j = 0; j < nSymbols; j++)
        {   
            int temp;
            fscanf(filePointer, "%d",&temp);
            transition[index].push_back(temp);
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
        int len = input.size();
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
