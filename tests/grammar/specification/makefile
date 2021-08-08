
sources = ./../../../pimlico/*/*.cpp

test: $(headers) ./test.cpp
	g++-7 ./test.cpp \
			../../../pimlico/*/*.cpp \
			../../../pimlico/*/*/*.cpp \
			../../../pimlico/*/*/*/*.cpp \
			-o ./test \
			-I ../../../pimlico \
			-std=c++17 \
			-g3

clean:
	@rm -f ./test
