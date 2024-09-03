# saxpy
## q1
1. the time cost of the first run is much slower(1.3x) than the later two whose time cost is very close.
2. the time cost of cuda version saxpy is much slower(3.2x) than the sequnential cpu-based version.

|  |  total time cost | bw | kernel time |
| :----: | :---------------: | :---------------: | :------: |
|   saxpy serial    |       80.616 ms    |      18.484 GB/s      | |
|   saxpy ispc    |       74.142 ms    |      20.098 GB/s      | |
|   saxpy task ispc    |      79.588 ms    |      18.730 GB/s     | |
|   cuda saxpy    |      236.122 ms      |      4.787 GB/s      | 5.645ms|  

## q2


