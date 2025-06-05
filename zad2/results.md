## Operating System

```
$ hostnamectl
Virtualization: wsl
Operating System: Ubuntu 24.04.2 LTS
Kernel: Linux 6.6.87.1-microsoft-standard-WSL2
Architecture: x86-64
```

## Compiler

```
$ gcc --version
gcc (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0
```

## CPU

```
$ lscpu
Architecture:             x86_64
  CPU op-mode(s):         32-bit, 64-bit
  Address sizes:          48 bits physical, 48 bits virtual
  Byte Order:             Little Endian
CPU(s):                   24
  On-line CPU(s) list:    0-23
Vendor ID:                AuthenticAMD
  Model name:             AMD Ryzen 9 7900X 12-Core Processor
    CPU family:           25
    Model:                97
    Thread(s) per core:   2
    Core(s) per socket:   12
    Socket(s):            1
    Stepping:             2
    BogoMIPS:             9399.78
    Flags:                fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36
                          clflush mmx fxsr sse sse2 ht syscall nx mmxext fxsr_opt pdpe1gb rdtscp
                          lm constant_tsc rep_good nopl tsc_reliable nonsto p_tsc cpuid extd_apicid
                          pni pclmulqdq ssse3 fma cx16 sse4_1 sse4_2 movbe popcnt aes xsave avx f16c
                          rdrand hypervisor lahf_lm cmp_legacy svm cr8_legacy abm sse4a misalignsse
                          3dnowprefetch osvw topoext perfctr_core ssbd ibrs ibpb stibp vmmcall
                          fsgsbase bmi1 avx2 smep bmi2 erms invpcid avx512f avx512dq rdseed adx smap
                          avx512ifma clflushopt clwb avx512cd sha_ni avx512bw avx512vl xsaveo pt
                          xsavec xgetbv1 xsaves avx512_bf16 clzero xsaveerptr arat npt nrip_save
                          tsc_scale vmcb_clean flushbyasid decodeassists pausefilter pfthreshold
                          v_vmsave_vmload avx512vbmi umip avx512_vbmi2 gf ni vaes vpclmulqdq
                          avx512_vnni avx512_bitalg avx512_vpopcntdq rdpid fsrm
Caches (sum of all):
  L1d:                    384 KiB (12 instances)
  L1i:                    384 KiB (12 instances)
  L2:                     12 MiB (12 instances)
  L3:                     32 MiB (1 instance)
```

## Basic Version

```
$ gcc -mfma -O2 ge1.c -o ge1
```

```
perf stat -e fp_ret_sse_avx_ops.all,task-clock ./ge1 333
Time (seconds): 2.709000e-03

 Performance counter stats for './ge1 333':

          25449643      fp_ret_sse_avx_ops.all:u         #    5.956 G/sec
              4.27 msec task-clock:u                     #    0.905 CPUs utilized

       0.004723702 seconds time elapsed

       0.003817000 seconds user
       0.000000000 seconds sys
```

```
$ perf stat -e fp_ret_sse_avx_ops.all,task-clock ./ge1 1111
Time (seconds): 1.090970e-01

 Performance counter stats for './ge1 1111':

         923479872      fp_ret_sse_avx_ops.all:u         #    7.830 G/sec
            117.94 msec task-clock:u                     #    0.974 CPUs utilized

       0.121088416 seconds time elapsed

       0.107258000 seconds user
       0.007661000 seconds sys
```

```
$ perf stat -e fp_ret_sse_avx_ops.all,task-clock ./ge1 2222
Time (seconds): 1.215120e+00

 Performance counter stats for './ge1 2222':

        7350797076      fp_ret_sse_avx_ops.all:u         #    5.853 G/sec
           1255.93 msec task-clock:u                     #    0.992 CPUs utilized

       1.265453226 seconds time elapsed

       1.228534000 seconds user
       0.027830000 seconds sys
```

```
$ perf stat -e fp_ret_sse_avx_ops.all,task-clock ./ge1 3333
Time (seconds): 6.197356e+00

 Performance counter stats for './ge1 3333':

       24767274143      fp_ret_sse_avx_ops.all:u         #    4.265 G/sec
           5806.82 msec task-clock:u                     #    0.918 CPUs utilized

       6.328828540 seconds time elapsed

       5.744047000 seconds user
       0.062395000 seconds sys
```

```
$ perf stat -e fp_ret_sse_avx_ops.all,task-clock ./ge1 6666
Time (seconds): 4.859530e+01

 Performance counter stats for './ge1 6666':

      197804889762      fp_ret_sse_avx_ops.all:u         #    4.009 G/sec
          49343.96 msec task-clock:u                     #    0.984 CPUs utilized

      50.154072319 seconds time elapsed

      49.081912000 seconds user
       0.259734000 seconds sys
```

