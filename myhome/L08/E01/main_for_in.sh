#!/bin/bash
if [ $# -ne 3 ]; then
  echo "Usage $0 directory function outfile"
  exit 1
fi

if [ ! -d $1 ]; then
  echo "$1 is not a directory"
  exit 1
fi

if [ ! -f $3 ]; then
  echo "$3 is not a file"
  exit 1
fi

OUTPUT="$(find $1 -type f -regextype posix-extended -regex ".*\.c$" -exec grep -H -n $2 \{} \; | tr ":" " " | sort -k1,1 -k2,2n)"

echo "$OUTPUT"

for i in words; do
  # TODO: capire come lo fa con for in
done

exit 0
