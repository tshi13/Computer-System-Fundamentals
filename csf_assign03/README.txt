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

So in total, we record the total_cycles for all 32 combinations and find the best cache configuration. 
We will test with swim.trace first!

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


Intermediate Result:
From the experiment above, we can see that test case #9 was the best cache configuration with the lowest total cycles:

fully-associative, Write-back + Write-allocate, LRU, block_size = 16
./csim 1 2048 16 write-allocate write-back lru < swim.trace
Total cycles:8506793


---------------------------------------------------------------------------------------------------------------------------
Since it is expensive for us to achieve fully associative and more realistic to to have multiple sets, 
we decided to keep the parameters of this successful configuration, and only change the associativity 
factor (and also the number of blocks in each set to keep a constant cache size). 
We started from "direct mapping" spectrum and moved towards the "fully associative" spectrum：

direct mapped
./csim 2048 1 16 write-allocate write-back lru < swim.trace
Total cycles:9191193

2-way
./csim 1024 2 16 write-allocate write-back lru < swim.trace
Total cycles:8551193

4-way
./csim 512 4 16 write-allocate write-back lru < swim.trace
Total cycles:8521593

8-way
./csim 256 8 16 write-allocate write-back lru < swim.trace
Total cycles:8509993

16-way
./csim 128 16 16 write-allocate write-back lru < swim.trace
Total cycles:8507593

32-way
./csim 64 32 16 write-allocate write-back lru < swim.trace
Total cycles:8507993

64-way
./csim 32 64 16 write-allocate write-back lru < swim.trace
Total cycles:8507193

128-way
./csim 16 128 16 write-allocate write-back lru < swim.trace
Total cycles:8507193

256-way
./csim 8 256 16 write-allocate write-back lru < swim.trace
Total cycles:8507193

512-way
./csim 4 512 16 write-allocate write-back lru < swim.trace
Total cycles:8506393

1024-way
./csim 2 1024 16 write-allocate write-back lru < swim.trace
Total cycles:8506793

fully associative
./csim 1 2048 16 write-allocate write-back lru < swim.trace
Total cycles:8506793

As we're moving from direct-mapping to fully-associative, we see that the total cycle count is decreasing.
Yet this decreasing process is slowly decreasing, and we get to a diminishing point: 512-way set-associative cache
After this "diminishing point", we're seeing significantly less "returns". In fact, the total cycle has increases from
8506393 to 8506793 after this. Thus this is the sweet point where we still have good performance, while not straining the 
hardware to achieve a fully associative cache.

FINAL BEST CONFIG:
512-way, write-allocate + write-back, LRU, block size = 16bytes
./csim 4 512 16 write-allocate write-back lru < swim.trace
Total cycles:8506393


-------------------------------------------------------------------------------------------------------------
Just for confirmation, let's test everything for gcc.trace as well

1. direct mapped, Write-back + Write-allocate, LRU, block_size = 16
./csim 2048 1 16 write-allocate write-back lru < gcc.trace
Total cycles:9684083

2. direct mapped, write-back + write-allocate, LRU, block_size = 64
./csim 512 1 64 write-allocate write-back lru < gcc.trace
Total cycles:16483683

3. direct mapped, write-back + write-allocate, FIFO, block_size = 16
./csim 2048 1 16 write-allocate write-back fifo < gcc.trace
Total cycles:9684083

4. direct mapped, write-back + write-allocate, FIFO, block_size = 64
./csim 512 1 64 write-allocate write-back fifo < gcc.trace
Total cycles:16483683

5. direct mapped, write-through + no-write-allocate, LRU, block_size = 16
./csim 2048 1 16 no-write-allocate write-through lru < gcc.trace
Total cycles:23360068

6. direct mapped, write-through + no-write-allocate, LRU, block_size = 64
./csim 512 1 64 no-write-allocate write-through lru < gcc.trace
Total cycles:28118514

7. direct mapped, write-through + no-write-allocate, FIFO, block_size = 16
./csim 2048 1 16 no-write-allocate write-through fifo < gcc.trace
Total cycles:23360068

8. direct mapped, write-through + no-write-allocate, FIFO, block_size = 64
./csim 512 1 64 no-write-allocate write-through fifo < gcc.trace
Total cycles:28118514

9. fully-associative, Write-back + Write-allocate, LRU, block_size = 16
./csim 1 2048 16 write-allocate write-back lru < gcc.trace
Total cycles:8538483

10. fully-associative, write-back + write-allocate, LRU, block_size = 64
./csim 1 512 64 write-allocate write-back lru < gcc.trace
Total cycles:10838883

11. fully-associative, write-back + write-allocate, FIFO, block_size = 16
./csim 1 2048 16 write-allocate write-back fifo < gcc.trace
Total cycles:8805283

