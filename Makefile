cluster: main.cpp
	clang++ -g $< -o $@

gen: gen.cpp
	clang++ -g $< -o $@

clean: cluster
	rm $<
