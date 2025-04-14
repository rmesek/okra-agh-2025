#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

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
std::string normalize_text(const std::string& text) {
  std::string result;
  std::string word;
  std::string last_word;

  for (char c : text) {
    // Rule 1: Skip non-printable ASCII
    if (c < 32 || c > 126) continue;

    // Rule 3: Convert to lowercase
    if (isalpha(c)) c = tolower(c);

    // Rule 4: Convert punctuation to commas
    if (ispunct(c)) c = ',';

    // Handle new word delimiters
    if (c == ' ' || c == ',') {
      // Rule 5: Handle consecutive duplicate words
      if (!word.empty()) {
        if (word != last_word) result += word;
        last_word = word;
        word.clear();
      }

      // Rule 2: Skip consecutive whitespace
      if (c == ' ' && !result.empty() && result.back() == ' ') continue;

      result += c;
      continue;
    }

    // Handle regular characters
    word += c;
  }
  // Handle the last word
  if (word != last_word) result += word;

  return result;
}

int test() {
  std::string test1 =
      "  This is...  a TEST text\t with   EXTRA spaces\nand punctuation!! "
      "And\t\tand repeated repeated words words.";
  std::cout << "Original: \"" << test1 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text(test1) << "\"" << std::endl;

  std::string test2 = "Hello hello world... World.";
  std::cout << "Original: \"" << test2 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text(test2) << "\"" << std::endl;

  std::string test3 = "\t\n  First.first, ,, second   second; third\n\n";
  std::cout << "Original: \"" << test3 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text(test3) << "\"" << std::endl;

  std::string test4 = "No duplicates here.";
  std::cout << "Original: \"" << test4 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text(test4) << "\"" << std::endl;

  std::string test5 = "     leading space and trailing space     ";
  std::cout << "Original: \"" << test5 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text(test5) << "\"" << std::endl;

  std::string test6 = "word1 word1, word2 word2";
  std::cout << "Original: \"" << test6 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text(test6) << "\"" << std::endl;

  std::string test7 = "Hello\x07 World\x7F!";
  std::cout << "Original: \"" << test7 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text(test7) << "\"" << std::endl;

  return 1;
}

int benchmark(const int repeats) {
  // Read all content directly from standard input into a string
  std::string content{std::istreambuf_iterator<char>(std::cin),
                      std::istreambuf_iterator<char>()};

  // Check if the content is empty
  if (content.empty()) {
    std::cerr << "Error: No content received from standard input." << std::endl;
    return 1;
  }

  // Prepare a string to hold the normalized content
  std::string normalized_content;

  // Measure the time taken to normalize the text multiple times
  const auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < repeats; ++i) {
    normalized_content = normalize_text(content);
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