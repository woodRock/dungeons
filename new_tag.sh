#!/bin/bash

# Version
TAG="v0.0.5"

git add -A
git commit -m "Fix (${1})"
git push
git tag -d "${TAG}"
git tag "${TAG}"
git push origin "${TAG}"
