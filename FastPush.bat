@echo off

set /p id="Enter Commit Name: "

git add  .
git commit -m"%id%".
git push
