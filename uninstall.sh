#!/bin/bash
INSTALL_PATH='/usr/local/bin/roll'
if [ ! -f $INSTALL_PATH ]; then
    echo "[*]Not Installed"
    exit    
fi

echo "[*] Uninstalling roll"

SUDO=''
if [ "$EUID" -ne 0 ]
  then echo "[*] Requiring sudo to remove Binary"
  SUDO='sudo'
fi
$SUDO rm $INSTALL_PATH 
echo "[*] ...Done"
