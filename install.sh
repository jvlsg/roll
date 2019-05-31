#!/bin/bash
INSTALL_PATH='/usr/local/bin/roll'
if [ -f $INSTALL_PATH ]; then
    echo "[*]Alerady Installed"
    exit    
fi


echo "[*] Installing roll in $INSTALL_PATH"
make roll

SUDO=''
if [ "$EUID" -ne 0 ]
  then echo "[*] Requiring sudo to move Binary"
  SUDO='sudo'
fi
$SUDO mv roll.o $INSTALL_PATH 
echo "[*] ...Done"
