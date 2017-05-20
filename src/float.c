#include <bcc/tokens.h>

int matches_float(const char * str, char ** img)
{
		enum states { A, B, C, D, E, F, G, H};
		enum states state = A;
		int multiplier = 1;
		const char * beg = str;
		
		while(1) {
				//printf("State %d\n", state );
				switch(state) {
				case A:
						if (*str == '.'){
								state = B;
								str++;
						}
						else if (IS_DIGIT(*str)){
								state = C;
								str++;
						} 
						else return 0;
						break;
						
				case B:
						if ( IS_DIGIT(*str)){
								str++;
								state = D;
						} 
						else return 0;
						break;
						
				case C:
						if(IS_DIGIT(*str)){
								str++;
						}
						else if ( *str == 'e' || *str == 'E'){
								str++;
								state = F;
						}
						else if ( *str == '.'){
								str++;
								state = D;
						}
						else return (str - beg);
						break;

				case D:
						if (IS_DIGIT(*str))
								str++;
				
						else if ( *str == 'e' || *str == 'E'){
								str++;
								state = F;
						}
						else return (str - beg);
						break;

				case E:


						break;

				case F:
						if ( *str == '-'){
								str++;
								state = G;
						} 
						else if (IS_DIGIT(*str)){
								str++;
								state = H;
						}
						else return 0;

						break;

				case G:
						if(IS_DIGIT(*str)){
								str++;
								state = H;
						}
						else return 0;

						break;

				case H:
						if(IS_DIGIT(*str))
								str++;
						else return (str - beg);

						break;

				default:
						break;
				}
		}	
}
