Scriptname StringUtil extends ScriptObject Native

{ Returns the length of the given string }
int Function GetLength(string asString) Native Global

{ Returns the character at the passed index of the given string }
string Function GetNthCharacter(string asString, int aiIndex) Native Global

{ Returns true if the given character is a letter, if more than a character is passed, only the first character is checked }
bool Function IsLetter(string asChar) Native Global

{ Returns true if the given character is a digit, if more than a character is passed, only the first character is checked }
bool Function IsDigit(string asChar) Native Global

{ Returns true if the given character is punctuation, if more than a character is passed, only the first character is checked }
bool Function IsPunctuation(string asChar) Native Global

{ Returns true if the given character is "printable", if more than a character is passed, only the first character is checked }
bool Function IsPrintable(string asChar) Native Global

{ Returns the ACSII character code of the given char, if more than a character is passed, only the first character is returned }
int Function AsInt(string asChar) Native Global

{ Returns the string representation of the given ASCII character code }
string Function AsChar(int aiChar) Native Global

{ Returns index of the start of the string to find in the passed string, start index determines what character of the passed string the search starts at }
int Function Find(string asString, string asToFind, int aiStartIndex = 0) Native Global

{ Returns a portion of the passed string beginning from the passed index, if length is 0, the rest of the string is returned }
string Function Substring(string asString, int aiStartIndex, int aiLength = 0) Native Global

{ Returns a string as an array of characters, a delimiter can optionally be passed to only split the string at certain characters }
string[] Function StringToArray(string asString, string asDelim = "") Native Global

{ Returns a array of strings as a single string, a delimiter can optionally be passed to be inserted between the strings }
string Function ArrayToString(string[] aaString, string asDelim = "") Native Global
