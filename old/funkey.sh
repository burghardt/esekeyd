#!/bin/sh

# Setup some basic environment...

export USER="einstein"
export USERNAME=${USER}
export HOME=/home/${USERNAME}
export PATH=${HOME}/bin:/usr/local/bin:/usr/bin:/bin:/usr/X11R6/bin
export DISPLAY=:0.0

# Include my BASH startup scripts...
. ${HOME}/.bash_profile

# Global section...
case $1 in
129)
  sudo /sbin/init 3
  ;;
130)
  sudo /sbin/init 4
  ;;
145)
  aumix -v -5
  ;;
146)
  aumix -v +5
  ;;
147)
  aumix -v 0
  ;;
149)
  eject
  ;;
esac	

# Check if user is logged in... if not exit...

users | grep einstein > /dev/null

if [ $? -ne 0 ]; then
    exit 0
fi

# User related commands...

case $1 in
131)
  xman &
  ;;
132)
  xterm -bg black -fg green3 -cr yellowgreen -hc seagreen -ms black -bdc +cm +dc +pc -rvc -ulc -T "xTerm" -e "/bin/bash --login" &
  ;;
133)
  netscape &
  ;;
134)
  xterm -e mutt &
  ;;
135)
  xmms -r
  ;;
136)
  xmms -s
  ;;
137)
  xmms -f
  ;;
144)
  xmms -t
  ;;
148)
  xmms &
  ;;
150)
  xcalc &
  ;;
151)
  xset dpms force standby
  ;;
*)
  echo "funkey.sh: no binding for $1" >> /tmp/funlog
esac

# Exit cleanly... even on fatal error...

exit 0
