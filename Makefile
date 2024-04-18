final:
	gcc -o program.exe src/main.c src/account.c src/iban_generator.c src/login.c src/read_data.c src/temp_file.c src/transactions.c -Iinclude
