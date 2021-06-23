
header_path = ./../../../pimlico
headers = $(header_path)/*.hpp

test: $(headers) ./test.cpp
	clang++ ./test.cpp -o ./test -I $(header_path) -std=c++17

clean:
	@rm -f ./test
