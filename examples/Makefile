all: adder hello_world name multiplier compare_ints

adder: adder.bc
	../bcc adder.s < adder.bc > /dev/null

hello_world: hello_world.bc
	../bcc hello_world.s < hello_world.bc > /dev/null

name: name.bc
	../bcc name.s < name.bc > /dev/null

multiplier: multiplier.bc
	../bcc multiplier.s < multiplier.bc > /dev/null

compare: compare.bc
	../bcc compare.s < compare.bc > /dev/null

clean:
	rm -f *.s adder hello_world name multiplier compare_ints
