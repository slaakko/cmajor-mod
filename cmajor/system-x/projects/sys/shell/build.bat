@echo off
scmlg --system-x --verbose shlexer.lexer
sxcmc -rv sh.cmp
sxcmc -rv -c=release sh.cmp
