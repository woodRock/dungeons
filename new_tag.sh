#!/bin/bash

TAG="v0.1.1"

git add -A
git commit -m "Fix ${0}"
git push
git tag -d "${TAG}"
git tag "${TAG}"
git push origin "${TAG}"
