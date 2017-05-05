#include <bcc/tokens.h>
#include <unistd.h>
#include <fcntl.h>

struct token_list * scan(const char * filename);
struct token * get_next_token(const char ** source);
