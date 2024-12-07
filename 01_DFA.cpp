// sample input for DFA that accepts strings having odd 0s and odd 1s
// 4        // stateCount
// 0        // initState
// 1 3      // finalStateCount  // finalStates[]
// 2 0 1    // alphaCount       // alphabets
// 0 1 2    // states[]         // transitions[][]
// 1 0 3
// 2 3 0
// 3 2 1

#include <iostream>
#include <string>
#include <vector>
#include <map>
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
    int finalStateCount;
    vector<bool> isStateFinal;
    // - a finite set of alphabets
    int alphaCount;
    vector<char> alphabets;
    map<char, int> alphaIndex;
    // - a finite set of transitions
    vector<vector<int>> transitions;

public:
    bool configure(string path);
    void show_table();
    void simulate();
};

int main()
{
    DFA dfa;
    dfa.configure("./1_DFA.txt");
    dfa.show_table();

    while (true)
    {
        dfa.simulate();
    }
}

bool DFA :: configure(string path)
{
    FILE *filePointer = fopen(path.c_str(), "r");
    if (filePointer == NULL)
    {
        printf("Error: file not found!!\n");
        return false;
    }

    // - a finite set of states
    fscanf(filePointer, "%d", &this->stateCount);
    // printf("stateCount = %d\n", this->stateCount);

    // - a single initial state
    fscanf(filePointer, "%d", &this->initState);
    // printf("initState = %d\n", this->initState);

    // - a finite set of final states
    fscanf(filePointer, "%d", &this->finalStateCount);
    // printf("finalStateCount = %d\n", this->finalStateCount);

    this->isStateFinal = vector<bool>(this->stateCount, false);
    for (int state = 0; state < this->finalStateCount; state++)
    {
        int index;
        fscanf(filePointer, "%d", &index);
        // printf("isStateFinal = %d\n", index);
        if (0 <= index && index < this->stateCount)
        {
            this->isStateFinal[index] = true;
        }
    }

    // - a finite set of alphabets
    fscanf(filePointer, "%d", &this->alphaCount);
    // printf("alphaCount = %d\n", this->alphaCount);

    for (int index = 0; index < this->alphaCount; index++)
    {
        char alpha;
        fscanf(filePointer, "%*c %c", &alpha);
        // printf("alpha = %c, index = %d\n", alpha, index);
        this->alphabets.push_back(alpha);
        this->alphaIndex.insert({alpha, index});
    }

    // - a finite set of transitions
    this->transitions = vector<vector<int>>(this->stateCount, vector<int>(this->alphaCount));
    for (int state = 0; state < this->stateCount; state++)
    {
        fscanf(filePointer, "%*d");
        for (int alpha = 0; alpha < this->alphaCount; alpha++)
        {
            int nextState;
            fscanf(filePointer, "%d", &nextState);
            // printf("state = %d, alpha = %d, next = %d\n", state, alpha, nextState);
            this->transitions[state][alpha] = nextState;
        }
    }

    fclose(filePointer);
    return true;
}
void DFA :: show_table()
{
    printf("DFA_:");
    string lineSep = "+-----+";
    for (int alpha = 0; alpha < this->alphaCount; alpha++)
    {
        lineSep += "-----+";
    }
    printf("\n%s", lineSep.c_str());
    printf("\n|   # |");
    for (int alpha = 0; alpha < this->alphaCount; alpha++)
    {
        printf("   %c |", this->alphabets[alpha]);
    }

    for (int state = 0; state < this->stateCount; state++)
    {
        printf("\n%s\n|", lineSep.c_str());
        printf((this->isStateFinal[state]) ? "*" : " ");
        printf("%3d |", state);
        for (int alpha = 0; alpha < this->alphaCount; alpha++)
        {
            printf(" %3d |", this->transitions[state][alpha]);
        }
    }
    printf("\n%s\n", lineSep.c_str());
}
void DFA :: simulate()
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