@echo off
mcmunit -v --config=debug strings.cmp
mcmunit -v --config=release strings.cmp
