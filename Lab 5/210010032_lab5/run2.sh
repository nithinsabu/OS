cp schedule.c /usr/src/minix/servers/sched/;
cp -R /Workloads /home/byte-unixbench-mod/UnixBench/workload_mix/;
cd /usr/src/;
make build MKUPDATE=yes