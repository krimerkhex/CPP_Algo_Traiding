all: clean uninstall dvi install dist

install:
	bash bash/install.sh
	mv Algorithmic_trading/Algorithmic_trading.app Algorithmic_trading.app

uninstall:
	rm -rf Algorithmic_trading.app

clean:
	bash bash/clean.sh

dist: install
	rm -rf Algorithmic_trading.tar.gz
	tar -zcf Algorithmic_trading.tar.gz Algorithmic_trading.app

dvi:
	open info/dvi.html
