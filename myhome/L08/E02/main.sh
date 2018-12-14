#!/bin/bash

while read line; do
  echo "$line" | wc
done < $1

exit 0
