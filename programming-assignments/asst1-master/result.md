# prog1
Block  (not linear)
| numThreads | mandelbrot serial | mandelbrot thread | speed up |
| :----: | :---------------: | :---------------: | :------: |
|   2    |      323.372      |      163.498      |  1.98x   |
|   3    |      322.705      |      196.574      |  1.64x   |
|   4    |      321.038      |      132.100      |  2.43x   |
|   5    |      321.739      |      131.379      |  2.45x   |
|   6    |      322.555      |      100.893      |  3.20x   |
|   7    |      321.006      |      95.217       |  3.39x   |
|   8    |      325.123      |      80.361       |  4.05x   |
|   16   |      319.649      |      45.038       |  7.10x   |
|   32   |      326.308      |      35.416       |  9.21x   |

Interleave  (almost linear)
| numThreads | mandelbrot serial | mandelbrot thread | speed up |
| :----: | :---------------: | :---------------: | :------: |
|   2    |      321.035      |      164.865      |  1.95x   |
|   3    |      321.003      |      109.166      |  2.94x   |
|   4    |      321.816      |      82.730      |  3.89x   |
|   5    |      320.824      |      67.859      |  4.73x   |
|   6    |      323.270      |      57.016      |  5.67x   |
|   7    |      319.722      |      50.024       |  6.39x   |
|   8    |      320.599      |      44.277       |  7.24x   |
|   16   |      320.369      |      25.816       |  12.41x   |
|   32   |      320.468      |      25.569       |  12.53x |



# prog 2
statistics in myexp -s 10000
| vector width | total vector instructions | vector utilization(%) | utilized vector lanes | total vector lanes |
| :----: | :---------------: | :---------------: | :------: | :------: |
|   2    |      162515      |      87.6      |  248622   | 325030 |
|   4    |      94571      |     82.3      |  311252   | 378284 |
|   8    |      51627      |      79.6      |  328624   | 413016 |
|   16    |      26967     |      78.3      |  337864   | 431472 |


When vector width increase, the vector utilization decrease. Because the wider vector width, the larger possibility of divergency, the less utilization. 


# prog3
part1  
Expected speedup: 8x  
Actual speedup: 5.85x  
Reason: Divergence between each lane. Ispc implement: interleaved assignment, block assignment, dynamic assignment(not sure).  

part2  
| numTasks |  serial | ispc | ispc task |
| :----: | :---------------: | :---------------: | :------: |
|   2    |      157.708      |      28.087 (5.61x)      |  14.042 (11.23x)   |
|   4    |      157.534    |      27.840 (5.66x)      |  11.225 (14.03x)  |
|   8    |      158.803      |      27.902 (5.69x)      |  7.124 (22.29x)   |
|   16    |      158.116      |      28.075 (5.63x)     |  4.577 (34.55x)  |
|   32    |      157.204      |      28.073 (5.60x)     |  2.616 (60.08x)  |


The best count of task is determined by the count of vector register(Not sure).  

