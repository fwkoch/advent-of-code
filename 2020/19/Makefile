.PHONY: clean run

clean:
	rm -f rules.js

rules.js:
	tsc

run: clean rules.js
	node rules.js
