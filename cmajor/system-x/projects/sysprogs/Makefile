all: cu ed sh tb

clean:
	sxcmc -ve coreutil\coreutil.cms
	sxcmc -ve -c=release --opt=0 coreutil\coreutil.cms
	sxcmc -ve -c=release --opt=1 coreutil\coreutil.cms
	sxcmc -ve -c=release --opt=2 coreutil\coreutil.cms
	sxcmc -ve -c=release --opt=3 coreutil\coreutil.cms
	sxcmc -ve editor/editor.cms
	sxcmc -ve -c=release --opt=0 editor\editor.cms
	sxcmc -ve -c=release --opt=1 editor\editor.cms
	sxcmc -ve -c=release --opt=2 editor\editor.cms
	sxcmc -ve -c=release --opt=3 editor\editor.cms
	sxcmc -ve shell/sh.cmp
	sxcmc -ve -c=release --opt=0 shell\sh.cmp
	sxcmc -ve -c=release --opt=1 shell\sh.cmp
	sxcmc -ve -c=release --opt=2 shell\sh.cmp
	sxcmc -ve -c=release --opt=3 shell\sh.cmp
	sxcmc -ve tab/tab.cmp
	sxcmc -ve -c=release --opt=0 tab\tab.cmp
	sxcmc -ve -c=release --opt=1 tab\tab.cmp
	sxcmc -ve -c=release --opt=2 tab\tab.cmp
	sxcmc -ve -c=release --opt=3 tab\tab.cmp

install:
	makebindir.bat
	xcopy /Y /-I /F coreutil\cat\bin\debug\cat.x ..\..\bin\catd.x
	xcopy /Y /-I /F coreutil\cat\bin\release\2\cat.x ..\..\bin\cat.x
	xcopy /Y /-I /F coreutil\chmod\bin\debug\chmod.x ..\..\bin\chmodd.x
	xcopy /Y /-I /F coreutil\chmod\bin\release\2\chmod.x ..\..\bin\chmod.x
	xcopy /Y /-I /F coreutil\cp\bin\debug\cp.x ..\..\bin\cpd.x
	xcopy /Y /-I /F coreutil\cp\bin\release\2\cp.x ..\..\bin\cp.x
	xcopy /Y /-I /F coreutil\echo\bin\debug\echo.x ..\..\bin\echod.x
	xcopy /Y /-I /F coreutil\echo\bin\release\2\echo.x ..\..\bin\echo.x
	xcopy /Y /-I /F coreutil\head\bin\debug\head.x ..\..\bin\headd.x
	xcopy /Y /-I /F coreutil\head\bin\release\2\head.x ..\..\bin\head.x
	xcopy /Y /-I /F coreutil\hexdump\bin\debug\hexdump.x ..\..\bin\hexdumpd.x
	xcopy /Y /-I /F coreutil\hexdump\bin\release\2\hexdump.x ..\..\bin\hexdump.x
	xcopy /Y /-I /F coreutil\kill\bin\debug\kill.x ..\..\bin\killd.x
	xcopy /Y /-I /F coreutil\kill\bin\release\2\kill.x ..\..\bin\kill.x
	xcopy /Y /-I /F coreutil\less\bin\debug\less.x ..\..\bin\lessd.x
	xcopy /Y /-I /F coreutil\less\bin\release\2\less.x ..\..\bin\less.x
	xcopy /Y /-I /F coreutil\login\bin\debug\login.x ..\..\bin\logind.x
	xcopy /Y /-I /F coreutil\login\bin\release\2\login.x ..\..\bin\login.x
	xcopy /Y /-I /F coreutil\ls\bin\debug\ls.x ..\..\bin\lsd.x
	xcopy /Y /-I /F coreutil\ls\bin\release\2\ls.x ..\..\bin\ls.x
	xcopy /Y /-I /F coreutil\mkdir\bin\debug\mkdir.x ..\..\bin\mkdird.x
	xcopy /Y /-I /F coreutil\mkdir\bin\release\2\mkdir.x ..\..\bin\mkdir.x
	xcopy /Y /-I /F coreutil\mv\bin\debug\mv.x ..\..\bin\mvd.x
	xcopy /Y /-I /F coreutil\mv\bin\release\2\mv.x ..\..\bin\mv.x
	xcopy /Y /-I /F coreutil\passwd\bin\debug\passwd.x ..\..\bin\passwdd.x
	xcopy /Y /-I /F coreutil\passwd\bin\release\2\passwd.x ..\..\bin\passwd.x
	xcopy /Y /-I /F coreutil\rm\bin\debug\rm.x ..\..\bin\rmd.x
	xcopy /Y /-I /F coreutil\rm\bin\release\2\rm.x ..\..\bin\rm.x
	xcopy /Y /-I /F coreutil\rmdir\bin\debug\rmdir.x ..\..\bin\rmdird.x
	xcopy /Y /-I /F coreutil\rmdir\bin\release\2\rmdir.x ..\..\bin\rmdir.x
	xcopy /Y /-I /F coreutil\sudo\bin\debug\sudo.x ..\..\bin\sudod.x
	xcopy /Y /-I /F coreutil\sudo\bin\release\2\sudo.x ..\..\bin\sudo.x
	xcopy /Y /-I /F coreutil\time\bin\debug\time.x ..\..\bin\timed.x
	xcopy /Y /-I /F coreutil\time\bin\release\2\time.x ..\..\bin\time.x
	xcopy /Y /-I /F coreutil\tail\bin\debug\tail.x ..\..\bin\taild.x
	xcopy /Y /-I /F coreutil\tail\bin\release\2\tail.x ..\..\bin\tail.x
	xcopy /Y /-I /F coreutil\whoami\bin\debug\whoami.x ..\..\bin\whoamid.x
	xcopy /Y /-I /F coreutil\whoami\bin\release\2\whoami.x ..\..\bin\whoami.x
	xcopy /Y /-I /F editor\bin\debug\e.x ..\..\bin\ed.x
	xcopy /Y /-I /F editor\bin\release\2\e.x ..\..\bin\e.x
	xcopy /Y /-I /F shell\bin\debug\sh.x ..\..\bin\shd.x
	xcopy /Y /-I /F shell\bin\release\2\sh.x ..\..\bin\sh.x
	xcopy /Y /-I /F tab\bin\debug\tab.x ..\..\bin\tabd.x
	xcopy /Y /-I /F tab\bin\release\2\tab.x ..\..\bin\tab.x

