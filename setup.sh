#!/usr/bin/env bash
#
# setup.sh — one-time initializer for repos generated from this template.
#
# Replaces the TEMPLATE / template / Template placeholders throughout the
# codebase with your project's name (in the matching case convention),
# renames any files containing "template" in their name, and then removes
# itself.
#
# Usage:
#   ./setup.sh
#   (you'll be prompted for the three name forms)
#
# Or non-interactively:
#   ./setup.sh MY_PROJECT my_project MyProject

set -euo pipefail

SCRIPT_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/$(basename "${BASH_SOURCE[0]}")"
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

is_upper_snake() { [[ "$1" =~ ^[A-Z][A-Z0-9]*(_[A-Z0-9]+)*$ ]]; }
is_lower_snake() { [[ "$1" =~ ^[a-z][a-z0-9]*(_[a-z0-9]+)*$ ]]; }
is_pascal_case() { [[ "$1" =~ ^([A-Z][a-z0-9]*)+$ ]]; }

prompt_until_valid() {
    local prompt="$1"
    local validator="$2"
    local example="$3"
    local value=""

    while true; do
        read -r -p "$prompt" value
        if "$validator" "$value"; then
            echo "$value"
            return 0
        fi
        echo "  Invalid format. Example: $example" >&2
    done
}

if [[ $# -eq 3 ]]; then
    UPPER_NAME="$1"
    LOWER_NAME="$2"
    PASCAL_NAME="$3"

    if ! is_upper_snake "$UPPER_NAME"; then
        echo "Error: '$UPPER_NAME' is not UPPER_SNAKE_CASE (e.g. MY_PROJECT)" >&2
        exit 1
    fi
    if ! is_lower_snake "$LOWER_NAME"; then
        echo "Error: '$LOWER_NAME' is not lower_snake_case (e.g. my_project)" >&2
        exit 1
    fi
    if ! is_pascal_case "$PASCAL_NAME"; then
        echo "Error: '$PASCAL_NAME' is not PascalCase (e.g. MyProject)" >&2
        exit 1
    fi
else
    echo "This will rename the TEMPLATE placeholders to your project's name."
    echo
    UPPER_NAME=$(prompt_until_valid "Project name in UPPER_SNAKE_CASE (replaces TEMPLATE): " is_upper_snake "MY_PROJECT")
    LOWER_NAME=$(prompt_until_valid "Project name in lower_snake_case (replaces template): " is_lower_snake "my_project")
    PASCAL_NAME=$(prompt_until_valid "Project name in PascalCase (replaces Template): " is_pascal_case "MyProject")
fi

echo
echo "This will replace, throughout the project:"
echo "  TEMPLATE -> $UPPER_NAME"
echo "  template -> $LOWER_NAME"
echo "  Template -> $PASCAL_NAME"
echo "and rename any files/dirs containing 'template'."
echo
read -r -p "Continue? [y/N] " CONFIRM
if [[ ! "$CONFIRM" =~ ^[Yy]$ ]]; then
    echo "Aborted."
    exit 0
fi

cd "$ROOT_DIR"

# Replace text contents first (while filenames still match what find will
# locate), skipping VCS metadata, build/, bin/ and this script itself.
FILES=$(find . \
    -path ./.git -prune -o \
    -path ./build -prune -o \
    -path ./bin -prune -o \
    -type f -not -name "$(basename "$SCRIPT_PATH")" -print)

while IFS= read -r file; do
    [[ -z "$file" ]] && continue
    if grep -qE 'TEMPLATE|template|Template' "$file" 2>/dev/null; then
        sed -i.bak \
            -e "s/TEMPLATE/${UPPER_NAME}/g" \
            -e "s/Template/${PASCAL_NAME}/g" \
            -e "s/template/${LOWER_NAME}/g" \
            "$file"
        rm -f "${file}.bak"
        echo "Updated contents: $file"
    fi
done <<< "$FILES"

# Rename files/dirs containing "template" in their name, deepest first so
# renaming a directory doesn't invalidate paths to files still inside it.
find . -path ./.git -prune -o -path ./build -prune -o -path ./bin -prune -o -depth -name '*template*' -print | while IFS= read -r path; do
    dir=$(dirname "$path")
    base=$(basename "$path")
    new_base="${base//template/${LOWER_NAME}}"
    if [[ "$base" != "$new_base" ]]; then
        mv -v "$path" "$dir/$new_base"
    fi
done

# Fill in the LICENSE placeholders using your git identity and the
# current year. Uses a control-character sed delimiter (rather than the
# default '/') so names containing '/', '&', or similar don't break the
# substitution or need per-character escaping.
if [[ -f LICENSE ]]; then
    AUTHOR_NAME="$(git config --global user.name || true)"
    if [[ -z "$AUTHOR_NAME" ]]; then
        read -r -p "git config --global user.name is not set. Enter your name for the LICENSE: " AUTHOR_NAME
    fi
    YEAR="$(date +%Y)"

    DELIM=$'\x01'

    # '&' is still special in sed's replacement text regardless of delimiter
    # (it means "whole match"), so escape it explicitly.
    AUTHOR_NAME_ESCAPED="${AUTHOR_NAME//&/\\&}"
    YEAR_ESCAPED="${YEAR//&/\\&}"

    sed -i.bak \
        -e "s${DELIM}AUTHOR_NAME${DELIM}${AUTHOR_NAME_ESCAPED}${DELIM}g" \
        -e "s${DELIM}YEAR${DELIM}${YEAR_ESCAPED}${DELIM}g" \
        LICENSE
    rm -f LICENSE.bak

    echo "Filled in LICENSE for ${AUTHOR_NAME} (${YEAR})"
fi

echo
echo "Done. Removing setup.sh..."
rm -- "$SCRIPT_PATH"

echo "Project initialized as '$PASCAL_NAME'."