12. fully-associative, write-back + write-allocate, FIFO, block_size = 64
./csim 1 512 64 write-allocate write-back fifo < gcc.trace
Total cycles:11672483

13. fully-associative, write-through + no-write-allocate, LRU, block_size = 16
./csim 1 2048 16 no-write-allocate write-through lru < gcc.trace
Total cycles:22729562

14. fully-associative, write-through + no-write-allocate, LRU, block_size = 64
./csim 1 512 64 no-write-allocate write-through lru < gcc.trace
Total cycles:24336201

15. fully-associative, write-through + no-write-allocate, FIFO, block_size = 16
./csim 1 2048 16 no-write-allocate write-through fifo < gcc.trace
Total cycles:22830074

16. fully-associative, write-through + no-write-allocate, FIFO, block_size = 64
./csim 1 512 64 no-write-allocate write-through fifo < gcc.trace
Total cycles:24796844

17. 2-way, Write-back + Write-allocate, LRU, block_size = 16
./csim 1024 2 16 write-allocate write-back lru < gcc.trace
Total cycles:8668083

18. 2-way, write-back + write-allocate, LRU, block_size = 64
./csim 256 2 64 write-allocate write-back lru < gcc.trace
Total cycles:11786083

19. 2-way, write-back + write-allocate, FIFO, block_size = 16
./csim 1024 2 16 write-allocate write-back fifo < gcc.trace
Total cycles:8929683

20. 2-way, write-back + write-allocate, FIFO, block_size = 64
./csim 256 2 64 write-allocate write-back fifo < gcc.trace
Total cycles:12491683

21. 2-way, write-through + no-write-allocate, LRU, block_size = 16
./csim 1024 2 16 no-write-allocate write-through lru < gcc.trace
Total cycles:22795395

22. 2-way, write-through + no-write-allocate, LRU, block_size = 64
./csim 256 2 64 no-write-allocate write-through lru < gcc.trace
Total cycles:24886524

23. 2-way, write-through + no-write-allocate, FIFO, block_size = 16
./csim 1024 2 16 no-write-allocate write-through fifo < gcc.trace
Total cycles:22915040

24. 2-way, write-through + no-write-allocate, FIFO, block_size = 64
./csim 256 2 64 no-write-allocate write-through fifo < gcc.trace
Total cycles:25320820

25. 4-way, Write-back + Write-allocate, LRU, block_size = 16
./csim 512 4 16 write-allocate write-back lru < gcc.trace
Total cycles:8607683

26. 4-way, write-back + write-allocate, LRU, block_size = 64
./csim 128 4 64 write-allocate write-back lru < gcc.trace
Total cycles:11197283

27. 4-way, write-back + write-allocate, FIFO, block_size = 16
./csim 512 4 16 write-allocate write-back fifo < gcc.trace
Total cycles:8875283

28. 4-way, write-back + write-allocate, FIFO, block_size = 64
./csim 128 4 64 write-allocate write-back fifo < gcc.trace
Total cycles:12026083

29. 4-way, write-through + no-write-allocate, LRU, block_size = 16
./csim 512 4 16 no-write-allocate write-through lru < gcc.trace
Total cycles:22754658

30. 4-way, write-through + no-write-allocate, LRU, block_size = 64
./csim 128 4 64 no-write-allocate write-through lru < gcc.trace
Total cycles:24510362

31. 4-way, write-through + no-write-allocate, FIFO, block_size = 16
./csim 512 4 16 no-write-allocate write-through fifo < gcc.trace
Total cycles:22878289

32. 4-way, write-through + no-write-allocate, FIFO, block_size = 64
./csim 128 4 64 no-write-allocate write-through fifo < gcc.trace
Total cycles:24969370


As we can see, we have the same results for gcc.trace:
test #9 had the lowest total cycle count as well

fully-associative, Write-back + Write-allocate, LRU, block_size = 16
./csim 1 2048 16 write-allocate write-back lru < gcc.trace
Total cycles:8538483

Modifying this configuration to have a 512 sets:

512-way, write-allocate + write-back, LRU, block size = 16bytes
./csim 4 512 16 write-allocate write-back lru < gcc.trace
Total cycles:8547283

Gives us a really good total cycle count again, which is just slightly higher than its fully-associative counterpart,
but at much more efficiency.


---------------------------------------------------------------------------------------------

CONCLUSION:

By experimenting with all 32 combinations for both swim.trace and gcc.trace, we found the best configuration, 
which was a fully-associative cache. To promote efficiency, we were able to modify the associativity factor to
strike a balance between performance and efficiency.

Our final configuration is 
512-way, write-allocate + write-back, LRU, block size = 16bytes
