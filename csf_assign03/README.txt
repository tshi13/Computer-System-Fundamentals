Ian Zheng yzheng67
Taiming Shi tshi13

MS1
Ian: Makefile
Taiming: main.cpp

MS2
Ian: majority of cache functionality
Taiming: parse_line, finish up debugging

MS3
Ian: refactoring all code, adding comments
Taiming: finished FIFO, cache report

----------------------------------------------------------------------------
Cache Simulator Report

We will be using cache size total = 32,768 Bytes
Parameters we experimented with:
1. Associativity factor: direct-mapped (1 block/set), fully-associative (1 set of n blocks), 2-way, 4-way
2. Write-back + Write-allocate, write-through + no-write-allocate
3. LRU, FIFO
4. Block Size: 16 bytes, 64 bytes

So in total, we record the total_cycles for all 32 combinations and find the best cache configuration

1. direct mapped, Write-back + Write-allocate, LRU, block_size = 16
./csim 2048 1 16 write-allocate write-back lru < swim.trace
Total cycles:9191193

2. direct mapped, write-back + write-allocate, LRU, block_size = 64
./csim 512 1 64 write-allocate write-back lru < swim.trace
Total cycles:14208793

3. direct mapped, write-back + write-allocate, FIFO, block_size = 16
./csim 2048 1 16 write-allocate write-back fifo < swim.trace
Total cycles:9191193

4. direct mapped, write-back + write-allocate, FIFO, block_size = 64
./csim 512 1 64 write-allocate write-back fifo < swim.trace
Total cycles:14208793

5. direct mapped, write-through + no-write-allocate, LRU, block_size = 16
./csim 2048 1 16 no-write-allocate write-through lru < swim.trace
Total cycles:9771352

6. direct mapped, write-through + no-write-allocate, LRU, block_size = 64
./csim 512 1 64 no-write-allocate write-through lru < swim.trace
Total cycles:13284020

7. direct mapped, write-through + no-write-allocate, FIFO, block_size = 16
./csim 2048 1 16 no-write-allocate write-through fifo < swim.trace
Total cycles:9771352

8. direct mapped, write-through + no-write-allocate, FIFO, block_size = 64
./csim 512 1 64 no-write-allocate write-through fifo < swim.trace
Total cycles:13284020

9. fully-associative, Write-back + Write-allocate, LRU, block_size = 16
./csim 1 2048 16 write-allocate write-back lru < swim.trace
Total cycles:8506793

10. fully-associative, write-back + write-allocate, LRU, block_size = 64
./csim 1 512 64 write-allocate write-back lru < swim.trace
Total cycles:10331993

11. fully-associative, write-back + write-allocate, FIFO, block_size = 16
./csim 1 2048 16 write-allocate write-back fifo < swim.trace
Total cycles:8775993

12. fully-associative, write-back + write-allocate, FIFO, block_size = 64
./csim 1 512 64 write-allocate write-back fifo < swim.trace
Total cycles:10949593

13. fully-associative, write-through + no-write-allocate, LRU, block_size = 16
./csim 1 2048 16 no-write-allocate write-through lru < swim.trace
Total cycles:9539218

14. fully-associative, write-through + no-write-allocate, LRU, block_size = 64
./csim 1 512 64 no-write-allocate write-through lru < swim.trace
Total cycles:11331312

15. fully-associative, write-through + no-write-allocate, FIFO, block_size = 16
./csim 1 2048 16 no-write-allocate write-through fifo < swim.trace
Total cycles:9560808

16. fully-associative, write-through + no-write-allocate, FIFO, block_size = 64
./csim 1 512 64 no-write-allocate write-through fifo < swim.trace
Total cycles:11487849

17. 2-way, Write-back + Write-allocate, LRU, block_size = 16
./csim 1024 2 16 write-allocate write-back lru < swim.trace
Total cycles:8551193

18. 2-way, write-back + write-allocate, LRU, block_size = 64
./csim 256 2 64 write-allocate write-back lru < swim.trace
Total cycles:10525593

19. 2-way, write-back + write-allocate, FIFO, block_size = 16
./csim 1024 2 16 write-allocate write-back fifo < swim.trace
Total cycles:8836793

20. 2-way, write-back + write-allocate, FIFO, block_size = 64
./csim 256 2 64 write-allocate write-back fifo < swim.trace
Total cycles:11130393

21. 2-way, write-through + no-write-allocate, LRU, block_size = 16
./csim 1024 2 16 no-write-allocate write-through lru < swim.trace
Total cycles:9563209

22. 2-way, write-through + no-write-allocate, LRU, block_size = 64
./csim 256 2 64 no-write-allocate write-through lru < swim.trace
Total cycles:11414492

23. 2-way, write-through + no-write-allocate, FIFO, block_size = 16
./csim 1024 2 16 no-write-allocate write-through fifo < swim.trace
Total cycles:9583580

24. 2-way, write-through + no-write-allocate, FIFO, block_size = 64
./csim 256 2 64 no-write-allocate write-through fifo < swim.trace
Total cycles:11570953

25. 4-way, Write-back + Write-allocate, LRU, block_size = 16
./csim 512 4 16 write-allocate write-back lru < swim.trace
Total cycles:8521593

26. 4-way, write-back + write-allocate, LRU, block_size = 64
./csim 128 4 64 write-allocate write-back lru < swim.trace
Total cycles:10418393

27. 4-way, write-back + write-allocate, FIFO, block_size = 16
./csim 512 4 16 write-allocate write-back fifo < swim.trace
Total cycles:8796793

28. 4-way, write-back + write-allocate, FIFO, block_size = 64
./csim 128 4 64 write-allocate write-back fifo < swim.trace
Total cycles:11005593

29. 4-way, write-through + no-write-allocate, LRU, block_size = 16
./csim 512 4 16 no-write-allocate write-through lru < swim.trace
Total cycles:9550414

30. 4-way, write-through + no-write-allocate, LRU, block_size = 64
./csim 128 4 64 no-write-allocate write-through lru < swim.trace
Total cycles:11366502

31. 4-way, write-through + no-write-allocate, FIFO, block_size = 16
./csim 512 4 16 no-write-allocate write-through fifo < swim.trace
Total cycles:9565596

32. 4-way, write-through + no-write-allocate, FIFO, block_size = 64
./csim 128 4 64 no-write-allocate write-through fifo < swim.trace
Total cycles:11510204


Result:
From the experiment above, we can see that test case #9 was the best cache configuration with the lowest total cycles:

fully-associative, Write-back + Write-allocate, LRU, block_size = 16
./csim 1 2048 16 write-allocate write-back lru < swim.trace
Total cycles:8506793