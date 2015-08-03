#!/bin/bash
#
# VDR 3D Control Example Script
# ---------------------------
#
# Example uses irsend with remote "sony" sends 3D keycommands
#

REMOTE="Example2000"
DEVICE="/dev/lircd"
IRSEND="irsend SEND_ONCE"

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

MODES=(Disable SideBySide SideBySideLR TopBottom CheckBoard FrameSequential Converted2d)

NEW_MODE=${MODES[0]}

if [ "" != "$1" ]
  then
   [ $1 < ${#MODES[*]} ] && NEW_MODE=${MODES[$1]}
fi

case $NEW_MODE
 in
           Disable)
                   echo "3dcontrol disabled 3D Mode... "
                   $IRSEND --device=$DEVICE $REMOTE 3D-OFF
                  ;;
        SideBySide)
                   echo "3dcontrol set 3D Mode: SideBySide "
                   $IRSEND --device=$DEVICE $REMOTE 3D-SBS
                  ;;
      SideBySideLR)
                   echo "3dcontrol set 3D Mode: SideBySide LR "
                   $IRSEND --device=$DEVICE $REMOTE 3D-SBS_LR
                  ;;
         TopBottom)
                   echo "3dcontrol set 3D Mode: TopBottom "
                   $IRSEND --device=$DEVICE $REMOTE 3D-TB
                  ;;
        CheckBoard)
                   echo "3dcontrol set 3D Mode: CheckBoard "
                   $IRSEND --device=$DEVICE $REMOTE 3D-CB
                  ;;
   FrameSequential)
                   echo "3dcontrol set 3D Mode: FrameSequential "
                   $IRSEND --device=$DEVICE $REMOTE 3D-FS
                  ;;
       Converted2d)
                   echo "3dcontrol set 3D Mode: Converted2d "
                   $IRSEND --device=$DEVICE $REMOTE 2D-3D
                  ;;
esac

exit 0
