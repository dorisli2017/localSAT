timeout 2m /usr/bin/time -f "wtime=%e" ./f00 $1 &> $1.f00
timeout 2m /usr/bin/time -f "wtime=%e" ./t00 $1 &> $1.t00
timeout 2m /usr/bin/time -f "wtime=%e" ./t01 $1 &> $1.t01
timeout 2m /usr/bin/time -f "wtime=%e" ./f01 $1 &> $1.f01
timeout 2m /usr/bin/time -f "wtime=%e" ./f11 $1 &> $1.f11
timeout 2m /usr/bin/time -f "wtime=%e" ./f10 $1 &> $1.f10
timeout 2m /usr/bin/time -f "wtime=%e" ./t10 $1 &> $1.t10
timeout 2m /usr/bin/time -f "wtime=%e" ./t11 $1 &> $1.t11
timeout 2m /usr/bin/time -f "wtime=%e" ./t21 $1 &> $1.t21
timeout 2m /usr/bin/time -f "wtime=%e" ./t20 $1 &> $1.t20
timeout 2m /usr/bin/time -f "wtime=%e" ./f20 $1 &> $1.f20
timeout 2m /usr/bin/time -f "wtime=%e" ./f21 $1 &> $1.f21


