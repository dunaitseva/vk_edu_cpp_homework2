#!/bin/bash

CMAKE_BINARY_DIR="build"
TMP_DIR_NAME="tmp"
TMP_FILE="$TMP_DIR_NAME/format-diff"

./scripts/build.sh -t
mkdir -p ${TMP_DIR_NAME}
touch $TMP_FILE

check-ret-code() {
  if [ $? -ne 0 ]; then
    echo "${ERROR_TTY_COLOR}Errors if file $1. Linters, that found error $2${TTY_COLOR_RESET}"
  else
    echo "${INFO_TTY_COLOR}PROCESSING $1 with $2 was success${TTY_COLOR_RESET}"
  fi
}

CPPCHECK_CHECKS="warning,style,performance,portability,unusedFunction"

INFO_TTY_COLOR=$(tput setaf 2)
ERROR_TTY_COLOR=$(tput setaf 1)
TTY_COLOR_RESET=$(tput sgr0)
DIRS=("project" "tests")
for dir in ${DIRS[*]}; do
  for file in $(find -P $dir -type f); do
    echo "${INFO_TTY_COLOR}PROCESSING $file${TTY_COLOR_RESET}"

    cpplint --filter=-legal/copyright $file
    check-ret-code $file "cpplint"
    cppcheck --enable=$CPPCHECK_CHECKS --error-exitcode=1 -I project/include --suppress=missingIncludeSystem $file
    check-ret-code $file "cppcheck"
#    infer run --compilation-database $CMAKE_BINARY_DIR/compile_commands.json
#    check-ret-code $file "infer"
#    clang-tidy $file -- -I project/include
#    check-ret-code $file "clang-tidy"

  done
done
