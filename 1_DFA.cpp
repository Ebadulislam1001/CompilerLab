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

int main()
{
    // characteristics of a dfa
    int nStates, initialState, nFinalStates, nSymbols;
    vector<bool> isFinal(nStates, false);
    map<char, int> symbolIndex;
    vector<vector<int>> transition;

    // fetching input from the file
    FILE *filePointer = fopen("1_DFA.txt", "r");
    fscanf(filePointer, "%d %d", &nStates, &initialState);
    fscanf(filePointer, "%d", &nFinalStates);
    for (int i = 0; i < nFinalStates; i++)
    {
        int temp;
        fscanf(filePointer, "%d", &temp);
        if (temp >= 0 && temp < nStates)
            isFinal[temp] = true;
    }
    fscanf(filePointer, "%d", &nSymbols);
    for (int i = 0; i < nSymbols; i++)
    {
        char temp;
        fscanf(filePointer, "%*c %c", &temp);
        symbolIndex.insert({temp, i});
    }
    for (int i = 0; i < nStates; i++)
    {
        vector<int> row;
        fscanf(filePointer, "%*d");
        for (int j = 0; j < nSymbols; j++)
        {   
            int temp;
            fscanf(filePointer, "%d", &temp);
            row.push_back(temp);
        }
        transition.push_back(row);
    }
    fclose(filePointer);

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

        // String traversal starts here
        int currentState = initialState;
        int currentSymbol;
        for (int i = 0; i < input.size(); i++)
        {
            printf("State q%d on input %c leads us to ", currentState, input[i]);
            if (symbolIndex.find(input[i]) == symbolIndex.end()) // if the symbol is not defined
            {
                currentState = -1;
                printf("the null state", currentState);

            }
            else
            {
                currentSymbol = symbolIndex[input[i]];
                currentState = transition[currentState][currentSymbol];
                printf("state q%d\n", currentState);
            }
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
