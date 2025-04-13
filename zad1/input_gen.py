import random
import string
import os


def generate_random_word(min_len=3, max_len=10):
    """Generates a random word using ASCII letters."""
    length = random.randint(min_len, max_len)
    return "".join(random.choice(string.ascii_letters) for _ in range(length))


def generate_sample_file(filename, size_in_mb):
    """
    Generates a sample text file of a specified size in MB.

    The file will contain a mix of:
    - Random words (lowercase and uppercase)
    - Sequences of whitespace characters (spaces, tabs, newlines)
    - Various punctuation marks
    - Non-printable ASCII characters (codes < 32 or > 126)
    - Occasional repeated words
    """
    target_size_bytes = size_in_mb * 1024 * 1024
    print(f"Generating file '{filename}' of approximately {size_in_mb} MB...")

    # Define character sets
    whitespace_chars = [" ", "\t", "\n", "\r"]
    punctuation_chars = list(string.punctuation)
    # Generate list of non-printable chars based on ASCII codes
    non_printable_chars = [chr(i) for i in list(range(0, 32)) + list(range(127, 256))]
    # Remove whitespace chars from non_printable list if they are handled separately
    non_printable_chars = [c for c in non_printable_chars if c not in whitespace_chars]

    last_word = ""
    words_generated = 0

    try:
        # Open file in binary write mode to precisely control byte size and allow any character
        with open(filename, "wb") as f:
            current_size = 0
            while current_size < target_size_bytes:
                chunk = b""
                rand_choice = random.random()

                if rand_choice < 0.65:  # Add a word
                    word = generate_random_word()
                    # Occasionally make uppercase
                    if random.random() < 0.1:
                        word = word.upper()
                    # Occasionally repeat the last word explicitly
                    if random.random() < 0.05 and last_word:
                        chunk = last_word.encode("utf-8", errors="ignore")
                    else:
                        chunk = word.encode("utf-8", errors="ignore")
                    last_word = word  # Store the actual word generated
                    words_generated += 1
                    # Add a space after most words
                    if random.random() < 0.9:
                        chunk += b" "

                elif rand_choice < 0.85:  # Add whitespace sequence
                    seq_len = random.randint(1, 5)
                    chunk = "".join(
                        random.choice(whitespace_chars) for _ in range(seq_len)
                    ).encode("utf-8", errors="ignore")

                elif rand_choice < 0.95:  # Add punctuation
                    punc = random.choice(punctuation_chars)
                    chunk = punc.encode("utf-8", errors="ignore")
                    # Add a space after punctuation sometimes
                    if random.random() < 0.5:
                        chunk += b" "

                else:  # Add non-printable char
                    if non_printable_chars:
                        # Encode as latin-1 which maps 0-255 directly to bytes
                        chunk = random.choice(non_printable_chars).encode(
                            "latin-1", errors="ignore"
                        )
                    else:
                        # Fallback if non_printable_chars list is somehow empty
                        chunk = generate_random_word(1, 1).encode(
                            "utf-8", errors="ignore"
                        )

                f.write(chunk)
                current_size = f.tell()

                # Add explicit word repetition occasionally
                if words_generated > 10 and random.random() < 0.03 and last_word:
                    repeat_chunk = f" {last_word}".encode("utf-8", errors="ignore")
                    f.write(repeat_chunk)
                    current_size = f.tell()
                    words_generated = 0  # Reset counter

            # Ensure the file is exactly the target size if slightly under
            if current_size < target_size_bytes:
                padding_size = target_size_bytes - current_size
                f.write(b" " * padding_size)

        final_size_mb = os.path.getsize(filename) / (1024 * 1024)
        print(f"File '{filename}' generated successfully.")
        print(f"Actual file size: {final_size_mb:.2f} MB")

    except IOError as e:
        print(f"Error writing to file {filename}: {e}")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")


if __name__ == "__main__":
    output_filename = "large_input_file.txt"
    # Set desired file size in Megabytes
    # Start with a smaller size (e.g., 1-10 MB) for faster testing
    file_size_mb = 0.1

    generate_sample_file(output_filename, file_size_mb)
    print("\nRun the script again to generate a new file or modify 'file_size_mb'.")
