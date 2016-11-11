#!/bin/sh
# --events      make a root file
# --pedestal    make pedestal files
# --reversed    for new format
# --format      0 : For the data before July 2009
#               1 : For the data after July 2009
# --infile      file to use
# --mapfile     map file
#
# $Id: unpacker.sh,v 1.6 2011/01/30 18:09:35 fujiik Exp $
#
# To create event by event root files
#./unpacker --events --reversed --mapfile=map/channelmap_090228.txt --infile $1
#./unpacker --events --reversed --mapfile=map/channelmap_090304.txt --infile $1
#./unpacker --events --reversed --mapfile=map/channelmap_090305.txt --infile $1
#./unpacker --events --reversed --mapfile=map/channelmap_090307.txt --infile $1
#./unpacker --events --reversed --mapfile=map/channelmap_090324.txt --infile $1
#./unpacker --events --reversed --format 0 --mapfile=map/channelmap_090324.txt --infile $1
#./unpacker --events --reversed --format 1 --mapfile=map/channelmap_20090713.txt --infile $1
#./unpacker --events --reversed --format 1 --mapfile=map/channelmap_20100912.txt --infile $1
#./unpacker --events --reversed --format 1 --mapfile=map/channelmap_20121207.txt --infile $1
#./unpacker --events --reversed --format 1 --mapfile=map/ChannelMap_20161030.txt --infile $1
./unpacker --events --reversed --format 1 --mapfile=map/ChannelMap_20161109.txt --infile $1
