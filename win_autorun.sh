#!/bin/bash
# *
# *       _ _       _
# *  _ __| | | _ __/ |
# * | '_ \_  _| '_ \ |
# * | .__/ |_|| .__/_|
# * |_|       |_|
# ***********************
# ./win_Autorun [USB NAME]

E_XCD=85
E_NOTROOT=87
HOSTNAME=`id -un`

if [[ "$UID" -ne "$ROOT_UID" ]]; then

	echo "Must be run as root!"
	exit $E_NOTROOT

elif [ "$1" = "-h" ]; then

	echo "*       _ _       _"
        echo "*  _ __| | | _ __/ |"
        echo "* | '_ \\_  _| '_\\  |"
        echo "* | .__/ |_|| .__/_|"
        echo "* |_|       |_|"
        echo "*********help**********"
	echo "Usage:"
	echo "	./win_Autorun [USB PATH]"

else

	echo "*       _ _       _"
        echo "*  _ __| | | _ __/ |"
        echo "* | '_ \\_  _| '_\\  |"
        echo "* | .__/ |_|| .__/_|"
        echo "* |_|       |_|"
        echo "*********autorun********"
	echo "[*] Enter USB full Path"
	read PATH_USB
	echo "[*] Changing directory."
	cd $PATH_USB

	if [ `pwd` != "$PATH_USB" ]; then
		echo "Cant change to $PATH_USB"
		exit $E_XCD
	fi

	echo "[*] enter autorun executable file name."
	read AUTORUN_FILE
	echo "[autorun]" > autorun.inf
	echo "open=$AUTORUN_FILE" >> autorun.inf
	echo "[*] IS it a win 7 (yes / no)"
	read win7

	if [ $win7 = "yes" ]; then
		echo "action=Start ShellRun-CD" >> autorun.inf
	fi
fi

exit
