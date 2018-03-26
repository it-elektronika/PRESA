!#bin/bash
threshold=$(du -b "/home/pi/PRESA/scripts/touch.txt" | cut -f 1)
bool=true
rm -f /home/pi/output_fs1.txt
rm -f /home/pi/PRESA/output_fs2.txt

while $bool;do

  actualsize=$(du -b "/home/pi/PRESA/scripts/touch.txt" | cut -f 1)
  echo $actualsize
  du -b "/home/pi/PRESA/data/param_curr.txt" > output_fs1.txt
  if [ $actualsize -gt $threshold ]; then
    bool=false
  fi
done

if ! $bool; then
  pkill -f evtest
  rm -f /home/pi/PRESA/scripts/touch.txt
  rm -f /home/pi/PRESA/scripts/output.txt
  cd PRESA
  make clean
  make
  du -b "/home/pi/PRESA/data/param_curr.txt" > output_fs2.txt
  ./presa > output.txt
  #rm /home/pi/touch.txt
fi
