# Using ACS to solve traditional TSP problem
## Process of ACS
1. extracting the test data from "test_data.txt" and calculating the distance matrix
- Pheromone initialization.
- Ants initialization. The part will choose a start city for each ant.
- constructing the solution of each ant and updating the pheromone.
- After all ants have created a new solution, we will process on global pheromone update.
- stop if we meet the terminated conditions, otherwise go back to step 4.

## Experiment:
- test case: oliver30
- dimentions : 30 locations
- Best: 423.741
- order:  0 -> 1 -> 29 -> 28 -> 27 -> 26 -> 25 -> 23 -> 24 -> 22 -> 21 -> 20 -> 19 -> 18 -> 17 -> 16 -> 15 -> 14 -> 13 -> 12 -> 11 -> 10 -> 9 -> 8 -> 7 -> 6 -> 5 -> 4 -> 3 -> 2 -> 0

## Reference
 Marco Dorigo, Luca Maria Gambardella, "Ant Colony System: A Cooperative Learning Approach to the Traveling Salesman Problem", *IEEE Transactions on Evolutionary Computation*, Vol.1, No.1, 1997
