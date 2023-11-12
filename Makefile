.PHONY: kl_vec

kl_vec:
	clang -Wall -Wextra kl_vec.c -o kl_vec
	./kl_vec
