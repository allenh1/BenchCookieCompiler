all: compiler adder hello_world name multiplier also and

compiler:
	cd ../ && make && cd examples

also: hello_c mult_table strtest compute strlen hello_bcc

and: compare high-low

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

mult_table: mult_table.bc
	../bcc mult_table.s < mult_table.bc > /dev/null

strtest: strtest.bc
	../bcc strtest.s < strtest.bc &> /dev/null

compute: compute.bc
	../bcc compute.s < compute.bc > /dev/null

strlen: strlen.bc
	../bcc strlen.s < strlen.bc > /dev/null

hello_c: hello_c.bc
	../bcc hello_c.s < hello_c.bc > /dev/null

hello_bcc: hello_bcc.c
	gcc hello_bcc.c hello_c.o -o hello_bcc

high-low: high-low.bc
	../bcc high-low.s < high-low.bc > /dev/null

clean:
	rm -f *.s *.o *~ adder hello_world name multiplier
	rm -f strtest compute strlen mult_table compare
	rm -f hello_bcc high-low
