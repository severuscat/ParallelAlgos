# Qsort
## Run
```bash
cd qsort
./run.sh
```
## Results
```
START
start gen...ok!
iter 0  duration_par    4.59286sec      duration_seq    15.2583sec
check ok!
start gen...ok!
iter 1  duration_par    4.74745sec      duration_seq    15.4533sec
check ok!
start gen...ok!
iter 2  duration_par    4.9065sec       duration_seq    15.4537sec
check ok!
start gen...ok!
iter 3  duration_par    4.64551sec      duration_seq    15.2377sec
check ok!
start gen...ok!
iter 4  duration_par    4.67557sec      duration_seq    15.1919sec
check ok!


average_time_seq        15.319sec
average_time_par        4.71358sec
boost   3.24997
DONE
```
## Comment
Пробовала распараллелить partition как в par_qsort_v1.hpp и par_qsort_v2.hpp, но это не сработало.
для v1
```
yanos@yanos:~/ParallelAlgos/qsort$ ./run.sh 
START
start gen...ok!
iter 0  duration_par    40.5988sec      duration_seq    9.79753sec
check ok!
start gen...ok!
iter 1  duration_par    40.2973sec      duration_seq    9.52711sec
check ok!
start gen...ok!
iter 2  duration_par    44.1069sec      duration_seq    10.3468sec
check ok!
start gen...ok!
iter 3  duration_par    43.6164sec      duration_seq    9.87481sec
check ok!
start gen...ok!
iter 4  duration_par    43.7202sec      duration_seq    11.1361sec
check ok!


average_time_seq        10.1365sec
average_time_par        42.4679sec
boost   0.238685
DONE
```

для v2
```
yanos@yanos:~/ParallelAlgos/qsort$ ./run.sh 
START
start gen...ok!
iter 0  duration_par    34.97sec        duration_seq    10.3166sec
check ok!
start gen...ok!
iter 1  duration_par    38.3384sec      duration_seq    10.5744sec
check ok!
start gen...ok!
iter 2  duration_par    41.6573sec      duration_seq    10.4083sec
check ok!
start gen...ok!
iter 3  duration_par    42.7614sec      duration_seq    10.6659sec
check ok!
start gen...ok!
iter 4  duration_par    41.8645sec      duration_seq    11.0761sec
check ok!


average_time_seq        10.6083sec
average_time_par        39.9183sec
boost   0.265749
DONE
```