cu:
	sxcmc -v coreutil\coreutil.cms
	sxcmc -v -c=release --opt=0 coreutil\coreutil.cms
	sxcmc -v -c=release --opt=1 coreutil\coreutil.cms
	sxcmc -v -c=release --opt=2 coreutil\coreutil.cms
	sxcmc -v -c=release --opt=3 coreutil\coreutil.cms

ed:
	sxcmc -v editor\editor.cms
	sxcmc -v -c=release --opt=0 editor\editor.cms
	sxcmc -v -c=release --opt=1 editor\editor.cms
	sxcmc -v -c=release --opt=2 editor\editor.cms
	sxcmc -v -c=release --opt=3 editor\editor.cms

sh:
	sxcmc -v shell\sh.cmp
	sxcmc -v -c=release --opt=0 shell\sh.cmp
	sxcmc -v -c=release --opt=1 shell\sh.cmp
	sxcmc -v -c=release --opt=2 shell\sh.cmp
	sxcmc -v -c=release --opt=3 shell\sh.cmp
	
tb:
	sxcmc -v tab/tab.cmp
	sxcmc -v -c=release --opt=0 tab\tab.cmp
	sxcmc -v -c=release --opt=1 tab\tab.cmp
	sxcmc -v -c=release --opt=2 tab\tab.cmp
	sxcmc -v -c=release --opt=3 tab\tab.cmp
	
s:
	sxcmc -v sx\sx.cmp
	sxcmc -v -c=release --opt=0 sx\sx.cmp
	sxcmc -v -c=release --opt=1 sx\sx.cmp
	sxcmc -v -c=release --opt=2 sx\sx.cmp
	sxcmc -v -c=release --opt=3 sx\sx.cmp
