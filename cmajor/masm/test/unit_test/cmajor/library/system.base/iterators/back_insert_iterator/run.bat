@echo off
mcmunit -v --config=debug back_insert_iterator.cmp
mcmunit -v --config=release back_insert_iterator.cmp
