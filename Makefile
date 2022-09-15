CROSS_COMPILE ?= aarch64-linux-gnu-

# PREFIX = /usr


# AS = $(CROSS_COMPILE)as
# LD = $(CROSS_COMPILE)ld
CC = $(CROSS_COMPILE)gcc

# ASFLAGS = -g
# LDFLAGS = -g -static -o
# LIBPATH = -L/usr/lib64/gcc/aarch64-linux-gnu/11.3.0 -L/usr/lib64
# LIBPATH = -L/usr/aarch64-linux-gnu/lib64 -L/usr/aarch64-linux-gnu/lib
# OBJPATH = /usr/lib64/gcc/aarch64-linux-gnu/11.3.0/../../../../lib64
# OBJPATH = /usr/aarch64-linux-gnu/lib
# LIBS = -lc -lstdc++
# PREOBJ = $(OBJPATH)/crt1.o $(OBJPATH)/crti.o
# POSTOBJ = $(OBJPATH)/crtn.o

SRCS = main.c lodepng.c
ASM_SRCS = tr.s
# OBJS = $(SRCS:.s=.o)

EXE = prog5

# all: $(SRCS) $(EXE)

# clean:
# 	rm -rf $(EXE) $(OBJS)

# $(EXE): $(OBJS)
# 	$(LD) $(LDFLAGS) $(LIBPATH) $(PREOBJ) $(OBJS) $(POSTOBJ) $(LIBPATH) $(LIBS) -o $@

# .s.o:
# 	$(AS) $(ASFLAGS) $< -o $@

all: asm O0 O1 O2 O3 Ofast
asm: $(SRCS)
	$(CC) -DIMPL_ASM -pie -static -g $(SRCS) $(ASM_SRCS) -o $(EXE)_asm $(LIBPATH) $(LIBS)
O0: $(SRCS)
	$(CC) -O0 -pie -static -g $(SRCS) -o $(EXE)_o0 $(LIBPATH) $(LIBS)
O1: $(SRCS)
	$(CC) -O1 -pie -static -g $(SRCS) -o $(EXE)_o1 $(LIBPATH) $(LIBS)
O2: $(SRCS)
	$(CC) -O2 -pie -static -g $(SRCS) -o $(EXE)_o2 $(LIBPATH) $(LIBS)
O3: $(SRCS)
	$(CC) -O3 -pie -static -g $(SRCS) -o $(EXE)_o3 $(LIBPATH) $(LIBS)
Ofast: $(SRCS)
	$(CC) -Ofast -pie -static -g $(SRCS) -o $(EXE)_ofast $(LIBPATH) $(LIBS)
