##############################################################################
# This script wrtie down the channel map file and channel position map file  #
#                                                                            #
# ./PadMap argv[1] argv[2] argv[3] argv[4]                                   #
#      argv[1]: (Input)  HardwareConnect file                                #
#      argv[2]: (Input)  Connector-Layerpad file                             #
#      argv[3]: (Output) channel map file                                    #
#      argv[4]: (Output) channel position map file                           #
##############################################################################
#
#!/bin/sh 
#./PadMap HardwareConnect_20090713.txt Connector-LayerPad.txt channelmap_test.txt ch2pos_test.txt
./PadMap HardwareConnect_20100324_test.txt Connector-LayerPad.txt channelmap_20100324_test.txt ch2pos_20100324_test.txt

