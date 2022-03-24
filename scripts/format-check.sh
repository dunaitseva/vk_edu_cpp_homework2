#!/bin/bash

TMP_DIR_NAME="tmp"
TMP_FILE="$TMP_DIR_NAME/format-diff"

mkdir -p ${TMP_DIR_NAME}
touch $TMP_FILE

INFO_TTY_COLOR=$(tput setaf 2)
ERROR_TTY_COLOR=$(tput setaf 1)
TTY_COLOR_RESET=$(tput sgr0)
DIRS=("project" "tests")
for dir in ${DIRS[*]}; do
  for file in $(find -P $dir -type f); do
    echo "${INFO_TTY_COLOR}PROCESSING $file${TTY_COLOR_RESET}"
    clang-format --style=google --sort-includes $file >$TMP_FILE
    diff $file $TMP_FILE >/dev/null
    if [ $? -ne 0 ]; then
      echo "${ERROR_TTY_COLOR}Error if file $file format (codestyle: Google)${TTY_COLOR_RESET}"
      exit 1
    fi

  done
done
