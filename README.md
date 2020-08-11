# Chess app rough cut

## Hosted live
https://playchessnstuff.com

## Project Description
This is a project that takes four zero sum board games (chess, checkers, connect four, and tic tac toe), and creates a general purpose decision tree AI using the min max algorithm to play them. It also has both local and online multiplayer for all of these games

## Tools used

The user interface is a React app that calls a web assembly module. All of the logic for each game and the decision tree AI was written in C++ and compiled to web assembly. The back end validates each move and stores a "master" copy of each turn using the same C++ game logic applied on the front end, but with a python wrapper.