#!/bin/sh

# High stat high gain
p=5.0
delxfile='"Results/deltax0.dat"'
#delxfile='"Results/deltax1.dat"'
#delxfile='"Results/deltax2.dat"'
#for run in 18609 18606 18605 18601 18600 18598 18597 18595 18594 18592 18591; do
for run in 18564 18563 18562 18561 18552 18551 18550 18549 18548 18547; do
   root -l -q -b reco.C\(${run},$p,$delxfile\) >& reco-${run}.log &
done
exit 0

#--
#  Serial processing
#--
# B=1T
root -l -q -b reco.C\(18609,$p,$delxfile\)
root -l -q -b reco.C\(18606,$p,$delxfile\)
root -l -q -b reco.C\(18605,$p,$delxfile\)
root -l -q -b reco.C\(18601,$p,$delxfile\)
root -l -q -b reco.C\(18600,$p,$delxfile\)
root -l -q -b reco.C\(18598,$p,$delxfile\)
root -l -q -b reco.C\(18597,$p,$delxfile\)
root -l -q -b reco.C\(18595,$p,$delxfile\)
root -l -q -b reco.C\(18594,$p,$delxfile\)
root -l -q -b reco.C\(18592,$p,$delxfile\)
root -l -q -b reco.C\(18591,$p,$delxfile\)
exit 0

# Low stat high gain
p=5.0
delxfile='"Results/deltax0.dat"'
# B=1T
root -l -q -b reco.C\(18564,$p,$delxfile\)
root -l -q -b reco.C\(18563,$p,$delxfile\)
root -l -q -b reco.C\(18562,$p,$delxfile\)
root -l -q -b reco.C\(18561,$p,$delxfile\)
root -l -q -b reco.C\(18552,$p,$delxfile\)
root -l -q -b reco.C\(18551,$p,$delxfile\)
root -l -q -b reco.C\(18550,$p,$delxfile\)
root -l -q -b reco.C\(18549,$p,$delxfile\)
root -l -q -b reco.C\(18548,$p,$delxfile\)
root -l -q -b reco.C\(18547,$p,$delxfile\)
exit 0

p=5.0
delxfile='"Results/deltax0.dat"'
# B=1T
root -l -q -b reco.C\(18412,$p,$delxfile\)
root -l -q -b reco.C\(18413,$p,$delxfile\)
root -l -q -b reco.C\(18414,$p,$delxfile\)
root -l -q -b reco.C\(18415,$p,$delxfile\)
root -l -q -b reco.C\(18417,$p,$delxfile\)
root -l -q -b reco.C\(18418,$p,$delxfile\)
root -l -q -b reco.C\(18419,$p,$delxfile\)
root -l -q -b reco.C\(18420,$p,$delxfile\)
root -l -q -b reco.C\(18421,$p,$delxfile\)
root -l -q -b reco.C\(18423,$p,$delxfile\)
root -l -q -b reco.C\(18424,$p,$delxfile\)
exit 0

p=2.0
delxfile='"Results/deltax0.dat"'
# B=1T
root -l -q -b reco.C\(18241,$p,$delxfile\)
root -l -q -b reco.C\(18238,$p,$delxfile\)
root -l -q -b reco.C\(18218,$p,$delxfile\)
root -l -q -b reco.C\(18221,$p,$delxfile\)
root -l -q -b reco.C\(18224,$p,$delxfile\)
root -l -q -b reco.C\(18226,$p,$delxfile\)
root -l -q -b reco.C\(18228,$p,$delxfile\)
root -l -q -b reco.C\(18230,$p,$delxfile\)
root -l -q -b reco.C\(18233,$p,$delxfile\)
root -l -q -b reco.C\(18243,$p,$delxfile\)
exit 0


p=5.0
#delxfile='"Results/deltax0.dat"'
#delxfile='"Results/deltax1.bigerror.z15.dat"'
delxfile='"Results/deltax2.bigerror.z15.dat"'
#delxfile='"Results/deltax1.dat"'
#delxfile='"Results/deltax1.useedgeforstart.dat"'
#delxfile='"Results/deltax2.useedgeforstart.dat"'

# B=1T
root -l -q -b reco.C\(17319,$p,$delxfile\)
root -l -q -b reco.C\(17324,$p,$delxfile\)
root -l -q -b reco.C\(17327,$p,$delxfile\)
root -l -q -b reco.C\(17331,$p,$delxfile\)
root -l -q -b reco.C\(17340,$p,$delxfile\)
#root -l -q -b reco.C\(17344,$p,$delxfile\)
root -l -q -b reco.C\(17352,$p,$delxfile\)
root -l -q -b reco.C\(17356,$p,$delxfile\)
root -l -q -b reco.C\(17359,$p,$delxfile\)
root -l -q -b reco.C\(17364,$p,$delxfile\)
root -l -q -b reco.C\(17367,$p,$delxfile\)

exit 0

# B=1T
root -l -q -b reco.C\(7049,$p,$delxfile\)
root -l -q -b reco.C\(7046,$p,$delxfile\)
root -l -q -b reco.C\(7051,$p,$delxfile\)
root -l -q -b reco.C\(7053,$p,$delxfile\)
root -l -q -b reco.C\(7059,$p,$delxfile\)
root -l -q -b reco.C\(7061,$p,$delxfile\)
root -l -q -b reco.C\(7065,$p,$delxfile\)
root -l -q -b reco.C\(7020,$p,$delxfile\)
root -l -q -b reco.C\(7023,$p,$delxfile\)

# B=0T
root -l -q -b reco.C\(6953,$p,$delxfile\)
root -l -q -b reco.C\(6957,$p,$delxfile\)
root -l -q -b reco.C\(6958,$p,$delxfile\)
root -l -q -b reco.C\(6972,$p,$delxfile\)
root -l -q -b reco.C\(6973,$p,$delxfile\)
root -l -q -b reco.C\(6974,$p,$delxfile\)
root -l -q -b reco.C\(6975,$p,$delxfile\)
root -l -q -b reco.C\(6976,$p,$delxfile\)
root -l -q -b reco.C\(6977,$p,$delxfile\)
exit 0
