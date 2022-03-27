#!/bin/bash

SCRIPTS_DIR="scripts"
BIN_DIR="build"
TPM_DIR="tmp"

GEN_SCRIPT="./$SCRIPTS_DIR/matrix_generator.py"
GEN_OUTPUT="./$TPM_DIR/test.matrix"

INPUT=$GEN_OUTPUT

BIN="$BIN_DIR/transpose"
BIN_OUT="./$TPM_DIR/transposed.matrix"

REF_SCRIPT="./$SCRIPTS_DIR/transpose.py"
REF_OUT="./$TPM_DIR/expected.matrix"

INFO_TTY_COLOR=$(tput setaf 5 && tput bold)
SUCCESS_TTY_COLOR=$(tput setaf 2)
ERROR_TTY_COLOR=$(tput setaf 1 && tput bold)
TTY_COLOR_RESET=$(tput sgr0)

ret_code=0
check-ret-code() {
  if [ $? -ne 0 ]; then
    ret_code=1
    echo "${ERROR_TTY_COLOR}Something went wrong. Process: ${1}${TTY_COLOR_RESET}"
  else
    echo "${SUCCESS_TTY_COLOR}Success. Process: ${1}${TTY_COLOR_RESET}"
  fi
}

mkdir -p tmp
echo "${SUCCESS_TTY_COLOR}Directory tmp prepared.${TTY_COLOR_RESET}"

echo "${INFO_TTY_COLOR}Generate matrix. Matrix size ${1} x ${2}.${TTY_COLOR_RESET}"
python3 ${GEN_SCRIPT} "$1" "$2" ${GEN_OUTPUT}
check-ret-code "Matrix generator"

echo "${INFO_TTY_COLOR}Reference transpose script processing with files input ${INPUT} and output ${REF_OUT}.${TTY_COLOR_RESET}"
python3 ${REF_SCRIPT} ${INPUT} ${REF_OUT}
check-ret-code "Reference script"

echo "${INFO_TTY_COLOR}Transpose program processing with files input ${INPUT} and output ${BIN_OUT}.${TTY_COLOR_RESET}"
./$BIN ${INPUT} ${BIN_OUT}
check-ret-code "Transpose program"

echo "${INFO_TTY_COLOR}Check difference ${BIN_OUT} and ${REF_OUT}.${TTY_COLOR_RESET}"
diff ${REF_OUT} ${BIN_OUT}
check-ret-code "Check difference"


echo "${INFO_TTY_COLOR}Delete temple files ${INPUT}, ${BIN_OUT}, ${REF_OUT}.${TTY_COLOR_RESET}"
rm ${INPUT}
rm ${BIN_OUT}
rm ${REF_OUT}
check-ret-code "Delete temple files"

exit $ret_code