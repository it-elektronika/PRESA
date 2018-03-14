!#bin/bash
threshold=$(du -b "/home/pi/PRESA/scripts/touch.txt" | cut -f 1)
bool=true

while $bool;do

  actualsize=$(du -b "/home/pi/PRESA/scripts/touch.txt" | cut -f 1)
  echo $actualsize

  if [ $actualsize -gt $threshold ]; then
    bool=false
  fi
done

if ! $bool; then
  pkill -f evtest
  rm touch.txt
  cd PRESA
  ./presa
  #rm /home/pi/touch.txt
fi
