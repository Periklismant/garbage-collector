#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../headers/instructions.h"
#include "../headers/error.h"
#include "../headers/stack.h"

#ifndef NEXT_INSTRUCTION
#define NEXT_INSTRUCTION \
		goto *(void *)(label_tab[fix((int) *pc)]);

#define POP_WITH_CHECK(s, data) \
		if(!isEmpty(s)) \
			data = pop(s); \
		else return STACK_EMPTY;

#define PUSH_WITH_CHECK(s, data) \
		err = push(s, data);  \
			if(err!=0) \
				return err;
	

#endif

int fix(int index){
	if(index == 0x0000002A)
		index = 0x00000019;
	else if(index == 0x00000030)
		index = 0x0000001A;
	else if(index == 0x00000031)
		index = 0x0000001B;
	else if(index == 0x00000032)
		index = 0x0000001C;
	else if(index == 0x00000019 || index == 0x0000001A || index = 0x0000001B || index = 0x0000001C)
		index = 0x00000000 // mas paei sto halt (undefined behavior)
	return index;
}

int main(int argc, char * argv[]){

	FILE *f;
	f = fopen(argv[1], "r");
	if(f == NULL){
		printf("cannot open input file\n");
		return 1;
	}
	char *buff; 
	fseek(f, 0, SEEK_END);
	int len = ftell(f);   // to find length
	rewind(f);
	buff = (char *)malloc((len+1)*sizeof(char));
	if(buff == NULL){
		printf("out of memory\n");
		return 1;
	}
	int result = fread(buff, 1, len, f);  // writes to buffer
	if(result!=len){
		printf("Reading error\n");
		return 1;
	}
	fclose(f);
	//  DEBUG //
	/*int i=0;
	while(i<len){
		printf("Byte value in position %d is %x\n", i, buff[i] & 0xff);
		i++;
	}*/
	//  DEBUG END //

	static void *label_tab[] = {
	&&halt_label,
	&&jump_label,
	&&jnz_label,
	&&dup_label,
	&&swap_label,
	&&drop_label,
	&&push4_label,
	&&push2_label,
	&&push1_label,
	&&add_label,
	&&sub_label,
	&&mul_label,
	&&div_label,
	&&mod_label,
	&&eq_label,
	&&ne_label,
	&&lt_label,
	&&gt_label,
	&&le_label,
	&&ge_label,
	&&not_label,
	&&and_label,
	&&or_label,
	&&input_label,
	&&output_label,
	&&clock_label,
	&&cons_label,
	&&hd_label,
	&&tl_label
};

	clock_t begin = clock();
	char *pc = &buff[0];
	char opcode;
	char temp_char;
	Stack* s = newStack(len);  // Κάθε byte του προγράμματος μπορεί να προσθέσει το πολύ ένα byte στη στοίβα από τη συνεισφορά του.
	int jump_addr_low, jump_addr_high;
	int jump_addr;
	int temp;
	int temp_int;
	int err;
	int a,b;
	clock_t end;
	double time_spent;

//next_instruction:
	opcode = pc[0];
	switch(opcode){
		case HALT:
		halt_label:
		//	printf("HALT FOUND!\n");
			break;
		case JUMP:
		jump_label:
		    //printf("JUMP FOUND!\n");
			jump_addr_low = ((int) pc[1]) & 0x000000ff;
			//printf("%d\n", jump_addr_low);
			jump_addr_high = ((int) pc[2]) & 0x000000ff;
			//printf("%d\n", jump_addr_high);
			jump_addr = (int) ((jump_addr_low | (jump_addr_high << 8)) & 0x0000ffff);
			//printf("%d\n", jump_addr);
			//index = ((int) buff[jump_addr]) & 0xff; // OUT OF BOUNDS??
			pc = &buff[jump_addr];
			//printf("%d\n", index);
			NEXT_INSTRUCTION;
		case JNZ:
		jnz_label:
		//	printf("JNZ FOUND!\n");
			POP_WITH_CHECK(s,temp);
		//	printf("Temp is %d\n", temp);
			if(temp!=0){
				jump_addr_low = ((int) pc[1]) & 0x000000ff;
		//		printf("Low: %x\n", jump_addr_low);
				jump_addr_high = ((int) pc[2]) & 0x000000ff;
		//		printf("High: %x\n", jump_addr_high);
				jump_addr = (int) ((jump_addr_low | (jump_addr_high << 8)) & 0x0000ffff);
		//		printf("jump_addr: %x\n", jump_addr);
				//index = ((int) buff[jump_addr]) & 0xff;  //OUT OF BOUNDS??
				pc = &buff[jump_addr];
		//		printf("Address is %d and value is %d", jump_addr, *pc);
			}
			else
				pc += JNZ_SIZEOF;
			NEXT_INSTRUCTION;
		case DUP:
		dup_label:
		//	printf("DUP FOUND!\n");
			temp_int = (int) pc[1];
			temp = getith(s, temp_int);
			PUSH_WITH_CHECK(s, temp);
			pc += DUP_SIZEOF;
			NEXT_INSTRUCTION;
		case SWAP:
		swap_label:
		//	printf("SWAP FOUND!\n");
			POP_WITH_CHECK(s,temp); 
			temp_int = (int) pc[1];
			if(s->top - (temp_int-1) < 0)
				return STACK_OUT_OF_BOUNDS; 
			PUSH_WITH_CHECK(s, getith(s, temp_int-1));
			err = setith(s, temp_int, temp);
			if(err !=0) 
				return err;
			pc += SWAP_SIZEOF;
			NEXT_INSTRUCTION;
		case DROP:
		drop_label:
		//	printf("DROP FOUND!\n");
			POP_WITH_CHECK(s,temp);
			pc += DROP_SIZEOF;
			NEXT_INSTRUCTION;
		case PUSH4:
		push4_label:
		//	printf("PUSH4 FOUND!\n");
			//printf("Combining %x, %x, %x and %x\n", pc[1], pc[2], pc[3], pc[4]);
			temp = (((int) pc[1]) & 0x000000ff) | ((((int) pc[2]) << 8) & 0x0000ff00) | ((((int) pc[3]) << 16) & 0x00ff0000) | ((((int) pc[4]) << 24) & 0xff000000);
			PUSH_WITH_CHECK(s, temp);
			//printf("Pushed %d into the stack\n", temp);
			pc += PUSH4_SIZEOF;
			NEXT_INSTRUCTION;
		case PUSH2:
		push2_label:
		//	printf("PUSH2 FOUND!\n");
			//printf("Combining %x and %x\n", pc[1], pc[2]);
			temp = (((int) pc[1]) & 0x000000ff) | ((((int) pc[2]) << 8) & 0x0000ff00);
			PUSH_WITH_CHECK(s, temp);
			//printf("Pushed %d into the stack\n", temp);
			pc += PUSH2_SIZEOF;
			NEXT_INSTRUCTION;
		case PUSH1:
		push1_label:
		//	printf("PUSH1 FOUND!\n");
			PUSH_WITH_CHECK(s, ((int) pc[1]) & 0x000000ff);
			//printf("Pushed %d into the stack\n", pc[1]);
			pc += PUSH1_SIZEOF;
			NEXT_INSTRUCTION;
		case ADD:
		add_label:
		//	printf("ADD FOUND!\n");
			POP_WITH_CHECK(s,b);
			//printf("b is %d\n", b);
			POP_WITH_CHECK(s,a);
			//printf("a is %d\n", a);
			temp_int = a + b;
			//printf("a + b is %d\n", temp_int);
			if((a>0 && b>0 && temp_int<0) || (a<0 && b<0 && temp_int>0))
				return ARITHMETIC_OVERFLOW;
			PUSH_WITH_CHECK(s, temp_int);
			pc += ADD_SIZEOF;
			NEXT_INSTRUCTION;
		case SUB:
		sub_label:
		//	printf("SUB FOUND!\n");
			POP_WITH_CHECK(s,b);
			//printf("b is %x\n", b);
			POP_WITH_CHECK(s,a);
			//printf("a is %x\n", a);
			temp_int = a - b;
			//printf("a - b is %d\n", temp_int);
			if((a>0 && b<0 && temp_int<0) || (a<0 && b>0 && temp_int>0))
				return ARITHMETIC_OVERFLOW;
			PUSH_WITH_CHECK(s, temp_int);
			pc += SUB_SIZEOF;
			NEXT_INSTRUCTION;
		case MUL:
		mul_label:
		//	printf("MUL FOUND!\n");
			POP_WITH_CHECK(s,b);
			//printf("b is %x\n", b);
			POP_WITH_CHECK(s,a);
			//printf("a is %x\n", a);
			temp_int = a*b;
			//printf("a * b is %d\n", temp_int);
			if(a != 0 && temp_int / a !=b)
				return ARITHMETIC_OVERFLOW;
			PUSH_WITH_CHECK(s, temp_int);
			pc += MUL_SIZEOF;
			NEXT_INSTRUCTION;
		case DIV:
		div_label:
		//	printf("DIV FOUND!\n");
			POP_WITH_CHECK(s,b);
			//printf("b is %x\n", b);
			POP_WITH_CHECK(s,a);
			//printf("a is %x\n", a);
			if(b==0)
				return DIVISION_BY_ZERO;
			temp_int = a/b;  
			//printf("a / b is %d\n", temp_int);
			PUSH_WITH_CHECK(s, temp_int);
			pc += DIV_SIZEOF;
			NEXT_INSTRUCTION;
		case MOD:
		mod_label:
		//	printf("MOD FOUND!\n");
			POP_WITH_CHECK(s,b);
			//printf("b is %x\n", b);
			POP_WITH_CHECK(s,a);
			//printf("a is %x\n", a);
			if(b==0)
				return DIVISION_BY_ZERO;
			temp_int = a%b;  
			//printf("a mod b is %d\n", temp_int);
			PUSH_WITH_CHECK(s, temp_int);
			pc += MOD_SIZEOF;
			NEXT_INSTRUCTION;
		case EQ:
		eq_label:
		//	printf("EQ FOUND!\n");
			POP_WITH_CHECK(s,b);
			//printf("b is %x\n", b);
			POP_WITH_CHECK(s,a);
			//printf("a is %x\n", a);
			temp_int = a == b;
		//	printf("a == b is %d\n", temp_int);
			PUSH_WITH_CHECK(s, temp_int);
		//	printf("After EQ, top of stack is %d\n", s->array[s->top]);
			pc += EQ_SIZEOF;
			NEXT_INSTRUCTION;
		case NE:
		ne_label:
		//	printf("NE FOUND!\n");
			POP_WITH_CHECK(s,b);
			//printf("b is %x\n", b);
			POP_WITH_CHECK(s,a);
			//printf("a is %x\n", a);
			temp_int = a != b;
			//printf("a != b is %d\n", temp_int);
			PUSH_WITH_CHECK(s, temp_int);
			pc += NE_SIZEOF;
			NEXT_INSTRUCTION;
		case LT:
		lt_label:
		//	printf("LT FOUND!\n");
			POP_WITH_CHECK(s,b);
			//printf("b is %x\n", b);
			POP_WITH_CHECK(s,a);
			//printf("a is %x\n", a);
			temp_int = a < b;
			//printf("a < b is %d\n", temp_int);
			PUSH_WITH_CHECK(s, temp_int);
			pc += LT_SIZEOF;
			NEXT_INSTRUCTION;
		case GT:
		gt_label:
		//	printf("GT FOUND!\n");
			POP_WITH_CHECK(s,b);
			//printf("b is %x\n", b);
			POP_WITH_CHECK(s,a);
			//printf("a is %x\n", a);
			temp_int = a > b;
			//printf("a > b is %d\n", temp_int);
			PUSH_WITH_CHECK(s, temp_int);
			pc += GT_SIZEOF;
			NEXT_INSTRUCTION;
		case LE:
		le_label:
		//	printf("LE FOUND!\n");
			POP_WITH_CHECK(s,b);
			//printf("b is %x\n", b);
			POP_WITH_CHECK(s,a);
			//printf("a is %x\n", a);
			temp_int = a <= b;
			//printf("a <= b is %d\n", temp_int);
			PUSH_WITH_CHECK(s, temp_int);
			pc += LE_SIZEOF;
			NEXT_INSTRUCTION;
		case GE:
		ge_label:
		//	printf("GE FOUND!\n");
			POP_WITH_CHECK(s,b);
			//printf("b is %x\n", b);
			POP_WITH_CHECK(s,a);
			//printf("a is %x\n", a);
			temp_int = a >= b;
			//printf("a >= b is %d\n", temp_int);
			PUSH_WITH_CHECK(s, temp_int);
			pc += GE_SIZEOF;
			NEXT_INSTRUCTION;
		case NOT:
		not_label:
		//	printf("NOT FOUND!\n");
			POP_WITH_CHECK(s,b);
			//printf("b is %x\n", b);
			temp_int = !b; 
			//printf("!b is %d\n", temp_int);
			PUSH_WITH_CHECK(s, temp_int);
			pc += NOT_SIZEOF;
			NEXT_INSTRUCTION;
		case AND:
		and_label:
		//	printf("AND FOUND!\n");
			POP_WITH_CHECK(s,b);
			//printf("b is %x\n", b);
			POP_WITH_CHECK(s,a);
			//printf("a is %x\n", a);
			temp_int = a && b;
			//printf("a && b is %d\n", temp_int);
			PUSH_WITH_CHECK(s, temp_int);
			pc += AND_SIZEOF;
			NEXT_INSTRUCTION;
		case OR:
		or_label:
		//	printf("OR FOUND!\n");
			POP_WITH_CHECK(s,b);
			//printf("b is %x\n", b);
			POP_WITH_CHECK(s,a);
			//printf("a is %x\n", a);
			temp_int = a || b;
			//printf("a || b is %d\n", temp_int);
			PUSH_WITH_CHECK(s, temp_int);
			pc += OR_SIZEOF;
			NEXT_INSTRUCTION;
		case INPUT:
		input_label:
		//	printf("INPUT FOUND!\n");
			scanf("%c", &temp_char);
			//printf("I read this \' %x \' character.\n", temp_char);
			PUSH_WITH_CHECK(s, ((int) temp_char) & 0x000000ff);
			pc += INPUT_SIZEOF;
			NEXT_INSTRUCTION;
		case OUTPUT:
		output_label:
		//	printf("OUTPUT FOUND!\n");
			POP_WITH_CHECK(s,temp);
			printf("%c", (char) (temp & 0x000000ff));
			pc += OUTPUT_SIZEOF;
			NEXT_INSTRUCTION;
		case CLOCK:
		clock_label:
		//	printf("CLOCK FOUND!\n");
			end = clock();
			time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
			printf("%0.6lf\n", time_spent);
			pc += CLOCK_SIZEOF;
			NEXT_INSTRUCTION;
		case CONS:
		cons_label:
			printf("CONS FOUND!\n");
			pc += CONS_SIZEOF;
			NEXT_INSTRUCTION;

		default:
			break;
	}
	// DEBUG //
	/*while(s->top>=0){
		int x = pop(s) & 0xff;
		printf("Element %u is: %d\n", s->top + 1, x);
	}*/
	// DEBUG END //
	free(s->array);
	free(s);
	free(buff);
	/*end = clock();
	time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
	printf("Time elapsed is %0.6lf seconds\n", time_spent); */
	return 0;
}