all: debug release

clean:
	cd dbg && del_main.bat
	cd rel && del_main.bat

debug:
	cd dbg && sxas -v Main.s

release:
	cd rel && sxas -v Main.s
