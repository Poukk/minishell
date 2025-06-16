#!/bin/sh

rm -rf .git/hooks/*
cp commit-msg ../.git/hooks/
chmod +x ../.git/hooks/commit-msg
