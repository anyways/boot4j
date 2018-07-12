CC=gcc
RM=rm

BOOT4J_EXE=boot4j.exe

all: clean $(BOOT4J_EXE)

$(BOOT4J_EXE): boot4j.c
	$(CC) "-I$(JAVA_HOME)\include" "-I$(JAVA_HOME)\include\win32" $^ -o $@

clean:
	$(RM) -rf ./*.exe ./*.o
