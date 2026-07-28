#!/bin/bash

set -euo pipefail

scripts_dir="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"
project="$(realpath "$scripts_dir/..")"
stage="$(mktemp -d)"
output="$project/cc3k.zip"

trap 'rm -rf "$stage"' EXIT

missing_newlines="$(cd "$project" && find src -name '*.cc' -type f -exec sh -c 'for file do [ -s "$file" ] && [ -n "$(tail -c 1 "$file")" ] && echo "$file"; done; exit 0' sh {} +)"

if [[ -n "$missing_newlines" ]]; then
  echo "The following files are missing a newline at the end:"
  echo "$missing_newlines"
  echo "Correcting..."
  for file in $missing_newlines; do
    echo >> "$project/$file"
  done
fi

find "$project/src" -name '*.cc' -exec cp {} "$stage" \;

for file in "$stage"/*.cc; do
  awk '
    /^#ifdef[[:space:]]+__INTELLISENSE__[[:space:]]*$/ {
      skipping = 1
      next
    }
    skipping && /^#else([[:space:]].*)?$/ {
      skipping = 0
      inside_else_branch = 1
      next
    }
    skipping && /^#endif([[:space:]].*)?$/ {
      skipping = 0
      next
    }
    inside_else_branch && /^#endif([[:space:]].*)?$/ {
      inside_else_branch = 0
      next
    }
    skipping {
      next
    }
    {
      print
    }
  ' "$file" > "$file.tmp"
  mv "$file.tmp" "$file"
done

cd "$stage"
"$project/scripts/depcrawl"

cp -R "$project/test-demos" "$stage/test-demos"

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