#ifndef _PARSE_H_
#define _PARSE_H_

/// Initializes the parser with information on how to parse the strings given.
/// indent_block: !0 = the set of whitespace before any printed characters, if it exists, will be put as it's own token.
/// tab_value: >0 = all tab characters are converted to tab_value amount of spaces. 0 = a tab is read as a tab. <0 = tabs are ignored. Ignored if indent_block and whitespace are both 0.
/// num_start_with_dec: !0 = leading periods signal the start of a decimal number. 0 = leading periods go into separate tokens
/// singlequote_str: !0 = single quote represents the start and end of a character or string and entire string will be stored as a single token. #' odd = last quote stored as token by itself.
/// doublequote_str: !0 = double quote represents the start and end of a character or string and entire string will be stored as a single token. #" odd = last quote stored as token by itself.
/// escape_character: Character that will be used as escape in strings.
/// last_quote_str: !0 = if #' || #" odd last quote will be string without ending quote. 0 = odd quote means last quote is in token by itself along with everything after it. Next parse puts everything in a
//// string until the first quote is met.
/////// example: !0 = "test" "test" "est => "est
//////// that" => "est\nthat"
///////           0 = "test" "test" "est => " est
//////// that" => " est that "
/// whitespace: !0 = whitespace is put into it's own block.
/// word_chars: string of characters that will be included in a single token
/// begin_chars: string of characters that will be able to start a token.
/// begin_chars_in_word: !0 = begin_chars included in word_chars. 0 = begin_chars not included in word_chars unless directly put in word_chars.
void parser_init(int indent_block, int tab_value, int num_start_with_dec, int singlequote_str, int doublequote_str, char escape_character, int last_quote_str, int whitespace, char *word_chars, char *begin_chars, int begin_chars_in_word);

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
