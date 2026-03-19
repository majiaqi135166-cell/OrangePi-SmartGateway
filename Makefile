# 定义编译器
CC = gcc

# 定义编译选项（-Wall 开启所有警告，方便找错）
CFLAGS = -Wall

# 最终生成的可执行文件名
TARGET = smart_monitor

# 所有的源文件
SRCS = main.c functions.c

# 默认规则：输入 make 就会执行这个
all:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# 清理规则：输入 make clean 就会删除编译生成的文件
clean:
	rm -f $(TARGET)
