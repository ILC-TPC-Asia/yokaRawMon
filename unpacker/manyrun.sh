#!/bin/sh
#--
# Loop over runs and unpack them to create root files
#--
#for run in 18609 18606 18605 18601 18600 18598 18597 18595 18594 18592 18591; do
for run in 18564 18563 18562 18561 18552 18551 18550 18549 18548 18547; do
   sh unpacker.sh rawdata/readout-${run}_0.dat >& u-${run}.log &
done
#--
# That's it!
#--
exit 0
