//The purpose of the program is to simulate a car garage
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#define MAX 256

struct timeval t1; //start time of program

typedef struct resources {
	char* typeR; //type of resource
	char* nameR; //name of resource
	int num_system; //amount of systems
	sem_t s;
} Resources;

typedef struct services {
	char* typeS; //type of service
	char* nameS; //name of service
	int num_hours; //number of hours for the service
	int num_resources; //number resources for the service
	int* list_resources; //list of resources
} Services;

typedef struct requests {
	int num_car; //number of car
	int hour_arrived; // the time the car arrived at the garage
	int num_services; //number of kind of services
	int* list_services; //list of services
} Requests;
Resources* resources = NULL;
Services* services = NULL;
Requests* requests = NULL;
int length_Resources = 0, length_Services = 0, length_Requests = 0; //number of lines in file
Resources* scanResources(char* file); //scan file of resources to array of structure Resources
Services* scanServices(char* file); //scan file of services to array of structure Services
Requests* scanRequests(char* file); //scan file of requests to array of structure Requests
void* takeRequests(void*); //function for implementation threads of requests
void bubbleSort(int arr[], int n);
void swap(int *xp, int *yp);
void printError(char *str); //error and exit
int main(int argc, char*file[]) {
	if (argc != 4) {
		printError("must insert valid arguments!");
	}
	int i, ans;
	pthread_t *t;
	resources = scanResources(file[1]);
	if (resources == NULL)
		printError("the function failed\n");
	services = scanServices(file[2]);
	if (services == NULL)
		printError("the function failed\n");
	requests = scanRequests(file[3]);
	if (requests == NULL)
		printError("the function failed\n");
	gettimeofday(&t1, NULL); //start timer
	t = (pthread_t*) malloc(sizeof(pthread_t) * length_Requests); //array of threads
	if (t == NULL)
		printError("allocation failed");
	for (i = 0; i < length_Requests; i++) {
		ans = pthread_create(&t[i], NULL, takeRequests, &requests[i]);
		if (ans != 0)
			printError("creation thread failed!");
	}
	for (i = 0; i < length_Requests; i++) { //wait to all threads
		pthread_join(t[i], NULL);
	}

//free allocation memory
	for (i = 0; i < length_Resources; i++) {
		free(resources[i].typeR);
		free(resources[i].nameR);

	}
	free(resources);
	for (i = 0; i < length_Services; i++) {
		free(services[i].typeS);
		free(services[i].nameS);
		if (services[i].list_resources != NULL)
			free(services[i].list_resources);

	}
	free(services);
	for (i = 0; i < length_Requests; i++) {
		free(requests[i].list_services);
	}
	free(requests);
	return 0;
}

void* takeRequests(void* request) {
	int num_services = (*(Requests*) request).num_services;
	int* list_services = (int*) malloc(sizeof(int) * num_services);
	if (list_services == NULL)
		printError("allocation failed!");
	int hour_arrived = (*(Requests*) request).hour_arrived;
	int num_car = (*(Requests*) request).num_car;
	int* sources;
	struct timeval t2; //stop time
	int i, j, s, r, v = 0;
	int elapsedTime; //current time that passed
	for (i = 0; i < num_services; i++) {
		list_services[i] = (*(Requests*) request).list_services[i];
	}
	sleep(hour_arrived); //sleep until the car arrives to garage
	printf("car: %d , arrived to garage at %d\n", num_car, hour_arrived);
	printf("car: %d , time: %d , the request is up for discussion\n", num_car,
			hour_arrived);
	for (j = 0; j < num_services; j++) {
		for (i = 0; i < length_Services; i++) {
			if (list_services[j] == atoi(services[i].typeS)) { //find suitable service in array service
				if (services[i].num_resources == 0) { //the service doesn't requests resources
					gettimeofday(&t2, NULL); //stop time
					elapsedTime = ((int) (t2.tv_sec - t1.tv_sec) * 1.0); //calculate passing time from start program until current time
					printf("car: %d , time: %d , started %s\n", num_car,
							elapsedTime, services[i].nameS);
					sleep(services[i].num_hours); //service time
					gettimeofday(&t2, NULL); //stop time
					elapsedTime = (int) ((t2.tv_sec - t1.tv_sec) * 1.0); //calculate passing time from start program until current time
					printf("car: %d , time: %d , completed %s\n", num_car,
							elapsedTime, services[i].nameS);
					if (j == num_services - 1) //last service
						printf("car: %d , time: %d , completed all services\n",
								num_car, elapsedTime);
				}

				else { //the service need resources
					sources = (int*) malloc(
							sizeof(int) * (services[i].num_resources)); //to save the indexes of resources in array Resources
					if (sources == NULL)
						printError("allocation failed!");
					for (r = 0; r < services[i].num_resources; r++) { //passing all resources of specific service
						for (s = 0; s < length_Resources; s++) //search the index resource in array Resources
							if (services[i].list_resources[r] == atoi(resources[s].typeR))
								sources[v++] = s;
					}
					for (v = 0; v < services[i].num_resources; v++) { //down to semaphore resources
						sem_wait(&(resources[sources[v]].s));//wait to resources in increasing order
					}
					gettimeofday(&t2, NULL); //stop time
					elapsedTime = (int) ((t2.tv_sec - t1.tv_sec) * 1.0); //calculate passing time from start program until current time
					printf("car: %d , time: %d , started %s\n", num_car,elapsedTime, services[i].nameS);
					sleep(services[i].num_hours); //service time
					gettimeofday(&t2, NULL); //stop time
					elapsedTime = (int) ((t2.tv_sec - t1.tv_sec) * 1.0); //calculate passing time from start program until current time
					printf("car: %d , time: %d , completed %s\n", num_car,elapsedTime, services[i].nameS);
					if (j == num_services - 1) //last service
						printf("car: %d , time: %d , completed all services\n",
								num_car, elapsedTime);
					for (v = 0; v < services[i].num_resources; v++) { //up to semaphore resources
						sem_post(&(resources[sources[v]].s));
					}
					v = 0;

				}
			}
		}

	}
	return NULL;
}

