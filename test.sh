timeout 2m /usr/bin/time -f "wtime=%e" ./fl $1 &> $1.fl
timeout 2m /usr/bin/time -f "wtime=%e" ./tl $1 &> $1.tl

