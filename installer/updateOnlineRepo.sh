#!/bin/bash

echo "	>>	git add ./repository/"
sleep 1
git add ./repository/

echo "	>>	git commit -m \"update repository\""
sleep 1
git commit -m "update repository"

echo "	>>	git push"
sleep 1
git push

# Prompt the user to press Enter
read -p "Press Enter to close the terminal"