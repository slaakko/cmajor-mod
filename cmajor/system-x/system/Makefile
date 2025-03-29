all: sys mn

clean:
	sxcmc -ev System.cms
	sxcmc -ev -c=release --opt=0 System.cms
	sxcmc -ev -c=release --opt=1 System.cms
	sxcmc -ev -c=release --opt=2 System.cms
	sxcmc -ev -c=release --opt=3 System.cms
	$(MAKE) -C Main clean

sys:
	sxcmc -vs System.cms
	sxcmc -vs -c=release --opt=0 System.cms
	sxcmc -vs -c=release --opt=1 System.cms
	sxcmc -vs -c=release --opt=2 System.cms
	sxcmc -vs -c=release --opt=3 System.cms

mn:
	$(MAKE) -C Main
