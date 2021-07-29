# car_garage - threads and synchronization (Linux & C)
This exercise simulates a car repair garage,deals with the use of many threads that share common resources between them,
and ensures the correct and fair distribution of these resources.
The garage has various capable resources: "Lift" (lift device), engine oil drain cart, kit Computerized engine test, light adjustment system, front wheel adjustment kit ("front"), and more. There are also several systems of the same type in the garage, which can be used to perform identical treatments at the same time on Different cars.
A resource can also be a worker, who only has the skill needed to perform A certain treatment. Many cars come to the garage that require care. 
Each car may require several types of care, And of course these may be different from one car to another. 
Each type of treatment requires a fixed time: one hour, two hours, three, four hours and even more. 
No more promotions from one car treatment at a time, even if the car needs several treatments: they are performed in one another one, and even then, only if provided that the resources necessary for their implementation are available. 
There are 3 input files: resources.txt: The list of resources in the garage. 
services.txt: List of all types of treatments that can be obtained in the garage. 
requests.txt: List of cars seeking garage care.
This exercise is a simulation of a real system, visualization allows freedom in determining the durations of the execution stages: we think an hour of car care using one second in the program. That way, where k hours are required to perform an action, it is simulated using the sleep (k) command (when k is in seconds).
