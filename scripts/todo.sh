#!/usr/bin/env sh

grep "TODO:" -nR src include |
    (echo "File:Line: Message" && cut -d : -f1,2,4 ) |
    sed 's/\*\/$//' |
    column -t -s :
