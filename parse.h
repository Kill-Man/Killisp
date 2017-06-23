#ifndef _PARSE_H_
#define _PARSE_H_

/// Initializes the parser with information on how to parse the strings given.
/// indent_block: !0 = the set of whitespace before any printed characters, if it exists, will be put as it's own token.
/// tab_value: >0 = all tab characters are converted to tab_value amount of spaces. 0 = a tab is read as a tab. <0 = tabs are ignored. Ignored if indent_block and whitespace are both 0.
/// singlequote_str: !0 = single quote represents the start and end of a character or string and entire string will be stored as a single token. #' % 2 != 0 = last quote stored as token by itself.
/// doublequote_str: !0 = double quote represents the start and end of a character or string and entire string will be stored as a single token. #" % 2 != 0 = last quote stored as token by itself.
/// escape_character: Character that will be used as escape in strings.
/// whitespace: !0 = whitespace is put into it's own block.
void parser_init(int indent_block, int tab_value, int singlequote_str, int doublequote_str, char escape_character, int whitespace);

/* !!!!!DO LATER (AFTER DONE WITH LANGUAGE)!!!!!
/// Splits a single string with multiple newlines into a set of single lines.
/// indiv_lines: Set of char * strings to hold each individual line. Dynamically Allocated.
/// string: char * string with the newlines to be sepparated into.
/// error: String that is output if newline is found in string.
/// error_code: Exit code if newline found in string.
void div_indiv_lines(char ***indiv_lines, const char *string, const char *error, const int error_code);
*/

/// Splits token_str into individual tokens to be stored dynamically into token_set.
/// token_set: Set of char * strings to store each individual token. Dynamically Allocated.
/// token_str: const char * string that will be tokenized.
void tokenize(char ***token_set, const char *token_str, const char *error, const int error_code);

#endif // _PARSE_H_
