NAME = mycc

LLVM_CONFIG = /usr/bin/llvm-config-13

NO_WARNING =  -Wno-return-type \
	-Wno-c++11-compat-deprecated-writable-strings \
	-Wno-deprecated-register \
	-Wno-switch \
CXXFLAGS = `$(LLVM_CONFIG) --cppflags` -std=c++14 $(NO_WARNING) -fpermissive -g
LDFLAGS = `$(LLVM_CONFIG) --ldflags`
LIBS = `$(LLVM_CONFIG) --libs --system-libs`

OBJS = parser.o lexer.o IRgen.o codegen.o

all : $(NAME)

parser.cpp: parser.y
	bison -d -o parser.cpp parser.y

parser.hpp: parser.cpp

lexer.cpp: lexer.l
	flex -o lexer.cpp lexer.l

%.o: %.cpp ast.h misc.h
	g++ -c $(CXXFLAGS) -g -o $@ $< 

$(NAME): $(OBJS)
	g++ -o $@ $(OBJS) $(LIBS) $(LDFLAGS)

.PHONY: clean
clean:
	-rm -f *.o
	-rm -f parser.hpp parser.cpp lexer.cpp mycc
	-rm -f *.ll
