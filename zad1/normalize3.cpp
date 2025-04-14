#include <chrono>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#define MAX_WORD_LEN 256

/**
 * @brief Normalizes a text string according to specified rules.
 *
 * Rules:
 * 1. Remove non-printable ASCII characters (codes < 32 or > 126).
 * 2. Replace sequences of whitespace characters with a single space.
 * 3. Convert all letters to lowercase.
 * 4. Convert punctuation to commas.
 * 5. Eliminate consecutive duplicate words ("hi hi world"->"hi world").
 *
 * @param text The input string to normalize.
 * @return The normalized string.
 */
char* normalize_text(const char* text, const ssize_t size) {
  char* result = new char[size + 1];  // Null-terminated string
  ssize_t result_len = 0;
  char* word = new char[MAX_WORD_LEN];
  ssize_t word_len = 0;
  char* last_word = new char[MAX_WORD_LEN];
  ssize_t last_word_len = 0;
  enum State { NORMAL, COMMA, SPACE, DUPLICATE_SPACE } state = NORMAL;

  for (size_t i = 0; i < size; ++i) {
    char c = text[i];
    if (c < 32 || c > 126) continue;  // rule 1

    if (c == ' ') {
      if (state == SPACE || state == DUPLICATE_SPACE) {  // rule 2
        state = DUPLICATE_SPACE;
      } else {
        state = SPACE;
      }
    } else if (isalpha(c)) {  // rule 3
      c = tolower(c);
      state = NORMAL;
    } else if (ispunct(c)) {  // rule 4
      c = ',';
      state = COMMA;
    } else {
      state = NORMAL;
    }

    if (state == COMMA || state == SPACE) {
      // end of a word
      if (word_len > 0) {  // rule 5
        if (strcmp(word, last_word) != 0) {
          memcpy(result + result_len, word, word_len + 1);
          result_len += word_len;
        }
        memcpy(last_word, word, word_len + 1);
        last_word_len = word_len;
        word_len = 0;
      }
      result[result_len++] = c;  // only delimiter
    } else if (state == NORMAL) {
      // in a word
      word[word_len++] = c;  // only alphanumeric
    }
  }
  if (strcmp(word, last_word) != 0) {  // last word
    memcpy(result + result_len, word, word_len + 1);
    result_len += word_len;
  }

  // Clean up
  result[result_len] = '\0';
  delete[] word;
  delete[] last_word;

  return result;
}

int test() {
  std::string test1 =
      "  This is...  a TEST text\t with   EXTRA spaces\nand punctuation!! "
      "And\t\tand repeated repeated words words.";
  std::cout << "Original: \"" << test1 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text(test1.c_str(), test1.length())
            << "\"" << std::endl;

  std::string test2 = "Hello hello world... World.";
  std::cout << "Original: \"" << test2 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text(test2.c_str(), test2.length())
            << "\"" << std::endl;

  std::string test3 = "\t\n  First.first, ,, second   second; third\n\n";
  std::cout << "Original: \"" << test3 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text(test3.c_str(), test3.length())
            << "\"" << std::endl;

  std::string test4 = "No duplicates here.";
  std::cout << "Original: \"" << test4 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text(test4.c_str(), test4.length())
            << "\"" << std::endl;

  std::string test5 = "     leading space and trailing space     ";
  std::cout << "Original: \"" << test5 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text(test5.c_str(), test5.length())
            << "\"" << std::endl;

  std::string test6 = "word1 word1, word2 word2";
  std::cout << "Original: \"" << test6 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text(test6.c_str(), test6.length())
            << "\"" << std::endl;

  std::string test7 = "Hello\x07 World\x7F!";
  std::cout << "Original: \"" << test7 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text(test7.c_str(), test7.length())
            << "\"" << std::endl;

  return 1;
}

int benchmark(const int repeats) {
  // Read all content directly from standard input into a string
  std::string content{std::istreambuf_iterator<char>(std::cin),
                      std::istreambuf_iterator<char>()};
  const char* content_cstr = content.c_str();
  ssize_t content_length = content.length();

  // Check if the content is empty
  if (content.empty()) {
    std::cerr << "Error: No content received from standard input." << std::endl;
    return 1;
  }

  // Prepare a pointer to hold the normalized content
  char* normalized_content;

  // Measure the time taken to normalize the text multiple times
  const auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < repeats; ++i) {
    normalized_content = normalize_text(content_cstr, content_length);
  }
  const auto finish = std::chrono::steady_clock::now();
  const std::chrono::duration<double> elapsed_seconds = finish - start;

  // Print the final normalized content to standard output
  std::cout << normalized_content << std::endl;

  // Print the elapsed time to standard error
  std::cerr << "\033[95m"  // Start color
            << "Benchmark (" << repeats
            << " iterations): " << elapsed_seconds.count() << " seconds"
            << "\033[0m"  // Reset color
            << std::endl;

  return 0;
}

int main() {
  constexpr int repeats{1000};  // with 100KB files

  // return test();
  return benchmark(repeats);
}