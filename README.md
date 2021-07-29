# car_garage - threads and synchronization (Linux & C)
This exercise simulates a car repair garage,deals with the use of many threads that share common resources between them,
and ensures the correct and fair distribution of these resources.
The garage has various types of resources: "Lift" (lifting device), engine oil drain cart,
Computerized engine test kit, light adjustment system, front wheel adjustment kit ("front"), and more.
There are also several systems of the same type in the garage, which can be used to perform identical treatments 
at the same time in different cars. 
A resource can also be an employee, who only has the skill needed to perform certain treatment.
Many cars come to the garage that require care. Each car may require several types of care,
And of course these may be different from one car to another.
Each type of treatment requires a fixed time: one hour, two hours, three, four hours and even more. 
It is forbidden to perform more than one treatment on a specific car at the same time, even if the car needs several treatments: they are performed in one another one, 
and even then, only if provided that the resources necessary for their implementation are available.
In the exercise there are 3 input files:
1.) resources.txt: The list of resources in the garage
- Each row in the file has three fields, separated by a tab mark (‘\ t’ :)
- Resource type: A number that identifies the resource as a single value
- resource name for example: "Light Orientation System"
- The number of such systems owned by the garage.
2.) services.txt: List of all types of treatments that can be obtained in the garage.
Each row in the file has several fields, separated by a tab sign (‘\ t’ :)
- Type of treatment: A number that identifies the treatment is unique
- The name of the treatment (For example: "Adjust lights")
- The number of hours this type of treatment requires (1 - 8)
- Number of resources required to perform this type of treatment (length of list below)
- List of resource numbers needed to perform the treatment (there may be a blank list)
3.) requests.txt: List of cars seeking garage care.
Each row in the file has several fields, separated by a tab sign (‘\ t’ :)
- The license number of the car (for example, 4120274.)
- Time of arrival of the car at the garage (0 - 23.)
- The number of types of care the car needs (the length of the list below)
- List of treatment numbers required for the same car (one or more)

As I wrote, the exercise is a simulation of a real vehicle repair system, the simulation allows freedom in determining the durations of the execution stages: 
they do not have to be the same as the execution times in a real system. So in my code an hour in reality will be converted to one second. 
That way, where k hours are required to perform a treatment i will use the command sleep (k) when k is in seconds.
