### Branch counters

Report the cycles for taken, predicted, and mispredicted branches and provide a short 
explanation for the results you see:

```
branchalways
"PAPI_BR_CN":"1000014053",
"PAPI_BR_MSP":"11381",
"PAPI_BR_PRC":"1000013976"


branchhalf
"PAPI_BR_CN":"1000013814",
PAPI_BR_MSP":"11663",
"PAPI_BR_PRC":"1000013723"

Explanation: The branchalways is more correctly than the branchhalf. pseudo_random() returns true mostly, while pseudo_random() % 128 < 64 is more likely to return false.
```
### Impact of branch counters

Report the cycles per iteration for both predicted and 50% predicted code. Report the numbers of issued and committed instructions. 

```
branchalways
cycles per iteration:"9.811948544",
"PAPI_TOT_IIS":"18000374433",
PAPI_TOT_INS":"18000004488"


branchhalf
cycles per iteration:"10.024225426",
"PAPI_TOT_IIS":"19000470847",
"PAPI_TOT_INS":"19000004107"

Explanation: branchalways: "x++" will executed almost every time; branchhalf: only half of the "x++" will be executed

```


### Bandwidth of sequential test 

Compute bandwidth, i.e., MB/s for the sequential test, explain your answer. 


```
Bandwidth: 128 * 1024 * 1024 * 64 / (1024 * 1024) / 8 / 23 = 1024 / 23 = 44.5 MB/S

Explanation Sequential test almost all memory accesses hit L2, while in the random test we see 137M L2 misses.   
Nevertheless, 23 cycles per one iteration of the loop seems a bit high for a workload that hits L2.  
The reason is that the system bottlenecks on the throughput of the memory controller (i.e., the fastest it can ship cache lines to the core are at a speed of one cache line per 23 cycles).
            

```

### TLB misses for both tests

Report the number of TLB misses in sequential and random tests and provide a short explanation for the numbers

```
TLB misses sequential: "27054"
TLB misses random: "133885897"

Explanation: data in memory will be prefectched, so sequential has less TLB misses.
```

### Performance of the hash table

Perform an experiment to compute the number of cycles needed to compute the hash function, report your 
findings and explain. 

```
Cycles for key_hash: "578045"

Explanation <text>

```

Perform a simple performance analysis for the lookup function, explain what you see. 

```
Cycles per lookup (fits into L3): "204.830"
Cycles per lookup (does not fit into L3): X

Explanation <text>
```

