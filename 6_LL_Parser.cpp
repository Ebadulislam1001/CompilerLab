// sample input for a CFG
// A CB#    // LHS RHS#
// B +CB#
// B #
// C ED#
// D *ED#
// D #
// E (A)#
// E a#     // '#' stands for Epsilon

#include <bits/stdc++.h>
using namespace std;

// characteristics of a CFG             // (global variables)
char startSymbol = '#';                 // startSymbol of the grammar
vector<char> V;                         // set of variables
vector<char> T;                         // set of terminals
set<char> nullVariables;                // set of variables that may produce epsilon
vector<pair<char, string>> productions; // vector of pairs where: first is LHS, second is RHS
map<char, set<char>> firstSets;         // map where key is a variable and value is its firstSet
map<char, set<char>> followSets;        // map where key is a variable and value is its followSet

set<char> FIRST(char LHS, set<char> fiSet, set<char> path);
set<char> FIRST(string RHS, set<char> fiSet, set<char> path);
int index(vector<char> V, char c)
{
    for (int i = 0; i < V.size(); i++)
    {
        if (V[i] == c)
        {
            return i;
        }
    }
    return -1;
}

int main()
{
    // reading the grammar from CFG.txt
    char del, var, sym;
    FILE *filePointer;
    filePointer = fopen("5_CFG.txt", "r");
    do
    {
        // reading a production
        char LHS;
        string RHS;
        // reading LHS
        fscanf(filePointer, "%c%*c", &LHS);
        // setting first variable that appears as the startSymbol
        if (startSymbol == '#')
            startSymbol = LHS;
        // reading RHS
        do
        {
            fscanf(filePointer, "%c", &del);
            RHS.push_back(del);
            if (del < 'A' || del > 'Z')
            {
                if (find(T.begin(), T.end(), del) == T.end())
                    T.push_back(del);
            }
        } while (del != '#');
        // pushing LHS & RHS as a pair
        productions.push_back({LHS, RHS});
        // checking for EOF
        fscanf(filePointer, "%c", &del);
    } while (del == '\n');
    fclose(filePointer);
    filePointer = fopen("5_FnF.txt", "r");
    T.erase(find(T.begin(), T.end(), '#'));
    T.push_back('$');

    // filling sets of variables
    for (int i = 0; i < productions.size(); i++)
    {
        if (find(V.begin(), V.end(), productions[i].first) == V.end())
            V.push_back(productions[i].first);
    }

    // reading the fiSets & foSets from FnF.txt
    for (int i = 0; i < V.size(); i++)
    {
        // reading the variable
        fscanf(filePointer, "%c%c", &var, &del);
        set<char> fiSet;
        // reading the fiSet
        while (del != '_')
        {
            fscanf(filePointer, "%c%c", &sym, &del);
            fiSet.insert(sym);
        }
        firstSets.insert({var, fiSet});
        // checking for del
        fscanf(filePointer, "%c", &del);
    }
    for (int i = 0; i < V.size(); i++)
    {
        // reading the variable
        char var, sym;
        // reading the set
        fscanf(filePointer, "%c%c", &var, &del);
        // reading the foSet
        set<char> foSet;
        while (del != '_')
        {
            fscanf(filePointer, "%c%c", &sym, &del);
            foSet.insert(sym);
        }
        followSets.insert({var, foSet});
        // checking for del
        fscanf(filePointer, "%c", &del);
    }
    fclose(filePointer);

    // finding variables that produce epsilon directly
    for (int i = 0; i < productions.size(); i++)
    {
        if (productions[i].second.size() == 1)
        {
            nullVariables.insert(productions[i].first);
        }
    }
    // finding variables that produce epsilon indirectly
    int change = 1;
    while (change == 1)
    {
        change = 0;
        for (int i = 0; i < productions.size(); i++)
        {
            int potential = 1;
            if (nullVariables.find(productions[i].first) != nullVariables.end())
            {
                continue;
            }
            for (int j = 0; j < productions[i].second.size() - 1; j++)
            {
                if (nullVariables.find(productions[i].second[j]) == nullVariables.end())
                {
                    potential = 0;
                    break;
                }
            }
            if (potential == 1)
            {
                nullVariables.insert(productions[i].first);
                change = 1;
            }
        }
    }

    // preparing to fill the LL parse table
    vector<vector<int>> parseTable;
    for (int i = 0; i < V.size(); i++)
    {
        vector<int> row(T.size(), 0);
        parseTable.push_back(row);
    }

    // filling the LL parse table
    for (int i = 0; i < productions.size(); i++)
    {
        // for every production A -> RHS
        if (productions[i].second.size() == 1)
        {
            // if RHS == # : fill it in table[A][follow(A)]
            char var = productions[i].first; // to access A
            auto it = followSets.find(var);  // to access follow(A)
            for (auto jt = it->second.begin(); jt != it->second.end(); jt++)
            {
                int x = index(V, var);
                int y = index(T, *jt);
                if (parseTable[x][y] == 0)
                {
                    parseTable[x][y] = i + 1;
                }
                else
                {
                    printf("\nThis CFG isnt feasible for LL(1) parser");
                    return 0;
                }
            }
        }
        else
        {
            // if RHS != # : fill it in table[A][first(RHS)];
            char var = productions[i].first; // to access A
            set<char> fiSet;
            fiSet = FIRST(productions[i].second, fiSet, fiSet);
            for (auto jt = fiSet.begin(); jt != fiSet.end(); jt++)
            {
                int x = index(V, var);
                int y = index(T, *jt);
                if (parseTable[x][y] == 0)
                {
                    parseTable[x][y] = i + 1;
                }
                else
                {
                    printf("\nThis CFG isnt feasible for LL(1) parser");
                    return 0;
                }
            }
        }
    }

    printf("\nLL(1) Parse Table\n\n");
    for (int j = 0; j < T.size(); j++)
    {
        printf("\t%c", T[j]);
    }
    for (int i = 0; i < V.size(); i++)
    {
        printf("\n%c", V[i]);
        for (int j = 0; j < T.size(); j++)
        {
            printf("\t%d", parseTable[i][j]);
        }
    }
    printf("\n\n");
    filePointer = fopen("6_LL_ParseTable.txt", "w");
    {
        fprintf(filePointer, "%d", V.size());
        for (int j = 0; j < T.size(); j++)
        {
            fprintf(filePointer, " %c", T[j]);
        }
        for (int i = 0; i < V.size(); i++)
        {
            fprintf(filePointer, "\n%c", V[i]);
            for (int j = 0; j < T.size(); j++)
            {
                fprintf(filePointer, " %d", parseTable[i][j]);
            }
        }
    }
    fclose(filePointer);

    return 0;
}

set<char> FIRST(char LHS, set<char> fiSet, set<char> path)
{
    if (firstSets.find(LHS) != firstSets.end())
    {
        return firstSets.find(LHS)->second;
    }
    if (!isupper(LHS))
    {
        fiSet.insert(LHS);
        return fiSet;
    }
    if (path.find(LHS) == path.end())
    {
        path.insert(LHS);
        for (int i = 0; i < productions.size(); i++)
        {
            if (productions[i].first == LHS)
            {
                fiSet.merge(FIRST(productions[i].second, fiSet, path));
            }
        }
    }
    return fiSet;
}
set<char> FIRST(string RHS, set<char> fiSet, set<char> path)
{
    if (!isupper(RHS[0]))
    {
        fiSet.insert(RHS[0]);
    }
    else if (nullVariables.find(RHS[0]) == nullVariables.end())
    {
        fiSet.merge(FIRST(RHS[0], fiSet, path));
    }
    else if (nullVariables.find(RHS[0]) != nullVariables.end())
    {
        fiSet.merge(FIRST(RHS[0], fiSet, path));
        fiSet.erase('#');
        fiSet.merge(FIRST(RHS.substr(1), fiSet, path));
    }
    return fiSet;
}