#!/usr/bin/env bash

folders=$(ls | grep -i "lab*")
echo "${folders}"

for folder in $folders; do # Not recommended, will break on whitespace
    echo $folders
      if [[ -d ${folder} ]]; then
        echo `pwd`
        echo `ls`
        command="cpplint --recursive ${folder}/*"
          echo "${command}"
          if ${command} | grep "error"; then
            echo "::error ::Review your code"
            exit 1
          fi
      fi
done
