#!/bin/bash
REPO_URL="https://github.com/BayLak-ONE/Snake_Engine.git"
CURRENT_DIR=$(pwd)
if [ -d ".git" ]; then
    git pull origin main
else
    git init
    git remote add origin "$REPO_URL
    git fetch origi
    git reset --hard origin/main
fi
