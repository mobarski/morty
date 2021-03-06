## Benchmark

XG 6254 -> Intel Xeon Gold 6254 @ 3.10GHz  gcc 10.3.0  
I5 4590 -> Intel Core I5 4590 @ 3.30GHz    gcc 10.3.0  

### loops.hla n=28 for loop

| variant     | machine | times                    | mean | stdev |
| ----------- | ------- | ------------------------ | ---- | ----- |
| switch      | XG 6254 | 3714,3711,3717,3765,3715 | 3724 |    23 |
| switch -O   | XG 6254 | 3056,3075,3108,3088,3077 | 3018 |    19 |
| indirect -O | XG 6254 | 2473,2470,2472,2473,2472 | 2472 |     1 |
| indirect    | XG 6254 | 2804,2912,2912,2946,2878 | 2890 |    54 |
| switch      | I5 4590 | 4254,4215,4081,4092,4187 | 4166 |    76 |
| switch -O   | I5 4590 | 3608,3517,3434,3444,3544 | 3509 |    72 |
| indirect -O | I5 4590 | 2688,2633,2651,2634,2640 | 2649 |    73 |
| indirect    | I5 4590 | 3397,3367,3351,3278,3351 | 3349 |    44 |

n=28    -> 3080335064 VM cycles  
n=28 -O -> 2485511743 VM cycles  

### loops.hla n=28 times loop

| variant     | machine | times                    | mean | stdev |
| ----------- | ------- | ------------------------ | ---- | ----- |
| switch      | XG 6254 | 1813,1807,1831,1810,1818 | 1816 |     9 |
| switch -O   | XG 6254 | 1210,1216,1219,1211,1220 | 1215 |     5 |
| indirect -O | XG 6254 | 947,948,943,947,949      |  947 |     2 |
| indirect    | XG 6254 | 1349,1363,1338,1346,1339 | 1347 |    10 |
| switch      | I5 4590 | 2084,2068,2020,1994,2047 | 2043 |    36 |
| switch -O   | I5 4590 | 1465,1445,1421,1419,1463 | 1443 |    22 |
| indirect -O | I5 4590 | 1460,1402,1363,1359,1371 | 1391 |    42 |
| indirect    | I5 4590 | 1595,1630,1596,1619,1594 | 1607 |    17 |

n=28    -> 1499214288 VM cycles  
n=28 -O -> 1017323984 VM cycles  


### fibo.hla n=38

| variant     | machine | times                    | mean | stdev |
| ----------- | ------- | ------------------------ | ---- | ----- |
| switch      | XG 6254 | 1901,1885,1924,1897,1896 | 1901 |    14 |
| switch -O   | XG 6254 | 1752,1753,1770,1725,1780 | 1756 |    21 |
| indirect -O | XG 6254 | 1923,1919,1923,1910,1969 | 1929 |    23 |
| indirect    | XG 6254 | 2056,2032,2048,2023,2035 | 2039 |    13 |
| switch      | I5 4590 | 2076,2062,2027,2068,2040 | 2055 |    20 |
| switch -O   | I5 4590 | 1921,1849,1909,1918,1913 | 1902 |    30 |
| indirect -O | I5 4590 | 2300,2296,2289,2241,2293 | 2284 |    24 |
| indirect    | I5 4590 | 2496,2456,2430,2415,2401 | 2440 |    38 |

n=38    -> 1391411682 VM cycles  
n=38 -O -> 1264919711 VM cycles  

### array.hla n=100000

| variant     | machine | times                    | mean | stdev |
| ----------- | ------- | ------------------------ | ---- | ----- |
| switch      | XG 6254 | 2183,2181,2189,2193,2196 | 2188 |     6 |
| switch -O   | XG 6254 | 1480,1454,1465,1455,1461 | 1463 |    11 |
| indirect -O | XG 6254 | 1220,1214,1217,1216,1221 | 1218 |     3 |
| indirect    | XG 6254 | 1446,1446,1445,1446,1460 | 1449 |     6 |
| switch      | I5 4590 | 1991,2174,2009,2109,2050 | 2067 |    75 |
| switch -O   | I5 4590 | 1251,1207,1192,1191,1258 | 1220 |    32 |
| indirect -O | I5 4590 | 1101,1181,1098,1092,1152 | 1125 |    40 |
| indirect    | I5 4590 | 2156,2074,1996,1989,1997 | 2042 |    72 |

n=100k    -> 1400803027 VM cycles  
n=100k -O ->  800503026 VM cycles  

### array.hla n=100000 no iaddget, no iaddset

| variant     | machine | times                    | mean | stdev |
| ----------- | ------- | ------------------------ | ---- | ----- |
| switch -O   | I5 4590 | 1703,1687,1683,1689,1698 | 1692 |     8 |
| indirect -O | I5 4590 | 1442,1441,1442,1441,1491 | 1451 |    22 |

### sieve.hla n=5000

| variant     | machine | times                    | mean | stdev |
| ----------- | ------- | ------------------------ | ---- | ----- |
| switch      | XG 6254 | 1815,1813,1812,1813,1812 | 1813 |     1 |
| switch -O   | XG 6254 | 1203,1178,1196,1181,1180 | 1188 |    11 |
| indirect -O | XG 6254 | 1044,1063,1035,1034,1035 | 1042 |    12 |
| indirect    | XG 6254 | 1348,1347,1350,1347,1347 | 1348 |     1 |
| switch      | I5 4590 | 2056,2048,2028,2029,2115 | 2055 |    36 |
| switch -O   | I5 4590 | 1360,1355,1332,1336,1409 | 1358 |    31 |
| indirect -O | I5 4590 | 1264,1269,1260,1259,1316 | 1274 |    24 |
| indirect    | I5 4590 | 1751,1738,1746,1740,1801 | 1755 |    26 |
| python3     | I5 4590 | 8167,8153,8335,8458,8180 | 8259 |   134 |
| gcc         | I5 4590 | 422,424,419,422,421      |  422 |     2 |
| gcc -O3     | I5 4590 | 83,83,83,85,85           |   84 |     1 |


n=5k    -> 1216455018 VM cycles  
n=5k -O ->  804185018 VM cycles  

### DEBUG_FREQ

| variant    | machine | times                    | mean | stdev |
| ---------- | ------- | ------------------------ | ---- | ----- |
| debug_freq | I5 4590 | 2499,2454,2509,2386,2508 | 2471 |    53 |
| no debug   | I5 4590 | 1899,1953,1895,1934,1943 | 1925 |    26 |

