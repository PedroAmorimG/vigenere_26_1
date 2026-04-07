#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <iterator>
#include <stdint.h>
#include <cctype>

double portuguese_freq[95] = {
    16, 0.00005, 0.15, 0.0000005,  0.00005, 0.0000005, 0.0005, 0.15, 0.033, 0.033,
    0.00004, 0.00015, 0.86, 0.2, 1.1, 0.05, 0.6, 0.5, 0.27, 0.21,
    0.18, 0.25 , 0.19, 0.18, 0.25, 0.24, 0.01, 0.0007, 0.04, 0.00002,
    0.04, 0.0004, 0.000006, 0.24, 0.17, 0.3, 0.1, 0.1, 0.1, 0.1,
    0.08, 0.2, 0.08, 0.03, 0.1, 0.1, 0.1, 0.08, 0.02, 0.01, 
    0.1, 0.3, 0.3, 0.1, 0.02, 0.08, 0.006, 0.025, 0.0086, 0.00009,
    0.0000005, 0.00006, 0.00022, 0.000012, 0.0000005, 14.63, 1.04, 3.88, 4.99, 12.57, 
    1.02, 1.30, 0.78, 6.18, 0.40, 0.02, 2.78, 4.74, 5.05, 10.73, 
    2.52, 1.20, 6.53, 7.81, 4.34, 4.63, 1.67, 0.01, 0.21, 0.01,
    0.47, 0.000006, 0.00005, 0.00005, 0.00002
};
    
double english_freq[95] = {      
    20,  0.00005, 0.15, 0.0000005,  0.00005, 0.0000005, 0.0005, 0.15, 0.033, 0.033,
    0.00004, 0.00015, 0.86, 0.2, 1.1, 0.05, 0.6, 0.5, 0.27, 0.21,
    0.18, 0.25 , 0.19, 0.18, 0.25, 0.24, 0.01, 0.0007, 0.04, 0.00002,
    0.04, 0.0004, 0.000006, 0.24, 0.17, 0.3, 0.1, 0.1, 0.1, 0.1,
    0.08, 0.2, 0.08, 0.03, 0.1, 0.1, 0.1, 0.08, 0.02, 0.01, 
    0.1, 0.3, 0.3, 0.1, 0.02, 0.08, 0.006, 0.025, 0.0086, 0.00009,
    0.0000005, 0.00006, 0.00022, 0.000012, 0.0000005,
    8.17, 1.49, 2.78, 4.25, 12.70, 2.23, 2.02,
    6.09, 6.97, 0.15, 0.77, 4.03, 2.41,
    6.75, 7.51, 1.93, 0.10, 5.99,
    6.33, 9.06, 2.76, 0.98, 2.36, 0.15,
    1.97, 0.07, 0.000006, 0.00005, 0.00005, 0.00002
};

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
        if (c >= 0x00 && c <= 0x7F)
        {
            shift_vector.push_back(c - ' ');
        }
        else
        {
            throw std::runtime_error("Esperava-se apenas letras na chave.");
        }
    }
}

