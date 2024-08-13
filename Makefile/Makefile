A = a b c
B = $(foreach f, $(A),$(f).c)

C = a b c d/

D = $(filter %/, $(C))
E = $(filter-out %/, $(C))

file = $(wildcard *.c)

files2 = a.c b.c c.c d.c
files2 = $(wildcard $(files2))

all:
	@echo B = $(B)
	@echo D = $(D)
	@echo E = $(E)
	@echo $(files2)
	@echo files = $(files2)