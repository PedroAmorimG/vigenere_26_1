#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <iterator>
#include <stdint.h>
#include <cctype>

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

// Remove tudo que não é letra
std::string clean_text(const std::string &text)
{
    std::string result;
    for (char c : text)
    {
        if (isalpha(c))
            result.push_back(toupper(c));
    }
    return result;
}

// Divide em grupos
std::vector<std::string> split_groups(const std::string &text, int key_size)
{
    std::vector<std::string> groups(key_size);

    for (int i = 0; i < text.size(); i++)
    {
        groups[i % key_size] += text[i];
    }

    return groups;
}

// Frequência
std::vector<int> frequency(const std::string &group)
{
    std::vector<int> freq(26, 0);

    for (char c : group)
        freq[c - 'A']++;

    return freq;
}

// Descobre letra da chave
char find_key_letter(const std::string &group)
{
    double portuguese_freq[26] = {
        14.63, 1.04, 3.88, 4.99, 12.57, 1.02, 1.30,
        0.78, 6.18, 0.40, 0.02, 2.78, 4.74,
        5.05, 10.73, 2.52, 1.20, 6.53,
        7.81, 4.34, 4.63, 1.67, 0.01, 0.21,
        0.01, 0.47
    };

    int n = group.size();
    double best_score = 1e9;
    int best_shift = 0;

    // testa todos os shifts possíveis
    for (int shift = 0; shift < 26; shift++)
    {
        std::vector<int> freq(26, 0);

        // aplica "descriptografia" com o shift
        for (char c : group)
        {
            int decrypted = (c - 'A' - shift + 26) % 26;
            freq[decrypted]++;
        }

        // calcula erro (chi-square simplificado)
        double score = 0.0;
        for (int i = 0; i < 26; i++)
        {
            double expected = portuguese_freq[i] * n / 100.0;
            if (expected > 0)
            {
                double diff = freq[i] - expected;
                score += (diff * diff) / expected;
            }
        }

        // guarda o melhor
        if (score < best_score)
        {
            best_score = score;
            best_shift = shift;
        }
    }

    return 'A' + best_shift;
}

// Descobre chave inteira
std::string find_key(const std::string &text, int key_size)
{
    auto groups = split_groups(text, key_size);
    std::string key;

    for (auto &g : groups)
        key += find_key_letter(g);

    return key;
}

int main(int argc, char *argv[])
{

    try
    {
        if (argc != 3 && argc != 4)
        {
            std::cout << "Usage:\n";
            std::cout << argv[0] << " <input_file_path> <encrypt|decrypt> <key>\n";
            std::cout << argv[0] << " <input_file_path> attack\n";
            return 1;
        }

        std::string input_text;
        std::string result_text;
        std::string file_path = argv[1];
        std::string operation = argv[2];
        std::string key;
        if (operation != "attack")
        {
            key = argv[3];
        }
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
        else if (operation == "attack")
        {
            std::string cleaned = clean_text(input_text);

            std::cout << "=== ATTACK MODE ===" << std::endl;

            for (int size = 2; size <= 10; size++)
            {
                std::string key = find_key(cleaned, size);

                std::string decrypted;
                reverse_vigenere(input_text, key, decrypted);

                std::cout << "\nKey size " << size << ": " << key << std::endl;
                std::cout << "Decrypted: " << decrypted.substr(0, 100) << "..." << std::endl;
            }

            return 0;
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
