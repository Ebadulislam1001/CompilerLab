// sample input for DFA that accepts strings having odd 0s and odd 1s
// 4 0      //nStates       //initialState
// 1 3      //nFinalStates  //finalStates[]
// 2 0 1    //nSymbols      //symbol[]
// 0 1 2    //state[]       //transition[][]
// 1 0 3
// 2 3 0
// 3 2 1

#include <bits/stdc++.h>
using namespace std;

int main()
{
    // fetching automata input from the file
    FILE *filePointer = fopen("1_DFA.txt", "r");
    int nStates, initialState;
    fscanf(filePointer, "%d %d", &nStates, &initialState);

    int nFinalStates;
    fscanf(filePointer, "%d", &nFinalStates);

    vector<bool> isFinal(nStates, false);
    for (int i = 0; i < nFinalStates; i++)
    {
        int temp;
        fscanf(filePointer, "%d", &temp);
        if (0 <= temp && temp < nStates)
            isFinal[temp] = true;
    }

    int nSymbols;
    fscanf(filePointer, "%d", &nSymbols);

    map<char, int> symbolIndex;
    for (int i = 0; i < nSymbols; i++)
    {
        char temp;
        fscanf(filePointer, "%*c %c", &temp);
        symbolIndex.insert({temp, i});
    }

    vector<vector<int>> transition(nStates, vector<int>(nSymbols));
    for (int i = 0; i < nStates; i++)
    {
        fscanf(filePointer, "%*d");
        for (int j = 0; j < nSymbols; j++)
        {   
            int temp;
            fscanf(filePointer, "%d", &temp);
            transition[i][j] = temp;
        }
    }

    fclose(filePointer);
    printf("Automata setup complete\n");
    // automata setup complete
    
    while (true)
    {
        string input;
        printf("\nInput String (or '!' to exit): ");
        getline(cin, input);
        if (input == "!")
        {
            return 0;
        }

        // String traversal starts here
        int currentState = initialState;
        int nextState = -1;
        int currentSymbol = -1;
        for (int i = 0; i < input.size(); i++)
        {
            printf("State q%d on input %c leads us to ", currentState, input[i]);
            if (symbolIndex.find(input[i]) == symbolIndex.end()) // if the symbol is not defined
            {
                currentState = -1;
                printf("the null state");
                break;
            }
            else
            {
                currentSymbol = symbolIndex[input[i]];
                nextState = transition[currentState][currentSymbol];
            }
            if (currentState < 0) // if there is no transition possible
            {
                printf("the null state");
                break;
            }
            printf("state q%d\n", nextState);
            currentState = nextState;
        }
        // check if string ends on a valid final state
        if (0 <= currentState && currentState < nStates && isFinal[currentState] == 1)
        {
            printf("\nThe input string is accepted\n ");
        }
        else
        {
            printf("\nThe input string is rejected\n ");
        }
    }
}
