
SRCS := ASTNode.cpp \
        CodeGenerator.cpp \
        ErrorWarningTracker.cpp \
        ExpressionRecord.cpp \
        OperatorRecord.cpp \
        Scanner.cpp \
        Parser.cpp \
        Token.cpp \
        main.cpp

EXE := MicroCompiler

MAKEFLAGS := --no-print-directory
DEPEND_FILE := .dependlist

CC := g++
CFLAGS := --std=c++11 -g -Wall $(INC_DIRS)

LD := g++
LDFLAGS := 

OBJS := $(SRCS:%.cpp=%.o)

all: $(EXE)

$(EXE): $(OBJS)
	@echo "Linking $(EXE)"
	@$(LD) $(LDFLAGS) -o $(EXE) $(OBJS)

%.o:%.cpp
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	@echo "Cleaning $(EXE)"
	@$(RM) $(OBJS) $(EXE) $(DEPEND_FILE) *~

.PHONY: depend
depend:
	@echo "Building dependencies for: $(SRCS)"
	@/bin/cat < /dev/null > $(DEPEND_FILE); \
	 for file in $(SRCS) ; do \
	  srcDepend=$${file}.d; \
	  objFile=`echo $$file | /bin/sed -e 's~\.cpp~.o~'`; \
	  $(CC) $(CFLAGS) -MM -MT $$objFile \
             -MF $$srcDepend $$file; \
	  /bin/cat < $$srcDepend >> $(DEPEND_FILE); \
	  $(RM) $$srcDepend; \
	done

-include $(DEPEND_FILE)
