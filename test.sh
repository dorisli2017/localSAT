timeout 5m /usr/bin/time -f "wtime=%e" ./lt $1 1962042455 &> $1.l1t
timeout 5m /usr/bin/time -f "wtime=%e" ./lt $1 1112841915 &> $1.l2t
timeout 5m /usr/bin/time -f "wtime=%e" ./lt $1 566263966 &> $1.l3t
#timeout 5m /usr/bin/time -f "wtime=%e" ./lrb $1 1962042455 &> $1.l1rb
#timeout 5m /usr/bin/time -f "wtime=%e" ./lrb $1 1112841915 &> $1.l2rb
#timeout 5m /usr/bin/time -f "wtime=%e" ./lrb $1 566263966 &> $1.l3rb
#timeout 5m /usr/bin/time -f "wtime=%e" ./lrb $1 1962042455 &> $1.lrb1
#timeout 5m /usr/bin/time -f "wtime=%e" ./lrb $1 1112841915 &> $1.lrb2
#timeout 5m /usr/bin/time -f "wtime=%e" ./lrb $1 566263966 &> $1.lrb3
#timeout 5m /usr/bin/time -f "wtime=%e" ./lnp $1 1962042455 &> $1.lnp1
#timeout 5m /usr/bin/time -f "wtime=%e" ./lnp $1 1112841915 &> $1.lnp2
#timeout 5m /usr/bin/time -f "wtime=%e" ./lnp $1 566263966 &> $1.lrnp3
#timeout 5m /usr/bin/time -f "wtime=%e" ./lnws $1 1962042455 &> $1.lnws1
#timeout 5m /usr/bin/time -f "wtime=%e" ./lnws $1 1112841915 &> $1.lnws2
#timeout 5m /usr/bin/time -f "wtime=%e" ./lnws $1 566263966 &> $1.lnws3
#timeout 5m /usr/bin/time -f "wtime=%e" ./lnebw $1 1962042455 &> $1.lnebw1
#timeout 5m /usr/bin/time -f "wtime=%e" ./lnebw $1 1112841915 &> $1.lnebw2
#timeout 5m /usr/bin/time -f "wtime=%e" ./lnebw $1 566263966 &> $1.lnebw3