Requests* scanRequests(char* file) {
	int fd_read, rbytes, size = 0, i = 0, j = 0;
	Requests *req;
	char* buff;
	char temp[256];
	char* field;
	//open file to read
	fd_read = open(file, O_RDONLY);
	if (fd_read == -1)
		printError("Error open file");
	//calculate size of file
	rbytes = read(fd_read, temp, MAX);
	if (rbytes == -1)
		printError("Error read file");
	if (rbytes == 0)
		return NULL;
	while (rbytes > 0) {
		size += rbytes;
		rbytes = read(fd_read, temp, MAX);
		if (rbytes == -1)
			printError("Error read");
	}
	buff = (char*) malloc(sizeof(char) * (size + 1)); //allocation memory for scan file
	if (buff == NULL)
		printError("allocation failed!");
	lseek(fd_read, 0, SEEK_SET); //pointer to start file
	//read file to buff
	rbytes = read(fd_read, buff, size);
	if (rbytes == -1)
		printError("Error read file");
	while (rbytes > 0) {
		rbytes = read(fd_read, buff, size);
		if (rbytes == -1)
			printError("Error read");
	}
	buff[size] = '\0';
	req = (Requests*) malloc(sizeof(Requests)); //allocation memory for array of requests
	if (req == NULL)
		printError("allocation failed!");
	//scan fields
	field = strtok(buff, "\t");
	while (field != NULL) { //scan fields in line
		req[i].num_car = atoi(field);
		field = strtok(NULL, "\t"); //next field
		req[i].hour_arrived = atoi(field);
		field = strtok(NULL, "\t"); //next field
		req[i].num_services = atoi(field);

		req[i].list_services = (int*) malloc(
				sizeof(int) * (req[i].num_services)); //allocation memory for int array
		for (j = 0; j < req[i].num_services; j++) {
			field = strtok(NULL, "\n\t"); //next field until end of line or next field
			req[i].list_services[j] = atoi(field);
		}
		i++;
		field = strtok(NULL, "\n\t"); //first field in the next line
		if (field != NULL)
			req = (Requests*) realloc(req, sizeof(Requests) * (i + 1)); //extend array of Requests in one place

	}
	length_Requests = i;
	return req;
}
Resources* scanResources(char* file) {
	int fd_read, rbytes, size = 0, i = 0, ans;
	Resources* res;
	char* buff;
	char temp[256];
	char* field;
	fd_read = open(file, O_RDONLY); //open file to read
	if (fd_read == -1)
		printError("Error open file");

//calculate size of file
	rbytes = read(fd_read, temp, MAX);
	if (rbytes == -1)
		printError("Error read file");
	if (rbytes == 0)
		return NULL; //empty file
	while (rbytes > 0) {
		size += rbytes;
		rbytes = read(fd_read, temp, MAX);
		if (rbytes == -1) {
			printError("Error read");
		}
	}
	buff = (char*) malloc(sizeof(char) * (size + 1)); //allocation memory to scan document
	if (buff == NULL)
		printError("allocation failed");
	lseek(fd_read, 0, SEEK_SET); //pointer to start file
	//read file to buff
	rbytes = read(fd_read, buff, size);
	if (rbytes == -1)
		printError("Error read file");

	while (rbytes > 0) {
		rbytes = read(fd_read, buff, size);
		if (rbytes == -1)
			printError("Error read");
	}
	buff[size] = '\0';
	res = (Resources*) malloc(sizeof(Resources));//allocation array of Resources
	if (res == NULL)
		printError("fail allocation");
	//scan fields
	field = strtok(buff, "\t");
	while (field != NULL) {	//scan field from one line
		res[i].typeR = (char*) malloc(sizeof(char) * (strlen(field) + 1));//string allocation memory
		if (res[i].typeR == NULL)
			printError("allocation failed!");
		strcpy((res[i].typeR), field);
		field = strtok(NULL, "\t");	//next field
		res[i].nameR = (char*) malloc(sizeof(char) * (strlen(field) + 1));//string allocation memory
		if (res[i].nameR == NULL)
			printError("allocation failed");
		strcpy((res[i].nameR), field);
		field = strtok(NULL, "\n");	//last field in line
		res[i].num_system = atoi(field);
		ans = sem_init(&(res[i].s), 0, res[i].num_system);	//init semaphore
		if (ans != 0)
			printError("initial semaphore error");
		field = strtok(NULL, "\t");	//first field in next line
		i++;	//num of lines
		if (field != NULL)	//if not end of file
			res = (Resources*) realloc(res, sizeof(Resources) * (i + 1));//extend array Resources in one place

	}
	length_Resources = i;
	return res;
}
Services* scanServices(char* file) {
	int fd_read, rbytes, size = 0, i = 0, j = 0;
	Services *ser;
	char* buff;
	char temp[256];
	char* field;
	//open file to read
	fd_read = open(file, O_RDONLY);
	if (fd_read == -1)
		printError("Error open file");
	//calculate size of file
	rbytes = read(fd_read, temp, MAX);
	if (rbytes == -1)
		printError("Error read file");
	if (rbytes == 0)
		return NULL;
	while (rbytes > 0) {
		size += rbytes;
		rbytes = read(fd_read, temp, MAX);
		if (rbytes == -1)
			printError("Error read");
	}
	buff = (char*) malloc(sizeof(char) * (size + 1)); //allocation memory for scan file
	if (buff == NULL)
		printError("allocation failed");
	lseek(fd_read, 0, SEEK_SET); //pointer to start file
	//read file to buff
	rbytes = read(fd_read, buff, size);
	if (rbytes == -1)
		printError("Error read file");
	while (rbytes > 0) {
		rbytes = read(fd_read, buff, size);
		if (rbytes == -1)
			printError("Error read");
	}
	buff[size] = '\0';
	ser = (Services*) malloc(sizeof(Services)); //allocation memory for array of Services
	if (ser == NULL)
		printError("fail allocation");
	//scan fields
	field = strtok(buff, "\t");
	while (field != NULL) { //scan fields in line
		ser[i].typeS = (char*) malloc(sizeof(char) * (strlen(field) + 1)); //allocation memory for string
		if (ser[i].typeS == NULL)
			printError("fail allocation");
		strcpy((ser[i].typeS), field);
		field = strtok(NULL, "\t"); //next field
		ser[i].nameS = (char*) malloc(sizeof(char) * (strlen(field) + 1)); //allocation memory for string
		if (ser[i].nameS == NULL)
			printError("fail allocation");
		strcpy((ser[i].nameS), field);
		field = strtok(NULL, "\t"); //next field
		ser[i].num_hours = atoi(field);
		field = strtok(NULL, "\n\t"); //next field until end of line or next field
		ser[i].num_resources = atoi(field);
		if (ser[i].num_resources != 0) { //if have resources
			ser[i].list_resources = (int*) malloc(
					sizeof(int) * (ser[i].num_resources)); //allocation memory for int array
			for (j = 0; j < ser[i].num_resources; j++) {
				field = strtok(NULL, "\n\t"); //next resource
				ser[i].list_resources[j] = atoi(field);
			}
			//sort list_resources for avoiding deadlock
			bubbleSort(ser[i].list_resources, ser[i].num_resources);

		} else
			ser[i].list_resources = NULL;
		i++;
		field = strtok(NULL, "\n\t"); //first field in the next line
		if (field != NULL)
			ser = (Services*) realloc(ser, sizeof(Services) * (i + 1)); //extend array of Service in one place

	}
	length_Services = i;
	return ser;
}
void bubbleSort(int arr[], int n) {
	int i, j;
	for (i = 0; i < n - 1; i++)

		// Last i elements are already in place
		for (j = 0; j < n - i - 1; j++)
			if (arr[j] > arr[j + 1])
				swap(&arr[j], &arr[j + 1]);
}
void swap(int *xp, int *yp) {
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void printError(char *str) {
	printf("%s\n", str);
	exit(1);
}

