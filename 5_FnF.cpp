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
vector<pair<char, string>> productions; // vector of pairs where: first is LHS, second is RHS
set<char> nullVariables;                // ebad is so handsome
set<char> variables;                    // sets of variables
map<char, set<char>> firstSets;         // map where key is a variable and value is its firstSet
map<char, set<char>> followSets;        // map where key is a variable and value is its followSet

// first & follow functions prototypes and algorithms
set<char> FIRST(char LHS, set<char> fiSet, set<char> path);
// LHS is the symbol whose firstSet is to be found
//
// if this firstSet of this variable is already calculated
//      return that previuosly calculated firstSet
// if LHS symbol is a terminal:
//      fiSet += LHS
//      return fiSet
// if LHS is a variable that doesnt exists in path i.e. its not a self dependent loop:
//      path += LHS
//      find FIRST of all RHS strings that corresponds with this LHS symbol
//      merge these firstSets so found with fiSet
//      return fiSet
set<char> FIRST(string RHS, set<char> fiSet, set<char> path);
// RHS is the string whose firstSet is to be found
//
// if first char of RHS string is a terminal:
//      fiSet += RHS[0]
// else if first char of RHS string is a variable that doesnt produce epsilon:
//      fiSet += FIRST(RHS[0], fiSet, path)
// else if first char of RHS string is a variable that does produce epsilon:
//      fiSet += FIRST(RHS[0],fiSet,path)
//      fiSet -= '#'
//      fiSet += FIRST(RHS.substr(1),fiSet,path)
// return fiSet
set<char> FOLLOW(char var, set<char> foSet, set<char> path);
// var is the variable whose followSet is to be found
//
// if the followSet of this variable is already calculated
//      return that previuosly calculated followSet
// if var is a variable that doesnt exist in path i.e. its not a self dependent loop:
//      if var == startSymbol:
//          foSet += '$'
//      check RHS of each production for the occurence of the var symbol
//      if the var symbol is found in an RHS of a production:
//          foSet = FIRST(RHS.subst(after var), emptySet, emptySet)
//          if foSet contains '#':
//              foSet -= '#'
//              path += var
//              foSet += FOLLOW(LHS, foSet, path)
// return foSet

int main()
{
    // fetching input from the file
    char del;
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
        } while (del != '#');
        // LHS & RHS pushed as a pair
        productions.push_back({LHS, RHS});
        // checking for EOF
        fscanf(filePointer, "%c", &del);
    } while (del == '\n');
    // printing all the productions
    printf("\nprinting PRODUCTIONS:");
    for (int i = 0; i < productions.size(); i++)
    {
        // first refers to LHS, second refers to RHS
        cout << "\n{" << productions[i].first << "} -> {" << productions[i].second << "}";
    }

    // filling sets of variables
    for (int i = 0; i < productions.size(); i++)
    {
        variables.insert(productions[i].first);
    }
    // finding variables that produce epsilon directly
    for (int i = 0; i < productions.size(); i++)
    {
        // checking each production one by one
        if (productions[i].second.size() == 1)
        {
            // RHS of this production is a epsilon, add the LHS variable in nullVariable set
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
            // iterating through all productions one by one
            int potential = 1;
            if (nullVariables.find(productions[i].first) != nullVariables.end())
            {
                // this variable is already found to be producing epsilon. No need to check again
                continue;
                // check the next productions
            }
            // if this variable is not yet found to be producing epsilon
            for (int j = 0; j < productions[i].second.size() - 1; j++)
            {
                // checking if this production is made up of only those variables that are found to be producing epsilon
                if (nullVariables.find(productions[i].second[j]) == nullVariables.end())
                {
                    // there is a variable which is not yet found to be producing epsilon
                    // we cant yet claim that this variable produces epsilon
                    potential = 0;
                    break;
                }
            }
            if (potential == 1)
            {
                // this production is made up of only those variables that are found to be producing epsilon
                // hence this variable (indirectly) produces epsilon
                nullVariables.insert(productions[i].first);
                change = 1;
            }
        }
    }

    // finding firstSets
    for (auto it = variables.begin(); it != variables.end(); it++)
    {
        // initialising an empty char set
        set<char> fiSet;
        // finding firstSet of each variable one by one and storing it in our temp set fiSet
        fiSet = FIRST(*it, fiSet, fiSet);
        // pushing variable with its firstSet in the map as a key value pair
        firstSets.insert({*it, fiSet});
    }
    printf("\n\nprinting FIRST SETS:");
    for (auto it = firstSets.begin(); it != firstSets.end(); it++)
    {
        printf("\n%c\t{", it->first);
        for (auto jt = it->second.begin(); jt != it->second.end(); jt++)
        {
            printf("%c, ", *jt);
        }
        printf("}");
    }

    // finding followSets
    for (auto it = variables.begin(); it != variables.end(); it++)
    {
        // initialising an empty char set
        set<char> foSet;
        // finding followSet of each variable one by one and storing it in our temp set foSet
        foSet = FOLLOW(*it, foSet, foSet);
        // pushing variable with its followSet in the map as a key value pair
        followSets.insert({*it, foSet});
    }
    printf("\n\nprinting FOLLOW SETS:");
    for (auto it = followSets.begin(); it != followSets.end(); it++)
    {
        printf("\n%c\t{", it->first);
        for (auto jt = it->second.begin(); jt != it->second.end(); jt++)
        {
            printf("%c, ", *jt);
        }
        printf("}");
    }
    printf("\n\n");

    // write first and follow sets in a txt file
    filePointer = fopen("5_FnF.txt", "w");

    // write first sets in a txt file
    for (auto it = firstSets.begin(); it != firstSets.end(); it++)
    {
        fprintf(filePointer, "%c", it->first);
        for (auto jt = it->second.begin(); jt != it->second.end(); jt++)
        {
            fprintf(filePointer, " %c", *jt);
        }
        fprintf(filePointer, "_\n");
    }
    // write folllow sets in a txt file
    for (auto it = followSets.begin(); it != followSets.end(); it++)
    {
        fprintf(filePointer, "%c", it->first);
        for (auto jt = it->second.begin(); jt != it->second.end(); jt++)
        {
            fprintf(filePointer, " %c", *jt);
        }
        fprintf(filePointer, "_\n");
    }
    return 0;
}

