# part a
## original performance
### 1. test result on super_super_light
| |  student | reference | perf |
| :----: | :---------------: | :---------------: | :------: |
|   serial    |      6.828     |      9.781      |  0.70(ok)   |
|   parallel + always spawn    |      6.706    |      136.878      |  0.05(ok) |
|   parallel + thread pool + spin    |     6.612      |      24.548      |  0.27(ok)   |
|   parallel + thread pool + sleep    |      6.73      |      62.79     |  0.11(ok)  |

### 2. test result on super_light
| |  student | reference | perf |
| :----: | :---------------: | :---------------: | :------: |
|   serial    |      52.355     |      68.558      |  0.76(ok)   |
|   parallel + always spawn    |      52.6    |      130.918     |  0.40(ok) |
|   parallel + thread pool + spin    |     52.517      |      25.469      |  2.06  (not ok)   |
|   parallel + thread pool + sleep    |      52.72     |      63.71     |  0.83(ok)  |

## naive implement performance
### 1. test result on super_super_light
| |  student | reference | perf |
| :----: | :---------------: | :---------------: | :------: |
|   serial    |     6.658   |  9.801   |    0.68  (OK)   |
|   parallel + always spawn    |      124.943  | 136.389  |   0.92  (OK) |
|   parallel + thread pool + spin    |    72.897  |  21.258   |   3.43  (NOT OK)   |
|   parallel + thread pool + sleep    |      600.207  | 62.69   |    9.57  (NOT OK)  |

### 2. test result on super_light
| |  student | reference | perf |
| :----: | :---------------: | :---------------: | :------: |
|   serial    |      65.634  |  68.837   |   0.95  (OK)   |
|   parallel + always spawn    |      125.036  | 129.037   |  0.97  (OK) |
|   parallel + thread pool + spin    |    57.904  |  26.998   |   2.14  (NOT OK)   |
|   parallel + thread pool + sleep    |      600.232  | 62.818   |   9.56  (NOT OK)  |