void convert_plain_text_to_int_array(std::string plain_text, std::vector<uint8_t> &plain_text_vector)
{
    for (auto c : plain_text)
    {
        if (c >= ' ' && c <= '~')
        {
            plain_text_vector.push_back(c - ' ');
        }
        
        /*else if (c == ' ')
        {
            plain_text_vector.push_back(c);
        }*/
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
        /*if (c == ' ')
        {
            cipher_text.push_back(c);
            continue;
        }*/

        c += shift_vector.at(key_position);

        cipher_text.push_back(((c % 95) + ' '));
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
        /*if (c == ' ')
        {
            plain_text.push_back(c);
            continue;
        }*/

        uint8_t shift = shift_vector.at(key_position);
        plain_text.push_back((c + 95 - shift) % 95 + ' ');
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
        freq[c - 'a']++;

    return freq;
}

// Descobre letra da chave
char find_key_letter(const std::string &group, double freq_table[95])
{
    int n = group.size();
    double best_score = 1e9;
    int best_shift = 0;

    // testa todos os shifts possíveis
    for (int shift = 0; shift < 95; shift++)
    {
        std::vector<int> freq(95, 0);

        // aplica "descriptografia" com o shift
        for (char c : group)
        {
            int decrypted = (c - ' ' - shift + 95) % 95;
            freq[decrypted]++;
        }

        // calcula erro (chi-square simplificado)
        double score = 0.0;
        for (int i = 0; i < 95; i++)
        {
            double expected = freq_table[i] * n / 100.0;
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

    return best_shift + ' ';
}

// Descobre chave inteira
std::string find_key(const std::string &text, int key_size, double freq_table[95])
{
    auto groups = split_groups(text, key_size);
    std::string key;

    for (auto &g : groups)
    {
        key += find_key_letter(g, freq_table);
    }

    return key;
}

std::vector<int> find_key_size(const std::string &text, double constante)
{
    double best_score = 1e9;
    double best_score2 = 1e9;
    double best_score3 = 1e9;
    std::vector<int> keys = {2, 2, 2};

    for (int size = 2; size <= 10; size++) {
        auto groups = split_groups(text, size);
        double score = 0.0;
        int valid_groups = 0;

        for (auto &g : groups) {
            if (g.size() < 2) continue;
            std::vector<int> freq(95, 0);

            // computa frequências de cada letra
            for (char c : g) {
                int index = (c - ' ') % 95;
                freq[index]++;
            }

            double ic = 0.0;
            int N = g.size();

            for (int i = 0; i < 95; i++) {
                ic += freq[i] * (freq[i] - 1);
            }

            ic /= (double)(N * (N - 1));
            score += ic;
            valid_groups++;
        }
        if (valid_groups > 0) {score = score / valid_groups;}
        
        double diff = std::abs(score - constante);

        // guarda os 3 melhores
        if (diff < best_score) {
            best_score3 = best_score2;
            best_score2 = best_score;
            best_score = diff;

            keys[2] = keys[1];
            keys[1] = keys[0];
            keys[0] = size;

        } else if (diff < best_score2) {
            best_score3 = best_score2;
            best_score2 = diff;

            keys[2] = keys[1];
            keys[1] = size;

        } else if (diff < best_score3) {
            best_score3 = diff;
            keys[2] = size;
        }

    }

    return keys;
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
            throw std::runtime_error("Não foi possível abrir o arquivo.");
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
            //std::string cleaned = clean_text(input_text);

            std::cout << "=== ATTACK MODE ===" << std::endl;

            std::vector<int> size_pt = find_key_size(input_text, 0.07813849);
            std::vector<int> size_en = find_key_size(input_text, 0.06549669);

            for (int size : size_pt) {
                std::string key_pt = find_key(input_text, size, portuguese_freq);
                std::string dec_pt;
                reverse_vigenere(input_text, key_pt, dec_pt);

                std::cout << "Key size " << size << " (PT): " << key_pt << std::endl;
                std::cout << "Decrypted PT: " << dec_pt.substr(0, 150) << "...\n";
            }

            std::cout << std::endl;

            for (int size : size_en) {
                std::string key_en = find_key(input_text, size, english_freq);
                std::string dec_en;
                reverse_vigenere(input_text, key_en, dec_en);

                std::cout << "Key size " << size << " (EN): " << key_en << std::endl;
                std::cout << "Decrypted EN: " << dec_en.substr(0, 150) << "...\n";
            }

            return 0;
        }         
        else
        {
            throw std::runtime_error("A operação esperada é criptografar ou descriptografar.");
        }
        output_file_path = build_output_file_path(file_path, output_suffix);
        output_file.open(output_file_path);
        if (!output_file.is_open())
        {
            throw std::runtime_error("Não foi possível criar o arquivo de saida.");
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
