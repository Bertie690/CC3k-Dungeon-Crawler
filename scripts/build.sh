#! /bin/bash

set -euo pipefail

prog_name="${1:-a.out}"

scripts_dir="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"
root_dir="$(realpath "$scripts_dir/..")"

cd "$root_dir"

"$scripts_dir/depcrawl"
"$scripts_dir/compile" "$root_dir/order.txt" "$root_dir/$prog_name"
