#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <iterator>
#include <stdint.h>

std::string build_output_file_path(const std::string &input_file_path, const std::string &suffix)
{
    std::size_t extension_position = input_file_path.find_last_of('.');
    if (extension_position == std::string::npos)
    {
        return input_file_path + suffix + ".txt";
    }

    return input_file_path.substr(0, extension_position) + suffix + ".txt";
}

void convert_key_to_shift_array(std::string key, std::vector<uint8_t> &shift_vector)
{
    for (auto c : key)
    {
        if (c >= 'A' && c <= 'Z')
        {
            shift_vector.push_back(c - 'A');
        }
        else if (c >= 'a' && c <= 'z')
        {
            shift_vector.push_back(c - 'a');
        }
        else
        {
            throw std::runtime_error("Expected only letters on key.");
        }
    }
}

void convert_plain_text_to_int_array(std::string plain_text, std::vector<uint8_t> &plain_text_vector)
{
    for (auto c : plain_text)
    {
        if (c >= 'A' && c <= 'Z')
        {
            plain_text_vector.push_back(c - 'A');
        }
        else if (c >= 'a' && c <= 'z')
        {
            plain_text_vector.push_back(c - 'a');
        }
        else if (c == ' ')
        {
            plain_text_vector.push_back(c);
        }
        else
        {
            continue;
        }
    }
}

void vigenere(std::string plain_text, std::string key, std::string &cipher_text)
{
    uint32_t key_position = 0;
    std::vector<uint8_t> shift_vector;
    std::vector<uint8_t> plain_text_vector;
    convert_key_to_shift_array(key, shift_vector);
    convert_plain_text_to_int_array(plain_text, plain_text_vector);

    for (auto c : plain_text_vector)
    {
        if (c == ' ')
        {
            cipher_text.push_back(c);
            continue;
        }

        c += shift_vector.at(key_position);

        cipher_text.push_back(((c % 26) + 'A'));
        key_position++;
        if (key_position >= shift_vector.size())
        {
            key_position = 0;
        }
    }
}

void reverse_vigenere(std::string cipher_text, std::string key, std::string &plain_text)
{
    uint32_t key_position = 0;
    std::vector<uint8_t> shift_vector;
    std::vector<uint8_t> cipher_text_vector;
    convert_key_to_shift_array(key, shift_vector);
    convert_plain_text_to_int_array(cipher_text, cipher_text_vector);

    for (auto c : cipher_text_vector)
    {
        if (c == ' ')
        {
            plain_text.push_back(c);
            continue;
        }

        uint8_t shift = shift_vector.at(key_position);
        plain_text.push_back(((c + 26 - shift) % 26) + 'A');
        key_position++;
        if (key_position >= shift_vector.size())
        {
            key_position = 0;
        }
    }
}

int main(int argc, char *argv[])
{

    try
    {
        if (argc != 4)
        {
            std::cout << "Usage: " << argv[0] << " <input_file_path> <encrypt|decrypt> <key>" << std::endl;
            return 1;
        }

        std::string input_text;
        std::string key = argv[3];
        std::string result_text;
        std::string file_path = argv[1];
        std::string operation = argv[2];
        std::string output_suffix;
        std::string output_file_path;
        std::ifstream input_file;
        std::ofstream output_file;

        input_file.open(file_path);
        if (!input_file.is_open())
        {
            throw std::runtime_error("Could not open input file.");
        }

        input_text.assign(std::istreambuf_iterator<char>(input_file),
                          std::istreambuf_iterator<char>());

        if (operation == "encrypt")
        {
            vigenere(input_text, key, result_text);
            output_suffix = "_encrypted";
        }
        else if (operation == "decrypt")
        {
            reverse_vigenere(input_text, key, result_text);
            output_suffix = "_decrypted";
        }
        else
        {
            throw std::runtime_error("Expected operation to be encrypt or decrypt.");
        }

        output_file_path = build_output_file_path(file_path, output_suffix);
        output_file.open(output_file_path);
        if (!output_file.is_open())
        {
            throw std::runtime_error("Could not create output file.");
        }

        output_file << result_text;

        std::cout << "Result text: " << result_text << std::endl;
        std::cout << "Saved to file: " << output_file_path << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << e.what();
    };

    return 0;
}
