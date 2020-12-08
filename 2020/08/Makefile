.PHONY: clean run

clean:
	rm -f bootcode.js

bootcode.js:
	tsc

run: clean bootcode.js
	node bootcode.js
