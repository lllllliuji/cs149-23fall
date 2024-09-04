# saxpy
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

# scan
naive implement score: 4.4 / 5
| Element Count|  Ref Time | Student Time | Score |
| :----: | :---------------: | :---------------: | :------: |
|   1000000   |       3.588    |      3.023      | 1.25|
|   10000000    |       15.501     |      18.571      | 1.25|
|   20000000    |      27.766    |      35.884     |0.96 |
|   40000000    |      54.373      |      71.639      |  0.94|  
low performance cause: there are a number of threads in each threads block have no work to do.
# render
## q1