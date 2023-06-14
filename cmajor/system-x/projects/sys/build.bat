@echo off
sxcmc -rv coreutil\coreutil.cms
sxcmc -rv -c=release coreutil\coreutil.cms
sxcmc -rv editor\editor.cms
sxcmc -rv -c=release editor\editor.cms
sxcmc -rv shell\sh.cmp
sxcmc -rv -c=release shell\sh.cmp
sxcmc -rv tab\tab.cmp
sxcmc -rv -c=release tab\tab.cmp
sxcmc -rv sx\sx.cmp
sxcmc -rv -c=release sx\sx.cmp
