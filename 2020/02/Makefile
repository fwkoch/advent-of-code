.PHONY: clean run

clean:
	rm countPasswords.js

countPasswords.js:
	tsc countPasswords.ts

run: clean countPasswords.js
	node countPasswords.js
