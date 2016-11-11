#!/bin/sh

# High stat high gain
p=5.0
delxfile='"Results/deltax0.dat"'
#delxfile='"Results/deltax1.dat"'
#delxfile='"Results/deltax2.dat"'
#for run in 18609 18606 18605 18601 18600 18598 18597 18595 18594 18592 18591; do
#for run in 18564 18563 18562 18561 18552 18551 18550 18549 18548 18547; do
#for run in 19636 19637 19638 19639 19640 19641 19642 19643 19644 19645 19646 19647 19648 19653 19654 19655 19656 19657; do
#for run in 19667 19668 19669 19670 19671 19672 19673 19674 19675 19676 19677 19678 19680; do
#for run in 19687 19692 19693 19696 19697 19698 19699 19700 19701 19702 19703 19704; do
#for run in 19718 19719 19720 19721 19722 19723 19724 19725 19726 19727 19728 19742 19741 19740 19739 19738 19737 19736 19735 19734 19733 19732 19747 19748 19749 19750 19751 19752 19753 19754 19755 19756 19757 19758; do
#for run in 19653 19654 19655 19656 19657; do
#for run in 20034; do
#for run in 19782 19783 19784 19786 19787 19788 19789 19790 19791 19792 19793 19794; do
for run in 19791 19792 19793 19794; do
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
