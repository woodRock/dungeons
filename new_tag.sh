#!/bin/bash

TAG="v0.0.1"

git add -A
git commit -m "Fix (${1})"
git push
git tag -d "${TAG}"
git tag "${TAG}"
git push origin "${TAG}"
