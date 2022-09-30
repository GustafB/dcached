# dcached
Small, portable key value store... Nothing new here.

All data is read from memory, and a WAL is used to recover state in the event of a service restart or crash.

## build
```
git clone https://github.com/gustafb/dcached
cd dcached
cmake .
make
```

## CLI interface
Allows setting, getting and deleting entries as per below. `diag` simply prints the current log path and size of the cache.
```
./dcached.out
set key value
get key
del key
diag
```

## wip
- add programmatic interface for c++
- add basic tcp interface
- daemonize client/server
- replication/consensus???
- implement LST and compaction
