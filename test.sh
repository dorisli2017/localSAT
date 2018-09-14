timeout 5m /usr/bin/time -f "wtime=%e" ./lt05 $1 1962042455 &> $1.l1t05
timeout 5m /usr/bin/time -f "wtime=%e" ./lt05 $1 1112841915 &> $1.l2t05
timeout 5m /usr/bin/time -f "wtime=%e" ./lt05 $1 566263966 &> $1.l3t05
timeout 5m /usr/bin/time -f "wtime=%e" ./lt1 $1 1962042455 &> $1.l1t1
timeout 5m /usr/bin/time -f "wtime=%e" ./lt1 $1 1112841915 &> $1.l2t1
timeout 5m /usr/bin/time -f "wtime=%e" ./lt1 $1 566263966 &> $1.l3t1
timeout 5m /usr/bin/time -f "wtime=%e" ./lt2 $1 1962042455 &> $1.l1t2
timeout 5m /usr/bin/time -f "wtime=%e" ./lt2 $1 1112841915 &> $1.l2t2
timeout 5m /usr/bin/time -f "wtime=%e" ./lt2 $1 566263966 &> $1.l3t2
timeout 5m /usr/bin/time -f "wtime=%e" ./lt10 $1 1962042455 &> $1.l1t10
timeout 5m /usr/bin/time -f "wtime=%e" ./lt10 $1 1112841915 &> $1.l2t10
timeout 5m /usr/bin/time -f "wtime=%e" ./lt10 $1 566263966 &> $1.l3t10
#timeout 5m /usr/bin/time -f "wtime=%e" ./lnws $1 1962042455 &> $1.lnws1
#timeout 5m /usr/bin/time -f "wtime=%e" ./lnws $1 1112841915 &> $1.lnws2
#timeout 5m /usr/bin/time -f "wtime=%e" ./lnws $1 566263966 &> $1.lnws3
#timeout 5m /usr/bin/time -f "wtime=%e" ./lnebw $1 1962042455 &> $1.lnebw1
#timeout 5m /usr/bin/time -f "wtime=%e" ./lnebw $1 1112841915 &> $1.lnebw2
#timeout 5m /usr/bin/time -f "wtime=%e" ./lnebw $1 566263966 &> $1.lnebw3


