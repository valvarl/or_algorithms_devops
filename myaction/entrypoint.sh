#!/usr/bin/env bash

folders=("lab1")
for folder in "${folders[@]}"
do
  echo "${folder}"
  if [[ -d ${folder} ]]; then
    command="cpplint --recursive ${folder}/*"
    echo "${command}"
    if ${command} | grep "error"; then
      echo "::error ::Review your code"
      exit 1
    fi
  fi
done
