# 1.saxpy
## q1
1. the time cost of the first run is much slower(1.3x) than the later two runs whose time costs are very close.
2. the time cost of cuda version saxpy is much slower(3.2x) than the sequnential cpu-based version.

|  |  total time cost | bw | kernel time |
| :----: | :---------------: | :---------------: | :------: |
|   saxpy serial    |       80.616 ms    |      18.484 GB/s      | |
|   saxpy ispc    |       74.142 ms    |      20.098 GB/s      | |
|   saxpy task ispc    |      79.588 ms    |      18.730 GB/s     | |
|   cuda saxpy    |      236.122 ms      |      4.787 GB/s      | 5.645ms|  

## q2
no idea.

# 2. scan
## 2.1 Exclusive prefix-sum
### 2.1.1 naive implement score: 4.4 / 5
| Element Count|  Ref Time | Student Time | Score |
| :----: | :---------------: | :---------------: | :------: |
|   1000000   |       3.588    |      3.023      | 1.25|
|   10000000    |       15.501     |      18.571      | 1.25|
|   20000000    |      27.766    |      35.884     |0.96 |
|   40000000    |      54.373      |      71.639      |  0.94|  
* low performance cause: there are a number of threads in each threads block that have no work to do.  
### 2.1.2 wise implement score: 5 / 5
| Element Count|  Ref Time | Student Time | Score |
| :----: | :---------------: | :---------------: | :------: |
|   1000000   |       4.082     |      1.877      | 1.25|
|   10000000    |       14.287     |      10.349      | 1.25|
|   20000000    |      26.958     |      19.288     | 1.25 |
|   40000000    |      52.420       |      34.649     |   1.25|  
* As the computation proceeds, caculate different grid size and block size for each iteration.
### 2.1.3 Thrust library
| Element Count|  Ref Time | 
| :----: | :---------------: |
|   1000000   |       0.795     | 
|   10000000    |      1.750    |  
|   20000000    |      2.041     |   
|   40000000    |     3.024       |    
* This library is fast insanely! 
## 2.2 Find repeats
| Element Count|  Ref Time | Student Time | Score |
| :----: | :---------------: | :---------------: | :------: |
|   1000000   |        4.194     |      3.659     | 1.25|
|   10000000    |       23.162      |      15.339       | 1.25|
|   20000000    |      45.370      |      30.838    | 1.25 |
|   40000000    |      84.620        |      49.283     |   1.25|  

# 3 render
## q1