## SIMD Version

```
$ gcc -mfma -O2 ge2.c -o ge2
```

```
$ perf stat -e fp_ret_sse_avx_ops.all,task-clock ./ge2 333
Time (seconds): 1.629000e-03

 Performance counter stats for './ge2 333':

          25449643      fp_ret_sse_avx_ops.all:u         #    8.041 G/sec
              3.17 msec task-clock:u                     #    0.884 CPUs utilized

       0.003580134 seconds time elapsed

       0.000231000 seconds user
       0.000000000 seconds sys
```

```
$ perf stat -e fp_ret_sse_avx_ops.all,task-clock ./ge2 1111
Time (seconds): 6.874200e-02

 Performance counter stats for './ge2 1111':

         923479872      fp_ret_sse_avx_ops.all:u         #   11.642 G/sec
             79.32 msec task-clock:u                     #    0.979 CPUs utilized

       0.081004815 seconds time elapsed

       0.066840000 seconds user
       0.011795000 seconds sys
```

```
$ perf stat -e fp_ret_sse_avx_ops.all,task-clock ./ge2 2222
Time (seconds): 8.545080e-01

 Performance counter stats for './ge2 2222':

        7350797076      fp_ret_sse_avx_ops.all:u         #    8.186 G/sec
            898.01 msec task-clock:u                     #    0.988 CPUs utilized

       0.908923436 seconds time elapsed

       0.865704000 seconds user
       0.031623000 seconds sys
```

```
$ perf stat -e fp_ret_sse_avx_ops.all,task-clock ./ge2 3333
Time (seconds): 4.543042e+00

 Performance counter stats for './ge2 3333':

       24767274143      fp_ret_sse_avx_ops.all:u         #    5.718 G/sec
           4331.12 msec task-clock:u                     #    0.927 CPUs utilized

       4.670240797 seconds time elapsed

       4.268446000 seconds user
       0.063098000 seconds sys
```

```
$ perf stat -e fp_ret_sse_avx_ops.all,task-clock ./ge2 6666
Time (seconds): 3.929290e+01

 Performance counter stats for './ge2 6666':

      197804889762      fp_ret_sse_avx_ops.all:u         #    4.989 G/sec
          39651.62 msec task-clock:u                     #    0.985 CPUs utilized

      40.257624223 seconds time elapsed

      39.429194000 seconds user
       0.220626000 seconds sys
```

## SIMD and BLOCKING version

```
$ gcc -mfma -O2 ge3.c -o ge3
```

```
$ perf stat -e fp_ret_sse_avx_ops.all,task-clock ./ge3 333
Time (seconds): 1.447000e-03

 Performance counter stats for './ge3 333':

          25449643      fp_ret_sse_avx_ops.all:u         #    8.875 G/sec
              2.87 msec task-clock:u                     #    0.873 CPUs utilized

       0.003285216 seconds time elapsed

       0.000087000 seconds user
       0.000000000 seconds sys
```

```
$ perf stat -e fp_ret_sse_avx_ops.all,task-clock ./ge3 1111
Time (seconds): 5.827000e-02

 Performance counter stats for './ge3 1111':

         923479872      fp_ret_sse_avx_ops.all:u         #   13.227 G/sec
             69.82 msec task-clock:u                     #    0.983 CPUs utilized

       0.071046677 seconds time elapsed

       0.067146000 seconds user
       0.000000000 seconds sys
```

```
$ perf stat -e fp_ret_sse_avx_ops.all,task-clock ./ge3 2222
Time (seconds): 4.786020e-01

 Performance counter stats for './ge3 2222':

        7350797076      fp_ret_sse_avx_ops.all:u         #   13.986 G/sec
            525.58 msec task-clock:u                     #    0.987 CPUs utilized

       0.532718018 seconds time elapsed

       0.485667000 seconds user
       0.039485000 seconds sys
```

```
$ perf stat -e fp_ret_sse_avx_ops.all,task-clock ./ge3 3333
Time (seconds): 1.851049e+00

 Performance counter stats for './ge3 3333':

       24767274143      fp_ret_sse_avx_ops.all:u         #   13.920 G/sec
           1779.26 msec task-clock:u                     #    0.908 CPUs utilized

       1.958770318 seconds time elapsed

       1.729572000 seconds user
       0.047236000 seconds sys
```

```
$ perf stat -e fp_ret_sse_avx_ops.all,task-clock ./ge3 6666
Time (seconds): 1.482420e+01

 Performance counter stats for './ge3 6666':

      197804889762      fp_ret_sse_avx_ops.all:u         #   12.712 G/sec
          15560.43 msec task-clock:u                     #    0.980 CPUs utilized

      15.883680825 seconds time elapsed

      15.259237000 seconds user
       0.297817000 seconds sys
```
