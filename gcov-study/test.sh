#!/bin/bash


distro = ./*.html

for entry in ./*.html
do
  echo "$entry"
done

echo "${#distro[@]}"
