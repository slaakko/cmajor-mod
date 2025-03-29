all: sys progs

clean:
	$(MAKE) -C system clean
	$(MAKE) -C projects\sysprogs clean

sys:
	$(MAKE) -C system

progs:
	$(MAKE) -C projects\sysprogs
	$(MAKE) -C projects\sysprogs install
