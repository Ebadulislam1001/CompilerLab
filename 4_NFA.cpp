// sample input of an NFA
// 3 0               //nStates       //initialState
// 1 1              //nFinalStates  //finalStates[]
// 3 a b c          //nSymbols      //symbol[]
// 0 0,1,2 1 -1     //state[]       //transition[][]
// 1 -1 2 1
// 2 -1 0,2 -1

#include <bits/stdc++.h>
using namespace std;

// to inherit the signature from multiple states
vector<int> logicalOR(vector<int> A, vector<int> B)
{
    vector<int> C;
    for (int i = 0; i < A.size() && i < B.size(); i++)
    {
        C.push_back(A[i] || B[i]);
    }
    return C;
}
// to inherit the isFinal property from multiple states
int logicalOR(int A, int B)
{
    int C;
    C = A || B;
    return C;
}

int main()
{
    // characteristics of a nfa
    int nStates, initialState, nFinalStates, nSymbols;
    char symbol[32];
    vector<int> transition[128][32];
    // whether tp print the details or not
    bool printProcess = true;

    // fetching input from the file
    FILE *readPointer = fopen("4_NFA.txt", "r");

    fscanf(readPointer, "%d", &nStates);
    fscanf(readPointer, "%d", &initialState);
    fscanf(readPointer, "%d", &nFinalStates);
    vector<int> isFinal(nStates, 0);
    for (int i = 0; i < nFinalStates; i++)
    {
        int index;
        fscanf(readPointer, "%d", &index);
        // setting isFinal = 1 for the given final states
        if (index >= 0 && index < nStates)
            isFinal[index] = 1;
    }
    // reading the set of input symbols
    fscanf(readPointer, "%d", &nSymbols);
    for (int i = 0; i < nSymbols; i++)
    {
        fscanf(readPointer, "%*c %c", &symbol[i]);
    }
    // uptil this part the file is same as DFA

    for (int i = 0; i < nStates; i++)
    {
        int index;
        fscanf(readPointer, "%d", &index);
        for (int j = 0; j < nSymbols; j++)
        {
            char del;
            do
            {
                int index;
                fscanf(readPointer, "%d%c", &index, &del);
                transition[i][j].push_back(index);
            } while (del == ',');
        }
    }
    fclose(readPointer);
    // reading from file part is done

    vector<int> nullSign(nStates, 0);
    vector<vector<int>> sign(nStates, nullSign);
    for (int i = 0; i < nStates; i++)
    {
        for (int j = 0; j < nStates; j++)
        {
            sign[i].push_back(0);
        }
        sign[i][i] = 1;
    }
    // printing the initial NFA
    if (printProcess == true)
    {
        printf("\nInitial NFA\n");
        for (int i = 0; i < nStates; i++)
        {
            for (int j = 0; j < nSymbols; j++)
            {
                printf("{");
                for (int k = 0; k < transition[i][j].size(); k++)
                {
                    printf("%d,", transition[i][j][k]);
                }
                printf("}\t");
            }
            printf("\n");
        }
    }
    // checking non determinancy
    for (int i = 0; i < nStates; i++)
    {
        if (printProcess == true)
            printf("\nchecking determinancy for state %d\n", i);
        for (int j = 0; j < nSymbols; j++)
        {
            if (transition[i][j].size() > 1) // non-deterministic behaviour detected
            {
                int match;
                if (printProcess == true)
                    printf("non determinancy found at cell %d,%d\n", i, j);
                vector<int> buffer = transition[i][j];
                vector<int> unionStateSign = nullSign;
                for (int k = 0; k < buffer.size(); k++)
                {
                    if (buffer[k] >= 0)
                        unionStateSign = logicalOR(unionStateSign, sign[buffer[k]]);
                }
                if (printProcess == true)
                {
                    printf("sign of this combination = ");
                    for (int x = 0; x < unionStateSign.size(); x++)
                    {
                        printf("%d", unionStateSign[x]);
                    }
                    printf("\n");
                }
                bool matchFound = 0;
                for (int k = 0; k < nStates; k++)
                {
                    if (sign[k] == unionStateSign)
                    {
                        match = k;
                        matchFound = 1;
                        transition[i][j].clear();
                        transition[i][j].push_back(k);
                        if (printProcess == true)
                        {
                            printf("this combination is already present as state %d\n", k);
                            printf("overwritten as a single state %d\n", k);
                        }
                        // non deterministic cell is updated
                    }
                }
                if (matchFound == 0)
                {
                    transition[i][j].clear();
                    transition[i][j].push_back(nStates);
                    if (printProcess == true)
                    {
                        printf("this is a new combination\n");
                        printf("overwritten as a single state %d\n", match);
                        printf("new size = %d\n", transition[i][j].size());
                    }
                    // non deterministic cell is updated

                    // new row is created for a new state
                    sign.push_back(unionStateSign);
                    isFinal.push_back(0);
                    for (int k = 0; k < buffer.size(); k++) // loop for calculating isFinal of the new state
                    {
                        if (buffer[k] >= 0)
                            isFinal[nStates] = logicalOR(isFinal[nStates], isFinal[buffer[k]]);
                    }
                    if (isFinal[nStates] == 1)
                    {
                        nFinalStates++;
                    }
                    if (printProcess == true)
                    {
                        printf("new state is added as state %d with sign = ", nStates);
                        for (int x = 0; x < sign[nStates].size(); x++) // loop for printing sign of the new state
                        {
                            printf("%d", sign[nStates][x]);
                        }
                        printf(" & isFinal = %d\n", isFinal[nStates]);
                    }
                    // calculating transitions of the new state
                    for (int k = 0; k < nSymbols; k++)
                    {
                        for (int l = 0; l < buffer.size(); l++)
                        {
                            for (int m = 0; m < transition[buffer[l]][k].size(); m++)
                            {
                                if (transition[buffer[l]][k][m] >= 0)
                                    transition[nStates][k].push_back(transition[buffer[l]][k][m]);
                            }
                        }
                        if (transition[nStates][k].size() == 0)
                        {
                            transition[nStates][k].push_back(-1);
                        }
                    }
                    if (printProcess == true)
                    {
                        printf("and transitions = ");
                        for (int k = 0; k < nSymbols; k++)
                        {
                            printf("{");
                            for (int l = 0; l < transition[nStates][k].size(); l++)
                            {
                                printf("%d, ", transition[nStates][k][l]);
                            }
                            printf("}  ");
                        }
                        printf("\n");
                    }
                    nStates += 1;
                }
            }
        }
        if (printProcess == true)
            printf("determinancy cleared for state %d\n", i);
    }
    // NFA is supposedsly converted to a DFA
    // Lets check
    // printing the initial NFA
    if (printProcess == true)
    {
        printf("\nFinal DFA\n");
        for (int i = 0; i < nStates; i++)
        {
            for (int j = 0; j < nSymbols; j++)
            {
                printf("{");
                for (int k = 0; k < transition[i][j].size(); k++)
                {
                    printf("%d", transition[i][j][k]);
                }
                printf("}\t");
            }
            printf("\n");
        }
    }
    // yoohoo, its working
    // writing it on a separate txt file
    FILE *writePointer = fopen("4_NFAtoDFA.txt", "w");
    fprintf(writePointer, "%d ", nStates);
    fprintf(writePointer, "%d\n", initialState);
    fprintf(writePointer, "%d", nFinalStates);
    for (int i = 0; i < nStates; i++)
    {
        if (isFinal[i] == 1)
            fprintf(writePointer, " %d", i);
    }

    fprintf(writePointer, "\n%d", nSymbols);
    for (int i = 0; i < nSymbols; i++)
    {
        fprintf(writePointer, " %c", symbol[i]);
    }
    for (int i = 0; i < nStates; i++)
    {
        fprintf(writePointer, "\n%d", i);
        for (int j = 0; j < nSymbols; j++)
        {
            fprintf(writePointer, " %d", transition[i][j][0]);
        }
    }
    fclose(writePointer);
    printf("\nGiven NFA is converted into the corresponding DFA.");
    return 0;
}