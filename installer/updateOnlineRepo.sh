#!/bin/bash

echo "\n	>>	git add ./repository/\n"
sleep 1
git add ./repository/

echo "\n	>>	git commit -m \"update repository\"\n"
sleep 1
git commit -m "update repository"

echo "\n	>>	git push\n"
sleep 1
git push

# Prompt the user to press Enter
read -p "Press Enter to close the terminal"