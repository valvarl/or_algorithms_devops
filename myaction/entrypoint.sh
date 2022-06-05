#!/usr/bin/env bash

folders=$(ls | grep -i "lab*")
echo "${folders}"

for folder in $folders; do # Not recommended, will break on whitespace
    echo "${folder}"
      if [[ -d ${folder} ]]; then
        command="cpplint --recursive --linelength=120 --exclude=*/test.cpp --filter=-legal/copyright,-build/include_subdir,-runtime/references,-runtime/threadsafe_fn ${folder}/*"
          echo "${command}"
          if ${command} | grep "error"; then
            echo "::error ::Review your code"
            exit 1
          fi
      fi
done
