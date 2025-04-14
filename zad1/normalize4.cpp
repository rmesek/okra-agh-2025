#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

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
  std::string word, last_word;
  enum State { NORMAL, COMMA, SPACE, DUPLICATE_SPACE } state = NORMAL;

  for (char c : text) {
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
      if (!word.empty()) {  // rule 5
        if (word != last_word) result += word;
        last_word = word;
        word.clear();
      }
      result += c;  // only delimiter
    } else if (state == NORMAL) {
      // in a word
      word += c;  // only alphanumeric
    }
  }
  if (word != last_word) result += word;  // last word

  return result;
}

std::string normalize_text_threaded(const std::string& text) {
  unsigned int num_threads = std::thread::hardware_concurrency();

  // For very small texts or single thread systems, use the original function
  if (text.size() < 1000 || num_threads <= 1) {
    return normalize_text(text);
  }

  // Split the text into chunks
  std::vector<std::string> chunks;
  std::vector<std::thread> threads;
  std::vector<std::string> results(num_threads);

  size_t chunk_size = text.size() / num_threads;
  for (unsigned int i = 0; i < num_threads; ++i) {
    size_t start = i * chunk_size;
    size_t end = (i == num_threads - 1) ? text.size() : (i + 1) * chunk_size;
    chunks.push_back(text.substr(start, end - start));
  }

  // Process each chunk in a separate thread
  for (unsigned int i = 0; i < num_threads; ++i) {
    threads.emplace_back(
        [&chunks, &results, i]() { results[i] = normalize_text(chunks[i]); });
  }

  // Wait for all threads to complete
  for (auto& thread : threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }

  std::string combined_result;
  for (const auto& result : results) {
    combined_result += result;
  }
  return combined_result;
}

int test() {
  std::string test1 =
      "  This is...  a TEST text\t with   EXTRA spaces\nand punctuation!! "
      "And\t\tand repeated repeated words words.";
  std::cout << "Original: \"" << test1 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text_threaded(test1) << "\""
            << std::endl;

  std::string test2 = "Hello hello world... World.";
  std::cout << "Original: \"" << test2 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text_threaded(test2) << "\""
            << std::endl;

  std::string test3 = "\t\n  First.first, ,, second   second; third\n\n";
  std::cout << "Original: \"" << test3 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text_threaded(test3) << "\""
            << std::endl;

  std::string test4 = "No duplicates here.";
  std::cout << "Original: \"" << test4 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text_threaded(test4) << "\""
            << std::endl;

  std::string test5 = "     leading space and trailing space     ";
  std::cout << "Original: \"" << test5 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text_threaded(test5) << "\""
            << std::endl;

  std::string test6 = "word1 word1, word2 word2";
  std::cout << "Original: \"" << test6 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text_threaded(test6) << "\""
            << std::endl;

  std::string test7 = "Hello\x07 World\x7F!";
  std::cout << "Original: \"" << test7 << "\"" << std::endl;
  std::cout << "Normalized: \"" << normalize_text_threaded(test7) << "\""
            << std::endl;

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
    normalized_content = normalize_text_threaded(content);
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