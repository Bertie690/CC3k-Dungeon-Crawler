#!/bin/bash

set -euo pipefail

scripts_dir="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"
project="$(realpath "$scripts_dir/..")"
stage="$(mktemp -d)"
output="$project/cc3k.zip"

trap 'rm -rf "$stage"' EXIT

missing_newlines="$(cd "$project" && find src -name '*.cc' -type f -exec sh -c 'for file do [ -s "$file" ] && [ -n "$(tail -c 1 "$file")" ] && echo "$file"; done; exit 0' sh {} +)"

if [[ -n "$missing_newlines" ]]; then
  echo "$missing_newlines"
  exit 1
fi

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