#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Libraries/File_Handling.cpp"
using namespace std;

class DFA
{
private:
    // A DFA (Deterministic Finite Automaton) can be represented as 5 tuples:

    // - a finite set of states
    int stateCount;
    // - a single initial state
    int initState;
    // - a finite set of final states
    vector<bool> isStateFinal;
    // - a finite set of alphabets
    vector<char> alphabets;
    map<char, int> alphaIndex;
    // - a finite set of transitions
    vector<vector<int>> transitions;

public:
    bool configure(string filePath);
    void show_table();
    void simulate();
};

int main()
{
    // ON START:
    DFA dfa;
    string filePath = "./01_DFA.txt";
    bool valid = dfa.configure(filePath);

    // FOREVER:
    while (valid)
    {
        dfa.show_table();
        dfa.simulate();
    }
}

bool DFA ::configure(string filePath)
{
    FILE *filePointer = fopen(filePath.c_str(), "r");
    if (filePointer == NULL)
    {
        printf("Error: file not found!!\n");
        return false;
    }

    // - a finite set of states
    this->stateCount = read_int(filePointer);
    // printf("stateCount = %d\n", this->stateCount);

    // - a single initial state
    this->initState = read_int(filePointer);
    // printf("initState = %d\n", this->initState);

    // - a finite set of final states
    int finalStateCount = read_int(filePointer);
    // printf("finalStateCount = %d\n", finalStateCount);

    bool atleastOneFinalState = false;
    this->isStateFinal = vector<bool>(this->stateCount, false);
    for (int state = 0; state < finalStateCount; state++)
    {
        int index = read_int(filePointer);
        // printf("finalState = %d\n", index);
        if (0 <= index && index < this->stateCount)
        {
            this->isStateFinal[index] = true;
            atleastOneFinalState = true;
        }
        else
        {
            printf("Error: Invalid Final State!!\n");
        }
    }
    if (!atleastOneFinalState)
    {
        printf("Error: DFA must have atleast one final state!!\n");
        return false;
    }

    // - a finite set of alphabets
    int alphaCount = read_int(filePointer);
    // printf("alphaCount = %d\n", alphaCount);

    for (int index = 0; index < alphaCount; index++)
    {
        skip_chars(filePointer, 1);
        char alpha = read_char(filePointer);
        // printf("alpha = %c, index = %d\n", alpha, index);
        this->alphabets.push_back(alpha);
        this->alphaIndex.insert({alpha, index});
    }

    // - a finite set of transitions
    this->transitions = vector<vector<int>>(this->stateCount, vector<int>(alphaCount));
    for (int state = 0; state < this->stateCount; state++)
    {
        int index = read_int(filePointer); // unused

        for (int alpha = 0; alpha < alphaCount; alpha++)
        {
            int nextState = read_int(filePointer);
            // printf("state = %d, alpha = %d, next = %d\n", state, alpha, nextState);
            this->transitions[state][alpha] = nextState;
        }
    }

    fclose(filePointer);
    return true;
}
void DFA ::show_table()
{
    printf("\nDFA_:");
    string lineSep = "+-----+";
    int alphaCount = this->alphabets.size();
    for (int alpha = 0; alpha < alphaCount; alpha++)
    {
        lineSep += "-----+";
    }
    printf("\n%s", lineSep.c_str());
    printf("\n|   # |");
    for (int alpha = 0; alpha < alphaCount; alpha++)
    {
        printf("   %c |", this->alphabets[alpha]);
    }

    for (int state = 0; state < this->stateCount; state++)
    {
        printf("\n%s\n|", lineSep.c_str());
        printf((this->isStateFinal[state]) ? "*" : " ");
        printf("%3d |", state);
        for (int alpha = 0; alpha < alphaCount; alpha++)
        {
            printf(" %3d |", this->transitions[state][alpha]);
        }
    }
    printf("\n%s\n", lineSep.c_str());
}
void DFA ::simulate()
{
    string input;
    printf("\nInput String : ");
    getline(cin, input);

    // intialising env variables
    int currentState = this->initState;
    int nextState = -1;
    int currentAlpha = -1;

    // to iterate over the `input` string
    for (int i = 0; i < input.size(); i++)
    {
        printf("State Q%d on input %c leads us to : ", currentState, input[i]);

        // if the input symbol is undefined
        if (this->alphaIndex.find(input[i]) == this->alphaIndex.end())
        {
            currentState = -1;
            printf("the null state\n");
            printf("The input symbol is undefined\n");
            break;
        }
        // else
        currentAlpha = this->alphaIndex[input[i]];
        nextState = this->transitions[currentState][currentAlpha];

        // if the input symbols leads you to the null state
        if (nextState < 0)
        {
            printf("the null state\n");
            break;
        }
        // else
        printf("state Q%d\n", nextState);
        currentState = nextState;
    }

    // check if string ends on a valid final state
    if ((0 <= currentState && currentState < this->stateCount) && (this->isStateFinal[currentState]))
    {
        printf("\nThe input string is accepted\n");
    }
    else
    {
        printf("\nThe input string is rejected\n");
    }
}