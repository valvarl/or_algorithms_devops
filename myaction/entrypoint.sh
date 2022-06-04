#!/usr/bin/env bash

folders=$(ls | grep -i "lab*")
echo "${folders}"

for folder in $folders; do # Not recommended, will break on whitespace
    echo $folders
      if [[ -d ${folder} ]]; then
        command="cpplint --recursive --exclude=*/test.cpp --filter=-legal/copyright,-build/include_subdir --linelength=120 ${folder}/*"
        echo "${command}"
        if ${command} | grep "error"; then
          echo "::error ::Review your code"
          exit 1
        fi
      fi
done
