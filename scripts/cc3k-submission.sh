#!/bin/bash

set -euo pipefail

scripts_dir="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"
project="$(realpath "$scripts_dir/..")"
stage="$(mktemp -d)"
output="$project/cc3k.zip"

trap 'rm -rf "$stage"' EXIT

find "$project/src" -name '*.cc' -exec cp {} "$stage" \;

cd "$stage"
"$project/scripts/depcrawl"

rm -f "$output"
zip "$output" ./*.cc order.txt syslibs.txt

echo
read -r -p "submit $output to code-cc3k? y/n " response

if [[ "$response" == "y" || "$response" == "Y" ]]; then
  /u/cs_build/bin/marmoset_submit \
    --username=-l2dohert-mw2taylo-s4suri- \
    submit CS246_PROJECT code-cc3k "$output"
else
  echo "not submitting.."
fi