# XY-Model
Simulation of XY-Model
# Usage
This code is parallelized with std c++, you may need to change the number of thread in the main.cpp, set the ```MAX_THREAD``` macro to the max logical threads of your CPU

if you want to save the simulation state, you may need to chage the output of the state folder at the very beginning of the main.cpp 
```string stateFileFolder = "c:\\XYModel_State\\";```

You can tweak the code so the final energy converge to the right value by checking the chage of the energy if it does converge

