# Vector_Manipulation-MPI

## Build
```
$ make 
```
## Run

```
$ mpirun --host localhost:<number_of_process> vector_mpi <vector_size>
```
Exemple: 

```
$ mpirun --host localhost:4 vector_mpi 20
```

## Remove executable
```
$ make clean
```