set<char> FIRST(char LHS, set<char> fiSet, set<char> path)
{

    // if this firstSet of this variable is already calculated
    if (firstSets.find(LHS) != firstSets.end())
    {
        // return that previuosly calculated firstSet
        return firstSets.find(LHS)->second;
    }
    // if LHS symbol is a terminal:
    if (!isupper(LHS))
    {
        // fiSet += LHS
        fiSet.insert(LHS);
        // return fiSet
        return fiSet;
    }
    // if LHS is a variable that doesnt exists in path i.e. its not a self dependent loop
    if (path.find(LHS) == path.end())
    {
        // path += LHS
        path.insert(LHS);
        for (int i = 0; i < productions.size(); i++)
        {
            // find FIRST of all RHS strings that corresponds with this LHS variable
            if (productions[i].first == LHS)
            {
                // merge these firstSets so found with fiSet
                fiSet.merge(FIRST(productions[i].second, fiSet, path));
            }
        }
    }
    // return fiSet
    return fiSet;
}
set<char> FIRST(string RHS, set<char> fiSet, set<char> path)
{
    // if first char of RHS string is a terminal:
    if (!isupper(RHS[0]))
    {
        // fiSet += RHS[0]]
        fiSet.insert(RHS[0]);
    }
    // else if first char of RHS string is a variable that doesnt produce epsilon:
    else if (nullVariables.find(RHS[0]) == nullVariables.end())
    {
        // fiSet += FIRST(RHS[0], fiSet, path)
        fiSet.merge(FIRST(RHS[0], fiSet, path));
    }
    // else if first char of RHS string is a variable that does produce epsilon:
    else if (nullVariables.find(RHS[0]) != nullVariables.end())
    {
        // fiSet += FIRST(RHS[0],fiSet,path)
        fiSet.merge(FIRST(RHS[0], fiSet, path));
        // fiSet -= '#'
        fiSet.erase('#');
        // fiSet += FIRST(RHS.substr(1),fiSet,path)
        fiSet.merge(FIRST(RHS.substr(1), fiSet, path));
    }
    // return fiSet
    return fiSet;
}
set<char> FOLLOW(char var, set<char> foSet, set<char> path)
{
    // if the followSet of this variable is already calculated
    if (followSets.find(var) != followSets.end())
    {
        // return that previuosly calculated followSet
        return followSets.find(var)->second;
    }
    // if var is a variable that exists in path i.e. its not a self dependent loop:
    if (path.find(var) == path.end())
    {
        // if var == startSymbol:
        if (var == startSymbol)
        {
            // foSet += '$'
            foSet.insert('$');
        }
        // check RHS of each production for the occurence of the var symbol
        for (int i = 0; i < productions.size(); i++)
        {
            // if the var symbol is found in an RHS of a production:
            if (productions[i].second.find(var) < productions[i].second.size())
            {
                set<char> emptySet;
                // foSet = FIRST(RHS.subst(after var), emptySet, emptySet)
                foSet.merge(FIRST(productions[i].second.substr(productions[i].second.find(var) + 1), emptySet, emptySet));

                // if foSet contains '#':
                if (foSet.find('#') != foSet.end())
                {
                    // foSet -= '#'
                    foSet.erase('#');
                    // path += var
                    path.insert(var);
                    // foSet += FOLLOW(LHS, foSet, path)
                    foSet.merge(FOLLOW(productions[i].first, foSet, path));
                } //
            }
        }
    }
    // return foSet
    return foSet;
}