loadI  128          => r0
load   r0           => r1
loadI  132          => r2
load   r2           => r3
loadI  136          => r4
load   r4           => r5
mult   r3   , r5    => r3
add    r1   , r3    => r1
store  r1           => r0
output 128               
