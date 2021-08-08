
sources = ./../../../pimlico/*/*.cpp

root = ../../../

test: $(headers) ./test.cpp
	clang++ ./test.cpp \
			$(root)pimlico/*/*.cpp \
			$(root)pimlico/*/*/*/*.cpp \
			-I $(root)pimlico \
			-o ./test \
			-std=c++17 \
			-g3

clean:
	@rm -f ./test
